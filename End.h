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
	End(FontData* font, InputManager* inputMNG, GameManager* gameMNG);	// コンストラクタ
	~End();					// デストラクタ
	void EndControll();		// エンド画面処理関数
	void DrawEnd();			// エンド画面描画関数
private:
	FontData* fontData;				// 生成した入力管理クラスのポインタ
	InputManager* inputManager;		// 生成した入力管理クラスのポインタ
	GameManager* gameManager;		// ゲーム進行管理クラスのポインタ
	GameMain* gameMain;				// ゲームメインクラスのポインタ

	const static int SELECT_NUM_MAX = 1;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "終了",
													   "いいえ"
	};

	int selectNum[SELECT_NUM_MAX + 1];				// 選択中のボタン番号

	void LoadImages();	// 画像読み込み
	void DeleteImages();	// 画像データメモリから消去

	int i_EndImage;	// 背景画像
	int i_MenuImage[3];	// 項目名画像
};

#endif // !_END_H_