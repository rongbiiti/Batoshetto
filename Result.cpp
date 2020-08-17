#include "Result.h"

////////////////////////////////////////////////
// フォントデータ管理オブジェクトのポインタ、入力管理オブジェクトのポインタ、
// ゲーム進行管理オブジェクトのポインタ、ヒットしたプレイヤーがREDかBLUEかを受け取る。
////////////////////////////////////////////////
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	this->hitPlayerNumber = hitplayernum;
	waitTime = 0;
	timeOutFlg = FALSE;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

	LoadImages();
}

////////////////////////////////////////////////
// 通信対戦で、タイムアウトが起きたとき
////////////////////////////////////////////////
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	waitTime = 0;
	timeOutFlg = TRUE;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

	LoadImages();
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
			dicideNumFlg[i] = true;
			gameManager->gameMain->PlayDicideSE();
		}
	}


	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
		
	}

	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// ゲームパッド1のBボタン入力。
		dicideNumFlg[GameManager::BLUE] = true;
		gameManager->gameMain->PlayDicideSE();
		return;
	}

	// キーボードを押し続けていた場合、REDとBLUEを強制的に同じ項目を選択したことにして、処理を進める
	if (inputManager->In_Key()[KEY_INPUT_RETURN] >= 30) {
		inputManager->In_Key()[KEY_INPUT_RETURN] = 0;
		selectNum[GameManager::RED] = selectNum[GameManager::BLUE];
		dicideNumFlg[GameManager::RED] = true;
		gameManager->gameMain->PlayDicideSE();
	}

	// どちらも項目を決定していたら、シーン遷移をする
	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {

		// もしもお互いの項目番号が違ったら、選び直させる。
		if (selectNum[GameManager::RED] != selectNum[GameManager::BLUE]) {
			for (int i = 0; i < 2; i++) {
				selectNum[i] = 0;
				dicideNumFlg[i] = false;
				gameManager->gameMain->PlayCanselSE();
			}
			return;
		}
		else {
			// シーン遷移
			SceneTransition();
		}
	}

}

////////////////////////////////////////////////
// リザルト画面操作：ネット用
////////////////////////////////////////////////
void Result::ResultControll_Net(void) {

	// どちらも項目を決定していたら、シーン遷移をする
	if (dicideNumFlg[GameManager::RED]) {
		// シーン遷移
		SceneTransition();
	}


	if (dicideNumFlg[GameManager::RED]) return;


	if (inputManager->GetButtonDown(PAD_UP, GameManager::RED) || inputManager->GetButtonHold(PAD_UP, GameManager::RED, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetButtonDown(PAD_DOWN, 0) || inputManager->GetButtonHold(PAD_DOWN, 0, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetButtonDown(B, GameManager::RED)) {
		// ゲームパッド1のBボタン入力。
		dicideNumFlg[GameManager::RED] = true;
		gameManager->gameMain->PlayDicideSE();
	}


	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// ゲームパッド1のBボタン入力。
		dicideNumFlg[GameManager::RED] = true;
		gameManager->gameMain->PlayDicideSE();
		return;
	}
	
}

////////////////////////////////////////////////
// シーン遷移
////////////////////////////////////////////////
void Result::SceneTransition() {
	// 少し待ってから遷移する
	if (!(SCENE_TRANSITION_WAITING_TIME < ++waitTime))  return;
	waitTime = 0;
	switch (selectNum[GameManager::RED])
	{
	case 0:
		if (gameManager->gameMain->GetNetBallteFlg()) {
			ReMatch();
		}
		else {
			Return_to_Game();
		}
		
		break;
	case 1:
		if (gameManager->gameMain->GetNetBallteFlg()) {
			Return_to_Title();
		}
		else {
			Return_to_Title();
		}
		
		break;
	}
}

////////////////////////////////////////////////
// 描画用
////////////////////////////////////////////////
void Result::DrawResult() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	DrawGraph(0, 0, i_ResultImage, TRUE);	// 背景画像描画

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
			DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 1.0f, 0, gameManager->gameMain->GetCursorImage(i), TRUE);
		}

	}

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

////////////////////////////////////////////////
// 描画用：ネット対戦用
////////////////////////////////////////////////
void Result::DrawResult_Net() {
	if (timeOutFlg) {
		DrawTimeOut();
		return;
	}

	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	DrawGraph(0, 0, i_ResultImage, TRUE);	// 背景画像描画

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

	// 項目を決定していたら、長い四角を表示する
	if (dicideNumFlg[0]) {
		DrawBox(0, starty + y * selectNum[0] - 15, GameMain::SCREEN_WIDTH, starty + y * selectNum[0] + 15, COLOR_VALUE_PLAYER[0], 1);
	}
	else {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * 0), starty + y * selectNum[0], 1.0f, 0, gameManager->gameMain->GetCursorImage(0), TRUE);
	}

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

////////////////////////////////////////////////
// 描画用
////////////////////////////////////////////////
void Result::DrawTimeOut() {


	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	DrawGraph(0, 0, i_ResultImage, TRUE);	// 背景画像描画

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
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * 0), starty + y * selectNum[0], 1.0f, 0, gameManager->gameMain->GetCursorImage(0), TRUE);
	}

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

////////////////////////////////////////////////
// 画面を抜けて難易度選択画面へ
////////////////////////////////////////////////
void Result::Return_to_Game() {
	gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
	gameManager->gameMain->CreateDifficultySelectSceneObj();
	gameManager->gameMain->MainObjDelete();
	this->~Result();
}

////////////////////////////////////////////////
// ネット対戦で、同じ条件で相手と再戦する
////////////////////////////////////////////////
void Result::ReMatch() {
	gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);
	gameManager->gameMain->network->VariableInit();
	gameManager->gameMain->network->SetSendRand();
	this->~Result();
}

////////////////////////////////////////////////
// 試合を中断してタイトル画面へ戻る
////////////////////////////////////////////////
void Result::Return_to_Title() {
	gameManager->SetPhaseStatus(GameManager::TITLE);
	gameManager->gameMain->CreateTitleObj();
	gameManager->gameMain->MainObjDelete();
	this->~Result();
}

void Result::LoadImages() {
	if (!(i_ResultImage = LoadGraph("Image/Result.png"))) return;
}

void Result::DeleteImages() {
	i_ResultImage = DeleteGraph(i_ResultImage);
}
Result::~Result() {
	DeleteImages();
}