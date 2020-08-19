#include "Title.h"

Title::Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG) {
	fontData = font;
	inputManager = inputMNG;
	gameManager = gameMNG;
	gameMain = gameManager->gameMain;
	titleBullet = new TitleBullet(fontData);

	// カーソル位置初期化
	selectNum[0] = 0;
	selectNum[1] = 0;
	LoadImages();

	backAnimWaitTime = 240;
}

void Title::TitleControll() {
	if (isOpenHelpScreen) {
		HelpScreenControll();
		return;
	}

	if (!titleBullet->GetIsAlive()) {
		if (300 <= ++backAnimWaitTime) {
			titleBullet->BulletInit(TRUE, GetRand(1), GetRand(1));
			backAnimWaitTime = 0;
		}
	}
	else {
		titleBullet->TitleBulletControll();
	}

	for (int i = 0; i < 2; i++) {
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
				gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
				gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
				this->~Title();
				break;
			case 1:
				gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
				gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
				gameMain->SetNetBallteFlg(TRUE);
				this->~Title();
				break;
			case 2:
				gameManager->SetPhaseStatus(GameManager::OPTION);
				gameMain->CreateOptionObj(i, Option::TITLE);
				this->~Title();
				break;
			case 3:
				isOpenHelpScreen = true;
				break;
			case 4:
				gameManager->SetPhaseStatus(GameManager::END);
				gameMain->CreateEndObj();
				this->~Title();
				break;
			}
			gameMain->PlayDicideSE();
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

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN) == 1) {
		// ゲームパッド1のBボタン入力。
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
			gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
			this->~Title();
			break;
		case 1:
			gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
			gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
			gameMain->SetNetBallteFlg(TRUE);
			this->~Title();
			break;
		case 2:
			gameManager->SetPhaseStatus(GameManager::OPTION);
			gameMain->CreateOptionObj(GameManager::BLUE + 1, Option::TITLE);
			this->~Title();
			break;
		case 3:
			isOpenHelpScreen = true;
			break;
		case 4:
			gameManager->SetPhaseStatus(GameManager::END);
			gameMain->CreateEndObj();
			this->~Title();
			break;
		}
		gameMain->PlayDicideSE();
	}
}

void Title::HelpScreenControll() {
	for (int i = 0; i < 2; i++) {
		if (inputManager->GetButtonDown(B, i) ||
			inputManager->GetButtonDown(A, i)) {

			// パッドAボタンかBボタンが押されていたらヘルプ画面終了
			isOpenHelpScreen = false;
			gameMain->PlayCanselSE();
			return;
		}
	}

	if (inputManager->GetKeyDown(KEY_INPUT_ESCAPE) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// キーボードESCかRETURNが押されていたらヘルプ画面終了
		isOpenHelpScreen = false;
		gameMain->PlayCanselSE();
		return;
	}
}

void Title::DrawHelpScreen() {
	DrawGraph(0, 0, i_helpImage, TRUE);
}

void Title::DrawTitle() {
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// 背景黒色で塗りつぶし

	// ヘルプを開いていたらヘルプ画像表示
	if (isOpenHelpScreen) {
		DrawHelpScreen();
		return;
	}

	DrawGraph(0, 0, i_BGImage, TRUE);	// 背景画像

	titleBullet->DrawTitleBullet();

	// ビルドした日
	DrawFormatStringToHandle(0, 0, 0xFFFFFF, fontData->f_FontData[0], "ビルドした日：2020/08/14");
	
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	DrawRotaGraph(x, starty - 200, 1, 0, i_LogoImage, TRUE);	// タイトルロゴ

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		DrawRotaGraph(x, starty + y * i, 1, 0, i_MenuImage[i], TRUE);	// 各項目画像
	}

	for (int i = 0; i < 2; i++) {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 3 + (GameMain::SCREEN_WIDTH / 3 * i), starty + y * selectNum[i], 1.0f, 0, gameMain->GetCursorImage(i), TRUE);
	}
	
}

void Title::LoadImages() {
	if (!(i_helpImage = LoadGraph("Image/Help.png"))) return;
	if (!(i_LogoImage = LoadGraph("Image/TitleLogo.png"))) return;
	if (!(i_BGImage = LoadGraph("Image/BackImage.png"))) return;
	if (!(i_MenuImage[0] = LoadGraph("Image/StartButton.png"))) return;
	if (!(i_MenuImage[1] = LoadGraph("Image/OnlineButton.png"))) return;
	if (!(i_MenuImage[2] = LoadGraph("Image/OptionButton.png"))) return;
	if (!(i_MenuImage[3] = LoadGraph("Image/HelpButton.png"))) return;
	if (!(i_MenuImage[4] = LoadGraph("Image/EndButton.png"))) return;
}

Title::~Title() {
	i_helpImage = DeleteGraph(i_helpImage);
	i_LogoImage = DeleteGraph(i_LogoImage);
	i_BGImage = DeleteGraph(i_BGImage);
	i_MenuImage[0] = DeleteGraph(i_MenuImage[0]);
	i_MenuImage[1] = DeleteGraph(i_MenuImage[1]);
	i_MenuImage[2] = DeleteGraph(i_MenuImage[2]);
	i_MenuImage[3] = DeleteGraph(i_MenuImage[3]);
	i_MenuImage[4] = DeleteGraph(i_MenuImage[4]);
	gameMain->title = NULL;
}