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

	bool IsAlive() { return isAlive; }						// 弾が存在しているかを返す
	int GetRicochetCount() { return ricochetCount; }		// 弾の残り跳弾回数を返す

private:
	Collision* collision;					// コンストラクタで生成した衝突判定をするオブジェクトを保存しておく変数
	GameMain* gameMain;						// 弾発射時にPlayerから受け取ったGameMainクラスのポインタを保存しておく変数
	const int BulletMoveSpeed = 20;			// 弾の速度
	const int BulletRicochetCount = 5;		// 弾の跳弾回数の初期値
	const int Size = 4;						// 弾の大きさ（直径）
	const unsigned int color = 0xFFFFFF;	// 弾の色

	bool RemainingRicochetTimesCheck(void);	// 跳弾回数が残っているかチェックする
	bool IsScreenOutside(void);				// 画面外に出ていないかチェックする
	bool IsHitPlayer(void);					// プレイヤーと当たっているかチェックする
	bool IsHitBlock(void);					// ブロックと当たっているかチェックする
	void ChangeAngle(void);					// 弾の進行方向を変更させる関数

	bool isAlive;			// 弾が生きているか
	float x, y;				// 座標 x,y
	float moveX, moveY;		// 移動 x,y
	float preX, preY;		// 移動前の座標
	int ricochetCount;		// 残り跳弾回数
	int movespeedx, movespeedy;			// 移動速度
	float angle;				// 角度
	bool hitFlg;			// ヒットしたかのフラグ。trueになった1フレームのみ当たり判定をせず、falseにして処理を中断して抜けるために使う。
	bool shooterHitOK;		// 撃ったほうにヒットしてもいいか
	float preprex, preprey;
};

#endif // !_BULLET_H_