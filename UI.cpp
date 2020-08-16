#include "UI.h"

////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
UI::UI(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	fontData = gameMain->fontData;
	LoadImages();
	TransitionParameterReset();
	isBattleStart = TRUE;
	firstAnimWaitTime = 0;
	firstAnimX = 500;
	firstAnimY = 0;
	firstAnimAlpha = 90;
}

////////////////////////////////////////////////
// パラメータを変更処理
////////////////////////////////////////////////
void UI::UIControll() {

}

////////////////////////////////////////////////
// 試合開始時のアニメ。演出が終わっていたらFALSEを返す
////////////////////////////////////////////////
bool UI::BattleStartAnim() {
	// 試合開始直後で無ければ処理を終わる
	if (!isBattleStart) return false;

	// 規定の秒数を過ぎていたら処理を終わる
	if (180 < ++firstAnimWaitTime) {
		isBattleStart = FALSE;
		return false;
	}
	
	if (firstAnimWaitTime <= 45) {
		firstAnimY += 500 / 45;
		return true;
	}
	if (firstAnimWaitTime <= 90) {
		return true;
	}
	if (firstAnimWaitTime <= 180) {
		--firstAnimAlpha;
		return true;
	}

	return false;

}

////////////////////////////////////////////////
// 隠れる・撃つの切替時のアニメーション
// 処理が終わっていたらtrue、まだならfalseが返る
////////////////////////////////////////////////
bool UI::TransitionAnimationWaiting() {
	if (BattleStartAnim()) return false;

	if (++animationWaitingTime <= 1) {
		gameMain->network->BufferClear();
		gameMain->network->StructsReset();
	}
	
	if (animationWaitingTime <= 35) {

		if ((gameManager->GetPhaseStatus() == GameManager::SHOT && gameManager->GetNowShooter() == GameManager::BLUE) ||
			(gameManager->GetPhaseStatus() == GameManager::HIDE && gameManager->GetNowHider() == GameManager::BLUE)) {
			transitionX -= (gameMain->SCREEN_WIDTH_HALF + 80) / 35;
		}
		else {
			transitionX += (gameMain->SCREEN_WIDTH_HALF + 10) / 35;
		}

		return false;
	}
	if (animationWaitingTime <= 55) {
		
		return false;
	}
	if (animationWaitingTime <= 90) {
		transitionY -= 270 / 35;
		transitionExRate -= 1.0 / 35.0;
		return false;
	}

	if (gameManager->GetPhaseStatus() == GameManager::SHOT) {
		gameManager->ShooterFlgChange(true);
	}
	else {
		gameManager->ShooterFlgChange(false);
	}
	transitionFlg = false;
	
	return true;
}

void UI::DrawTransitionAnimation() {
	// 試合開始直後で無ければ処理を終わる
	if (isBattleStart) {
		DrawBattleStartAnim();
		return;
	}

	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2;

	if (gameManager->GetPhaseStatus() == GameManager::HIDE) {
		//DrawFormatStringToHandle(transitionX, transitionY, COLOR_VALUE_PLAYER[gameManager->GetNowHider()], fontData->f_FontData[1], "%s動け！", PlayerName[gameManager->GetNowHider()]);
		DrawRotaGraph(transitionX, transitionY, transitionExRate, 0, i_OrderImage[gameManager->GetNowHider()][gameManager->GetPhaseStatus() - GameManager::HIDE], TRUE);

		// 操作方法表示
		if (gameManager->GetDifficulty() == GameManager::CASUAL && animationWaitingTime >= 90) {
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "左スティック：移動");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 40, 0xFFFFFF, fontData->f_FontData[0], "左スティック：移動");
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "Xボタン：パス");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 60, 0xFFFFFF, fontData->f_FontData[0], "Xボタン：パス");
		}
		
	}
	else {
		//DrawFormatStringToHandle(transitionX, transitionY, COLOR_VALUE_PLAYER[gameManager->GetNowShooter()], fontData->f_FontData[1], "%s撃て！", PlayerName[gameManager->GetNowShooter()]);
		DrawRotaGraph(transitionX, transitionY, transitionExRate, 0, i_OrderImage[gameManager->GetNowShooter()][gameManager->GetPhaseStatus() - GameManager::HIDE], TRUE);

		// 操作方法表示
		if (gameManager->GetDifficulty() == GameManager::CASUAL && animationWaitingTime >= 90) {
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "左スティックか");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 40, 0xFFFFFF, fontData->f_FontData[0], "左スティックか");
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "十字キー上下：ねらう");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 60, 0xFFFFFF, fontData->f_FontData[0], "十字キー上下：ねらう");

			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "Xボタン：パス");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 100, 0xFFFFFF, fontData->f_FontData[0], "Xボタン：パス");
			fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[0], "Bボタン：発射");
			DrawFormatStringToHandle(transitionX - fontwidth / 2, transitionY + 120, 0xFFFFFF, fontData->f_FontData[0], "Bボタン：発射");
		}
		
	}

}

