// �Փ˔���̏����������Ă���N���X�B
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "DxLib.h"

class Collision {
public:

	typedef struct POINT	// x���W��y���W���܂Ƃ߂ēn����悤�ɂ��邽�߂̍\����
	{
		float x, y;	// ���W x,y
		int num;	// �ӂ̔ԍ�
		bool flg;	// �L���Ȑ��l�������Ă��邩
	}Vector2;

	// ��̃I�u�W�F�N�g��X��Y�̒��S���W�A���a�̑傫�����󂯎��A��`��������A�������Ă��邩�ۂ���bool�^�ŕԂ��֐��B
	bool IsHit(int x1, int y1, int size1, int x2, int y2, int size2);

	// �I�u�W�F�N�g1��X���S���W���A�I�u�W�F�N�g2�̕��̒��ɂ��邩��Ԃ��֐��B
	// �e���u���b�N�̕��̒��ɂ������𔻒肷�邽�߂Ɏg���Ă���B
	bool IsHitWicth(int x1, int x2, int size2);

	// �I�u�W�F�N�g1��Y���S���W���A�I�u�W�F�N�g2�̍����̒��ɂ��邩��Ԃ��֐��B
	// �e���u���b�N�̍����̒��ɂ������𔻒肷�邽�߂Ɏg���Ă���B
	bool IsHitHeight(int y1, int y2, int size2);

	// �v���C���[�̑_���Ă���������u���b�N�ƏՓ˂����ł��v���C���[�ɋ߂��Փ˓_��Ԃ�
	// �����́A�v���C���[�̒��SX�AY�A�^�[�Q�b�gX�AY�A�u���b�N�̒��SX�AY�A���a�T�C�Y�B
	Vector2 IsHitTargetAndBlock(float px, float py, float tx, float ty, float bx, float by, float bsize);

	// ���Ɛ����Փ˂��Ă��邩�Ԃ�
	Vector2 GetHitLineAndLine(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2);
	
private:
	// IsHitTargetAndBlock�ł́A��`�Ɛ�������C�ɓ����蔻�肵�Ă���B
	// ���́A��`��4�̐����Ƃ��āA�h�����Ɛ����h�̓����蔻���4��s���Ă���̂��B
	// IsHitTargetAndBlock�̒��ŁA���̊֐���4��Ă΂�Ă���B
	bool IsHitLineAndLine(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2);

	// 2�_�Ԃ̋������v�Z���ĕԂ��B
	float GetPointsDistance(float x1, float y1, float x2, float y2);

	Vector2 position = { 0, 0, 0, false };	// �\���̐���

};

#endif // !_COLLISION_H_