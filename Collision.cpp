#include "Collision.h"

// 二つのオブジェクトのXとYの中心座標、直径の大きさを受け取り、矩形判定をし、当たっているか否かをbool型で返す関数。
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

// オブジェクト1のX中心座標が、オブジェクト2の幅の中にいるかを返す関数。
// 弾がブロックの幅の中にいたかを判定するために使っている。
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

// オブジェクト1のY中心座標が、オブジェクト2の高さの中にいるかを返す関数。
// 弾がブロックの高さの中にいたかを判定するために使っている。
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