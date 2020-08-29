#include "TitleBullet.h"
#include <math.h>

////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
TitleBullet::TitleBullet(FontData* font) {
	fontData = font;

	isAlive = false;			// 弾が生きているか
	x = 0, y = 0;				// 座標 x,y
	preX = 0, preY = 0;
	moveX = 0, moveY = 0;		// 移動 x,y
	ricochetCount = 0;		// 残り跳弾回数
	movespeedX = BulletMoveSpeed_X, movespeedY = BulletMoveSpeed_Y;			// 移動速度
	angle = 0;				// 角度
	effect = new Effect;	// エフェクトオブジェクトの生成
	collision = new Collision;	// 衝突判定してくれるオブジェクトを生成し、ポインタを保存しておく

	LoadSounds();		// 音データ読み込み
}

////////////////////////////////////////////////
// 弾の初期化
////////////////////////////////////////////////
void TitleBullet::BulletInit(bool alive, int randx, int randy) {
	isAlive = alive;					// 弾の存在フラグ

	if (randy == 0) {
		y = 50;
		angle = 90;	// 下を向く
	}
	else {
		y = GameMain::SCREEN_HEIGHT - 50;
		angle = 270;		// 上を向く
	}

	if (randx == 0) {
		x = 50;
		if (angle == 270) {
			angle = 315;	// 下にも動いてたら左下向くようにする
		}
		else if (angle == 90) {
			angle = 45;	// 上にも動いてたら左上向くようにする
		}
	}
	else {
		x = GameMain::SCREEN_WIDTH - 50;
		if (angle == 270) {
			angle = 225;	// 下にも動いてたら右下向くようにする
		}
		else if (angle == 90) {
			angle = 135;		// 上にも動いてたら右上向くようにする
		}
	}

	preX = x;							// 移動前座標を初期化
	preY = y;
	ricochetCount = BulletRicochetCount;// 弾の残り跳弾回数リセット
	ChangeAngle();						// angleをもとに、移動量をリセットする
	PlaySoundMem(s_Fire, DX_PLAYTYPE_BACK);	// 発射時の音を鳴らす
}

////////////////////////////////////////////////
// 弾の移動処理
////////////////////////////////////////////////
void TitleBullet::TitleBulletControll() {
	// もし跳弾回数が0未満なら処理を抜ける
	if (RemainingRicochetTimesCheck()) return;

	// 移動前の座標を保存
	preX = x;
	preY = y;

	// 移動する
	x += moveX;
	y += moveY;

	IsScreenOutside();	// 画面外にいってないかチェック
}

////////////////////////////////////////////////
// 弾の描画処理
////////////////////////////////////////////////
void TitleBullet::DrawTitleBullet() {
	if (0 <= ricochetCount) {
		int dx = (int)x;
		int dy = (int)y;
		DrawLine(dx, dy, (int)preX, (int)preY, COLOR_VALUE_PLAYER[0], 4);
		DrawCircle(dx, dy, Size, color);
	}

	effect->DrawRicochetEffect();		// エフェクト描画
	effect->DrawHitEffect();
}

////////////////////////////////////////////////
// 弾の残り跳弾回数チェック
////////////////////////////////////////////////
bool TitleBullet::RemainingRicochetTimesCheck() {
	// 跳弾回数0未満なら生存フラグを折る
	if (ricochetCount < 0) {
		isAlive = false;
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// 弾が画面端に行ってないかチェック
////////////////////////////////////////////////
bool TitleBullet::IsScreenOutside() {
	if (x >= GameMain::SCREEN_WIDTH || x <= 0) {
		--ricochetCount;				// 跳弾回数減らす
		float effAng = 0;
		if (x <= 0) {
			effAng = 0;
		}
		else {
			effAng = (180 * 3.14) / 180;
		}
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);		// 跳弾音再生
		angle = (360 - angle) + 180;	// 向きの上下を反転させる
		if (angle > 360) angle -= 360;	// 360度におさまるようにする
		ChangeAngle();					// 角度をもとに進行方向変更
		x = preX;
		y = preY;
		preX = x;
		preY = y;
		effect->InitRicochetCount(BulletRicochetCount - ricochetCount - 1, preX, preY, effAng);	// エフェクト描画
		return true;
	}

	if (y >= GameMain::SCREEN_HEIGHT || y <= 0) {
		--ricochetCount;				// 跳弾回数減らす
		float effAng = 0;
		if (y <= 0) {
			effAng = (90 * 3.14) / 180;
		}
		else {
			effAng = (270 * 3.14) / 180;
		}
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);
		angle = (360 - angle);			// 向きの左右を反転させる
		ChangeAngle();					// 角度をもとに進行方向変更
		x = preX;
		y = preY;
		preX = x;
		preY = y;
		effect->InitRicochetCount(BulletRicochetCount - ricochetCount - 1, preX, preY, effAng);	// エフェクト描画
		return true;
	}

	return false;
}

////////////////////////////////////////////////
// 弾の角度変更
////////////////////////////////////////////////
void TitleBullet::ChangeAngle() {
	float rad = (angle / 360) * DX_PI_F * 2;	// ラジアンに変換する
	moveX = (movespeedX * cosf(rad));
	moveY = (movespeedY * sinf(rad));
}

////////////////////////////////////////////////
// 弾の効果音音量変更
////////////////////////////////////////////////
void TitleBullet::ChangeVolume(float persent) {
	int volume = 153.0f * persent;

	ChangeVolumeSoundMem(volume, s_Fire);
	ChangeVolumeSoundMem(volume, s_Ricochet);
}

////////////////////////////////////////////////
// 弾の効果音読み込み
////////////////////////////////////////////////
void TitleBullet::LoadSounds() {
	if ((s_Fire = LoadSoundMem("sounds/SE/Fire.mp3")) == -1) return;
	if ((s_Ricochet = LoadSoundMem("sounds/SE/Ricochet.mp3")) == -1) return;
}

// デストラクタ
TitleBullet::~TitleBullet() {
	delete collision;
	delete effect;
	collision = NULL;
	effect = NULL;

	s_Fire = DeleteSoundMem(s_Fire);
	s_Ricochet = DeleteSoundMem(s_Ricochet);
}