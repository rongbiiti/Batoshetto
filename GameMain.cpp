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
	pauseFlg = false;
	pausePushPLNum = 0;
	netBattleFlg = false;
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
	SetGraphMode(DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, 32);	// ウィンドウの計算用大きさ設定
	ChangeWindowMode(TRUE);				// ウィンドウモードで起動
	SetDoubleStartValidFlag(TRUE);		// ウィンドウを2個まで多重起動を許可
	if (DxLib_Init() == -1) return -1;	// DxLibの初期化処理が上手くいかなかったら強制終了
	SetAlwaysRunFlag(TRUE);
	offscreen_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);	// ウィンドウの描画時の大きさを設定
	SetDrawScreen(offscreen_handle);

	LoadCursorImages();

	// 入力管理クラスを生成
	CreateInputManagerObj();

	// フォントデータ管理クラスを生成
	CreateFontDataObj();

	// ゲーム進行管理クラスを生成
	CreateGameManagerObj();

	// タイトル画面クラスを生成
	CreateTitleObj();

	// 通信対戦用クラスを生成
	CreateNetworkObj();

	return 1;
}

// ゲームリプレイ時などにクラスを生成しなおす
void GameMain::Init() {
	gameManager->Init();	
	CreateOptionObj(0, 0);
	CreateUIObj();
	CreateBlockObj();
	CreateBulletObj();	
	CreatePlayerObj();
}

void GameMain::MainObjDelete() {	
	// UIクラス消去
	delete ui;
	ui = NULL;

	// 弾クラス消去
	delete bullet;
	bullet = NULL;

	// ブロッククラス消去
	for (int i = 0; i < BLOCK_MAX; i++) {
		delete block[i];
		block[i] = NULL;
	}
	// ブロック画像消去
	DeleteBlockImages();

	// プレイヤークラス消去
	delete player[GameManager::RED];
	delete player[GameManager::BLUE];
	player[GameManager::RED] = NULL;
	player[GameManager::BLUE] = NULL;
	
}

// ゲームループ
void GameMain::GameLoop(void) {
	inputManager->InputKey();	// 入力を受け取る

	while (ProcessMessage() == 0 && (inputManager->GetPadInput()[GameManager::RED].in_Button[BACK] == 0 &&
									 inputManager->GetPadInput()[GameManager::BLUE].in_Button[BACK] == 0) &&
									 inputManager->In_Key()[KEY_INPUT_F11] == 0 &&
									 gameManager->GetPhaseStatus() != GameManager::QUIT) {
		FPSUpdate();	//FPS更新

		SetDrawScreen(offscreen_handle);
		ClearDrawScreen();

		inputManager->InputKey();	// 入力を受け取る
		Update();	// オブジェクトの処理を進めて値を更新する
		Output();	// オブジェクトの描画系関数を呼び出す

		network->DrawNetWorkData();

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
	case GameManager::TITLE:
		if (title == nullptr) {
			CreateTitleObj();
		}
		title->TitleControll();
		
		return;
		break;




	// ここらへんネットワーク系///////////////////////////////////////////////////
	case GameManager::IPADDRESS_SELECT:
		network->IPAddressSelect();
		if (inputManager->GetButtonDown(A,0) || inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
			network->VariableInit();
			CreateDifficultySelectSceneObj();
			gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
		}
		return;
		break;
	case GameManager::CONNECT_TYPE_SELECT:
		network->CommunicationMethodSelect();
		if (inputManager->GetButtonDown(A, 0) || inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
			network->VariableInit();
			network->InitIPAddress();
			gameManager->SetPhaseStatus(GameManager::IPADDRESS_SELECT);
		}
		return;
		break;
	case GameManager::CONNECTION_WAIT:
		network->ConnectionWait();
		if (inputManager->GetButtonDown(A, 0) || inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
			network->VariableInit();
			gameManager->SetPhaseStatus(GameManager::CONNECT_TYPE_SELECT);
		}
		return;
		break;
	///////////////////////////////////////////////////////////////////////////



	case GameManager::END:
		if (end == nullptr) {
			CreateEndObj();
		}
		end->EndControll();

		return;
		break;
	case GameManager::DIFFICULTYSELECT:
		if(netBattleFlg) diffiSelectScene->DifficultySelectControll_Net();
		else diffiSelectScene->DifficultySelectControll();
		if (diffiSelectScene->GetReturnFlg()) {
			delete diffiSelectScene;
			diffiSelectScene = NULL;
		}

		return;
		break;
	case GameManager::INIT:
		// 初期化フェーズ
		Init();
		
		return;
		break;

	case GameManager::HIDE:
		// 隠れる側フェーズ
		// 通信対戦中
		if (netBattleFlg) {
			if (!ui->TransitionAnimationWaiting()) return;
			gameManager->HideTimerControll_Net();
			player[gameManager->GetNowHider()]->HidingPlayerControll_Net();
		}
		// 1台のパソコンで対戦中
		else {
			if (!ui->TransitionAnimationWaiting()) return;
			if (PauseProcess()) return;
			gameManager->HideTimerControll();
			player[gameManager->GetNowHider()]->HidingPlayerControll();
		}
		
		return;
		break;

	case GameManager::SHOT:
		// 撃つ側フェーズ
		// 通信対戦中
		if (netBattleFlg) {
			if (!ui->TransitionAnimationWaiting()) return;
			gameManager->ShotTimerControll_Net();
			player[gameManager->GetNowShooter()]->ShooterPlayerControll_Net();
		}
		// 1台のパソコンで対戦中
		else {
			if (!ui->TransitionAnimationWaiting()) return;
			if (PauseProcess()) return;
			gameManager->ShotTimerControll();
			player[gameManager->GetNowShooter()]->ShooterPlayerControll();
		}
		
		return;
		break;

	case GameManager::RECOCHETWAIT:
		// 弾の跳弾待ちフェーズ
		bullet->BulletControll();
		return;
		break;

	case GameManager::RESULT:
		// リザルト画面
		if (netBattleFlg) {
			result->ResultControll_Net();
		}
		else {
			result->ResultControll();
		}
		return;
		break;
	case GameManager::OPTION:
		option->OptionControll();
		if (inputManager->GetButtonDown(A, 0) || inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
			CreateTitleObj();
		}
		break;
	case GameManager::QUIT:

		break;
	}
	
}

