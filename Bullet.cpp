#include "Bullet.h"
#include "GameMain.h"
#include <math.h>

// コンストラクタ
Bullet::Bullet(void)
{
	isAlive = false;			// 弾が生きているか
	x = 0, y = 0;				// 座標 x,y
	preX = 0, preY = 0;
	moveX = 0, moveY = 0;		// 移動 x,y
	ricochetCount = 0;		// 残り跳弾回数
	movespeedX = BulletMoveSpeed_X, movespeedY = BulletMoveSpeed_Y;			// 移動速度
	angle = 0;				// 角度
	hitFlg = false;
	collision = new Collision;	// 衝突判定してくれるオブジェクトを生成し、ポインタを保存しておく
	gameMain = NULL;
	isPlayerHit = false;
	waitingTimeAfterPlayerHit = 0;
	HitPlayerNum = 0;
	LoadSounds();
}

// 初期化用関数
void Bullet::BulletInit(bool alive, float rx, float ry, float ang, GameMain* main) {
	isAlive = alive;					// 弾の存在フラグ
	x = rx;								// 弾のX座標
	y = ry;								// 弾のY座標
	preX = x;							// 移動前座標を初期化
	preY = y;
	ricochetCount = BulletRicochetCount;// 弾の残り跳弾回数リセット
	angle = ang;						// 弾の進行方向の角度
	ChangeAngle();						// angleをもとに、移動量をリセットする
	gameMain = main;					// GameMainのポインタ
	shooterHitOK = false;				// 撃った瞬間に撃つ側に当たり判定しないようにする
	isPlayerHit = false;
	waitingTimeAfterPlayerHit = 0;
	HitPlayerNum = 0;
	PlaySoundMem(s_Fire, DX_PLAYTYPE_BACK);
}

// 弾が実際に動くときの処理
void Bullet::BulletControll(void) {
	if (isPlayerHit && ResultTransitionWaiting()) {
		gameMain->gameManager->SetPhaseStatus(GameManager::RESULT, gameMain->player[HitPlayerNum]->GetPlayerNum());
	}
	// もし跳弾回数が0未満なら処理を抜ける
	if (RemainingRicochetTimesCheck()) return;

	// 移動前の座標を保存
	preX = x;
	preY = y;

	// 移動する
	x += moveX;
	y += moveY;

	if (IsScreenOutside()) return;	// 画面外にいってないかチェック
	if (IsHitPlayer() && !isPlayerHit) return;		// プレイヤーと当たり判定

	// ブロックと連続で当たり判定しないように、前のフレームでブロックと当たっていたら
	// 今回のフレームはブロックと当たり判定しないで抜ける
	/*if (hitFlg) {
		hitFlg = false;
		return;
	}*/

	// もし跳弾回数が0未満なら処理を抜ける
	if (RemainingRicochetTimesCheck()) return;

	if (gameMain->inputManager->GetPadInput()[gameMain->gameManager->GetNowShooter()].in_Button[InputManager::PAD_RIGHT] != 0)return;

	if (IsHitBlock()) return;		// ブロックと当たり判定
}

// 描画
void Bullet::DrawBullet(void) {
	// もし跳弾回数が0未満なら処理を抜ける
	if (RemainingRicochetTimesCheck()) return;

	int dx = (int)x;
	int dy = (int)y;

	DrawLine(dx, dy, (int)preX, (int)preY, COLOR_VALUE_PLAYER[gameMain->gameManager->GetNowShooter()], 4);
	DrawCircle(dx, dy, Size, color);
}

// 跳弾回数が0未満になっていないかチェックする
bool Bullet::RemainingRicochetTimesCheck(void) {
	if (ricochetCount < 0) {
		isAlive = false;
		// もし0未満なら生存フラグをfalseにして隠れる側フェーズに移行する
		if (!isPlayerHit) { 
			gameMain->gameManager->ToHidePhase(); 			
		}
		return true;
	}
	return false;
}

// 角度をもとに進行方向変更
void Bullet::ChangeAngle(void) {
	float rad = (angle / 360) * DX_PI_F * 2;	// ラジアンに変換する
	moveX = (movespeedX * cosf(rad));
	moveY = (movespeedY * sinf(rad));
}

