#include "End.h"

End::End(FontData* font, InputManager* inputMNG, GameManager* gameMNG) {
	fontData = font;
	inputManager = inputMNG;
	gameManager = gameMNG;
	gameMain = gameManager->gameMain;
	LoadImages();

	// カーソル位置初期化
	selectNum[0] = 0;
	selectNum[1] = 0;
}

void End::EndControll() {
	for (int i = 0; i < 2; i++) {
		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
			// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameManager->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
			// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameManager->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// ゲームパッド1のBボタン入力。
			switch (selectNum[i])
			{
			case 0:
				//if (DxLib_Init() == -1) return -1;
				//DxLib_End();
				gameManager->SetPhaseStatus(GameManager::QUIT);
				this->~End();
				break;
			case 1:
				gameManager->SetPhaseStatus(GameManager::TITLE);

				gameMain->CreateTitleObj();

				this->~End();
				break;
			}
			gameManager->gameMain->PlayDicideSE();
			return;
		}
	}

	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN) == 1) {
		// ゲームパッド1のBボタン入力。
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			//if (DxLib_Init() == -1) return -1;
			//DxLib_End();
			gameManager->SetPhaseStatus(GameManager::QUIT);
			this->~End();
			break;
		case 1:
			gameManager->SetPhaseStatus(GameManager::TITLE);

			gameMain->CreateTitleObj();

			this->~End();
			break;
		}
		gameManager->gameMain->PlayDicideSE();
	}
}

void End::DrawEnd() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	//DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// 背景黒色で塗りつぶし
	DrawGraph(0, 0, i_EndImage, TRUE);	// 背景描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 文字の幅、		画面の横中心、　　　　　　　	Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// Pauseの文字描画
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "終了しますか？");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "終了しますか？");

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xeeff14, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	for (int i = 0; i < 2; i++) {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 1.0f, 0, gameManager->gameMain->GetCursorImage(i), TRUE);
	}
}

//画像読み込み
void End::LoadImages() {
	if (!(i_EndImage = LoadGraph("Image/TitleBulletBackImage.png"))) return;
}

void End::DeleteImages() {
	i_EndImage = DeleteGraph(i_EndImage);
}
End::~End() {
	DeleteImages();
}