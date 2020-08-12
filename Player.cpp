#include "Player.h"
#include "DxLib.h"
#include <math.h>

// コンストラクタ。REDかBLUEか、色、撃つ側か否か、GameMainオブジェクトのポインタを引数で受け取る。
Player::Player(int num, unsigned int color, bool shooter, GameMain* main) {
	x = PlayerStartPositionX[num];		// X中心座標初期化
	y = PlayerStartPositionY[num];		// Y中心座標初期化
	preX = x;							// 移動前X座標初期化
	preY = y;							// 移動前Y座標初期化
	size = PlayerSize;					// 直径サイズ初期化
	moveSpeed = PlayerMoveSpeed;		// 移動速度初期化
	
	if (num == GameManager::RED) {
		// 受け取った番号がREDだったら、角度の初期値を右になるようにする
		angle = 0;
	} else if (num == GameManager::BLUE) {
		// BLUEだったら、初期値は左
		angle = 180;
	}
	else {
		angle = 0;
	}
	targetx = cosf(angle * DX_PI_F / 180) * 10000 + x;	// 狙っているX位置を初期化
	targety = sinf(angle * DX_PI_F / 180) * 10000 + y;	// 狙っているY位置を初期化
	targetx2 = 0, targety2 = 0, angle2 = 0;
	
	this->color = color;	// 色初期化
	isShooter = shooter;	// 最初撃つ側か否かの引数を入れる
	this->num = num;		// REDかBLUEかを入れる。

	gameMain = main;		// GameMainオブジェクトのポインタを入れる
	inputManager = main->inputManager;	// 入力管理オブジェクトのポインタを入れる。

	collision = new Collision;	// 衝突判定管理オブジェクトを生成する
	effect = new Effect;	// エフェクトオブジェクトの生成

	LoadImages();	//画像読み込み

	shotFlg = FALSE;
	passFlg = FALSE;

	net = gameMain->network;
}

// 撃つ側時の操作処理
void Player::ShooterPlayerControll(void) {	
	int shooter = gameMain->gameManager->GetNowShooter();
	// 角度変更
	// コントローラーのスティック
	if (abs(inputManager->GetPadInput()[shooter].in_Stick_LX) + abs(inputManager->GetPadInput()[shooter].in_Stick_LY) >= 0.97f) {
		// X軸とY軸の倒した量の合計が規定の値以下なら角度を固定させる
		float rad = atan2(inputManager->GetPadInput()[shooter].in_Stick_LY, inputManager->GetPadInput()[shooter].in_Stick_LX);
		angle = -rad * 180.0f / DX_PI_F;
		if (angle < 0) angle += 360;
	}

	// 方向キーを押した瞬間の角度を参照し、角度増分量をプラスかマイナスか判断する
	if (inputManager->GetButtonDown(PAD_UP, shooter) || inputManager->GetButtonDown(PAD_DOWN, shooter)) {
		ChangeDirectionalKeyAng();
	}
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyDown(KEY_INPUT_DOWN)) {
		ChangeDirectionalKeyAng();
	}

	// コントローラーの十字キーでも角度操作を受付。
	// Aボタンを押していると、速度がアップする。
	if (inputManager->GetPadInput()[shooter].in_Button[PAD_UP] != 0) {
		angle += directionalKeyAng;
		if (inputManager->GetPadInput()[shooter].in_Button[A] != 0) {
			angle += directionalKeyAng * 2;
		}
	}
	if (inputManager->GetPadInput()[shooter].in_Button[PAD_DOWN] != 0) {
		angle -= directionalKeyAng;
		if (inputManager->GetPadInput()[shooter].in_Button[A] != 0) {
			angle -= directionalKeyAng * 2;
		}
	}

	// キーボードでの角度変更
	if (inputManager->In_Key()[KEY_INPUT_UP] != 0) {
		angle += directionalKeyAng;
		if (inputManager->In_Key()[KEY_INPUT_LSHIFT] != 0) {
			angle += directionalKeyAng * 2;
		}
	}
	if (inputManager->In_Key()[KEY_INPUT_DOWN] != 0) {
		angle -= directionalKeyAng;
		if (inputManager->In_Key()[KEY_INPUT_LSHIFT] != 0) {
			angle -= directionalKeyAng * 2;
		}
	}

	angle = AngleCorrection(angle);

	targetx = cosf(angle * DX_PI_F / 180.0f) * 10000 + x;	// 狙っている方向のX座標
	targety = sinf(angle * DX_PI_F / 180.0f) * 10000 + y;	// 狙っている方向のY座標
	
	int blocknum = 0;
	// なにかしら有効な値が入っていたら、ターゲット位置をその座標にする
	if (TrajectoryPrecalculation_ToBlock(&blocknum)) {
		CalcHitAfterAngle_ToBlock(blocknum);
		blocknumber = blocknum;
	}
	else {	// 有効な値がなかったとき、つまりブロックと当たらず画面端を狙っている場合の処理。
		blocknum = TrajectoryPrecalculation_ToWindow();
		CalcHitAfterAngle_ToWindow(blocknum);
	}

	//TargetPointWindowHitCheck();

	// PASSして隠れる側フェーズに
	if (inputManager->GetPadInput()[shooter].in_Button[X] == 1 || inputManager->In_Key()[KEY_INPUT_SPACE] == 1) {
		gameMain->gameManager->ToHidePhase();
		return;
	}

	// 発射ボタンを押すと、弾オブジェクトの初期化関数に値を入れて、フェーズを進める。
	// または、制限時間になったら勝手に発射する
	if (inputManager->GetPadInput()[shooter].in_Button[B] == 1 || inputManager->In_Key()[KEY_INPUT_F] == 1 || gameMain->gameManager->GetShotTime() < 1) {
		// 弾の初期化。生存フラグをtrue、X進行方向、Y進行方向、角度、GameMainオブジェクトのポインタを渡す
		CreateBullet();
		effect->InitEffectCount();	// エフェクトのフレームカウント初期化
		effect->MuzzleFlashEffectFlg = TRUE;	// マズルフラッシュエフェクトをTRUEに
		gameMain->gameManager->SetPhaseStatus(GameManager::RECOCHETWAIT);	// フェーズを進める
		return;
	}
}

