#include "Option.h"

// �R���X�g���N�^
Option::Option(GameMain* main, int pushPLnum, int prescreennum, int prephase) {
	gameMain = main;
	inputManager = gameMain->inputManager;
	fontData = gameMain->fontData;
	LoadFile();
	BGMVolume = saveData.bgmVolume;	// BGM�̉���
	SEVolume = saveData.seVolume;	// SE�̉���
	screenNum = VOLUME;
	optionPushPLNum = pushPLnum;
	previousNum = prescreennum;
	previousGamePhase = prephase;
	selectNum[0] = 0;
	selectNum[1] = 0;
	
}

// �I�v�V������ʂ̏����֐�
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

// �I�v�V�����g�b�v��ʂ̏���
void Option::ControllOptionTop() {
	for (int i = 0; i < 2; i++) {

		if (optionPushPLNum == GameManager::BLUE + 1) break;	// �|�[�Y�{�^�����������̂��L�[�{�[�h�̃L�[����Ȃ�A�R���g���[���[�̑�����󂯕t���Ȃ��B
		if (optionPushPLNum != i) continue;					// �|�[�Y�{�^�����������v���C���[�łȂ��ꍇ�A�������X�L�b�v

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
			return;
		}
	}


	if (optionPushPLNum != GameManager::BLUE + 1) return;	// �|�[�Y�{�^�����������̂��R���g���[���[��������A�L�[�{�[�h�̏������󂯕t���Ȃ���return


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
	}
}

// ���ʒ��߉�ʂ̏���
void Option::ControllVolumeScreen() {
	for (int i = 0; i < 2; i++) {

		if (optionPushPLNum == GameManager::BLUE + 1) break;	// �|�[�Y�{�^�����������̂��L�[�{�[�h�̃L�[����Ȃ�A�R���g���[���[�̑�����󂯕t���Ȃ��B
		if (optionPushPLNum != i) continue;					// �|�[�Y�{�^�����������v���C���[�łȂ��ꍇ�A�������X�L�b�v

		if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] == 1 || inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] >= 18) {
			// �Q�[���p�b�h�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
			// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
			if (--selectNum[i] < 0) {
				selectNum[i] = VOLUME_SELECT_NUM_MAX;
			}
			if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] >= 18) {
				inputManager->GetPadInput()[i].in_Button[InputManager::PAD_UP] -= 4;
			}
		}

		if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] == 1 || inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] >= 18) {
			// �Q�[���p�b�h�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
			// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
			if (++selectNum[i] > VOLUME_SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] >= 18) {
				inputManager->GetPadInput()[i].in_Button[InputManager::PAD_DOWN] -= 4;
			}
		}

		if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_RIGHT] == 1 || inputManager->GetPadInput()[i].in_Button[InputManager::PAD_RIGHT] >= 18) {
			// �Q�[���p�b�h�̕����p�b�h�E�̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
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
			
			if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_RIGHT] >= 18) {
				inputManager->GetPadInput()[i].in_Button[InputManager::PAD_RIGHT] -= 4;
			}
		}

		if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_LEFT] == 1 || inputManager->GetPadInput()[i].in_Button[InputManager::PAD_LEFT] >= 18) {
			// �Q�[���p�b�h�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
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
			if (inputManager->GetPadInput()[i].in_Button[InputManager::PAD_LEFT] >= 18) {
				inputManager->GetPadInput()[i].in_Button[InputManager::PAD_LEFT] -= 4;
			}
		}

		if (inputManager->GetPadInput()[i].in_Button[InputManager::B] == 1) {
			// �Q�[���p�b�h1��B�{�^�����́B
			switch (selectNum[i])
			{
			case 2:
				if (previousNum == PAUSE) {
					ChangeBulletSoundVolume();
				}
				SaveFile();
				BackOptionMenu();
				break;
			}
			return;
		}
	}


	if (optionPushPLNum != GameManager::BLUE + 1) return;	// �|�[�Y�{�^�����������̂��R���g���[���[��������A�L�[�{�[�h�̏������󂯕t���Ȃ���return


	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->In_Key()[KEY_INPUT_UP] == 1 || inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = VOLUME_SELECT_NUM_MAX;
		}
		if (inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
			inputManager->In_Key()[KEY_INPUT_UP] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_DOWN] == 1 || inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > VOLUME_SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		if (inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
			inputManager->In_Key()[KEY_INPUT_DOWN] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_RIGHT] == 1 || inputManager->In_Key()[KEY_INPUT_RIGHT] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
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
		if (inputManager->In_Key()[KEY_INPUT_RIGHT] >= 18) {
			inputManager->In_Key()[KEY_INPUT_RIGHT] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_LEFT] == 1 || inputManager->In_Key()[KEY_INPUT_LEFT] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
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
		if (inputManager->In_Key()[KEY_INPUT_LEFT] >= 18) {
			inputManager->In_Key()[KEY_INPUT_LEFT] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_F] == 1 || inputManager->In_Key()[KEY_INPUT_RETURN] == 1) {
		// �Q�[���p�b�h1��B�{�^�����́B
		switch (selectNum[GameManager::BLUE])
		{
		case 2:
			if (previousNum == PAUSE) {
				ChangeBulletSoundVolume();
			}
			SaveFile();
			BackOptionMenu();
			break;
		}
	}
}