////////////////////////////////////////////////
// 試合開始時のアニメ描画
////////////////////////////////////////////////
void UI::DrawBattleStartAnim() {
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * firstAnimAlpha / 90);
	fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[1], "あ");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 70) + y, 0xFFFFFF, gameMain->fontData->f_FontData[2], "勝");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 200) + y, 0xFFFFFF, gameMain->fontData->f_FontData[2], "負");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 330) + y, 0xFFFFFF, gameMain->fontData->f_FontData[2], "開");
	DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 460) + y, 0xFFFFFF, gameMain->fontData->f_FontData[2], "始");

	//fontwidth = GetDrawFormatStringWidthToHandle(gameMain->fontData->f_FontData[1], "Battle Start");
	//DrawFormatStringToHandle(x - fontwidth / 2, firstAnimY - (500 - 560), 0xFFFFFF, gameMain->fontData->f_FontData[1], "Battle Start");
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void UI::TransitionParameterReset() {
	animationWaitingTime = 0;
	transitionExRate = 2.0;
	if ( ( gameManager->GetPhaseStatus() == GameManager::SHOT && gameManager->GetNowShooter() == GameManager::BLUE ) ||
		 ( gameManager->GetPhaseStatus() == GameManager::HIDE && gameManager->GetNowHider() == GameManager::BLUE ) ) {
		transitionX = 1340;
	}
	else {
		transitionX = 0;
	}
	
	transitionY = 330;
	transitionFlg = true;
}

////////////////////////////////////////////////
// プレイヤーの残り時間をゲージで描画する関数
////////////////////////////////////////////////
void UI::DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum) {
	double percent = (maxtime - nowtime) / maxtime * 100;
	DrawCircleGauge(x, y, percent, i_PlayerGuage[playernum]);
}

////////////////////////////////////////////////
// 背景画像描画
////////////////////////////////////////////////
void UI::DrawBackGround() {
	DrawGraph(0, 0, i_BackGroundImage, 0);
}

////////////////////////////////////////////////
// 画像読み込み
////////////////////////////////////////////////
void UI::LoadImages() {
	if (!(i_PlayerGuage[0] = LoadGraph("Image/Gauge.png"))) return;
	if (!(i_PlayerGuage[1] = LoadGraph("Image/Gauge2.png"))) return;
	if (!(i_BackGroundImage = LoadGraph("Image/BackImage.png"))) return;
	if (!(i_OrderImage[0][0] = LoadGraph("Image/Order_Move_RED.png"))) return;
	if (!(i_OrderImage[0][1] = LoadGraph("Image/Order_Shot_RED.png"))) return;
	if (!(i_OrderImage[1][0] = LoadGraph("Image/Order_Move_BLUE.png"))) return;
	if (!(i_OrderImage[1][1] = LoadGraph("Image/Order_Shot_BLUE.png"))) return;
}

////////////////////////////////////////////////
// 画像消去
////////////////////////////////////////////////
void UI::DeleteImages() {
	i_PlayerGuage[0] = DeleteGraph(i_PlayerGuage[0]);
	i_PlayerGuage[1] = DeleteGraph(i_PlayerGuage[1]);
	i_BackGroundImage = DeleteGraph(i_BackGroundImage);
	i_OrderImage[0][0] = DeleteGraph(i_OrderImage[0][0]);
	i_OrderImage[0][1] = DeleteGraph(i_OrderImage[0][1]);
	i_OrderImage[1][0] = DeleteGraph(i_OrderImage[1][0]);
	i_OrderImage[1][1] = DeleteGraph(i_OrderImage[1][1]);
}

UI::~UI() {
	DeleteImages();
}