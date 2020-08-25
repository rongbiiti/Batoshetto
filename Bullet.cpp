#include "Bullet.h"
#include "GameMain.h"
#include <math.h>

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
Bullet::Bullet(void)
{
	isAlive = false;			// �e�������Ă��邩
	x = 0, y = 0;				// ���W x,y
	preX = 0, preY = 0;
	moveX = 0, moveY = 0;		// �ړ� x,y
	ricochetCount = 0;		// �c�蒵�e��
	movespeedX = BulletMoveSpeed_X, movespeedY = BulletMoveSpeed_Y;			// �ړ����x
	angle = 0;				// �p�x
	hitFlg = false;
	collision = new Collision;	// �Փ˔��肵�Ă����I�u�W�F�N�g�𐶐����A�|�C���^��ۑ����Ă���
	effect = new Effect;	// �G�t�F�N�g�I�u�W�F�N�g�̐���
	gameMain = NULL;
	isPlayerHit = false;
	waitingTimeAfterPlayerHit = 0;
	HitPlayerNum = 0;
	LoadSounds();
	toHidePhaseTransitionWaitTime = 0;
	bnum = 0;
}

////////////////////////////////////////////////
// �������p�֐�
////////////////////////////////////////////////
void Bullet::BulletInit(bool alive, float rx, float ry, float ang, GameMain* main) {
	isAlive = alive;					// �e�̑��݃t���O
	x = rx;								// �e��X���W
	y = ry;								// �e��Y���W
	preX = x;							// �ړ��O���W��������
	preY = y;
	ricochetCount = BulletRicochetCount;// �e�̎c�蒵�e�񐔃��Z�b�g
	angle = ang;						// �e�̐i�s�����̊p�x
	ChangeAngle();						// angle�����ƂɁA�ړ��ʂ����Z�b�g����
	gameMain = main;					// GameMain�̃|�C���^
	shooterHitOK = false;				// �������u�ԂɌ����ɓ����蔻�肵�Ȃ��悤�ɂ���
	isPlayerHit = false;				// �v���C���[�ɓ����������̃t���O�܂�
	waitingTimeAfterPlayerHit = 0;		// �v���C���[�q�b�g��̗]�C���Z�b�g
	HitPlayerNum = 0;					// ���������v���C���[�̔ԍ����Z�b�g
	PlaySoundMem(s_Fire, DX_PLAYTYPE_BACK);	// ���ˉ��Đ�
}

////////////////////////////////////////////////
// �e�����ۂɓ����Ƃ��̏���
////////////////////////////////////////////////
void Bullet::BulletControll(void) {
	// �v���C���[�Ƀq�b�g���Ă��Ă��q�b�g��]�C���Ԃ��߂��Ă����烊�U���g��ʂֈڍs
	if (isPlayerHit && ResultTransitionWaiting()) {
		gameMain->gameManager->SetPhaseStatus(GameManager::RESULT, gameMain->player[HitPlayerNum]->GetPlayerNum());
		return;
	}

	// �B��鑤�t�F�[�Y�֍s���O�ɁA�G�t�F�N�g��������܂ő҂�
	if (0 < toHidePhaseTransitionWaitTime--) {
		if (0 == toHidePhaseTransitionWaitTime) {
			isAlive = false;
			gameMain->gameManager->ToHidePhase();
		}
		return;
	}

	// �������e�񐔂�0�����Ȃ珈���𔲂���
	if (RemainingRicochetTimesCheck()) return;

	// �ړ��O�̍��W��ۑ�
	preX = x;
	preY = y;

	// �ړ�����
	x += moveX;
	y += moveY;

	IsScreenOutside();	// ��ʊO�ɂ����ĂȂ����`�F�b�N
	if (IsHitPlayer() && !isPlayerHit) return;		// �v���C���[�Ɠ����蔻��

	 /*�u���b�N�ƘA���œ����蔻�肵�Ȃ��悤�ɁA�O�̃t���[���Ńu���b�N�Ɠ������Ă�����
	 ����̃t���[���̓u���b�N�Ɠ����蔻�肵�Ȃ��Ŕ�����*/
	if (hitFlg) {
		hitFlg = false;
		return;
	}

	// �������e�񐔂�0�����Ȃ珈���𔲂���
	if (RemainingRicochetTimesCheck()) return;

	if (gameMain->inputManager->GetPadInput()[gameMain->gameManager->GetNowShooter()].in_Button[PAD_RIGHT] != 0 && gameMain->GetNetBallteFlg())return;

	if (IsHitBlock()) return;		// �u���b�N�Ɠ����蔻��
}

