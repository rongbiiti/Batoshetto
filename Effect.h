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
	void InitRicochetCount(int num, int x, int y, float angle);
	void DrawRicochetEffect();	//���e�G�t�F�N�g�`��
	void DrawEffect(int x, int y, float angle);	//�G�t�F�N�g��`��
	bool MuzzleFlashEffectFlg = FALSE;	// �}�Y���t���b�V��flg�A�f�t�H���g��FALSE
	


private:
	bool RicochetEffectFlg[6];		// ���e���G�t�F�N�gflg
	int i_ShotEffect[4];	// �}�Y���t���b�V���̉摜
	int i_RicochetEffect[4];	//���e���G�t�F�N�g�̉摜
	int effectcount;	// �G�t�F�N�g�̃t���[���J�E���g
	int RicochetEffectCount[6];		//���e���G�t�F�N�g�̃t���[���J�E���g
	int rico_x[6];
	int rico_y[6];
	float rico_angle[6];

};

#endif // !_UI_H_