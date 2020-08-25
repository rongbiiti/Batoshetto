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
	Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG);		// コンストラクタ
	~Title();						// デストラクタ
	void TitleControll();			// 処理用関数
	void DrawTitle();				// 描画用関数

	TitleBullet* titleBullet;		// 背景で反射している弾の描画クラスポインタ

private:
	FontData* fontData;				// 生成したフォント管理クラスのポインタ
	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	GameManager* gameManager;		// ゲーム進行管理クラスポインタ
	GameMain* gameMain;				// ゲームメインクラスのポインタ

	void HelpScreenControll();	// ヘルプ画面操作
	void DrawHelpScreen();		// ヘルプ画面描画
	void ProcessingOnDetermining(int controllerNum);	// 決定ボタン押下時の処理。引数は、コントローラーの番号

	int backAnimWaitTime;		// 背景の跳弾アニメの待ち時間

	void LoadImages();		// 画像読み込み

	const static int SELECT_NUM_MAX = 4;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Start",
													   "Online",
													   "Option",
													   "Help",
													   "End"
	};

	int selectNum[3];				// 選択中のボタン番号
	bool isOpenHelpScreen;		// ヘルプ画面を開いているか

	int i_helpImage;	// ヘルプ画像
	int i_LogoImage;	// タイトルロゴ画像
	int i_BGImage;		// 背景画像
	int i_MenuImage[5];	// 項目名画像
};

#endif // !_TITLE_H_