// 画面外に出ていないかチェックする
bool Bullet::IsScreenOutside(void) {
	if (x >= GameMain::SCREEN_WIDTH || x <= 0) {
		--ricochetCount;				// 跳弾回数減らす
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);
		angle = (360 - angle) + 180;	// 向きの上下を反転させる
		if (angle > 360) angle -= 360;	// 360度におさまるようにする
		ChangeAngle();					// 角度をもとに進行方向変更
		x = preX;
		y = preY;
		x = x + cosf(angle * DX_PI_F / 180.0f) * (movespeedX / 2);	// 狙っている方向のX座標
		y = y + sinf(angle * DX_PI_F / 180.0f) * (movespeedY / 2);	// 狙っている方向のY座標
		shooterHitOK = true;			// 撃つ側と当たり判定できるようにする
		RemainingRicochetTimesCheck();	// 跳弾回数0未満なら隠れる側フェーズに移行
		return true;
	}

	if (y >= GameMain::SCREEN_HEIGHT || y <= 0) {
		--ricochetCount;				// 跳弾回数減らす
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);
		angle = (360 - angle);			// 向きの左右を反転させる
		ChangeAngle();					// 角度をもとに進行方向変更
		x = preX;					
		y = preY;
		x = x + cosf(angle * DX_PI_F / 180.0f) * (movespeedX / 2);	// 狙っている方向のX座標
		y = y + sinf(angle * DX_PI_F / 180.0f) * (movespeedY / 2);	// 狙っている方向のY座標
		shooterHitOK = true;			// 撃つ側と当たり判定できるようにする
		RemainingRicochetTimesCheck();	// 跳弾回数0未満なら隠れる側フェーズに移行
		return true;					
	}

	return false;
}

// プレイヤーと当たり判定
bool Bullet::IsHitPlayer(void) {
	if (isPlayerHit) return false;
	// プレイヤーのX、Y、直径サイズ
	int playerX, playerY, playerSize;

	for (int i = 0; i < 2; i++) {
		// 撃つ側と添え字のプレイヤーの色が同じでshooterHitOKがfalseなら処理をcontinueして次のプレイヤーを見る
		if (gameMain->gameManager->GetNowShooter() == gameMain->player[i]->GetPlayerNum() && !shooterHitOK) continue;
		playerX = gameMain->player[i]->GetPlayerX();		// プレイヤーのX座標
		playerY = gameMain->player[i]->GetPlayerY();		// プレイヤーのY座標
		playerSize = gameMain->player[i]->GetPlayerSize();	// プレイヤーの直径サイズ

		// プレイヤーと当たり判定し、当たっていたらリザルト画面に飛ぶようにGameManagerにお願いし、プレイヤーの色を渡す
		if (collision->IsHit((int)x, (int)y, Size, playerX, playerY, playerSize)) {
			HitPlayerNum = i;
			isPlayerHit = true;
			PlaySoundMem(s_PlayerHit, DX_PLAYTYPE_BACK);
			PlaySoundMem(s_Blood, DX_PLAYTYPE_BACK);
			ricochetCount = -1;
			return true;
		}
	}

	return false;
}

