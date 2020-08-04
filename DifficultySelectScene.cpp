#include "DifficultySelectScene.h"

// �R���X�g���N�^
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

// ��Փx�I����ʂ̏���
void DifficultySelectScene::DifficultySelectControll() {
	for (int i = 0; i < 2; i++) {

		if (dicideNumFlg[i]) continue;

		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
			// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
			// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;
			}
		}

		if (inputManager->GetButtonDown(B, i)) {
			// �Q�[���p�b�h1��B�{�^�����́B
			dicideNumFlg[i] = true;

		}
	}


	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
	}

	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
	}

	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN) == 1) {
		// �Q�[���p�b�h1��B�{�^�����́B
		dicideNumFlg[GameManager::BLUE] = true;
		return;
	}

	if (inputManager->In_Key()[KEY_INPUT_RETURN] >= 30) {
		inputManager->In_Key()[KEY_INPUT_DOWN] = 0;
		SetDifficulty();
		gameMangaer->gameMain->Init();
		gameMangaer->gameMain->diffiSelectScene = NULL;
		this->~DifficultySelectScene();
		return;
	}

	// �ǂ�������ڂ����肵�Ă�����A�V�[���J�ڂ�����
	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {

		// ���������݂��̍��ڔԍ����������A�I�ђ�������B
		if (selectNum[GameManager::RED] != selectNum[GameManager::BLUE]) {
			for (int i = 0; i < 2; i++) {
				selectNum[i] = 0;
				dicideNumFlg[i] = false;
			}
			return;
		}
		else {

			// �����҂��Ă���J�ڂ���
			if (!(SCENE_TRANSITION_WAITING_TIME < ++waitTime))  return;
			waitTime = 0;
			SetDifficulty();
			gameMangaer->gameMain->Init();
			gameMangaer->gameMain->diffiSelectScene = NULL;
			this->~DifficultySelectScene();
		}
	}
}

// ��Փx�I����ʂ̕`�揈��
void DifficultySelectScene::DrawDifficultySelectScene() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

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
			DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 10, COLOR_VALUE_PLAYER[i], 1, 1);
		}

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

	gameMangaer->SetPhaseStatus(GameManager::HIDE);
}

// �f�X�g���N�^
DifficultySelectScene::~DifficultySelectScene() {

}