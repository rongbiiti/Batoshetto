#include "UI.h"

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
UI::UI(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	fontData = gameMain->fontData;

	LoadImages();		// �摜�ǂݍ���

	// �ϐ�������
	TransitionParameterReset();
	isBattleStart = TRUE;
	firstAnimWaitTime = 0;
	firstAnimX = 500;
	firstAnimY = 0;
	firstAnimAlpha = 90;
}

////////////////////////////////////////////////
// �����J�n���̃A�j���B���o���I����Ă�����FALSE��Ԃ�
////////////////////////////////////////////////
bool UI::BattleStartAnim() {
	// �����J�n����Ŗ�����Ώ������I���
	if (!isBattleStart) return false;

	// �K��̕b�����߂��Ă����珈�����I���
	if (180 < ++firstAnimWaitTime) {
		isBattleStart = FALSE;
		return false;
	}
	
	if (firstAnimWaitTime <= 45) {
		firstAnimY += 500 / 45;		// ���ɓ�����
		return true;
	}
	if (firstAnimWaitTime <= 90) {
		// ������Ƒҋ@
		return true;
	}
	if (firstAnimWaitTime <= 180) {
		--firstAnimAlpha;			// �����x���グ�ď���
		return true;
	}

	return false;

}

////////////////////////////////////////////////
// �B���E���̐ؑ֎��̃A�j���[�V����
// �������I����Ă�����true�A�܂��Ȃ�false���Ԃ�
////////////////////////////////////////////////
bool UI::TransitionAnimationWaiting() {
	if (BattleStartAnim()) return false;	// �����J�n���A�j�������Ă�����ȉ��̏��������Ȃ��ő������^�[��

	// �ŏ��̃t���[���Ńl�b�g�ΐ펞�̃o�b�t�@�[�N���A����
	// �\���̂�����������
	if (++animationWaitingTime <= 1) {
		gameMain->network->BufferClear();
		gameMain->network->StructsReset();
	}
	
	// ���āI�Ɠ����I�̕�����X���W�𓮂����BRED�͉E�ɓ����Ă����ABLUE�͍��ɓ����Ă����悤�ɏ����𕪊�
	if (animationWaitingTime <= 35) {

		if ((gameManager->GetPhaseStatus() == GameManager::SHOT && gameManager->GetNowShooter() == GameManager::BLUE) ||
			(gameManager->GetPhaseStatus() == GameManager::HIDE && gameManager->GetNowHider() == GameManager::BLUE)) {
			transitionX -= (gameMain->SCREEN_WIDTH_HALF + 80) / 35;
		}
		else {
			transitionX += (gameMain->SCREEN_WIDTH_HALF + 10) / 35;
		}

		return false;
	}
	if (animationWaitingTime <= 55) {
		// ������Ƒҋ@
		return false;
	}
	if (animationWaitingTime <= 90) {
		// �k�����Ȃ����ɓ�����
		transitionY -= 270 / 35;
		transitionExRate -= 1.0 / 35.0;
		return false;
	}

	// �����܂ŗ��āA�����̉摜���ς��悤�ɂ���
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

	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2;

	// �B��鑤���̏���
	if (gameManager->GetPhaseStatus() == GameManager::HIDE) {
		// �����I�̉摜�\��
		DrawRotaGraph(transitionX, transitionY, transitionExRate, 0, i_OrderImage[gameManager->GetNowHider()][gameManager->GetPhaseStatus() - GameManager::HIDE], TRUE);

		// ������@�\��
		if (gameManager->GetDifficulty() == GameManager::CASUAL && animationWaitingTime >= 90) {
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "���X�e�B�b�N�F�ړ�");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 40, 0xFFFFFF, fontData->f_FontData[0], "���X�e�B�b�N�F�ړ�");
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "X�{�^���F�p�X");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 60, 0xFFFFFF, fontData->f_FontData[0], "X�{�^���F�p�X");
		}
		
	}
	// �������̏���
	else {
		// ���āI�̉摜�\��
		DrawRotaGraph(transitionX, transitionY, transitionExRate, 0, i_OrderImage[gameManager->GetNowShooter()][gameManager->GetPhaseStatus() - GameManager::HIDE], TRUE);

		// ������@�\��
		if (gameManager->GetDifficulty() == GameManager::CASUAL && animationWaitingTime >= 90) {
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "���X�e�B�b�N��");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 40, 0xFFFFFF, fontData->f_FontData[0], "���X�e�B�b�N��");
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "�\���L�[�㉺�F�˂炤");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 60, 0xFFFFFF, fontData->f_FontData[0], "�\���L�[�㉺�F�˂炤");

			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "X�{�^���F�p�X");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 100, 0xFFFFFF, fontData->f_FontData[0], "X�{�^���F�p�X");
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "B�{�^���F����");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 120, 0xFFFFFF, fontData->f_FontData[0], "B�{�^���F����");
		}
		
	}

}