// ブロックと当たり判定
bool Bullet::IsHitBlock(void) {
	// もし跳弾回数が0未満なら処理を抜ける
	if (RemainingRicochetTimesCheck()) return true;

	// ブロックの中心X、Y、直径サイズ
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
		cross[i] = collision->IsHitTargetAndBlock(preX, preY, x, y, (float)blockX, (float)blockY, (float)blockSize);
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
		
		shooterHitOK = true;	// 撃つ側のプレイヤーに当たるようにする
		gameMain->block[num]->DecrementBlockHP();	// ブロックのHPを減らす
		if (!gameMain->block[num]->IsAlive()) {
			PlaySoundMem(s_BlockBreak, DX_PLAYTYPE_BACK);
			return true;
		}
		x = crossPosition.x;	// 狙っている方向のX座標
		y = crossPosition.y;	// 狙っている方向のY座標
		--ricochetCount;		// 当たっていたら、跳弾回数を減らす
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);
		//hitFlg = true;	// 連続でブロックに当たらないようにフラグを立てる


		lastHitPointX = x - cosf(angle * DX_PI_F / 180.0f) * (movespeedX / 2.5f);	// 狙っている方向のX座標
		lastHitPointY = y - sinf(angle * DX_PI_F / 180.0f) * (movespeedY / 2.5f);	// 狙っている方向のY座標

		blockX = gameMain->block[num]->GetBlockX();
		blockY = gameMain->block[num]->GetBlockY();
		blockSize = gameMain->block[num]->GetBlockSize();

		if (collision->IsHitWicth((int)lastHitPointX, blockX, blockSize)) {
			// 移動前座標が幅の中なら、向きの上下を変える
			angle = (360 - angle);
		}
		else if (collision->IsHitHeight((int)lastHitPointY, blockY, blockSize)) {
			// 高さの中なら、向きの左右を変える
			angle = (360 - angle) + 180;
			if (angle > 360) angle -= 360;
		}
		else {
			// 角に当たった場合、左右のブロックの幅の中にいないかもう一度確かめる
			if ((collision->IsHitWicth((int)lastHitPointX, gameMain->block[num - 1]->GetBlockX(), gameMain->block[num - 1]->GetBlockSize()) && gameMain->block[num - 1]->IsAlive()) ||
				(collision->IsHitWicth((int)lastHitPointX, gameMain->block[num + 1]->GetBlockX(), gameMain->block[num + 1]->GetBlockSize()) && gameMain->block[num + 1]->IsAlive())) {
				// 移動前座標が幅の中なら、向きの上下を変える
				angle = (360 - angle);
			}
			// 上下のブロックの高さの中にいないかもう一度確かめる
			else if ((collision->IsHitHeight((int)lastHitPointY, gameMain->block[num - 3]->GetBlockY(), gameMain->block[num - 3]->GetBlockSize()) && gameMain->block[num - 3]->IsAlive()) ||
					 (collision->IsHitHeight((int)lastHitPointY, gameMain->block[num + 3]->GetBlockY(), gameMain->block[num + 3]->GetBlockSize()) && gameMain->block[num + 3]->IsAlive())) {
				// 高さの中なら、向きの左右を変える
				angle = (360 - angle) + 180;
				if (angle > 360) angle -= 360;
			}
			else {
				// 角なら、真逆の向きに
				angle = angle + 180;
				if (angle > 360) angle -= 360;
			}			
		}

		ChangeAngle();	// 角度をもとに進行方向変更
		x = lastHitPointX;
		y = lastHitPointY;
		x = x + cosf(angle * DX_PI_F / 180.0f) * (movespeedX / 2.5f);	// 狙っている方向のX座標
		y = y + sinf(angle * DX_PI_F / 180.0f) * (movespeedY / 2.5f);	// 狙っている方向のY座標
		RemainingRicochetTimesCheck();
		return true;
	}

	
	return false;
}

bool Bullet::ResultTransitionWaiting(void) {
	if (++waitingTimeAfterPlayerHit <= 240) {
		return false;
	}
	return true;
}

Bullet::~Bullet() {
	
}

// 音データ読み込み
void Bullet::LoadSounds(void) {
	if ((s_Fire = LoadSoundMem("sounds/Fire.mp3")) == -1) return;
	if ((s_Ricochet = LoadSoundMem("sounds/Ricochet.mp3")) == -1) return;
	if ((s_BlockBreak = LoadSoundMem("sounds/BlockBreak.mp3")) == -1) return;
	if ((s_PlayerHit = LoadSoundMem("sounds/PlayerHit.mp3")) == -1) return;
	if ((s_Blood = LoadSoundMem("sounds/Blood_Sibuki.mp3")) == -1) return;
}

// 音量変更
void Bullet::ChangeVolume(float persent) {
	int volume = 255.0f * persent;

	ChangeVolumeSoundMem(s_Fire, volume);
	ChangeVolumeSoundMem(s_Ricochet, volume);
	ChangeVolumeSoundMem(s_BlockBreak, volume);
	ChangeVolumeSoundMem(s_PlayerHit, volume);
	ChangeVolumeSoundMem(s_Blood, volume);
}