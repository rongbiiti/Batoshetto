#include "GameMain.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameMain gameMain;

	int success = gameMain.FirstInit();

	if (success == 1) {
		gameMain.GameLoop();
	}

	// �I������
	if (DxLib_Init() == -1) return -1;
	DxLib_End();
	return 0;
}