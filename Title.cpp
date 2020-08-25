#include "Title.h"

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
Title::Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG) {
	fontData = font;
	inputManager = inputMNG;
	gameManager = gameMNG;
	gameMain = gameManager->gameMain;
	titleBullet = new TitleBullet(fontData);	// �w�i�Œ��e���Ă�e�N���X����

	// �J�[�\���ʒu������
	selectNum[0] = 0;
	selectNum[1] = 0;
	selectNum[2] = 0;

	LoadImages();	// �摜�ǂݍ���

	backAnimWaitTime = 240;
}
////////////////////////////////////////////////
// �����p�֐�
////////////////////////////////////////////////
void Title::TitleControll() {

	// �w���v��ʂ��J���Ă�����A�w���v��ʂ̏��������đ������^�[��
	if (isOpenHelpScreen) {
		HelpScreenControll();
		return;
	}

	// �w�i�̒e�̏����B�s���Ă�����֐����Ăяo���ď���������
	// ����ł�����A�҂����Ԃ�i�߂�B
	if (!titleBullet->GetIsAlive()) {
		// �҂����Ԃ����ȏ�ɂȂ�����A�e�����������āA�҂����Ԃ����Z�b�g����
		if (300 <= ++backAnimWaitTime) {
			// ����X���W��Y���W�̃t���O���A�����ō���ēn��
			titleBullet->BulletInit(TRUE, GetRand(1), GetRand(1));
			backAnimWaitTime = 0;
		}
	}
	else {
		titleBullet->TitleBulletControll();
	}



	///////////////////////////////////////////////////////////////////////////////
	////////////////////////////// �R���g���[���[���� //////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 2; i++) {
		if (inputManager->GetButtonDown(PAD_UP, i) || inputManager->GetButtonHold(PAD_UP, i, 4)) {
			// �Q�[���p�b�h�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���ŏ�Ɉړ�
			// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
			if (--selectNum[i] < 0) {
				selectNum[i] = SELECT_NUM_MAX;
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(PAD_DOWN, i) || inputManager->GetButtonHold(PAD_DOWN, i, 4)) {
			// �Q�[���p�b�h�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŉ��Ɉړ�
			// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
			if (++selectNum[i] > SELECT_NUM_MAX) {
				selectNum[i] = 0;	
			}
			gameMain->PlayCursorSE();
		}

		if (inputManager->GetButtonDown(B, i)) {
			// �Q�[���p�b�h��B�{�^������
			ProcessingOnDetermining(selectNum[i]);
			return;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////// �L�[�{�[�h���� //////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	// 2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// �L�[�{�[�h�����L�[��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum[GameManager::BLUE] < 0) {
			selectNum[GameManager::BLUE] = SELECT_NUM_MAX;
		}
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// �L�[�{�[�h�����L�[���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum[GameManager::BLUE] > SELECT_NUM_MAX) {
			selectNum[GameManager::BLUE] = 0;
		}
		
		gameMain->PlayCursorSE();
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN) == 1) {
		// �L�[�{�[�h�������
		ProcessingOnDetermining(GameManager::BLUE + 1);
	}
}

////////////////////////////////////////////////
// ����{�^���������̏����B�����́A�R���g���[���[�̔ԍ�
////////////////////////////////////////////////
void Title::ProcessingOnDetermining(int controllerNum) {

	int num = controllerNum;
	if (controllerNum == GameManager::BLUE + 1) --num;

	switch (selectNum[num])
	{
	case 0:
		// �ʏ�v���C�B��Փx�I����ʂւ���
		gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
		gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
		this->~Title();
		break;

	case 1:
		// ���[�J���ʐM�B�ʐM�ΐ�t���O�����Ă�
		gameManager->SetPhaseStatus(GameManager::DIFFICULTYSELECT);
		gameManager->gameMain->diffiSelectScene = new DifficultySelectScene(inputManager, fontData, gameManager);
		gameMain->SetNetBallteFlg(TRUE);
		this->~Title();
		break;

	case 2:
		// �I�v�V������ʊJ��
		gameManager->SetPhaseStatus(GameManager::OPTION);
		gameMain->CreateOptionObj(controllerNum, Option::TITLE);
		this->~Title();
		break;

	case 3:
		// �w���v��ʊJ��
		isOpenHelpScreen = true;
		break;

	case 4:
		// �G���h�m�F��ʊJ��
		gameManager->SetPhaseStatus(GameManager::END);
		gameMain->CreateEndObj();
		this->~Title();
		break;
	}

	gameMain->PlayDicideSE();	// ���莞���炷
}

