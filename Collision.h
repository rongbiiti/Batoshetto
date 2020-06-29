#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "DxLib.h"

class Collision {
public:
	bool IsHit(int x1, int y1, int size1, int x2, int y2, int size2);
	bool IsHitWicth(int x1, int x2, int size2);
	bool IsHitHeight(int y1, int y2, int size2);
private:

};

#endif // !_COLLISION_H_