// 撃つ側の操作処理・通信対戦
void Player::ShooterPlayerControll_Net() {
	int shooter = gameMain->gameManager->GetNowShooter();
	if (shooter == net->GetConnectType()) {

		// 受信確認待ち中の処理
		if (net->GetIsWaitRecvCheck()) {
			// 相手の受信確認を待つ
			net->PostRecvCheck();

			// 受信確認ができたら、フェーズをすすめる
			if (net->GetRecvCheck()) {
				if (shotFlg) {
					CreateBullet();
					effect->InitEffectCount();	// エフェクトのフレームカウント初期化
					effect->MuzzleFlashEffectFlg = TRUE;	// マズルフラッシュエフェクトの添え字を入れる
					gameMain->gameManager->SetPhaseStatus(GameManager::RECOCHETWAIT);	// フェーズを進める
					shotFlg = FALSE;
				}
				else if (passFlg) {
					gameMain->gameManager->ToHidePhase();
					passFlg = FALSE;
				}
				
				net->SetIsWaitRecvCheck(FALSE);
			}
			// 0.5秒ごとに再送
			else if(gameMain->gameManager->GetShotTime() % 30 == 0){
				if(shotFlg) net->SendShooterInfo(angle, TRUE, FALSE);
				else if(passFlg) net->SendShooterInfo(angle, FALSE, TRUE);
				
			}
			return;
		}

		// 角度変更
	// コントローラーのスティック
		if (abs(inputManager->GetPadInput()[0].in_Stick_LX) + abs(inputManager->GetPadInput()[0].in_Stick_LY) >= 0.97f) {
			// X軸とY軸の倒した量の合計が規定の値以下なら角度を固定させる
			float rad = atan2(inputManager->GetPadInput()[0].in_Stick_LY, inputManager->GetPadInput()[0].in_Stick_LX);
			angle = -rad * 180.0f / DX_PI_F;
			if (angle < 0) angle += 360;
		}

		// 方向キーを押した瞬間の角度を参照し、角度増分量をプラスかマイナスか判断する
		if (inputManager->GetButtonDown(PAD_UP, 0) || inputManager->GetButtonDown(PAD_DOWN, 0)) {
			ChangeDirectionalKeyAng();
		}
		if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyDown(KEY_INPUT_DOWN)) {
			ChangeDirectionalKeyAng();
		}

		// コントローラーの十字キーでも角度操作を受付。
		// Aボタンを押していると、速度がアップする。
		if (inputManager->GetPadInput()[0].in_Button[PAD_UP] != 0) {
			angle += directionalKeyAng;
			if (inputManager->GetPadInput()[0].in_Button[A] != 0) {
				angle += directionalKeyAng * 2;
			}
		}
		if (inputManager->GetPadInput()[0].in_Button[PAD_DOWN] != 0) {
			angle -= directionalKeyAng;
			if (inputManager->GetPadInput()[0].in_Button[A] != 0) {
				angle -= directionalKeyAng * 2;
			}
		}

		// キーボードでの角度変更
		if (inputManager->In_Key()[KEY_INPUT_UP] != 0) {
			angle += directionalKeyAng;
			if (inputManager->In_Key()[KEY_INPUT_LSHIFT] != 0) {
				angle += directionalKeyAng * 2;
			}
		}
		if (inputManager->In_Key()[KEY_INPUT_DOWN] != 0) {
			angle -= directionalKeyAng;
			if (inputManager->In_Key()[KEY_INPUT_LSHIFT] != 0) {
				angle -= directionalKeyAng * 2;
			}
		}

		angle = AngleCorrection(angle);

		targetx = cosf(angle * DX_PI_F / 180.0f) * 10000 + x;	// 狙っている方向のX座標
		targety = sinf(angle * DX_PI_F / 180.0f) * 10000 + y;	// 狙っている方向のY座標

		int blocknum = 0;
		// なにかしら有効な値が入っていたら、ターゲット位置をその座標にする
		if (TrajectoryPrecalculation_ToBlock(&blocknum)) {
			CalcHitAfterAngle_ToBlock(blocknum);
			blocknumber = blocknum;
		}
		else {	// 有効な値がなかったとき、つまりブロックと当たらず画面端を狙っている場合の処理。
			blocknum = TrajectoryPrecalculation_ToWindow();
			CalcHitAfterAngle_ToWindow(blocknum);
		}

		//TargetPointWindowHitCheck();

		// PASSして隠れる側フェーズに
		if (inputManager->GetPadInput()[0].in_Button[X] == 1 || inputManager->In_Key()[KEY_INPUT_SPACE] == 1) {
			net->SendShooterInfo(angle, FALSE, TRUE);
			passFlg = TRUE;
			shotFlg = FALSE;
			return;
		}

		// 発射ボタンを押すと、弾オブジェクトの初期化関数に値を入れて、フェーズを進める。
		// または、制限時間になったら勝手に発射する
		if (inputManager->GetPadInput()[0].in_Button[B] == 1 || inputManager->In_Key()[KEY_INPUT_F] == 1 || gameMain->gameManager->GetShotTime() <= 1) {
			net->SendShooterInfo(angle, TRUE, FALSE);
			shotFlg = TRUE;
			passFlg = FALSE;
			return;
		}

		net->SendShooterInfo(angle, FALSE, FALSE);
		
	}
	else {
		net->PostShooterInfo();
		Network::ShooterInfo shooterInfo = net->GetShooterInfo();
		angle = shooterInfo.angle;
		targetx = cosf(angle * DX_PI_F / 180.0f) * 10000 + x;	// 狙っている方向のX座標
		targety = sinf(angle * DX_PI_F / 180.0f) * 10000 + y;	// 狙っている方向のY座標
		int blocknum = 0;
		// なにかしら有効な値が入っていたら、ターゲット位置をその座標にする
		if (TrajectoryPrecalculation_ToBlock(&blocknum)) {
			CalcHitAfterAngle_ToBlock(blocknum);
			blocknumber = blocknum;
		}
		else {	// 有効な値がなかったとき、つまりブロックと当たらず画面端を狙っている場合の処理。
			blocknum = TrajectoryPrecalculation_ToWindow();
			CalcHitAfterAngle_ToWindow(blocknum);
		}
		if (shooterInfo.passFlg) {
			gameMain->gameManager->ToHidePhase();
		}
		else if (shooterInfo.shotFlg) {
			CreateBullet();
			effect->InitEffectCount();	// エフェクトのフレームカウント初期化
			effect->MuzzleFlashEffectFlg = TRUE;	// マズルフラッシュエフェクトの添え字を入れる
			gameMain->gameManager->SetPhaseStatus(GameManager::RECOCHETWAIT);	// フェーズを進める
		}

		// 受信したことの応答が必要な場合の処理
		if (shooterInfo.isRecvCheck) {
			net->SendRecvCheck();
			net->SetIsWaitRecvCheck(FALSE);
		}

	}
}

