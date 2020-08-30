#include "End.h"

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
End::End(FontData* font, InputManager* inputMNG, GameManager* gameMNG) {
	fontData = font;
	inputManager = inputMNG;
	gameManager = gameMNG;
	gameMain = gameManager->gameMain;
	LoadImages();

	// �J�[�\���ʒu������
	selectNum[0] = 0;
	selectNum[1] = 0;
}

////////////////////////////////////////////////
// ����
////////////////////////////////////////////////
void End::EndControll() {
	for (int i = 0; i < 2; i++) {
		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
			// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameManager->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
			// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameManager->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// �Q�[���p�b�h1��B�{�^�����́B
			switch (selectNum[i])
			{
			case 0:
				//if (DxLib_Init() == -1) return -1;
				//DxLib_End();
				gameManager->SetPhaseStatus(GameManager::QUIT);
				this->~End();
				break;
			case 1:
				gameManager->SetPhaseStatus(GameManager::TITLE);

				gameMain->CreateTitleObj();

				this->~End();
				break;
			}
			gameManager->gameMain->PlayDicideSE();
			return;
		}
	}

	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// �L�[�{�[�h�����L�[��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// �L�[�{�[�h�����L�[���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN) == 1) {
		// �L�[�{�[�h�������
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			gameManager->SetPhaseStatus(GameManager::QUIT);
			this->~End();
			break;

		case 1:
			gameManager->SetPhaseStatus(GameManager::TITLE);
			gameMain->CreateTitleObj();
			this->~End();
			break;
		}

		gameManager->gameMain->PlayDicideSE();
	}
}

////////////////////////////////////////////////
// �`��
////////////////////////////////////////////////
void End::DrawEnd() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawGraph(0, 0, i_EndImage, TRUE);	// �w�i�`��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ��ʂ̉����S�A�@�@�@�@�@�@�@	Y���̑����ʁA�@����Y�̈ʒu
	int x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// �I�����܂����H�̕����`��
	DrawRotaGraph(x, starty - 200, 1, 0, i_MenuImage[0], 1);

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		DrawRotaGraph(x, starty + y * i, 1, 0, i_MenuImage[i + 1], 1);
	}

	for (int i = 0; i < 2; i++) {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 1.0f, 0, gameManager->gameMain->GetCursorImage(i), TRUE);
	}
}

////////////////////////////////////////////////
// �摜�ǂݍ���
////////////////////////////////////////////////
void End::LoadImages() {
	if (!(i_EndImage = LoadGraph("Image/TitleBulletBackImage.png"))) return;
	if (!(i_MenuImage[0] = LoadGraph("Image/End_jp.png"))) return;
	if (!(i_MenuImage[1] = LoadGraph("Image/EndYes_jp.png"))) return;
	if (!(i_MenuImage[2] = LoadGraph("Image/EndNo_jp.png"))) return;
}

////////////////////////////////////////////////
// �摜����
////////////////////////////////////////////////
void End::DeleteImages() {
	i_EndImage = DeleteGraph(i_EndImage);

	for (int i = 0; i < 3; i++) {
		i_MenuImage[i] = DeleteGraph(i_MenuImage[i]);
	}
}

////////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////////
End::~End() {
	DeleteImages();
}