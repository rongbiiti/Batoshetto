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
	void InitEffectCount() { effectcount = 0; }	// 初期値設定
	void InitRicochetCount(int num, int x, int y, float angle);	// 初期値設定
	void InitHitEffectCount(int x, int y);	// 初期値設定
	void DrawRicochetEffect();	//跳弾エフェクト描画
	void DrawEffect(int x, int y, float angle);	//エフェクトを描画
	void DrawHitEffect();	//ヒット時エフェクトの描画
	bool MuzzleFlashEffectFlg = FALSE;	// マズルフラッシュflg、デフォルトはFALSE
	bool HitEffectFlg = FALSE;		// ヒット時エフェクトflg
	void DeleteImages();	// 画像消去


private:
	bool RicochetEffectFlg[6];		// 跳弾時エフェクトflg
	int i_ShotEffect[4];	// マズルフラッシュの画像
	int i_RicochetEffect[4];	//跳弾時エフェクトの画像
	int i_HitEffect[6];		//ヒット時エフェクトの画像
	int effectcount;	// エフェクトのフレームカウント
	int RicochetEffectCount[6];		//跳弾時エフェクトのフレームカウント
	int HitEffectcount;				// ヒット時エフェクトのフレームカウント
	int rico_x[6];					// 跳弾時エフェクトのX座標
	int rico_y[6];					// 跳弾時エフェクトのY座標
	float rico_angle[6];			// 跳弾時エフェクトのアングル変更
	int hit_effect_x;				// ヒット時エフェクトのX座標
	int hit_effect_y;				//　ヒット時エフェクトのY座標

};

#endif // !_UI_H_