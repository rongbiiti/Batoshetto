#include "Title.h"

Title::Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG) {
	fontData = font;
	inputManager = inputMNG;
	gameManager = gameMNG;
}

void Title::DrawTitle() {
	SetFontSize(120);
	DrawFormatString(300, 100, 0xeeff14, "バトシェット");
	SetFontSize(60);
	DrawFormatString(565, 350, 0xeeff14, "Start");
	DrawFormatString(550, 450, 0xeeff14, "Option");
	DrawFormatString(595, 550, 0xeeff14, "End");
}

Title::~Title() {

}