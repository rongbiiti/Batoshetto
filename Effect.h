#pragma once
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "DxLib.h"
#include "Player.h"

class Effect {
public:
	Effect();	// �R���X�g���N�^
	~Effect();	// �f�X�g���N�^
	void LoadImages();	// �摜�ǂݍ���
	void InitEffectCount() { effectcount = 0; }

	void DrawEffect(int x, int y, float angle);	//�G�t�F�N�g��`��
	int EffectStatus = 0;	// �ǂ̃G�t�F�N�g���𔻕ʂ���ϐ��A�f�t�H���g��0

private:
	int i_ShotEffect[4];	// �}�Y���t���b�V���̉摜
	int effectcount;	// �G�t�F�N�g�̃t���[���J�E���g
};

#endif // !_UI_H_