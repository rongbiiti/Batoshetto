#include "Title.h"

////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
Title::Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG) {
	fontData = font;
	inputManager = inputMNG;
	gameManager = gameMNG;
	gameMain = gameManager->gameMain;
	titleBullet = new TitleBullet(fontData);	// 背景で跳弾してる弾クラス生成

	// カーソル位置初期化
	selectNum[0] = 0;
	selectNum[1] = 0;
	selectNum[2] = 0;

	LoadImages();	// 画像読み込み

	backAnimWaitTime = 240;
}
////////////////////////////////////////////////
// 処理用関数
////////////////////////////////////////////////
void Title::TitleControll() {

	// ヘルプ画面を開いていたら、ヘルプ画面の処理をして早期リターン
	if (isOpenHelpScreen) {
		HelpScreenControll();
		return;
	}

	// 背景の弾の処理。行きていたら関数を呼び出して処理させる
	// 死んでいたら、待ち時間を進める。
	if (!titleBullet->GetIsAlive()) {
		// 待ち時間が一定以上になったら、弾を初期化して、待ち時間をリセットする
		if (300 <= ++backAnimWaitTime) {
			// 初期X座標とY座標のフラグを、乱数で作って渡す
			titleBullet->BulletInit(TRUE, GetRand(1), GetRand(1));
			backAnimWaitTime = 0;
		}
	}
	else {
		titleBullet->TitleBulletControll();
	}



	///////////////////////////////////////////////////////////////////////////////
	////////////////////////////// コントローラー処理 //////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 2; i++) {
		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// ゲームパッドの方向パッド上の入力。18フレ以上押し続けてたら連続で上に移動
			// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// ゲームパッドの方向パッド下の入力。18フレ以上押し続けてたら連続で下に移動
			// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;	
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// ゲームパッドのBボタン入力
			ProcessingOnDetermining(selectNum[i]);
			return;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////// キーボード処理 //////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	// 2プレイヤーのカーソルを操作する。
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// キーボード方向キー上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// キーボード方向キー下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN) == 1) {
		// キーボード決定入力
		ProcessingOnDetermining(GameManager::BLUE + 1);
	}
}

////////////////////////////////////////////////
// 決定ボタン押下時の処理。引数は、コントローラーの番号
////////////////////////////////////////////////
void Title::ProcessingOnDetermining(int controllerNum) {

	int num = controllerNum;
	if (controllerNum == GameManager::BLUE + 1) --num;

	switch (selectNum[num])
	{
	case 0:
		// 通常プレイ。難易度選択画面へする
		gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
		gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
		this->~Title();
		break;

	case 1:
		// ローカル通信。通信対戦フラグを建てる
		gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
		gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
		gameMain->SetNetBallteFlg(TRUE);
		this->~Title();
		break;

	case 2:
		// オプション画面開く
		gameManager->SetPhaseStatus(GameManager::OPTION);
		gameMain->CreateOptionObj(controllerNum, Option::TITLE);
		this->~Title();
		break;

	case 3:
		// ヘルプ画面開く
		isOpenHelpScreen = true;
		break;

	case 4:
		// エンド確認画面開く
		gameManager->SetPhaseStatus(GameManager::END);
		gameMain->CreateEndObj();
		this->~Title();
		break;
	}

	gameMain->PlayDicideSE();	// 決定時音鳴らす
}

////////////////////////////////////////////////
// ヘルプ画面の処理
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// ヘルプ画面の描画
////////////////////////////////////////////////
void Title::DrawHelpScreen() {
	DrawGraph(0, 0, i_helpImage, TRUE);
}

////////////////////////////////////////////////
// タイトル画面描画
////////////////////////////////////////////////
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
	DrawFormatStringToHandle(0, 0, 0xFFFFFF, fontData->f_FontData[0], "ビルドした日：2020/08/25");
	
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	DrawRotaGraph(x, starty - 200, 1, 0, i_LogoImage, TRUE);	// タイトルロゴ

	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xffffff, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	for (int i = 0; i < 2; i++) {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 3 + (GameMain::SCREEN_WIDTH / 3 * i), starty + y * selectNum[i], 1.0f, 0, gameMain->GetCursorImage(i), TRUE);
	}
	
}

////////////////////////////////////////////////
// 画像読み込み
////////////////////////////////////////////////
void Title::LoadImages() {
	if (!(i_helpImage = LoadGraph("Image/Help.png"))) return;
	if (!(i_LogoImage = LoadGraph("Image/TitleLogo.png"))) return;
	if (!(i_BGImage = LoadGraph("Image/TitleBulletBackImage.png"))) return;
	if (!(i_MenuImage[0] = LoadGraph("Image/StartButton.png"))) return;
	if (!(i_MenuImage[1] = LoadGraph("Image/OnlineButton.png"))) return;
	if (!(i_MenuImage[2] = LoadGraph("Image/OptionButton.png"))) return;
	if (!(i_MenuImage[3] = LoadGraph("Image/HelpButton.png"))) return;
	if (!(i_MenuImage[4] = LoadGraph("Image/EndButton.png"))) return;
}

////////////////////////////////////////////////
// デストラクタ
////////////////////////////////////////////////
Title::~Title() {
	delete titleBullet;
	titleBullet = NULL;

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