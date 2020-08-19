// �X�e�[�W��ɔz�u����u���b�N���Ǘ�����N���X
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "DxLib.h"
#include "FontData.h"
#include "GameMain.h"

class GameMain;
class Block {
public:
	Block(int num, FontData* font, GameMain* main);				// �R���X�g���N�^�B�u���b�N�̔ԍ��ƁA�t�H���g�Ǘ��I�u�W�F�N�g�̃|�C���^������B
	~Block();									// �f�X�g���N�^�B
	void DrawBlocks(void);						// �u���b�N��`�悷��֐�
	const static int BLOCK_SIZE = 80;			// �u���b�N�T�C�Y
	const static int BLOCK_ONE_MAX = 9;			// �P�u���b�N���̍ő吔�A�G�L�X�p�[�g�̃u���b�N�ő吔
	const static int BLOCK_Casual_MAX = 5;		// �J�W���A���u���b�N�̍ő吔

	bool IsAlive(void) { return isAlive; }		// �u���b�N�����݂��Ă��邩��Ԃ��B
	int GetBlockX(void) { return x; }			// �u���b�N��X���S���W��Ԃ��B
	int GetBlockY(void) { return y; }			// �u���b�N��Y���S���W��Ԃ��B
	int GetBlockSize(void) { return size; }		// �u���b�N�̒��a��Ԃ��B
	void ReadFile(void);

	void DecrementBlockHP(void);				// �u���b�N��HP�����炷�֐��BBullet����Ă΂��B
private:
	// X�AY,
	const int BlockStartPosition[9][2] = { {280,40}, {600,40},{840,40},{280,280},{600,280},{840,280},{280,520}, {520,520},{840,520}, };
	const int BlockStartPosition2[5][2] = { {280,40}, {840,40},{520,280},{280,520},{840,520},};
	
	int BlockPosition[BLOCK_ONE_MAX][BLOCK_ONE_MAX] = {};				// �R�~�R�u���b�N�̎��(�G�L�X�p�[�g�p)
	int BlockPosition2[BLOCK_ONE_MAX][BLOCK_ONE_MAX] = {};				// �R�~�R�u���b�N�̎�ނQ(�G�L�X�p�[�g�p)
	int BlockPosition_Casual[BLOCK_Casual_MAX][BLOCK_ONE_MAX] = {};		// �R�~�R�u���b�N�̎��(�J�W���A���p)
	int BlockPosition_Casual2[BLOCK_Casual_MAX][BLOCK_ONE_MAX] = {};	// �R�~�R�u���b�N�̎�ނQ(�J�W���A���p)

	// X�AY�A�T�C�Y�̏��B
	const int BlockStartHP = 3;		// �u���b�N�̏���HP

	FontData* fontData;				// �R���X�g���N�^�Ŏ󂯎�����t�H���g�Ǘ��I�u�W�F�N�g�̃|�C���^��ۑ����Ă����ϐ�
	GameMain* gamemain;				// �R���X�g���N�^�Ŏ󂯎�����Q�[�����C���̃|�C���^��ۑ����Ă����ϐ�

	bool isAlive;			// �����Ă��邩		
	int x, y;				// ���W x,y
	int size;				// �傫��
	int HP;					// �u���b�N��HP
	int num;
	int rnd;				// �u���b�N�̎��
	FILE *fp;				// �t�@�C���p

	int breakAnimWaitTime;	// �j��A�j���̑҂�����
	float breakAngle;		// �j��A�j���̊p�x
	
};

#endif // !_BLOCK_H_