// �^�C�g����ʂ֖߂�
void Option::Return_to_Title() {
	gameMain->gameManager->SetPhaseStatus(GameManager::TITLE);
	gameMain->CreateTitleObj();
	this->~Option();
}

// �|�[�Y��ʂ֖߂�
void Option::Return_to_PauseScreen() {
	gameMain->gameManager->SetPhaseStatus(previousGamePhase);
	gameMain->CreatePauseScreenObj();
	this->~Option();
}

// �I�v�V������ʂ̕`��
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

// �I�v�V������ʂ̃g�b�v��`��
void Option::DrawOptionMain() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// �w�i���F�œh��Ԃ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �����̕��A		��ʂ̉����S�A�@�@�@�@�@�@�@	Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// Pause�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Option");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "Option");

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	// �|�[�Y��ʉ������̂��R���g���[���[�Ȃ�Ԃ����A�L�[�{�[�h�Ȃ�K����\��������
	if (optionPushPLNum <= GameManager::BLUE) {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * optionPushPLNum), starty + y * selectNum[optionPushPLNum], 10, COLOR_VALUE_PLAYER[optionPushPLNum], 1, 1);
	}
	else if (optionPushPLNum == GameManager::BLUE + 1) {
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * GameManager::BLUE), starty + y * selectNum[GameManager::BLUE], 10, COLOR_VALUE_PLAYER[GameManager::BLUE], 1, 1);
	}
}

// ���ʒ��߉�ʂ�`��
void Option::DrawVolumeMenu() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// �w�i���F�œh��Ԃ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �����̕��A		��ʂ̉����S�A�@�@�@�@�@�@�@	Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400, extraX = 300, extraFlg = 1;

	// Pause�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Volume");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "Volume");

	// �|�[�Y��ʉ������̂��R���g���[���[�Ȃ�Ԃ����A�L�[�{�[�h�Ȃ�K����\��������
	if (optionPushPLNum <= GameManager::BLUE && !(selectNum[optionPushPLNum] < VOLUME_SELECT_NUM_MAX)) {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * optionPushPLNum), starty - 100 + y * selectNum[optionPushPLNum], 10, COLOR_VALUE_PLAYER[optionPushPLNum], 1, 1);
	}
	else if (optionPushPLNum == GameManager::BLUE + 1 && !(selectNum[GameManager::BLUE] < VOLUME_SELECT_NUM_MAX)) {
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * GameManager::BLUE), starty - 100 + y * selectNum[GameManager::BLUE], 10, COLOR_VALUE_PLAYER[GameManager::BLUE], 1, 1);
	}
	else if (optionPushPLNum <= GameManager::BLUE) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", VolumeMenuName[selectNum[optionPushPLNum]].c_str());
		DrawBox(x - fontwidth / 2 - extraX, starty - 130 + y * selectNum[optionPushPLNum], x + fontwidth / 2 - extraX, starty - 130 + y * selectNum[optionPushPLNum] + y - 10, COLOR_VALUE_PLAYER[optionPushPLNum], 1);
	}
	else if (optionPushPLNum == GameManager::BLUE + 1) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", VolumeMenuName[selectNum[GameManager::BLUE]].c_str());
		DrawBox(x - fontwidth / 2 - extraX, starty - 130 + y * selectNum[GameManager::BLUE], x + fontwidth / 2 - extraX, starty - 130 + y * selectNum[GameManager::BLUE] + y - 10, COLOR_VALUE_PLAYER[GameManager::BLUE], 1);
	}

	// �e���ږ��`��
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

// �e���߉�ʂ���g�b�v�ɖ߂邽�߂̊֐�
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

// �e�̉��ʒ���
void Option::ChangeBulletSoundVolume() {
	gameMain->bullet->ChangeVolume(SEVolume);
}

// �Z�[�u�f�[�^�Ǎ���
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

// �Z�[�u�f�[�^�ۑ�
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

// �f�X�g���N�^
Option::~Option() {

}