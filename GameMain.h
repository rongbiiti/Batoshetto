// �Q�[���S�̂̐i�s���Ǘ�����N���X
#ifndef _GAMEMAIN_H_
#define _GAMEMAIN_H_

#include "DxLib.h"
#include "InputManager.h"
#include "FontData.h"
#include "Block.h"
#include "Player.h"
#include "GameManager.h"
#include "Result.h"
#include "Bullet.h"
#include "PauseScreen.h"
#include "Title.h"
#include  "UI.h"

class Bullet;
class Player;
class GameManager;
class Bullet;
class Result;
class InputManager;
class PauseScreen;
class Title;
class UI;
const static unsigned int COLOR_VALUE_PLAYER[2] = { 0xE71122 , 0x1122E7 };
class GameMain {
public:
	GameMain();				// �R���X�g���N�^
	int FirstInit(void);	// �Q�[�����[�v�ɓ���O�ɂ��鏉��������
	void GameLoop(void);	// �Q�[�����[�v

	const static int SCREEN_WIDTH = 1280;	// �v�Z�Ɏg����
	const static int SCREEN_HEIGHT = 720;	// �v�Z�Ɏg������
	const static int BLOCK_MAX = 81;		// �X�e�[�W��ɔz�u����u���b�N�̍ő吔
	
	int SCREEN_WIDTH_HALF;					// �v�Z�Ɏg����ʂ̉����̔����̐��l
	int SCREEN_HEIGHT_HALF;					// �v�Z�Ɏg����ʂ̍����̔����̐��l

	void SetPauseFlg(bool value) { pauseFlg = value; }

	InputManager* inputManager;		// �����������͊Ǘ��N���X�̃|�C���^
	FontData* fontData;				// ���������t�H���g�f�[�^�Ǘ��N���X�̃|�C���^
	Player* player[2];				// ���������v���C���[�N���X�̃|�C���^
	Bullet* bullet;					// ���������e�N���X�̃|�C���^
	GameManager* gameManager;		// ���������Q�[���i�s�Ǘ��N���X�̃|�C���^
	Block* block[BLOCK_MAX];		// ���������u���b�N�N���X�̃|�C���^
	Result* result;					// �����������U���g��ʊǗ��N���X�̃|�C���^
	Title* title;

private:
	PauseScreen* pauseScreen;		// ���������|�[�Y��ʃN���X�̃|�C���^
	UI* ui;

	bool FPSUpdate(void);		// FPS���Œ肷�邽�߂̊֐�
	void UpdateWait(void);		// FPS���Œ肷�邽�߂̊֐�

	void Init(void);			// �Q�[�����v���C���ȂǂɃN���X�𐶐����Ȃ���
	
	void Update(void);			// �I�u�W�F�N�g�̏�����i�߂Ēl���X�V����
	void Output(void);			// �I�u�W�F�N�g�̕`��n�֐����Ăяo��

	bool IsPushPauseButton(void);	// �|�[�Y��ʂ��J����{�^���������ꂽ���`�F�b�N

	void DrawDebugInfo(void);	// �f�o�b�O����`�悷�邽�߂̊֐�

	const static int DRAW_SCREEN_WIDTH = 1280;	// �`�掞�̃E�B���h�E�̕�
	const static int DRAW_SCREEN_HEIGHT = 720;	// �`�掞�̃E�B���h�E�̍���

	int mStartTime;      //����J�n����
	int mCount;          //�J�E���^
	float mFps;          //fps
	static const int N = 60;	//���ς����T���v����
	static const int FPS = 60;	//�ݒ肵��FPS

	int offscreen_handle;		// �`���ʂ�傫������̂Ɏg���ϐ�

	bool pauseFlg;				// �|�[�Y��ʂ̃t���O

};
#endif // !_GAMEMAIN_H_