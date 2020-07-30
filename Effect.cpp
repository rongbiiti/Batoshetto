#include "Effect.h"
#include "Player.h"

// コンストラクタ
Effect::Effect(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	LoadImages();
}

//エフェクトを描画する
void Effect::DrawEffect() {
	//DrawRotaGraph();
}

// 画像読み込み
void Effect::LoadImages() {

}

Effect::~Effect() {
	//DeleteImages();
}