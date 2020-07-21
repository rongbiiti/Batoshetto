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
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);	// �E�B���h�E�̌v�Z�p�傫���ݒ�
	ChangeWindowMode(TRUE);				// �E�B���h�E���[�h�ŋN��
	if (DxLib_Init() == -1) return -1;	// DxLib�̏�������������肭�����Ȃ������狭���I��
	offscreen_handle = MakeScreen(DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, FALSE);	// �E�B���h�E�̕`�掞�̑傫����ݒ�
	SetDrawScreen(offscreen_handle);

	// ���͊Ǘ��N���X�𐶐�
	CreateInputManagerObj();

	// �t�H���g�f�[�^�Ǘ��N���X�𐶐�
	CreateFontDataObj();

	// �Q�[���i�s�Ǘ��N���X�𐶐�
	CreateGameManagerObj();

	// �^�C�g����ʃN���X�𐶐�
	CreateTitleObj();

	return 1;
}

// �Q�[�����v���C���ȂǂɃN���X�𐶐����Ȃ���
void GameMain::Init() {
	gameManager->Init();

	ui->~UI();
	CreateUIObj();

	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i]->~Block();
	}
	CreateBlockObj();

	bullet->~Bullet();
	CreateBulletObj();

	player[GameManager::RED]->~Player();
	player[GameManager::BLUE]->~Player();
	CreatePlayerObj();

}

// �Q�[�����[�v
void GameMain::GameLoop(void) {
	inputManager->InputKey();	// ���͂��󂯎��

	while (ProcessMessage() == 0 && (inputManager->GetPadInput()[GameManager::RED].in_Button[InputManager::BACK] == 0 &&
									inputManager->GetPadInput()[GameManager::BLUE].in_Button[InputManager::BACK] == 0) &&
									inputManager->In_Key()[KEY_INPUT_F11] == 0) {
		FPSUpdate();	//FPS�X�V

		SetDrawScreen(offscreen_handle);
		ClearDrawScreen();

		inputManager->InputKey();	// ���͂��󂯎��
		Update();	// �I�u�W�F�N�g�̏�����i�߂Ēl���X�V����
		Output();	// �I�u�W�F�N�g�̕`��n�֐����Ăяo��

		SetDrawScreen(DX_SCREEN_BACK);
		// ��ʂ�`��p�̑傫���Ɉ����L�΂��ĕ`�悷��
		DrawExtendGraph(0, 0, DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, offscreen_handle, FALSE);
		ScreenFlip();

		UpdateWait();		//FPS�ҋ@
	}
}

