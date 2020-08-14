#include "GameMain.h"
#include "GameManager.h"
#include "Player.h"
#include "Bullet.h"
#include "Result.h"

// �R���X�g���N�^
GameMain::GameMain(void) {	
	SCREEN_WIDTH_HALF = SCREEN_WIDTH / 2;		// �v�Z�Ɏg����ʂ̉����̔����̐��l�����������Ă���
	SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;		// �v�Z�Ɏg����ʂ̍����̔����̐��l�����������Ă���
	mStartTime = 0;		//����J�n����
	mCount = 0;			//�J�E���^
	mFps = 0;
	pauseFlg = false;
	pausePushPLNum = 0;
	netBattleFlg = false;
}

// FPS���Œ肷�邽�߂̊֐�
bool GameMain::FPSUpdate(void) {
	if (mCount == 0) { //1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //60�t���[���ڂȂ畽�ς��v�Z����
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

// FPS���Œ肷�邽�߂̊֐�
void GameMain::UpdateWait(void) {
	int tookTime = GetNowCount() - mStartTime;	//������������
	int waitTime = mCount * 1000 / FPS - tookTime;	//�҂ׂ�����
	if (waitTime > 0) {
		Sleep(waitTime);	//�ҋ@
	}
}

int GameMain::FirstInit(void) {
	SetMainWindowText("�o�g�V�F�b�g");	// �E�B���h�E�̖��O�ݒ�
	SetGraphMode(DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, 32);	// �E�B���h�E�̌v�Z�p�傫���ݒ�
	ChangeWindowMode(TRUE);				// �E�B���h�E���[�h�ŋN��
	SetDoubleStartValidFlag(TRUE);		// �E�B���h�E��2�܂ő��d�N��������
	if (DxLib_Init() == -1) return -1;	// DxLib�̏�������������肭�����Ȃ������狭���I��
	SetAlwaysRunFlag(TRUE);
	offscreen_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);	// �E�B���h�E�̕`�掞�̑傫����ݒ�
	SetDrawScreen(offscreen_handle);

	LoadCursorImages();

	// ���͊Ǘ��N���X�𐶐�
	CreateInputManagerObj();

	// �t�H���g�f�[�^�Ǘ��N���X�𐶐�
	CreateFontDataObj();

	// �Q�[���i�s�Ǘ��N���X�𐶐�
	CreateGameManagerObj();

	// �^�C�g����ʃN���X�𐶐�
	CreateTitleObj();

	// �ʐM�ΐ�p�N���X�𐶐�
	CreateNetworkObj();

	return 1;
}

// �Q�[�����v���C���ȂǂɃN���X�𐶐����Ȃ���
void GameMain::Init() {
	gameManager->Init();	
	CreateOptionObj(0, 0);
	CreateUIObj();
	CreateBlockObj();
	CreateBulletObj();	
	CreatePlayerObj();
}

void GameMain::MainObjDelete() {	
	// UI�N���X����
	delete ui;
	ui = NULL;

	// �e�N���X����
	delete bullet;
	bullet = NULL;

	// �u���b�N�N���X����
	for (int i = 0; i < BLOCK_MAX; i++) {
		delete block[i];
		block[i] = NULL;
	}
	// �u���b�N�摜����
	DeleteBlockImages();

	// �v���C���[�N���X����
	delete player[GameManager::RED];
	delete player[GameManager::BLUE];
	player[GameManager::RED] = NULL;
	player[GameManager::BLUE] = NULL;
	
}

// �Q�[�����[�v
void GameMain::GameLoop(void) {
	inputManager->InputKey();	// ���͂��󂯎��

	while (ProcessMessage() == 0 && (inputManager->GetPadInput()[GameManager::RED].in_Button[BACK] == 0 &&
									 inputManager->GetPadInput()[GameManager::BLUE].in_Button[BACK] == 0) &&
									 inputManager->In_Key()[KEY_INPUT_F11] == 0 &&
									 gameManager->GetPhaseStatus() != GameManager::QUIT) {
		FPSUpdate();	//FPS�X�V

		SetDrawScreen(offscreen_handle);
		ClearDrawScreen();

		inputManager->InputKey();	// ���͂��󂯎��
		Update();	// �I�u�W�F�N�g�̏�����i�߂Ēl���X�V����
		Output();	// �I�u�W�F�N�g�̕`��n�֐����Ăяo��

		network->DrawNetWorkData();

		SetDrawScreen(DX_SCREEN_BACK);
		// ��ʂ�`��p�̑傫���Ɉ����L�΂��ĕ`�悷��
		DrawExtendGraph(0, 0, DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, offscreen_handle, FALSE);
		ScreenFlip();

		UpdateWait();		//FPS�ҋ@
	}
}

// �I�u�W�F�N�g�̏�����i�߂Ēl���X�V����
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




	// ������ւ�l�b�g���[�N�n///////////////////////////////////////////////////
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
		// �������t�F�[�Y
		Init();
		
		return;
		break;

	case GameManager::HIDE:
		// �B��鑤�t�F�[�Y
		// �ʐM�ΐ풆
		if (netBattleFlg) {
			if (!ui->TransitionAnimationWaiting()) return;
			gameManager->HideTimerControll_Net();
			player[gameManager->GetNowHider()]->HidingPlayerControll_Net();
		}
		// 1��̃p�\�R���őΐ풆
		else {
			if (!ui->TransitionAnimationWaiting()) return;
			if (PauseProcess()) return;
			gameManager->HideTimerControll();
			player[gameManager->GetNowHider()]->HidingPlayerControll();
		}
		
		return;
		break;

	case GameManager::SHOT:
		// �����t�F�[�Y
		// �ʐM�ΐ풆
		if (netBattleFlg) {
			if (!ui->TransitionAnimationWaiting()) return;
			gameManager->ShotTimerControll_Net();
			player[gameManager->GetNowShooter()]->ShooterPlayerControll_Net();
		}
		// 1��̃p�\�R���őΐ풆
		else {
			if (!ui->TransitionAnimationWaiting()) return;
			if (PauseProcess()) return;
			gameManager->ShotTimerControll();
			player[gameManager->GetNowShooter()]->ShooterPlayerControll();
		}
		
		return;
		break;

	case GameManager::RECOCHETWAIT:
		// �e�̒��e�҂��t�F�[�Y
		bullet->BulletControll();
		return;
		break;

	case GameManager::RESULT:
		// ���U���g���
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

// �I�u�W�F�N�g�̕`��n�֐����Ăяo��
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
		// �v���C���[�`��
		ui->DrawBackGround();
		player[GameManager::RED]->DrawPlayer();
		player[GameManager::BLUE]->DrawPlayer();

		// �u���b�N�`��
		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i]->DrawBlocks();
		}
		
		// �B���t�F�[�Y���̕����`��
		//DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "%s�����I", PlayerName[gameManager->GetNowHider()]);
		ui->DrawTransitionAnimation();

		
		ui->DrawPlayerGuage(player[nowhider]->GetPlayerX(), player[nowhider]->GetPlayerY(), float(gameManager->HidePhaseTime), float(gameManager->GetHideTime()), nowhider);

		// ��M�m�F�҂����̏���
		if (network->GetIsWaitRecvCheck()) {
			player[network->GetConnectType()]->DrawWaitRecvCheck();
		}

		break;

	case GameManager::SHOT:
		ui->DrawBackGround();
		// �v���C���[�`��
		player[GameManager::RED]->DrawPlayer();
		player[GameManager::BLUE]->DrawPlayer();

		// �u���b�N�`��
		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i]->DrawBlocks();
		}
		// �����t�F�[�Y�̕����`��A�����̑_���Ă�������`��
		//DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "%s���āI", PlayerName[gameManager->GetNowShooter()]);
		ui->DrawTransitionAnimation();

		if (ui->GetTransitionFlg()) return;
		player[gameManager->GetNowShooter()]->DrawTargetAngle();

		ui->DrawPlayerGuage(player[nowshooter]->GetPlayerX(), player[nowshooter]->GetPlayerY(), float(gameManager->ShotPhaseTime), float(gameManager->GetShotTime()), nowshooter);

		// ��M�m�F�҂����̏���
		if (network->GetIsWaitRecvCheck()) {
			player[network->GetConnectType()]->DrawWaitRecvCheck();
		}
		break;

	case GameManager::RECOCHETWAIT:
		ui->DrawBackGround();
		// �v���C���[�`��
		player[GameManager::RED]->DrawPlayer();
		player[GameManager::BLUE]->DrawPlayer();

		// �u���b�N�`��
		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i]->DrawBlocks();
		}
		// �e�`��֐�
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
		// ���U���g��ʕ`��
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

	DrawDebugInfo();	// �f�o�b�O���`��
}

