#include "Option.h"

////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
Option::Option(GameMain* main, int pushPLnum, int prescreennum, int prephase) {
	gameMain = main;
	inputManager = gameMain->inputManager;
	fontData = gameMain->fontData;
	LoadFile();
	BGMVolume = saveData.bgmVolume;	// BGMの音量
	SEVolume = saveData.seVolume;	// SEの音量
	screenNum = VOLUME;
	optionPushPLNum = pushPLnum;
	previousNum = prescreennum;
	previousGamePhase = prephase;
	selectNum[0] = 0;
	selectNum[1] = 0;
	
	LoadImages();
}

////////////////////////////////////////////////
// オプション画面の処理関数
////////////////////////////////////////////////
void Option::OptionControll() {
	switch (screenNum)
	{
	case TOP:
		ControllOptionTop();
		break;
	case VOLUME:
		ControllVolumeScreen();
		break;
	}
}

////////////////////////////////////////////////
// オプショントップ画面の処理
////////////////////////////////////////////////
void Option::ControllOptionTop() {
	for (int i = 0; i < 2; i++) {

		if (optionPushPLNum == GameManager::BLUE + 1) break;	// ポーズボタンを押したのがキーボードのキーからなら、コントローラーの操作を受け付けない。
		if (optionPushPLNum != i) continue;					// ポーズボタンを押したプレイヤーでない場合、処理をスキップ

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
				screenNum = VOLUME;
				selectNum[0] = 0;
				selectNum[1] = 0;
				break;
			case 1:
				switch (previousNum)
				{
				case TITLE:
					Return_to_Title();
					break;
				case PAUSE:
					Return_to_PauseScreen();
					break;
				}
				break;
			}
			gameMain->PlayDicideSE();
			return;
		}
	}


	if (optionPushPLNum != GameManager::BLUE + 1) return;	// ポーズボタンを押したのがコントローラーだったら、キーボードの処理を受け付けないでreturn


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
			screenNum = VOLUME;
			selectNum[0] = 0;
			selectNum[1] = 0;
			break;
		case 1:
			switch (previousNum)
			{
			case TITLE:
				Return_to_Title();
				break;
			case PAUSE:
				Return_to_PauseScreen();
				break;
			}
			break;
		}
		gameMain->PlayDicideSE();
	}
}

////////////////////////////////////////////////
// 音量調節画面の処理
////////////////////////////////////////////////
void Option::ControllVolumeScreen() {
	for (int i = 0; i < 2; i++) {

		if (optionPushPLNum == GameManager::BLUE + 1) break;	// ポーズボタンを押したのがキーボードのキーからなら、コントローラーの操作を受け付けない。
		if (optionPushPLNum != i) continue;					// ポーズボタンを押したプレイヤーでない場合、処理をスキップ

		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// ゲームパッドの方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
			// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
			if (--selectNum[i] < 0) {
				selectNum[i] = VOLUME_SELECT_NUM_MAX;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// ゲームパッドの方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
			// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
			if (++selectNum[i] > VOLUME_SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_RIGHT, i) || inputManager->GetButtonHold(PAD_RIGHT, i, 4)) {
			// ゲームパッドの方向パッド右の入力。18フレ以上押し続けてたら連続でインクリメント
			switch (selectNum[i])
			{
			case 0:
				BGMVolume += 0.1f;
				if (BGMVolume > 1) {
					BGMVolume = 1;
				}
				break;
			case 1:
				SEVolume += 0.1f;
				if (SEVolume > 1) {
					SEVolume = 1;
				}
				break;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_LEFT, i) || inputManager->GetButtonHold(PAD_LEFT, i, 4)) {
			// ゲームパッドの方向パッド左の入力。18フレ以上押し続けてたら連続でデクリメント
			switch (selectNum[i])
			{
			case 0:
				BGMVolume -= 0.1f;
				if (BGMVolume < 0) {
					BGMVolume = 0;
				}
				break;
			case 1:
				SEVolume -= 0.1f;
				if (SEVolume < 0) {
					SEVolume = 0;
				}
				break;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// ゲームパッド1のBボタン入力。
			switch (selectNum[i])
			{
			case 2:
				if (previousNum == PAUSE) {
					ChangeBulletSoundVolume();
				}
				gameMain->ChangeVolume(BGMVolume, SEVolume);
				gameMain->gameManager->ChangeVolume(SEVolume);
				SaveFile();
				BackOptionMenu();
				break;
			}
			gameMain->PlayDicideSE();
			return;
		}
	}


	if (optionPushPLNum != GameManager::BLUE + 1) return;	// ポーズボタンを押したのがコントローラーだったら、キーボードの処理を受け付けないでreturn


	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = VOLUME_SELECT_NUM_MAX;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum[GameManager::BLUE] > VOLUME_SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_RIGHT) || inputManager->GetKeyHold(KEY_INPUT_RIGHT, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			BGMVolume += 0.1f;
			if (BGMVolume > 1) {
				BGMVolume = 1;
			}
			break;
		case 1:
			SEVolume += 0.1f;
			if (SEVolume > 1) {
				SEVolume = 1;
			}
			break;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_LEFT) || inputManager->GetKeyHold(KEY_INPUT_LEFT, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			BGMVolume -= 0.1f;
			if (BGMVolume < 0) {
				BGMVolume = 0;
			}
			break;
		case 1:
			SEVolume -= 0.1f;
			if (SEVolume < 0) {
				SEVolume = 0;
			}
			break;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// ゲームパッド1のBボタン入力。
		switch (selectNum[GameManager::BLUE])
		{
		case 2:
			if (previousNum == PAUSE) {
				ChangeBulletSoundVolume();
			}
			gameMain->ChangeVolume(BGMVolume, SEVolume);
			gameMain->gameManager->ChangeVolume(SEVolume);
			SaveFile();
			BackOptionMenu();
			break;
		}
		gameMain->PlayDicideSE();
	}
}

