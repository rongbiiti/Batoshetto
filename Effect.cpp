#include "Effect.h"
#include "Player.h"

// �R���X�g���N�^
Effect::Effect(GameMain* main) {
	gameMain = main;
	gameManager = gameMain->gameManager;
	LoadImages();
}

//�G�t�F�N�g��`�悷��
void Effect::DrawEffect() {
	//DrawRotaGraph();
}

// �摜�ǂݍ���
void Effect::LoadImages() {

}

Effect::~Effect() {
	//DeleteImages();
}