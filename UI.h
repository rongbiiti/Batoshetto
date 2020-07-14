// 金城龍吾PresentsUIクラス
#ifndef _UI_H_
#define _UI_H_

#include "DxLib.h"
#include "GameMain.h"

class GameMain;	// クラスのプロトタイプ宣言
class UI {
public:
	UI(GameMain* main);	// コンストラクタ

	void UIControll();	// パラメータを変更処理

	// プレイヤーの残り時間をゲージで描画する関数
	void DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum);

private:
	GameMain* gameMain;		// ゲームメインクラスのポインタ

	void LoadImages();	// 画像読み込み

	int i_PlayerGuage[2];	// ゲージの画像
};

#endif // !_UI_H_