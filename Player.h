// プレイヤー管理クラス
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

	void BlockHitCheck(void);		// ブロックと当たり判定

	GameMain* gameMain;
	InputManager* inputManager;
	int x, y;				// 座標 x,y
	float targetx, targety;	// 照準の座標
	int size;				// サイズ直径
	unsigned int color;		// 色
	int moveSpeed;			// 移動速度
	bool isShooter;			// 撃つ方か、否か。
	int angle;				// 角度
	int preX, preY;			// 移動前の座標
	int num;				// REDかBLUEか
};

#endif // !_PLAYER_H_