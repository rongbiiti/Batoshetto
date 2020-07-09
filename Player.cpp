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
	targetx = cosf(angle * DX_PI_F / 180) * 10000 + x;	// �_���Ă���X�ʒu��������
	targety = sinf(angle * DX_PI_F / 180) * 10000 + y;	// �_���Ă���Y�ʒu��������
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
	int shooter = gameMain->gameManager->GetNowShooter();
	// �p�x�ύX
	// �R���g���[���[�̃X�e�B�b�N
	if (abs(inputManager->GetPadInput()[shooter].in_Stick_LX) + abs(inputManager->GetPadInput()[shooter].in_Stick_LY) >= 0.97f) {
		// X����Y���̓|�����ʂ̍��v���K��̒l�ȉ��Ȃ�p�x���Œ肳����
		float rad = atan2(inputManager->GetPadInput()[shooter].in_Stick_LY, inputManager->GetPadInput()[shooter].in_Stick_LX);
		angle = -rad * 180.0f / DX_PI_F;
		if (angle < 0) angle += 360;
	}
	

	// �L�[�{�[�h�ł̊p�x�ύX
	if (inputManager->In_Key()[KEY_INPUT_UP] != 0) {
		angle += 2;
	}
	if (inputManager->In_Key()[KEY_INPUT_DOWN] != 0) {
		angle -= 2;
	}

	AngleCorrection(angle);

	targetx = cosf(angle * DX_PI_F / 180) * 10000 + x;	// �_���Ă��������X���W
	targety = sinf(angle * DX_PI_F / 180) * 10000 + y;	// �_���Ă��������Y���W
	
	int blocknum = 0;
	// �Ȃɂ�����L���Ȓl�������Ă�����A�^�[�Q�b�g�ʒu�����̍��W�ɂ���
	if (TrajectoryPrecalculation_ToBlock(&blocknum)) {
		CalcHitAfterAngle_ToBlock(blocknum);
	}
	else {	// �L���Ȓl���Ȃ������Ƃ��A�܂�u���b�N�Ɠ����炸��ʒ[��_���Ă���ꍇ�̏����B
		blocknum = TrajectoryPrecalculation_ToWindow();
		CalcHitAfterAngle_ToWindow(blocknum);
	}

	//TargetPointWindowHitCheck();

	// ���˃{�^���������ƁA�e�I�u�W�F�N�g�̏������֐��ɒl�����āA�t�F�[�Y��i�߂�B
	if (inputManager->GetPadInput()[gameMain->gameManager->GetNowShooter()].in_Button[InputManager::A] == 1 || inputManager->In_Key()[KEY_INPUT_F] == 1) {
		float rx = cosf(angle * DX_PI_F / 180) + x;		// X�i�s����
		float ry = sinf(angle * DX_PI_F / 180) + y;		// Y�i�s����

		// �e�̏������B�����t���O��true�AX�i�s�����AY�i�s�����A�p�x�AGameMain�I�u�W�F�N�g�̃|�C���^��n��
		gameMain->bullet->BulletInit(true, rx, ry, (float)angle, gameMain);
		gameMain->gameManager->SetPhaseStatus(GameManager::RECOCHETWAIT);	// �t�F�[�Y��i�߂�
	}
}

// �B��鑤���̑��쏈��
void Player::HidingPlayerControll(void) {
	int hider = gameMain->gameManager->GetNowHider();

	// �ړ��O�̍��W���L�����Ă���
	preX = x;
	preY = y;
	
	// �ړ�����
	if (inputManager->GetPadInput()[hider].in_Stick_LY >= 0.45f) {
		// ���X�e�B�b�N����ɌX�����Ă������Ɉړ�����
		y -= moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LY <= -0.45f) {
		// ���X�e�B�b�N�����ɌX�����Ă����牺�Ɉړ�����
		y += moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LX <= -0.45f) {
		// ���X�e�B�b�N�����ɌX�����Ă����獶�Ɉړ�����
		x -= moveSpeed;
	}
	if (inputManager->GetPadInput()[hider].in_Stick_LX >= 0.45f) {
		// ���X�e�B�b�N���E�ɌX�����Ă�����E�Ɉړ�����
		x += moveSpeed;
	}

	x = XCoordinateCorrection(x, num, size);
	y = YCoordinateCorrection(y, size);

	// �u���b�N�����Ɠ����蔻�肷��B
	BlockHitCheck();
}

