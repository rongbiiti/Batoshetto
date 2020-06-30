// ゲームルールを管理するクラス
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "GameMain.h"
const static char PlayerName[2][5] = { "RED", "BLUE" };
class GameMain;
class GameManager {
public:
	GameManager(GameMain* main);
	~GameManager();
	enum PHASE {
		INIT,
		HIDE,
		SHOT,
		RECOCHETWAIT,
		RESULT
	};
	PHASE GamePhase;

	enum PLAYERCOLOR {
		RED,
		BLUE
	};
	PLAYERCOLOR PlayerColor;

	void ShooterChange(void);
	int GetHideTime() { return t_HideTime; }
	int GetShotTime() { return t_ShotTime; }
	int GetPhaseStatus() { return PhaseStatus; }
	int GetNowShooter() { return NowShooter; }
	int GetNowHider() { return NowHider; }

	void SetHideTime(void) { t_HideTime = HidePhaseTime; }
	void SetShotTime(void) { t_ShotTime = ShotPhaseTime; }
	void SetPhaseStatus(int value);
	void SetPhaseStatus(int value, int hitPlayerNum);

	void HideTimerControll(void);
	void ShotTimerControll(void);

	void ToHidePhase(void);	
	
private:
	GameMain* gameMain;

	const int HidePhaseTime = 300;
	const int ShotPhaseTime = 600;
	const int FirstShooter = RED;
	const int FirstHider = BLUE;

	int t_HideTime;
	int t_ShotTime;
	int PhaseStatus;
	int NowShooter;
	int NowHider;
	
};

#endif // !_GAMEMANAGER_H_