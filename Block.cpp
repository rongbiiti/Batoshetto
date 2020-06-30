#include "Block.h"

Block::Block(int num, FontData* font) {
	isAlive = true;
	x = BlockStartPosition[num][0];
	y = BlockStartPosition[num][1];
	size = BlockStartPosition[num][2];
	HP = BlockStartHP;
	fontData = font;
}

void Block::DrawBlocks(void) {
	if (!isAlive) return;
	int dx1, dx2, dy1, dy2;
	dx1 = x - size / 2;
	dx2 = x + size / 2;
	dy1 = y - size / 2;
	dy2 = y + size / 2;
	
	DrawBox(dx1, dy1, dx2, dy2, 0x7A6611, 1);
	DrawFormatStringToHandle(x - size / 3, y - size / 3, 0xFFFFFF, fontData->f_FontData[1], "%d", HP);
}

void Block::DecrementBlockHP(void) {
	if (--HP <= 0) {
		isAlive = false;
	}
}

Block::~Block() {
	
}