//�^�C�g����ʂ̔w�i�̒e�̃N���X
#ifndef _TITLEBULLET_H_
#define _TITLEBULLET_H_

#include "DxLib.h"
#include "FontData.h"
#include "Effect.h"
#include "Collision.h"

class Effect;
class TitleBullet {
public:
	TitleBullet(FontData* font);
	~TitleBullet();
	void BulletInit(bool alive, int randx, int randy);	// �������p�֐�
	void TitleBulletControll();
	void DrawTitleBullet();
	void ChangeVolume(float persent);	// ���ʕύX

	bool GetIsAlive() { return isAlive; }		// �e�������Ă��邩�Ԃ�

private:
	FontData* fontData;				// �����������͊Ǘ��N���X�̃|�C���^
	Collision* collision;					// �R���X�g���N�^�Ő��������Փ˔��������I�u�W�F�N�g��ۑ����Ă����ϐ�

	const float BulletMoveSpeed_X = 16.0f;			// �e�̑��x_X
	const float BulletMoveSpeed_Y = 16.0f;			// �e�̑��x_Y
	const int BulletRicochetCount = 5;		// �e�̒��e�񐔂̏����l
	const int Size = 4;						// �e�̑傫���i���a�j
	const unsigned int color = 0xFFFFFF;	// �e�̐F

	bool RemainingRicochetTimesCheck(void);	// ���e�񐔂��c���Ă��邩�`�F�b�N����
	bool IsScreenOutside(void);				// ��ʊO�ɏo�Ă��Ȃ����`�F�b�N����
	void ChangeAngle(void);					// �e�̐i�s������ύX������֐�
	
	Effect* effect;		//�G�t�F�N�g�I�u�W�F�N�g�̃|�C���^

	bool isAlive;			// �e�������Ă��邩
	float x, y;				// ���W x,y
	float moveX, moveY;		// �ړ� x,y
	float preX, preY;		// �ړ��O�̍��W
	int ricochetCount;		// �c�蒵�e��
	float movespeedX, movespeedY;			// �ړ����x
	float angle;				// �p�x
	float lastHitPointX, lastHitPointY;	// �Ō�Ƀu���b�N�ȂǂƏՓ˂������W

	void LoadSounds(void);	// ���f�[�^�ǂݍ���

	int s_Fire;	// ���ˉ�
	int s_Ricochet;	// ���e��
};

#endif // !_TITLEBULLET_H_