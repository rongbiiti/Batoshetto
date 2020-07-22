#include "Block.h"
#include "GameMain.h"

// �R���X�g���N�^�B�u���b�N�̔ԍ��ƁA�t�H���g�Ǘ��I�u�W�F�N�g�̃|�C���^������B
Block::Block(int num, FontData* font ,GameMain* main) {

	int knt,rem;
	int gamemode;
	isAlive = true;			// �����t���O�I��
	gamemain = main;		// �Q�[�����C���̃|�C���^
	size = BLOCK_SIZE;		// �T�C�Y
	HP = BlockStartHP;		// HP������
	fontData = font;		// �t�H���g�f�[�^�̃|�C���^
	gamemode = main->gameManager->GetDifficulty();

	knt =  num / BLOCK_ONE_MAX;		// �u���b�N�̏����ʒu�����ŋ��߂�
	rem =  num % BLOCK_ONE_MAX;		// �R�~�R�u���b�N�̉��Ԗڂ���]��ŋ��߂�

	// �R�~�R�u���b�N�̔ԍ������ɍ��W���w��
	if (rem == 0) {
		switch (gamemode)
		{
		case 1:
			x = BlockStartPosition2[knt][0];
			y = BlockStartPosition2[knt][1];
			break;
		case 2:
			x = BlockStartPosition[knt][0];
			y = BlockStartPosition[knt][1];
			break;
		}
	}
	else if (rem < 3 && rem > 0) {
		x = gamemain->block[num - 1]->GetBlockX() + BLOCK_SIZE;
		y = gamemain->block[num - 1]->GetBlockY();
	}
	else if (rem < 9 && rem >= 3) {
		x = gamemain->block[num - 3]->GetBlockX();
		y = gamemain->block[num - 3]->GetBlockY() + BLOCK_SIZE;
	}


	// �]�肪�O�������ꍇ�R�~�R�u���b�N�̎n�܂�Ȃ̂ŗ����łQ��ނ̂����ǂ���̃u���b�N���g�������߂�
	if (rem == 0) {
		//�_�Ώ̂ɂ��邽�߂ɐ^�񒆈ȍ~�̃u���b�N�͓_�Ώ̂ɂȂ��Ă���u���b�N�Ŏ擾����������������
		if (knt < 5) {
			rnd = GetRand(1);
		}
		else {
			rnd = gamemain->block[((BLOCK_ONE_MAX - 1) - knt) * BLOCK_ONE_MAX]->rnd;
		}
	}
	else {
		//�u���b�N�����ザ��Ȃ��ꍇ�͈�O�̃u���b�N�Ŏ擾������������
		rnd = gamemain->block[num - 1]->rnd;
	}

	//�擾�������������ɂQ��ނ���u���b�N��I��
	switch (rnd)
	{
	case 0:
		if (gamemode == 2) {
			if (BlockPosition[knt][rem] == 0) {
				isAlive = false;
			}
		}
		else if (gamemode == 1) {
			if (BlockPosition_Casual[knt][rem] == 0) {
				isAlive = false;
			}
		}
		break;
	case 1:
		if (gamemode == 2) {
			if (BlockPosition2[knt][rem] == 0) {
				isAlive = false;
			}
		}
		else if (gamemode == 1) {
			if (BlockPosition_Casual[knt][rem] == 0) {
				isAlive = false;
			}
		}
		break;
	}

	if (gamemode == 1) {
		if (num > 44) {
			isAlive = false;
		}
	}

	this->num = num;
}

// �u���b�N��`�悷��֐�
void Block::DrawBlocks(void) {
	if (!isAlive) return;		// �u���b�N������ł����珈���𔲂���
	//�O���b�h���`��
	for (int i = 0; i < GameMain::SCREEN_WIDTH / BLOCK_SIZE; i++) {
		DrawLine(BLOCK_SIZE * i, 0, BLOCK_SIZE * i, GameMain::SCREEN_HEIGHT, 0xffffff);
		DrawLine(0, BLOCK_SIZE * i, GameMain::SCREEN_WIDTH, BLOCK_SIZE * i, 0xffffff);
	}
	int dx1, dx2, dy1, dy2;		// �`��p�̃��[�J���ϐ�
	dx1 = x - size / 2;
	dx2 = x + size / 2;
	dy1 = y - size / 2;
	dy2 = y + size / 2;
	if (HP == 3) {
		DrawBox(dx1, dy1, dx2, dy2, 0x7A6611, 1);
	}
	else if (HP == 2) {
		DrawBox(dx1, dy1, dx2, dy2, 0xFFFF00, 1);
	}
	else if (HP == 1) {
		DrawBox(dx1, dy1, dx2, dy2, 0xFF0000, 1);
	}
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