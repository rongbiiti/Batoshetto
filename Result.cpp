#include "Result.h"

// �t�H���g�f�[�^�Ǘ��I�u�W�F�N�g�̃|�C���^�A���͊Ǘ��I�u�W�F�N�g�̃|�C���^�A
// �Q�[���i�s�Ǘ��I�u�W�F�N�g�̃|�C���^�A�q�b�g�����v���C���[��RED��BLUE�����󂯎��B
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	this->hitPlayerNumber = hitplayernum;
}

// �`��p
void Result::DrawResult() {
	// �����Ŏ󂯎�����q�b�g�����v���C���[�̔ԍ����AGameManager�̌��݂̌����Ɠ������ۂ��𔻒肷��B
	if (hitPlayerNumber == gameManager->GetNowShooter()) {
		// �����������ꍇ�A���ł��������Ƃ�\������B
		DrawFormatStringToHandle(500, 360, 0xFFFFFF, fontData->f_FontData[1], "%s�̎��ŁI��", PlayerName[gameManager->GetNowShooter()]);
	}
	else {
		// ������ꍇ�A�����̏����ƕ\������
		DrawFormatStringToHandle(500, 360, 0xFFFFFF, fontData->f_FontData[1], "%s�̏����I", PlayerName[gameManager->GetNowShooter()]);
	}
	
	// F�������ƁAGameManager�̃t�F�[�Y���������t�F�[�Y�ɕύX���A���g�̃f�X�g���N�^���ĂԁB
	DrawFormatStringToHandle(500, 560, 0xFFFFFF, fontData->f_FontData[1], "A�����čĊJ");
	if (inputManager->GetPadInput()[GameManager::RED].in_Button[InputManager::A] == 1 ||
		inputManager->GetPadInput()[GameManager::BLUE].in_Button[InputManager::A] == 1 || 
		inputManager->In_Key()[KEY_INPUT_F] == 1) {
		gameManager->SetPhaseStatus(GameManager::INIT);
		this->~Result();
	}
}

Result::~Result() {
}