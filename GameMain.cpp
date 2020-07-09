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

	inputManager = new InputManager;		// ���͊Ǘ��N���X�𐶐��B�|�C���^��ۑ����Ă����B
	fontData = new FontData;				// �t�H���g�f�[�^�Ǘ��N���X�𐶐��B�|�C���^��ۑ����Ă����B

	gameManager = new GameManager(this);	// �Q�[���i�s�Ǘ��N���X�𐶐��B�|�C���^��ۑ����Ă����B

	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i] = new Block(i, fontData);	// �u���b�N�I�u�W�F�N�g�𐶐��B�|�C���^��ۑ����Ă����B
	}

	bullet = new Bullet();					// �e�I�u�W�F�N�g�𐶐��B�|�C���^��ۑ����Ă����B

	player[GameManager::RED] = new Player(GameManager::RED, 0xE71122, true, this);		// �v���C���[RED�𐶐��B�|�C���^��ۑ����Ă����B
	player[GameManager::BLUE] = new Player(GameManager::BLUE, 0x1122E7, false, this);	// �v���C���[BLUE�𐶐��B�|�C���^��ۑ����Ă����B

	return 1;
}

// �Q�[�����v���C���ȂǂɃN���X�𐶐����Ȃ���
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

// �Q�[�����[�v
void GameMain::GameLoop(void) {
	inputManager->InputKey();	// ���͂��󂯎��

	while (ProcessMessage() == 0 && (inputManager->GetPadInput()[GameManager::RED].in_Button[InputManager::BACK] == 0 &&
									inputManager->GetPadInput()[GameManager::BLUE].in_Button[InputManager::BACK] == 0) &&
									inputManager->In_Key()[KEY_INPUT_ESCAPE] == 0) {
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
	switch (gameManager->GetPhaseStatus())
	{
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
		return;
		break;
	}
	
}

// �I�u�W�F�N�g�̕`��n�֐����Ăяo��
void GameMain::Output(void) {
	// �v���C���[�`��
	player[GameManager::RED]->DrawPlayer();
	player[GameManager::BLUE]->DrawPlayer();
	
	// �u���b�N�`��
	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i]->DrawBlocks();
	}
	DrawDebugInfo();	// �f�o�b�O���`��

	switch (gameManager->GetPhaseStatus())
	{
	case GameManager::HIDE:
		// �B���t�F�[�Y���̕����`��
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "�B���I");
		break;

	case GameManager::SHOT:
		// �����t�F�[�Y�̕����`��A�����̑_���Ă�������`��
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "���āI");
		player[gameManager->GetNowShooter()]->DrawTargetAngle();
		break;

	case GameManager::RECOCHETWAIT:
		// �e�`��֐�
		if (bullet->IsAlive()) {
			bullet->DrawBullet();
			DrawFormatStringToHandle(420, 120, 0xFFFFFF, fontData->f_FontData[1], "���e�c�� %d��", bullet->GetRicochetCount());
		}
		break;

	case GameManager::RESULT:
		// ���U���g��ʕ`��
		result->DrawResult();
		return;
		break;
	}
}

// �f�o�b�O����`�悷�邽�߂̊֐�
void GameMain::DrawDebugInfo(void) {
	DrawFormatStringToHandle(0, 0, 0xFFFFFF, fontData->f_FontData[0], "%.1fFPS", mFps);
	DrawFormatStringToHandle(0, 20, 0xFFFFFF, fontData->f_FontData[0], "�B��鎞�Ԏc��%d�t��", gameManager->GetHideTime());
	DrawFormatStringToHandle(0, 40, 0xFFFFFF, fontData->f_FontData[0], "�_���鎞�Ԏc��%d�t��", gameManager->GetShotTime());
	
}