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
	// X�AY�A�T�C�Y�̏��B
	const int BlockStartPosition[6][3] = { {400,100,128}, {200,300,128},{400,500,128},{800,100,128},{1000,300,128},{800,500,128} };
	// X�AY�A�T�C�Y�̏��B
	const int BlockStartHP = 3;

	FontData* fontData;

	bool isAlive;			// �����Ă��邩
	int x, y;				// ���W x,y
	int size;				// �傫��
	int HP;					// �u���b�N��HP
};

#endif // !_BLOCK_H_