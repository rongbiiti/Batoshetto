#include "Result.h"

// フォントデータ管理オブジェクトのポインタ、入力管理オブジェクトのポインタ、
// ゲーム進行管理オブジェクトのポインタ、ヒットしたプレイヤーがREDかBLUEかを受け取る。
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	this->hitPlayerNumber = hitplayernum;
}

void Result::ResultControll(void) {
	for (int i = 0; i < 2; i++) {
		if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] == 1 || inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] >= 18) {
			// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
			// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] >= 18) {
				inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] -= 4;
			}
		}

		if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] == 1 || inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] >= 18) {
			// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
			// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] >= 18) {
				inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] -= 4;
			}
		}

		if (inputManager->GetPadInput()[i].in_Button[InputManager::B] == 1) {
			// ゲームパッド1のBボタン入力。
			switch (selectNum[i])
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


	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (inputManager->In_Key()[KEY_INPUT_UP] == 1 || inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		if (inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
			inputManager->In_Key()[KEY_INPUT_UP] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_DOWN] == 1 || inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		if (inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
			inputManager->In_Key()[KEY_INPUT_DOWN] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_F] == 1 || inputManager->In_Key()[KEY_INPUT_RETURN] == 1) {
		// ゲームパッド1のBボタン入力。
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			Return_to_Game();
			break;
		case 1:
			Return_to_Title();
			break;
		}
	}

	// Fを押すと、GameManagerのフェーズを初期化フェーズに変更し、自身のデストラクタを呼ぶ。
	if (inputManager->GetPadInput()[GameManager::RED].in_Button[InputManager::B] == 1 ||
		inputManager->GetPadInput()[GameManager::BLUE].in_Button[InputManager::B] == 1 ||
		inputManager->In_Key()[KEY_INPUT_F] == 1) {
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

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	for (int i = 0; i < 2; i++) {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 10, COLOR_VALUE_PLAYER[i], 1, 1);
	}

	
		
	//fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[0], "B押して再開（キーボードはF）");
	//DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, 560, 0xFFFFFF, fontData->f_FontData[0], "B押して再開（キーボードはF）");
}

// ポーズ画面を抜けて試合を再開する
void Result::Return_to_Game() {
	gameManager->SetPhaseStatus(GameManager::INIT);
	this->~Result();
}

// 試合を中断してタイトル画面へ戻る
void Result::Return_to_Title() {
	gameManager->SetPhaseStatus(GameManager::TITLE);
	this->~Result();
}

Result::~Result() {
}