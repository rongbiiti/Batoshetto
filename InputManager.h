// ���͊Ǘ��N���X
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "DxLib.h"
#include <string>

class InputManager {
public:
	InputManager();			// �R���X�g���N�^
	void InputKey(void);	// ���͂��󂯎��֐�
	void InputButton(void);	// Xbox�R���g���[���[�̓��͂��󂯎��֐�

	int k_Buf[256];			// �ǂ̃L�[��������Ă��邩�̌��ʂ��L�����Ă���֐�
	int k_Button[16];		// �ǂ̃{�^����������Ă��邩�̌��ʂ��L�����Ă�֐�
	float ThumbLX;
	float ThumbLY;
	float ThumbRX;
	float ThumbRY;
	XINPUT_STATE input;
	const std::string buttonName[16] = { "�����p�b�h��",
									     "�����p�b�h��",
									     "�����p�b�h��",
									     "�����p�b�h�E",
									     "START",
									     "BACK",
									     "���X�e�B�b�N��������",
									     "�E�X�e�B�b�N��������",
									     "LB",
									     "RB",
									     "",
									     "",
									     "A",
									     "B",
									     "X",
									     "Y"
	};
private:
	char Key[256];
};

#endif // !_INPUTMANAGER_H_