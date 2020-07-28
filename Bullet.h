// 弾管理クラス
#ifndef _BULLET_H_
#define _BULLET_H_

#include "DxLib.h"
#include "Collision.h"
#include "GameMain.h"

class Bullet {
public:
	Bullet(void);				// コンストラクタ。メンバ変数の初期化をする。衝突判定をしてくれるクラスも生成する。
	~Bullet(void);				// デストラクタ

	void BulletInit(bool alive, float rx, float ry, float ang, GameMain* main);	// 初期化用関数
	void BulletControll(void);	// 弾が実際に動くときの処理
	void DrawBullet();			// 弾を描画する
	void DrawSHINOBIEXECUTION();

	bool IsAlive() { return isAlive; }						// 弾が存在しているかを返す
	int GetRicochetCount() { return ricochetCount; }		// 弾の残り跳弾回数を返す
	float GetBulletSPD_X() { return BulletMoveSpeed_X; }	// 弾のX方向の移動速度を返す
	float GetBulletSPD_Y() { return BulletMoveSpeed_Y; }	// 弾のY方向の移動速度を返す

	void ChangeVolume(float persent);	// 音量変更

private:
	Collision* collision;					// コンストラクタで生成した衝突判定をするオブジェクトを保存しておく変数
	GameMain* gameMain;						// 弾発射時にPlayerから受け取ったGameMainクラスのポインタを保存しておく変数
	const float BulletMoveSpeed_X = 17.0f;			// 弾の速度_X
	const float BulletMoveSpeed_Y = 17.0f;			// 弾の速度_Y
	const int BulletRicochetCount = 5;		// 弾の跳弾回数の初期値
	const int Size = 4;						// 弾の大きさ（直径）
	const unsigned int color = 0xFFFFFF;	// 弾の色

	bool RemainingRicochetTimesCheck(void);	// 跳弾回数が残っているかチェックする
	bool IsScreenOutside(void);				// 画面外に出ていないかチェックする
	bool IsHitPlayer(void);					// プレイヤーと当たっているかチェックする
	bool IsHitBlock(void);					// ブロックと当たっているかチェックする
	void ChangeAngle(void);					// 弾の進行方向を変更させる関数
	bool ResultTransitionWaiting(void);		// リザルト画面に飛ぶ前に少し待つ

	bool isAlive;			// 弾が生きているか
	float x, y;				// 座標 x,y
	float moveX, moveY;		// 移動 x,y
	float preX, preY;		// 移動前の座標
	int ricochetCount;		// 残り跳弾回数
	float movespeedX, movespeedY;			// 移動速度
	float angle;				// 角度
	bool hitFlg;			// ヒットしたかのフラグ。trueになった1フレームのみ当たり判定をせず、falseにして処理を中断して抜けるために使う。
	bool shooterHitOK;		// 撃ったほうにヒットしてもいいか
	float lastHitPointX, lastHitPointY;	// 最後にブロックなどと衝突した座標
	bool isPlayerHit;		// trueだとヒットした
	int waitingTimeAfterPlayerHit;	// プレイヤーヒット後の余韻時間
	int HitPlayerNum;		// ヒットしたプレイヤーの番号

	void LoadSounds(void);	// 音データ読み込み

	int s_Fire;	// 発射音
	int s_Ricochet;	// 跳弾音
	int s_BlockBreak;	// ブロック破壊音
	int s_PlayerHit[2];	// プレイヤーにヒットした音
	int s_Blood;		// 血が出る音
};

#endif // !_BULLET_H_