#include "InputManager.h"

// コンストラクタ
InputManager::InputManager() {
	// 配列初期化
	for (int i = 0; i < 256; i++) {
		in_Key[i] = 0;		// 0に戻る
	}

	// スティックのデッドゾーンを設定する
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.2);
}

// 入力を受け取る関数
void InputManager::InputKey(void) {
	// キーボードの全キーを押されているかどうかをチェックして、配列に入れる
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) in_Key[i]++;		// 入力されたキーは押されている間どんどん数字が増えていく。
		else        in_Key[i] = 0;		// 離すと一気に0に戻る
	}
	InputButton();
}

// Xboxコントローラーの入力を受け取る関数
void InputManager::InputButton(void) {
	// XInputコントローラーの入力情報を配列にいれる
	GetJoypadXInputState(DX_INPUT_PAD1, &input);

	// ボタンだけを、キーボードと同じようにチェックする。
	for (int i = 0; i < 16; i++)
	{
		if (i == 10 || i == 11) continue;
		if (input.Buttons[i]) in_Button[i]++;
		else				  in_Button[i] = 0;
		// デバッグ用描画
		DrawFormatString(400 + i % 2 * 200, 200 + i / 2 * 16, 0xFFFFFF, "%s:%d", buttonName[i].c_str(), in_Button[i]);
	}

	in_Stick_LX = (float)(input.ThumbLX / 32767.0f);	// スティックの傾きはshort型で保存されているので、
	in_Stick_LY = (float)(input.ThumbLY / 32767.0f);	// わかりやすいようにfloat型に計算している。
	in_Stick_RX = (float)(input.ThumbRX / 32767.0f);
	in_Stick_RY = (float)(input.ThumbRY / 32767.0f);

	// デバッグ用描画
	DrawFormatString(900, 0, 0xFFFFFF, "LeftTrigger:%d RightTrigger:%d", input.LeftTrigger, input.RightTrigger);
	DrawFormatString(900, 16, 0xFFFFFF, "ThumbLX:%.4f ThumbLY:%.4f", in_Stick_LX, in_Stick_LY);
	DrawFormatString(900, 32, 0xFFFFFF, "ThumbRX:%.4f ThumbRY:%.4f", in_Stick_RX, in_Stick_RY);
}