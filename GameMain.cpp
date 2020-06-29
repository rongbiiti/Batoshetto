#include "GameMain.h"

GameMain::GameMain(void) {
	SCREEN_WIDTH_HALF = SCREEN_WIDTH / 2;
	SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;
	main = this;
}

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

	inputManager = new InputManager;
	fontData = new FontData;

	gameManager = new GameManager(this);

	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i] = new Block(i, fontData);
	}

	bullet = new Bullet();

	player[0] = new Player(0, 0xE71122, true, this);
	player[1] = new Player(1, 0x1122E7, false, this);

	return 1;
}

void GameMain::GameLoop(void) {
	inputManager->InputKey();

	while (ProcessMessage() == 0 && (inputManager->k_Buf[KEY_INPUT_ESCAPE] == 0)) {
		FPSUpdate();	//FPS更新

		SetDrawScreen(offscreen_handle);
		ClearDrawScreen();

		inputManager->InputKey();	// 入力を受け取る
		Update();
		Output();

		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, offscreen_handle, FALSE);	// 画面を描画用の大きさに引き伸ばして描画する
		ScreenFlip();

		UpdateWait();		//FPS待機
	}
}

void GameMain::Update(void) {
	switch (gameManager->GetPhaseStatus())
	{
	case GameManager::INIT:
		inputManager = new InputManager;
		fontData = new FontData;

		gameManager = new GameManager(this);

		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i] = new Block(i, fontData);
		}

		bullet = new Bullet();

		player[0] = new Player(0, 0xE71122, true, this);
		player[1] = new Player(1, 0x1122E7, false, this);
		return;
		break;
	case GameManager::HIDE:
		gameManager->HideTimerControll();
		player[gameManager->GetNowHider()]->HidingPlayerControll();
		//PlayerHitCheck();
		return;
		break;

	case GameManager::SHOT:
		gameManager->ShotTimerControll();
		player[gameManager->GetNowShooter()]->ShooterPlayerControll();
		//DrawTargetAngle();
		//PlayerHitCheck();
		return;
		break;

	case GameManager::RECOCHETWAIT:
		bullet->BulletControll();
		return;
		break;

	case GameManager::RESULT:
		return;
		break;
	}
	
}

void GameMain::Output(void) {
	player[GameManager::RED]->DrawPlayer();
	player[GameManager::BLUE]->DrawPlayer();
	
	//DrawWalls();
	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i]->DrawBlocks();
	}
	DrawDebugInfo();

	switch (gameManager->GetPhaseStatus())
	{
	case GameManager::HIDE:
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "隠れろ！");
		break;

	case GameManager::SHOT:
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "撃て！");
		player[gameManager->GetNowShooter()]->DrawTargetAngle();
		break;

	case GameManager::RECOCHETWAIT:
		if (bullet->IsAlive()) {
			bullet->DrawBullet();
			DrawFormatStringToHandle(420, 120, 0xFFFFFF, fontData->f_FontData[1], "跳弾残り %d回", bullet->GetRicochetCount());
		}
		break;

	case GameManager::RESULT:
		result->DrawResult();
		return;
		break;
	}
}

void GameMain::DrawDebugInfo(void) {
	DrawFormatStringToHandle(0, 0, 0xFFFFFF, fontData->f_FontData[0], "%.1fFPS", mFps);
	DrawFormatStringToHandle(0, 20, 0xFFFFFF, fontData->f_FontData[0], "隠れる時間残り%dフレ", gameManager->GetHideTime());
	DrawFormatStringToHandle(0, 40, 0xFFFFFF, fontData->f_FontData[0], "狙える時間残り%dフレ", gameManager->GetShotTime());
	//for (int i = 0; i < 2; i++) {
	//	DrawFormatStringToHandle(0 + i * 200, 60, 0xFFFFFF, fontData->f_FontData[0], "%d %dPのx", player[i]->x, i+1);
	//	DrawFormatStringToHandle(0 + i * 200, 80, 0xFFFFFF, fontData->[0], "%d %dPのy", player[i]->y, i+1);
	//	DrawFormatStringToHandle(0 + i * 200, 100, 0xFFFFFF, fontData->f_FontData[0], "%d %dPのtarx", player[i]->targetx, i + 1);
	//	DrawFormatStringToHandle(0 + i * 200, 120, 0xFFFFFF, fontData->f_FontData[0], "%d %dPのtary", player[i]->targety, i + 1);
	//	DrawFormatStringToHandle(0 + i * 200, 140, 0xFFFFFF, fontData->f_FontData[0], "%d %dPのangle", player[i]->angle, i + 1);
	//}
	//DrawFormatStringToHandle(0, 160, 0xFFFFFF, fontData->f_FontData[0], "弾の位置X %f", bullet->x);
	//DrawFormatStringToHandle(0, 180, 0xFFFFFF, fontData->f_FontData[0], "弾の位置Y %f", bullet->y);
	//DrawFormatStringToHandle(0, 200, 0xFFFFFF, fontData->f_FontData[0], "弾の進行X %f.5", cos(bullet->angle * DX_PI / 180) * g_Bullet.movespeedx);
	//DrawFormatStringToHandle(0, 220, 0xFFFFFF, fontData->f_FontData[0], "弾の進行Y %f.5", sin(bullet->angle * DX_PI / 180) * g_Bullet.movespeedy);
}