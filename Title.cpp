#include "Title.h"

Title::Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG) {
	fontData = font;
	inputManager = inputMNG;
	gameManager = gameMNG;

	// カーソル位置初期化
	selectNum[0] = 0;
	selectNum[1] = 0;
}

void Title::TitleControll() {
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
				gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
			
				gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
				
				this->~Title();
				break;
			case 1:
				break;
			case 2:
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
			gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);

			gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);

			this->~Title();
			break;
		case 1:
			break;
		case 2:
			break;
		}
	}
}

void Title::DrawTitle() {
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// 背景黒色で塗りつぶし
	
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "バトシェット");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "バトシェット");

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xeeff14, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	for (int i = 0; i < 2; i++) {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 10, COLOR_VALUE_PLAYER[i], 1, 1);
	}
	/*if (inputManager->GetPadInput()[GameManager::RED].in_Button[InputManager::B] == 1 ||
		inputManager->GetPadInput()[GameManager::BLUE].in_Button[InputManager::B] == 1 ||
		inputManager->In_Key()[KEY_INPUT_F] == 1) {
		gameManager->SetPhaseStatus(GameManager::INIT);
		this->~Title();
	}*/
}

Title::~Title() {

}