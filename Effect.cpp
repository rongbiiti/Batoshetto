#include "Effect.h"
#include "Player.h"

// コンストラクタ
Effect::Effect(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	LoadImages();
}

//エフェクトを描画する
void Effect::DrawEffect(int x, int y) {
	//DrawRotaGraph(x,y, 1.5f, player -> angle * DX_PI_F / 180.0f, i_ShotEffect[0], TRUE);
}

// 画像読み込み
void Effect::LoadImages() {
	if (!(i_ShotEffect[0] = LoadGraph("Image/Gauge.png"))) return;
}

Effect::~Effect() {
	//DeleteImages();
}