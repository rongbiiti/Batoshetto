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
	float ThumbLX;			// ���X�e�B�b�NX��
	float ThumbLY;			// ���X�e�B�b�NY��
	float ThumbRX;			// �E�X�e�B�b�NX��
	float ThumbRY;			// �E�X�e�B�b�NY��

	XINPUT_STATE input;		//DXLib�Œ�`����Ă���XINPUT�̓��͂��󂯎�邱�Ƃ��ł���\����

	// �{�^���̖��O
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
	char Key[256];		// �L�[�{�[�h��������Ă��邩�����̔z��ɑS�ē����B
};

#endif // !_INPUTMANAGER_H_