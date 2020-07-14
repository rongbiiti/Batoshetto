#include "Block.h"
#include "GameMain.h"

// コンストラクタ。ブロックの番号と、フォント管理オブジェクトのポインタを入れる。
Block::Block(int num, FontData* font) {
	isAlive = true;			// 生存フラグオン
	int knt,rem;
	knt =  num / BLOCK_ONE_MAX;
	rem =  num % BLOCK_ONE_MAX;
	//x = BlockStartPosition[knt][0] + num % GameMain::BLOCK_MAX * BLOCK_SIZE;		// 番号をもとにX座標初期位置を決める
	//y = BlockStartPosition[knt][1] + num % GameMain::BLOCK_MAX * BLOCK_SIZE;		// Y座標初期位置
	switch (rem)
	{
	case 0:
		x = BlockStartPosition[knt][0];
		y = BlockStartPosition[knt][1];
		break;
	case 1:
		x = BlockStartPosition[knt][0] + 1 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1];
		break;
	case 2:
		x = BlockStartPosition[knt][0] + 2 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1];
		break;
	case 3:
		x = BlockStartPosition[knt][0];
		y = BlockStartPosition[knt][1] + 1 * BLOCK_SIZE;
		break;
	case 4:
		x = BlockStartPosition[knt][0] + 1 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1] + 1 * BLOCK_SIZE;
		break;
	case 5:
		x = BlockStartPosition[knt][0] + 2 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1] + 1 * BLOCK_SIZE;
		break;
	case 6:
		x = BlockStartPosition[knt][0];
		y = BlockStartPosition[knt][1] + 2 * BLOCK_SIZE;
		break;
	case 7:
		x = BlockStartPosition[knt][0] + 1 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1] + 2 * BLOCK_SIZE;
		break;
	case 8:
		x = BlockStartPosition[knt][0] + 2 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1] + 2 * BLOCK_SIZE;
		break;
	}
	if (BlockPosition[knt][rem] == 0) {
		isAlive = false;
	}
	size = BLOCK_SIZE;		// サイズ
	HP = BlockStartHP;		// HP初期化
	fontData = font;		// フォントデータのポインタ
	this->num = num;
}

// ブロックを描画する関数
void Block::DrawBlocks(void) {
	if (!isAlive) return;		// ブロックが死んでいたら処理を抜ける
	int dx1, dx2, dy1, dy2;		// 描画用のローカル変数
	dx1 = x - size / 2;
	dx2 = x + size / 2;
	dy1 = y - size / 2;
	dy2 = y + size / 2;
	DrawBox(dx1, dy1, dx2, dy2, 0x7A6611, 1);
	DrawFormatStringToHandle(x - size / 3 - 10, y, 0xFFFFFF, fontData->f_FontData[0], "HP%d,%d番", HP, num);
}

// ブロックのHPを減らす関数。Bulletから呼ばれる。
void Block::DecrementBlockHP(void) {
	if (--HP <= 0) {
		// HPをデクリメントして0以下なら生存フラグfalseにする
		isAlive = false;
	}
}

Block::~Block() {
	
}