//タイトルのクラス
#ifndef _TITLE_H_
#define _TITLE_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"
#include "TitleBullet.h"
#include <string>

class GameMain;
class GameManager;
class TitleBullet;
class Title {
public:
	Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG);
	~Title();
	void TitleControll();
	void DrawTitle();

	TitleBullet* titleBullet;		// 背景で反射している弾の描画
private:
	FontData* fontData;				// 生成したフォント管理クラスのポインタ
	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	GameManager* gameManager;
	GameMain* gameMain;

	//void Start();		// 試合を再開する
	//void OpenOptionScreen();	// オプション画面を開く
	//void EndSocreen();					// ゲームを終了する
	void HelpScreenControll();	// ヘルプ画面操作
	void DrawHelpScreen();		// ヘルプ画面描画

	int backAnimWaitTime;		// 背景の跳弾アニメの待ち時間

	void LoadImages();		// 画像読み込み

	const static int SELECT_NUM_MAX = 4;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Start",
													   "Online",
													   "Option",
													   "Help",
													   "End"
	};

	int selectNum[SELECT_NUM_MAX + 1];				// 選択中のボタン番号
	bool isOpenHelpScreen;		// ヘルプ画面を開いているか

	int i_helpImage;	// ヘルプ画像
	int i_LogoImage;	// タイトルロゴ画像
	int i_BGImage;		// 背景画像
	int i_MenuImage[5];	// 項目名画像
};

#endif // !_TITLE_H_