////////////////////////////////////////////////
// �w���v��ʂ̏���
////////////////////////////////////////////////
void Title::HelpScreenControll() {
	for (int i = 0; i < 2; i++) {
		if (inputManager->GetButtonDown(B, i) ||
			inputManager->GetButtonDown(A, i)) {

			// �p�b�hA�{�^����B�{�^����������Ă�����w���v��ʏI��
			isOpenHelpScreen = false;
			gameMain->PlayCanselSE();
			return;
		}
	}

	if (inputManager->GetKeyDown(KEY_INPUT_ESCAPE) || inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		// �L�[�{�[�hESC��RETURN��������Ă�����w���v��ʏI��
		isOpenHelpScreen = false;
		gameMain->PlayCanselSE();
		return;
	}
}

////////////////////////////////////////////////
// �w���v��ʂ̕`��
////////////////////////////////////////////////
void Title::DrawHelpScreen() {
	DrawGraph(0, 0, i_helpImage, TRUE);
}

////////////////////////////////////////////////
// �^�C�g����ʕ`��
////////////////////////////////////////////////
void Title::DrawTitle() {
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// �w�i���F�œh��Ԃ�

	// �w���v���J���Ă�����w���v�摜�\��
	if (isOpenHelpScreen) {
		DrawHelpScreen();
		return;
	}

	DrawGraph(0, 0, i_BGImage, TRUE);	// �w�i�摜

	titleBullet->DrawTitleBullet();

	// �r���h������
	DrawFormatStringToHandle(0, 0, 0xFFFFFF, fontData->f_FontData[0], "�r���h�������F2020/08/25");
	
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	DrawRotaGraph(x, starty - 200, 1, 0, i_LogoImage, TRUE);	// �^�C�g�����S

	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xffffff, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	for (int i = 0; i < 2; i++) {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawRotaGraph(GameMain::SCREEN_WIDTH / 3 + (GameMain::SCREEN_WIDTH / 3 * i), starty + y * selectNum[i], 1.0f, 0, gameMain->GetCursorImage(i), TRUE);
	}
	
}

////////////////////////////////////////////////
// �摜�ǂݍ���
////////////////////////////////////////////////
void Title::LoadImages() {
	if (!(i_helpImage = LoadGraph("Image/Help.png"))) return;
	if (!(i_LogoImage = LoadGraph("Image/TitleLogo.png"))) return;
	if (!(i_BGImage = LoadGraph("Image/TitleBulletBackImage.png"))) return;
	if (!(i_MenuImage[0] = LoadGraph("Image/StartButton.png"))) return;
	if (!(i_MenuImage[1] = LoadGraph("Image/OnlineButton.png"))) return;
	if (!(i_MenuImage[2] = LoadGraph("Image/OptionButton.png"))) return;
	if (!(i_MenuImage[3] = LoadGraph("Image/HelpButton.png"))) return;
	if (!(i_MenuImage[4] = LoadGraph("Image/EndButton.png"))) return;
}

////////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////////
Title::~Title() {
	delete titleBullet;
	titleBullet = NULL;

	i_helpImage = DeleteGraph(i_helpImage);
	i_LogoImage = DeleteGraph(i_LogoImage);
	i_BGImage = DeleteGraph(i_BGImage);
	i_MenuImage[0] = DeleteGraph(i_MenuImage[0]);
	i_MenuImage[1] = DeleteGraph(i_MenuImage[1]);
	i_MenuImage[2] = DeleteGraph(i_MenuImage[2]);
	i_MenuImage[3] = DeleteGraph(i_MenuImage[3]);
	i_MenuImage[4] = DeleteGraph(i_MenuImage[4]);

	gameMain->title = NULL;
}