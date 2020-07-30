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

	void DrawEffect();	
	

private:
	GameMain* gameMain;		// ゲームメインクラスのポインタ
	GameManager* gameManager;	// ゲーム進行管理クラスのポインタ
};

#endif // !_UI_H_