// 隠れる側時の操作処理
void Player::HidingPlayerControll(void) {
	int hider = gameMain->gameManager->GetNowHider();

	// 移動前の座標を記憶しておく
	preX = x;
	preY = y;
	
	// 移動処理
	if (inputManager->GetPadInput()[hider].in_Stick_LY >= 0.45f || inputManager->In_Key()[KEY_INPUT_UP] != 0) {
		// 左スティックが上に傾けられていたら上に移動する
		y -= moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LY <= -0.45f || inputManager->In_Key()[KEY_INPUT_DOWN] != 0) {
		// 左スティックが下に傾けられていたら下に移動する
		y += moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LX <= -0.45f || inputManager->In_Key()[KEY_INPUT_LEFT] != 0) {
		// 左スティックが左に傾けられていたら左に移動する
		x -= moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LX >= 0.45f || inputManager->In_Key()[KEY_INPUT_RIGHT] != 0) {
		// 左スティックが右に傾けられていたら右に移動する
		x += moveSpeed;
	}

	x = XCoordinateCorrection(x, num, size);
	y = YCoordinateCorrection(y, size);

	// ブロックたちと当たり判定する。
	BlockHitCheck();

	// PASSして撃つ側フェーズに
	if (inputManager->GetPadInput()[hider].in_Button[X] == 1 || inputManager->In_Key()[KEY_INPUT_SPACE] == 1) {
		gameMain->gameManager->ToShotPhase();

	}
}

