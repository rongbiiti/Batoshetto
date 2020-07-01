// ���U���g��ʕ`��p�N���X
#ifndef _RESULT_H_
#define _RESULT_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"

class GameManager;
class Result {
public:
	// �t�H���g�f�[�^�Ǘ��I�u�W�F�N�g�̃|�C���^�A���͊Ǘ��I�u�W�F�N�g�̃|�C���^�A
	// �Q�[���i�s�Ǘ��I�u�W�F�N�g�̃|�C���^�A�q�b�g�����v���C���[��RED��BLUE�����󂯎��B
	Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum);
	~Result();					// �f�X�g���N�^
	void DrawResult(void);		// �`��
private:
	FontData* fontData;			// �t�H���g�f�[�^�Ǘ��I�u�W�F�N�g�̃|�C���^
	InputManager* inputManager;	// ���͊Ǘ��I�u�W�F�N�g�̃|�C���^
	GameManager* gameManager;	// �Q�[���i�s�Ǘ��I�u�W�F�N�g�̃|�C���^
	int hitPlayerNumber;		// �q�b�g�����v���C���[��RED��BLUE���̈�����ۑ����Ă����ϐ��B
};

#endif // !_RESULT_H_