// ��Փx�I����ʂ̊Ǘ��N���X
#ifndef _DIFFICULTYSELECTSCENE_H_
#define _DIFFICULTYSELECTSCENE_H_

#include "DxLib.h"
#include "InputManager.h"
#include "FontData.h"
#include "GameManager.h"

class DifficultySelectScene {
public:
	DifficultySelectScene(InputManager* input, FontData* font, GameManager* gameMNG);	// �R���X�g���N�^
	~DifficultySelectScene();					// �f�X�g���N�^

	void DifficultySelectControll();		// ��Փx�I����ʂ̏���
	void DifficultySelectControll_Net();	// ��Փx�I����ʁF�ʐM�ΐ�p
	void DrawDifficultySelectScene();	// ��Փx�I����ʂ̕`�揈��
	void DrawDifficultySelectScene_Net();	// ��Փx�I����ʂ̕`�揈���F�ʐM�ΐ�p
	bool GetReturnFlg() { return returnFlg; }	// �^�C�g������Q�ƁBTRUE�Ȃ�A��Փx�I����ʂ�delete����

private:
	InputManager* inputManager;	// �R���X�g���N�^�̈����Ŏ󂯎�����|�C���^
	FontData* fontData;			// �R���X�g���N�^�̈����Ŏ󂯎�����|�C���^
	GameManager* gameMangaer;	// �R���X�g���N�^�̈����Ŏ󂯎�����|�C���^

	void SetDifficulty();	// ��Փx��GameManager�̕ϐ��ɃZ�b�g���ăV�[����J��
	void LoadImages();		// �摜�ǂݍ���

	void DeleteImages();	// �摜�f�[�^�������������

	const static int SELECT_NUM_MAX = 1;
	const static int SCENE_TRANSITION_WAITING_TIME = 60;	// �V�[���J�ڂ���Ƃ��̂�����Ƃ����҂�����
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Casual",
													   "Expert"
	};

	int waitTime;
	int selectNum[SELECT_NUM_MAX + 1];	// �I�𒆂̃{�^���ԍ�
	bool dicideNumFlg[2];				// �ԍ������肵�����̃t���O
	bool returnFlg;		// �^�C�g����ʂ֖߂肽���t���O

	int i_BackImage;	// �w�i�摜
	int i_MenuImage[5];	// ���ږ��摜

};

#endif // !_DIFFICULTYSELECTSCENE_H_