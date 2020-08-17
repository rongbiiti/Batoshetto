#include "Result.h"

////////////////////////////////////////////////
// �t�H���g�f�[�^�Ǘ��I�u�W�F�N�g�̃|�C���^�A���͊Ǘ��I�u�W�F�N�g�̃|�C���^�A
// �Q�[���i�s�Ǘ��I�u�W�F�N�g�̃|�C���^�A�q�b�g�����v���C���[��RED��BLUE�����󂯎��B
////////////////////////////////////////////////
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	this->hitPlayerNumber = hitplayernum;
	waitTime = 0;
	timeOutFlg = FALSE;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

	LoadImages();
}

////////////////////////////////////////////////
// �ʐM�ΐ�ŁA�^�C���A�E�g���N�����Ƃ�
////////////////////////////////////////////////
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	waitTime = 0;
	timeOutFlg = TRUE;
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		selectNum[i] = 0;
	}

	for (int i = 0; i < 2; i++) {
		dicideNumFlg[i] = false;
	}

	LoadImages();
}

void Result::ResultControll(void) {
	for (int i = 0; i < 2; i++) {
		
		if (dicideNumFlg[i]) continue;

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
			dicideNumFlg[i] = true;
			gameManager->gameMain->PlayDicideSE();
		}
	}


	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
		
	}

	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (!dicideNumFlg[GameManager::BLUE] && inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// �Q�[���p�b�h1��B�{�^�����́B
		dicideNumFlg[GameManager::BLUE] = true;
		gameManager->gameMain->PlayDicideSE();
		return;
	}

	// �L�[�{�[�h�����������Ă����ꍇ�ARED��BLUE�������I�ɓ������ڂ�I���������Ƃɂ��āA������i�߂�
	if (inputManager->In_Key()[KEY_INPUT_RETURN] >= 30) {
		inputManager->In_Key()[KEY_INPUT_RETURN] = 0;
		selectNum[GameManager::RED] = selectNum[GameManager::BLUE];
		dicideNumFlg[GameManager::RED] = true;
		gameManager->gameMain->PlayDicideSE();
	}

	// �ǂ�������ڂ����肵�Ă�����A�V�[���J�ڂ�����
	if (dicideNumFlg[GameManager::RED] && dicideNumFlg[GameManager::BLUE]) {

		// ���������݂��̍��ڔԍ����������A�I�ђ�������B
		if (selectNum[GameManager::RED] != selectNum[GameManager::BLUE]) {
			for (int i = 0; i < 2; i++) {
				selectNum[i] = 0;
				dicideNumFlg[i] = false;
				gameManager->gameMain->PlayCanselSE();
			}
			return;
		}
		else {
			// �V�[���J��
			SceneTransition();
		}
	}

}

////////////////////////////////////////////////
// ���U���g��ʑ���F�l�b�g�p
////////////////////////////////////////////////
void Result::ResultControll_Net(void) {

	// �ǂ�������ڂ����肵�Ă�����A�V�[���J�ڂ�����
	if (dicideNumFlg[GameManager::RED]) {
		// �V�[���J��
		SceneTransition();
	}


	if (dicideNumFlg[GameManager::RED]) return;


	if (inputManager->GetButtonDown(PAD_UP, GameManager::RED) || inputManager->GetButtonHold(PAD_UP, GameManager::RED, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetButtonDown(PAD_DOWN, 0) || inputManager->GetButtonHold(PAD_DOWN, 0, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetButtonDown(B, GameManager::RED)) {
		// �Q�[���p�b�h1��B�{�^�����́B
		dicideNumFlg[GameManager::RED] = true;
		gameManager->gameMain->PlayDicideSE();
	}


	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::RED] < 0) {
			selectNum[GameManager::RED] = SELECT_NUM_MAX;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::RED] > SELECT_NUM_MAX) {
			selectNum[GameManager::RED] = 0;
		}
		gameManager->gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// �Q�[���p�b�h1��B�{�^�����́B
		dicideNumFlg[GameManager::RED] = true;
		gameManager->gameMain->PlayDicideSE();
		return;
	}
	
}

////////////////////////////////////////////////
// �V�[���J��
////////////////////////////////////////////////
void Result::SceneTransition() {
	// �����҂��Ă���J�ڂ���
	if (!(SCENE_TRANSITION_WAITING_TIME < ++waitTime))  return;
	waitTime = 0;
	switch (selectNum[GameManager::RED])
	{
	case 0:
		if (gameManager->gameMain->GetNetBallteFlg()) {
			ReMatch();
		}
		else {
			Return_to_Game();
		}
		
		break;
	case 1:
		if (gameManager->gameMain->GetNetBallteFlg()) {
			Return_to_Title();
		}
		else {
			Return_to_Title();
		}
		
		break;
	}
}

