//�^�C�g���̃N���X
#ifndef _TITLE_H_
#define _TITLE_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"

class GameManager;
class Title {
public:
	void DrawTitle();
private:
	FontData* fontData;				// �����������͊Ǘ��N���X�̃|�C���^
	InputManager* inputManager;		// �����������͊Ǘ��N���X�̃|�C���^
};

#endif // !_TITLE_H_