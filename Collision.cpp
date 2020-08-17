#include "Collision.h"
#include <math.h>

////////////////////////////////////////////////
// ��̃I�u�W�F�N�g��X��Y�̒��S���W�A���a�̑傫�����󂯎��A��`��������A�������Ă��邩�ۂ���bool�^�ŕԂ��֐��B
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// �I�u�W�F�N�g1��X���S���W���A�I�u�W�F�N�g2�̕��̒��ɂ��邩��Ԃ��֐��B
// �e���u���b�N�̕��̒��ɂ������𔻒肷�邽�߂Ɏg���Ă���B
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// �I�u�W�F�N�g1��Y���S���W���A�I�u�W�F�N�g2�̍����̒��ɂ��邩��Ԃ��֐��B
// �e���u���b�N�̍����̒��ɂ������𔻒肷�邽�߂Ɏg���Ă���B
////////////////////////////////////////////////
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

////////////////////////////////////////////////
// �v���C���[�̑_���Ă���������u���b�N�ƏՓ˂��Ă��邩��Ԃ��֐�
////////////////////////////////////////////////
Collision::Vector2 Collision::IsHitTargetAndBlock(float px, float py, float tx, float ty, float bx, float by, float bsize) {

	float left, right, top, bottom;	// �u���b�N�̏㉺���E�̍��W
	Vector2 cross[4];				// ��r�p�̍\���̔z��
	for (int i = 0; i < 4; i++) {
		cross[i].x = -10000;		// �Ԉ���Ă���ԋ߂����W�ɂȂ�Ȃ��悤�ɂ���
		cross[i].y = -10000;
		cross[i].num = 0;
		cross[i].flg = false;
	}
	left = bx - bsize / 2;
	right = bx + bsize / 2;
	top = by - bsize / 2;
	bottom = by + bsize / 2;

	// �n���_�́A���ォ�玞�v���ɁA���ꂼ��2�񂸂n���Ă���B
	if (IsHitLineAndLine(left, top, right, top, px, py, tx, ty)) { cross[0] = position; }		// ��`�̍���ƉE������Ԑ��@���
	if (IsHitLineAndLine(right, top, right, bottom, px, py, tx, ty)) { cross[1] = position; }	// �E��ƉE�������Ԑ��@�@�@�@�E��
	if (IsHitLineAndLine(right, bottom, left, bottom, px, py, tx, ty)) { cross[2] = position; }	// �E���ƍ��������Ԑ��@�@�@�@���
	if (IsHitLineAndLine(left, bottom, left, top, px, py, tx, ty)) { cross[3] = position; }		// �����ƍ�������Ԑ��@�@�@�@����

	// �ǂ̓_����ԃv���C���[�ɋ߂������r����
	position.x = cross[0].x;	// �ŏ��̔z��̒l�������Ă���
	position.y = cross[0].y;
	position.num = 0;
	position.flg = cross[0].flg;

	float distance1 = 0;		// 2�_�Ԃ̋���
	float distance2 = 0;

	for (int i = 1; i < 4; i++) {		// �ŏ��̔z��̒l�͓���Ă���̂�2�Ԗڂ���J�n
		if (!cross[i].flg) continue;	// �L���Ȓl�������Ă��Ȃ�������X�L�b�v
		distance1 = GetPointsDistance(px, py, position.x, position.y);	// �v���C���[�ƏՓ˓_�܂ł̋���
		distance2 = GetPointsDistance(px, py, cross[i].x, cross[i].y);
		if (distance2 <= distance1) {	// ��r�����ق����v���C���[�Ƌ߂�������n���p�̍\���̂̒l���A���̒l�ɍX�V
			position.x = cross[i].x;
			position.y = cross[i].y;
			position.num = i;
			position.flg = true;
		}
	}
	
	return position;
}

////////////////////////////////////////////////
// IsHitTargetAndBlock�ł́A��`�Ɛ�������C�ɓ����蔻�肵�Ă���B
// ���́A��`��4�̐����Ƃ��āA�h�����Ɛ����h�̓����蔻���4��s���Ă���̂��B
// IsHitTargetAndBlock�̒��ŁA���̊֐���4��Ă΂�Ă���B
////////////////////////////////////////////////
bool Collision::IsHitLineAndLine(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2) {
	// B1B2�~B1A1�EB1B2�~B1A2 <= 0 �� A1A2�~A1B1�EA1A2�~A1B2 <= 0 �炵���B

	float result1 = (ax2 - ax1)*(by2 - by1) - (ay2 - ay1)*(bx2 - bx1);
	if (result1 == 0)
		return false; // ���s��������false

	float result2 = ((bx1 - ax1)*(by2 - by1) - (by1 - ay1)*(bx2 - bx1)) / result1;
	float result3 = ((bx1 - ax1)*(ay2 - ay1) - (by1 - ay1)*(ax2 - ax1)) / result1;

	if (result2 < 0.0 || result2 > 1.0)
		return false; // p1��p2�̊ԂɂȂ���_

	if (result3 < 0.0 || result3 > 1.0)
		return false; // p3��p4�̊ԂɂȂ���_

	position.x = ax1 + result2 * (ax2 - ax1);	// �����܂ŏ������ʂ�����A�\���̂ɒl�����Ă����B
	position.y = ay1 + result2 * (ay2 - ay1);
	position.flg = true;

	return true;
}

////////////////////////////////////////////////
Collision::Vector2 Collision::GetHitLineAndLine(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2) {
	Vector2 cross = { -10000, -10000, false };

	float result1 = (ax2 - ax1)*(by2 - by1) - (ay2 - ay1)*(bx2 - bx1);
	if (result1 == 0)
		return cross; // ���s��������false

	float result2 = ((bx1 - ax1)*(by2 - by1) - (by1 - ay1)*(bx2 - bx1)) / result1;
	float result3 = ((bx1 - ax1)*(ay2 - ay1) - (by1 - ay1)*(ax2 - ax1)) / result1;

	if (result2 < 0.0 || result2 > 1.0)
		return cross; // p1��p2�̊ԂɂȂ���_

	if (result3 < 0.0 || result3 > 1.0)
		return cross; // p3��p4�̊ԂɂȂ���_

	cross.x = ax1 + result2 * (ax2 - ax1);	// �����܂ŏ������ʂ�����A�\���̂ɒl�����Ă����B
	cross.y = ay1 + result2 * (ay2 - ay1);
	cross.flg = true;

	return cross;
}

////////////////////////////////////////////////
// 2�_�Ԃ̋������v�Z���ĕԂ��B
////////////////////////////////////////////////
float Collision::GetPointsDistance(float x1, float y1, float x2, float y2) {
	float distance = hypotf(x2 - x1, y2 - y1);

	return distance;
}