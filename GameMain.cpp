#include "GameMain.h"
#include "GameManager.h"
#include "Player.h"
#include "Bullet.h"
#include "Result.h"

// コンストラクタ
GameMain::GameMain(void) {	
	SCREEN_WIDTH_HALF = SCREEN_WIDTH / 2;		// 計算に使う画面の横幅の半分の数値を初期化しておく
	SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;		// 計算に使う画面の高さの半分の数値を初期化しておく
	mStartTime = 0;		//測定開始時刻
	mCount = 0;			//カウンタ
	mFps = 0;
}

// FPSを固定するための関数
bool GameMain::FPSUpdate(void) {
	if (mCount == 0) { //1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //60フレーム目なら平均を計算する
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

// FPSを固定するための関数
void GameMain::UpdateWait(void) {
	int tookTime = GetNowCount() - mStartTime;	//かかった時間
	int waitTime = mCount * 1000 / FPS - tookTime;	//待つべき時間
	if (waitTime > 0) {
		Sleep(waitTime);	//待機
	}
}

int GameMain::FirstInit(void) {
	SetMainWindowText("バトシェット");	// ウィンドウの名前設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);	// ウィンドウの計算用大きさ設定
	ChangeWindowMode(TRUE);				// ウィンドウモードで起動
	if (DxLib_Init() == -1) return -1;	// DxLibの初期化処理が上手くいかなかったら強制終了
	offscreen_handle = MakeScreen(DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, FALSE);	// ウィンドウの描画時の大きさを設定
	SetDrawScreen(offscreen_handle);

	inputManager = new InputManager;		// 入力管理クラスを生成。ポインタを保存しておく。
	fontData = new FontData;				// フォントデータ管理クラスを生成。ポインタを保存しておく。

	gameManager = new GameManager(this);	// ゲーム進行管理クラスを生成。ポインタを保存しておく。

	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i] = new Block(i, fontData);	// ブロックオブジェクトを生成。ポインタを保存しておく。
	}

	bullet = new Bullet();					// 弾オブジェクトを生成。ポインタを保存しておく。

	player[GameManager::RED] = new Player(GameManager::RED, 0xE71122, true, this);		// プレイヤーREDを生成。ポインタを保存しておく。
	player[GameManager::BLUE] = new Player(GameManager::BLUE, 0x1122E7, false, this);	// プレイヤーBLUEを生成。ポインタを保存しておく。

	return 1;
}

// ゲームリプレイ時などにクラスを生成しなおす
void GameMain::Init() {
	gameManager->~GameManager();
	gameManager = new GameManager(this);

	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i]->~Block();
		block[i] = new Block(i, fontData);
	}

	bullet->~Bullet();
	bullet = new Bullet();

	player[GameManager::RED]->~Player();
	player[GameManager::RED] = new Player(0, 0xE71122, true, this);

	player[GameManager::BLUE]->~Player();
	player[GameManager::BLUE] = new Player(1, 0x1122E7, false, this);
}

// ゲームループ
void GameMain::GameLoop(void) {
	inputManager->InputKey();	// 入力を受け取る

	while (ProcessMessage() == 0 && (inputManager->GetPadInput()[GameManager::RED].in_Button[InputManager::BACK] == 0 &&
									inputManager->GetPadInput()[GameManager::BLUE].in_Button[InputManager::BACK] == 0) &&
									inputManager->In_Key()[KEY_INPUT_ESCAPE] == 0) {
		FPSUpdate();	//FPS更新

		SetDrawScreen(offscreen_handle);
		ClearDrawScreen();

		inputManager->InputKey();	// 入力を受け取る
		Update();	// オブジェクトの処理を進めて値を更新する
		Output();	// オブジェクトの描画系関数を呼び出す

		SetDrawScreen(DX_SCREEN_BACK);
		// 画面を描画用の大きさに引き伸ばして描画する
		DrawExtendGraph(0, 0, DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, offscreen_handle, FALSE);
		ScreenFlip();

		UpdateWait();		//FPS待機
	}
}

