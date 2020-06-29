#include "GameMain.h"

GameMain::GameMain(void) {
	SCREEN_WIDTH_HALF = SCREEN_WIDTH / 2;
	SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;
	main = this;
}

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
		FPSUpdate();	//FPS�X�V

		SetDrawScreen(offscreen_handle);
		ClearDrawScreen();

		inputManager->InputKey();	// ���͂��󂯎��
		Update();
		Output();

		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, DRAW_SCREEN_WIDTH, DRAW_SCREEN_HEIGHT, offscreen_handle, FALSE);	// ��ʂ�`��p�̑傫���Ɉ����L�΂��ĕ`�悷��
		ScreenFlip();

		UpdateWait();		//FPS�ҋ@
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
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "�B���I");
		break;

	case GameManager::SHOT:
		DrawFormatStringToHandle(500, 120, 0xFFFFFF, fontData->f_FontData[1], "���āI");
		player[gameManager->GetNowShooter()]->DrawTargetAngle();
		break;

	case GameManager::RECOCHETWAIT:
		if (bullet->IsAlive()) {
			bullet->DrawBullet();
			DrawFormatStringToHandle(420, 120, 0xFFFFFF, fontData->f_FontData[1], "���e�c�� %d��", bullet->GetRicochetCount());
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
	DrawFormatStringToHandle(0, 20, 0xFFFFFF, fontData->f_FontData[0], "�B��鎞�Ԏc��%d�t��", gameManager->GetHideTime());
	DrawFormatStringToHandle(0, 40, 0xFFFFFF, fontData->f_FontData[0], "�_���鎞�Ԏc��%d�t��", gameManager->GetShotTime());
	//for (int i = 0; i < 2; i++) {
	//	DrawFormatStringToHandle(0 + i * 200, 60, 0xFFFFFF, fontData->f_FontData[0], "%d %dP��x", player[i]->x, i+1);
	//	DrawFormatStringToHandle(0 + i * 200, 80, 0xFFFFFF, fontData->[0], "%d %dP��y", player[i]->y, i+1);
	//	DrawFormatStringToHandle(0 + i * 200, 100, 0xFFFFFF, fontData->f_FontData[0], "%d %dP��tarx", player[i]->targetx, i + 1);
	//	DrawFormatStringToHandle(0 + i * 200, 120, 0xFFFFFF, fontData->f_FontData[0], "%d %dP��tary", player[i]->targety, i + 1);
	//	DrawFormatStringToHandle(0 + i * 200, 140, 0xFFFFFF, fontData->f_FontData[0], "%d %dP��angle", player[i]->angle, i + 1);
	//}
	//DrawFormatStringToHandle(0, 160, 0xFFFFFF, fontData->f_FontData[0], "�e�̈ʒuX %f", bullet->x);
	//DrawFormatStringToHandle(0, 180, 0xFFFFFF, fontData->f_FontData[0], "�e�̈ʒuY %f", bullet->y);
	//DrawFormatStringToHandle(0, 200, 0xFFFFFF, fontData->f_FontData[0], "�e�̐i�sX %f.5", cos(bullet->angle * DX_PI / 180) * g_Bullet.movespeedx);
	//DrawFormatStringToHandle(0, 220, 0xFFFFFF, fontData->f_FontData[0], "�e�̐i�sY %f.5", sin(bullet->angle * DX_PI / 180) * g_Bullet.movespeedy);
}