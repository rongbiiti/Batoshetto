#include "Result.h"

// フォントデータ管理オブジェクトのポインタ、入力管理オブジェクトのポインタ、
// ゲーム進行管理オブジェクトのポインタ、ヒットしたプレイヤーがREDかBLUEかを受け取る。
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	this->hitPlayerNumber = hitplayernum;
	waitTime = 0;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}
}

// 通信対戦で、タイムアウトが起きたとき
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	waitTime = 0;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}
}

void Result::ResultControll(void) {
	for (int i = 0; i < 2; i++) {
		
		if (dicideNumFlg[i]) continue;

		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
			// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
			// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
		}

		if (inputManager->GetButtonDown(B, i)) {
			// ゲームパッド1のBボタン入力。
			dicideNumFlg[i] = true;
			
		}
	}


	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		if (inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
			inputManager->In_Key()[KEY_INPUT_UP] -= 4;
		}
	}

	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		if (inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
			inputManager->In_Key()[KEY_INPUT_DOWN] -= 4;
		}
	}

	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// ゲームパッド1のBボタン入力。
		dicideNumFlg[GameManager::BLUE] = true;
		return;
	}

	if (inputManager->In_Key()[KEY_INPUT_RETURN] >= 30) {
		inputManager->In_Key()[KEY_INPUT_DOWN] = 0;
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			Return_to_Game();
			break;
		case 1:
			Return_to_Title();
			break;
		}
		return;
	}

	// どちらも項目を決定していたら、シーン遷移をする
	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {

		// もしもお互いの項目番号が違ったら、選び直させる。
		if (selectNum[GameManager::RED] != selectNum[GameManager::BLUE]) {
			for (int i = 0; i < 2; i++) {
				selectNum[i] = 0;
				dicideNumFlg[i] = false;
			}
			return;
		}
		else {

			// 少し待ってから遷移する
			if (!(SCENE_TRANSITION_WAITING_TIME < ++waitTime))  return;
			waitTime = 0;
			switch (selectNum[GameManager::RED])
			{
			case 0:
				Return_to_Game();
				break;
			case 1:
				Return_to_Title();
				break;
			}
		}
	}

}

// 描画用
void Result::DrawResult() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// Pauseの文字描画
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "GAME SET !!");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "GAME SET !!");

	// 引数で受け取ったヒットしたプレイヤーの番号が、GameManagerの現在の撃つ側と同じか否かを判定する。
	if (hitPlayerNumber == gameManager->GetNowShooter()) {
		// 同じだった場合、自滅だったことを表示する。
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%sの自滅！ｗ", PlayerName[gameManager->GetNowShooter()]);
		DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "%sの自滅！ｗ", PlayerName[gameManager->GetNowShooter()]);
	}
	else {
		// 違った場合、撃つ側の勝利と表示する
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%sの勝ち！", PlayerName[gameManager->GetNowShooter()]);
		DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "%sの勝ち！", PlayerName[gameManager->GetNowShooter()]);
	}

	for (int i = 0; i < 2; i++) {

		// 項目を決定していたら、長い四角を表示する
		if (dicideNumFlg[i]) {
			if (i == GameManager::RED) {
				DrawBox(0, starty + y * selectNum[i] - 15, GameMain::SCREEN_WIDTH / 2, starty + y * selectNum[i] + 15, COLOR_VALUE_PLAYER[i], 1);
			}
			else {
				DrawBox(GameMain::SCREEN_WIDTH, starty + y * selectNum[i] - 15, GameMain::SCREEN_WIDTH / 2, starty + y * selectNum[i] + 15, COLOR_VALUE_PLAYER[i], 1);
			}
		}
		else {
			// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
			DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 10, COLOR_VALUE_PLAYER[i], 1, 1);
		}

	}

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

// 描画用
void Result::DrawTimeOut() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// Pauseの文字描画
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "相手からの応答が10秒間ありませんでした");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "相手からの応答が10秒間ありませんでした");

	// 項目を決定していたら、長い四角を表示する
	if (dicideNumFlg[0]) {
		if (0 == GameManager::RED) {
			DrawBox(0, starty + y * selectNum[0] - 15, GameMain::SCREEN_WIDTH / 2, starty + y * selectNum[0] + 15, COLOR_VALUE_PLAYER[0], 1);
		}
		else {
			DrawBox(GameMain::SCREEN_WIDTH, starty + y * selectNum[0] - 15, GameMain::SCREEN_WIDTH / 2, starty + y * selectNum[0] + 15, COLOR_VALUE_PLAYER[0], 1);
		}
	}
	else {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * 0), starty + y * selectNum[0], 10, COLOR_VALUE_PLAYER[0], 1, 1);
	}

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

// 画面を抜けて難易度選択画面へ
void Result::Return_to_Game() {
	gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
	gameManager->gameMain->CreateDifficultySelectSceneObj();
	gameManager->gameMain->MainObjDelete();
	this->~Result();
}

// 試合を中断してタイトル画面へ戻る
void Result::Return_to_Title() {
	gameManager->SetPhaseStatus(GameManager::TITLE);
	gameManager->gameMain->CreateTitleObj();
	gameManager->gameMain->MainObjDelete();
	this->~Result();
}

Result::~Result() {
}