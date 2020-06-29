#include "Player.h"
#include <math.h>

Player::Player(int num, unsigned int color, bool shooter, GameMain* main) {
	x = PlayerStartPositionX[num];
	y = PlayerStartPositionY[num];
	size = PlayerSize;
	moveSpeed = PlayerMoveSpeed;
	targetx = cosf(angle * DX_PI_F / 180) * 300 + x;
	targety = sinf(angle * DX_PI_F / 180) * 300 + y;
	if (num == GameManager::RED) {
		angle = 0;
	} else if (num == GameManager::BLUE) {
		angle = 180;
	}
	
	this->color = color;
	isShooter = shooter;
	this->num = num;

	gameMain = main;
	inputManager = main->inputManager;

	collision = new Collision;
}

void Player::ShooterPlayerControll(void) {
	preX = x;
	preY = y;
	if (inputManager->k_Buf[KEY_INPUT_W] != 0) {
		y -= moveSpeed;
	}
	if (inputManager->k_Buf[KEY_INPUT_S] != 0) {
		y += moveSpeed;
	}
	/*if (inputManager->k_Buf[KEY_INPUT_A] != 0) {
		x -= moveSpeed;
	}
	if (inputManager->k_Buf[KEY_INPUT_D] != 0) {
		x += moveSpeed;
	}*/
	if (inputManager->k_Buf[KEY_INPUT_UP] != 0) {
		angle -= 2;
	}
	if (inputManager->k_Buf[KEY_INPUT_DOWN] != 0) {
		angle += 2;
	}

	if (GameMain::SCREEN_HEIGHT < y) {
		y = GameMain::SCREEN_HEIGHT;
	}
	else if (y < 0) {
		y = 0;
	}

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

	BlockHitCheck();

	if (360 < angle) {
		angle = 0;
	}
	else if (angle < 0) {
		angle = 360;
	}

	targetx = cosf(angle * DX_PI_F / 180) * 300 + x;
	targety = sinf(angle * DX_PI_F / 180) * 300 + y;

	if (inputManager->k_Buf[KEY_INPUT_F] == 1) {
		float rx = cosf(angle * DX_PI_F / 180) + x;
		float ry = sinf(angle * DX_PI_F / 180) + y;
		gameMain->bullet->BulletInit(true, rx, ry, (float)angle, gameMain);
		gameMain->gameManager->SetPhaseStatus(GameManager::RECOCHETWAIT);
	}
}

void Player::HidingPlayerControll(void) {
	preX = x;
	preY = y;

	if (inputManager->k_Buf[KEY_INPUT_W] != 0) {
		y -= moveSpeed;
	}
	if (inputManager->k_Buf[KEY_INPUT_S] != 0) {
		y += moveSpeed;
	}
	if (inputManager->k_Buf[KEY_INPUT_A] != 0) {
		x -= moveSpeed;
	}
	if (inputManager->k_Buf[KEY_INPUT_D] != 0) {
		x += moveSpeed;
	}

	if (GameMain::SCREEN_HEIGHT < y) {
		y = GameMain::SCREEN_HEIGHT;
	}
	else if (y < 0) {
		y = 0;
	}

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

	BlockHitCheck();
}

void Player::DrawPlayer(void) {
	DrawCircle(x, y, size, color);
}

void Player::DrawTargetAngle(void) {
	DrawLine(x, y, (int)targetx, (int)targety, color, 3);
}

void Player::BlockHitCheck(void) {
	int blockX, blockY, blockSize;

	for (int i = 0; i < GameMain::BLOCK_MAX; i++) {
		if (!gameMain->block[i]->IsAlive()) continue;

		blockX = gameMain->block[i]->GetBlockX();
		blockY = gameMain->block[i]->GetBlockY();
		blockSize = gameMain->block[i]->GetBlockSize();
		if (collision->IsHit(x, y, size, blockX, blockY, blockSize)) {

			if (collision->IsHitWicth(preX, blockX, blockSize)) {
				y = preY;
			}
			else if (collision->IsHitHeight(preY, blockY, blockSize)) {
				x = preX;
			}
			else {
				y = preY;
				x = preX;
			}
			return;
		}
	}
}