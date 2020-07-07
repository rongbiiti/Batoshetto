// プレイヤー管理クラス
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Collision.h"
#include "GameMain.h"

class GameMain;
class Player {
public:
	// コンストラクタ。REDかBLUEか、色、撃つ側か否か、GameMainオブジェクトのポインタを引数で受け取る。
	Player(int num, unsigned int color, bool shooter, GameMain* main);
	~Player();							// デストラクタ
	void HidingPlayerControll(void);	// 隠れる側時の操作処理
	void ShooterPlayerControll(void);	// 撃つ側時の操作処理

	void DrawPlayer(void);				// 描画用
	void DrawTargetAngle(void);			// 撃つ側時に狙っている方向に線を引いて描画する
	void SetIsShooter(bool value) { isShooter = value; }		// メンバ変数isShooterに値をセットする

	bool GetIsShooter(void) { return isShooter; }		// 現在撃つ側か否かを返す
	int GetPlayerX(void) { return x; }					// プレイヤーのX中心座標を返す
	int GetPlayerY(void) { return y; }					// プレイヤーのY中心座標を返す
	int GetPlayerSize(void) { return size; }			// プレイヤーの直径サイズを返す
	int GetPlayerNum(void) { return num; }				// プレイヤーがREDかBLUEかを返す

private:
	Collision* collision;	// 生成した衝突判定するクラスのポインタ

	const int PlayerMoveSpeed = 5;						// プレイヤーの移動速度
	const int PlayerStartPositionX[2] = { 60, 1100 };	// プレイヤーの初期X座標
	const int PlayerStartPositionY[2] = { 200, 600 };	// プレイヤーの初期Y座標
	const int PlayerSize = 80;							// プレイヤーの直径サイズ

	float AngleCorrection(float ang);			// 角度を0～360度に収まるように調整
	int XCoordinateCorrection(int posx, int pnum, int size);	// X座標が指定の範囲を超えていないか、プレイヤーの番号をもとに修正
	int YCoordinateCorrection(int posy, int size);				// Y座標が指定の範囲を超えないように修正

	void BlockHitCheck(void);					// ブロックと当たり判定
	void TargetPointWindowHitCheck(void);		// 弾が進む方向がウィンドウを飛び出してないかチェックする
	bool TrajectoryPrecalculation_ToBlock(int* blocknum);	// ブロックに対する弾道の事前計算。
	int TrajectoryPrecalculation_ToWindow(void);	// ウィンドウに対する弾道の事前計算。
	void CalcHitAfterAngle_ToBlock(int blocknum);	// ブロックのどの辺と衝突しているか判断して角度を変更する
	void CalcHitAfterAngle_ToWindow(int num);		// ウィンドウのどの端と衝突しているか判断して角度を変更する

	GameMain* gameMain;			// コンストラクタで受け取ったGameMainのポインタを保存しておく変数
	InputManager* inputManager;	// 入力管理クラスのポインタを入れる変数
	int x, y;					// 座標 x,y
	float targetx, targety, targetx2, targety2;		// 照準の座標
	int size;					// サイズ直径
	unsigned int color;			// 色
	int moveSpeed;				// 移動速度
	bool isShooter;				// 撃つ方か、否か。
	float angle, angle2;		// 角度
	int preX, preY;				// 移動前の座標
	int num;					// REDかBLUEか
};

#endif // !_PLAYER_H_