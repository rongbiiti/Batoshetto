#include "GameManager.h"
#include <string>

// コンストラクタ。GameMainオブジェクトのポインタを受け取る。
GameManager::GameManager(GameMain* main) {
	// 初期化して、フェーズを隠れる側フェーズに移行する
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	PhaseStatus = TITLE;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
	gameMain = main;
}

// 時間系の変数初期化
void GameManager::Init() {
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
}

// 撃つ側・隠れる側を交代する関数。
void GameManager::ShooterChange(void) {
	// 現在の撃つ側がREDかBLUEかを参照し、処理を分岐
	switch (NowShooter)
	{
	case RED:
		gameMain->player[RED]->SetIsShooter(false);	// REDを隠れる側に
		gameMain->player[BLUE]->SetIsShooter(true);	// BLUEを撃つ側に
		NowShooter = BLUE;							// 現在の撃つ側をBLUEに
		NowHider = RED;								// 現在の隠れる側をREDに
		break;
	case BLUE:
		gameMain->player[RED]->SetIsShooter(true);	// REDを撃つ側に
		gameMain->player[BLUE]->SetIsShooter(false);// BLUEを隠れる側に
		NowShooter = RED;							// 現在の撃つ側をREDに
		NowHider = BLUE;							// 現在の撃つ側をBLUEに
		break;
	}
}

// ゲームのフェーズをセットする。引数に、PHASE列挙体を使ってくれ。
void GameManager::SetPhaseStatus(int value) {
	PhaseStatus = value;
}

// オーバーロードで、当たったプレイヤーがREDかBLUEかを追加している。Bulletから呼ばれる。
void GameManager::SetPhaseStatus(int value, int hitPlayerNum) {
	PhaseStatus = value;

	// リザルト画面を生成する。
	// GameMainオブジェクトのリザルトオブジェクト記憶ポインタに、生成したリザルトのポインタを入れる
	gameMain->CreateResultObj(hitPlayerNum);
}

// 隠れる側の残り時間を管理している関数。
void GameManager::HideTimerControll(void) {
	// 隠れる側残り時間を減らす
	--t_HideTime;

	// 残り時間0以下になったら撃つ側のフェーズに移行する。
	if (t_HideTime < 1) {
		ToShotPhase();
	}
}

// 撃つ側の残り時間を管理している関数。
void GameManager::ShotTimerControll(void) {
	// 撃つ側残り時間を減らす
	--t_ShotTime;

	//残り時間0以下になったら隠れる側のフェーズに移行する。
	if (t_ShotTime < 1) {
		ToHidePhase();
	}
}

// 隠れる側のフェーズに移行する処理がまとめてある。
void GameManager::ToHidePhase(void) {
	SetHideTime();
	SetPhaseStatus(HIDE);
	ShooterChange();
	gameMain->ui->TransitionParameterReset();
}

// 撃つ側のフェーズに移行する処理がまとめてある。
void GameManager::ToShotPhase(void) {
	SetShotTime();
	SetPhaseStatus(SHOT);
	gameMain->ui->TransitionParameterReset();
}

GameManager::~GameManager() {
	
}