// 隠れる側の操作処理・通信対戦
void Player::HidingPlayerControll_Net() {
	int hider = gameMain->gameManager->GetNowHider();
	if (hider == net->GetConnectType()) {
		// 受信確認待ち中の処理
		if (net->GetIsWaitRecvCheck()) {
			// 相手の受信確認を待つ
			net->PostRecvCheck();

			// 受信確認ができたら、フェーズをすすめる
			if (net->GetRecvCheck()) {
				gameMain->gameManager->ToShotPhase();
				net->SetIsWaitRecvCheck(FALSE);
			}
			// 0.5秒ごとに再送
			else if (gameMain->gameManager->GetHideTime() % 30 == 0) {
				net->SendHiderInfo(x, y, TRUE);
			}
			return;
		}

		// 受信したことの応答が必要な場合の処理
		/*if (shooterInfo.isRecvCheck) {

		}*/
		// 移動前の座標を記憶しておく
		preX = x;
		preY = y;

		// 移動処理
		if (inputManager->GetPadInput()[0].in_Stick_LY >= 0.45f || inputManager->In_Key()[KEY_INPUT_UP] != 0) {
			// 左スティックが上に傾けられていたら上に移動する
			y -= moveSpeed;
		}
		if (inputManager->GetPadInput()[0].in_Stick_LY <= -0.45f || inputManager->In_Key()[KEY_INPUT_DOWN] != 0) {
			// 左スティックが下に傾けられていたら下に移動する
			y += moveSpeed;
		}
		if (inputManager->GetPadInput()[0].in_Stick_LX <= -0.45f || inputManager->In_Key()[KEY_INPUT_LEFT] != 0) {
			// 左スティックが左に傾けられていたら左に移動する
			x -= moveSpeed;
		}
		if (inputManager->GetPadInput()[0].in_Stick_LX >= 0.45f || inputManager->In_Key()[KEY_INPUT_RIGHT] != 0) {
			// 左スティックが右に傾けられていたら右に移動する
			x += moveSpeed;
		}

		x = XCoordinateCorrection(x, num, size);
		y = YCoordinateCorrection(y, size);

		// ブロックたちと当たり判定する。
		BlockHitCheck();

		// PASSして撃つ側フェーズに
		if (inputManager->GetPadInput()[0].in_Button[X] == 1 || inputManager->In_Key()[KEY_INPUT_SPACE] == 1 || gameMain->gameManager->GetHideTime() <= 1) {
			net->SendHiderInfo(x, y, TRUE);
			return;
		}
		net->SendHiderInfo(x, y, FALSE);
	}
	else {

		net->PostHiderInfo();
		Network::HiderInfo hiderInfo = net->GetHiderInfo();
		x = hiderInfo.x;
		y = hiderInfo.y;
		if (hiderInfo.passFlg) {
			gameMain->gameManager->ToShotPhase();
			
		}

		// 受信したことの応答が必要な場合の処理
		if (hiderInfo.isRecvCheck) {
			net->SendRecvCheck();
			net->SetIsWaitRecvCheck(FALSE);
		}
	}
}

