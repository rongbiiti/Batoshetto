#pragma once
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "DxLib.h"
#include "GameMain.h"

class GameMain;	// クラスのプロトタイプ宣言
class GameManager;
class Effect {
public:
	Effect(GameMain* main);	// コンストラクタ
	~Effect();	// デストラクタ
	void LoadImages();	// 画像読み込み

	void DrawEffect(int x, int y);	//エフェクトを描画
	

private:
	GameMain* gameMain;		// ゲームメインクラスのポインタ
	GameManager* gameManager;	// ゲーム進行管理クラスのポインタ
	int i_ShotEffect[4];	// マズルフラッシュの画像
};

#endif // !_UI_H_