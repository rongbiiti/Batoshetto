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
	void InitHitEffectCount(int x, int y);
	void DrawRicochetEffect();	//跳弾エフェクト描画
	void DrawEffect(int x, int y, float angle);	//エフェクトを描画
	void DrawHitEffect();
	bool MuzzleFlashEffectFlg = FALSE;	// マズルフラッシュflg、デフォルトはFALSE
	bool HitEffectFlg = FALSE;
	


private:
	bool RicochetEffectFlg[6];		// 跳弾時エフェクトflg
	int i_ShotEffect[4];	// マズルフラッシュの画像
	int i_RicochetEffect[4];	//跳弾時エフェクトの画像
	int i_HitEffect[6];		//ヒット時エフェクトの画像
	int effectcount;	// エフェクトのフレームカウント
	int RicochetEffectCount[6];		//跳弾時エフェクトのフレームカウント
	int HitEffectcount;
	int rico_x[6];
	int rico_y[6];
	float rico_angle[6];
	int hit_effect_x;
	int hit_effect_y;

};

#endif // !_UI_H_