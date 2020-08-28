// 難易度選択画面の管理クラス
#ifndef _DIFFICULTYSELECTSCENE_H_
#define _DIFFICULTYSELECTSCENE_H_

#include "DxLib.h"
#include "InputManager.h"
#include "FontData.h"
#include "GameManager.h"

class DifficultySelectScene {
public:
	DifficultySelectScene(InputManager* input, FontData* font, GameManager* gameMNG);	// コンストラクタ
	~DifficultySelectScene();					// デストラクタ

	void DifficultySelectControll();		// 難易度選択画面の処理
	void DifficultySelectControll_Net();	// 難易度選択画面：通信対戦用
	void DrawDifficultySelectScene();	// 難易度選択画面の描画処理
	void DrawDifficultySelectScene_Net();	// 難易度選択画面の描画処理：通信対戦用
	bool GetReturnFlg() { return returnFlg; }	// タイトルから参照。TRUEなら、難易度選択画面をdeleteする

private:
	InputManager* inputManager;	// コンストラクタの引数で受け取ったポインタ
	FontData* fontData;			// コンストラクタの引数で受け取ったポインタ
	GameManager* gameMangaer;	// コンストラクタの引数で受け取ったポインタ

	void SetDifficulty();	// 難易度をGameManagerの変数にセットしてシーンを遷移
	void LoadImages();		// 画像読み込み

	void DeleteImages();	// 画像データメモリから消去

	const static int SELECT_NUM_MAX = 1;
	const static int SCENE_TRANSITION_WAITING_TIME = 60;	// シーン遷移するときのちょっとした待ち時間
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Casual",
													   "Expert"
	};

	int waitTime;
	int selectNum[SELECT_NUM_MAX + 1];	// 選択中のボタン番号
	bool dicideNumFlg[2];				// 番号を決定したかのフラグ
	bool returnFlg;		// タイトル画面へ戻りたいフラグ

	int i_BackImage;	// 背景画像
	int i_MenuImage[5];	// 項目名画像

};

#endif // !_DIFFICULTYSELECTSCENE_H_