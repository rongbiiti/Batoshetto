#include "Effect.h"
#include "Player.h"

// �R���X�g���N�^
Effect::Effect(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	LoadImages();
}

//�G�t�F�N�g��`�悷��
void Effect::DrawEffect(int x, int y) {
	//DrawRotaGraph(x,y, 1.5f, player -> angle * DX_PI_F / 180.0f, i_ShotEffect[0], TRUE);
}

// �摜�ǂݍ���
void Effect::LoadImages() {
	if (!(i_ShotEffect[0] = LoadGraph("Image/Gauge.png"))) return;
}

Effect::~Effect() {
	//DeleteImages();
}