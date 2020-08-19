#include "Block.h"
#include "GameMain.h"
#pragma warning(disable : 4996)

void Block::ReadFile(void){
	int c;
	bool flg = FALSE;
	char buf[100] = {0};

	//�t�@�C����ǂݎ���p�ŊJ���i�G�L�X�p�[�g�p�j
	fp = fopen("Block.csv","r");

	//�t�@�C���I�[�v���Ɏ��s������G���[���b�Z�[�W���o��
	if (fp == NULL) {
		printfDx("read error\n");
		return;
	}

	//�u���b�N�����t�@�C������ǂݎ��A�z��ɏ������ށi�G�L�X�p�[�g�p�j
	for (int i = 0; i < BLOCK_ONE_MAX * 2; i++) {
		for (int j = 0; j < BLOCK_ONE_MAX; j++)
		{
			c = fgetc(fp);
			//','��'\n'�i���s�j�Ȃ�buf�ɕ����A�����ēY������j�����߂�
			if (c != ',' && c != '\n') {
				strcat(buf, (const char*)&c);
				j--;
			}
			else
			{
				//','��'\n'�i���s�j����Ȃ��ꍇ�͂��ꂼ��Ή������z���atoi�֐��ő��
				if (!flg) {
					BlockPosition[i][j] = atoi(buf);
				}
				else if(i >= BLOCK_ONE_MAX)
				{
					flg = TRUE;
					BlockPosition2[i / BLOCK_ONE_MAX][j] = atoi(buf);
				}
				//�����A������buf��������
				memset(buf, 0, sizeof(buf));
			}
		}
	}
	flg = FALSE;
	//�t�@�C�������
	fclose(fp);

	//�t�@�C����ǂݎ���p�ŊJ���i�J�W���A���p�j
	fp = fopen("Block_Casual.csv", "r");

	//�t�@�C���I�[�v���Ɏ��s������G���[���b�Z�[�W���o��
	if (fp == NULL) {
		printfDx("read error\n");
		return;
	}

	//�u���b�N�����t�@�C������ǂݎ��A�z��ɏ������ށi�J�W���A���p�j
	for (int i = 0; i < BLOCK_Casual_MAX * 2; i++) {
		for (int j = 0; j < BLOCK_ONE_MAX; j++)
		{
			c = fgetc(fp);
			//','��'\n'�i���s�j�Ȃ�buf�ɕ����A�����ēY������j�����߂�
			if (c != ',' && c != '\n') {
				strcat(buf, (const char*)&c);
				j--;
			}
			else
			{
				//','��'\n'�i���s�j����Ȃ��ꍇ�͂��ꂼ��Ή������z���atoi�֐��ő��
				if (!flg) {
					BlockPosition_Casual[i][j] = atoi(buf);
				}
				else if (i >= BLOCK_Casual_MAX)
				{
					flg = TRUE;
					BlockPosition_Casual2[i / BLOCK_Casual_MAX][j] = atoi(buf);
				}
				//�����A������buf��������
				memset(buf, 0, sizeof(buf));
			}
		}
	}
	flg = FALSE;
	//�t�@�C�������
	fclose(fp);

}

