#include "InputManager.h"

InputManager::InputManager() {
	for (int i = 0; i < 256; i++) {
		k_Buf[i] = 0;		// 0に戻る
	}
}

void InputManager::InputKey(void) {
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) k_Buf[i]++;		// 入力されたキーは押されている間どんどん数字が増えていく。
		else        k_Buf[i] = 0;		// 離すと一気に0に戻る
	}
}