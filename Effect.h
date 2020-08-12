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
	void InitRicochetCount(int num, int x, int y, float angle);
	void DrawRicochetEffect();	//跳弾エフェクト描画
	void DrawEffect(int x, int y, float angle);	//エフェクトを描画
	bool MuzzleFlashEffectFlg = FALSE;	// マズルフラッシュflg、デフォルトはFALSE
	


private:
	bool RicochetEffectFlg[6];		// 跳弾時エフェクトflg
	int i_ShotEffect[4];	// マズルフラッシュの画像
	int i_RicochetEffect[4];	//跳弾時エフェクトの画像
	int effectcount;	// エフェクトのフレームカウント
	int RicochetEffectCount[6];		//跳弾時エフェクトのフレームカウント
	int rico_x[6];
	int rico_y[6];
	float rico_angle[6];

};

#endif // !_UI_H_