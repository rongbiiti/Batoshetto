// ���͊Ǘ��N���X
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "DxLib.h"
#include <string>

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

class InputManager {
public:
	InputManager();			// �R���X�g���N�^
	void InputKey(void);	// ���͂��󂯎��֐�
	void InputButton(void);	// Xbox�R���g���[���[�̓��͂��󂯎��֐�

	int* In_Key()		{ return in_Key; }		// �L�[���͂̌��ʂ𑼂��珑���������Ȃ��悤�ɂ��Ă���

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
									     "10",
									     "11",
									     "A",
									     "B",
									     "X",
									     "Y"
	};

	typedef struct PAD_INPUT	// x���W��y���W���܂Ƃ߂ēn����悤�ɂ��邽�߂̍\����
	{
		int in_Button[16];			// �ǂ̃{�^�������t���[��������Ă��邩�̌��ʂ��L�����Ă�֐�
		float in_Stick_LX;			// ���X�e�B�b�NX��
		float in_Stick_LY;			// ���X�e�B�b�NY��
		float in_Stick_RX;			// �E�X�e�B�b�NX��
		float in_Stick_RY;			// �E�X�e�B�b�NY��
	}GamePad;

	bool GetButtonDown(BUTTON buttonCode, int playerNum);	// �w�肵���v���C���[�̃p�b�h�̃{�^�����������u�Ԃ����󂯎��
	bool GetButtonHold(BUTTON buttonCode, int playerNum);	// �w�肵���v���C���[�̃p�b�h�̃{�^�������������Ă��邩���󂯎��
	bool GetButtonHold(BUTTON buttonCode, int playerNum, int reduceValue);	// ���������Ă��邩���󂯎��A�{�^���̉��������b�������炷�B

	bool GetKeyDown(int KeyCode);	// �L�[�{�[�h�̃L�[���������u�Ԃ����󂯎��
	bool GetKeyHold(int KeyCode);	// �L�[�{�[�h�̃L�[�����������Ă��邩���󂯎��
	bool GetKeyHold(int KeyCode, int reduceValue);	// ���������Ă��邩���󂯎��A�L�[�̉��������b�������炷�B

	GamePad* GetPadInput() { return gamePad; }	// �Q�[���p�b�h�̍\���̃|�C���^��n��
	
private:
	const static int HOLD_FRAMECOUNT = 18;

	char Key[256];				// �L�[�{�[�h��������Ă��邩�����̔z��ɑS�ē����B
	XINPUT_STATE input[2];		// DXLib�Œ�`����Ă���XINPUT�̓��͂��󂯎�邱�Ƃ��ł���\����

	int in_Key[256];			// �ǂ̃L�[�����t���[��������Ă��邩�̌��ʂ��L�����Ă���֐�
	GamePad gamePad[2];			// �w�肵���ԍ��̃R���g���[���[�̍\���̂�n��
};

#endif // !_INPUTMANAGER_H_