////////////////////////////////////////////////
// �����J�n���̃A�j���`��
////////////////////////////////////////////////
void UI::DrawBattleStartAnim() {
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * firstAnimAlpha / 90);	// �����x��ς���

	fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[1], "��");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 70) + y, 0xFFFFFF, gameMain->fontData->f_FontData[2], "��");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 200) + y, 0xFFFFFF, gameMain->fontData->f_FontData[2], "��");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 330) + y, 0xFFFFFF, gameMain->fontData->f_FontData[2], "�J");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 460) + y, 0xFFFFFF, gameMain->fontData->f_FontData[2], "�n");

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);	// �����x�����Ƃɖ߂�
}

////////////////////////////////////////////////
// �A�j���p�̕ϐ����Z�b�g
////////////////////////////////////////////////
void UI::TransitionParameterReset() {
	animationWaitingTime = 0;
	transitionExRate = 2.0;

	// BLUE�̃^�[���Ȃ�AX���W�����ʒu����ʉE���ɂ���
	if ( ( gameManager->GetPhaseStatus() == GameManager::SHOT && gameManager->GetNowShooter() == GameManager::BLUE ) ||
		 ( gameManager->GetPhaseStatus() == GameManager::HIDE && gameManager->GetNowHider() == GameManager::BLUE ) ) {
		transitionX = 1340;
	}
	// RED�Ȃ�AX���W�����ʒu����ʍ����ɂ���
	else {
		transitionX = 0;
	}
	
	transitionY = 330;
	transitionFlg = true;
}

////////////////////////////////////////////////
// �v���C���[�̎c�莞�Ԃ��Q�[�W�ŕ`�悷��֐�
////////////////////////////////////////////////
void UI::DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum) {
	double percent = (maxtime - nowtime) / maxtime * 100;
	DrawCircleGauge(x, y, percent, i_PlayerGuage[playernum]);
}

////////////////////////////////////////////////
// �w�i�摜�`��
////////////////////////////////////////////////
void UI::DrawBackGround() {
	DrawGraph(0, 0, i_BackGroundImage, 0);
}

////////////////////////////////////////////////
// �摜�ǂݍ���
////////////////////////////////////////////////
void UI::LoadImages() {
	if (!(i_PlayerGuage[0] = LoadGraph("Image/Gauge.png"))) return;
	if (!(i_PlayerGuage[1] = LoadGraph("Image/Gauge2.png"))) return;
	if (!(i_BackGroundImage = LoadGraph("Image/BackImage.png"))) return;
	if (!(i_OrderImage[0][0] = LoadGraph("Image/Order_Move_RED.png"))) return;
	if (!(i_OrderImage[0][1] = LoadGraph("Image/Order_Shot_RED.png"))) return;
	if (!(i_OrderImage[1][0] = LoadGraph("Image/Order_Move_BLUE.png"))) return;
	if (!(i_OrderImage[1][1] = LoadGraph("Image/Order_Shot_BLUE.png"))) return;
}

////////////////////////////////////////////////
// �摜����
////////////////////////////////////////////////
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