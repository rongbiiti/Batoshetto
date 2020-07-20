// ゲームルールを管理するクラス
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "GameMain.h"
const static char PlayerName[2][5] = { "RED", "BLUE" };
class GameMain;
class GameManager {
public:
	GameManager(GameMain* main);		// コンストラクタ。GameMainオブジェクトのポインタを受け取る。
	~GameManager();						// デストラクタ。

	// GameMainでゲームの処理進行を管理しているswitch文をわかりやすくするために使っている列挙体。
	// GameMain以外でも使っているところがある。
	enum PHASE {
		TITLE,
		DIFFICULTYSELECT,
		INIT,
		HIDE,
		SHOT,
		RECOCHETWAIT,
		RESULT
	};

	// プレイヤーの管理が配列なので、その添え字をわかりやすく指定するときに使う。
	enum PLAYERCOLOR {
		RED,
		BLUE
	};

	// 難易度。ブロックの配置方法が変わる
	enum DIFFICULTY {
		BEGINNER,
		CASUAL,
		EXPERT
	};

	const static int HidePhaseTime = 240;		// 隠れる側残り時間の初期値。
	const static int ShotPhaseTime = 600;		// 撃つ側残り時間の初期値。
	const static int FirstShooter = RED;		// REDとBLUE、どっちが試合の最初に撃つ方か
	const static int FirstHider = BLUE;		// REDとBLUE、どっちが試合の最初に隠れる方か

	void ShooterChange(void);						// 撃つ側・隠れる側を交代する関数。
	int GetDifficulty() { return difficulty; }		// ゲームの難易度を返す。
	int GetHideTime() { return t_HideTime; }		// 隠れる側残り時間を返す。
	int GetShotTime() { return t_ShotTime; }		// 撃つ側残り時間を返す。
	int GetPhaseStatus() { return PhaseStatus; }	// 現在のゲームのフェーズを返す。
	int GetNowShooter() { return NowShooter; }		// 現在の撃つ側がREDかBLUEかを返す。
	int GetNowHider() { return NowHider; }			// 現在の隠れる側がREDかBLUEかを返す。

	void SetDifficulty(int value) { difficulty = value; }	// 難易度をセットする。
	void SetHideTime(void) { t_HideTime = HidePhaseTime; }	// 隠れる側残り時間をリセットする。
	void SetShotTime(void) { t_ShotTime = ShotPhaseTime; }	// 撃つ側残り時間をリセットする。
	void SetPhaseStatus(int value);							// ゲームのフェーズをセットする。引数に、PHASE列挙体を使ってくれ。
	void SetPhaseStatus(int value, int hitPlayerNum);		// オーバーロードで、当たったプレイヤーがREDかBLUEかを追加している。Bulletから呼ばれる。

	void HideTimerControll(void);					// 隠れる側の残り時間を管理している関数。
	void ShotTimerControll(void);					// 撃つ側の残り時間を管理している関数。

	void ToHidePhase(void);							// 隠れる側のフェーズに移行する処理がまとめてある。
	void ToShotPhase(void);							// 撃つ側のフェーズに移行する処理がまとめてある。

	GameMain* gameMain;					// GameMainオブジェクトのポインタを保存しておく変数。

	void Init();	// 変数初期化
	
private:
	int difficulty;		// ゲームの難易度

	int t_HideTime;		// 隠れる側の残り時間
	int t_ShotTime;		// 撃つ側の残り時間
	int PhaseStatus;	// 現在のゲームのフェーズ
	int NowShooter;		// 現在の撃つ側
	int NowHider;		// 現在の隠れる側
	
};

#endif // !_GAMEMANAGER_H_