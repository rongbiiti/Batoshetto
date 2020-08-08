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
	Result(FontData* font, InputManager* input, GameManager* gameMNG);
	Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum);
	~Result();					// �f�X�g���N�^
	void ResultControll(void);	// ����
	void ResultControll_Net(void);	// �l�b�g�ΐ�p�̃��U���g����
	void DrawResult(void);		// �`��
	void DrawTimeOut(void);		// �^�C���A�E�g�������Ƃ�`��
private:
	FontData* fontData;			// �t�H���g�f�[�^�Ǘ��I�u�W�F�N�g�̃|�C���^
	InputManager* inputManager;	// ���͊Ǘ��I�u�W�F�N�g�̃|�C���^
	GameManager* gameManager;	// �Q�[���i�s�Ǘ��I�u�W�F�N�g�̃|�C���^
	int hitPlayerNumber;		// �q�b�g�����v���C���[��RED��BLUE���̈�����ۑ����Ă����ϐ��B

	void Return_to_Game();		// �|�[�Y��ʂ𔲂��Ď������ĊJ����
	void Return_to_Title();		// �����𒆒f���ă^�C�g����ʂ֖߂�

	const static int SELECT_NUM_MAX = 1;
	const static int SCENE_TRANSITION_WAITING_TIME = 60;	// �V�[���J�ڂ���Ƃ��̂�����Ƃ����҂�����
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "ReMatch",
													   "Return to Title"
	};

	int waitTime;
	int selectNum[SELECT_NUM_MAX + 1];	// �I�𒆂̃{�^���ԍ�
	bool dicideNumFlg[2];				// �ԍ������肵�����̃t���O
};

#endif // !_RESULT_H_