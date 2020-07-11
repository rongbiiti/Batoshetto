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
	movespeedx = BulletMoveSpeed, movespeedy = BulletMoveSpeed;			// 移動速度
	angle = 0;				// 角度
	hitFlg = false;
	collision = new Collision;	// 衝突判定してくれるオブジェクトを生成し、ポインタを保存しておく
	gameMain = NULL;
}

// 初期化用関数
void Bullet::BulletInit(bool alive, float rx, float ry, float ang, GameMain* main) {
	isAlive = alive;					// 弾の存在フラグ
	x = rx;								// 弾のX座標
	y = ry;								// 弾のY座標
	ricochetCount = BulletRicochetCount;// 弾の残り跳弾回数リセット
	angle = ang;						// 弾の進行方向の角度
	ChangeAngle();						// angleをもとに、移動量をリセットする
	gameMain = main;					// GameMainのポインタ
	shooterHitOK = false;				// 撃った瞬間に撃つ側に当たり判定しないようにする
}

// 弾が実際に動くときの処理
void Bullet::BulletControll(void) {
	// もし跳弾回数が0未満なら処理を抜ける
	if (RemainingRicochetTimesCheck()) return;

	// 移動前の座標を保存
	preX = x;
	preY = y;

	// 移動する
	x += moveX;
	y += moveY;

	if (IsScreenOutside()) return;	// 画面外にいってないかチェック
	if (IsHitPlayer()) return;		// プレイヤーと当たり判定

	// ブロックと連続で当たり判定しないように、前のフレームでブロックと当たっていたら
	// 今回のフレームはブロックと当たり判定しないで抜ける
	if (hitFlg) {
		hitFlg = false;
		return;
	}

	if (IsHitBlock()) return;		// ブロックと当たり判定

}

// 描画
void Bullet::DrawBullet(void) {
	int dx = (int)x;
	int dy = (int)y;
	DrawCircle(dx, dy, Size, color);
}

// 跳弾回数が0未満になっていないかチェックする
bool Bullet::RemainingRicochetTimesCheck(void) {
	if (ricochetCount < 0) {
		// もし0未満なら生存フラグをfalseにして隠れる側フェーズに移行する
		isAlive = false;
		gameMain->gameManager->ToHidePhase();
		return true;
	}
	return false;
}

// 角度をもとに進行方向変更
void Bullet::ChangeAngle(void) {
	float rad = (angle / 360) * DX_PI_F * 2;	// ラジアンに変換する
	moveX = (movespeedx * cosf(rad));
	moveY = (movespeedy * sinf(rad));
}

// 画面外に出ていないかチェックする
bool Bullet::IsScreenOutside(void) {
	if (x >= GameMain::SCREEN_WIDTH || x <= 0) {
		--ricochetCount;				// 跳弾回数減らす
		angle = (360 - angle) + 180;	// 向きの上下を反転させる
		if (angle > 360) angle -= 360;	// 360度におさまるようにする
		ChangeAngle();					// 角度をもとに進行方向変更
		x = preX;
		y = preY;
		x += moveX;
		y += moveY;
		shooterHitOK = true;			// 撃つ側と当たり判定できるようにする
		RemainingRicochetTimesCheck();	// 跳弾回数0未満なら隠れる側フェーズに移行
		return true;
	}

	if (y >= GameMain::SCREEN_HEIGHT || y <= 0) {
		--ricochetCount;				// 跳弾回数減らす
		angle = (360 - angle);			// 向きの左右を反転させる
		ChangeAngle();					// 角度をもとに進行方向変更
		x = preX;					
		y = preY;
		x += moveX;
		y += moveY;
		shooterHitOK = true;			// 撃つ側と当たり判定できるようにする
		RemainingRicochetTimesCheck();	// 跳弾回数0未満なら隠れる側フェーズに移行
		return true;					
	}

	return false;
}

// プレイヤーと当たり判定
bool Bullet::IsHitPlayer(void) {
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
			gameMain->gameManager->SetPhaseStatus(GameManager::RESULT, gameMain->player[i]->GetPlayerNum());
			return true;
		}
	}

	return false;
}

// ブロックと当たり判定
bool Bullet::IsHitBlock(void) {
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
			return true;
		}
		x = crossPosition.x;	// 狙っている方向のX座標
		y = crossPosition.y;	// 狙っている方向のY座標
		--ricochetCount;		// 当たっていたら、跳弾回数を減らす
		hitFlg = true;	// 連続でブロックに当たらないようにフラグを立てる


		preprex = x - cosf(angle * DX_PI_F / 180) * 5;	// 狙っている方向のX座標
		preprey = y - sinf(angle * DX_PI_F / 180) * 5;	// 狙っている方向のY座標

		blockX = gameMain->block[num]->GetBlockX();
		blockY = gameMain->block[num]->GetBlockY();
		blockSize = gameMain->block[num]->GetBlockSize();

		if (collision->IsHitWicth((int)preprex, blockX, blockSize)) {
			// 移動前座標が幅の中なら、向きの上下を変える
			angle = (360 - angle);
		}
		else if (collision->IsHitHeight((int)preprey, blockY, blockSize)) {
			// 高さの中なら、向きの左右を変える
			angle = (360 - angle) + 180;
			if (angle > 360) angle -= 360;
		}
		else {
			// 角なら、真逆の向きに
			angle = angle + 180;
			if (angle > 360) angle -= 360;
		}

		ChangeAngle();	// 角度をもとに進行方向変更
		x = preprex;
		y = preprey;
		
		return true;
	}

	
	return false;
}

Bullet::~Bullet() {
	
}