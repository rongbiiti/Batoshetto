//タイトル画面の背景の弾のクラス
#ifndef _TITLEBULLET_H_
#define _TITLEBULLET_H_

#include "DxLib.h"
#include "FontData.h"
#include "Effect.h"
#include "Collision.h"

class Effect;
class TitleBullet {
public:
	TitleBullet(FontData* font);
	~TitleBullet();
	void BulletInit(bool alive, int randx, int randy);	// 初期化用関数
	void TitleBulletControll();
	void DrawTitleBullet();
	void ChangeVolume(float persent);	// 音量変更

	bool GetIsAlive() { return isAlive; }		// 弾が生きているか返す

private:
	FontData* fontData;				// 生成した入力管理クラスのポインタ
	Collision* collision;					// コンストラクタで生成した衝突判定をするオブジェクトを保存しておく変数

	const float BulletMoveSpeed_X = 16.0f;			// 弾の速度_X
	const float BulletMoveSpeed_Y = 16.0f;			// 弾の速度_Y
	const int BulletRicochetCount = 5;		// 弾の跳弾回数の初期値
	const int Size = 4;						// 弾の大きさ（直径）
	const unsigned int color = 0xFFFFFF;	// 弾の色

	bool RemainingRicochetTimesCheck(void);	// 跳弾回数が残っているかチェックする
	bool IsScreenOutside(void);				// 画面外に出ていないかチェックする
	void ChangeAngle(void);					// 弾の進行方向を変更させる関数
	
	Effect* effect;		//エフェクトオブジェクトのポインタ

	bool isAlive;			// 弾が生きているか
	float x, y;				// 座標 x,y
	float moveX, moveY;		// 移動 x,y
	float preX, preY;		// 移動前の座標
	int ricochetCount;		// 残り跳弾回数
	float movespeedX, movespeedY;			// 移動速度
	float angle;				// 角度
	float lastHitPointX, lastHitPointY;	// 最後にブロックなどと衝突した座標

	void LoadSounds(void);	// 音データ読み込み

	int s_Fire;	// 発射音
	int s_Ricochet;	// 跳弾音
};

#endif // !_TITLEBULLET_H_