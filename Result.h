// リザルト画面描画用クラス
#ifndef _RESULT_H_
#define _RESULT_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"

class GameManager;
class Result {
public:
	// フォントデータ管理オブジェクトのポインタ、入力管理オブジェクトのポインタ、
	// ゲーム進行管理オブジェクトのポインタ、ヒットしたプレイヤーがREDかBLUEかを受け取る。
	Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum);
	~Result();					// デストラクタ
	void DrawResult(void);		// 描画
private:
	FontData* fontData;			// フォントデータ管理オブジェクトのポインタ
	InputManager* inputManager;	// 入力管理オブジェクトのポインタ
	GameManager* gameManager;	// ゲーム進行管理オブジェクトのポインタ
	int hitPlayerNumber;		// ヒットしたプレイヤーがREDかBLUEかの引数を保存しておく変数。
};

#endif // !_RESULT_H_