//�^�C�g���̃N���X
#ifndef _PAUSE_SCREEN_H_
#define _PAUSE_SCREEN_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameMain.h"
#include <string>

class GameMain;
class PauseScreen {
public:
	PauseScreen(FontData* font, InputManager* input, GameMain* main, int pushPLnum);				// �R���X�g���N�^
	~PauseScreen();				// �f�X�g���N�^

	void PauseScreenControll();	// �|�[�Y��ʏ���
	
	void DrawPauseScreen();		// �|�[�Y��ʕ`��

private:
	FontData* fontData;				// ���������t�H���g�f�[�^�N���X�̃|�C���^
	InputManager* inputManager;		// �����������͊Ǘ��N���X�̃|�C���^
	GameMain* gameMain;		// �Q�[���i�s�Ǘ��N���X�̃|�C���^

	void Return_to_Game();		// �|�[�Y��ʂ𔲂��Ď������ĊJ����
	void OpenOptionScreen();	// �I�v�V������ʂ��J��
	void Return_to_Title();		// �����𒆒f���ă^�C�g����ʂ֖߂�

	const static int SELECT_NUM_MAX = 2;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Return to Game",
													   "Option",
													   "Return to Title"
	};

	int selectNum[SELECT_NUM_MAX + 1];				// �I�𒆂̃{�^���ԍ�
	int pausePushPLNum;			// �|�[�Y�{�^�����������v���C���[�̔ԍ� 0=RED 1=BLUE 2=�L�[�{�[�h����̓���

};

#endif // !_PAUSE_SCREEN_H_