// オブジェクトの描画系関数を呼び出す
void GameMain::Output(void) {
	float x1 = 0;
	float x2 = 0;
	int nowhider = gameManager->GetNowHider();
	int nowshooter = gameManager->GetNowShooter();	

	switch (gameManager->GetPhaseStatus())
	{
	case GameManager::TITLE:
		title->DrawTitle();

		return;
		break;
	case GameManager::IPADDRESS_SELECT:
		network->DrawIPAddressSelect();

		return;
		break;
	case GameManager::CONNECT_TYPE_SELECT:
		network->DrawCommunicationMethodSelect();
		return;
		break;
	case GameManager::CONNECTION_WAIT:
		network->DrawConnectionWait();
		return;
		break;
	case GameManager::END:
		end->DrawEnd();

		return;
		break;
	case GameManager::DIFFICULTYSELECT:
		if(netBattleFlg) diffiSelectScene->DrawDifficultySelectScene_Net();
		else diffiSelectScene->DrawDifficultySelectScene();
		
		return;
		break;
	case GameManager::HIDE:
		// プレイヤー描画
		ui->DrawBackGround();
		player[GameManager::RED]->DrawPlayer();
		player[GameManager::BLUE]->DrawPlayer();

		// ブロック描画
		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i]->DrawBlocks();
		}
		
		// 隠れるフェーズ時の文字描画
		//DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "%s動け！", PlayerName[gameManager->GetNowHider()]);
		ui->DrawTransitionAnimation();

		
		ui->DrawPlayerGuage(player[nowhider]->GetPlayerX(), player[nowhider]->GetPlayerY(), float(gameManager->HidePhaseTime), float(gameManager->GetHideTime()), nowhider);

		// 受信確認待ち中の処理
		if (network->GetIsWaitRecvCheck()) {
			player[network->GetConnectType()]->DrawWaitRecvCheck();
		}

		break;

	case GameManager::SHOT:
		ui->DrawBackGround();
		// プレイヤー描画
		player[GameManager::RED]->DrawPlayer();
		player[GameManager::BLUE]->DrawPlayer();

		// ブロック描画
		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i]->DrawBlocks();
		}
		// 撃つ側フェーズの文字描画、撃つ側の狙っている方向描画
		//DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "%s撃て！", PlayerName[gameManager->GetNowShooter()]);
		ui->DrawTransitionAnimation();

		if (ui->GetTransitionFlg()) return;
		player[gameManager->GetNowShooter()]->DrawTargetAngle();

		ui->DrawPlayerGuage(player[nowshooter]->GetPlayerX(), player[nowshooter]->GetPlayerY(), float(gameManager->ShotPhaseTime), float(gameManager->GetShotTime()), nowshooter);

		// 受信確認待ち中の処理
		if (network->GetIsWaitRecvCheck()) {
			player[network->GetConnectType()]->DrawWaitRecvCheck();
		}
		break;

	case GameManager::RECOCHETWAIT:
		ui->DrawBackGround();
		// プレイヤー描画
		player[GameManager::RED]->DrawPlayer();
		player[GameManager::BLUE]->DrawPlayer();

		// ブロック描画
		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i]->DrawBlocks();
		}
		// 弾描画関数
		if (bullet->IsAlive()) {
			bullet->DrawBullet();

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

			int fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[2], "%d", bullet->GetRicochetCount());

			if(0 <= bullet->GetRicochetCount())
			DrawFormatStringToHandle(SCREEN_WIDTH_HALF - fontwidth / 2, SCREEN_HEIGHT_HALF - fontwidth, 0xFFFFFF, fontData->f_FontData[1], "%d", bullet->GetRicochetCount());

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		bullet->DrawSHINOBIEXECUTION();
		break;

	case GameManager::RESULT:
		// リザルト画面描画
		if (netBattleFlg) {
			result->DrawResult_Net();
		}
		else {
			result->DrawResult();
		}
		
		return;
		break;
	case GameManager::OPTION:
		option->DrawOption();
		break;
	}

	if (pauseFlg) {
		pauseScreen->DrawPauseScreen();
	}

	DrawDebugInfo();	// デバッグ情報描画
}

