#include "Result.h"

Result::Result(FontData* font, InputManager* input, GameManager* gameMNG, int num) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	this->num = num;
}

void Result::DrawResult() {
	if (num == gameManager->GetNowShooter()) {
		DrawFormatStringToHandle(500, 360, 0xFFFFFF, fontData->f_FontData[1], "%s�̎��ŁI��", PlayerName[gameManager->GetNowShooter()]);
	}
	else {
		DrawFormatStringToHandle(500, 360, 0xFFFFFF, fontData->f_FontData[1], "%s�̏����I", PlayerName[gameManager->GetNowShooter()]);
	}
	
	DrawFormatStringToHandle(500, 560, 0xFFFFFF, fontData->f_FontData[1], "F�����čĊJ");
	if (inputManager->k_Buf[KEY_INPUT_F] == 1) {
		gameManager->SetPhaseStatus(GameManager::INIT);
		this->~Result();
	}
}

Result::~Result() {
	
}