////////////////////////////////////////////////
// �`��
////////////////////////////////////////////////
void Bullet::DrawBullet(void) {
	// ���e�񐔂�0�����Ȃ�`�悵�Ȃ�
	if (0 <= ricochetCount) {
		int dx = (int)x;
		int dy = (int)y;
		DrawLine(dx, dy, (int)preX, (int)preY, COLOR_VALUE_PLAYER[gameMain->gameManager->GetNowShooter()], 4);
		DrawCircle(dx, dy, Size, color);
	}
	effect->DrawRicochetEffect();		// �G�t�F�N�g�`��
	effect->DrawHitEffect();
}

////////////////////////////////////////////////
// ���e�񐔂�0�����ɂȂ��Ă��Ȃ����`�F�b�N����
////////////////////////////////////////////////
bool Bullet::RemainingRicochetTimesCheck(void) {
	if (ricochetCount < 0) {
		
		// ����0�����Ȃ琶���t���O��false�ɂ��ĉB��鑤�t�F�[�Y�Ɉڍs����
		if (!isPlayerHit) { 
			toHidePhaseTransitionWaitTime = 12;
		}
		return true;
	}
	return false;
}

////////////////////////////////////////////////
// �p�x�����Ƃɐi�s�����ύX
////////////////////////////////////////////////
void Bullet::ChangeAngle(void) {
	float rad = (angle / 360) * DX_PI_F * 2;	// ���W�A���ɕϊ�����
	moveX = (movespeedX * cosf(rad));
	moveY = (movespeedY * sinf(rad));
}

////////////////////////////////////////////////
// ��ʊO�ɏo�Ă��Ȃ����`�F�b�N����
////////////////////////////////////////////////
bool Bullet::IsScreenOutside(void) {
	if (x >= GameMain::SCREEN_WIDTH || x <= 0) {
		--ricochetCount;				// ���e�񐔌��炷
		float effAng = 0;
		if (x <= 0) {
			effAng = 0;
			x = 0;
		}
		else {
			effAng = (180 * 3.14) / 180;
			x = GameMain::SCREEN_WIDTH;
		}
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);	// ���e���Đ�
		angle = (360 - angle) + 180;	// �����̏㉺�𔽓]������
		if (angle > 360) angle -= 360;	// 360�x�ɂ����܂�悤�ɂ���
		ChangeAngle();					// �p�x�����Ƃɐi�s�����ύX
		y = preY;
		effect->InitRicochetCount(BulletRicochetCount - ricochetCount - 1, x, y, effAng);
		preX = x;
		preY = y;
		shooterHitOK = true;			// �����Ɠ����蔻��ł���悤�ɂ���
		
		return true;
	}

	if (y >= GameMain::SCREEN_HEIGHT || y <= 0) {
		--ricochetCount;				// ���e�񐔌��炷
		float effAng = 0;
		if (y <= 0) {
			effAng = (90 * 3.14) / 180;
			y = 0;
		}
		else {
			effAng = (270 * 3.14) / 180;
			y = GameMain::SCREEN_HEIGHT;
		}
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);	// ���e���Đ�
		angle = (360 - angle);			// �����̍��E�𔽓]������
		ChangeAngle();					// �p�x�����Ƃɐi�s�����ύX
		x = preX;					
		effect->InitRicochetCount(BulletRicochetCount - ricochetCount - 1, x, y, effAng);
		preX = x;
		preY = y;
		shooterHitOK = true;			// �����Ɠ����蔻��ł���悤�ɂ���
		return true;					
	}

	return false;
}

