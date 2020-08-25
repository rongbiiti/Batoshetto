// 衝突判定の処理を書いているクラス。
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "DxLib.h"

class Collision {
public:

	typedef struct POINT	// x座標とy座標をまとめて渡せるようにするための構造体
	{
		float x, y;	// 座標 x,y
		int num;	// 辺の番号
		bool flg;	// 有効な数値が入っているか
	}Vector2;

	// 二つのオブジェクトのXとYの中心座標、直径の大きさを受け取り、矩形判定をし、当たっているか否かをbool型で返す関数。
	bool IsHit(int x1, int y1, int size1, int x2, int y2, int size2);

	// オブジェクト1のX中心座標が、オブジェクト2の幅の中にいるかを返す関数。
	// 弾がブロックの幅の中にいたかを判定するために使っている。
	bool IsHitWicth(int x1, int x2, int size2);

	// オブジェクト1のY中心座標が、オブジェクト2の高さの中にいるかを返す関数。
	// 弾がブロックの高さの中にいたかを判定するために使っている。
	bool IsHitHeight(int y1, int y2, int size2);

	// プレイヤーの狙っている方向がブロックと衝突した最もプレイヤーに近い衝突点を返す
	// 引数は、プレイヤーの中心X、Y、ターゲットX、Y、ブロックの中心X、Y、直径サイズ。
	Vector2 IsHitTargetAndBlock(float px, float py, float tx, float ty, float bx, float by, float bsize);

	// 線と線が衝突しているか返す
	Vector2 GetHitLineAndLine(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2);
	
private:
	// IsHitTargetAndBlockでは、矩形と線分を一気に当たり判定している。
	// 実は、矩形を4つの線分として、”線分と線分”の当たり判定を4回行っているのだ。
	// IsHitTargetAndBlockの中で、この関数は4回呼ばれている。
	bool IsHitLineAndLine(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2);

	// 2点間の距離を計算して返す。
	float GetPointsDistance(float x1, float y1, float x2, float y2);

	Vector2 position = { 0, 0, 0, false };	// 構造体生成

};

#endif // !_COLLISION_H_