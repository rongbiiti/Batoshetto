#include "Result.h"

////////////////////////////////////////////////
// フォントデータ管理オブジェクトのポインタ、入力管理オブジェクトのポインタ、
// ゲーム進行管理オブジェクトのポインタ、ヒットしたプレイヤーがREDかBLUEかを受け取る。
////////////////////////////////////////////////
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;

	// 弾に当たったプレイヤーの番号
	hitPlayerNumber = hitplayernum;

	// 変数初期化
	waitTime = 0;
	timeOutFlg = FALSE;
	soundFlg = FALSE;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

	// 画像音読み込み
	LoadImages();
	LoadSounds();
}

////////////////////////////////////////////////
// 通信対戦で、タイムアウトが起きたとき
////////////////////////////////////////////////
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;

	// 変数初期化
	waitTime = 0;
	timeOutFlg = TRUE;
	soundFlg = FALSE;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

	// 画像音読み込み
	LoadImages();
	LoadSounds();
}

////////////////////////////////////////////////
// 通信対戦で、タイムアウトが起きたとき
////////////////////////////////////////////////
void Result::ResultControll(void) {
	if (!soundFlg) {
		// 引数で受け取ったヒットしたプレイヤーの番号が、GameManagerの現在の撃つ側と同じか否かを判定する。
		if (hitPlayerNumber == gameManager->GetNowShooter()) {
			PlaySoundMem(s_LoseSE, DX_PLAYTYPE_BACK);
		}
		else {
			PlaySoundMem(s_WinSE, DX_PLAYTYPE_BACK);
		}
		soundFlg = TRUE;
	}

	for (int i = 0; i < 2; i++) {
		
		if (dicideNumFlg[i]) continue;

		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// ゲームパッドの方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
			// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameManager->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// ゲームパッドの方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
			// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameManager->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// ゲームパッドのBボタン入力。
			dicideNumFlg[i] = true;
			gameManager->gameMain->PlayDicideSE();
		}
	}


	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4))) {
		//  キーボードの方向キー上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
		
	}

	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4))) {
		//  キーボードの方向キー下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN))) {
		//  キーボードの決定ボタン入力。
		dicideNumFlg[GameManager::BLUE] = true;
		gameManager->gameMain->PlayDicideSE();
		return;
	}

	// キーボードを押し続けていた場合、REDとBLUEを強制的に同じ項目を選択したことにして、処理を進める
	if (!(dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) && inputManager->In_Key()[KEY_INPUT_RETURN] >= 30) {
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


	// 決定済みなら以下の処理をしない
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
		// キーボード方向キー上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// キーボード方向キー下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// キーボード決定入力
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
		Return_to_Title();
		
		break;
	}
}

////////////////////////////////////////////////
// 描画用
////////////////////////////////////////////////
void Result::DrawResult() {
	// 画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// 項目決定してたら、ジョジョに透明度を変えていく。タイトルに戻るときのみ。
	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE] && selectNum[GameManager::RED] == 1) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 60 * (60 - waitTime));
	}

	DrawGraph(0, 0, i_ResultImage, TRUE);	// 背景画像描画

	// 引数で受け取ったヒットしたプレイヤーの番号が、GameManagerの現在の撃つ側と同じか否かを判定する。
	if (hitPlayerNumber == gameManager->GetNowShooter()) {
		// 同じだった場合、自滅だったことを表示する。
		DrawRotaGraph(x, starty - 170, 1, 0, i_MenuImage[gameManager->GetNowShooter() + 2], 1);	// 自滅　の画像描画
	}
	else {
		// 違った場合、撃つ側の勝利と表示する
		DrawRotaGraph(x, starty - 170, 1, 0, i_MenuImage[gameManager->GetNowShooter()], 1);	// 勝ち　の画像描画
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
		DrawRotaGraph(x, starty + y * i, 1, 0, i_MenuImage[i + 4], 1);	// 項目描画
	}

	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE] && selectNum[GameManager::RED] == 1) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}

