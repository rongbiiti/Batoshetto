#include "Collision.h"

// ��̃I�u�W�F�N�g��X��Y�̒��S���W�A���a�̑傫�����󂯎��A��`��������A�������Ă��邩�ۂ���bool�^�ŕԂ��֐��B
bool Collision::IsHit(int x1, int y1, int size1, int x2, int y2, int size2) {
	int axLeft = x1 - size1 / 2;	// �I�u�W�F�N�g1�̍��[
	int axRight = x1 + size1 / 2;	// �I�u�W�F�N�g1�̉E�[
	int axTop = y1 - size1 / 2;		// �I�u�W�F�N�g1�̏�[
	int axBottom = y1 + size1 / 2;	// �I�u�W�F�N�g1�̉��[

	int bxLeft = x2 - size2 / 2;	// �I�u�W�F�N�g2�̍��[
	int bxRight = x2 + size2 / 2;	// �I�u�W�F�N�g2�̉E�[
	int bxTop = y2 - size2 / 2;		// �I�u�W�F�N�g2�̏�[
	int bxBottom = y2 + size2 / 2;	// �I�u�W�F�N�g2�̉��[

	// ��`�̓����蔻��
	if (bxLeft <= axRight && axLeft <= bxRight && bxTop <= axBottom && axTop <= bxBottom) {
		return true;
	}

	return false;
}

// �I�u�W�F�N�g1��X���S���W���A�I�u�W�F�N�g2�̕��̒��ɂ��邩��Ԃ��֐��B
// �e���u���b�N�̕��̒��ɂ������𔻒肷�邽�߂Ɏg���Ă���B
bool Collision::IsHitWicth(int x1, int x2, int size2) {
	int ax = x1;					// �I�u�W�F�N�g1�̒��SX���W

	int bxLeft = x2 - size2 / 2;	// �I�u�W�F�N�g2�̍��[
	int bxRight = x2 + size2 / 2;	// �I�u�W�F�N�g2�̉E�[

	// �I�u�W�F�N�g1�̒��S���I�u�W�F�N�g2�̕��̒��ɂ��邩���肷��
	if (bxLeft <= ax && ax <= bxRight) {
		return true;
	}

	return false;
}

// �I�u�W�F�N�g1��Y���S���W���A�I�u�W�F�N�g2�̍����̒��ɂ��邩��Ԃ��֐��B
// �e���u���b�N�̍����̒��ɂ������𔻒肷�邽�߂Ɏg���Ă���B
bool Collision::IsHitHeight(int y1, int y2, int size2) {
	int ay = y1;					// �I�u�W�F�N�g1�̒��SY���W

	int bxTop = y2 - size2 / 2;		// �I�u�W�F�N�g2�̏�[
	int bxBottom = y2 + size2 / 2;	// �I�u�W�F�N�g2�̉��[

	// �I�u�W�F�N�g2�̒��S���I�u�W�F�N�g2�̕��̒��ɂ��邩���肷��
	if (bxTop <= ay && ay <= bxBottom) {
		return true;
	}

	return false;
}