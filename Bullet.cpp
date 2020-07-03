#include "Bullet.h"
#include "GameMain.h"
#include <math.h>

// �R���X�g���N�^
Bullet::Bullet(void)
{
	isAlive = false;			// �e�������Ă��邩
	x = 0, y = 0;				// ���W x,y
	moveX = 0, moveY = 0;		// �ړ� x,y
	ricochetCount = 0;		// �c�蒵�e��
	movespeedx = BulletMoveSpeed, movespeedy = BulletMoveSpeed;			// �ړ����x
	angle = 0;				// �p�x
	hitFlg = false;
	collision = new Collision;	// �Փ˔��肵�Ă����I�u�W�F�N�g�𐶐����A�|�C���^��ۑ����Ă���
}

// �������p�֐�
void Bullet::BulletInit(bool alive, float rx, float ry, float ang, GameMain* main) {
	isAlive = alive;					// �e�̑��݃t���O
	x = rx;								// �e��X���W
	y = ry;								// �e��Y���W
	ricochetCount = BulletRicochetCount;// �e�̎c�蒵�e�񐔃��Z�b�g
	angle = ang;						// �e�̐i�s�����̊p�x
	ChangeAngle();						// angle�����ƂɁA�ړ��ʂ����Z�b�g����
	gameMain = main;					// GameMain�̃|�C���^
	shooterHitOK = false;				// �������u�ԂɌ����ɓ����蔻�肵�Ȃ��悤�ɂ���
}

// �e�����ۂɓ����Ƃ��̏���
void Bullet::BulletControll(void) {
	// �������e�񐔂�0�����Ȃ珈���𔲂���
	if (RemainingRicochetTimesCheck()) return;

	// �ړ��O�̍��W��ۑ�
	preX = x;
	preY = y;

	// �ړ�����
	x += moveX;
	y += moveY;

	if (IsScreenOutside()) return;	// ��ʊO�ɂ����ĂȂ����`�F�b�N
	if (IsHitPlayer()) return;		// �v���C���[�Ɠ����蔻��

	// �u���b�N�ƘA���œ����蔻�肵�Ȃ��悤�ɁA�O�̃t���[���Ńu���b�N�Ɠ������Ă�����
	// ����̃t���[���̓u���b�N�Ɠ����蔻�肵�Ȃ��Ŕ�����
	if (hitFlg) {
		hitFlg = false;
		return;
	}

	if (IsHitBlock()) return;		// �u���b�N�Ɠ����蔻��

}

// �`��
void Bullet::DrawBullet(void) {
	int dx = (int)x;
	int dy = (int)y;
	DrawCircle(dx, dy, Size, color);
}

// ���e�񐔂�0�����ɂȂ��Ă��Ȃ����`�F�b�N����
bool Bullet::RemainingRicochetTimesCheck(void) {
	if (ricochetCount < 0) {
		// ����0�����Ȃ琶���t���O��false�ɂ��ĉB��鑤�t�F�[�Y�Ɉڍs����
		isAlive = false;
		gameMain->gameManager->ToHidePhase();
		return true;
	}
	return false;
}

// �p�x�����Ƃɐi�s�����ύX
void Bullet::ChangeAngle(void) {
	float rad = (angle / 360) * DX_PI_F * 2;	// ���W�A���ɕϊ�����
	moveX = (movespeedx * cosf(rad));
	moveY = (movespeedy * sinf(rad));
}

