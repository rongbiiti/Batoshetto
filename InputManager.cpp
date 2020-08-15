#include "InputManager.h"
#include <math.h>

////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
InputManager::InputManager() {
	// 配列初期化
	for (int i = 0; i < 256; i++) {
		in_Key[i] = 0;		// 0に戻る
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 16; j++) {
			gamePad[i].in_Button[j] = 0;
		}
		gamePad[i].in_Stick_LX = 0;
		gamePad[i].in_Stick_LY = 0;
		gamePad[i].in_Stick_RX = 0;
		gamePad[i].in_Stick_RY = 0;
	}
	
}

////////////////////////////////////////////////
// 入力を受け取る関数
////////////////////////////////////////////////
void InputManager::InputKey(void) {
	// キーボードの全キーを押されているかどうかをチェックして、配列に入れる
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) in_Key[i]++;		// 入力されたキーは押されている間どんどん数字が増えていく。
		else        in_Key[i] = 0;		// 離すと一気に0に戻る
	}
	InputButton();
}

////////////////////////////////////////////////
// Xboxコントローラーの入力を受け取る関数
////////////////////////////////////////////////
void InputManager::InputButton(void) {
	// XInputコントローラーの入力情報を配列にいれる
	GetJoypadXInputState(DX_INPUT_PAD1, &input[0]);
	GetJoypadXInputState(DX_INPUT_PAD2, &input[1]);

	for (int num = 0; num < 2; num++) {
		// ボタンだけを、キーボードと同じようにチェックする。
		for (int i = 0; i < 16; i++)
		{
			if (i == 10 || i == 11) continue;
			if (input[num].Buttons[i]) gamePad[num].in_Button[i]++;
			else				  gamePad[num].in_Button[i] = 0;
			// デバッグ用描画
			//DrawFormatString(400 + i % 2 * 200, 200 + i / 2 * 16, 0xFFFFFF, "%s:%d", buttonName[i].c_str(), in_Button[i]);
		}

		gamePad[num].in_Stick_LX = (float)(input[num].ThumbLX / 32767.0f);	// スティックの傾きはshort型で保存されているので、
		gamePad[num].in_Stick_LY = (float)(input[num].ThumbLY / 32767.0f);	// わかりやすいようにfloat型に計算している。
		gamePad[num].in_Stick_RX = (float)(input[num].ThumbRX / 32767.0f);
		gamePad[num].in_Stick_RY = (float)(input[num].ThumbRY / 32767.0f);

		if (abs(gamePad[num].in_Stick_LX) <= 0.2f && abs(gamePad[num].in_Stick_LY) <= 0.2f) {
			gamePad[num].in_Stick_LX = 0;
			gamePad[num].in_Stick_LY = 0;
		}
		if (abs(gamePad[num].in_Stick_RX) <= 0.2f && abs(gamePad[num].in_Stick_RY) <= 0.2f) {
			gamePad[num].in_Stick_RX = 0;
			gamePad[num].in_Stick_RY = 0;
		}

		// デバッグ用描画
		//DrawFormatString(900, 0, 0xFFFFFF, "LeftTrigger:%d RightTrigger:%d", input.LeftTrigger, input.RightTrigger);
		//DrawFormatString(900, 16, 0xFFFFFF, "ThumbLX:%.4f ThumbLY:%.4f", in_Stick_LX, in_Stick_LY);
		//DrawFormatString(900, 32, 0xFFFFFF, "ThumbRX:%.4f ThumbRY:%.4f", in_Stick_RX, in_Stick_RY);
	}
}

////////////////////////////////////////////////
// 指定したプレイヤーのパッドのボタンを押した瞬間かを受け取る
////////////////////////////////////////////////
bool InputManager::GetButtonDown(BUTTON buttonCode, int playerNum) {
	if (gamePad[playerNum].in_Button[buttonCode] == 1) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// 指定したプレイヤーのパッドのボタンを押し続けているかを受け取る
////////////////////////////////////////////////
bool InputManager::GetButtonHold(BUTTON buttonCode, int playerNum) {
	if (gamePad[playerNum].in_Button[buttonCode] >= HOLD_FRAMECOUNT) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// 押し続けているかを受け取り、ボタンの押し続け秒数を減らす。
////////////////////////////////////////////////
bool InputManager::GetButtonHold(BUTTON buttonCode, int playerNum, int reduceValue) {
	if (gamePad[playerNum].in_Button[buttonCode] >= HOLD_FRAMECOUNT) {
		gamePad[playerNum].in_Button[buttonCode] -= reduceValue;
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// キーボードのキーを押した瞬間かを受け取る
////////////////////////////////////////////////
bool InputManager::GetKeyDown(int KeyCode) {
	if (in_Key[KeyCode] == 1) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// キーボードのキーを押し続けているかを受け取る
////////////////////////////////////////////////
bool InputManager::GetKeyHold(int KeyCode) {
	if (in_Key[KeyCode] >= HOLD_FRAMECOUNT) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// 押し続けているかを受け取り、ボタンの押し続け秒数を減らす。
////////////////////////////////////////////////
bool InputManager::GetKeyHold(int KeyCode, int reduceValue) {
	if (in_Key[KeyCode] >= HOLD_FRAMECOUNT) {
		in_Key[KeyCode] -= reduceValue;
		return true;
	}
	return false;
}