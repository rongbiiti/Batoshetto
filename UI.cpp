#include "UI.h"

// �R���X�g���N�^
UI::UI(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	fontData = gameMain->fontData;
	LoadImages();
	TransitionParameterReset();
}

// �p�����[�^��ύX����
void UI::UIControll() {

}

// �B���E���̐ؑ֎��̃A�j���[�V����
// �������I����Ă�����true�A�܂��Ȃ�false���Ԃ�
bool UI::TransitionAnimationWaiting() {
	if (++animationWaitingTime == 1) {
		gameMain->network->BufferClear();
	}
	if (animationWaitingTime <= 40) {
		transitionX += 500 / 40;
		return false;
	}
	if (animationWaitingTime <= 60) {
		
		return false;
	}
	if (animationWaitingTime <= 100) {
		transitionY -= 270 / 40;
		return false;
	}

	if (gameManager->GetPhaseStatus() == GameManager::SHOT) {
		gameManager->ShooterFlgChange(true);
	}
	else {
		gameManager->ShooterFlgChange(false);
	}
	transitionFlg = false;
	
	return true;
}

void UI::DrawTransitionAnimation() {
	if (gameManager->GetPhaseStatus() == GameManager::HIDE) {
		DrawFormatStringToHandle(transitionX, transitionY, COLOR_VALUE_PLAYER[gameManager->GetNowHider()], fontData->f_FontData[1], "%s�����I", PlayerName[gameManager->GetNowHider()]);
	}
	else {
		DrawFormatStringToHandle(transitionX, transitionY, COLOR_VALUE_PLAYER[gameManager->GetNowShooter()], fontData->f_FontData[1], "%s���āI", PlayerName[gameManager->GetNowShooter()]);
	}
}

void UI::TransitionParameterReset() {
	animationWaitingTime = 0;
	transitionX = 0;
	transitionY = 330;
	transitionFlg = true;
}

// �v���C���[�̎c�莞�Ԃ��Q�[�W�ŕ`�悷��֐�
void UI::DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum) {
	double percent = (maxtime - nowtime) / maxtime * 100;
	DrawCircleGauge(x, y, percent, i_PlayerGuage[playernum]);
}

// �w�i�摜�`��
void UI::DrawBackGround() {
	DrawGraph(0, 0, i_BackGroundImage, 0);
}

// �摜�ǂݍ���
void UI::LoadImages() {
	if (!(i_PlayerGuage[0] = LoadGraph("Image/Gauge.png"))) return;
	if (!(i_PlayerGuage[1] = LoadGraph("Image/Gauge2.png"))) return;
	if (!(i_BackGroundImage = LoadGraph("Image/BackImage.png"))) return;
	
}

// �摜����
void UI::DeleteImages() {
	DeleteGraph(i_PlayerGuage[0]);
	DeleteGraph(i_PlayerGuage[1]);
	DeleteGraph(i_BackGroundImage);
	i_PlayerGuage[0] = NULL;
	i_PlayerGuage[1] = NULL;
	i_BackGroundImage = NULL;
}

UI::~UI() {
	DeleteImages();
}