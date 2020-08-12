#include "UI.h"

// �R���X�g���N�^
UI::UI(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	fontData = gameMain->fontData;
	LoadImages();
	TransitionParameterReset();
	isBattleStart = TRUE;
	firstAnimWaitTime = 0;
	firstAnimX = 500;
	firstAnimY = 0;
	firstAnimAlpha = 90;
}

// �p�����[�^��ύX����
void UI::UIControll() {

}

// �����J�n���̃A�j���B���o���I����Ă�����FALSE��Ԃ�
bool UI::BattleStartAnim() {
	// �����J�n����Ŗ�����Ώ������I���
	if (!isBattleStart) return false;

	// �K��̕b�����߂��Ă����珈�����I���
	if (210 < ++firstAnimWaitTime) {
		isBattleStart = FALSE;
		return false;
	}
	
	if (firstAnimWaitTime <= 60) {
		firstAnimY += 500 / 60;
		return true;
	}
	if (firstAnimWaitTime <= 120) {
		return true;
	}
	if (firstAnimWaitTime <= 210) {
		--firstAnimAlpha;
		return true;
	}

	return false;

}

// �B���E���̐ؑ֎��̃A�j���[�V����
// �������I����Ă�����true�A�܂��Ȃ�false���Ԃ�
bool UI::TransitionAnimationWaiting() {
	if (BattleStartAnim()) return false;

	if (++animationWaitingTime <= 1) {
		gameMain->network->BufferClear();
		gameMain->network->StructsReset();
	}
	
	if (animationWaitingTime <= 40) {
		transitionX += ( gameMain->SCREEN_WIDTH_HALF + 50 ) / 40;
		return false;
	}
	if (animationWaitingTime <= 60) {
		
		return false;
	}
	if (animationWaitingTime <= 100) {
		transitionY -= 270 / 40;
		transitionExRate -= 1.0 / 40.0;
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
	// �����J�n����Ŗ�����Ώ������I���
	if (isBattleStart) {
		DrawBattleStartAnim();
		return;
	}

	if (gameManager->GetPhaseStatus() == GameManager::HIDE) {
		//DrawFormatStringToHandle(transitionX, transitionY, COLOR_VALUE_PLAYER[gameManager->GetNowHider()], fontData->f_FontData[1], "%s�����I", PlayerName[gameManager->GetNowHider()]);
		DrawRotaGraph(transitionX, transitionY, transitionExRate, 0, i_OrderImage[gameManager->GetNowHider()][gameManager->GetPhaseStatus() - GameManager::HIDE], TRUE);
	}
	else {
		//DrawFormatStringToHandle(transitionX, transitionY, COLOR_VALUE_PLAYER[gameManager->GetNowShooter()], fontData->f_FontData[1], "%s���āI", PlayerName[gameManager->GetNowShooter()]);
		DrawRotaGraph(transitionX, transitionY, transitionExRate, 0, i_OrderImage[gameManager->GetNowShooter()][gameManager->GetPhaseStatus() - GameManager::HIDE], TRUE);
	}

}

// �����J�n���̃A�j���`��
void UI::DrawBattleStartAnim() {
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * firstAnimAlpha / 90);
	fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[1], "��");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 70), 0xFFFFFF, gameMain->fontData->f_FontData[2], "��");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 200), 0xFFFFFF, gameMain->fontData->f_FontData[2], "��");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 330), 0xFFFFFF, gameMain->fontData->f_FontData[2], "�J");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 460), 0xFFFFFF, gameMain->fontData->f_FontData[2], "�n");

	fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[1], "Battle Start");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 560), 0xFFFFFF, gameMain->fontData->f_FontData[1], "Battle Start");
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void UI::TransitionParameterReset() {
	animationWaitingTime = 0;
	transitionExRate = 2.0;
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
	if (!(i_OrderImage[0][0] = LoadGraph("Image/Order_Move_RED.png"))) return;
	if (!(i_OrderImage[0][1] = LoadGraph("Image/Order_Shot_RED.png"))) return;
	if (!(i_OrderImage[1][0] = LoadGraph("Image/Order_Move_BLUE.png"))) return;
	if (!(i_OrderImage[1][1] = LoadGraph("Image/Order_Shot_BLUE.png"))) return;
}

// �摜����
void UI::DeleteImages() {
	i_PlayerGuage[0] = DeleteGraph(i_PlayerGuage[0]);
	i_PlayerGuage[1] = DeleteGraph(i_PlayerGuage[1]);
	i_BackGroundImage = DeleteGraph(i_BackGroundImage);
	i_OrderImage[0][0] = DeleteGraph(i_OrderImage[0][0]);
	i_OrderImage[0][1] = DeleteGraph(i_OrderImage[0][1]);
	i_OrderImage[1][0] = DeleteGraph(i_OrderImage[1][0]);
	i_OrderImage[1][1] = DeleteGraph(i_OrderImage[1][1]);
}

UI::~UI() {
	DeleteImages();
}