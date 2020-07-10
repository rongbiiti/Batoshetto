#include "Result.h"

// フォントデータ管理オブジェクトのポインタ、入力管理オブジェクトのポインタ、
// ゲーム進行管理オブジェクトのポインタ、ヒットしたプレイヤーがREDかBLUEかを受け取る。
Result::Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;
	this->hitPlayerNumber = hitplayernum;
}

// 描画用
void Result::DrawResult() {
	// 引数で受け取ったヒットしたプレイヤーの番号が、GameManagerの現在の撃つ側と同じか否かを判定する。
	if (hitPlayerNumber == gameManager->GetNowShooter()) {
		// 同じだった場合、自滅だったことを表示する。
		DrawFormatStringToHandle(500, 360, 0xFFFFFF, fontData->f_FontData[1], "%sの自滅！ｗ", PlayerName[gameManager->GetNowShooter()]);
	}
	else {
		// 違った場合、撃つ側の勝利と表示する
		DrawFormatStringToHandle(500, 360, 0xFFFFFF, fontData->f_FontData[1], "%sの勝ち！", PlayerName[gameManager->GetNowShooter()]);
	}
	
	// Fを押すと、GameManagerのフェーズを初期化フェーズに変更し、自身のデストラクタを呼ぶ。
	DrawFormatStringToHandle(500, 560, 0xFFFFFF, fontData->f_FontData[1], "A押して再開");
	if (inputManager->GetPadInput()[GameManager::RED].in_Button[InputManager::A] == 1 ||
		inputManager->GetPadInput()[GameManager::BLUE].in_Button[InputManager::A] == 1 || 
		inputManager->In_Key()[KEY_INPUT_F] == 1) {
		gameManager->SetPhaseStatus(GameManager::INIT);
		this->~Result();
	}
}

Result::~Result() {
}