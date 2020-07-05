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
	targetx = cosf(angle * DX_PI_F / 180) * 300 + x;	// 狙っているX位置を初期化
	targety = sinf(angle * DX_PI_F / 180) * 300 + y;	// 狙っているY位置を初期化
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
	// 移動前の座標を記憶しておく
	preX = x;
	preY = y;
	
	// 角度変更
	float rad = atan2(inputManager->In_Stick_LY(), inputManager->In_Stick_LX());
	angle = -rad * 180.0f / DX_PI_F;
	if (angle < 0) angle += 360;
	

	// 角度が0～360の範囲になるようにしている。
	if (360 < angle) {
		angle = 0;
	}
	else if (angle < 0) {
		angle = 360;
	}

	// Y座標が画面外に行っていたら戻す
	if (GameMain::SCREEN_HEIGHT < y) {
		y = GameMain::SCREEN_HEIGHT;
	}
	else if (y < 0) {
		y = 0;
	}

	// X座標が定められた位置を超えていたら戻す
	// 中心線仕様を実装するかもしれないので、REDとBLUEで一応分けている。
	if (num == GameManager::PLAYERCOLOR::RED) {
		if (GameMain::SCREEN_WIDTH < x) {
			x = GameMain::SCREEN_WIDTH;
		}
		else if (x < 0) {
			x = 0;
		}
	}
	else if(num == GameManager::PLAYERCOLOR::BLUE){
		if (GameMain::SCREEN_WIDTH < x) {
			x = GameMain::SCREEN_WIDTH;
		}
		else if (x < 0) {
			x = 0;
		}
	}

	// ブロックたちと当たり判定する。
	//BlockHitCheck();	

	targetx = cosf(angle * DX_PI_F / 180) * GameMain::SCREEN_WIDTH + x;	// 狙っている方向のX座標
	targety = sinf(angle * DX_PI_F / 180) * GameMain::SCREEN_WIDTH + y;	// 狙っている方向のY座標

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

	Collision::Vector2 crossPosition = { -10000, -10000, false };	// 比較用。
	float distance1 = -10000;
	float distance2 = -10000;
	int num = 0;
	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {		// 最初の配列の値は入れているので2番目から開始
		if (!cross[i].flg) continue;					// 有効な値が入っていなかったらスキップ
		distance1 = hypot(crossPosition.x - x, crossPosition.y - y);	// プレイヤーと衝突点までの距離
		distance2 = hypot(cross[i].x - x, cross[i].y - y);
		if (distance2 < distance1) {					// 比較したほうがプレイヤーと近かったら渡す用の構造体の値を、その値に更新
			crossPosition.x = cross[i].x;
			crossPosition.y = cross[i].y;
			crossPosition.flg = true;
			num = i;
		}
	}

	// なにかしら有効な値が入っていたら、ターゲット位置をその座標にする
	if (crossPosition.flg) {
		targetx = crossPosition.x;	// 狙っている方向のX座標
		targety = crossPosition.y;	// 狙っている方向のY座標
		// デバッグ用表示
		DrawFormatStringToHandle(200, 650, 0xffffff, gameMain->fontData->f_FontData[0], "衝突する座標X%.2f", targetx);
		DrawFormatStringToHandle(200, 670, 0xffffff, gameMain->fontData->f_FontData[0], "衝突する座標Y%.2f", targety);
		DrawFormatStringToHandle(200, 690, 0xffffff, gameMain->fontData->f_FontData[0], "衝突点までの距離%.2f", hypot(targetx - x,targety - y));
		CalcHitAfterAngle(num);
	}

	//TargetPointWindowHitCheck();

	// 発射ボタンを押すと、弾オブジェクトの初期化関数に値を入れて、フェーズを進める。
	if (inputManager->In_Button()[InputManager::A] == 1) {
		float rx = cosf(angle * DX_PI_F / 180) + x;		// X進行方向
		float ry = sinf(angle * DX_PI_F / 180) + y;		// Y進行方向

		// 弾の初期化。生存フラグをtrue、X進行方向、Y進行方向、角度、GameMainオブジェクトのポインタを渡す
		gameMain->bullet->BulletInit(true, rx, ry, (float)angle, gameMain);
		gameMain->gameManager->SetPhaseStatus(GameManager::RECOCHETWAIT);	// フェーズを進める
	}
}

// 隠れる側時の操作処理
void Player::HidingPlayerControll(void) {
	// 移動前の座標を記憶しておく
	preX = x;
	preY = y;

	// 移動処理
	if (inputManager->In_Stick_LY() >= 0.45f) {
		// 左スティックが上に傾けられていたら上に移動する
		y -= moveSpeed;
	}
	if (inputManager->In_Stick_LY() <= -0.45f) {
		// 左スティックが下に傾けられていたら下に移動する
		y += moveSpeed;
	}
	if (inputManager->In_Stick_LX() <= -0.45f) {
		// 左スティックが左に傾けられていたら左に移動する
		x -= moveSpeed;
	}
	if (inputManager->In_Stick_LX() >= 0.45f) {
		// 左スティックが右に傾けられていたら右に移動する
		x += moveSpeed;
	}

	// Y座標が画面外に行っていたら戻す
	if (GameMain::SCREEN_HEIGHT < y) {
		y = GameMain::SCREEN_HEIGHT;
	}
	else if (y < 0) {
		y = 0;
	}

	// X座標が定められた位置を超えていたら戻す
	// 中心線仕様を実装するかもしれないので、REDとBLUEで一応分けている。
	if (num == GameManager::PLAYERCOLOR::RED) {
		if (GameMain::SCREEN_WIDTH < x) {
			x = GameMain::SCREEN_WIDTH;
		}
		else if (x < 0) {
			x = 0;
		}
	}
	else if(num == GameManager::PLAYERCOLOR::BLUE){
		if (GameMain::SCREEN_WIDTH < x) {
			x = GameMain::SCREEN_WIDTH;
		}
		else if (x < 0) {
			x = 0;
		}
	}

	// ブロックたちと当たり判定する。
	BlockHitCheck();
}

// 描画用
void Player::DrawPlayer(void) {
	DrawCircle(x, y, size, color);
}

// 撃つ側時に狙っている方向に線を引いて描画する
void Player::DrawTargetAngle(void) {
	DrawLine(x, y, (int)targetx, (int)targety, color, 3);
	DrawCircle((int)targetx, (int)targety, 4, 0xFFFFFF);
	DrawLine((int)targetx, (int)targety, (int)targetx2, (int)targety2, color, 3);
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

void Player::CalcHitAfterAngle(int num) {
	// ブロックのX、Y、直径サイズのローカル変数
	int blockX, blockY, blockSize;

	// ローカル変数に、ブロックの座標とサイズを入れていく
	blockX = gameMain->block[num]->GetBlockX();
	blockY = gameMain->block[num]->GetBlockY();
	blockSize = gameMain->block[num]->GetBlockSize();

	float prex = targetx - cosf(angle * DX_PI_F / 180) * 5;	// 狙っている方向のX座標
	float prey = targety - sinf(angle * DX_PI_F / 180) * 5;	// 狙っている方向のY座標

	// プレイヤーの移動前X座標が幅の中にいたら、Y座標のみを戻して、X座標は変化させる
	if (collision->IsHitWicth(prex, blockX, blockSize)) {
		// 移動前座標が幅の中なら、向きの上下を変える
		angle2 = (360 - angle);
	}
	// プレイヤーの移動前Y座標が高さの中にいたら、X座標のみを戻して、Y座標は変化させる
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

Player::~Player() {
	
}