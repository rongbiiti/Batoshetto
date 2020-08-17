#include "Collision.h"
#include <math.h>

////////////////////////////////////////////////
// 二つのオブジェクトのXとYの中心座標、直径の大きさを受け取り、矩形判定をし、当たっているか否かをbool型で返す関数。
////////////////////////////////////////////////
bool Collision::IsHit(int x1, int y1, int size1, int x2, int y2, int size2) {
	int axLeft = x1 - size1 / 2;	// オブジェクト1の左端
	int axRight = x1 + size1 / 2;	// オブジェクト1の右端
	int axTop = y1 - size1 / 2;		// オブジェクト1の上端
	int axBottom = y1 + size1 / 2;	// オブジェクト1の下端

	int bxLeft = x2 - size2 / 2;	// オブジェクト2の左端
	int bxRight = x2 + size2 / 2;	// オブジェクト2の右端
	int bxTop = y2 - size2 / 2;		// オブジェクト2の上端
	int bxBottom = y2 + size2 / 2;	// オブジェクト2の下端

	// 矩形の当たり判定
	if (bxLeft <= axRight && axLeft <= bxRight && bxTop <= axBottom && axTop <= bxBottom) {
		return true;
	}
	

	return false;
}

////////////////////////////////////////////////
// オブジェクト1のX中心座標が、オブジェクト2の幅の中にいるかを返す関数。
// 弾がブロックの幅の中にいたかを判定するために使っている。
////////////////////////////////////////////////
bool Collision::IsHitWicth(int x1, int x2, int size2) {
	int ax = x1;					// オブジェクト1の中心X座標

	int bxLeft = x2 - size2 / 2;	// オブジェクト2の左端
	int bxRight = x2 + size2 / 2;	// オブジェクト2の右端

	// オブジェクト1の中心がオブジェクト2の幅の中にあるか判定する
	if (bxLeft <= ax && ax <= bxRight) {
		return true;
	}

	return false;
}

////////////////////////////////////////////////
// オブジェクト1のY中心座標が、オブジェクト2の高さの中にいるかを返す関数。
// 弾がブロックの高さの中にいたかを判定するために使っている。
////////////////////////////////////////////////
bool Collision::IsHitHeight(int y1, int y2, int size2) {
	int ay = y1;					// オブジェクト1の中心Y座標

	int bxTop = y2 - size2 / 2;		// オブジェクト2の上端
	int bxBottom = y2 + size2 / 2;	// オブジェクト2の下端

	// オブジェクト2の中心がオブジェクト2の幅の中にあるか判定する
	if (bxTop <= ay && ay <= bxBottom) {
		return true;
	}

	return false;
}

////////////////////////////////////////////////
// プレイヤーの狙っている方向がブロックと衝突しているかを返す関数
////////////////////////////////////////////////
Collision::Vector2 Collision::IsHitTargetAndBlock(float px, float py, float tx, float ty, float bx, float by, float bsize) {

	float left, right, top, bottom;	// ブロックの上下左右の座標
	Vector2 cross[4];				// 比較用の構造体配列
	for (int i = 0; i < 4; i++) {
		cross[i].x = -10000;		// 間違っても一番近い座標にならないようにする
		cross[i].y = -10000;
		cross[i].num = 0;
		cross[i].flg = false;
	}
	left = bx - bsize / 2;
	right = bx + bsize / 2;
	top = by - bsize / 2;
	bottom = by + bsize / 2;

	// 渡す点は、左上から時計回りに、それぞれ2回ずつ渡している。
	if (IsHitLineAndLine(left, top, right, top, px, py, tx, ty)) { cross[0] = position; }		// 矩形の左上と右上を結ぶ線　上辺
	if (IsHitLineAndLine(right, top, right, bottom, px, py, tx, ty)) { cross[1] = position; }	// 右上と右下を結ぶ線　　　　右辺
	if (IsHitLineAndLine(right, bottom, left, bottom, px, py, tx, ty)) { cross[2] = position; }	// 右下と左下を結ぶ線　　　　底辺
	if (IsHitLineAndLine(left, bottom, left, top, px, py, tx, ty)) { cross[3] = position; }		// 左下と左上を結ぶ線　　　　左辺

	// どの点が一番プレイヤーに近いかを比較する
	position.x = cross[0].x;	// 最初の配列の値を代入しておく
	position.y = cross[0].y;
	position.num = 0;
	position.flg = cross[0].flg;

	float distance1 = 0;		// 2点間の距離
	float distance2 = 0;

	for (int i = 1; i < 4; i++) {		// 最初の配列の値は入れているので2番目から開始
		if (!cross[i].flg) continue;	// 有効な値が入っていなかったらスキップ
		distance1 = GetPointsDistance(px, py, position.x, position.y);	// プレイヤーと衝突点までの距離
		distance2 = GetPointsDistance(px, py, cross[i].x, cross[i].y);
		if (distance2 <= distance1) {	// 比較したほうがプレイヤーと近かったら渡す用の構造体の値を、その値に更新
			position.x = cross[i].x;
			position.y = cross[i].y;
			position.num = i;
			position.flg = true;
		}
	}
	
	return position;
}

