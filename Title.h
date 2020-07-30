//タイトルのクラス
#ifndef _TITLE_H_
#define _TITLE_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"
#include <string>

class GameMain;
class GameManager;
class Title {
public:
	Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG);
	~Title();
	void TitleControll();
	void DrawTitle();
private:
	FontData* fontData;				// 生成した入力管理クラスのポインタ
	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	GameManager* gameManager;
	GameMain* gameMain;

	//void Start();		// 試合を再開する
	//void OpenOptionScreen();	// オプション画面を開く
	//void EndSocreen();					// ゲームを終了する

	const static int SELECT_NUM_MAX = 3;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Start",
													   "Online",
													   "Option",
													   "End"
	};

	int selectNum[SELECT_NUM_MAX + 1];				// 選択中のボタン番号
};

#endif // !_TITLE_H_