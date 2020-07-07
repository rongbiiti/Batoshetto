// �v���C���[�Ǘ��N���X
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Collision.h"
#include "GameMain.h"

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
	Collision* collision;	// ���������Փ˔��肷��N���X�̃|�C���^

	const int PlayerMoveSpeed = 5;						// �v���C���[�̈ړ����x
	const int PlayerStartPositionX[2] = { 60, 1100 };	// �v���C���[�̏���X���W
	const int PlayerStartPositionY[2] = { 200, 600 };	// �v���C���[�̏���Y���W
	const int PlayerSize = 80;							// �v���C���[�̒��a�T�C�Y

	float AngleCorrection(float ang);			// �p�x��0�`360�x�Ɏ��܂�悤�ɒ���
	int XCoordinateCorrection(int posx, int pnum, int size);	// X���W���w��͈̔͂𒴂��Ă��Ȃ����A�v���C���[�̔ԍ������ƂɏC��
	int YCoordinateCorrection(int posy, int size);				// Y���W���w��͈̔͂𒴂��Ȃ��悤�ɏC��

	void BlockHitCheck(void);					// �u���b�N�Ɠ����蔻��
	void TargetPointWindowHitCheck(void);		// �e���i�ޕ������E�B���h�E���яo���ĂȂ����`�F�b�N����
	bool TrajectoryPrecalculation_ToBlock(int* blocknum);	// �u���b�N�ɑ΂���e���̎��O�v�Z�B
	int TrajectoryPrecalculation_ToWindow(void);	// �E�B���h�E�ɑ΂���e���̎��O�v�Z�B
	void CalcHitAfterAngle_ToBlock(int blocknum);	// �u���b�N�̂ǂ̕ӂƏՓ˂��Ă��邩���f���Ċp�x��ύX����
	void CalcHitAfterAngle_ToWindow(int num);		// �E�B���h�E�̂ǂ̒[�ƏՓ˂��Ă��邩���f���Ċp�x��ύX����

	GameMain* gameMain;			// �R���X�g���N�^�Ŏ󂯎����GameMain�̃|�C���^��ۑ����Ă����ϐ�
	InputManager* inputManager;	// ���͊Ǘ��N���X�̃|�C���^������ϐ�
	int x, y;					// ���W x,y
	float targetx, targety, targetx2, targety2;		// �Ə��̍��W
	int size;					// �T�C�Y���a
	unsigned int color;			// �F
	int moveSpeed;				// �ړ����x
	bool isShooter;				// �������A�ۂ��B
	float angle, angle2;		// �p�x
	int preX, preY;				// �ړ��O�̍��W
	int num;					// RED��BLUE��
};

#endif // !_PLAYER_H_