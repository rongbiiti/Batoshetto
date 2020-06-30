// �v���C���[�Ǘ��N���X
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameMain.h"
#include "Collision.h"

class GameMain;
class Player {
public:
	Player(int num, unsigned int color, bool shooter, GameMain* main);
	~Player();
	void HidingPlayerControll(void);
	void ShooterPlayerControll(void);

	void DrawPlayer(void);
	void DrawTargetAngle(void);
	void SetIsShooter(bool value) { isShooter = value; }

	bool GetIsShooter(void) { return isShooter; }
	int GetPlayerX(void) { return x; }
	int GetPlayerY(void) { return y; }
	int GetPlayerSize(void) { return size; }
	int GetPlayerNum(void) { return num; }

private:
	Collision* collision;

	const int PlayerMoveSpeed = 3;
	const int PlayerStartPositionX[2] = { 60, 1100 };
	const int PlayerStartPositionY[2] = { 200, 600 };
	const int PlayerSize = 18;

	void BlockHitCheck(void);		// �u���b�N�Ɠ����蔻��

	GameMain* gameMain;
	InputManager* inputManager;
	int x, y;				// ���W x,y
	float targetx, targety;	// �Ə��̍��W
	int size;				// �T�C�Y���a
	unsigned int color;		// �F
	int moveSpeed;			// �ړ����x
	bool isShooter;			// �������A�ۂ��B
	int angle;				// �p�x
	int preX, preY;			// �ړ��O�̍��W
	int num;				// RED��BLUE��
};

#endif // !_PLAYER_H_