// �R���X�g���N�^�B�u���b�N�̔ԍ��ƁA�t�H���g�Ǘ��I�u�W�F�N�g�̃|�C���^������B
Block::Block(int num, FontData* font ,GameMain* main) {

	int knt,rem,gamemode;
	ReadFile();	//�t�@�C������u���b�N����ǂݎ��
	isAlive = true;			// �����t���O�I��
	gamemain = main;		// �Q�[�����C���̃|�C���^
	size = BLOCK_SIZE;		// �T�C�Y
	HP = BlockStartHP;		// HP������
	fontData = font;		// �t�H���g�f�[�^�̃|�C���^
	gamemode = main->gameManager->GetDifficulty();	// ��Փx

	breakAnimWaitTime = 0;
	breakAngle = 0;

	knt =  num / BLOCK_ONE_MAX;		// �u���b�N�̏����ʒu�����ŋ��߂�
	rem =  num % BLOCK_ONE_MAX;		// �R�~�R�u���b�N�̉��Ԗڂ���]��ŋ��߂�

	// �R�~�R�u���b�N�̔ԍ������ɍ��W���w��
	if (rem == 0) {
		switch (gamemode)
		{
		//�Q�[�����[�h���J�W���A��
		//����̃u���b�N�͏������W����
		case 1:
			x = BlockStartPosition2[knt][0];
			y = BlockStartPosition2[knt][1];
			break;
		//�Q�[�����[�h���G�L�X�p�[�g
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
		//�G�L�X�p�[�g
		if (gamemode == 2) {
			if (knt < 5) {
				rnd = GetRand(1);
			}
			else {
				rnd = gamemain->block[((BLOCK_ONE_MAX - 1) - knt) * BLOCK_ONE_MAX]->rnd;
			}
		//�J�W���A��
		}else if (gamemode == 1 && num <= 44) {
			if (knt < 3) {
				rnd = GetRand(1);
			}
			else {
				rnd = gamemain->block[((BLOCK_ONE_MAX - 5) - knt) * BLOCK_ONE_MAX]->rnd;
			}
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
				breakAnimWaitTime = 61;
			}
		}
		else if (gamemode == 1) {
			if (BlockPosition_Casual[knt][rem] == 0) {
				isAlive = false;
				breakAnimWaitTime = 61;
			}
		}
		break;
	case 1:
		if (gamemode == 2) {
			if (BlockPosition2[knt][rem] == 0) {
				isAlive = false;
				breakAnimWaitTime = 61;
			}
		}
		else if (gamemode == 1) {
			if (BlockPosition_Casual2[knt][rem] == 0) {
				isAlive = false;
				breakAnimWaitTime = 61;
			}
		}
		break;
	}

	//�Q�[�����[�h�ɂ����čő�u���b�N���𒴂��Ă���z��̃u���b�N�͏���
	if (gamemode == 1) {
		if (num > 44) {
			isAlive = false;
			breakAnimWaitTime = 61;
		}
	}

	this->num = num;
}

// �u���b�N��`�悷��֐�
void Block::DrawBlocks(void) {
	if (isAlive) {
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
		DrawRotaGraph(x, y, 1, 0, gamemain->GetBlockImage(HP - 1), true);
	}
	else {
		if (60 < breakAnimWaitTime) return;	// �A�j���I����Ă���`�悵�Ȃ�
		++breakAnimWaitTime;

		int dx1, dx2, dy1, dy2;		// �`��p�̃��[�J���ϐ�
		dx1 = x - size / 4;
		dx2 = x + size / 4;
		dy1 = y - size / 4;
		dy2 = y + size / 4;

		if (breakAnimWaitTime % 10 == 0) {
			breakAngle += (90.0f * 3.14f) / 180.0f;
		}

		DrawRotaGraph(dx1 - (breakAnimWaitTime / 1.5), dy1 - (breakAnimWaitTime / 2), 0.8f, breakAngle, gamemain->GetBreakBlockImage(0), true);	// ����
		DrawRotaGraph(dx2 + (breakAnimWaitTime / 1.5), dy1 - (breakAnimWaitTime / 2), 0.8f, breakAngle, gamemain->GetBreakBlockImage(1), true);		// �E��
		DrawRotaGraph(dx1 - (breakAnimWaitTime / 1.5), dy2 + (breakAnimWaitTime / 2), 0.8f, breakAngle, gamemain->GetBreakBlockImage(2), true);		// ����
		DrawRotaGraph(dx2 + (breakAnimWaitTime / 1.5), dy2 + (breakAnimWaitTime / 2), 0.8f, breakAngle, gamemain->GetBreakBlockImage(3), true);		// �E��
	}
	
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