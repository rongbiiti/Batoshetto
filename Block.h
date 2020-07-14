// �X�e�[�W��ɔz�u����u���b�N���Ǘ�����N���X
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "DxLib.h"
#include "FontData.h"

class Block {
public:
	Block(int num, FontData* font);				// �R���X�g���N�^�B�u���b�N�̔ԍ��ƁA�t�H���g�Ǘ��I�u�W�F�N�g�̃|�C���^������B
	~Block();									// �f�X�g���N�^�B
	void DrawBlocks(void);						// �u���b�N��`�悷��֐�
	const static int BLOCK_SIZE = 80;			// �u���b�N�T�C�Y
	const static int BLOCK_ONE_MAX = 9;

	bool IsAlive(void) { return isAlive; }		// �u���b�N�����݂��Ă��邩��Ԃ��B
	int GetBlockX(void) { return x; }			// �u���b�N��X���S���W��Ԃ��B
	int GetBlockY(void) { return y; }			// �u���b�N��Y���S���W��Ԃ��B
	int GetBlockSize(void) { return size; }		// �u���b�N�̒��a��Ԃ��B

	void DecrementBlockHP(void);				// �u���b�N��HP�����炷�֐��BBullet����Ă΂��B
private:
	// X�AY�A�T�C�Y�̏��B
	const int BlockStartPosition[9][2] = { {400,100}, {200,300},{400,500},{800,100},{1000,300},{800,500} };

	const int BlockPosition[6][BLOCK_ONE_MAX] = {  {1,1,1,
													1,0,1,
													1,1,1},
												   {0,0,0,
													1,0,0,
													1,1,0}, 
												   {0,1,1,
													0,0,1,
													0,0,0},
												   {0,1,0,
													0,1,0,
													0,1,0},
												   {1,1,0,
													1,1,0,
													0,0,0},
												   {1,1,1,
													0,0,0,
													0,0,0}, };
	// X�AY�A�T�C�Y�̏��B
	const int BlockStartHP = 3;		// �u���b�N�̏���HP

	FontData* fontData;				// �R���X�g���N�^�Ŏ󂯎�����t�H���g�Ǘ��I�u�W�F�N�g�̃|�C���^��ۑ����Ă����ϐ�

	bool isAlive;			// �����Ă��邩		
	int x, y;				// ���W x,y
	int size;				// �傫��
	int HP;					// �u���b�N��HP
	int num;

};

#endif // !_BLOCK_H_