// �`��p
void Player::DrawPlayer(void) {
	DrawCircle(x, y, size / 2, color);
}

// �������ɑ_���Ă�������ɐ��������ĕ`�悷��
void Player::DrawTargetAngle(void) {
	DrawLine(x, y, (int)targetx, (int)targety, color, 3);
	DrawLine((int)targetx, (int)targety, (int)targetx2, (int)targety2, color, 3);
	DrawCircle((int)targetx, (int)targety, 4, 0xFFFFFF);
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

// �e���i�ޕ������E�B���h�E���яo���ĂȂ����`�F�b�N����
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

// �u���b�N�̂ǂ̕ӂƏՓ˂��Ă��邩���f���Ċp�x��ύX����
void Player::CalcHitAfterAngle_ToBlock(int blocknum) {
	// �u���b�N��X�AY�A���a�T�C�Y�̃��[�J���ϐ�
	int blockX, blockY, blockSize;

	// ���[�J���ϐ��ɁA�u���b�N�̍��W�ƃT�C�Y�����Ă���
	blockX = gameMain->block[blocknum]->GetBlockX();
	blockY = gameMain->block[blocknum]->GetBlockY();
	blockSize = gameMain->block[blocknum]->GetBlockSize();

	float prex = targetx - cosf(angle * DX_PI_F / 180) * 5;	// �_���Ă��������X���W
	float prey = targety - sinf(angle * DX_PI_F / 180) * 5;	// �_���Ă��������Y���W

	// �^�[�Q�b�g�̈ړ��OX���W�����̒��ɂ�����AY���W�݂̂�߂��āAX���W�͕ω�������
	if (collision->IsHitWicth(prex, blockX, blockSize)) {
		// �ړ��O���W�����̒��Ȃ�A�����̏㉺��ς���
		angle2 = (360 - angle);
	}
	// �^�[�Q�b�g�̈ړ��OY���W�������̒��ɂ�����AX���W�݂̂�߂��āAY���W�͕ω�������
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

// �e���i�ޕ������E�B���h�E���яo���ĂȂ����`�F�b�N����
bool Player::TrajectoryPrecalculation_ToBlock(int* blocknum) {
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

	bool resurt = false;
	float distance1 = -10000;
	float distance2 = -10000;

	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {		// �ŏ��̔z��̒l�͓���Ă���̂�2�Ԗڂ���J�n
		if (!cross[i].flg) continue;					// �L���Ȓl�������Ă��Ȃ�������X�L�b�v
		distance1 = hypot(targetx - x, targety - y);	// �v���C���[�ƏՓ˓_�܂ł̋���
		distance2 = hypot(cross[i].x - x, cross[i].y - y);
		if (distance2 < distance1) {					// ��r�����ق����v���C���[�Ƌ߂�������n���p�̍\���̂̒l���A���̒l�ɍX�V
			targetx = cross[i].x;
			targety = cross[i].y;
			resurt = true;
			*blocknum = i;
		}
	}
	return resurt;
}

// �E�B���h�E�̂ǂ̒[�ƏՓ˂��Ă��邩���f���Ċp�x��ύX����
void Player::CalcHitAfterAngle_ToWindow(int num) {
	float prex = targetx - cosf(angle * DX_PI_F / 180) * 5;	// �_���Ă��������X���W
	float prey = targety - sinf(angle * DX_PI_F / 180) * 5;	// �_���Ă��������Y���W
	// �^�[�Q�b�g�̈ړ��OX���W�����̒��ɂ�����AY���W�݂̂�߂��āAX���W�͕ω�������
	if (num % 2 == 0) {
		// �ړ��O���W�����̒��Ȃ�A�����̏㉺��ς���
		angle2 = (360 - angle);
	}
	// �^�[�Q�b�g�̈ړ��OY���W�������̒��ɂ�����AX���W�݂̂�߂��āAY���W�͕ω�������
	else {
		// �����̒��Ȃ�A�����̍��E��ς���
		angle2 = (360 - angle) + 180;
		if (angle2 > 360) angle2 -= 360;
	}
	float rad = (angle2 / 360) * DX_PI_F * 2;	// ���W�A���ɕϊ�����
	targetx2 = cosf(rad) * 300 + targetx;	// �_���Ă��������X���W
	targety2 = sinf(rad) * 300 + targety;	// �_���Ă��������Y���W
}

// �E�B���h�E�ɑ΂���e���̎��O�v�Z�B
int Player::TrajectoryPrecalculation_ToWindow(void) {
	Collision::Vector2 cross[GameMain::BLOCK_MAX];
	float distance1 = -10000;
	float distance2 = -10000;

	for (int i = 0; i < 4; i++) {
		cross[i].x = -10000;		// �Ԉ���Ă��v���C���[�ƈ�ԋ߂��_�ƂȂ�Ȃ��悤�ɑ傫���l�ɂ��Ă���
		cross[i].y = -10000;
		cross[i].flg = false;
	}

	float top = 0, bottom = GameMain::SCREEN_HEIGHT, left = 0, right = GameMain::SCREEN_WIDTH;

	cross[0] = collision->GetHitLineAndLine((float)x, (float)y, targetx, targety, left, top, right, top);
	cross[1] = collision->GetHitLineAndLine((float)x, (float)y, targetx, targety, right, top, right, bottom);
	cross[2] = collision->GetHitLineAndLine((float)x, (float)y, targetx, targety, right, bottom, left, bottom);
	cross[3] = collision->GetHitLineAndLine((float)x, (float)y, targetx, targety, left, bottom, left, top);

	// �ǂ̓_����ԃv���C���[�ɋ߂������r����
	targetx = cross[0].x;	// �ŏ��̔z��̒l�������Ă���
	targety = cross[0].y;
	int result = 0;

	for (int i = 1; i < 4; i++) {		// �ŏ��̔z��̒l�͓���Ă���̂�2�Ԗڂ���J�n
		if (!cross[i].flg) continue;					// �L���Ȓl�������Ă��Ȃ�������X�L�b�v
		distance1 = hypot(targetx - x, targety - y);	// �v���C���[�ƏՓ˓_�܂ł̋���
		distance2 = hypot(cross[i].x - x, cross[i].y - y);
		if (distance2 < distance1) {					// ��r�����ق����v���C���[�Ƌ߂�������n���p�̍\���̂̒l���A���̒l�ɍX�V
			targetx = cross[i].x;
			targety = cross[i].y;
			result = i;
		}
	}
	return result;	
}

// �p�x��0�`360�x�Ɏ��܂�悤�ɒ���
float Player::AngleCorrection(float ang) {
	// �p�x��0�`360�͈̔͂ɂȂ�悤�ɂ��Ă���B
	if (360 < ang) {
		return 0;
	}
	else if (ang < 0) {
		return 360;
	}
	return ang;
}

// X���W���w��͈̔͂𒴂��Ă��Ȃ����A�v���C���[�̔ԍ������ƂɏC��
int Player::XCoordinateCorrection(int posx, int pnum, int size) {
	// X���W����߂�ꂽ�ʒu�𒴂��Ă�����߂�
	// ���S���d�l���������邩������Ȃ��̂ŁARED��BLUE�ňꉞ�����Ă���B
	int halfsize = size / 2;
	switch (pnum)
	{
	case GameManager::PLAYERCOLOR::RED:
		if (GameMain::SCREEN_WIDTH < posx + halfsize) {
			return GameMain::SCREEN_WIDTH - halfsize;
		}
		else if (posx - halfsize < 0) {
			return halfsize;
		}
		return posx;
		break;
	case GameManager::PLAYERCOLOR::BLUE:
		if (GameMain::SCREEN_WIDTH < posx + halfsize) {
			return GameMain::SCREEN_WIDTH - halfsize;
		}
		else if (posx - halfsize < 0) {
			return halfsize;
		}
		return posx;
		break;
	default:
		return posx;
		break;
	}
}

// Y���W���w��͈̔͂𒴂��Ȃ��悤�ɏC��
int Player::YCoordinateCorrection(int posy, int size) {
	// Y���W����ʊO�ɍs���Ă�����߂�
	int halfsize = size / 2;
	if (GameMain::SCREEN_HEIGHT < posy + halfsize) {
		return GameMain::SCREEN_HEIGHT - halfsize;
	}
	else if (posy - halfsize < 0) {
		return halfsize;
	}
	return posy;
}

Player::~Player() {
	
}