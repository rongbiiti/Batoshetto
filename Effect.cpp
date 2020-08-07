#include "Effect.h"

// �R���X�g���N�^
Effect::Effect() {
	LoadImages();
}

//�G�t�F�N�g��`�悷��
void Effect::DrawEffect(int x, int y, float angle) {
	
	if(effectcount<=12){
	DrawRotaGraph(x + cosf(angle * DX_PI_F / 180.0f) * 65, y + sinf(angle * DX_PI_F / 180.0f) * 65, 1.5f, angle * DX_PI_F / 180.0f, i_ShotEffect[effectcount/3], TRUE);
	effectcount++;
	}else{
		EffectStatus = 0;		// �G�t�F�N�g�X�e�[�^�X���O�ɖ߂�
	}
}

// �摜�ǂݍ���
void Effect::LoadImages() {
	if (!(i_ShotEffect[0] = LoadGraph("Image/ShotEffect01.png"))) return;
	if (!(i_ShotEffect[1] = LoadGraph("Image/ShotEffect02.png"))) return;
	if (!(i_ShotEffect[2] = LoadGraph("Image/ShotEffect03.png"))) return;
	if (!(i_ShotEffect[3] = LoadGraph("Image/ShotEffect04.png"))) return;

}

Effect::~Effect() {
	//DeleteImages();
}