// オブジェクトの処理を進めて値を更新する
void GameMain::Update(void) {
	switch (gameManager->GetPhaseStatus())
	{
	case GameManager::INIT:
		// 初期化フェーズ
		Init();
		
		return;
		break;

	case GameManager::HIDE:
		// 隠れる側フェーズ
		gameManager->HideTimerControll();
		player[gameManager->GetNowHider()]->HidingPlayerControll();
		return;
		break;

	case GameManager::SHOT:
		// 撃つ側フェーズ
		gameManager->ShotTimerControll();
		player[gameManager->GetNowShooter()]->ShooterPlayerControll();
		return;
		break;

	case GameManager::RECOCHETWAIT:
		// 弾の跳弾待ちフェーズ
		bullet->BulletControll();
		return;
		break;

	case GameManager::RESULT:
		// リザルト画面
		return;
		break;
	}
	
}

// オブジェクトの描画系関数を呼び出す
void GameMain::Output(void) {
	float x1 = 0;
	float x2 = 0;
	// プレイヤー描画
	player[GameManager::RED]->DrawPlayer();
	player[GameManager::BLUE]->DrawPlayer();
	
	// ブロック描画
	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i]->DrawBlocks();
	}
	DrawDebugInfo();	// デバッグ情報描画

	switch (gameManager->GetPhaseStatus())
	{
	case GameManager::HIDE:
		// 隠れるフェーズ時の文字描画
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "%s隠れろ！", PlayerName[gameManager->GetNowHider()]);
		DrawBox(0, 683, SCREEN_WIDTH - 1, SCREEN_HEIGHT, COLOR_VALUE_PLAYER[gameManager->GetNowHider()], 0);
		x1 = (float(SCREEN_WIDTH_HALF) / float(gameManager->HidePhaseTime)) * (gameManager->HidePhaseTime - gameManager->GetHideTime());
		x2 = (float(SCREEN_WIDTH_HALF) / float(gameManager->HidePhaseTime)) * (gameManager->GetHideTime()) + SCREEN_WIDTH_HALF;
		DrawBox(x1, 684, x2, SCREEN_HEIGHT - 2, COLOR_VALUE_PLAYER[gameManager->GetNowHider()], 1);
		DrawLine(SCREEN_WIDTH_HALF, 684, SCREEN_WIDTH_HALF, SCREEN_HEIGHT - 3, 0xffffff, 2);

		break;

	case GameManager::SHOT:
		// 撃つ側フェーズの文字描画、撃つ側の狙っている方向描画
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "%s撃て！", PlayerName[gameManager->GetNowShooter()]);
		DrawBox(0, 683, SCREEN_WIDTH - 1, SCREEN_HEIGHT, COLOR_VALUE_PLAYER[gameManager->GetNowShooter()], 0);
		x1 = (float(SCREEN_WIDTH_HALF) / float(gameManager->ShotPhaseTime)) * (gameManager->ShotPhaseTime - gameManager->GetShotTime());
		x2 = (float(SCREEN_WIDTH_HALF) / float(gameManager->ShotPhaseTime)) * (gameManager->GetShotTime()) + SCREEN_WIDTH_HALF;
		DrawBox(x1, 684, x2, SCREEN_HEIGHT - 2, COLOR_VALUE_PLAYER[gameManager->GetNowShooter()], 1);
		DrawLine(SCREEN_WIDTH_HALF, 684, SCREEN_WIDTH_HALF, SCREEN_HEIGHT - 3, 0xffffff, 2);

		player[gameManager->GetNowShooter()]->DrawTargetAngle();

		break;

	case GameManager::RECOCHETWAIT:
		// 弾描画関数
		if (bullet->IsAlive()) {
			bullet->DrawBullet();
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
			int fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d", bullet->GetRicochetCount());
			DrawFormatStringToHandle(SCREEN_WIDTH_HALF - fontwidth / 2, SCREEN_HEIGHT_HALF - fontwidth, 0xFFFFFF, fontData->f_FontData[1], "%d", bullet->GetRicochetCount());
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		break;

	case GameManager::RESULT:
		// リザルト画面描画
		result->DrawResult();
		return;
		break;
	}
}

// デバッグ情報を描画するための関数
void GameMain::DrawDebugInfo(void) {
	DrawFormatStringToHandle(0, 0, 0xFFFFFF, fontData->f_FontData[0], "%.1fFPS", mFps);
}