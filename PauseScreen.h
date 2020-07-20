//タイトルのクラス
#ifndef _PAUSE_SCREEN_H_
#define _PAUSE_SCREEN_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameMain.h"
#include <string>

class GameMain;
class PauseScreen {
public:
	PauseScreen(FontData* font, InputManager* input, GameMain* main, int pushPLnum);				// コンストラクタ
	~PauseScreen();				// デストラクタ

	void PauseScreenControll();	// ポーズ画面処理
	
	void DrawPauseScreen();		// ポーズ画面描画

private:
	FontData* fontData;				// 生成したフォントデータクラスのポインタ
	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	GameMain* gameMain;		// ゲーム進行管理クラスのポインタ

	void Return_to_Game();		// ポーズ画面を抜けて試合を再開する
	void OpenOptionScreen();	// オプション画面を開く
	void Return_to_Title();		// 試合を中断してタイトル画面へ戻る

	const static int SELECT_NUM_MAX = 2;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Return to Game",
													   "Option",
													   "Return to Title"
	};

	int selectNum[SELECT_NUM_MAX + 1];				// 選択中のボタン番号
	int pausePushPLNum;			// ポーズボタンを押したプレイヤーの番号 0=RED 1=BLUE 2=キーボードからの入力

};

#endif // !_PAUSE_SCREEN_H_