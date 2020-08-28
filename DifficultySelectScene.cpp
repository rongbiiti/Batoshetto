#include "DifficultySelectScene.h"

////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
DifficultySelectScene::DifficultySelectScene(InputManager* input, FontData* font, GameManager* gameMNG) {
	inputManager = input;
	fontData = font;
	gameMangaer = gameMNG;

	// 変数初期化
	waitTime = 0;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

	LoadImages();
}

////////////////////////////////////////////////
// 難易度選択画面の処理
////////////////////////////////////////////////
void DifficultySelectScene::DifficultySelectControll() {
	for (int i = 0; i < 2; i++) {
		
		// Aボタンを押した時、すでに難易度を決定していたら解除する
		if (dicideNumFlg[i]) {
			if (inputManager->GetButtonDown(A, i)) {
				dicideNumFlg[i] = false;
				gameMangaer->gameMain->PlayCanselSE();
			}
			continue;
		}

		// 難易度未決定の場合は、タイトル画面へ戻る
		if (inputManager->GetButtonDown(A, i)) {
			returnFlg = true;		// このフラグは、gameMainが参照している。trueなら、難易度選択画面クラスがdeleteされる。
			gameMangaer->gameMain->CreateTitleObj();		// タイトル画面を作り、Phaseもタイトルにする
			gameMangaer->gameMain->PlayCanselSE();
		}

		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
			// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameMangaer->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
			// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameMangaer->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// ゲームパッド1のBボタン入力。
			dicideNumFlg[i] = true;
			gameMangaer->gameMain->PlayDicideSE();
		}
	}

	// キーボードのESCを押した時、難易度を決定していなかったらタイトル画面へ戻る
	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
		returnFlg = true;
		gameMangaer->gameMain->CreateTitleObj();
		gameMangaer->gameMain->PlayCanselSE();
	}

	// キーボードのESCを押した時、難易度を決定していたら、その決定をキャンセルする
	if (dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
		dicideNumFlg[GameManager::BLUE] = false;
		gameMangaer->gameMain->PlayCanselSE();
	}


	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4))) {
		// キーボード方向キー上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4))) {
		// キーボード方向キー下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN))) {
		// キーボード決定入力
		dicideNumFlg[GameManager::BLUE] = true;
		gameMangaer->gameMain->PlayDicideSE();
		return;
	}

	// キーボードを押し続けていた場合、REDとBLUEを強制的に同じ項目を選択したことにして、処理を進める
	if (!(dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) && inputManager->In_Key()[KEY_INPUT_RETURN] >= 30) {
		inputManager->In_Key()[KEY_INPUT_RETURN] = 0;
		selectNum[GameManager::RED] = selectNum[GameManager::BLUE];
		dicideNumFlg[GameManager::RED] = true;
		gameMangaer->gameMain->PlayDicideSE();
	}

	// どちらも項目を決定していたら、シーン遷移をする
	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {

		// もしもお互いの項目番号が違ったら、選び直させる。
		if (selectNum[GameManager::RED] != selectNum[GameManager::BLUE]) {
			for (int i = 0; i < 2; i++) {
				selectNum[i] = 0;
				dicideNumFlg[i] = false;
			}
			gameMangaer->gameMain->PlayCanselSE();
			return;
		}
		else {

			// 少し待ってから遷移する
			if (!(SCENE_TRANSITION_WAITING_TIME < ++waitTime))  return;
			waitTime = 0;
			SetDifficulty();
			gameMangaer->gameMain->Init();
			returnFlg = true;
		}
	}
}

////////////////////////////////////////////////
// 難易度選択画面の処理・ネット対戦用
////////////////////////////////////////////////
void DifficultySelectScene::DifficultySelectControll_Net() {

	if (!dicideNumFlg[GameManager::RED] && inputManager->GetButtonDown(A, GameManager::RED)) {
		// ゲームパッド1のBボタン入力。
		returnFlg = true;
		gameMangaer->gameMain->CreateTitleObj();
		gameMangaer->gameMain->PlayCanselSE();
		return;
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetButtonDown(PAD_UP, GameManager::RED) || inputManager->GetButtonHold(PAD_UP, GameManager::RED, 4))) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetButtonDown(PAD_DOWN, GameManager::RED) || inputManager->GetButtonHold(PAD_DOWN, GameManager::RED, 4))) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && inputManager->GetButtonDown(B, GameManager::RED)) {
		// ゲームパッド1のBボタン入力。
		dicideNumFlg[GameManager::RED] = true;
		gameMangaer->gameMain->PlayDicideSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_ESCAPE))) {
		// キーボードエスケープキー入力
		returnFlg = true;
		gameMangaer->gameMain->CreateTitleObj();
		gameMangaer->gameMain->PlayCanselSE();
		return;
	}

	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4))) {
		// キーボード方向キー上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4))) {
		// キーボード方向キー下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN))) {
		// キーボード決定入力
		dicideNumFlg[GameManager::RED] = true;
		gameMangaer->gameMain->PlayDicideSE();
		return;
	}

	// どちらも項目を決定していたら、シーン遷移をする
	if (dicideNumFlg[GameManager::RED]) {
		// 少し待ってから遷移する
		if (!(SCENE_TRANSITION_WAITING_TIME < ++waitTime))  return;
		waitTime = 0;
		switch (selectNum[GameManager::RED])
		{
		case 0:
			gameMangaer->SetDifficulty(GameManager::CASUAL);
			break;
		case 1:
			gameMangaer->SetDifficulty(GameManager::EXPERT);
			break;
		}

		// ネット対戦用なので、難易度決定後、IPアドレス選択画面へ遷移する
		gameMangaer->SetPhaseStatus(GameManager::IPADDRESS_SELECT);
		gameMangaer->gameMain->network->VariableInit();	// ネットワークの変数初期化
		returnFlg = true;
	}
}

