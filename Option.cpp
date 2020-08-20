#include "Option.h"

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
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
	
	LoadImages();
}

////////////////////////////////////////////////
// �I�v�V������ʂ̏����֐�
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// �I�v�V�����g�b�v��ʂ̏���
////////////////////////////////////////////////
void Option::ControllOptionTop() {
	for (int i = 0; i < 2; i++) {

		if (optionPushPLNum == GameManager::BLUE + 1) break;	// �|�[�Y�{�^�����������̂��L�[�{�[�h�̃L�[����Ȃ�A�R���g���[���[�̑�����󂯕t���Ȃ��B
		if (optionPushPLNum != i) continue;					// �|�[�Y�{�^�����������v���C���[�łȂ��ꍇ�A�������X�L�b�v

		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
			// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
			// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
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
			gameMain->PlayDicideSE();
			return;
		}
	}


	if (optionPushPLNum != GameManager::BLUE + 1) return;	// �|�[�Y�{�^�����������̂��R���g���[���[��������A�L�[�{�[�h�̏������󂯕t���Ȃ���return


	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
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
		gameMain->PlayDicideSE();
	}
}

////////////////////////////////////////////////
// ���ʒ��߉�ʂ̏���
////////////////////////////////////////////////
void Option::ControllVolumeScreen() {
	for (int i = 0; i < 2; i++) {

		if (optionPushPLNum == GameManager::BLUE + 1) break;	// �|�[�Y�{�^�����������̂��L�[�{�[�h�̃L�[����Ȃ�A�R���g���[���[�̑�����󂯕t���Ȃ��B
		if (optionPushPLNum != i) continue;					// �|�[�Y�{�^�����������v���C���[�łȂ��ꍇ�A�������X�L�b�v

		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// �Q�[���p�b�h�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
			// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
			if (--selectNum[i] < 0) {
				selectNum[i] = VOLUME_SELECT_NUM_MAX;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// �Q�[���p�b�h�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
			// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
			if (++selectNum[i] > VOLUME_SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_RIGHT, i) || inputManager->GetButtonHold(PAD_RIGHT, i, 4)) {
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
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_LEFT, i) || inputManager->GetButtonHold(PAD_LEFT, i, 4)) {
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
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// �Q�[���p�b�h1��B�{�^�����́B
			switch (selectNum[i])
			{
			case 2:
				if (previousNum == PAUSE) {
					ChangeBulletSoundVolume();
				}
				gameMain->ChangeVolume(BGMVolume, SEVolume);
				gameMain->gameManager->ChangeVolume(SEVolume);
				SaveFile();
				BackOptionMenu();
				break;
			}
			gameMain->PlayDicideSE();
			return;
		}
	}


	if (optionPushPLNum != GameManager::BLUE + 1) return;	// �|�[�Y�{�^�����������̂��R���g���[���[��������A�L�[�{�[�h�̏������󂯕t���Ȃ���return


	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = VOLUME_SELECT_NUM_MAX;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > VOLUME_SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_RIGHT) || inputManager->GetKeyHold(KEY_INPUT_RIGHT, 4)) {
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
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_LEFT) || inputManager->GetKeyHold(KEY_INPUT_LEFT, 4)) {
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
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// �Q�[���p�b�h1��B�{�^�����́B
		switch (selectNum[GameManager::BLUE])
		{
		case 2:
			if (previousNum == PAUSE) {
				ChangeBulletSoundVolume();
			}
			gameMain->ChangeVolume(BGMVolume, SEVolume);
			gameMain->gameManager->ChangeVolume(SEVolume);
			SaveFile();
			BackOptionMenu();
			break;
		}
		gameMain->PlayDicideSE();
	}
}

////////////////////////////////////////////////
// �^�C�g����ʂ֖߂�
////////////////////////////////////////////////
void Option::Return_to_Title() {
	gameMain->gameManager->SetPhaseStatus(GameManager::TITLE);
	gameMain->CreateTitleObj();
	this->~Option();
}

////////////////////////////////////////////////
// �|�[�Y��ʂ֖߂�
////////////////////////////////////////////////
void Option::Return_to_PauseScreen() {
	gameMain->gameManager->SetPhaseStatus(previousGamePhase);
	gameMain->CreatePauseScreenObj();
	this->~Option();
}

////////////////////////////////////////////////
// �I�v�V������ʂ̕`��
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// �I�v�V������ʂ̃g�b�v��`��
////////////////////////////////////////////////
void Option::DrawOptionMain() {
	DrawBackGroundImage();

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

////////////////////////////////////////////////
// ���ʒ��߉�ʂ�`��
////////////////////////////////////////////////
void Option::DrawVolumeMenu() {
	DrawBackGroundImage();

	// �����̕��A		��ʂ̉����S�A�@�@�@�@�@�@�@	Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400, extraX = 300, extraFlg = 1;

	// Pause�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Volume");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "Volume");

	// �|�[�Y��ʉ������̂��R���g���[���[�Ȃ�Ԃ����A�L�[�{�[�h�Ȃ�K����\��������
	if (optionPushPLNum <= GameManager::BLUE && !(selectNum[optionPushPLNum] < VOLUME_SELECT_NUM_MAX)) {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * optionPushPLNum), starty - 100 + y * selectNum[optionPushPLNum], 1.0f, 0, gameMain->GetCursorImage(optionPushPLNum), TRUE);
	}
	else if (optionPushPLNum == GameManager::BLUE + 1 && !(selectNum[GameManager::BLUE] < VOLUME_SELECT_NUM_MAX)) {
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * GameManager::BLUE), starty - 100 + y * selectNum[GameManager::BLUE], 1.0f, 0, gameMain->GetCursorImage(GameManager::BLUE), TRUE);
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

////////////////////////////////////////////////
// �e���߉�ʂ���g�b�v�ɖ߂邽�߂̊֐�
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// �e�̉��ʒ���
////////////////////////////////////////////////
void Option::ChangeBulletSoundVolume() {
	gameMain->bullet->ChangeVolume(SEVolume);
	gameMain->ChangeVolume(BGMVolume, SEVolume);
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

////////////////////////////////////////////////
// �Z�[�u�f�[�^�ۑ�
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// �w�i�摜�`��
////////////////////////////////////////////////
void Option::DrawBackGroundImage() {
	DrawGraph(0, 0, i_BackImage, TRUE);		// �w�i�摜�`��

	// �ォ�甼�����̍����l�p�����Ԃ��Ė��邳�������Ă���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

////////////////////////////////////////////////
// �摜�ǂݍ���
////////////////////////////////////////////////
void Option::LoadImages() {
	if (!(i_BackImage = LoadGraph("Image/SelectBackImage.png"))) return;
}

////////////////////////////////////////////////
// �摜�f�[�^�������������
////////////////////////////////////////////////
void Option::DeleteImages() {
	i_BackImage = DeleteGraph(i_BackImage);
}

////////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////////
Option::~Option() {
	DeleteImages();
}