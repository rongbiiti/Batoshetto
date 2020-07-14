// ゲーム全体の進行を管理するクラス
#ifndef _GAMEMAIN_H_
#define _GAMEMAIN_H_

#include "DxLib.h"
#include "InputManager.h"
#include "FontData.h"
#include "Block.h"
#include "Player.h"
#include "GameManager.h"
#include "Result.h"
#include "Bullet.h"
#include "PauseScreen.h"
#include "Title.h"
#include  "UI.h"

class Bullet;
class Player;
class GameManager;
class Bullet;
class Result;
class InputManager;
class PauseScreen;
class Title;
class UI;
const static unsigned int COLOR_VALUE_PLAYER[2] = { 0xE71122 , 0x1122E7 };
class GameMain {
public:
	GameMain();				// コンストラクタ
	int FirstInit(void);	// ゲームループに入る前にする初期化処理
	void GameLoop(void);	// ゲームループ

	const static int SCREEN_WIDTH = 1280;	// 計算に使う幅
	const static int SCREEN_HEIGHT = 720;	// 計算に使う高さ
	const static int BLOCK_MAX = 81;		// ステージ上に配置するブロックの最大数
	
	int SCREEN_WIDTH_HALF;					// 計算に使う画面の横幅の半分の数値
	int SCREEN_HEIGHT_HALF;					// 計算に使う画面の高さの半分の数値

	void SetPauseFlg(bool value) { pauseFlg = value; }

	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	FontData* fontData;				// 生成したフォントデータ管理クラスのポインタ
	Player* player[2];				// 生成したプレイヤークラスのポインタ
	Bullet* bullet;					// 生成した弾クラスのポインタ
	GameManager* gameManager;		// 生成したゲーム進行管理クラスのポインタ
	Block* block[BLOCK_MAX];		// 生成したブロッククラスのポインタ
	Result* result;					// 生成したリザルト画面管理クラスのポインタ
	Title* title;

private:
	PauseScreen* pauseScreen;		// 生成したポーズ画面クラスのポインタ
	UI* ui;

	bool FPSUpdate(void);		// FPSを固定するための関数
	void UpdateWait(void);		// FPSを固定するための関数

	void Init(void);			// ゲームリプレイ時などにクラスを生成しなおす
	
	void Update(void);			// オブジェクトの処理を進めて値を更新する
	void Output(void);			// オブジェクトの描画系関数を呼び出す

	bool IsPushPauseButton(void);	// ポーズ画面を開閉するボタンが押されたかチェック

	void DrawDebugInfo(void);	// デバッグ情報を描画するための関数

	const static int DRAW_SCREEN_WIDTH = 1280;	// 描画時のウィンドウの幅
	const static int DRAW_SCREEN_HEIGHT = 720;	// 描画時のウィンドウの高さ

	int mStartTime;      //測定開始時刻
	int mCount;          //カウンタ
	float mFps;          //fps
	static const int N = 60;	//平均を取るサンプル数
	static const int FPS = 60;	//設定したFPS

	int offscreen_handle;		// 描画画面を大きくするのに使う変数

	bool pauseFlg;				// ポーズ画面のフラグ

};
#endif // !_GAMEMAIN_H_