////////////////////////////////////////////////
// 描画用：ネット対戦用
////////////////////////////////////////////////
void Result::DrawResult_Net() {
	
	// 画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// 項目決定してたら、ジョジョに透明度を変えていく。タイトルに戻るときのみ。
	if (dicideNumFlg[GameManager::RED] && selectNum[GameManager::RED] == 1) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 60 * (60 - waitTime));
	}

	DrawGraph(0, 0, i_ResultImage, TRUE);	// 背景画像描画

	// タイムアウトしていたら、タイムアウトしたことを描画
	if (timeOutFlg) {
		// 相手からの応答が10秒間ありませんでしたの文字描画
		DrawRotaGraph(x, starty - 170, 1, 0, i_MenuImage[6], 1);
	}
	// 普通に対戦が終了したら、勝敗結果を描画
	else {
		// 引数で受け取ったヒットしたプレイヤーの番号が、GameManagerの現在の撃つ側と同じか否かを判定する。
		if (hitPlayerNumber == gameManager->GetNowShooter()) {
			// 同じだった場合、自滅だったことを表示する。
			DrawRotaGraph(x, starty - 170, 1, 0, i_MenuImage[gameManager->GetNowShooter() + 2], 1);	// 自滅　の画像描画
		}
		else {
			// 違った場合、撃つ側の勝利と表示する
			DrawRotaGraph(x, starty - 170, 1, 0, i_MenuImage[gameManager->GetNowShooter()], 1);	// 勝ち　の画像描画
		}
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
		DrawRotaGraph(x, starty + y * i, 1, 0, i_MenuImage[i + 4], 1);	// 項目描画
	}

	if (dicideNumFlg[GameManager::RED] && selectNum[GameManager::RED] == 1) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}

////////////////////////////////////////////////
// 画面を抜けて難易度選択画面へ
////////////////////////////////////////////////
void Result::Return_to_Game() {
	gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
	gameManager->gameMain->CreateDifficultySelectSceneObj();
	gameManager->gameMain->MainObjDelete();		// ゲーム中のオブジェクト削除
	this->~Result();
}

////////////////////////////////////////////////
// ネット対戦で、同じ条件で相手と再戦する
////////////////////////////////////////////////
void Result::ReMatch() {
	gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);
	gameManager->gameMain->network->VariableInit();	// ネット用の変数リセット
	gameManager->gameMain->network->SetSendRand();	// 乱数再生成
	this->~Result();
}

////////////////////////////////////////////////
// 試合を中断してタイトル画面へ戻る
////////////////////////////////////////////////
void Result::Return_to_Title() {
	gameManager->SetPhaseStatus(GameManager::TITLE);
	gameManager->gameMain->CreateTitleObj();
	gameManager->gameMain->MainObjDelete();		// ゲーム中のオブジェクト削除
	gameManager->gameMain->PlayBattleBGM(TRUE);	// バトルBGM停止
	this->~Result();
}

////////////////////////////////////////////////
// 画像読み込み
////////////////////////////////////////////////
void Result::LoadImages() {
	if (!(i_ResultImage = LoadGraph("Image/Result.png"))) return;
	if (!(i_MenuImage[0] = LoadGraph("Image/RedWin.png"))) return;
	if (!(i_MenuImage[1] = LoadGraph("Image/BlueWin.png"))) return;
	if (!(i_MenuImage[2] = LoadGraph("Image/RedWarota.png"))) return;
	if (!(i_MenuImage[3] = LoadGraph("Image/BlueWarota.png"))) return;
	if (!(i_MenuImage[4] = LoadGraph("Image/ReMatch.png"))) return;
	if (!(i_MenuImage[5] = LoadGraph("Image/ReTitle.png"))) return;
	if (!(i_MenuImage[6] = LoadGraph("Image/Mokou.png"))) return;
}

////////////////////////////////////////////////
// 画像削除
////////////////////////////////////////////////
void Result::DeleteImages() {
	i_ResultImage = DeleteGraph(i_ResultImage);

	for (int i = 0; i < 7; i++) {
		i_MenuImage[i] = DeleteGraph(i_MenuImage[i]);
	}
}

////////////////////////////////////////////////
// 音データ読み込み
////////////////////////////////////////////////
void Result::LoadSounds() {
	if ((s_WinSE = LoadSoundMem("sounds/SE/Result_Win.mp3")) == -1) return;
	if ((s_LoseSE = LoadSoundMem("sounds/SE/Result_Zimetsu.mp3")) == -1) return;
}

////////////////////////////////////////////////
// 音データ消去
////////////////////////////////////////////////
void Result::DeleteSounds() {
	s_WinSE = DeleteSoundMem(s_WinSE);
	s_LoseSE = DeleteSoundMem(s_LoseSE);
}

////////////////////////////////////////////////
// 音の音量変更
////////////////////////////////////////////////
void Result::ChangeVolume(float SEVolume) {
	int volume = 255.0f * SEVolume;

	ChangeVolumeSoundMem(volume, s_WinSE);
	ChangeVolumeSoundMem(volume, s_LoseSE);
}

////////////////////////////////////////////////
// デストラクタ
////////////////////////////////////////////////
Result::~Result() {
	DeleteImages();
	DeleteSounds();
}