////////////////////////////////////////////////
// タイトル画面へ戻る
////////////////////////////////////////////////
void Option::Return_to_Title() {
	gameMain->gameManager->SetPhaseStatus(GameManager::TITLE);
	gameMain->CreateTitleObj();
	this->~Option();
}

////////////////////////////////////////////////
// ポーズ画面へ戻る
////////////////////////////////////////////////
void Option::Return_to_PauseScreen() {
	gameMain->gameManager->SetPhaseStatus(previousGamePhase);
	gameMain->CreatePauseScreenObj();
	this->~Option();
}

////////////////////////////////////////////////
// オプション画面の描画
////////////////////////////////////////////////
void Option::DrawOption() {
	switch (screenNum)
	{
	case TOP:
		DrawOptionMain();
		break;
	case VOLUME:
		DrawVolumeMenu();
		break;
	}
}

////////////////////////////////////////////////
// オプション画面のトップを描画
////////////////////////////////////////////////
void Option::DrawOptionMain() {
	DrawBackGroundImage();

	// 文字の幅、		画面の横中心、　　　　　　　	Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// Pauseの文字描画
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Option");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "Option");

	// 各項目名描画
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	// ポーズ画面押したのがコントローラーなら赤か青を、キーボードなら必ず青を表示させる
	if (optionPushPLNum <= GameManager::BLUE) {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * optionPushPLNum), starty + y * selectNum[optionPushPLNum], 10, COLOR_VALUE_PLAYER[optionPushPLNum], 1, 1);
	}
	else if (optionPushPLNum == GameManager::BLUE + 1) {
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * GameManager::BLUE), starty + y * selectNum[GameManager::BLUE], 10, COLOR_VALUE_PLAYER[GameManager::BLUE], 1, 1);
	}
}

