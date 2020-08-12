#include "Effect.h"

// コンストラクタ
Effect::Effect() {
	LoadImages();
	for (int i = 0; i < 6; i++) {
		rico_x[i] = 0;
		rico_y[i] = 0;
		rico_angle[i] = 0;
		RicochetEffectCount[i] = 0;
		RicochetEffectFlg[i] = FALSE;
	}
}

void Effect::InitRicochetCount(int num, int x, int y, float angle) {
	rico_x[num] = x;
	rico_y[num] = y;
	rico_angle[num] = angle;
	RicochetEffectCount[num] = 0;
	RicochetEffectFlg[num] = TRUE;
}

//マズルフラッシュエフェクトを描画する
void Effect::DrawEffect(int x, int y, float angle) {
	
	if (effectcount <= 12) {
		DrawRotaGraph(x + cosf(angle * DX_PI_F / 180.0f) * 65, y + sinf(angle * DX_PI_F / 180.0f) * 65, 1.5f, angle * DX_PI_F / 180.0f, i_ShotEffect[effectcount / 3], TRUE);
		effectcount++;
	}
	else {
		MuzzleFlashEffectFlg = FALSE;		// flgを０に戻す
	}
}

//跳弾時エフェクトを描画
void Effect::DrawRicochetEffect() {
	for(int i = 0; i<6; i++){
		if (RicochetEffectCount[i] <= 12 && RicochetEffectFlg[i]) {
			DrawRotaGraph(rico_x[i], rico_y[i], 1.0f, rico_angle[i], i_RicochetEffect[++RicochetEffectCount[i] / 3], TRUE);
		}
		else {
			RicochetEffectFlg[i] = FALSE;		// flgを０に戻す
		}
	}
}

// 画像読み込み
void Effect::LoadImages() {

	if (!(i_ShotEffect[0] = LoadGraph("Image/ShotEffect01.png"))) return;
	if (!(i_ShotEffect[1] = LoadGraph("Image/ShotEffect02.png"))) return;
	if (!(i_ShotEffect[2] = LoadGraph("Image/ShotEffect03.png"))) return;
	if (!(i_ShotEffect[3] = LoadGraph("Image/ShotEffect04.png"))) return;

	if (!(i_RicochetEffect[0] = LoadGraph("Image/ShotEffect01.png"))) return;
	if (!(i_RicochetEffect[1] = LoadGraph("Image/ShotEffect02.png"))) return;
	if (!(i_RicochetEffect[2] = LoadGraph("Image/ShotEffect03.png"))) return;
	if (!(i_RicochetEffect[3] = LoadGraph("Image/ShotEffect04.png"))) return;

}

Effect::~Effect() {
	//DeleteImages();
}