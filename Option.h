// �I�@�v�@�V�@���@���@��@��
#define _CRT_SECURE_NO_WARNINGS
#ifndef _OPTION_H_
#define _OPTION_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameManager.h"
#include <stdio.h>
#include <stdlib.h>

class GameMain;
class GameManager;

class Option {
public:
	Option(GameMain* main, int pushPLnum, int prescreennum, int prephase = 3);	// �R���X�g���N�^
	~Option();	// �f�X�g���N�^

	enum OPTION_MENU_NUM {
		TOP,
		VOLUME
	};
	
	// �O��ʂ��ǂ���������
	enum PREVIOUS_SCREEN {
		TITLE,
		PAUSE
	};

	void OptionControll();	// �I�v�V������ʂ̏����֐�
	void DrawOption();		// �I�v�V������ʂ̕`��

	float GetBGMVolume() { return BGMVolume; }
	float GetSEVolume() { return SEVolume; }

private:
	GameMain* gameMain;	// �Q�[�����C���̃|�C���^
	InputManager* inputManager;	// ���͊Ǘ��N���X�̃|�C���^
	FontData* fontData;	// �t�H���g�f�[�^�Ǘ��N���X�̃|�C���^

	// �Z�[�u�p�̍\����
	typedef struct {
		float bgmVolume = 1.0f;
		float seVolume = 1.0f;
	}option_save_data_t;

	option_save_data_t saveData;

	void ControllOptionTop();	// �I�v�V�����g�b�v��ʂ̏���
	void ControllVolumeScreen();// ���ʒ��߉�ʂ̏���

	void DrawOptionMain();	// �I�v�V������ʂ̃g�b�v��`��
	void DrawVolumeMenu();	// ���ʒ��߉�ʂ�`��
	void BackOptionMenu();	// �e���߉�ʂ���g�b�v�ɖ߂邽�߂̊֐�
	void Return_to_PauseScreen();	// �|�[�Y��ʂ֖߂�
	void Return_to_Title();		// �^�C�g����ʂ֖߂�

	void PlayCursorSound() {}	// �J�[�\�����Đ�
	void PlayDicideSound() {}	// ���艹�Đ�
	void PlayCanselSound() {}	// �L�����Z�����Đ�

	void ChangeBulletSoundVolume();	// �e�̉��ʒ���

	void LoadFile();	// �Z�[�u�f�[�^�ǂݍ���
	void SaveFile();	// �Z�[�u�f�[�^�ۑ�

	const static int SELECT_NUM_MAX = 1;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Volume",
													   "Back"
	};

	const static int VOLUME_SELECT_NUM_MAX = 2;
	const std::string VolumeMenuName[VOLUME_SELECT_NUM_MAX + 1] = { "BGM",
																	"SE",
																	"BACK"
	};

	int screenNum;	// ���ǂ̉�ʂ��J���Ă邩�̔ԍ�
	int selectNum[SELECT_NUM_MAX + 1];				// �I�𒆂̃{�^���ԍ�
	int optionPushPLNum;			// �I�v�V�������J�����v���C���[�̔ԍ� 0=RED 1=BLUE 2=�L�[�{�[�h����̓���
	int previousNum;	// �^�C�g�����痈�����|�[�Y���痈�����𕪊�ł���
	int previousGamePhase;	// �|�[�Y��ʂ���������Ƃ��A���O�̃t�F�[�Y��ۑ����Ă���

	float BGMVolume;	// BGM�̉���
	float SEVolume;		// SE�̉���
};

#endif // !_OPTION_H_