// 描画用
void Player::DrawPlayer(void) {
	DrawRotaGraph(x, y, 1.0f, angle * DX_PI_F / 180.0f,i_Playerimage[isShooter], TRUE);

	if(effect->MuzzleFlashEffectFlg == TRUE){		//TRUE時にエフェクト関数に移行
		effect->DrawEffect(x, y,angle);		// エフェクト描画
	}
}

// 撃つ側時に狙っている方向に線を引いて描画する
void Player::DrawTargetAngle(void) {
	DrawLine(x, y, (int)targetx, (int)targety, color, 3);
	DrawLine((int)targetx, (int)targety, (int)targetx2, (int)targety2, color, 3);
	DrawCircle((int)targetx, (int)targety, 4, 0xFFFFFF);
}

// ブロックと当たり判定
void Player::BlockHitCheck(void) {
	// ブロックのX、Y、直径サイズのローカル変数
	int blockX, blockY, blockSize;

	// ブロックたちと当たり判定。GameMainで定義されているブロックの最大数に達するまで繰り返す。
	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		// もしブロックの生存フラグが折れていたらそのブロックとの関係は終わり、次のブロックに会いに行く。
		if (!gameMain->block[i]->IsAlive()) continue;

		// ローカル変数に、ブロックの座標とサイズを入れていく
		blockX = gameMain->block[i]->GetBlockX();
		blockY = gameMain->block[i]->GetBlockY();
		blockSize = gameMain->block[i]->GetBlockSize();

		// コンストラクタで生成した衝突判定をするオブジェクトに引数を渡して、判定してもらう。
		if (collision->IsHit(x, y, size, blockX, blockY, blockSize)) {

			// プレイヤーの移動前X座標が幅の中にいたら、Y座標のみを戻して、X座標は変化させる
			if (collision->IsHitWicth(preX, blockX, blockSize)) {
				y = preY;
			}
			// プレイヤーの移動前Y座標が高さの中にいたら、X座標のみを戻して、Y座標は変化させる
			else if (collision->IsHitHeight(preY, blockY, blockSize)) {
				x = preX;
			}
			// ブロックの角っこだったら、XYどちらも戻す。
			else {
				// 角に当たった場合、左右のブロックの幅の中にいないかもう一度確かめる
				if ((collision->IsHitWicth(preX, gameMain->block[i - 1]->GetBlockX(), gameMain->block[i - 1]->GetBlockSize()) && gameMain->block[i - 1]->IsAlive()) ||
					(collision->IsHitWicth(preX, gameMain->block[i + 1]->GetBlockX(), gameMain->block[i + 1]->GetBlockSize()) && gameMain->block[i + 1]->IsAlive())) {
					y = preY;
				}
				// 上下のブロックの高さの中にいないかもう一度確かめる
				else if ((collision->IsHitHeight(preY, gameMain->block[i - 3]->GetBlockY(), gameMain->block[i - 3]->GetBlockSize()) && gameMain->block[i - 3]->IsAlive()) ||
						 (collision->IsHitHeight(preY, gameMain->block[i + 3]->GetBlockY(), gameMain->block[i + 3]->GetBlockSize()) && gameMain->block[i + 3]->IsAlive())) {
					x = preX;
				}
				else {
					y = preY;
					x = preX;
				}			
			}
		}
	}
}

