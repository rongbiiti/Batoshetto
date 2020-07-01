#include "InputManager.h"

// �R���X�g���N�^
InputManager::InputManager() {
	// �z�񏉊���
	for (int i = 0; i < 256; i++) {
		k_Buf[i] = 0;		// 0�ɖ߂�
	}

	// �X�e�B�b�N�̃f�b�h�]�[����ݒ肷��
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.2);
}

// ���͂��󂯎��֐�
void InputManager::InputKey(void) {
	// �L�[�{�[�h�̑S�L�[��������Ă��邩�ǂ������`�F�b�N���āA�z��ɓ����
	GetHitKeyStateAll(Key);
	for (int i = 0; i < 256; i++) {
		if (Key[i]) k_Buf[i]++;		// ���͂��ꂽ�L�[�͉�����Ă���Ԃǂ�ǂ񐔎��������Ă����B
		else        k_Buf[i] = 0;		// �����ƈ�C��0�ɖ߂�
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
		if (input.Buttons[i]) k_Button[i]++;
		else				  k_Button[i] = 0;
		// �f�o�b�O�p�`��
		DrawFormatString(400 + i % 2 * 200, 200 + i / 2 * 16, 0xFFFFFF, "%s:%d", buttonName[i].c_str(), k_Button[i]);
	}

	ThumbLX = (float)(input.ThumbLX / 32767.0f);	// �X�e�B�b�N�̌X����short�^�ŕۑ�����Ă���̂ŁA
	ThumbLY = (float)(input.ThumbLY / 32767.0f);	// �킩��₷���悤��float�^�Ɍv�Z���Ă���B
	ThumbRX = (float)(input.ThumbRX / 32767.0f);
	ThumbRY = (float)(input.ThumbRY / 32767.0f);

	// �f�o�b�O�p�`��
	DrawFormatString(900, 0, 0xFFFFFF, "LeftTrigger:%d RightTrigger:%d", input.LeftTrigger, input.RightTrigger);
	DrawFormatString(900, 16, 0xFFFFFF, "ThumbLX:%.4f ThumbLY:%.4f", ThumbLX, ThumbLY);
	DrawFormatString(900, 32, 0xFFFFFF, "ThumbRX:%.4f ThumbRY:%.4f", ThumbRX, ThumbRY);
}