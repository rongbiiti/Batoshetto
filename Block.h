#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "DxLib.h"
#include "FontData.h"

class Block {
public:
	Block(int num, FontData* font);
	~Block();
	void DrawBlocks(void);

	bool IsAlive(void) { return isAlive; }
	int GetBlockX(void) { return x; }
	int GetBlockY(void) { return y; }
	int GetBlockSize(void) { return size; }

	void DecrementBlockHP(void);
private:
	// X、Y、サイズの順。
	const int BlockStartPosition[6][3] = { {400,100,128}, {200,300,128},{400,500,128},{800,100,128},{1000,300,128},{800,500,128} };
	// X、Y、サイズの順。
	const int BlockStartHP = 3;

	FontData* fontData;

	bool isAlive;			// 生きているか
	int x, y;				// 座標 x,y
	int size;				// 大きさ
	int HP;					// ブロックのHP
};

#endif // !_BLOCK_H_