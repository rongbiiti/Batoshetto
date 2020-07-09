#include "Player.h"
#include "GameManager.h"
#include "GameMain.h"
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
	

	// キーボードでの角度変更
	if (inputManager->In_Key()[KEY_INPUT_UP] != 0) {
		angle += 2;
	}
	if (inputManager->In_Key()[KEY_INPUT_DOWN] != 0) {
		angle -= 2;
	}

	AngleCorrection(angle);

	targetx = cosf(angle * DX_PI_F / 180) * 10000 + x;	// 狙っている方向のX座標
	targety = sinf(angle * DX_PI_F / 180) * 10000 + y;	// 狙っている方向のY座標
	
	int blocknum = 0;
	// なにかしら有効な値が入っていたら、ターゲット位置をその座標にする
	if (TrajectoryPrecalculation_ToBlock(&blocknum)) {
		CalcHitAfterAngle_ToBlock(blocknum);
	}
	else {	// 有効な値がなかったとき、つまりブロックと当たらず画面端を狙っている場合の処理。
		blocknum = TrajectoryPrecalculation_ToWindow();
		CalcHitAfterAngle_ToWindow(blocknum);
	}

	//TargetPointWindowHitCheck();

	// 発射ボタンを押すと、弾オブジェクトの初期化関数に値を入れて、フェーズを進める。
	if (inputManager->GetPadInput()[gameMain->gameManager->GetNowShooter()].in_Button[InputManager::A] == 1 || inputManager->In_Key()[KEY_INPUT_F] == 1) {
		float rx = cosf(angle * DX_PI_F / 180) + x;		// X進行方向
		float ry = sinf(angle * DX_PI_F / 180) + y;		// Y進行方向

		// 弾の初期化。生存フラグをtrue、X進行方向、Y進行方向、角度、GameMainオブジェクトのポインタを渡す
		gameMain->bullet->BulletInit(true, rx, ry, (float)angle, gameMain);
		gameMain->gameManager->SetPhaseStatus(GameManager::RECOCHETWAIT);	// フェーズを進める
	}
}

// 隠れる側時の操作処理
void Player::HidingPlayerControll(void) {
	int hider = gameMain->gameManager->GetNowHider();

	// 移動前の座標を記憶しておく
	preX = x;
	preY = y;
	
	// 移動処理
	if (inputManager->GetPadInput()[hider].in_Stick_LY >= 0.45f) {
		// 左スティックが上に傾けられていたら上に移動する
		y -= moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LY <= -0.45f) {
		// 左スティックが下に傾けられていたら下に移動する
		y += moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LX <= -0.45f) {
		// 左スティックが左に傾けられていたら左に移動する
		x -= moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LX >= 0.45f) {
		// 左スティックが右に傾けられていたら右に移動する
		x += moveSpeed;
	}

	x = XCoordinateCorrection(x, num, size);
	y = YCoordinateCorrection(y, size);

	// ブロックたちと当たり判定する。
	BlockHitCheck();
}

// 描画用
void Player::DrawPlayer(void) {
	DrawCircle(x, y, size / 2, color);
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
				y = preY;
				x = preX;
			}
			// そして処理を抜ける
			return;
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

	float prex = targetx - cosf(angle * DX_PI_F / 180) * 5;	// 狙っている方向のX座標
	float prey = targety - sinf(angle * DX_PI_F / 180) * 5;	// 狙っている方向のY座標

	// ターゲットの移動前X座標が幅の中にいたら、Y座標のみを戻して、X座標は変化させる
	if (collision->IsHitWicth(prex, blockX, blockSize)) {
		// 移動前座標が幅の中なら、向きの上下を変える
		angle2 = (360 - angle);
	}
	// ターゲットの移動前Y座標が高さの中にいたら、X座標のみを戻して、Y座標は変化させる
	else if (collision->IsHitHeight(prey, blockY, blockSize)) {
		// 高さの中なら、向きの左右を変える
		angle2 = (360 - angle) + 180;
		if (angle2 > 360) angle2 -= 360;
	}
	// ブロックの角っこだったら、XYどちらも戻す。
	else {
		// 角なら、真逆の向きに
		angle2 = angle + 180;
		if (angle2 > 360) angle2 -= 360;
	}

	float rad = (angle2 / 360) * DX_PI_F * 2;	// ラジアンに変換する
	targetx2 = cosf(rad) * 300 + targetx;	// 狙っている方向のX座標
	targety2 = sinf(rad) * 300 + targety;	// 狙っている方向のY座標
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
	float prex = targetx - cosf(angle * DX_PI_F / 180) * 5;	// 狙っている方向のX座標
	float prey = targety - sinf(angle * DX_PI_F / 180) * 5;	// 狙っている方向のY座標
	// ターゲットの移動前X座標が幅の中にいたら、Y座標のみを戻して、X座標は変化させる
	if (num % 2 == 0) {
		// 移動前座標が幅の中なら、向きの上下を変える
		angle2 = (360 - angle);
	}
	// ターゲットの移動前Y座標が高さの中にいたら、X座標のみを戻して、Y座標は変化させる
	else {
		// 高さの中なら、向きの左右を変える
		angle2 = (360 - angle) + 180;
		if (angle2 > 360) angle2 -= 360;
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
	if (360 < ang) {
		return 0;
	}
	else if (ang < 0) {
		return 360;
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

Player::~Player() {
	
}