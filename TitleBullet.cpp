#include "TitleBullet.h"
#include <math.h>

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
TitleBullet::TitleBullet(FontData* font) {
	fontData = font;

	isAlive = false;			// �e�������Ă��邩
	x = 0, y = 0;				// ���W x,y
	preX = 0, preY = 0;
	moveX = 0, moveY = 0;		// �ړ� x,y
	ricochetCount = 0;		// �c�蒵�e��
	movespeedX = BulletMoveSpeed_X, movespeedY = BulletMoveSpeed_Y;			// �ړ����x
	angle = 0;				// �p�x
	effect = new Effect;	// �G�t�F�N�g�I�u�W�F�N�g�̐���
	collision = new Collision;	// �Փ˔��肵�Ă����I�u�W�F�N�g�𐶐����A�|�C���^��ۑ����Ă���

	LoadSounds();		// ���f�[�^�ǂݍ���
}

////////////////////////////////////////////////
// �e�̏�����
////////////////////////////////////////////////
void TitleBullet::BulletInit(bool alive, int randx, int randy) {
	isAlive = alive;					// �e�̑��݃t���O

	if (randy == 0) {
		y = 50;
		angle = 90;	// ��������
	}
	else {
		y = GameMain::SCREEN_HEIGHT - 50;
		angle = 270;		// �������
	}

	if (randx == 0) {
		x = 50;
		if (angle == 270) {
			angle = 315;	// ���ɂ������Ă��獶�������悤�ɂ���
		}
		else if (angle == 90) {
			angle = 45;	// ��ɂ������Ă��獶������悤�ɂ���
		}
	}
	else {
		x = GameMain::SCREEN_WIDTH - 50;
		if (angle == 270) {
			angle = 225;	// ���ɂ������Ă���E�������悤�ɂ���
		}
		else if (angle == 90) {
			angle = 135;		// ��ɂ������Ă���E������悤�ɂ���
		}
	}

	preX = x;							// �ړ��O���W��������
	preY = y;
	ricochetCount = BulletRicochetCount;// �e�̎c�蒵�e�񐔃��Z�b�g
	ChangeAngle();						// angle�����ƂɁA�ړ��ʂ����Z�b�g����
	PlaySoundMem(s_Fire, DX_PLAYTYPE_BACK);	// ���ˎ��̉���炷
}

////////////////////////////////////////////////
// �e�̈ړ�����
////////////////////////////////////////////////
void TitleBullet::TitleBulletControll() {
	// �������e�񐔂�0�����Ȃ珈���𔲂���
	if (RemainingRicochetTimesCheck()) return;

	// �ړ��O�̍��W��ۑ�
	preX = x;
	preY = y;

	// �ړ�����
	x += moveX;
	y += moveY;

	IsScreenOutside();	// ��ʊO�ɂ����ĂȂ����`�F�b�N
}

////////////////////////////////////////////////
// �e�̕`�揈��
////////////////////////////////////////////////
void TitleBullet::DrawTitleBullet() {
	if (0 <= ricochetCount) {
		int dx = (int)x;
		int dy = (int)y;
		DrawLine(dx, dy, (int)preX, (int)preY, COLOR_VALUE_PLAYER[0], 4);
		DrawCircle(dx, dy, Size, color);
	}

	effect->DrawRicochetEffect();		// �G�t�F�N�g�`��
	effect->DrawHitEffect();
}

////////////////////////////////////////////////
// �e�̎c�蒵�e�񐔃`�F�b�N
////////////////////////////////////////////////
bool TitleBullet::RemainingRicochetTimesCheck() {
	// ���e��0�����Ȃ琶���t���O��܂�
	if (ricochetCount < 0) {
		isAlive = false;
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// �e����ʒ[�ɍs���ĂȂ����`�F�b�N
////////////////////////////////////////////////
bool TitleBullet::IsScreenOutside() {
	if (x >= GameMain::SCREEN_WIDTH || x <= 0) {
		--ricochetCount;				// ���e�񐔌��炷
		float effAng = 0;
		if (x <= 0) {
			effAng = 0;
		}
		else {
			effAng = (180 * 3.14) / 180;
		}
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);		// ���e���Đ�
		angle = (360 - angle) + 180;	// �����̏㉺�𔽓]������
		if (angle > 360) angle -= 360;	// 360�x�ɂ����܂�悤�ɂ���
		ChangeAngle();					// �p�x�����Ƃɐi�s�����ύX
		x = preX;
		y = preY;
		preX = x;
		preY = y;
		effect->InitRicochetCount(BulletRicochetCount - ricochetCount - 1, preX, preY, effAng);	// �G�t�F�N�g�`��
		return true;
	}

	if (y >= GameMain::SCREEN_HEIGHT || y <= 0) {
		--ricochetCount;				// ���e�񐔌��炷
		float effAng = 0;
		if (y <= 0) {
			effAng = (90 * 3.14) / 180;
		}
		else {
			effAng = (270 * 3.14) / 180;
		}
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);
		angle = (360 - angle);			// �����̍��E�𔽓]������
		ChangeAngle();					// �p�x�����Ƃɐi�s�����ύX
		x = preX;
		y = preY;
		preX = x;
		preY = y;
		effect->InitRicochetCount(BulletRicochetCount - ricochetCount - 1, preX, preY, effAng);	// �G�t�F�N�g�`��
		return true;
	}

	return false;
}

////////////////////////////////////////////////
// �e�̊p�x�ύX
////////////////////////////////////////////////
void TitleBullet::ChangeAngle() {
	float rad = (angle / 360) * DX_PI_F * 2;	// ���W�A���ɕϊ�����
	moveX = (movespeedX * cosf(rad));
	moveY = (movespeedY * sinf(rad));
}

////////////////////////////////////////////////
// �e�̌��ʉ����ʕύX
////////////////////////////////////////////////
void TitleBullet::ChangeVolume(float persent) {
	int volume = 153.0f * persent;

	ChangeVolumeSoundMem(volume, s_Fire);
	ChangeVolumeSoundMem(volume, s_Ricochet);
}

////////////////////////////////////////////////
// �e�̌��ʉ��ǂݍ���
////////////////////////////////////////////////
void TitleBullet::LoadSounds() {
	if ((s_Fire = LoadSoundMem("sounds/SE/Fire.mp3")) == -1) return;
	if ((s_Ricochet = LoadSoundMem("sounds/SE/Ricochet.mp3")) == -1) return;
}

// �f�X�g���N�^
TitleBullet::~TitleBullet() {
	delete collision;
	delete effect;
	collision = NULL;
	effect = NULL;

	s_Fire = DeleteSoundMem(s_Fire);
	s_Ricochet = DeleteSoundMem(s_Ricochet);
}