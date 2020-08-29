#include "Effect.h"

// コンストラクタ
Effect::Effect() {
	LoadImages();

	// 配列の初期化
	for (int i = 0; i < 6; i++) {
		rico_x[i] = 0;
		rico_y[i] = 0;
		rico_angle[i] = 0;
		RicochetEffectCount[i] = 0;
		RicochetEffectFlg[i] = FALSE;
	}
	hit_effect_x = 0;
	hit_effect_y = 0;
	HitEffectcount = 0;
	HitEffectFlg = FALSE;		// flgを０に戻す
}

//初期値設定
void Effect::InitRicochetCount(int num, int x, int y, float angle) {
	rico_x[num] = x;
	rico_y[num] = y;
	rico_angle[num] = angle;
	RicochetEffectCount[num] = 0;
	RicochetEffectFlg[num] = TRUE;
}

//初期値設定
void Effect::InitHitEffectCount(int x, int y) {
	hit_effect_x = x;
	hit_effect_y = y;
	HitEffectcount = 0;
	HitEffectFlg = TRUE;		// flgを０に戻す
}

//マズルフラッシュエフェクトを描画する
void Effect::DrawEffect(int x, int y, float angle) {
	
	if (effectcount < 12) {
		DrawRotaGraph(x + cosf(angle * DX_PI_F / 180.0f) * 65, y + sinf(angle * DX_PI_F / 180.0f) * 65, 1.5f,
			angle * DX_PI_F / 180.0f, i_ShotEffect[effectcount / 3], TRUE);	// 3フレーム間隔で画像を更新

		effectcount++;
	}
	else {
		MuzzleFlashEffectFlg = FALSE;		// flgを０に戻す
	}
}

//跳弾時エフェクトを描画
void Effect::DrawRicochetEffect() {
	for(int i = 0; i<6; i++){
		if (RicochetEffectCount[i] < 12 && RicochetEffectFlg[i]) {
			DrawRotaGraph(rico_x[i], rico_y[i], 1.0f, rico_angle[i], i_RicochetEffect[(++RicochetEffectCount[i]-1) / 3], TRUE);
			//3フレーム間隔で画像を更新
		}
		else {
			RicochetEffectFlg[i] = FALSE;		// flgを０に戻す
		}
	}
}

//ヒット時エフェクトを描画
void Effect::DrawHitEffect() {
	if (HitEffectcount < 18 && HitEffectFlg) {
		DrawRotaGraph(hit_effect_x , hit_effect_y , 1.0f, 180.0f, i_HitEffect[HitEffectcount / 3], TRUE);
		//3フレーム間隔で画像を更新
		HitEffectcount++;
	}
	else {
		HitEffectFlg = FALSE;		// flgを０に戻す
	}
}

// 画像読み込み
void Effect::LoadImages() {

	if (!(i_ShotEffect[0] = LoadGraph("Image/ShotEffect01.png"))) return;
	if (!(i_ShotEffect[1] = LoadGraph("Image/ShotEffect02.png"))) return;
	if (!(i_ShotEffect[2] = LoadGraph("Image/ShotEffect03.png"))) return;
	if (!(i_ShotEffect[3] = LoadGraph("Image/ShotEffect04.png"))) return;

	if (!(i_RicochetEffect[0] = LoadGraph("Image/RicochetEffect01.png"))) return;
	if (!(i_RicochetEffect[1] = LoadGraph("Image/RicochetEffect02.png"))) return;
	if (!(i_RicochetEffect[2] = LoadGraph("Image/RicochetEffect03.png"))) return;
	if (!(i_RicochetEffect[3] = LoadGraph("Image/RicochetEffect04.png"))) return;

	if (!(i_HitEffect[0] = LoadGraph("Image/HitEffect01.png"))) return;
	if (!(i_HitEffect[1] = LoadGraph("Image/HitEffect02.png"))) return;
	if (!(i_HitEffect[2] = LoadGraph("Image/HitEffect03.png"))) return;
	if (!(i_HitEffect[3] = LoadGraph("Image/HitEffect04.png"))) return;
	if (!(i_HitEffect[4] = LoadGraph("Image/HitEffect05.png"))) return;
	if (!(i_HitEffect[5] = LoadGraph("Image/HitEffect06.png"))) return;

}

// 画像消去
void Effect::DeleteImages() {
	i_ShotEffect[0] = DeleteGraph(i_ShotEffect[0]);
	i_ShotEffect[1] = DeleteGraph(i_ShotEffect[1]);
	i_ShotEffect[2] = DeleteGraph(i_ShotEffect[2]);
	i_ShotEffect[3] = DeleteGraph(i_ShotEffect[3]);

	i_RicochetEffect[0] = DeleteGraph(i_RicochetEffect[0]);
	i_RicochetEffect[1] = DeleteGraph(i_RicochetEffect[1]);
	i_RicochetEffect[2] = DeleteGraph(i_RicochetEffect[2]);
	i_RicochetEffect[3] = DeleteGraph(i_RicochetEffect[3]);

	i_HitEffect[0] = DeleteGraph(i_HitEffect[0]);
	i_HitEffect[1] = DeleteGraph(i_HitEffect[1]);
	i_HitEffect[2] = DeleteGraph(i_HitEffect[2]);
	i_HitEffect[3] = DeleteGraph(i_HitEffect[3]);
	i_HitEffect[4] = DeleteGraph(i_HitEffect[4]);
	i_HitEffect[5] = DeleteGraph(i_HitEffect[5]);
}

Effect::~Effect() {
	DeleteImages();
}