////////////////////////////////////////////////
// 難易度選択画面の描画処理
////////////////////////////////////////////////
void DifficultySelectScene::DrawDifficultySelectScene() {
	// 画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 60 * (60-waitTime));
	}

	DrawGraph(0, 0, i_BackImage, TRUE);		// 背景画像描画

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 60 * (60 - waitTime));

	DrawRotaGraph(x, starty - 300, 1, 0, i_MenuImage[0], 1);	// DIFFICULTYSELECTの文字描画
	DrawRotaGraph(x, starty - 200, 1, 0, i_MenuImage[1], 1);	// 難易度を選んでくださいの文字描画

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
			DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 1.0f, 0, gameMangaer->gameMain->GetCursorImage(i), TRUE);
		}

	}

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		DrawRotaGraph(x, starty + y * i, 1, 0, i_MenuImage[i + 3], 1);
	}

	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}

////////////////////////////////////////////////
// 難易度選択画面の描画処理・ネット用
////////////////////////////////////////////////
void DifficultySelectScene::DrawDifficultySelectScene_Net() {
	// 画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	DrawGraph(0, 0, i_BackImage, TRUE);		// 背景画像描画

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawRotaGraph(x, starty - 300, 1, 0, i_MenuImage[0], 1);	// DIFFICULTYSELECTの文字描画
	DrawRotaGraph(x, starty - 200, 1, 0, i_MenuImage[1], 1);	// 難易度を選んでくださいの文字描画
	DrawRotaGraph(x, starty - 135, 1, 0, i_MenuImage[2], 1);	// ネット対戦のみ。

	// 項目を決定していたら、長い四角を表示する
	if (dicideNumFlg[GameManager::RED]) {
		DrawBox(0, starty + y * selectNum[GameManager::RED] - 15, GameMain::SCREEN_WIDTH, starty + y * selectNum[GameManager::RED] + 15, COLOR_VALUE_PLAYER[GameManager::RED], 1);
	}
	else {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * 0), starty + y * selectNum[0], 1.0f, 0, gameMangaer->gameMain->GetCursorImage(0), TRUE);
	}

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		DrawRotaGraph(x, starty + y * i, 1, 0, i_MenuImage[i + 3], 1);
	}
}
////////////////////////////////////////////////
// 難易度をGameManagerの変数にセットしてシーンを遷移
////////////////////////////////////////////////
void DifficultySelectScene::SetDifficulty() {

	switch (selectNum[GameManager::BLUE])
	{
	case 0:
		gameMangaer->SetDifficulty(GameManager::CASUAL);
		break;
	case 1:
		gameMangaer->SetDifficulty(GameManager::EXPERT);
		break;
	}
	
}

////////////////////////////////////////////////
// 画像読み込み
////////////////////////////////////////////////
void DifficultySelectScene::LoadImages() {
	if (!(i_BackImage = LoadGraph("Image/SelectBackImage.png"))) return;
	if (!(i_MenuImage[0] = LoadGraph("Image/ModeSelect.png"))) return;
	if (!(i_MenuImage[1] = LoadGraph("Image/ModeSelect_jp.png"))) return;
	if (!(i_MenuImage[2] = LoadGraph("Image/SameMode.png"))) return;
	if (!(i_MenuImage[3] = LoadGraph("Image/Casual.png"))) return;
	if (!(i_MenuImage[4] = LoadGraph("Image/Expert.png"))) return;
}

////////////////////////////////////////////////
// 画像データメモリから消去
////////////////////////////////////////////////
void DifficultySelectScene::DeleteImages() {
	i_BackImage = DeleteGraph(i_BackImage);
	i_MenuImage[0] = DeleteGraph(i_MenuImage[0]);
	i_MenuImage[1] = DeleteGraph(i_MenuImage[1]);
	i_MenuImage[2] = DeleteGraph(i_MenuImage[2]);
	i_MenuImage[3] = DeleteGraph(i_MenuImage[3]);
	i_MenuImage[4] = DeleteGraph(i_MenuImage[4]);
}

////////////////////////////////////////////////
// デストラクタ
////////////////////////////////////////////////
DifficultySelectScene::~DifficultySelectScene() {
	DeleteImages();
}