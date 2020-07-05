#include "Player.h"
#include "GameManager.h"
#include "GameMain.h"
#include <math.h>

// �R���X�g���N�^�BRED��BLUE���A�F�A�������ۂ��AGameMain�I�u�W�F�N�g�̃|�C���^�������Ŏ󂯎��B
Player::Player(int num, unsigned int color, bool shooter, GameMain* main) {
	x = PlayerStartPositionX[num];		// X���S���W������
	y = PlayerStartPositionY[num];		// Y���S���W������
	preX = x;							// �ړ��OX���W������
	preY = y;							// �ړ��OY���W������
	size = PlayerSize;					// ���a�T�C�Y������
	moveSpeed = PlayerMoveSpeed;		// �ړ����x������
	
	if (num == GameManager::RED) {
		// �󂯎�����ԍ���RED��������A�p�x�̏����l���E�ɂȂ�悤�ɂ���
		angle = 0;
	} else if (num == GameManager::BLUE) {
		// BLUE��������A�����l�͍�
		angle = 180;
	}
	else {
		angle = 0;
	}
	targetx = cosf(angle * DX_PI_F / 180) * 300 + x;	// �_���Ă���X�ʒu��������
	targety = sinf(angle * DX_PI_F / 180) * 300 + y;	// �_���Ă���Y�ʒu��������
	targetx2 = 0, targety2 = 0, angle2 = 0;
	
	this->color = color;	// �F������
	isShooter = shooter;	// �ŏ��������ۂ��̈���������
	this->num = num;		// RED��BLUE��������B

	gameMain = main;		// GameMain�I�u�W�F�N�g�̃|�C���^������
	inputManager = main->inputManager;	// ���͊Ǘ��I�u�W�F�N�g�̃|�C���^������B

	collision = new Collision;	// �Փ˔���Ǘ��I�u�W�F�N�g�𐶐�����
}

// �������̑��쏈��
void Player::ShooterPlayerControll(void) {
	// �ړ��O�̍��W���L�����Ă���
	preX = x;
	preY = y;
	
	// �p�x�ύX
	float rad = atan2(inputManager->In_Stick_LY(), inputManager->In_Stick_LX());
	angle = -rad * 180.0f / DX_PI_F;
	if (angle < 0) angle += 360;
	

	// �p�x��0�`360�͈̔͂ɂȂ�悤�ɂ��Ă���B
	if (360 < angle) {
		angle = 0;
	}
	else if (angle < 0) {
		angle = 360;
	}

	// Y���W����ʊO�ɍs���Ă�����߂�
	if (GameMain::SCREEN_HEIGHT < y) {
		y = GameMain::SCREEN_HEIGHT;
	}
	else if (y < 0) {
		y = 0;
	}

	// X���W����߂�ꂽ�ʒu�𒴂��Ă�����߂�
	// ���S���d�l���������邩������Ȃ��̂ŁARED��BLUE�ňꉞ�����Ă���B
	if (num == GameManager::PLAYERCOLOR::RED) {
		if (GameMain::SCREEN_WIDTH < x) {
			x = GameMain::SCREEN_WIDTH;
		}
		else if (x < 0) {
			x = 0;
		}
	}
	else if(num == GameManager::PLAYERCOLOR::BLUE){
		if (GameMain::SCREEN_WIDTH < x) {
			x = GameMain::SCREEN_WIDTH;
		}
		else if (x < 0) {
			x = 0;
		}
	}

	// �u���b�N�����Ɠ����蔻�肷��B
	//BlockHitCheck();	

	targetx = cosf(angle * DX_PI_F / 180) * GameMain::SCREEN_WIDTH + x;	// �_���Ă��������X���W
	targety = sinf(angle * DX_PI_F / 180) * GameMain::SCREEN_WIDTH + y;	// �_���Ă��������Y���W

	// �u���b�N��X�AY�A���a�T�C�Y�̃��[�J���ϐ�
	int blockX, blockY, blockSize;
	
	Collision::Vector2 cross[GameMain::BLOCK_MAX];
	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		cross[i].x = -10000;		// �Ԉ���Ă��v���C���[�ƈ�ԋ߂��_�ƂȂ�Ȃ��悤�ɑ傫���l�ɂ��Ă���
		cross[i].y = -10000;
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
		cross[i] = collision->IsHitTargetAndBlock((float)x, (float)y, targetx, targety, (float)blockX, (float)blockY, (float)blockSize);
	}

	Collision::Vector2 crossPosition = { -10000, -10000, false };	// ��r�p�B
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
			crossPosition.flg = true;
			num = i;
		}
	}

	// �Ȃɂ�����L���Ȓl�������Ă�����A�^�[�Q�b�g�ʒu�����̍��W�ɂ���
	if (crossPosition.flg) {
		targetx = crossPosition.x;	// �_���Ă��������X���W
		targety = crossPosition.y;	// �_���Ă��������Y���W
		// �f�o�b�O�p�\��
		DrawFormatStringToHandle(200, 650, 0xffffff, gameMain->fontData->f_FontData[0], "�Փ˂�����WX%.2f", targetx);
		DrawFormatStringToHandle(200, 670, 0xffffff, gameMain->fontData->f_FontData[0], "�Փ˂�����WY%.2f", targety);
		DrawFormatStringToHandle(200, 690, 0xffffff, gameMain->fontData->f_FontData[0], "�Փ˓_�܂ł̋���%.2f", hypot(targetx - x,targety - y));
		CalcHitAfterAngle(num);
	}

	//TargetPointWindowHitCheck();

	// ���˃{�^���������ƁA�e�I�u�W�F�N�g�̏������֐��ɒl�����āA�t�F�[�Y��i�߂�B
	if (inputManager->In_Button()[InputManager::A] == 1) {
		float rx = cosf(angle * DX_PI_F / 180) + x;		// X�i�s����
		float ry = sinf(angle * DX_PI_F / 180) + y;		// Y�i�s����

		// �e�̏������B�����t���O��true�AX�i�s�����AY�i�s�����A�p�x�AGameMain�I�u�W�F�N�g�̃|�C���^��n��
		gameMain->bullet->BulletInit(true, rx, ry, (float)angle, gameMain);
		gameMain->gameManager->SetPhaseStatus(GameManager::RECOCHETWAIT);	// �t�F�[�Y��i�߂�
	}
}

