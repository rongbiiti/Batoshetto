// ���͊Ǘ��N���X
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "DxLib.h"

class InputManager {
public:
	InputManager();			// �R���X�g���N�^
	void InputKey(void);	// ���͂��󂯎��֐�

	int k_Buf[256];			// �ǂ̃L�[��������Ă��邩�̌��ʂ��L�����Ă���֐�
private:
	char Key[256];
};

#endif // !_INPUTMANAGER_H_