#include "Block.h"
#include "GameMain.h"

// �R���X�g���N�^�B�u���b�N�̔ԍ��ƁA�t�H���g�Ǘ��I�u�W�F�N�g�̃|�C���^������B
Block::Block(int num, FontData* font) {
	isAlive = true;			// �����t���O�I��
	int knt,rem;
	knt =  num / BLOCK_ONE_MAX;
	rem =  num % BLOCK_ONE_MAX;
	//x = BlockStartPosition[knt][0] + num % GameMain::BLOCK_MAX * BLOCK_SIZE;		// �ԍ������Ƃ�X���W�����ʒu�����߂�
	//y = BlockStartPosition[knt][1] + num % GameMain::BLOCK_MAX * BLOCK_SIZE;		// Y���W�����ʒu
	switch (rem)
	{
	case 0:
		x = BlockStartPosition[knt][0];
		y = BlockStartPosition[knt][1];
		break;
	case 1:
		x = BlockStartPosition[knt][0] + 1 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1];
		break;
	case 2:
		x = BlockStartPosition[knt][0] + 2 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1];
		break;
	case 3:
		x = BlockStartPosition[knt][0];
		y = BlockStartPosition[knt][1] + 1 * BLOCK_SIZE;
		break;
	case 4:
		x = BlockStartPosition[knt][0] + 1 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1] + 1 * BLOCK_SIZE;
		break;
	case 5:
		x = BlockStartPosition[knt][0] + 2 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1] + 1 * BLOCK_SIZE;
		break;
	case 6:
		x = BlockStartPosition[knt][0];
		y = BlockStartPosition[knt][1] + 2 * BLOCK_SIZE;
		break;
	case 7:
		x = BlockStartPosition[knt][0] + 1 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1] + 2 * BLOCK_SIZE;
		break;
	case 8:
		x = BlockStartPosition[knt][0] + 2 * BLOCK_SIZE;
		y = BlockStartPosition[knt][1] + 2 * BLOCK_SIZE;
		break;
	}
	if (BlockPosition[knt][rem] == 0) {
		isAlive = false;
	}
	size = BLOCK_SIZE;		// �T�C�Y
	HP = BlockStartHP;		// HP������
	fontData = font;		// �t�H���g�f�[�^�̃|�C���^
	this->num = num;
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
	DrawFormatStringToHandle(x - size / 3 - 10, y, 0xFFFFFF, fontData->f_FontData[0], "HP%d,%d��", HP, num);
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