// �f�o�b�O����`�悷�邽�߂̊֐�
void GameMain::DrawDebugInfo(void) {
	DrawFormatStringToHandle(0, 0, 0xFFFFFF, fontData->f_FontData[0], "%.1fFPS", mFps);
}

// �|�[�Y��ʂ��J����{�^���������ꂽ���`�F�b�N
bool GameMain::IsPushPauseButton() {
	if (inputManager->GetButtonDown(START, GameManager::RED)) {
		// �|�[�Y��ʂ��J����Ă���Ƃ��A�|�[�Y�{�^�����������l�ƍ��������l����v���Ȃ���Ζ�������
		if (pauseFlg && pausePushPLNum != GameManager::RED) {
			return false;
		}
		pausePushPLNum = GameManager::RED;
		return true;
	}
	if (inputManager->GetButtonDown(START, GameManager::BLUE)) {
		// �|�[�Y��ʂ��J����Ă���Ƃ��A�|�[�Y�{�^�����������l�ƍ��������l����v���Ȃ���Ζ�������
		if (pauseFlg && pausePushPLNum != GameManager::BLUE) {
			return false;
		}
		pausePushPLNum = GameManager::BLUE;
		return true;
	}
	if (inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
		// �|�[�Y��ʂ��J����Ă���Ƃ��A�|�[�Y�{�^�����������l�ƍ��������l����v���Ȃ���Ζ�������
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
	// ���͊Ǘ��N���X�𐶐��B�|�C���^��ۑ����Ă����B
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