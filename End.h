//�^�C�g���̃N���X
#ifndef _END_H_
#define _END_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"
#include <string>

class GameMain;
class GameManager;
class End {
public:
	End(FontData* font, InputManager* inputMNG, GameManager* gameMNG);	// �R���X�g���N�^
	~End();					// �f�X�g���N�^
	void EndControll();		// �G���h��ʏ����֐�
	void DrawEnd();			// �G���h��ʕ`��֐�
private:
	FontData* fontData;				// �����������͊Ǘ��N���X�̃|�C���^
	InputManager* inputManager;		// �����������͊Ǘ��N���X�̃|�C���^
	GameManager* gameManager;		// �Q�[���i�s�Ǘ��N���X�̃|�C���^
	GameMain* gameMain;				// �Q�[�����C���N���X�̃|�C���^

	const static int SELECT_NUM_MAX = 1;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "�I��",
													   "������"
	};

	int selectNum[SELECT_NUM_MAX + 1];				// �I�𒆂̃{�^���ԍ�

	void LoadImages();	// �摜�ǂݍ���
	void DeleteImages();	// �摜�f�[�^�������������

	int i_EndImage;	// �w�i�摜
	int i_MenuImage[3];	// ���ږ��摜
};

#endif // !_END_H_