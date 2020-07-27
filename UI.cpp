#include "UI.h"

// コンストラクタ
UI::UI(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	fontData = gameMain->fontData;
	LoadImages();
	TransitionParameterReset();
}

// パラメータを変更処理
void UI::UIControll() {

}

// 隠れる・撃つの切替時のアニメーション
// 処理が終わっていたらtrue、まだならfalseが返る
bool UI::TransitionAnimationWaiting() {
	if (++animationWaitingTime <= 45) {
		transitionX += 500 / 45;
		return false;
	}
	if (animationWaitingTime <= 90) {
		
		return false;
	}

	transitionFlg = false;
	return true;
}

void UI::DrawTransitionAnimation() {
	if (gameManager->GetPhaseStatus() == GameManager::HIDE) {
		DrawFormatStringToHandle(transitionX, transitionY, COLOR_VALUE_PLAYER[gameManager->GetNowHider()], fontData->f_FontData[1], "%s動け！", PlayerName[gameManager->GetNowHider()]);
	}
	else {
		DrawFormatStringToHandle(transitionX, transitionY, COLOR_VALUE_PLAYER[gameManager->GetNowShooter()], fontData->f_FontData[1], "%s撃て！", PlayerName[gameManager->GetNowShooter()]);
	}
}

void UI::TransitionParameterReset() {
	animationWaitingTime = 0;
	transitionX = 0;
	transitionY = 120;
	transitionFlg = true;
}

// プレイヤーの残り時間をゲージで描画する関数
void UI::DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum) {
	double percent = (maxtime - nowtime) / maxtime * 100;
	DrawCircleGauge(x, y, percent, i_PlayerGuage[playernum]);
}

// 背景画像描画
void UI::DrawBackGround() {
	DrawGraph(0, 0, i_BackGroundImage, 0);
}

// 画像読み込み
void UI::LoadImages() {
	if (!(i_PlayerGuage[0] = LoadGraph("Image/Gauge.png"))) return;
	if (!(i_PlayerGuage[1] = LoadGraph("Image/Gauge2.png"))) return;
	if (!(i_BackGroundImage = LoadGraph("Image/BackImage.png"))) return;
	
}

// 画像消去
void UI::DeleteImages() {
	DeleteGraph(i_PlayerGuage[0]);
	DeleteGraph(i_PlayerGuage[1]);
	DeleteGraph(i_BackGroundImage);
}

UI::~UI() {
	//DeleteImages();
}