// 弾が進む方向がウィンドウを飛び出してないかチェックする
void Player::TargetPointWindowHitCheck(void) {
	// Y座標が画面外に行っていたら戻す
	if (GameMain::SCREEN_HEIGHT < targety) {
		targety = GameMain::SCREEN_HEIGHT;
	}
	else if (targety < 0) {
		targety = 0;
	}

	// X座標が定められた位置を超えていたら戻す
	// 中心線仕様を実装するかもしれないので、REDとBLUEで一応分けている。
	if (GameMain::SCREEN_WIDTH < targetx) {
		targetx = GameMain::SCREEN_WIDTH;
	}
	else if (targetx < 0) {
		targetx = 0;
	}
}

// ブロックのどの辺と衝突しているか判断して角度を変更する
void Player::CalcHitAfterAngle_ToBlock(int blocknum) {
	// ブロックのX、Y、直径サイズのローカル変数
	int blockX, blockY, blockSize;	

	// ローカル変数に、ブロックの座標とサイズを入れていく
	blockX = gameMain->block[blocknum]->GetBlockX();
	blockY = gameMain->block[blocknum]->GetBlockY();
	blockSize = gameMain->block[blocknum]->GetBlockSize();

	float prex = targetx - cosf(angle * DX_PI_F / 180.0f) * gameMain->bullet->GetBulletSPD_X();	// 狙っている方向のX座標
	float prey = targety - sinf(angle * DX_PI_F / 180.0f) * gameMain->bullet->GetBulletSPD_Y();	// 狙っている方向のY座標

	// ターゲットの移動前X座標が幅の中にいたら、Y座標のみを戻して、X座標は変化させる
	if (collision->IsHitWicth((int)prex, blockX, blockSize)) {
		// 移動前座標が幅の中なら、向きの上下を変える
		angle2 = (360.0f - angle);
	}
	// ターゲットの移動前Y座標が高さの中にいたら、X座標のみを戻して、Y座標は変化させる
	else if (collision->IsHitHeight((int)prey, blockY, blockSize)) {
		// 高さの中なら、向きの左右を変える
		angle2 = (360.0f - angle) + 180.0f;
		if (angle2 > 360.0f) angle2 -= 360.0f;
	}
	// ブロックの角っこだったら、XYどちらも戻す。
	else {
		// 角に当たった場合、左右のブロックの幅の中にいないかもう一度確かめる
		if ((collision->IsHitWicth((int)prex, gameMain->block[blocknum - 1]->GetBlockX(), gameMain->block[blocknum - 1]->GetBlockSize()) && gameMain->block[blocknum - 1]->IsAlive()) ||
			(collision->IsHitWicth((int)prex, gameMain->block[blocknum + 1]->GetBlockX(), gameMain->block[blocknum + 1]->GetBlockSize()) && gameMain->block[blocknum + 1]->IsAlive())) {
			// 移動前座標が幅の中なら、向きの上下を変える
			angle2 = (360.0f - angle);
		}
		// 上下のブロックの高さの中にいないかもう一度確かめる
		else if ((collision->IsHitHeight((int)prey, gameMain->block[blocknum - 3]->GetBlockY(), gameMain->block[blocknum - 3]->GetBlockSize()) && gameMain->block[blocknum - 3]->IsAlive()) ||
				 (collision->IsHitHeight((int)prey, gameMain->block[blocknum + 3]->GetBlockY(), gameMain->block[blocknum + 3]->GetBlockSize()) && gameMain->block[blocknum + 3]->IsAlive())) {
			// 高さの中なら、向きの左右を変える
			angle2 = (360.0f - angle) + 180.0f;
			if (angle2 > 360.0f) angle2 -= 360.0f;
		}
		else {
			// 角なら、真逆の向きに
			angle2 = angle + 180.0f;
			if (angle2 > 360.0f) angle2 -= 360.0f;
		}
		
	}

	float rad = (angle2 / 360.0f) * DX_PI_F * 2;	// ラジアンに変換する
	targetx2 = cosf(rad) * 300.0f + targetx;	// 狙っている方向のX座標
	targety2 = sinf(rad) * 300.0f + targety;	// 狙っている方向のY座標
}