////////////////////////////////////////////////
// �v���C���[�Ɠ����蔻��
////////////////////////////////////////////////
bool Bullet::IsHitPlayer(void) {
	if (isPlayerHit) return false;
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
			HitPlayerNum = i;
			isPlayerHit = true;
			PlaySoundMem(s_PlayerHit[HitPlayerNum], DX_PLAYTYPE_BACK);	// �v���C���[���ѐ�
			PlaySoundMem(s_Blood, DX_PLAYTYPE_BACK);					// �q�b�g��
			ricochetCount = -1;
			effect->InitHitEffectCount(playerX,playerY);
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////
// �u���b�N�Ɠ����蔻��
////////////////////////////////////////////////
bool Bullet::IsHitBlock(void) {
	// �������e�񐔂�0�����Ȃ珈���𔲂���
	if (RemainingRicochetTimesCheck()) return true;

	// �u���b�N�̒��SX�AY�A���a�T�C�Y
	int blockX, blockY, blockSize;

	Collision::Vector2 cross[GameMain::BLOCK_MAX];
	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		cross[i].x = -10000;		// �Ԉ���Ă��v���C���[�ƈ�ԋ߂��_�ƂȂ�Ȃ��悤�ɑ傫���l�ɂ��Ă���
		cross[i].y = -10000;
		cross[i].num = 0;
		cross[i].flg = false;
	}

	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		// �����u���b�N�̐����t���O���܂�Ă����炻�̃u���b�N�Ƃ̊֌W�͏I���A���̃u���b�N�ɉ�ɍs���B
		if (!gameMain->block[i]->IsAlive()) continue;

		// ���[�J���ϐ��ɁA�u���b�N�̍��W�ƃT�C�Y�����Ă���
		blockX = gameMain->block[i]->GetBlockX();
		blockY = gameMain->block[i]->GetBlockY();
		blockSize = gameMain->block[i]->GetBlockSize();

		// �R���X�g���N�^�Ő��������Փ˔��������I�u�W�F�N�g�Ɉ�����n���āA���肵�Ă��炤�B		
		cross[i] = collision->IsHitTargetAndBlock(preX, preY, x, y, (float)blockX, (float)blockY, (float)blockSize);
	}

	Collision::Vector2 crossPosition = { -10000, -10000, 0, false };	// ��r�p�B
	float distance1 = -10000;
	float distance2 = -10000;
	int num = 0;

	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {		// �ŏ��̔z��̒l�͓���Ă���̂�2�Ԗڂ���J�n
		if (!cross[i].flg) continue;					// �L���Ȓl�������Ă��Ȃ�������X�L�b�v
		distance1 = hypot(crossPosition.x - x, crossPosition.y - y);	// �v���C���[�ƏՓ˓_�܂ł̋���
		distance2 = hypot(cross[i].x - x, cross[i].y - y);
		if (distance2 < distance1) {					// ��r�����ق����v���C���[�Ƌ߂�������n���p�̍\���̂̒l���A���̒l�ɍX�V
			crossPosition.x = cross[i].x;
			crossPosition.y = cross[i].y;
			crossPosition.num = cross[i].num;
			crossPosition.flg = true;
			num = i;
		}
	}

	// �Ȃɂ�����L���Ȓl�������Ă�����A�^�[�Q�b�g�ʒu�����̍��W�ɂ���
	if (crossPosition.flg) {
		
		shooterHitOK = true;	// �����̃v���C���[�ɓ�����悤�ɂ���
		gameMain->block[num]->DecrementBlockHP();	// �u���b�N��HP�����炷
		if (!gameMain->block[num]->IsAlive()) {
			PlaySoundMem(s_BlockBreak, DX_PLAYTYPE_BACK);
			return true;
		}
		x = crossPosition.x;	// �_���Ă��������X���W
		y = crossPosition.y;	// �_���Ă��������Y���W
		--ricochetCount;		// �������Ă�����A���e�񐔂����炷
		float effAng = 0;
		if (crossPosition.num == 0) {
			effAng = (270 * 3.14) / 180;
		}
		else if (crossPosition.num == 1) {
			effAng = 0;
		}
		else if (crossPosition.num == 2) {
			effAng = (90 * 3.14) / 180;
		}
		else {
			effAng = (180 * 3.14) / 180;
		}
		effect->InitRicochetCount(BulletRicochetCount - ricochetCount - 1, crossPosition.x, crossPosition.y, effAng);
		bnum = crossPosition.num;
		PlaySoundMem(s_Ricochet, DX_PLAYTYPE_BACK);
		//hitFlg = true;	// �A���Ńu���b�N�ɓ�����Ȃ��悤�Ƀt���O�𗧂Ă�

		lastHitPointX = x - cosf(angle * DX_PI_F / 180.0f) * movespeedX;	// �_���Ă��������X���W
		lastHitPointY = y - sinf(angle * DX_PI_F / 180.0f) * movespeedY;	// �_���Ă��������Y���W

		blockX = gameMain->block[num]->GetBlockX();
		blockY = gameMain->block[num]->GetBlockY();
		blockSize = gameMain->block[num]->GetBlockSize();

		if (collision->IsHitWicth((int)lastHitPointX, blockX, blockSize)) {
			// �ړ��O���W�����̒��Ȃ�A�����̏㉺��ς���
			angle = (360 - angle);
		}
		else if (collision->IsHitHeight((int)lastHitPointY, blockY, blockSize)) {
			// �����̒��Ȃ�A�����̍��E��ς���
			angle = (360 - angle) + 180;
			if (angle > 360) angle -= 360;
		}
		else {
			// �p�ɓ��������ꍇ�A���E�̃u���b�N�̕��̒��ɂ��Ȃ���������x�m���߂�
			if ((collision->IsHitWicth((int)lastHitPointX, gameMain->block[num - 1]->GetBlockX(), gameMain->block[num - 1]->GetBlockSize()) && gameMain->block[num - 1]->IsAlive()) ||
				(collision->IsHitWicth((int)lastHitPointX, gameMain->block[num + 1]->GetBlockX(), gameMain->block[num + 1]->GetBlockSize()) && gameMain->block[num + 1]->IsAlive())) {
				// �ړ��O���W�����̒��Ȃ�A�����̏㉺��ς���
				angle = (360 - angle);
			}
			// �㉺�̃u���b�N�̍����̒��ɂ��Ȃ���������x�m���߂�
			else if ((collision->IsHitHeight((int)lastHitPointY, gameMain->block[num - 3]->GetBlockY(), gameMain->block[num - 3]->GetBlockSize()) && gameMain->block[num - 3]->IsAlive()) ||
					 (collision->IsHitHeight((int)lastHitPointY, gameMain->block[num + 3]->GetBlockY(), gameMain->block[num + 3]->GetBlockSize()) && gameMain->block[num + 3]->IsAlive())) {
				// �����̒��Ȃ�A�����̍��E��ς���
				angle = (360 - angle) + 180;
				if (angle > 360) angle -= 360;
			}
			else {
				// �p�Ȃ�A�^�t�̌�����
				angle = angle + 180;
				if (angle > 360) angle -= 360;
			}			
		}

		ChangeAngle();	// �p�x�����Ƃɐi�s�����ύX
		x = lastHitPointX;
		y = lastHitPointY;
		preX = x;
		preY = y;
		RemainingRicochetTimesCheck();
		

		return true;
	}
	return false;
}

