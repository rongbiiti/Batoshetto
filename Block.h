// ステージ上に配置するブロックを管理するクラス
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "DxLib.h"
#include "FontData.h"
#include "GameMain.h"

class GameMain;
class Block {
public:
	Block(int num, FontData* font, GameMain* main);				// コンストラクタ。ブロックの番号と、フォント管理オブジェクトのポインタを入れる。
	~Block();									// デストラクタ。
	void DrawBlocks(void);						// ブロックを描画する関数
	const static int BLOCK_SIZE = 80;			// ブロックサイズ
	const static int BLOCK_ONE_MAX = 9;			// １ブロック数の最大数、エキスパートのブロック最大数
	const static int BLOCK_Casual_MAX = 5;		// カジュアルブロックの最大数

	bool IsAlive(void) { return isAlive; }		// ブロックが存在しているかを返す。
	int GetBlockX(void) { return x; }			// ブロックのX中心座標を返す。
	int GetBlockY(void) { return y; }			// ブロックのY中心座標を返す。
	int GetBlockSize(void) { return size; }		// ブロックの直径を返す。
	void ReadFile(void);

	void DecrementBlockHP(void);				// ブロックのHPを減らす関数。Bulletから呼ばれる。
private:
	// X、Y,
	const int BlockStartPosition[9][2] = { {280,40}, {600,40},{840,40},{280,280},{600,280},{840,280},{280,520}, {520,520},{840,520}, };
	const int BlockStartPosition2[5][2] = { {280,40}, {840,40},{520,280},{280,520},{840,520},};
	
	int BlockPosition[BLOCK_ONE_MAX][BLOCK_ONE_MAX] = {};				// ３×３ブロックの種類(エキスパート用)
	int BlockPosition2[BLOCK_ONE_MAX][BLOCK_ONE_MAX] = {};				// ３×３ブロックの種類２(エキスパート用)
	int BlockPosition_Casual[BLOCK_Casual_MAX][BLOCK_ONE_MAX] = {};		// ３×３ブロックの種類(カジュアル用)
	int BlockPosition_Casual2[BLOCK_Casual_MAX][BLOCK_ONE_MAX] = {};	// ３×３ブロックの種類２(カジュアル用)

	// X、Y、サイズの順。
	const int BlockStartHP = 3;		// ブロックの初期HP

	FontData* fontData;				// コンストラクタで受け取ったフォント管理オブジェクトのポインタを保存しておく変数
	GameMain* gamemain;				// コンストラクタで受け取ったゲームメインのポインタを保存しておく変数

	bool isAlive;			// 生きているか		
	int x, y;				// 座標 x,y
	int size;				// 大きさ
	int HP;					// ブロックのHP
	int num;
	int rnd;				// ブロックの種類
	FILE *fp;				// ファイル用

	int breakAnimWaitTime;	// 破壊アニメの待ち時間
	float breakAngle;		// 破壊アニメの角度
	
};

#endif // !_BLOCK_H_