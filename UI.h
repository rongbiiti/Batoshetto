// ���间��PresentsUI�N���X
#ifndef _UI_H_
#define _UI_H_

#include "DxLib.h"
#include "GameMain.h"

class GameMain;	// �N���X�̃v���g�^�C�v�錾
class UI {
public:
	UI(GameMain* main);	// �R���X�g���N�^

	void UIControll();	// �p�����[�^��ύX����

	// �v���C���[�̎c�莞�Ԃ��Q�[�W�ŕ`�悷��֐�
	void DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum);

private:
	GameMain* gameMain;		// �Q�[�����C���N���X�̃|�C���^

	void LoadImages();	// �摜�ǂݍ���

	int i_PlayerGuage[2];	// �Q�[�W�̉摜
};

#endif // !_UI_H_