#include "DifficultySelectScene.h"

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
DifficultySelectScene::DifficultySelectScene(InputManager* input, FontData* font, GameManager* gameMNG) {
	inputManager = input;
	fontData = font;
	gameMangaer = gameMNG;
	waitTime = 0;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

}

////////////////////////////////////////////////
// ��Փx�I����ʂ̏���
////////////////////////////////////////////////
void DifficultySelectScene::DifficultySelectControll() {
	for (int i = 0; i < 2; i++) {
		
		// A�{�^�������������A���łɓ�Փx�����肵�Ă������������
		if (dicideNumFlg[i]) {
			if (inputManager->GetButtonDown(A, i)) {
				dicideNumFlg[i] = false;
				gameMangaer->gameMain->PlayCanselSE();
			}
			continue;
		}

		// ��Փx������̏ꍇ�́A�^�C�g����ʂ֖߂�
		if (inputManager->GetButtonDown(A, i)) {
			returnFlg = true;		// ���̃t���O�́AgameMain���Q�Ƃ��Ă���Btrue�Ȃ�A��Փx�I����ʃN���X��delete�����B
			gameMangaer->gameMain->CreateTitleObj();		// �^�C�g����ʂ����APhase���^�C�g���ɂ���
			gameMangaer->gameMain->PlayCanselSE();
		}

		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
			// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameMangaer->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
			// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
			gameMangaer->gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// �Q�[���p�b�h1��B�{�^�����́B
			dicideNumFlg[i] = true;
			gameMangaer->gameMain->PlayDicideSE();
		}
	}

	// �L�[�{�[�h��ESC�����������A��Փx�����肵�Ă��Ȃ�������^�C�g����ʂ֖߂�
	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
		returnFlg = true;
		gameMangaer->gameMain->CreateTitleObj();
		gameMangaer->gameMain->PlayCanselSE();
	}

	// �L�[�{�[�h��ESC�����������A��Փx�����肵�Ă�����A���̌�����L�����Z������
	if (dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_ESCAPE)) {
		dicideNumFlg[GameManager::BLUE] = false;
		gameMangaer->gameMain->PlayCanselSE();
	}


	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4))) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4))) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN))) {
		// �Q�[���p�b�h1��B�{�^�����́B
		dicideNumFlg[GameManager::BLUE] = true;
		gameMangaer->gameMain->PlayDicideSE();
		return;
	}

	// �L�[�{�[�h�����������Ă����ꍇ�ARED��BLUE�������I�ɓ������ڂ�I���������Ƃɂ��āA������i�߂�
	if (inputManager->In_Key()[KEY_INPUT_RETURN] >= 30) {
		inputManager->In_Key()[KEY_INPUT_RETURN] = 0;
		selectNum[GameManager::RED] = selectNum[GameManager::BLUE];
		dicideNumFlg[GameManager::RED] = true;
		gameMangaer->gameMain->PlayDicideSE();
	}

	// �ǂ�������ڂ����肵�Ă�����A�V�[���J�ڂ�����
	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {

		// ���������݂��̍��ڔԍ����������A�I�ђ�������B
		if (selectNum[GameManager::RED] != selectNum[GameManager::BLUE]) {
			for (int i = 0; i < 2; i++) {
				selectNum[i] = 0;
				dicideNumFlg[i] = false;
			}
			gameMangaer->gameMain->PlayCanselSE();
			return;
		}
		else {

			// �����҂��Ă���J�ڂ���
			if (!(SCENE_TRANSITION_WAITING_TIME < ++waitTime))  return;
			waitTime = 0;
			SetDifficulty();
			gameMangaer->gameMain->Init();
			returnFlg = true;
		}
	}
}

