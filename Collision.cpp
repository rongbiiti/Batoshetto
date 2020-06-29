#include "Collision.h"

bool Collision::IsHit(int x1, int y1, int size1, int x2, int y2, int size2) {
	int axLeft = x1 - size1 / 2;
	int axRight = x1 + size1 / 2;
	int axTop = y1 - size1 / 2;
	int axBottom = y1 + size1 / 2;

	int bxLeft = x2 - size2 / 2;
	int bxRight = x2 + size2 / 2;
	int bxTop = y2 - size2 / 2;
	int bxBottom = y2 + size2 / 2;

	if (bxLeft <= axRight && axLeft <= bxRight && bxTop <= axBottom && axTop <= bxBottom) {
		return true;
	}

	return false;
}

bool Collision::IsHitWicth(int x1, int x2, int size2) {
	int ax = x1;
	int bxLeft = x2 - size2 / 2;
	int bxRight = x2 + size2 / 2;

	if (bxLeft <= ax && ax <= bxRight) {
		return true;
	}

	return false;
}

bool Collision::IsHitHeight(int y1, int y2, int size2) {
	int ay = y1;
	int bxTop = y2 - size2 / 2;
	int bxBottom = y2 + size2 / 2;

	if (bxTop <= ay && ay <= bxBottom) {
		return true;
	}

	return false;
}