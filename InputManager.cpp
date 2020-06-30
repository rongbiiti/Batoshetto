#include "InputManager.h"

InputManager::InputManager() {
	for (int i = 0; i < 256; i++) {
		k_Buf[i] = 0;		// 0に戻る
	}
	SetJoypadDeadZone(DX_INPUT_PAD1, 1);
}

void InputManager::InputKey(void) {
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) k_Buf[i]++;		// 入力されたキーは押されている間どんどん数字が増えていく。
		else        k_Buf[i] = 0;		// 離すと一気に0に戻る
	}
	InputButton();
}

void InputManager::InputButton(void) {
	GetJoypadXInputState(DX_INPUT_PAD1, &input);

	for (int i = 0; i < 16; i++)
	{
		if (i == 10 || i == 11) continue;
		if (input.Buttons[i]) k_Button[i]++;
		else				  k_Button[i] = 0;
		DrawFormatString(400 + i % 2 * 200, 200 + i / 2 * 16, 0xFFFFFF, "%s:%d", buttonName[i].c_str(), k_Button[i]);
	}
	ThumbLX = (float)(input.ThumbLX / 32767.0f);
	ThumbLY = (float)(input.ThumbLY / 32767.0f);
	ThumbRX = (float)(input.ThumbRX / 32767.0f);
	ThumbRY = (float)(input.ThumbRY / 32767.0f);

	DrawFormatString(900, 0, 0xFFFFFF, "LeftTrigger:%d RightTrigger:%d", input.LeftTrigger, input.RightTrigger);
	DrawFormatString(900, 16, 0xFFFFFF, "ThumbLX:%.4f ThumbLY:%.4f", ThumbLX, ThumbLY);
	DrawFormatString(900, 32, 0xFFFFFF, "ThumbRX:%.4f ThumbRY:%.4f", ThumbRX, ThumbRY);
}