// �B��鑤���̑��쏈��
void Player::HidingPlayerControll(void) {
	// �ړ��O�̍��W���L�����Ă���
	preX = x;
	preY = y;

	// �ړ�����
	if (inputManager->In_Stick_LY() >= 0.45f) {
		// ���X�e�B�b�N����ɌX�����Ă������Ɉړ�����
		y -= moveSpeed;
	}
	if (inputManager->In_Stick_LY() <= -0.45f) {
		// ���X�e�B�b�N�����ɌX�����Ă����牺�Ɉړ�����
		y += moveSpeed;
	}
	if (inputManager->In_Stick_LX() <= -0.45f) {
		// ���X�e�B�b�N�����ɌX�����Ă����獶�Ɉړ�����
		x -= moveSpeed;
	}
	if (inputManager->In_Stick_LX() >= 0.45f) {
		// ���X�e�B�b�N���E�ɌX�����Ă�����E�Ɉړ�����
		x += moveSpeed;
	}

	// Y���W����ʊO�ɍs���Ă�����߂�
	if (GameMain::SCREEN_HEIGHT < y) {
		y = GameMain::SCREEN_HEIGHT;
	}
	else if (y < 0) {
		y = 0;
	}

	// X���W����߂�ꂽ�ʒu�𒴂��Ă�����߂�
	// ���S���d�l���������邩������Ȃ��̂ŁARED��BLUE�ňꉞ�����Ă���B
	if (num == GameManager::PLAYERCOLOR::RED) {
		if (GameMain::SCREEN_WIDTH < x) {
			x = GameMain::SCREEN_WIDTH;
		}
		else if (x < 0) {
			x = 0;
		}
	}
	else if(num == GameManager::PLAYERCOLOR::BLUE){
		if (GameMain::SCREEN_WIDTH < x) {
			x = GameMain::SCREEN_WIDTH;
		}
		else if (x < 0) {
			x = 0;
		}
	}

	// �u���b�N�����Ɠ����蔻�肷��B
	BlockHitCheck();
}

// �`��p
void Player::DrawPlayer(void) {
	DrawCircle(x, y, size, color);
}

// �������ɑ_���Ă�������ɐ��������ĕ`�悷��
void Player::DrawTargetAngle(void) {
	DrawLine(x, y, (int)targetx, (int)targety, color, 3);
	DrawCircle((int)targetx, (int)targety, 4, 0xFFFFFF);
	DrawLine((int)targetx, (int)targety, (int)targetx2, (int)targety2, color, 3);
}

