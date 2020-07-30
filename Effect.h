#pragma once
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "DxLib.h"
#include "GameMain.h"

class GameMain;	// �N���X�̃v���g�^�C�v�錾
class GameManager;
class Effect {
public:
	Effect(GameMain* main);	// �R���X�g���N�^
	~Effect();	// �f�X�g���N�^
	void LoadImages();	// �摜�ǂݍ���

	void DrawEffect();	
	

private:
	GameMain* gameMain;		// �Q�[�����C���N���X�̃|�C���^
	GameManager* gameManager;	// �Q�[���i�s�Ǘ��N���X�̃|�C���^
};

#endif // !_UI_H_