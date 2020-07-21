#include "PauseScreen.h"

// �R���X�g���N�^
PauseScreen::PauseScreen(FontData* font, InputManager* input, GameMain* main, int pushPLnum) {
	// �����Ŏ󂯎�����|�C���^�����[�J���ϐ��ɃR�s�[
	fontData = font;
	inputManager = input;
	gameMain = main;
	pausePushPLNum = pushPLnum;

	// �J�[�\���ʒu������
	selectNum[0] = 0;
	selectNum[1] = 0;
}

// �|�[�Y��ʏ���
void PauseScreen::PauseScreenControll() {
	for (int i = 0; i < 2; i++) {
		if (pausePushPLNum == GameManager::BLUE + 1) break;	// �|�[�Y�{�^�����������̂��L�[�{�[�h�̃L�[����Ȃ�A�R���g���[���[�̑�����󂯕t���Ȃ��B
		if (pausePushPLNum != i) continue;					// �|�[�Y�{�^�����������v���C���[�łȂ��ꍇ�A�������X�L�b�v
		if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] == 1 || inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] >= 18) {
			// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
			// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] >= 18) {
				inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] -= 4;
			}
		}

		if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] == 1 || inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] >= 18) {
			// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
			// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] >= 18) {
				inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] -= 4;
			}
		}

		if (inputManager->GetPadInput()[i].in_Button[InputManager::B] == 1) {
			// �Q�[���p�b�h1��B�{�^�����́B
			switch (selectNum[i])
			{
			case 0:
				Return_to_Game();
				break;
			case 1:
				break;
			case 2:
				Return_to_Title();
				break;
			}
			return;
		}
	}


	if (pausePushPLNum != GameManager::BLUE + 1) return;	// �|�[�Y�{�^�����������̂��R���g���[���[��������A�L�[�{�[�h�̏������󂯕t���Ȃ���return


	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->In_Key()[KEY_INPUT_UP] == 1 || inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		if (inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
			inputManager->In_Key()[KEY_INPUT_UP] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_DOWN] == 1 || inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		if (inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
			inputManager->In_Key()[KEY_INPUT_DOWN] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_F] == 1 || inputManager->In_Key()[KEY_INPUT_RETURN] == 1) {
		// �Q�[���p�b�h1��B�{�^�����́B
		switch (selectNum[GameManager::BLUE])
		{
		case 0:
			Return_to_Game();
			break;
		case 1:
			break;
		case 2:
			Return_to_Title();
			break;
		}
	}
}

// �|�[�Y��ʕ`��
void PauseScreen::DrawPauseScreen() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// �w�i���F�œh��Ԃ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �����̕��A		��ʂ̉����S�A�@�@�@�@�@�@�@	Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	// Pause�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Pause");
	DrawFormatStringToHandle(x - fontwidth / 2, GameMain::SCREEN_HEIGHT / 4 - fontwidth, 0xFFFFFF, fontData->f_FontData[1], "Pause");

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	// �|�[�Y��ʉ������̂��R���g���[���[�Ȃ�Ԃ����A�L�[�{�[�h�Ȃ�K����\��������
	if (pausePushPLNum <= GameManager::BLUE) {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * pausePushPLNum), starty + y * selectNum[pausePushPLNum], 10, COLOR_VALUE_PLAYER[pausePushPLNum], 1, 1);
	}
	else if (pausePushPLNum == GameManager::BLUE + 1) {
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * GameManager::BLUE), starty + y * selectNum[GameManager::BLUE], 10, COLOR_VALUE_PLAYER[GameManager::BLUE], 1, 1);
	}
}

// �|�[�Y��ʂ𔲂��Ď������ĊJ����
void PauseScreen::Return_to_Game() {
	gameMain->SetPauseFlg(false);
}

// �I�v�V������ʂ��J��
void PauseScreen::OpenOptionScreen() {

}

// �����𒆒f���ă^�C�g����ʂ֖߂�
void PauseScreen::Return_to_Title() {
	gameMain->SetPauseFlg(false);
	gameMain->gameManager->SetPhaseStatus(GameManager::TITLE);
}

// �f�X�g���N�^
PauseScreen::~PauseScreen() {

}