#include "GameManager.h"
#include <string>

////////////////////////////////////////////////
// コンストラクタ。GameMainオブジェクトのポインタを受け取る。
////////////////////////////////////////////////
GameManager::GameManager(GameMain* main) {
	// 初期化して、フェーズを隠れる側フェーズに移行する
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	PhaseStatus = TITLE;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
	gameMain = main;

	LoadSounds();
}

////////////////////////////////////////////////
// 時間系の変数初期化
////////////////////////////////////////////////
void GameManager::Init() {
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
	PhaseStatus = HIDE;
}

////////////////////////////////////////////////
// 撃つ側・隠れる側を交代する関数。
////////////////////////////////////////////////
void GameManager::ShooterChange(void) {
	// 現在の撃つ側がREDかBLUEかを参照し、処理を分岐
	switch (NowShooter)
	{
	case RED:
		NowShooter = BLUE;							// 現在の撃つ側をBLUEに
		NowHider = RED;								// 現在の隠れる側をREDに
		break;
	case BLUE:
		NowShooter = RED;							// 現在の撃つ側をREDに
		NowHider = BLUE;							// 現在の撃つ側をBLUEに
		break;
	}
}

////////////////////////////////////////////////
// 撃つ側・隠れる側のフラグを変える
// allchangeflgがtrueなら、REDとBLUEどちらのフラグも変える
////////////////////////////////////////////////
void GameManager::ShooterFlgChange(bool allchangeflg) {
	switch (NowShooter)
	{
	case RED:
		if (allchangeflg == false) {
			gameMain->player[RED]->SetIsShooter(false);		// REDを隠れる側に
			gameMain->player[BLUE]->SetIsShooter(false);	// BLUEを撃つ側に
		}
		else {
			gameMain->player[RED]->SetIsShooter(true);		// REDを隠れる側に
		}
		
		break;
	case BLUE:
		if (allchangeflg == false) {
			gameMain->player[RED]->SetIsShooter(false);		// REDを撃つ側に
			gameMain->player[BLUE]->SetIsShooter(false);		// BLUEを隠れる側に
		}
		else {
			gameMain->player[BLUE]->SetIsShooter(true);		// REDを隠れる側に
		}
		
		break;
	}
}

////////////////////////////////////////////////
// ゲームのフェーズをセットする。引数に、PHASE列挙体を使ってくれ。
////////////////////////////////////////////////
void GameManager::SetPhaseStatus(int value) {

	PhaseStatus = value;
}

////////////////////////////////////////////////
// オーバーロードで、当たったプレイヤーがREDかBLUEかを追加している。Bulletから呼ばれる。
////////////////////////////////////////////////
void GameManager::SetPhaseStatus(int value, int hitPlayerNum) {
	PhaseStatus = value;

	// リザルト画面を生成する。
	// GameMainオブジェクトのリザルトオブジェクト記憶ポインタに、生成したリザルトのポインタを入れる
	gameMain->CreateResultObj(hitPlayerNum);
}

////////////////////////////////////////////////
// 隠れる側の残り時間を管理している関数。
////////////////////////////////////////////////
void GameManager::HideTimerControll(void) {
	// 隠れる側残り時間を減らす
	--t_HideTime;

	// 残り時間0以下になったら撃つ側のフェーズに移行する。
	if (t_HideTime < 1) {
		ToShotPhase();
	}

	// 残り時間が迫ってきたときの音
	if (t_HideTime <= HidePhaseTime / 4) {
		if (t_HideTime % 12 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	else if (t_HideTime <= HidePhaseTime / 2) {
		if (t_HideTime % 24 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	else if (t_HideTime <= HidePhaseTime) {
		if (t_HideTime % 30 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
}

////////////////////////////////////////////////
// 撃つ側の残り時間を管理している関数。
////////////////////////////////////////////////
void GameManager::ShotTimerControll(void) {
	// 撃つ側残り時間を減らす
	--t_ShotTime;

	// 残り時間が迫ってきたときの音
	if (t_ShotTime <= ShotPhaseTime / 4) {
		if (t_ShotTime % 12 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	else if (t_ShotTime <= ShotPhaseTime / 2) {
		if (t_ShotTime % 24 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	else if (t_ShotTime <= ShotPhaseTime) {
		if (t_ShotTime % 30 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	
}

////////////////////////////////////////////////
// 隠れる側の残り時間を管理している関数。ネット用
////////////////////////////////////////////////
void GameManager::HideTimerControll_Net(void) {
	// 隠れる側残り時間を減らす
	--t_HideTime;

	if (!gameMain->network->GetIsWaitRecvCheck() && t_HideTime < 0) {
		gameMain->network->SetIsWaitRecvCheck(TRUE);
	}

	if (t_HideTime < -600 && gameMain->network->GetIsWaitRecvCheck()) {
		SetPhaseStatus(RESULT);
		gameMain->CreateResultObj_TimeOut();
	}
}

////////////////////////////////////////////////
// 撃つ側の残り時間を管理している関数。ネット用
////////////////////////////////////////////////
void GameManager::ShotTimerControll_Net(void) {
	// 撃つ側残り時間を減らす
	--t_ShotTime;

	if (!gameMain->network->GetIsWaitRecvCheck() && t_ShotTime < 0) {
		gameMain->network->SetIsWaitRecvCheck(TRUE);
	}

	if (t_ShotTime < -600 && gameMain->network->GetIsWaitRecvCheck()) {
		SetPhaseStatus(RESULT);
		gameMain->CreateResultObj_TimeOut();
	}
}

////////////////////////////////////////////////
// 隠れる側のフェーズに移行する処理がまとめてある。
////////////////////////////////////////////////
void GameManager::ToHidePhase(void) {
	SetHideTime();
	SetPhaseStatus(HIDE);
	ShooterChange();
	gameMain->ui->TransitionParameterReset();
}

////////////////////////////////////////////////
// 撃つ側のフェーズに移行する処理がまとめてある。
////////////////////////////////////////////////
void GameManager::ToShotPhase(void) {
	SetShotTime();
	SetPhaseStatus(SHOT);
	gameMain->ui->TransitionParameterReset();
}

////////////////////////////////////////////////
// 音データ読み込み
////////////////////////////////////////////////
void GameManager::LoadSounds() {
	if ((s_TimeLimitSE = LoadSoundMem("sounds/limitgauge.mp3")) == -1) return;
}

////////////////////////////////////////////////
// 音の音量変更
////////////////////////////////////////////////
void GameManager::ChangeVolume(float SEVolume) {
	int volume = 255.0f * SEVolume;

	ChangeVolumeSoundMem(volume, s_TimeLimitSE);
}

GameManager::~GameManager() {
	
}