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
#include "End.h"
#include  "UI.h"
#include "DifficultySelectScene.h"
#include "Option.h"
#include "Network.h"

class Bullet;
class Player;
class GameManager;
class Bullet;
class Result;
class InputManager;
class PauseScreen;
class Title;
class End;
class UI;
class Block;
class DifficultySelectScene;
class Option;
const static unsigned int COLOR_VALUE_PLAYER[2] = { 0xdc143c , 0x4169e1 };
class GameMain {
public:
	GameMain();				// コンストラクタ
	int FirstInit(void);	// ゲームループに入る前にする初期化処理
	void GameLoop(void);	// ゲームループ

	void Init(void);			// ゲームリプレイ時などにクラスを生成しなおす
	void MainObjDelete(void);	// プレイヤーやブロックなど主要なオブジェクトのデストラクタを呼ぶ

	const static int SCREEN_WIDTH = 1280;	// 計算に使う幅
	const static int SCREEN_HEIGHT = 720;	// 計算に使う高さ
	const static int BLOCK_MAX = 81;		// ステージ上に配置するブロックの最大数
	
	int SCREEN_WIDTH_HALF;					// 計算に使う画面の横幅の半分の数値
	int SCREEN_HEIGHT_HALF;					// 計算に使う画面の高さの半分の数値

	void SetPauseFlg(bool value) { pauseFlg = value; }	// ポーズ画面を開いているかのフラグを切り替え
	int GetBlockImage(int num) { return i_BlockImages[num]; }	// ブロックの画像を渡す

	void CreateInputManagerObj();				// 入力管理クラスを生成し、ポインタを保存しておく
	void CreateFontDataObj();					// フォントデータ管理クラスを生成し、ポインタを保存しておく
	void CreatePlayerObj();						// プレイヤー管理クラスを生成し、ポインタを保存しておく
	void CreateBulletObj();						// 弾管理クラスを生成し、ポインタを保存しておく
	void CreateGameManagerObj();				// ゲーム進行管理クラスを生成し、ポインタを保存しておく
	void CreateBlockObj();						// ブロック管理クラスを生成し、ポインタを保存しておく
	void CreateResultObj(int hitPNum);			// リザルト画面管理クラスを生成し、ポインタを保存しておく
	void CreateTitleObj();						// タイトル画面管理クラスを生成し、ポインタを保存しておく
	void CreateEndObj();						// エンド画面管理クラスを生成し、ポインタを保存しておく
	void CreateDifficultySelectSceneObj();		// 難易度選択画面管理クラスを生成し、ポインタを保存しておく
	void CreatePauseScreenObj();				// ポーズ画面管理クラスを生成し、ポインタを保存しておく
	void CreateUIObj();							// UI管理クラスを生成し、ポインタを保存しておく
	void CreateOptionObj(int pushPLnum, int prescreennum);						// オプション画面クラスを生成し、ポインタを保存しておく
	void CreateNetworkObj();					// 通信対戦用クラスを生成し、ポインタを保存しておく

	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	FontData* fontData;				// 生成したフォントデータ管理クラスのポインタ
	Player* player[2];				// 生成したプレイヤークラスのポインタ
	Bullet* bullet;					// 生成した弾クラスのポインタ
	GameManager* gameManager;		// 生成したゲーム進行管理クラスのポインタ
	Block* block[BLOCK_MAX];		// 生成したブロッククラスのポインタ
	Result* result;					// 生成したリザルト画面管理クラスのポインタ
	Title* title;					// 生成したタイトル画面管理クラスのポインタ
	End* end;						// 生成したエンド画面管理クラスのポインタ
	DifficultySelectScene* diffiSelectScene;	// 生成した難易度選択画面のポインタ
	PauseScreen* pauseScreen;		// 生成したポーズ画面クラスのポインタ
	UI* ui;							// 生成したUI管理クラスのポインタ
	Option* option;					// 生成したオプション画面クラスのポインタ
	Network* network;				// 生成した通信対戦用クラスのポインタ

private:
	bool FPSUpdate(void);		// FPSを固定するための関数
	void UpdateWait(void);		// FPSを固定するための関数
	
	void Update(void);			// オブジェクトの処理を進めて値を更新する
	void Output(void);			// オブジェクトの描画系関数を呼び出す

	bool PauseProcess(void);	// ポーズ画面に関する処理がまとまっている
	bool IsPushPauseButton(void);	// ポーズ画面を開閉するボタンが押されたかチェック

	void DrawDebugInfo(void);	// デバッグ情報を描画するための関数

	void LoadBlockImages();			// ブロック画像読み込み
	void DeleteBlockImages();	// ブロックの画像消去

	const static int DRAW_SCREEN_WIDTH = 1280;	// 描画時のウィンドウの幅
	const static int DRAW_SCREEN_HEIGHT = 720;	// 描画時のウィンドウの高さ

	int mStartTime;      //測定開始時刻
	int mCount;          //カウンタ
	float mFps;          //fps
	static const int N = 60;	//平均を取るサンプル数
	static const int FPS = 60;	//設定したFPS

	int offscreen_handle;		// 描画画面を大きくするのに使う変数

	bool pauseFlg;				// ポーズ画面のフラグ
	int pausePushPLNum;			// ポーズボタンを押したプレイヤーの番号 0=RED 1=BLUE 2=キーボードからの入力

	int i_BlockImages[3];	// ブロックの画像

};
#endif // !_GAMEMAIN_H_