////////////////////////////////////////////////
// ���U���g��ʂɔ�ԑO�ɏ����҂�
////////////////////////////////////////////////
bool Bullet::ResultTransitionWaiting(void) {
	
	if (++waitingTimeAfterPlayerHit <= 480) {
		if (waitingTimeAfterPlayerHit == 210) {
			gameMain->PlayBattleBGM(TRUE);
			PlaySoundMem(s_MatchEnd, DX_PLAYTYPE_BACK);
		}
		return false;
	}
	return true;
}

////////////////////////////////////////////////
// ��������̕`��
////////////////////////////////////////////////
void Bullet::DrawSHINOBIEXECUTION() {
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2;

	if (210 <= waitingTimeAfterPlayerHit) {
		// �w�i������������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
		DrawBox(0, 0, GameMain::SCREEN_WIDTH, GameMain::SCREEN_HEIGHT, 0x000000, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �����`��
		fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[1], "��");
		DrawFormatStringToHandle(x - fontwidth / 2, 100, 0xdc143c, gameMain->fontData->f_FontData[2], "��");
		DrawFormatStringToHandle(x - fontwidth / 2, 230, 0xdc143c, gameMain->fontData->f_FontData[2], "��");
		DrawFormatStringToHandle(x - fontwidth / 2, 360, 0xdc143c, gameMain->fontData->f_FontData[2], "��");
		DrawFormatStringToHandle(x - fontwidth / 2, 490, 0xdc143c, gameMain->fontData->f_FontData[2], "��");
	}
	else if (waitingTimeAfterPlayerHit <= 480) {
		return;
	}

	return;
}

////////////////////////////////////////////////
// ���f�[�^�ǂݍ���
////////////////////////////////////////////////
void Bullet::LoadSounds(void) {
	if ((s_Fire = LoadSoundMem("sounds/Fire.mp3")) == -1) return;
	if ((s_Ricochet = LoadSoundMem("sounds/Ricochet.mp3")) == -1) return;
	if ((s_BlockBreak = LoadSoundMem("sounds/BlockBreak.mp3")) == -1) return;
	if ((s_PlayerHit[0] = LoadSoundMem("sounds/PlayerHit.mp3")) == -1) return;
	if ((s_PlayerHit[1] = LoadSoundMem("sounds/PlayerHit2.mp3")) == -1) return;
	if ((s_Blood = LoadSoundMem("sounds/Blood_Sibuki.mp3")) == -1) return;
	if ((s_MatchEnd = LoadSoundMem("sounds/MatchEnd.mp3")) == -1) return;
}

////////////////////////////////////////////////
// ���ʕύX
////////////////////////////////////////////////
void Bullet::ChangeVolume(float persent) {
	int volume = 255.0f * persent;

	ChangeVolumeSoundMem(volume, s_Fire);
	ChangeVolumeSoundMem(volume, s_Ricochet);
	ChangeVolumeSoundMem(volume, s_BlockBreak);
	ChangeVolumeSoundMem(volume, s_PlayerHit[0]);
	ChangeVolumeSoundMem(volume, s_PlayerHit[1]);
	ChangeVolumeSoundMem(volume, s_Blood);
	ChangeVolumeSoundMem(volume, s_MatchEnd);
}

////////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////////
Bullet::~Bullet() {
	delete collision;
	delete effect;
	s_Fire = DeleteSoundMem(s_Fire);
	s_Ricochet = DeleteSoundMem(s_Ricochet);
	s_BlockBreak = DeleteSoundMem(s_BlockBreak);
	s_PlayerHit[0] = DeleteSoundMem(s_PlayerHit[0]);
	s_PlayerHit[1] = DeleteSoundMem(s_PlayerHit[1]);
	s_Blood = DeleteSoundMem(s_Blood);
	s_MatchEnd = DeleteSoundMem(s_MatchEnd);
}