#include "DifficultySelectScene.h"

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
DifficultySelectScene::DifficultySelectScene(InputManager* input, FontData* font, GameManager* gameMNG) {
	inputManager = input;
	fontData = font;
	gameMangaer = gameMNG;

	// �ϐ�������
	waitTime = 0;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

	LoadImages();
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
		// �L�[�{�[�h�����L�[��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4))) {
		// �L�[�{�[�h�����L�[���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN))) {
		// �L�[�{�[�h�������
		dicideNumFlg[GameManager::BLUE] = true;
		gameMangaer->gameMain->PlayDicideSE();
		return;
	}

	// �L�[�{�[�h�����������Ă����ꍇ�ARED��BLUE�������I�ɓ������ڂ�I���������Ƃɂ��āA������i�߂�
	if (!(dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) && inputManager->In_Key()[KEY_INPUT_RETURN] >= 30) {
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
		// �L�[�{�[�h�G�X�P�[�v�L�[����
		returnFlg = true;
		gameMangaer->gameMain->CreateTitleObj();
		gameMangaer->gameMain->PlayCanselSE();
		return;
	}

	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4))) {
		// �L�[�{�[�h�����L�[��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4))) {
		// �L�[�{�[�h�����L�[���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameMangaer->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::RED] && (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN))) {
		// �L�[�{�[�h�������
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
	// ��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 60 * (60-waitTime));
	}

	DrawGraph(0, 0, i_BackImage, TRUE);		// �w�i�摜�`��

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 60 * (60 - waitTime));

	DrawRotaGraph(x, starty - 300, 1, 0, i_MenuImage[0], 1);	// DIFFICULTYSELECT�̕����`��
	DrawRotaGraph(x, starty - 200, 1, 0, i_MenuImage[1], 1);	// ��Փx��I��ł��������̕����`��

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
		DrawRotaGraph(x, starty + y * i, 1, 0, i_MenuImage[i + 3], 1);
	}

	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}

////////////////////////////////////////////////
// ��Փx�I����ʂ̕`�揈���E�l�b�g�p
////////////////////////////////////////////////
void DifficultySelectScene::DrawDifficultySelectScene_Net() {
	// ��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	DrawGraph(0, 0, i_BackImage, TRUE);		// �w�i�摜�`��

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawRotaGraph(x, starty - 300, 1, 0, i_MenuImage[0], 1);	// DIFFICULTYSELECT�̕����`��
	DrawRotaGraph(x, starty - 200, 1, 0, i_MenuImage[1], 1);	// ��Փx��I��ł��������̕����`��
	DrawRotaGraph(x, starty - 135, 1, 0, i_MenuImage[2], 1);	// �l�b�g�ΐ�̂݁B

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
		DrawRotaGraph(x, starty + y * i, 1, 0, i_MenuImage[i + 3], 1);
	}
}
////////////////////////////////////////////////
// ��Փx��GameManager�̕ϐ��ɃZ�b�g���ăV�[����J��
////////////////////////////////////////////////
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
// �摜�ǂݍ���
////////////////////////////////////////////////
void DifficultySelectScene::LoadImages() {
	if (!(i_BackImage = LoadGraph("Image/SelectBackImage.png"))) return;
	if (!(i_MenuImage[0] = LoadGraph("Image/ModeSelect.png"))) return;
	if (!(i_MenuImage[1] = LoadGraph("Image/ModeSelect_jp.png"))) return;
	if (!(i_MenuImage[2] = LoadGraph("Image/SameMode.png"))) return;
	if (!(i_MenuImage[3] = LoadGraph("Image/Casual.png"))) return;
	if (!(i_MenuImage[4] = LoadGraph("Image/Expert.png"))) return;
}

////////////////////////////////////////////////
// �摜�f�[�^�������������
////////////////////////////////////////////////
void DifficultySelectScene::DeleteImages() {
	i_BackImage = DeleteGraph(i_BackImage);
	i_MenuImage[0] = DeleteGraph(i_MenuImage[0]);
	i_MenuImage[1] = DeleteGraph(i_MenuImage[1]);
	i_MenuImage[2] = DeleteGraph(i_MenuImage[2]);
	i_MenuImage[3] = DeleteGraph(i_MenuImage[3]);
	i_MenuImage[4] = DeleteGraph(i_MenuImage[4]);
}

////////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////////
DifficultySelectScene::~DifficultySelectScene() {
	DeleteImages();
}