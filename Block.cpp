#include "Block.h"
#include "GameMain.h"

// コンストラクタ。ブロックの番号と、フォント管理オブジェクトのポインタを入れる。
Block::Block(int num, FontData* font ,GameMain* main) {

	int knt,rem;
	int gamemode;
	isAlive = true;			// 生存フラグオン
	gamemain = main;		// ゲームメインのポインタ
	size = BLOCK_SIZE;		// サイズ
	HP = BlockStartHP;		// HP初期化
	fontData = font;		// フォントデータのポインタ
	gamemode = main->gameManager->GetDifficulty();

	knt =  num / BLOCK_ONE_MAX;		// ブロックの初期位置を商で求める
	rem =  num % BLOCK_ONE_MAX;		// ３×３ブロックの何番目かを余りで求める

	// ３×３ブロックの番号を元に座標を指定
	if (rem == 0) {
		switch (gamemode)
		{
		case 1:
			x = BlockStartPosition2[knt][0];
			y = BlockStartPosition2[knt][1];
			break;
		case 2:
			x = BlockStartPosition[knt][0];
			y = BlockStartPosition[knt][1];
			break;
		}
	}
	else if (rem < 3 && rem > 0) {
		x = gamemain->block[num - 1]->GetBlockX() + BLOCK_SIZE;
		y = gamemain->block[num - 1]->GetBlockY();
	}
	else if (rem < 9 && rem >= 3) {
		x = gamemain->block[num - 3]->GetBlockX();
		y = gamemain->block[num - 3]->GetBlockY() + BLOCK_SIZE;
	}


	// 余りが０だった場合３×３ブロックの始まりなので乱数で２種類のうちどちらのブロックを使うか決める
	if (rem == 0) {
		//点対称にするために真ん中以降のブロックは点対称になっているブロックで取得した乱数を代入する
		if (knt < 5) {
			rnd = GetRand(1);
		}
		else {
			rnd = gamemain->block[((BLOCK_ONE_MAX - 1) - knt) * BLOCK_ONE_MAX]->rnd;
		}
	}
	else {
		//ブロックが左上じゃない場合は一個前のブロックで取得した乱数を代入
		rnd = gamemain->block[num - 1]->rnd;
	}

	//取得した乱数を元に２種類あるブロックを選別
	switch (rnd)
	{
	case 0:
		if (gamemode == 2) {
			if (BlockPosition[knt][rem] == 0) {
				isAlive = false;
			}
		}
		else if (gamemode == 1) {
			if (BlockPosition_Casual[knt][rem] == 0) {
				isAlive = false;
			}
		}
		break;
	case 1:
		if (gamemode == 2) {
			if (BlockPosition2[knt][rem] == 0) {
				isAlive = false;
			}
		}
		else if (gamemode == 1) {
			if (BlockPosition_Casual[knt][rem] == 0) {
				isAlive = false;
			}
		}
		break;
	}

	if (gamemode == 1) {
		if (num > 44) {
			isAlive = false;
		}
	}

	this->num = num;
}

// ブロックを描画する関数
void Block::DrawBlocks(void) {
	if (!isAlive) return;		// ブロックが死んでいたら処理を抜ける
	//グリッド線描画
	for (int i = 0; i < GameMain::SCREEN_WIDTH / BLOCK_SIZE; i++) {
		DrawLine(BLOCK_SIZE * i, 0, BLOCK_SIZE * i, GameMain::SCREEN_HEIGHT, 0xffffff);
		DrawLine(0, BLOCK_SIZE * i, GameMain::SCREEN_WIDTH, BLOCK_SIZE * i, 0xffffff);
	}
	int dx1, dx2, dy1, dy2;		// 描画用のローカル変数
	dx1 = x - size / 2;
	dx2 = x + size / 2;
	dy1 = y - size / 2;
	dy2 = y + size / 2;
	if (HP == 3) {
		DrawBox(dx1, dy1, dx2, dy2, 0x7A6611, 1);
	}
	else if (HP == 2) {
		DrawBox(dx1, dy1, dx2, dy2, 0xFFFF00, 1);
	}
	else if (HP == 1) {
		DrawBox(dx1, dy1, dx2, dy2, 0xFF0000, 1);
	}
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