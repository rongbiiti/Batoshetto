// ステージ上に配置するブロックを管理するクラス
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "DxLib.h"
#include "FontData.h"

class Block {
public:
	Block(int num, FontData* font);				// コンストラクタ。ブロックの番号と、フォント管理オブジェクトのポインタを入れる。
	~Block();									// デストラクタ。
	void DrawBlocks(void);						// ブロックを描画する関数

	bool IsAlive(void) { return isAlive; }		// ブロックが存在しているかを返す。
	int GetBlockX(void) { return x; }			// ブロックのX中心座標を返す。
	int GetBlockY(void) { return y; }			// ブロックのY中心座標を返す。
	int GetBlockSize(void) { return size; }		// ブロックの直径を返す。

	void DecrementBlockHP(void);				// ブロックのHPを減らす関数。Bulletから呼ばれる。
private:
	// X、Y、サイズの順。
	const int BlockStartPosition[6][3] = { {400,100,128}, {200,300,128},{400,500,128},{800,100,128},{1000,300,128},{800,500,128} };
	// X、Y、サイズの順。
	const int BlockStartHP = 3;		// ブロックの初期HP

	FontData* fontData;				// コンストラクタで受け取ったフォント管理オブジェクトのポインタを保存しておく変数

	bool isAlive;			// 生きているか		
	int x, y;				// 座標 x,y
	int size;				// 大きさ
	int HP;					// ブロックのHP
	int num;
};

#endif // !_BLOCK_H_