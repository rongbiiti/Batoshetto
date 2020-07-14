//タイトルのクラス
#ifndef _TITLE_H_
#define _TITLE_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"

class GameManager;
class Title {
public:
	Title(FontData* font, InputManager* inputMNG, GameManager* gameMNG);
	~Title();
	void DrawTitle();
private:
	FontData* fontData;				// 生成した入力管理クラスのポインタ
	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	GameManager*gameManager;
};

#endif // !_TITLE_H_