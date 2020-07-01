// 衝突判定の処理を書いているクラス。
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "DxLib.h"

class Collision {
public:
	// 二つのオブジェクトのXとYの中心座標、直径の大きさを受け取り、矩形判定をし、当たっているか否かをbool型で返す関数。
	bool IsHit(int x1, int y1, int size1, int x2, int y2, int size2);

	// オブジェクト1のX中心座標が、オブジェクト2の幅の中にいるかを返す関数。
	// 弾がブロックの幅の中にいたかを判定するために使っている。
	bool IsHitWicth(int x1, int x2, int size2);

	// オブジェクト1のY中心座標が、オブジェクト2の高さの中にいるかを返す関数。
	// 弾がブロックの高さの中にいたかを判定するために使っている。
	bool IsHitHeight(int y1, int y2, int size2);
private:

};

#endif // !_COLLISION_H_