////////////////////////////////////////////////
// ��Փx�I����ʂ̏����E�l�b�g�ΐ�p
////////////////////////////////////////////////
void DifficultySelectScene::DifficultySelectControll_Net() {

	if (!dicideNumFlg[GameManager::RED] && inputManager->GetButtonDown(A, GameManager::RED)) {
		// �Q�[���p�b�h1��B�{�^�����́B
		returnFlg = true;
		gameMangaer->gameMain->CreateTitleObj();
		gameMangaer->gameMain->PlayCanselSE();
		return;
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetButtonDown(PAD_UP, GameManager::RED) || inputManager->GetButtonHold(PAD_UP, GameManager::RED, 4))) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetButtonDown(PAD_DOWN, GameManager::RED) || inputManager->GetButtonHold(PAD_DOWN, GameManager::RED, 4))) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && inputManager->GetButtonDown(B, GameManager::RED)) {
		// �Q�[���p�b�h1��B�{�^�����́B
		dicideNumFlg[GameManager::RED] = true;
		gameMangaer->gameMain->PlayDicideSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_ESCAPE))) {
		// �Q�[���p�b�h1��B�{�^�����́B
		returnFlg = true;
		gameMangaer->gameMain->CreateTitleObj();
		gameMangaer->gameMain->PlayCanselSE();
		return;
	}

	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4))) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4))) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN))) {
		// �Q�[���p�b�h1��B�{�^�����́B
		dicideNumFlg[GameManager::RED] = true;
		gameMangaer->gameMain->PlayDicideSE();
		return;
	}

	// �ǂ�������ڂ����肵�Ă�����A�V�[���J�ڂ�����
	if (dicideNumFlg[GameManager::RED]) {
		// �����҂��Ă���J�ڂ���
		if (!(SCENE_TRANSITION_WAITING_TIME < ++waitTime))  return;
		waitTime = 0;
		switch (selectNum[GameManager::RED])
		{
		case 0:
			gameMangaer->SetDifficulty(GameManager::CASUAL);
			break;
		case 1:
			gameMangaer->SetDifficulty(GameManager::EXPERT);
			break;
		}

		// �l�b�g�ΐ�p�Ȃ̂ŁA��Փx�����AIP�A�h���X�I����ʂ֑J�ڂ���
		gameMangaer->SetPhaseStatus(GameManager::IPADDRESS_SELECT);
		gameMangaer->gameMain->network->VariableInit();	// �l�b�g���[�N�̕ϐ�������
		returnFlg = true;
	}
}

////////////////////////////////////////////////
// ��Փx�I����ʂ̕`�揈��
////////////////////////////////////////////////
void DifficultySelectScene::DrawDifficultySelectScene() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 60 * (60-waitTime));
	}

	// DIFFICULTYSELECT�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Mode Select");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "Mode Select");

	// ��Փx��I��ł��������̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "��Փx��I��ł�������");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "��Փx��I��ł�������");
	

	for (int i = 0; i < 2; i++) {

		// ���ڂ����肵�Ă�����A�����l�p��\������
		if (dicideNumFlg[i]) {
			if (i == GameManager::RED) {
				DrawBox(0, starty + y * selectNum[i] - 15, GameMain::SCREEN_WIDTH / 2, starty + y * selectNum[i] + 15, COLOR_VALUE_PLAYER[i], 1);
			}
			else {
				DrawBox(GameMain::SCREEN_WIDTH, starty + y * selectNum[i] - 15, GameMain::SCREEN_WIDTH / 2, starty + y * selectNum[i] + 15, COLOR_VALUE_PLAYER[i], 1);
			}
		}
		else {
			// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
			DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 1.0f, 0, gameMangaer->gameMain->GetCursorImage(i), TRUE);
		}

	}

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}

////////////////////////////////////////////////
// ��Փx�I����ʂ̕`�揈���E�l�b�g�p
////////////////////////////////////////////////
void DifficultySelectScene::DrawDifficultySelectScene_Net() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	// DIFFICULTYSELECT�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Mode Select");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "Mode Select");

	// ��Փx��I��ł��������̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "��Փx��I��ł�������");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "��Փx��I��ł�������");

	// �l�b�g�ΐ�̂݁B
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "������Փx��I�������l���m�ŗV�т܂�");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 135, 0xFFFFFF, fontData->f_FontData[1], "������Փx��I�������l���m�ŗV�т܂�");

	// ���ڂ����肵�Ă�����A�����l�p��\������
	if (dicideNumFlg[GameManager::RED]) {
		DrawBox(0, starty + y * selectNum[GameManager::RED] - 15, GameMain::SCREEN_WIDTH, starty + y * selectNum[GameManager::RED] + 15, COLOR_VALUE_PLAYER[GameManager::RED], 1);
	}
	else {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * 0), starty + y * selectNum[0], 1.0f, 0, gameMangaer->gameMain->GetCursorImage(0), TRUE);
	}

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

// ��Փx��GameManager�̕ϐ��ɃZ�b�g���ăV�[����J��
void DifficultySelectScene::SetDifficulty() {

	switch (selectNum[GameManager::BLUE])
	{
	case 0:
		gameMangaer->SetDifficulty(GameManager::CASUAL);
		break;
	case 1:
		gameMangaer->SetDifficulty(GameManager::EXPERT);
		break;
	}
	
}

////////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////////
DifficultySelectScene::~DifficultySelectScene() {

}