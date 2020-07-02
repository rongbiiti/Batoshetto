#include "Block.h"

// �R���X�g���N�^�B�u���b�N�̔ԍ��ƁA�t�H���g�Ǘ��I�u�W�F�N�g�̃|�C���^������B
Block::Block(int num, FontData* font) {
	isAlive = true;						// �����t���O�I��
	x = BlockStartPosition[num][0];		// �ԍ������Ƃ�X���W�����ʒu�����߂�
	y = BlockStartPosition[num][1];		// Y���W�����ʒu
	size = BlockStartPosition[num][2];	// �T�C�Y
	HP = BlockStartHP;					// HP������
	fontData = font;					// �t�H���g�f�[�^�̃|�C���^
}

// �u���b�N��`�悷��֐�
void Block::DrawBlocks(void) {
	if (!isAlive) return;		// �u���b�N������ł����珈���𔲂���
	int dx1, dx2, dy1, dy2;		// �`��p�̃��[�J���ϐ�
	dx1 = x - size / 2;
	dx2 = x + size / 2;
	dy1 = y - size / 2;
	dy2 = y + size / 2;
	
	DrawBox(dx1, dy1, dx2, dy2, 0x7A6611, 1);
	DrawFormatStringToHandle(x - size / 3, y - size / 3, 0xFFFFFF, fontData->f_FontData[1], "%d", HP);
}

// �u���b�N��HP�����炷�֐��BBullet����Ă΂��B
void Block::DecrementBlockHP(void) {
	if (--HP <= 0) {
		// HP���f�N�������g����0�ȉ��Ȃ琶���t���Ofalse�ɂ���
		isAlive = false;
	}
}

Block::~Block() {
	
}