#include "Block.h"
#include "GameMain.h"
#pragma warning(disable : 4996)

void Block::ReadFile(void){
	int c;
	bool flg = FALSE;
	char buf[100] = {0};

	//ファイルを読み取り専用で開く（エキスパート用）
	fp = fopen("Block.csv","r");

	//ファイルオープンに失敗したらエラーメッセージを出す
	if (fp == NULL) {
		printfDx("read error\n");
		return;
	}

	//ブロック情報をファイルから読み取り、配列に書き込む（エキスパート用）
	for (int i = 0; i < BLOCK_ONE_MAX * 2; i++) {
		for (int j = 0; j < BLOCK_ONE_MAX; j++)
		{
			c = fgetc(fp);
			//','か'\n'（改行）ならbufに文字連結して添え字のjを一回戻す
			if (c != ',' && c != '\n') {
				strcat(buf, (const char*)&c);
				j--;
			}
			else
			{
				//','か'\n'（改行）じゃない場合はそれぞれ対応した配列にatoi関数で代入
				if (!flg) {
					BlockPosition[i][j] = atoi(buf);
				}
				else if(i >= BLOCK_ONE_MAX)
				{
					flg = TRUE;
					BlockPosition2[i / BLOCK_ONE_MAX][j] = atoi(buf);
				}
				//文字連結したbufを初期化
				memset(buf, 0, sizeof(buf));
			}
		}
	}
	flg = FALSE;
	//ファイルを閉じる
	fclose(fp);

	//ファイルを読み取り専用で開く（カジュアル用）
	fp = fopen("Block_Casual.csv", "r");

	//ファイルオープンに失敗したらエラーメッセージを出す
	if (fp == NULL) {
		printfDx("read error\n");
		return;
	}

	//ブロック情報をファイルから読み取り、配列に書き込む（カジュアル用）
	for (int i = 0; i < BLOCK_Casual_MAX * 2; i++) {
		for (int j = 0; j < BLOCK_ONE_MAX; j++)
		{
			c = fgetc(fp);
			//','か'\n'（改行）ならbufに文字連結して添え字のjを一回戻す
			if (c != ',' && c != '\n') {
				strcat(buf, (const char*)&c);
				j--;
			}
			else
			{
				//','か'\n'（改行）じゃない場合はそれぞれ対応した配列にatoi関数で代入
				if (!flg) {
					BlockPosition_Casual[i][j] = atoi(buf);
				}
				else if (i >= BLOCK_Casual_MAX)
				{
					flg = TRUE;
					BlockPosition_Casual2[i / BLOCK_Casual_MAX][j] = atoi(buf);
				}
				//文字連結したbufを初期化
				memset(buf, 0, sizeof(buf));
			}
		}
	}
	flg = FALSE;
	//ファイルを閉じる
	fclose(fp);

}

// コンストラクタ。ブロックの番号と、フォント管理オブジェクトのポインタを入れる。
Block::Block(int num, FontData* font ,GameMain* main) {

	int knt,rem,gamemode;
	ReadFile();	//ファイルからブロック情報を読み取る
	isAlive = true;			// 生存フラグオン
	gamemain = main;		// ゲームメインのポインタ
	size = BLOCK_SIZE;		// サイズ
	HP = BlockStartHP;		// HP初期化
	fontData = font;		// フォントデータのポインタ
	gamemode = main->gameManager->GetDifficulty();	// 難易度

	breakAnimWaitTime = 0;
	breakAngle = 0;

	knt =  num / BLOCK_ONE_MAX;		// ブロックの初期位置を商で求める
	rem =  num % BLOCK_ONE_MAX;		// ３×３ブロックの何番目かを余りで求める

	// ３×３ブロックの番号を元に座標を指定
	if (rem == 0) {
		switch (gamemode)
		{
		//ゲームモードがカジュアル
		//左上のブロックは初期座標を代入
		case 1:
			x = BlockStartPosition2[knt][0];
			y = BlockStartPosition2[knt][1];
			break;
		//ゲームモードがエキスパート
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
		//エキスパート
		if (gamemode == 2) {
			if (knt < 5) {
				rnd = GetRand(1);
			}
			else {
				rnd = gamemain->block[((BLOCK_ONE_MAX - 1) - knt) * BLOCK_ONE_MAX]->rnd;
			}
		//カジュアル
		}else if (gamemode == 1 && num <= 44) {
			if (knt < 3) {
				rnd = GetRand(1);
			}
			else {
				rnd = gamemain->block[((BLOCK_ONE_MAX - 5) - knt) * BLOCK_ONE_MAX]->rnd;
			}
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
				breakAnimWaitTime = 61;
			}
		}
		else if (gamemode == 1) {
			if (BlockPosition_Casual[knt][rem] == 0) {
				isAlive = false;
				breakAnimWaitTime = 61;
			}
		}
		break;
	case 1:
		if (gamemode == 2) {
			if (BlockPosition2[knt][rem] == 0) {
				isAlive = false;
				breakAnimWaitTime = 61;
			}
		}
		else if (gamemode == 1) {
			if (BlockPosition_Casual2[knt][rem] == 0) {
				isAlive = false;
				breakAnimWaitTime = 61;
			}
		}
		break;
	}

	//ゲームモードにそって最大ブロック数を超えている配列のブロックは消す
	if (gamemode == 1) {
		if (num > 44) {
			isAlive = false;
			breakAnimWaitTime = 61;
		}
	}

	this->num = num;
}

// ブロックを描画する関数
void Block::DrawBlocks(void) {
	if (isAlive) {
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
		DrawRotaGraph(x, y, 1, 0, gamemain->GetBlockImage(HP - 1), true);
	}
	else {
		if (60 < breakAnimWaitTime) return;	// アニメ終わってたら描画しない
		++breakAnimWaitTime;

		int dx1, dx2, dy1, dy2;		// 描画用のローカル変数
		dx1 = x - size / 4;
		dx2 = x + size / 4;
		dy1 = y - size / 4;
		dy2 = y + size / 4;

		if (breakAnimWaitTime % 10 == 0) {
			breakAngle += (90.0f * 3.14f) / 180.0f;
		}

		DrawRotaGraph(dx1 - (breakAnimWaitTime / 1.5), dy1 - (breakAnimWaitTime / 2), 0.8f, breakAngle, gamemain->GetBreakBlockImage(0), true);	// 左上
		DrawRotaGraph(dx2 + (breakAnimWaitTime / 1.5), dy1 - (breakAnimWaitTime / 2), 0.8f, breakAngle, gamemain->GetBreakBlockImage(1), true);		// 右上
		DrawRotaGraph(dx1 - (breakAnimWaitTime / 1.5), dy2 + (breakAnimWaitTime / 2), 0.8f, breakAngle, gamemain->GetBreakBlockImage(2), true);		// 左下
		DrawRotaGraph(dx2 + (breakAnimWaitTime / 1.5), dy2 + (breakAnimWaitTime / 2), 0.8f, breakAngle, gamemain->GetBreakBlockImage(3), true);		// 右下
	}
	
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