// �I�u�W�F�N�g�̏�����i�߂Ēl���X�V����
void GameMain::Update(void) {
	if (IsPushPauseButton()) {
		if (pauseFlg) {
			pauseScreen->~PauseScreen();
		}
		else {
			CreatePauseScreenObj();
		}
		pauseFlg = !pauseFlg;
		return;
	}

	if (pauseFlg) {
		pauseScreen->PauseScreenControll();
		return;
	}

	switch (gameManager->GetPhaseStatus())
	{
	case GameManager::TITLE:
		if (title == nullptr) {
			CreateTitleObj();
		}
		title->TitleControll();
		
		return;
		break;
	case GameManager::DIFFICULTYSELECT:
		diffiSelectScene->DifficultySelectControll();

		return;
		break;
	case GameManager::INIT:
		// �������t�F�[�Y
		Init();
		
		return;
		break;

	case GameManager::HIDE:
		// �B��鑤�t�F�[�Y
		gameManager->HideTimerControll();
		player[gameManager->GetNowHider()]->HidingPlayerControll();
		return;
		break;

	case GameManager::SHOT:
		// �����t�F�[�Y
		gameManager->ShotTimerControll();
		player[gameManager->GetNowShooter()]->ShooterPlayerControll();
		return;
		break;

	case GameManager::RECOCHETWAIT:
		// �e�̒��e�҂��t�F�[�Y
		bullet->BulletControll();
		return;
		break;

	case GameManager::RESULT:
		// ���U���g���
		result->ResultControll();
		return;
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
	case GameManager::DIFFICULTYSELECT:
		diffiSelectScene->DrawDifficultySelectScene();
		return;
		break;
	case GameManager::HIDE:
		// �v���C���[�`��
		player[GameManager::RED]->DrawPlayer();
		player[GameManager::BLUE]->DrawPlayer();

		// �u���b�N�`��
		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i]->DrawBlocks();
		}
		
		// �B���t�F�[�Y���̕����`��
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "%s�B���I", PlayerName[gameManager->GetNowHider()]);

		ui->DrawPlayerGuage(player[nowhider]->GetPlayerX(), player[nowhider]->GetPlayerY(), float(gameManager->HidePhaseTime), float(gameManager->GetHideTime()), nowhider);

		break;

	case GameManager::SHOT:
		// �v���C���[�`��
		player[GameManager::RED]->DrawPlayer();
		player[GameManager::BLUE]->DrawPlayer();

		// �u���b�N�`��
		for (int i = 0; i < BLOCK_MAX; i++) {
			block[i]->DrawBlocks();
		}
		// �����t�F�[�Y�̕����`��A�����̑_���Ă�������`��
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "%s���āI", PlayerName[gameManager->GetNowShooter()]);

		player[gameManager->GetNowShooter()]->DrawTargetAngle();

		ui->DrawPlayerGuage(player[nowshooter]->GetPlayerX(), player[nowshooter]->GetPlayerY(), float(gameManager->ShotPhaseTime), float(gameManager->GetShotTime()), nowshooter);
		break;

	case GameManager::RECOCHETWAIT:
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
			int fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d", bullet->GetRicochetCount());
			DrawFormatStringToHandle(SCREEN_WIDTH_HALF - fontwidth / 2, SCREEN_HEIGHT_HALF - fontwidth, 0xFFFFFF, fontData->f_FontData[1], "%d", bullet->GetRicochetCount());
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		break;

	case GameManager::RESULT:
		// ���U���g��ʕ`��
		result->DrawResult();
		return;
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
	if (inputManager->GetPadInput()[GameManager::RED].in_Button[InputManager::START] == 1) {
		// �|�[�Y��ʂ��J����Ă���Ƃ��A�|�[�Y�{�^�����������l�ƍ��������l����v���Ȃ���Ζ�������
		if (pauseFlg && pausePushPLNum != GameManager::RED) {
			return false;
		}
		pausePushPLNum = GameManager::RED;
		return true;
	}
	if (inputManager->GetPadInput()[GameManager::BLUE].in_Button[InputManager::START] == 1) {
		// �|�[�Y��ʂ��J����Ă���Ƃ��A�|�[�Y�{�^�����������l�ƍ��������l����v���Ȃ���Ζ�������
		if (pauseFlg && pausePushPLNum != GameManager::BLUE) {
			return false;
		}
		pausePushPLNum = GameManager::BLUE;
		return true;
	}
	if (inputManager->In_Key()[KEY_INPUT_ESCAPE] == 1) {
		// �|�[�Y��ʂ��J����Ă���Ƃ��A�|�[�Y�{�^�����������l�ƍ��������l����v���Ȃ���Ζ�������
		if (pauseFlg && pausePushPLNum != GameManager::BLUE + 1) {
			return false;
		}
		pausePushPLNum = GameManager::BLUE + 1;
		return true;
	}
	return false;
}

void GameMain::CreateInputManagerObj() {
	// ���͊Ǘ��N���X�𐶐��B�|�C���^��ۑ����Ă����B
	inputManager = new InputManager;
}

void GameMain::CreateFontDataObj() {
	fontData = new FontData();
}

void GameMain::CreatePlayerObj() {
	player[GameManager::RED] = new Player(0, 0xE71122, true, this);
	player[GameManager::BLUE] = new Player(1, 0x1122E7, false, this);
}

void GameMain::CreateBulletObj() {
	bullet = new Bullet();
}

void GameMain::CreateGameManagerObj() {
	gameManager = new GameManager(this);
}

void GameMain::CreateBlockObj() {
	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i] = new Block(i, fontData, this);
	}
}

void GameMain::CreateResultObj(int hitPNum) {
	result = new Result(fontData, inputManager, gameManager, hitPNum);
}

void GameMain::CreateTitleObj() {
	title = new Title(fontData, inputManager, gameManager);
}

void GameMain::CreateDifficultySelectSceneObj() {
	diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
}

void GameMain::CreatePauseScreenObj() {
	pauseScreen = new PauseScreen(fontData, inputManager, this, pausePushPLNum);
}

void GameMain::CreateUIObj() {
	ui = new UI(this);
}