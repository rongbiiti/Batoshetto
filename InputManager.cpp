#include "InputManager.h"
#include <math.h>

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
InputManager::InputManager() {
	// �z�񏉊���
	for (int i = 0; i < 256; i++) {
		in_Key[i] = 0;		// 0�ɖ߂�
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
// ���͂��󂯎��֐�
////////////////////////////////////////////////
void InputManager::InputKey(void) {
	// �L�[�{�[�h�̑S�L�[��������Ă��邩�ǂ������`�F�b�N���āA�z��ɓ����
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) in_Key[i]++;		// ���͂��ꂽ�L�[�͉�����Ă���Ԃǂ�ǂ񐔎��������Ă����B
		else        in_Key[i] = 0;		// �����ƈ�C��0�ɖ߂�
	}
	InputButton();
}

////////////////////////////////////////////////
// Xbox�R���g���[���[�̓��͂��󂯎��֐�
////////////////////////////////////////////////
void InputManager::InputButton(void) {
	// XInput�R���g���[���[�̓��͏���z��ɂ����
	GetJoypadXInputState(DX_INPUT_PAD1, &input[0]);
	GetJoypadXInputState(DX_INPUT_PAD2, &input[1]);

	for (int num = 0; num < 2; num++) {
		// �{�^���������A�L�[�{�[�h�Ɠ����悤�Ƀ`�F�b�N����B
		for (int i = 0; i < 16; i++)
		{
			if (i == 10 || i == 11) continue;
			if (input[num].Buttons[i]) gamePad[num].in_Button[i]++;
			else				  gamePad[num].in_Button[i] = 0;
			// �f�o�b�O�p�`��
			//DrawFormatString(400 + i % 2 * 200, 200 + i / 2 * 16, 0xFFFFFF, "%s:%d", buttonName[i].c_str(), in_Button[i]);
		}

		gamePad[num].in_Stick_LX = (float)(input[num].ThumbLX / 32767.0f);	// �X�e�B�b�N�̌X����short�^�ŕۑ�����Ă���̂ŁA
		gamePad[num].in_Stick_LY = (float)(input[num].ThumbLY / 32767.0f);	// �킩��₷���悤��float�^�Ɍv�Z���Ă���B
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

		// �f�o�b�O�p�`��
		//DrawFormatString(900, 0, 0xFFFFFF, "LeftTrigger:%d RightTrigger:%d", input.LeftTrigger, input.RightTrigger);
		//DrawFormatString(900, 16, 0xFFFFFF, "ThumbLX:%.4f ThumbLY:%.4f", in_Stick_LX, in_Stick_LY);
		//DrawFormatString(900, 32, 0xFFFFFF, "ThumbRX:%.4f ThumbRY:%.4f", in_Stick_RX, in_Stick_RY);
	}
}

////////////////////////////////////////////////
// �w�肵���v���C���[�̃p�b�h�̃{�^�����������u�Ԃ����󂯎��
////////////////////////////////////////////////
bool InputManager::GetButtonDown(BUTTON buttonCode, int playerNum) {
	if (gamePad[playerNum].in_Button[buttonCode] == 1) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// �w�肵���v���C���[�̃p�b�h�̃{�^�������������Ă��邩���󂯎��
////////////////////////////////////////////////
bool InputManager::GetButtonHold(BUTTON buttonCode, int playerNum) {
	if (gamePad[playerNum].in_Button[buttonCode] >= HOLD_FRAMECOUNT) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// ���������Ă��邩���󂯎��A�{�^���̉��������b�������炷�B
////////////////////////////////////////////////
bool InputManager::GetButtonHold(BUTTON buttonCode, int playerNum, int reduceValue) {
	if (gamePad[playerNum].in_Button[buttonCode] >= HOLD_FRAMECOUNT) {
		gamePad[playerNum].in_Button[buttonCode] -= reduceValue;
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// �L�[�{�[�h�̃L�[���������u�Ԃ����󂯎��
////////////////////////////////////////////////
bool InputManager::GetKeyDown(int KeyCode) {
	if (in_Key[KeyCode] == 1) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// �L�[�{�[�h�̃L�[�����������Ă��邩���󂯎��
////////////////////////////////////////////////
bool InputManager::GetKeyHold(int KeyCode) {
	if (in_Key[KeyCode] >= HOLD_FRAMECOUNT) {
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// ���������Ă��邩���󂯎��A�{�^���̉��������b�������炷�B
////////////////////////////////////////////////
bool InputManager::GetKeyHold(int KeyCode, int reduceValue) {
	if (in_Key[KeyCode] >= HOLD_FRAMECOUNT) {
		in_Key[KeyCode] -= reduceValue;
		return true;
	}
	return false;
}