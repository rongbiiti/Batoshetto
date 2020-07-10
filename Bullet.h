// �e�Ǘ��N���X
#ifndef _BULLET_H_
#define _BULLET_H_

#include "DxLib.h"
#include "Collision.h"
#include "GameMain.h"

class Bullet {
public:
	Bullet(void);				// �R���X�g���N�^�B�����o�ϐ��̏�����������B�Փ˔�������Ă����N���X����������B
	~Bullet(void);				// �f�X�g���N�^

	void BulletInit(bool alive, float rx, float ry, float ang, GameMain* main);	// �������p�֐�
	void BulletControll(void);	// �e�����ۂɓ����Ƃ��̏���
	void DrawBullet();			// �e��`�悷��

	bool IsAlive() { return isAlive; }						// �e�����݂��Ă��邩��Ԃ�
	int GetRicochetCount() { return ricochetCount; }		// �e�̎c�蒵�e�񐔂�Ԃ�

private:
	Collision* collision;					// �R���X�g���N�^�Ő��������Փ˔��������I�u�W�F�N�g��ۑ����Ă����ϐ�
	GameMain* gameMain;						// �e���ˎ���Player����󂯎����GameMain�N���X�̃|�C���^��ۑ����Ă����ϐ�
	const int BulletMoveSpeed = 20;			// �e�̑��x
	const int BulletRicochetCount = 5;		// �e�̒��e�񐔂̏����l
	const int Size = 4;						// �e�̑傫���i���a�j
	const unsigned int color = 0xFFFFFF;	// �e�̐F

	bool RemainingRicochetTimesCheck(void);	// ���e�񐔂��c���Ă��邩�`�F�b�N����
	bool IsScreenOutside(void);				// ��ʊO�ɏo�Ă��Ȃ����`�F�b�N����
	bool IsHitPlayer(void);					// �v���C���[�Ɠ������Ă��邩�`�F�b�N����
	bool IsHitBlock(void);					// �u���b�N�Ɠ������Ă��邩�`�F�b�N����
	void ChangeAngle(void);					// �e�̐i�s������ύX������֐�

	bool isAlive;			// �e�������Ă��邩
	float x, y;				// ���W x,y
	float moveX, moveY;		// �ړ� x,y
	float preX, preY;		// �ړ��O�̍��W
	int ricochetCount;		// �c�蒵�e��
	int movespeedx, movespeedy;			// �ړ����x
	float angle;				// �p�x
	bool hitFlg;			// �q�b�g�������̃t���O�Btrue�ɂȂ���1�t���[���̂ݓ����蔻��������Afalse�ɂ��ď����𒆒f���Ĕ����邽�߂Ɏg���B
	bool shooterHitOK;		// �������ق��Ƀq�b�g���Ă�������
	float preprex, preprey;
};

#endif // !_BULLET_H_