// ��ʊO�ɏo�Ă��Ȃ����`�F�b�N����
bool Bullet::IsScreenOutside(void) {
	if (x >= GameMain::SCREEN_WIDTH || x <= 0) {
		--ricochetCount;				// ���e�񐔌��炷
		angle = (360 - angle) + 180;	// �����̏㉺�𔽓]������
		if (angle > 360) angle -= 360;	// 360�x�ɂ����܂�悤�ɂ���
		ChangeAngle();					// �p�x�����Ƃɐi�s�����ύX
		x = preX;
		y = preY;
		x += moveX;
		y += moveY;
		shooterHitOK = true;			// �����Ɠ����蔻��ł���悤�ɂ���
		RemainingRicochetTimesCheck();	// ���e��0�����Ȃ�B��鑤�t�F�[�Y�Ɉڍs
		return true;
	}

	if (y >= GameMain::SCREEN_HEIGHT || y <= 0) {
		--ricochetCount;				// ���e�񐔌��炷
		angle = (360 - angle);			// �����̍��E�𔽓]������
		ChangeAngle();					// �p�x�����Ƃɐi�s�����ύX
		x = preX;					
		y = preY;
		x += moveX;
		y += moveY;
		shooterHitOK = true;			// �����Ɠ����蔻��ł���悤�ɂ���
		RemainingRicochetTimesCheck();	// ���e��0�����Ȃ�B��鑤�t�F�[�Y�Ɉڍs
		return true;					
	}

	return false;
}

// �v���C���[�Ɠ����蔻��
bool Bullet::IsHitPlayer(void) {
	// �v���C���[��X�AY�A���a�T�C�Y
	int playerX, playerY, playerSize;

	for (int i = 0; i < 2; i++) {
		// �����ƓY�����̃v���C���[�̐F��������shooterHitOK��false�Ȃ珈����continue���Ď��̃v���C���[������
		if (gameMain->gameManager->GetNowShooter() == gameMain->player[i]->GetPlayerNum() && !shooterHitOK) continue;
		playerX = gameMain->player[i]->GetPlayerX();		// �v���C���[��X���W
		playerY = gameMain->player[i]->GetPlayerY();		// �v���C���[��Y���W
		playerSize = gameMain->player[i]->GetPlayerSize();	// �v���C���[�̒��a�T�C�Y

		// �v���C���[�Ɠ����蔻�肵�A�������Ă����烊�U���g��ʂɔ�Ԃ悤��GameManager�ɂ��肢���A�v���C���[�̐F��n��
		if (collision->IsHit((int)x, (int)y, Size, playerX, playerY, playerSize)) {
			gameMain->gameManager->SetPhaseStatus(GameManager::RESULT, gameMain->player[i]->GetPlayerNum());
			return true;
		}
	}

	return false;
}

// �u���b�N�Ɠ����蔻��
bool Bullet::IsHitBlock(void) {
	// �u���b�N�̒��SX�AY�A���a�T�C�Y
	int blockX, blockY, blockSize;

	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		if (!gameMain->block[i]->IsAlive()) continue;	// �u���b�N������ł�����continue�Ŏ��̃u���b�N�ɂ���

		blockX = gameMain->block[i]->GetBlockX();		// �u���b�N��X
		blockY = gameMain->block[i]->GetBlockY();		// �u���b�N��Y
		blockSize = gameMain->block[i]->GetBlockSize();	// �u���b�N�̒��a�T�C�Y

		// �u���b�N�Ɠ����蔻��
		if (collision->IsHit((int)x, (int)y, Size, blockX, blockY, blockSize)) {
			--ricochetCount;		// �������Ă�����A���e�񐔂����炷
			gameMain->block[i]->DecrementBlockHP();	// �u���b�N��HP�����炷
			
			if (collision->IsHitWicth((int)preX, blockX, blockSize)) {
				// �ړ��O���W�����̒��Ȃ�A�����̏㉺��ς���
				angle = (360 - angle);
			}
			else if (collision->IsHitHeight((int)preY, blockY, blockSize)) {
				// �����̒��Ȃ�A�����̍��E��ς���
				angle = (360 - angle) + 180;
				if (angle > 360) angle -= 360;
			}
			else {
				// �p�Ȃ�A�^�t�̌�����
				angle = angle + 180;
				if (angle > 360) angle -= 360;
			}

			ChangeAngle();	// �p�x�����Ƃɐi�s�����ύX
			x = preX;		// �ړ��O�̍��W�ɖ߂�
			y = preY;
			hitFlg = true;	// �A���Ńu���b�N�ɓ�����Ȃ��悤�Ƀt���O�𗧂Ă�

			shooterHitOK = true;	// �����̃v���C���[�ɓ�����悤�ɂ���
			return true;
		}
	}
	return false;
}

Bullet::~Bullet() {
	
}