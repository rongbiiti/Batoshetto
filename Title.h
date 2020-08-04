//�^�C�g���̃N���X
#ifndef _TITLE_H_
#define _TITLE_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"
#include <string>

class GameMain;
class GameManager;
class Title {
public:
	Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG);
	~Title();
	void TitleControll();
	void DrawTitle();
private:
	FontData* fontData;				// �����������͊Ǘ��N���X�̃|�C���^
	InputManager* inputManager;		// �����������͊Ǘ��N���X�̃|�C���^
	GameManager* gameManager;
	GameMain* gameMain;

	//void Start();		// �������ĊJ����
	//void OpenOptionScreen();	// �I�v�V������ʂ��J��
	//void EndSocreen();					// �Q�[�����I������

	const static int SELECT_NUM_MAX = 3;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Start",
													   "Online",
													   "Option",
													   "End"
	};

	int selectNum[SELECT_NUM_MAX + 1];				// �I�𒆂̃{�^���ԍ�
};

#endif // !_TITLE_H_