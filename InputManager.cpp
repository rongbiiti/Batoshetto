#include "InputManager.h"

InputManager::InputManager() {
	for (int i = 0; i < 256; i++) {
		k_Buf[i] = 0;		// 0�ɖ߂�
	}
}

void InputManager::InputKey(void) {
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) k_Buf[i]++;		// ���͂��ꂽ�L�[�͉�����Ă���Ԃǂ�ǂ񐔎��������Ă����B
		else        k_Buf[i] = 0;		// �����ƈ�C��0�ɖ߂�
	}
}