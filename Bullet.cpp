#include "Bullet.h"
#include <math.h>

Bullet::Bullet(void)
{
	isAlive = false;			// íeÇ™ê∂Ç´ÇƒÇ¢ÇÈÇ©
	x = 0, y = 0;				// ç¿ïW x,y
	moveX = 0, moveY = 0;		// à⁄ìÆ x,y
	ricochetCount = 0;		// écÇËíµíeâÒêî
	movespeedx = BulletMoveSpeed, movespeedy = BulletMoveSpeed;			// à⁄ìÆë¨ìx
	angle = 0;				// äpìx
	hitFlg = false;
	collision = new Collision;
}

void Bullet::BulletInit(bool alive, float rx, float ry, float ang, GameMain* main) {
	isAlive = alive;
	x = rx;
	y = ry;
	ricochetCount = BulletRicochetCount;
	angle = ang;
	ChangeAngle();
	gameMain = main;
	shooterHitOK = false;
}

void Bullet::BulletControll(void) {
	if (RemainingRicochetTimesCheck()) return;

	preX = x;
	preY = y;

	x += moveX;
	y += moveY;

	if (IsScreenOutside()) return;
	if (IsHitPlayer()) return;

	if (hitFlg) {
		hitFlg = false;
		return;
	}

	if (IsHitBlock()) return;

}

void Bullet::DrawBullet(void) {
	int dx = (int)x;
	int dy = (int)y;
	DrawCircle(dx, dy, Size, color);
}

bool Bullet::RemainingRicochetTimesCheck(void) {
	if (ricochetCount < 0) {
		isAlive = false;
		gameMain->gameManager->ToHidePhase();
		return true;
	}
	return false;
}

void Bullet::ChangeAngle(void) {
	float rad = (angle / 360) * DX_PI_F * 2;
	moveX = (movespeedx * cosf(rad));
	moveY = (movespeedy * sinf(rad));
}

bool Bullet::IsScreenOutside(void) {
	if (x >= GameMain::SCREEN_WIDTH - Size / 2 || x <= Size / 2) {
		--ricochetCount;
		angle = (360 - angle) + 180;
		if (angle > 360) angle -= 360;
		ChangeAngle();
		x = preX;
		y = preY;
		x += moveX;
		y += moveY;
		shooterHitOK = true;
		RemainingRicochetTimesCheck();
		return true;
	}

	if (y >= GameMain::SCREEN_HEIGHT - Size / 2 || y <= Size / 2) {
		--ricochetCount;
		angle = (360 - angle);
		ChangeAngle();
		x = preX;
		y = preY;
		x += moveX;
		y += moveY;
		shooterHitOK = true;
		RemainingRicochetTimesCheck();
		return true;
	}

	return false;
}

bool Bullet::IsHitPlayer(void) {
	int playerX, playerY, playerSize;

	for (int i = 0; i < 2; i++) {
		if (gameMain->gameManager->GetNowShooter() == gameMain->player[i]->GetPlayerNum() && !shooterHitOK) continue;
		playerX = gameMain->player[i]->GetPlayerX();
		playerY = gameMain->player[i]->GetPlayerY();
		playerSize = gameMain->player[i]->GetPlayerSize();

		if (collision->IsHit((int)x, (int)y, Size, playerX, playerY, playerSize)) {
			gameMain->gameManager->SetPhaseStatus(GameManager::RESULT, gameMain->player[i]->GetPlayerNum());
			return true;
		}
	}

	return false;
}

bool Bullet::IsHitBlock(void) {
	int blockX, blockY, blockSize;

	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		if (!gameMain->block[i]->IsAlive()) continue;

		blockX = gameMain->block[i]->GetBlockX();
		blockY = gameMain->block[i]->GetBlockY();
		blockSize = gameMain->block[i]->GetBlockSize();
		if (collision->IsHit((int)x, (int)y, Size, blockX, blockY, blockSize)) {
			--ricochetCount;
			gameMain->block[i]->DecrementBlockHP();
			
			if (collision->IsHitWicth((int)preX, blockX, blockSize)) {
				angle = (360 - angle);
			}
			else if (collision->IsHitHeight((int)preY, blockY, blockSize)) {
				angle = (360 - angle) + 180;
				if (angle > 360) angle -= 360;
			}
			else {
				angle = angle + 180;
				if (angle > 360) angle -= 360;
			}

			ChangeAngle();
			x = preX;
			y = preY;
			hitFlg = true;
			shooterHitOK = true;
			return true;
		}
	}
	return false;
}