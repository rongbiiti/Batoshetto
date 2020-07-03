#include "InputManager.h"

// �R���X�g���N�^
InputManager::InputManager() {
	// �z�񏉊���
	for (int i = 0; i < 256; i++) {
		in_Key[i] = 0;		// 0�ɖ߂�
	}

	// �X�e�B�b�N�̃f�b�h�]�[����ݒ肷��
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.2);
}

// ���͂��󂯎��֐�
void InputManager::InputKey(void) {
	// �L�[�{�[�h�̑S�L�[��������Ă��邩�ǂ������`�F�b�N���āA�z��ɓ����
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) in_Key[i]++;		// ���͂��ꂽ�L�[�͉�����Ă���Ԃǂ�ǂ񐔎��������Ă����B
		else        in_Key[i] = 0;		// �����ƈ�C��0�ɖ߂�
	}
	InputButton();
}

// Xbox�R���g���[���[�̓��͂��󂯎��֐�
void InputManager::InputButton(void) {
	// XInput�R���g���[���[�̓��͏���z��ɂ����
	GetJoypadXInputState(DX_INPUT_PAD1, &input);

	// �{�^���������A�L�[�{�[�h�Ɠ����悤�Ƀ`�F�b�N����B
	for (int i = 0; i < 16; i++)
	{
		if (i == 10 || i == 11) continue;
		if (input.Buttons[i]) in_Button[i]++;
		else				  in_Button[i] = 0;
		// �f�o�b�O�p�`��
		DrawFormatString(400 + i % 2 * 200, 200 + i / 2 * 16, 0xFFFFFF, "%s:%d", buttonName[i].c_str(), in_Button[i]);
	}

	in_Stick_LX = (float)(input.ThumbLX / 32767.0f);	// �X�e�B�b�N�̌X����short�^�ŕۑ�����Ă���̂ŁA
	in_Stick_LY = (float)(input.ThumbLY / 32767.0f);	// �킩��₷���悤��float�^�Ɍv�Z���Ă���B
	in_Stick_RX = (float)(input.ThumbRX / 32767.0f);
	in_Stick_RY = (float)(input.ThumbRY / 32767.0f);

	// �f�o�b�O�p�`��
	DrawFormatString(900, 0, 0xFFFFFF, "LeftTrigger:%d RightTrigger:%d", input.LeftTrigger, input.RightTrigger);
	DrawFormatString(900, 16, 0xFFFFFF, "ThumbLX:%.4f ThumbLY:%.4f", in_Stick_LX, in_Stick_LY);
	DrawFormatString(900, 32, 0xFFFFFF, "ThumbRX:%.4f ThumbRY:%.4f", in_Stick_RX, in_Stick_RY);
}