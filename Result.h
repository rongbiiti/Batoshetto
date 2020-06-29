#ifndef _RESULT_H_
#define _RESULT_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"

class GameManager;
class Result {
public:
	Result(FontData* font, InputManager* input, GameManager* gameMNG, int num);
	~Result();
	void DrawResult(void);
private:
	FontData* fontData;
	InputManager* inputManager;
	GameManager* gameManager;
	int num;
};

#endif // !_RESULT_H_