#include "Title.h"

Title::Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG) {
	fontData = font;
	inputManager = inputMNG;
	gameManager = gameMNG;

	// �J�[�\���ʒu������
	selectNum[0] = 0;
	selectNum[1] = 0;
}

void Title::DrawTitle() {
	DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x202020, 1);	// �w�i���F�œh��Ԃ�
	
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "Paus");
	DrawFormatStringToHandle(x - fontwidth -45, GameMain::SCREEN_HEIGHT / 4 - fontwidth, 0xeeff14, fontData->f_FontData[1], "�o�g�V�F�b�g");

	// �e���ږ��`��
	for (int i = 0; i < SELECT_NUM_MAX + 1; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%s", MenuName[i].c_str());
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xeeff14, fontData->f_FontData[1], "%s", MenuName[i].c_str());
	}

	for (int i = 0; i < 2; i++) {
		// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
		DrawCircle(GameMain::SCREEN_WIDTH / 4 + (GameMain::SCREEN_WIDTH / 2 * i), starty + y * selectNum[i], 10, COLOR_VALUE_PLAYER[i], 1, 1);
	}
}

Title::~Title() {

}