////////////////////////////////////////////////
// 音量調節画面を描画
////////////////////////////////////////////////
void Option::DrawVolumeMenu() {
	DrawBackGroundImage();

	// 文字の幅、		画面の横中心、　　　　　　　	Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400, extraX = 300, extraFlg = 1;

	// Pauseの文字描画
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Volume");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "Volume");

	// ポーズ画面押したのがコントローラーなら赤か青を、キーボードなら必ず青を表示させる
	if (optionPushPLNum <= GameManager::BLUE && !(selectNum[optionPushPLNum] < VOLUME_SELECT_NUM_MAX)) {
		// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * optionPushPLNum), starty - 100 + y * selectNum[optionPushPLNum], 1.0f, 0, gameMain->GetCursorImage(optionPushPLNum), TRUE);
	}
	else if (optionPushPLNum == GameManager::BLUE + 1 && !(selectNum[GameManager::BLUE] < VOLUME_SELECT_NUM_MAX)) {
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * GameManager::BLUE), starty - 100 + y * selectNum[GameManager::BLUE], 1.0f, 0, gameMain->GetCursorImage(GameManager::BLUE), TRUE);
	}
	else if (optionPushPLNum <= GameManager::BLUE) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", VolumeMenuName[selectNum[optionPushPLNum]].c_str());
		DrawBox(x - fontwidth / 2 - extraX, starty - 130 + y * selectNum[optionPushPLNum], x + fontwidth / 2 - extraX, starty - 130 + y * selectNum[optionPushPLNum] + y - 10, COLOR_VALUE_PLAYER[optionPushPLNum], 1);
	}
	else if (optionPushPLNum == GameManager::BLUE + 1) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", VolumeMenuName[selectNum[GameManager::BLUE]].c_str());
		DrawBox(x - fontwidth / 2 - extraX, starty - 130 + y * selectNum[GameManager::BLUE], x + fontwidth / 2 - extraX, starty - 130 + y * selectNum[GameManager::BLUE] + y - 10, COLOR_VALUE_PLAYER[GameManager::BLUE], 1);
	}

	// 各項目名描画
	for (int i = 0; i < VOLUME_SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", VolumeMenuName[i].c_str());
		if (!(i < VOLUME_SELECT_NUM_MAX)) extraFlg = 0;
		DrawFormatStringToHandle(x - fontwidth / 2 - extraX * extraFlg, starty - 130 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", VolumeMenuName[i].c_str());
	}

	extraX = 400;
	x = x - 100;
	DrawBox(x , starty - 115, x + extraX, starty - 95 , 0x222222, 1);
	DrawBox(x , starty - 115 + y, x + extraX, starty - 95 + y, 0x222222, 1);

	DrawBox(x, starty - 115, x + extraX * BGMVolume, starty - 95, 0xE7132c, 1);
	DrawBox(x, starty - 115 + y, x + extraX * SEVolume, starty - 95 + y, 0xE7132c, 1);

	DrawCircle(x + extraX * BGMVolume, starty - 105, 15, 0xEEEEEE, 1);
	DrawCircle(x + extraX * SEVolume, starty - 105 + y, 15, 0xEEEEEE, 1);

}

////////////////////////////////////////////////
// 各調節画面からトップに戻るための関数
////////////////////////////////////////////////
void Option::BackOptionMenu() {
	switch (previousNum)
	{
	case TITLE:
		Return_to_Title();
		break;
	case PAUSE:
		Return_to_PauseScreen();
		break;
	}
	screenNum = TOP;
	selectNum[0] = 0;
	selectNum[1] = 0;
}

////////////////////////////////////////////////
// 弾の音量調節
////////////////////////////////////////////////
void Option::ChangeBulletSoundVolume() {
	gameMain->bullet->ChangeVolume(SEVolume);
	gameMain->ChangeVolume(BGMVolume, SEVolume);
}

// セーブデータ読込み
void Option::LoadFile() {
	const char *fileName = "config.txt";
	FILE *fp;

	if ((fp = fopen(fileName, "r")) == NULL) {
		return;
	}
	else {
		fread(&saveData, sizeof(option_save_data_t), 1, fp);
		fclose(fp);
	}
}

////////////////////////////////////////////////
// セーブデータ保存
////////////////////////////////////////////////
void Option::SaveFile() {
	const char *fileName = "config.txt";
	FILE *fp;
	
	if ((fp = fopen(fileName, "w")) == NULL) {
		return;
	}
	else {
		saveData.bgmVolume = BGMVolume;
		saveData.seVolume = SEVolume;
		fwrite(&saveData, sizeof(option_save_data_t), 1, fp);
		fclose(fp);
	}
}

////////////////////////////////////////////////
// 背景画像描画
////////////////////////////////////////////////
void Option::DrawBackGroundImage() {
	DrawGraph(0, 0, i_BackImage, TRUE);		// 背景画像描画

	// 上から半透明の黒い四角をかぶせて明るさを下げている
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

////////////////////////////////////////////////
// 画像読み込み
////////////////////////////////////////////////
void Option::LoadImages() {
	if (!(i_BackImage = LoadGraph("Image/SelectBackImage.png"))) return;
}

////////////////////////////////////////////////
// 画像データメモリから消去
////////////////////////////////////////////////
void Option::DeleteImages() {
	i_BackImage = DeleteGraph(i_BackImage);
}

////////////////////////////////////////////////
// デストラクタ
////////////////////////////////////////////////
Option::~Option() {
	DeleteImages();
}