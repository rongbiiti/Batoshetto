#pragma once
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "DxLib.h"
#include "Player.h"

class Effect {
public:
	Effect();	// コンストラクタ
	~Effect();	// デストラクタ
	void LoadImages();	// 画像読み込み
	void InitEffectCount() { effectcount = 0; }

	void DrawEffect(int x, int y, float angle);	//エフェクトを描画
	int EffectStatus = 0;	// どのエフェクトかを判別する変数、デフォルトは0

private:
	int i_ShotEffect[4];	// マズルフラッシュの画像
	int effectcount;	// エフェクトのフレームカウント
};

#endif // !_UI_H_