// �v���C���[�Ǘ��N���X
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameMain.h"
#include "Collision.h"

class GameMain;
class Player {
public:
	// �R���X�g���N�^�BRED��BLUE���A�F�A�������ۂ��AGameMain�I�u�W�F�N�g�̃|�C���^�������Ŏ󂯎��B
	Player(int num, unsigned int color, bool shooter, GameMain* main);
	~Player();							// �f�X�g���N�^
	void HidingPlayerControll(void);	// �B��鑤���̑��쏈��
	void ShooterPlayerControll(void);	// �������̑��쏈��

	void DrawPlayer(void);				// �`��p
	void DrawTargetAngle(void);			// �������ɑ_���Ă�������ɐ��������ĕ`�悷��
	void SetIsShooter(bool value) { isShooter = value; }		// �����o�ϐ�isShooter�ɒl���Z�b�g����

	bool GetIsShooter(void) { return isShooter; }		// ���݌������ۂ���Ԃ�
	int GetPlayerX(void) { return x; }					// �v���C���[��X���S���W��Ԃ�
	int GetPlayerY(void) { return y; }					// �v���C���[��Y���S���W��Ԃ�
	int GetPlayerSize(void) { return size; }			// �v���C���[�̒��a�T�C�Y��Ԃ�
	int GetPlayerNum(void) { return num; }				// �v���C���[��RED��BLUE����Ԃ�

private:
	Collision* collision;			// ���������Փ˔��肷��N���X�̃|�C���^

	const int PlayerMoveSpeed = 3;						// �v���C���[�̈ړ����x
	const int PlayerStartPositionX[2] = { 60, 1100 };	// �v���C���[�̏���X���W
	const int PlayerStartPositionY[2] = { 200, 600 };	// �v���C���[�̏���Y���W
	const int PlayerSize = 18;		// �v���C���[�̒��a�T�C�Y

	void BlockHitCheck(void);		// �u���b�N�Ɠ����蔻��

	GameMain* gameMain;			// �R���X�g���N�^�Ŏ󂯎����GameMain�̃|�C���^��ۑ����Ă����ϐ�
	InputManager* inputManager;	// ���͊Ǘ��N���X�̃|�C���^������ϐ�
	int x, y;				// ���W x,y
	float targetx, targety;	// �Ə��̍��W
	int size;				// �T�C�Y���a
	unsigned int color;		// �F
	int moveSpeed;			// �ړ����x
	bool isShooter;			// �������A�ۂ��B
	int angle;				// �p�x
	int preX, preY;			// �ړ��O�̍��W
	int num;				// RED��BLUE��
};

#endif // !_PLAYER_H_