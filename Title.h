//�^�C�g���̃N���X
#ifndef _TITLE_H_
#define _TITLE_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"
#include "TitleBullet.h"
#include <string>

class GameMain;
class GameManager;
class TitleBullet;
class Title {
public:
	Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG);		// �R���X�g���N�^
	~Title();						// �f�X�g���N�^
	void TitleControll();			// �����p�֐�
	void DrawTitle();				// �`��p�֐�

	TitleBullet* titleBullet;		// �w�i�Ŕ��˂��Ă���e�̕`��N���X�|�C���^

private:
	FontData* fontData;				// ���������t�H���g�Ǘ��N���X�̃|�C���^
	InputManager* inputManager;		// �����������͊Ǘ��N���X�̃|�C���^
	GameManager* gameManager;		// �Q�[���i�s�Ǘ��N���X�|�C���^
	GameMain* gameMain;				// �Q�[�����C���N���X�̃|�C���^

	void HelpScreenControll();	// �w���v��ʑ���
	void DrawHelpScreen();		// �w���v��ʕ`��
	void ProcessingOnDetermining(int controllerNum);	// ����{�^���������̏����B�����́A�R���g���[���[�̔ԍ�

	int backAnimWaitTime;		// �w�i�̒��e�A�j���̑҂�����

	void LoadImages();		// �摜�ǂݍ���

	const static int SELECT_NUM_MAX = 4;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Start",
													   "Online",
													   "Option",
													   "Help",
													   "End"
	};

	int selectNum[3];				// �I�𒆂̃{�^���ԍ�
	bool isOpenHelpScreen;		// �w���v��ʂ��J���Ă��邩

	int i_helpImage;	// �w���v�摜
	int i_LogoImage;	// �^�C�g�����S�摜
	int i_BGImage;		// �w�i�摜
	int i_MenuImage[5];	// ���ږ��摜
};

#endif // !_TITLE_H_