// デバッグ情報を描画するための関数
void GameMain::DrawDebugInfo(void) {
	DrawFormatStringToHandle(0, 0, 0xFFFFFF, fontData->f_FontData[0], "%.1fFPS", mFps);
}

// ポーズ画面を開閉するボタンが押されたかチェック
bool GameMain::IsPushPauseButton() {
	if (inputManager->GetButtonDown(START, GameManager::RED)) {
		// ポーズ画面が開かれているとき、ポーズボタンを押した人と今押した人が一致しなければ無視する
		if (pauseFlg && pausePushPLNum != GameManager::RED) {
			return false;
		}
		pausePushPLNum = GameManager::RED;
		return true;
	}
	if (inputManager->GetButtonDown(START, GameManager::BLUE)) {
		// ポーズ画面が開かれているとき、ポーズボタンを押した人と今押した人が一致しなければ無視する
		if (pauseFlg && pausePushPLNum != GameManager::BLUE) {
			return false;
		}
		pausePushPLNum = GameManager::BLUE;
		return true;
	}
	if (inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
		// ポーズ画面が開かれているとき、ポーズボタンを押した人と今押した人が一致しなければ無視する
		if (pauseFlg && pausePushPLNum != GameManager::BLUE + 1) {
			return false;
		}
		pausePushPLNum = GameManager::BLUE + 1;
		return true;
	}
	return false;
}

bool GameMain::PauseProcess(void) {
	if (IsPushPauseButton()) {
		if (pauseFlg) {
			pauseScreen->~PauseScreen();
			pauseFlg = false;
		}
		else {
			CreatePauseScreenObj();
		}
		return true;
	}

	if (pauseFlg) {
		pauseScreen->PauseScreenControll();
		return true;
	}
	return false;
}

void GameMain::LoadBlockImages() {
	if (!(i_BlockImages[0] = LoadGraph("Image/Block03.png"))) return;
	if (!(i_BlockImages[1] = LoadGraph("Image/Block02.png"))) return;
	if (!(i_BlockImages[2] = LoadGraph("Image/Block01.png"))) return;
}

void GameMain::LoadCursorImages() {
	if (!(i_CursorImage[0] = LoadGraph("Image/PlayerCursor01.png"))) return;
	if (!(i_CursorImage[1] = LoadGraph("Image/PlayerCursor02.png"))) return;
}

void GameMain::DeleteBlockImages() {
	DeleteGraph(i_BlockImages[0]);
	DeleteGraph(i_BlockImages[1]);
	DeleteGraph(i_BlockImages[2]);
}

void GameMain::CreateInputManagerObj() {
	// 入力管理クラスを生成。ポインタを保存しておく。
	inputManager = new InputManager;
}

void GameMain::CreateFontDataObj() {
	fontData = new FontData();
}

void GameMain::CreatePlayerObj() {
	player[GameManager::RED] = new Player(0, 0xE71122, false, this);
	player[GameManager::BLUE] = new Player(1, 0x1122E7, false, this);
}

void GameMain::CreateBulletObj() {
	bullet = new Bullet();
	bullet->ChangeVolume(option->GetSEVolume());
}

void GameMain::CreateGameManagerObj() {
	gameManager = new GameManager(this);
}

void GameMain::CreateBlockObj() {
	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i] = new Block(i, fontData, this);
	}
	LoadBlockImages();
}

void GameMain::CreateResultObj(int hitPNum) {
	result = new Result(fontData, inputManager, gameManager, hitPNum);
}

void GameMain::CreateTitleObj() {
	title = new Title(fontData, inputManager, gameManager);
	if (ui != nullptr) {
		delete ui;
		ui = NULL;
	}
	gameManager->SetPhaseStatus(GameManager::TITLE);
	netBattleFlg = FALSE;
}

void GameMain::CreateEndObj() {
	end = new End(fontData, inputManager, gameManager);
	if (ui != nullptr) {
		delete ui;
		ui = NULL;
	}
}

void GameMain::CreateDifficultySelectSceneObj() {
	diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
}

void GameMain::CreatePauseScreenObj() {
	pauseScreen = new PauseScreen(fontData, inputManager, this, pausePushPLNum);
	pauseFlg = true;
}

void GameMain::CreateUIObj() {
	ui = new UI(this);
}

void GameMain::CreateOptionObj(int pushPLnum, int prescreennum) {
	if (prescreennum == Option::PAUSE) {
		option = new Option(this, pushPLnum, prescreennum, gameManager->GetPhaseStatus());
		return;
	}
	option = new Option(this, pushPLnum, prescreennum);
}

void GameMain::CreateNetworkObj() {
	network = new Network(fontData, inputManager, gameManager);
}

void GameMain::CreateResultObj_TimeOut() {
	result = new Result(fontData, inputManager, gameManager);
}