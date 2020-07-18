#include "UI.h"

// �R���X�g���N�^
UI::UI(GameMain* main) {
	gameMain = main;
	LoadImages();
}

// �p�����[�^��ύX����
void UI::UIControll() {

}

// �v���C���[�̎c�莞�Ԃ��Q�[�W�ŕ`�悷��֐�
void UI::DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum) {
	double percent = (maxtime - nowtime) / maxtime * 100;
	DrawCircleGauge(x, y, percent, i_PlayerGuage[playernum]);
}

// �摜�ǂݍ���
void UI::LoadImages() {
	if (!(i_PlayerGuage[0] = LoadGraph("Image/Gauge.png"))) return;
	if (!(i_PlayerGuage[1] = LoadGraph("Image/Gauge2.png"))) return;
	
}