////////////////////////////////////////////////
// IsHitTargetAndBlockでは、矩形と線分を一気に当たり判定している。
// 実は、矩形を4つの線分として、”線分と線分”の当たり判定を4回行っているのだ。
// IsHitTargetAndBlockの中で、この関数は4回呼ばれている。
////////////////////////////////////////////////
bool Collision::IsHitLineAndLine(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2) {
	// B1B2×B1A1・B1B2×B1A2 <= 0 ∪ A1A2×A1B1・A1A2×A1B2 <= 0 らしい。

	float result1 = (ax2 - ax1)*(by2 - by1) - (ay2 - ay1)*(bx2 - bx1);
	if (result1 == 0)
		return false; // 並行だったらfalse

	float result2 = ((bx1 - ax1)*(by2 - by1) - (by1 - ay1)*(bx2 - bx1)) / result1;
	float result3 = ((bx1 - ax1)*(ay2 - ay1) - (by1 - ay1)*(ax2 - ax1)) / result1;

	if (result2 < 0.0 || result2 > 1.0)
		return false; // p1とp2の間にない交点

	if (result3 < 0.0 || result3 > 1.0)
		return false; // p3とp4の間にない交点

	position.x = ax1 + result2 * (ax2 - ax1);	// ここまで処理が通ったら、構造体に値を入れていく。
	position.y = ay1 + result2 * (ay2 - ay1);
	position.flg = true;

	return true;
}

////////////////////////////////////////////////
Collision::Vector2 Collision::GetHitLineAndLine(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2) {
	Vector2 cross = { -10000, -10000, false };

	float result1 = (ax2 - ax1)*(by2 - by1) - (ay2 - ay1)*(bx2 - bx1);
	if (result1 == 0)
		return cross; // 並行だったらfalse

	float result2 = ((bx1 - ax1)*(by2 - by1) - (by1 - ay1)*(bx2 - bx1)) / result1;
	float result3 = ((bx1 - ax1)*(ay2 - ay1) - (by1 - ay1)*(ax2 - ax1)) / result1;

	if (result2 < 0.0 || result2 > 1.0)
		return cross; // p1とp2の間にない交点

	if (result3 < 0.0 || result3 > 1.0)
		return cross; // p3とp4の間にない交点

	cross.x = ax1 + result2 * (ax2 - ax1);	// ここまで処理が通ったら、構造体に値を入れていく。
	cross.y = ay1 + result2 * (ay2 - ay1);
	cross.flg = true;

	return cross;
}

////////////////////////////////////////////////
// 2点間の距離を計算して返す。
////////////////////////////////////////////////
float Collision::GetPointsDistance(float x1, float y1, float x2, float y2) {
	float distance = hypotf(x2 - x1, y2 - y1);

	return distance;
}