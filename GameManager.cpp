#include "GameManager.h"
#include <string>

GameManager::GameManager(GameMain* main) {
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	PhaseStatus = HIDE;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
	gameMain = main;
}

void GameManager::ShooterChange(void) {
	switch (NowShooter)
	{
	case RED:
		gameMain->player[RED]->SetIsShooter(false);
		gameMain->player[BLUE]->SetIsShooter(true);
		NowShooter = BLUE;
		NowHider = RED;
		break;
	case BLUE:
		gameMain->player[RED]->SetIsShooter(true);
		gameMain->player[BLUE]->SetIsShooter(false);
		NowShooter = RED;
		NowHider = BLUE;
		break;
	}
}

void GameManager::SetPhaseStatus(int value) {
	PhaseStatus = value;
}

void GameManager::SetPhaseStatus(int value, int hitPlayerNum) {
	PhaseStatus = value;
	gameMain->result = new Result(gameMain->fontData, gameMain->inputManager, this, hitPlayerNum);
	
}

void GameManager::HideTimerControll(void) {
	--t_HideTime;
	if (t_HideTime < 1) {
		SetShotTime();
		SetPhaseStatus(SHOT);
	}
}

void GameManager::ShotTimerControll(void) {
	--t_ShotTime;
	if (t_ShotTime < 1) {
		SetHideTime();
		SetPhaseStatus(HIDE);
		ShooterChange();
	}
}

void GameManager::ToHidePhase(void) {
	SetHideTime();
	SetPhaseStatus(HIDE);
	ShooterChange();
}

GameManager::~GameManager() {
	
}