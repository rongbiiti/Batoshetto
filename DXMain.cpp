#include "GameMain.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameMain gameMain;	// ゲームメイン生成

	int success = gameMain.FirstInit();

	if (success == 1) {
		gameMain.GameLoop();
	}

	// 終了処理
	if (DxLib_Init() == -1) return -1;
	DxLib_End();
	return 0;
}