#include "Block.h"

// コンストラクタ。ブロックの番号と、フォント管理オブジェクトのポインタを入れる。
Block::Block(int num, FontData* font) {
	isAlive = true;						// 生存フラグオン
	x = BlockStartPosition[num][0];		// 番号をもとにX座標初期位置を決める
	y = BlockStartPosition[num][1];		// Y座標初期位置
	size = BlockStartPosition[num][2];	// サイズ
	HP = BlockStartHP;					// HP初期化
	fontData = font;					// フォントデータのポインタ
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
	DrawFormatStringToHandle(x - size / 3, y - size / 3, 0xFFFFFF, fontData->f_FontData[1], "%d", HP);
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