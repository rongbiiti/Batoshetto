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

	int* In_Key()		{ return in_Key; }		// �L�[���͂̌��ʂ𑼂��珑���������Ȃ��悤�ɂ��Ă���
	int* In_Button()	{ return in_Button; }	// �{�^�����͂̌��ʂ𑼂��珑���������Ȃ��悤�ɂ��Ă���
	float In_Stick_LX() { return in_Stick_LX; }	// ���X�e�B�b�NX���̒l�𑼂��珑���������Ȃ��悤�ɂ��Ă���
	float In_Stick_LY() { return in_Stick_LY; }	// ���X�e�B�b�NY��Get
	float In_Stick_RX() { return in_Stick_RX; }	// �E�X�e�B�b�NX��Get
	float IN_Stick_RY() { return in_Stick_RY; }	// �E�X�e�B�b�NY��Get

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

	enum BUTTON {
		PAD_UP,
		PAD_DOWN,
		PAD_LEFT,
		PAD_RIGHT,
		START,
		BACK,
		L_PUSH,
		R_PUSH,
		LB,
		RB,
		A = 12,
		B,
		X,
		Y
	};

	// �{�^���̖��O�񋓑�
	
private:
	char Key[256];		// �L�[�{�[�h��������Ă��邩�����̔z��ɑS�ē����B
	XINPUT_STATE input;		//DXLib�Œ�`����Ă���XINPUT�̓��͂��󂯎�邱�Ƃ��ł���\����

	int in_Key[256];			// �ǂ̃L�[�����t���[��������Ă��邩�̌��ʂ��L�����Ă���֐�
	int in_Button[16];			// �ǂ̃{�^�������t���[��������Ă��邩�̌��ʂ��L�����Ă�֐�
	float in_Stick_LX;			// ���X�e�B�b�NX��
	float in_Stick_LY;			// ���X�e�B�b�NY��
	float in_Stick_RX;			// �E�X�e�B�b�NX��
	float in_Stick_RY;			// �E�X�e�B�b�NY��
};

#endif // !_INPUTMANAGER_H_