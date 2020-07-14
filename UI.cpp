#include "UI.h"

// コンストラクタ
UI::UI(GameMain* main) {
	gameMain = main;
	LoadImages();
}

// パラメータを変更処理
void UI::UIControll() {

}

// プレイヤーの残り時間をゲージで描画する関数
void UI::DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum) {
	double percent = (maxtime - nowtime) / maxtime * 100;
	DrawCircleGauge(x, y, percent, i_PlayerGuage[playernum]);
	DrawFormatString(600, 600, 0xFFFFFF, "%.3f", percent);
}

// 画像読み込み
void UI::LoadImages() {
	if (!(i_PlayerGuage[0] = LoadGraph("Image/Gauge.png"))) return;
}