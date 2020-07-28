//タイトルのクラス
#ifndef _END_H_
#define _END_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"
#include <string>

class GameMain;
class GameManager;
class End {
public:
	End(FontData* font, InputManager* inputMNG, GameManager* gameMNG);
	~End();
	void EndControll();
	void DrawEnd();
private:
	FontData* fontData;				// 生成した入力管理クラスのポインタ
	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	GameManager* gameManager;
	GameMain* gameMain;

	const static int SELECT_NUM_MAX = 1;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Yes",
													   "No"
	};

	int selectNum[SELECT_NUM_MAX + 1];				// 選択中のボタン番号
};

#endif // !_END_H_