// 弾が進む方向がウィンドウを飛び出してないかチェックする
bool Player::TrajectoryPrecalculation_ToBlock(int* blocknum) {
	// ブロックのX、Y、直径サイズのローカル変数
	int blockX, blockY, blockSize;

	Collision::Vector2 cross[GameMain::BLOCK_MAX];
	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		cross[i].x = -10000;		// 間違ってもプレイヤーと一番近い点とならないように大きい値にしておく
		cross[i].y = -10000;
		cross[i].flg = false;
	}

	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		// もしブロックの生存フラグが折れていたらそのブロックとの関係は終わり、次のブロックに会いに行く。
		if (!gameMain->block[i]->IsAlive()) continue;

		// ローカル変数に、ブロックの座標とサイズを入れていく
		blockX = gameMain->block[i]->GetBlockX();
		blockY = gameMain->block[i]->GetBlockY();
		blockSize = gameMain->block[i]->GetBlockSize();

		// コンストラクタで生成した衝突判定をするオブジェクトに引数を渡して、判定してもらう。		
		cross[i] = collision->IsHitTargetAndBlock((float)x, (float)y, targetx, targety, (float)blockX, (float)blockY, (float)blockSize);
	}

	bool resurt = false;
	float distance1 = -10000;
	float distance2 = -10000;

	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {		// 最初の配列の値は入れているので2番目から開始
		if (!cross[i].flg) continue;					// 有効な値が入っていなかったらスキップ
		distance1 = hypot(targetx - x, targety - y);	// プレイヤーと衝突点までの距離
		distance2 = hypot(cross[i].x - x, cross[i].y - y);
		if (distance2 < distance1) {					// 比較したほうがプレイヤーと近かったら渡す用の構造体の値を、その値に更新
			targetx = cross[i].x;
			targety = cross[i].y;
			resurt = true;
			*blocknum = i;
		}
	}
	return resurt;
}

// ウィンドウのどの端と衝突しているか判断して角度を変更する
void Player::CalcHitAfterAngle_ToWindow(int num) {
	float prex = targetx - cosf(angle * DX_PI_F / 180.0f) * gameMain->bullet->GetBulletSPD_X();	// 狙っている方向のX座標
	float prey = targety - sinf(angle * DX_PI_F / 180.0f) * gameMain->bullet->GetBulletSPD_Y();	// 狙っている方向のY座標
	// ターゲットの移動前X座標が幅の中にいたら、Y座標のみを戻して、X座標は変化させる
	if (num % 2 == 0) {
		// 移動前座標が幅の中なら、向きの上下を変える
		angle2 = (360.0f - angle);
	}
	// ターゲットの移動前Y座標が高さの中にいたら、X座標のみを戻して、Y座標は変化させる
	else {
		// 高さの中なら、向きの左右を変える
		angle2 = (360.0f - angle) + 180.0f;
		if (angle2 > 360.0f) angle2 -= 360.0f;
	}
	float rad = (angle2 / 360) * DX_PI_F * 2;	// ラジアンに変換する
	targetx2 = cosf(rad) * 300 + targetx;	// 狙っている方向のX座標
	targety2 = sinf(rad) * 300 + targety;	// 狙っている方向のY座標
}

