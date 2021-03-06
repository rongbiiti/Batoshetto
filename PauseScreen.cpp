#include "PauseScreen.h"

////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
PauseScreen::PauseScreen(FontData* font, InputManager* input, GameMain* main, int pushPLnum) {
	// 引数で受け取ったポインタをローカル変数にコピー
	fontData = font;
	inputManager = input;
	gameMain = main;
	pausePushPLNum = pushPLnum;

	// カーソル位置初期化
	selectNum[0] = 0;
	selectNum[1] = 0;
}

////////////////////////////////////////////////
// ポーズ画面処理
////////////////////////////////////////////////
void PauseScreen::PauseScreenControll() {
	for (int i = 0; i < 2; i++) {
		if (pausePushPLNum == GameManager::BLUE + 1) break;	// ポーズボタンを押したのがキーボードのキーからなら、コントローラーの操作を受け付けない。
		if (pausePushPLNum != i) continue;					// ポーズボタンを押したプレイヤーでない場合、処理をスキップ
		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
			// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
			// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// ゲームパッド1のBボタン入力。
			switch (selectNum[i])
			{
			case 0:
				Return_to_Game();
				break;
			case 1:
				OpenOptionScreen();
				break;
			case 2:
				Return_to_Title();
				break;
			}
			gameMain->PlayDicideSE();
			return;
		}
	}


	if (pausePushPLNum != GameManager::BLUE + 1) return;	// ポーズボタンを押したのがコントローラーだったら、キーボードの処理を受け付けないでreturn


	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// ゲームパッド1のBボタン入力。
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			Return_to_Game();
			break;
		case 1:
			OpenOptionScreen();
			break;
		case 2:
			Return_to_Title();
			break;
		}
		gameMain->PlayDicideSE();
	}
}

////////////////////////////////////////////////
// ポーズ画面描画
////////////////////////////////////////////////
void PauseScreen::DrawPauseScreen() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// 背景黒色で塗りつぶし
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 文字の幅、		画面の横中心、　　　　　　　	Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	// Pauseの文字描画
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Pause");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "Pause");

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	// ポーズ画面押したのがコントローラーなら赤か青を、キーボードなら必ず青を表示させる
	if (pausePushPLNum <= GameManager::BLUE) {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * pausePushPLNum), starty + y * selectNum[pausePushPLNum], 1, 0, gameMain->GetCursorImage(pausePushPLNum), TRUE);
	}
	else if (pausePushPLNum == GameManager::BLUE + 1) {
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * GameManager::BLUE), starty + y * selectNum[GameManager::BLUE], 1, 0, gameMain->GetCursorImage(GameManager::BLUE), TRUE);
	}
}

////////////////////////////////////////////////
// ポーズ画面を抜けて試合を再開する
////////////////////////////////////////////////
void PauseScreen::Return_to_Game() {
	gameMain->SetPauseFlg(false);
}

////////////////////////////////////////////////
// オプション画面を開く
////////////////////////////////////////////////
void PauseScreen::OpenOptionScreen() {
	gameMain->SetPauseFlg(false);
	gameMain->CreateOptionObj(pausePushPLNum, Option::PAUSE);
	gameMain->gameManager->SetPhaseStatus(GameManager::OPTION);
}

////////////////////////////////////////////////
// 試合を中断してタイトル画面へ戻る
////////////////////////////////////////////////
void PauseScreen::Return_to_Title() {
	gameMain->SetPauseFlg(false);
	gameMain->gameManager->SetPhaseStatus(GameManager::TITLE);
	gameMain->MainObjDelete();
	gameMain->PlayBattleBGM(TRUE);
	gameMain->CreateTitleObj();
}

////////////////////////////////////////////////
// デストラクタ
////////////////////////////////////////////////
PauseScreen::~PauseScreen() {

}