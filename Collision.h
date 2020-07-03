// �Փ˔���̏����������Ă���N���X�B
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "DxLib.h"

class Collision {
public:
	// ��̃I�u�W�F�N�g��X��Y�̒��S���W�A���a�̑傫�����󂯎��A��`��������A�������Ă��邩�ۂ���bool�^�ŕԂ��֐��B
	bool IsHit(int x1, int y1, int size1, int x2, int y2, int size2);

	// �I�u�W�F�N�g1��X���S���W���A�I�u�W�F�N�g2�̕��̒��ɂ��邩��Ԃ��֐��B
	// �e���u���b�N�̕��̒��ɂ������𔻒肷�邽�߂Ɏg���Ă���B
	bool IsHitWicth(int x1, int x2, int size2);

	// �I�u�W�F�N�g1��Y���S���W���A�I�u�W�F�N�g2�̍����̒��ɂ��邩��Ԃ��֐��B
	// �e���u���b�N�̍����̒��ɂ������𔻒肷�邽�߂Ɏg���Ă���B
	bool IsHitHeight(int y1, int y2, int size2);
private:

};

#endif // !_COLLISION_H_