////////////////////////////////////////////////
// �`��p
////////////////////////////////////////////////
void Result::DrawResult() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	DrawGraph(0, 0, i_ResultImage, TRUE);	// �w�i�摜�`��

	// Pause�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "GAME SET !!");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "GAME SET !!");

	// �����Ŏ󂯎�����q�b�g�����v���C���[�̔ԍ����AGameManager�̌��݂̌����Ɠ������ۂ��𔻒肷��B
	if (hitPlayerNumber == gameManager->GetNowShooter()) {
		// �����������ꍇ�A���ł��������Ƃ�\������B
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s�̎��ŁI��", PlayerName[gameManager->GetNowShooter()]);
		DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "%s�̎��ŁI��", PlayerName[gameManager->GetNowShooter()]);
	}
	else {
		// ������ꍇ�A�����̏����ƕ\������
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s�̏����I", PlayerName[gameManager->GetNowShooter()]);
		DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "%s�̏����I", PlayerName[gameManager->GetNowShooter()]);
	}

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
			DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 1.0f, 0, gameManager->gameMain->GetCursorImage(i), TRUE);
		}

	}

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

////////////////////////////////////////////////
// �`��p�F�l�b�g�ΐ�p
////////////////////////////////////////////////
void Result::DrawResult_Net() {
	if (timeOutFlg) {
		DrawTimeOut();
		return;
	}

	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	DrawGraph(0, 0, i_ResultImage, TRUE);	// �w�i�摜�`��

	// Pause�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "GAME SET !!");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "GAME SET !!");

	// �����Ŏ󂯎�����q�b�g�����v���C���[�̔ԍ����AGameManager�̌��݂̌����Ɠ������ۂ��𔻒肷��B
	if (hitPlayerNumber == gameManager->GetNowShooter()) {
		// �����������ꍇ�A���ł��������Ƃ�\������B
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s�̎��ŁI��", PlayerName[gameManager->GetNowShooter()]);
		DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "%s�̎��ŁI��", PlayerName[gameManager->GetNowShooter()]);
	}
	else {
		// ������ꍇ�A�����̏����ƕ\������
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s�̏����I", PlayerName[gameManager->GetNowShooter()]);
		DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "%s�̏����I", PlayerName[gameManager->GetNowShooter()]);
	}

	// ���ڂ����肵�Ă�����A�����l�p��\������
	if (dicideNumFlg[0]) {
		DrawBox(0, starty + y * selectNum[0] - 15, GameMain::SCREEN_WIDTH, starty + y * selectNum[0] + 15, COLOR_VALUE_PLAYER[0], 1);
	}
	else {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * 0), starty + y * selectNum[0], 1.0f, 0, gameManager->gameMain->GetCursorImage(0), TRUE);
	}

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

////////////////////////////////////////////////
// �`��p
////////////////////////////////////////////////
void Result::DrawTimeOut() {


	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 400;

	DrawGraph(0, 0, i_ResultImage, TRUE);	// �w�i�摜�`��

	// Pause�̕����`��
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "���肩��̉�����10�b�Ԃ���܂���ł���");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 300, 0xFFFFFF, fontData->f_FontData[1], "���肩��̉�����10�b�Ԃ���܂���ł���");

	// ���ڂ����肵�Ă�����A�����l�p��\������
	if (dicideNumFlg[0]) {
		if (0 == GameManager::RED) {
			DrawBox(0, starty + y * selectNum[0] - 15, GameMain::SCREEN_WIDTH / 2, starty + y * selectNum[0] + 15, COLOR_VALUE_PLAYER[0], 1);
		}
		else {
			DrawBox(GameMain::SCREEN_WIDTH, starty + y * selectNum[0] - 15, GameMain::SCREEN_WIDTH / 2, starty + y * selectNum[0] + 15, COLOR_VALUE_PLAYER[0], 1);
		}
	}
	else {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * 0), starty + y * selectNum[0], 1.0f, 0, gameManager->gameMain->GetCursorImage(0), TRUE);
	}

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}
}

////////////////////////////////////////////////
// ��ʂ𔲂��ē�Փx�I����ʂ�
////////////////////////////////////////////////
void Result::Return_to_Game() {
	gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
	gameManager->gameMain->CreateDifficultySelectSceneObj();
	gameManager->gameMain->MainObjDelete();
	this->~Result();
}

////////////////////////////////////////////////
// �l�b�g�ΐ�ŁA���������ő���ƍĐ킷��
////////////////////////////////////////////////
void Result::ReMatch() {
	gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);
	gameManager->gameMain->network->VariableInit();
	gameManager->gameMain->network->SetSendRand();
	this->~Result();
}

////////////////////////////////////////////////
// �����𒆒f���ă^�C�g����ʂ֖߂�
////////////////////////////////////////////////
void Result::Return_to_Title() {
	gameManager->SetPhaseStatus(GameManager::TITLE);
	gameManager->gameMain->CreateTitleObj();
	gameManager->gameMain->MainObjDelete();
	this->~Result();
}

void Result::LoadImages() {
	if (!(i_ResultImage = LoadGraph("Image/Result.png"))) return;
}

void Result::DeleteImages() {
	i_ResultImage = DeleteGraph(i_ResultImage);
}
Result::~Result() {
	DeleteImages();
}