// �u���b�N�Ɠ����蔻��
void Player::BlockHitCheck(void) {
	// �u���b�N��X�AY�A���a�T�C�Y�̃��[�J���ϐ�
	int blockX, blockY, blockSize;

	// �u���b�N�����Ɠ����蔻��BGameMain�Œ�`����Ă���u���b�N�̍ő吔�ɒB����܂ŌJ��Ԃ��B
	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		// �����u���b�N�̐����t���O���܂�Ă����炻�̃u���b�N�Ƃ̊֌W�͏I���A���̃u���b�N�ɉ�ɍs���B
		if (!gameMain->block[i]->IsAlive()) continue;

		// ���[�J���ϐ��ɁA�u���b�N�̍��W�ƃT�C�Y�����Ă���
		blockX = gameMain->block[i]->GetBlockX();
		blockY = gameMain->block[i]->GetBlockY();
		blockSize = gameMain->block[i]->GetBlockSize();

		// �R���X�g���N�^�Ő��������Փ˔��������I�u�W�F�N�g�Ɉ�����n���āA���肵�Ă��炤�B
		if (collision->IsHit(x, y, size, blockX, blockY, blockSize)) {

			// �v���C���[�̈ړ��OX���W�����̒��ɂ�����AY���W�݂̂�߂��āAX���W�͕ω�������
			if (collision->IsHitWicth(preX, blockX, blockSize)) {
				y = preY;
			}
			// �v���C���[�̈ړ��OY���W�������̒��ɂ�����AX���W�݂̂�߂��āAY���W�͕ω�������
			else if (collision->IsHitHeight(preY, blockY, blockSize)) {
				x = preX;
			}
			// �u���b�N�̊p������������AXY�ǂ�����߂��B
			else {
				y = preY;
				x = preX;
			}
			// �����ď����𔲂���
			return;
		}
	}
}

void Player::TargetPointWindowHitCheck(void) {
	// Y���W����ʊO�ɍs���Ă�����߂�
	if (GameMain::SCREEN_HEIGHT < targety) {
		targety = GameMain::SCREEN_HEIGHT;
	}
	else if (targety < 0) {
		targety = 0;
	}

	// X���W����߂�ꂽ�ʒu�𒴂��Ă�����߂�
	// ���S���d�l���������邩������Ȃ��̂ŁARED��BLUE�ňꉞ�����Ă���B
	if (GameMain::SCREEN_WIDTH < targetx) {
		targetx = GameMain::SCREEN_WIDTH;
	}
	else if (targetx < 0) {
		targetx = 0;
	}
}

void Player::CalcHitAfterAngle(int num) {
	// �u���b�N��X�AY�A���a�T�C�Y�̃��[�J���ϐ�
	int blockX, blockY, blockSize;

	// ���[�J���ϐ��ɁA�u���b�N�̍��W�ƃT�C�Y�����Ă���
	blockX = gameMain->block[num]->GetBlockX();
	blockY = gameMain->block[num]->GetBlockY();
	blockSize = gameMain->block[num]->GetBlockSize();

	float prex = targetx - cosf(angle * DX_PI_F / 180) * 5;	// �_���Ă��������X���W
	float prey = targety - sinf(angle * DX_PI_F / 180) * 5;	// �_���Ă��������Y���W

	// �v���C���[�̈ړ��OX���W�����̒��ɂ�����AY���W�݂̂�߂��āAX���W�͕ω�������
	if (collision->IsHitWicth(prex, blockX, blockSize)) {
		// �ړ��O���W�����̒��Ȃ�A�����̏㉺��ς���
		angle2 = (360 - angle);
	}
	// �v���C���[�̈ړ��OY���W�������̒��ɂ�����AX���W�݂̂�߂��āAY���W�͕ω�������
	else if (collision->IsHitHeight(prey, blockY, blockSize)) {
		// �����̒��Ȃ�A�����̍��E��ς���
		angle2 = (360 - angle) + 180;
		if (angle2 > 360) angle2 -= 360;
	}
	// �u���b�N�̊p������������AXY�ǂ�����߂��B
	else {
		// �p�Ȃ�A�^�t�̌�����
		angle2 = angle + 180;
		if (angle2 > 360) angle2 -= 360;
	}

	float rad = (angle2 / 360) * DX_PI_F * 2;	// ���W�A���ɕϊ�����
	targetx2 = cosf(rad) * 300 + targetx;	// �_���Ă��������X���W
	targety2 = sinf(rad) * 300 + targety;	// �_���Ă��������Y���W
}

Player::~Player() {
	
}