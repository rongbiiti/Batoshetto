// ���间��PresentsUI�N���X
#ifndef _UI_H_
#define _UI_H_

#include "DxLib.h"
#include "GameMain.h"

class GameMain;	// �N���X�̃v���g�^�C�v�錾
class UI {
public:
	UI(GameMain* main);	// �R���X�g���N�^
	~UI();	// �f�X�g���N�^

	bool TransitionAnimationWaiting();	// �B���E���̐ؑ֎��̃A�j���[�V�����̏���
	void DrawTransitionAnimation();		// �B���E���̐ؑ֎��̃A�j���[�V����
	void TransitionParameterReset();	// �J�ڃA�j���Ɏg���ϐ����Z�b�g

	bool GetTransitionFlg() {return transitionFlg;}// �J�ڃA�j�������̃t���O��Ԃ�

	// �v���C���[�̎c�莞�Ԃ��Q�[�W�ŕ`�悷��֐�
	void DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum);
	void DrawBackGround();	// �w�i�摜�`��
	void DeleteImages();// �摜�f�[�^�������������

private:
	GameMain* gameMain;		// �Q�[�����C���N���X�̃|�C���^
	GameManager* gameManager;	// �Q�[���i�s�Ǘ��N���X�̃|�C���^
	FontData* fontData;

	bool isBattleStart;	// �������J�n�������ォ
	int firstAnimWaitTime;	// �����J�n���̃A�j���̑҂�����
	int firstAnimX;			// �����J�n���A�j���̕�����X���W
	int firstAnimY;			// �����J�n���A�j���̕�����Y���W
	int firstAnimAlpha;		// �����J�n���A�j���̓����x

	bool BattleStartAnim();	// �����J�n���̃A�j���B���o���I����Ă�����FALSE��Ԃ�
	void DrawBattleStartAnim();	// �����J�n���̃A�j���`��

	int animationWaitingTime;	// �A�j���҂�����
	int transitionX;	// �J�ڗpX���W
	int transitionY;	// �J�ڗpY���W
	double transitionExRate;	// �w���摜�̑傫��
	bool transitionFlg;	// �J�ڃA�j������

	void LoadImages();	// �摜�ǂݍ���
	
	int i_PlayerGuage[2];	// �Q�[�W�̉摜
	int i_BackGroundImage;	// �w�i�摜
	int i_OrderImage[2][2];	// �����I�Ȃǂ̎w���摜
};

#endif // !_UI_H_