// ウィンドウに対する弾道の事前計算。
int Player::TrajectoryPrecalculation_ToWindow(void) {
	Collision::Vector2 cross[GameMain::BLOCK_MAX];
	float distance1 = -10000;
	float distance2 = -10000;

	for (int i = 0; i < 4; i++) {
		cross[i].x = -10000;		// 間違ってもプレイヤーと一番近い点とならないように大きい値にしておく
		cross[i].y = -10000;
		cross[i].flg = false;
	}

	float top = 0, bottom = GameMain::SCREEN_HEIGHT, left = 0, right = GameMain::SCREEN_WIDTH;

	cross[0] = collision->GetHitLineAndLine((float)x, (float)y, targetx, targety, left, top, right, top);
	cross[1] = collision->GetHitLineAndLine((float)x, (float)y, targetx, targety, right, top, right, bottom);
	cross[2] = collision->GetHitLineAndLine((float)x, (float)y, targetx, targety, right, bottom, left, bottom);
	cross[3] = collision->GetHitLineAndLine((float)x, (float)y, targetx, targety, left, bottom, left, top);

	// どの点が一番プレイヤーに近いかを比較する
	targetx = cross[0].x;	// 最初の配列の値を代入しておく
	targety = cross[0].y;
	int result = 0;

	for (int i = 1; i < 4; i++) {		// 最初の配列の値は入れているので2番目から開始
		if (!cross[i].flg) continue;					// 有効な値が入っていなかったらスキップ
		distance1 = hypot(targetx - x, targety - y);	// プレイヤーと衝突点までの距離
		distance2 = hypot(cross[i].x - x, cross[i].y - y);
		if (distance2 < distance1) {					// 比較したほうがプレイヤーと近かったら渡す用の構造体の値を、その値に更新
			targetx = cross[i].x;
			targety = cross[i].y;
			result = i;
		}
	}
	return result;	
}

// 角度を0～360度に収まるように調整
float Player::AngleCorrection(float ang) {
	// 角度が0～360の範囲になるようにしている。
	if (360.0f < ang) {
		return 0;
	}
	else if (ang < 0) {
		return 360.0f;
	}
	return ang;
}

// X座標が指定の範囲を超えていないか、プレイヤーの番号をもとに修正
int Player::XCoordinateCorrection(int posx, int pnum, int size) {
	// X座標が定められた位置を超えていたら戻す
	// 中心線仕様を実装するかもしれないので、REDとBLUEで一応分けている。
	int halfsize = size / 2;
	switch (pnum)
	{
	case GameManager::PLAYERCOLOR::RED:
		if (GameMain::SCREEN_WIDTH < posx + halfsize) {
			return GameMain::SCREEN_WIDTH - halfsize;
		}
		else if (posx - halfsize < 0) {
			return halfsize;
		}
		return posx;
		break;
	case GameManager::PLAYERCOLOR::BLUE:
		if (GameMain::SCREEN_WIDTH < posx + halfsize) {
			return GameMain::SCREEN_WIDTH - halfsize;
		}
		else if (posx - halfsize < 0) {
			return halfsize;
		}
		return posx;
		break;
	default:
		return posx;
		break;
	}
}

// Y座標が指定の範囲を超えないように修正
int Player::YCoordinateCorrection(int posy, int size) {
	// Y座標が画面外に行っていたら戻す
	int halfsize = size / 2;
	if (GameMain::SCREEN_HEIGHT < posy + halfsize) {
		return GameMain::SCREEN_HEIGHT - halfsize;
	}
	else if (posy - halfsize < 0) {
		return halfsize;
	}
	return posy;
}

void Player::ChangeDirectionalKeyAng(void) {
	if (270.0f < angle || angle < 90.0f) {
		directionalKeyAng = -1;
	}
	else if (90.0f <= angle && angle <= 270.0f) {
		directionalKeyAng = 1;
	}
}

// 弾の初期化。生存フラグをtrue、X進行方向、Y進行方向、角度、GameMainオブジェクトのポインタを渡す
void Player::CreateBullet(void) {
	float rx = cosf(angle * DX_PI_F / 180.0f) + x;		// X進行方向
	float ry = sinf(angle * DX_PI_F / 180.0f) + y;		// Y進行方向
	gameMain->bullet->BulletInit(true, rx, ry, (float)angle, gameMain);
}

// 画像読み込み
void Player::LoadImages() {
	if (!(i_Playerimage[0] = LoadGraph("Image/PlayerDefalut01.png"))) return;
	if (!(i_Playerimage[1] = LoadGraph("Image/PlayerShot.png"))) return;
}

// 画像データメモリから消去
void Player::DeleteImages() {
	DeleteGraph(i_Playerimage[0]);
	DeleteGraph(i_Playerimage[1]);
	i_Playerimage[0] = NULL;
	i_Playerimage[1] = NULL;
}

Player::~Player() {
	delete collision;
	delete effect;
	DeleteImages();
}