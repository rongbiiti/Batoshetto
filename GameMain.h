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
#include "DifficultySelectScene.h"

class Bullet;
class Player;
class GameManager;
class Bullet;
class Result;
class InputManager;
class PauseScreen;
class Title;
class UI;
class Block;
class DifficultySelectScene;
const static unsigned int COLOR_VALUE_PLAYER[2] = { 0xE71122 , 0x1122E7 };
class GameMain {
public:
	GameMain();				// �R���X�g���N�^
	int FirstInit(void);	// �Q�[�����[�v�ɓ���O�ɂ��鏉��������
	void GameLoop(void);	// �Q�[�����[�v

	void Init(void);			// �Q�[�����v���C���ȂǂɃN���X�𐶐����Ȃ���

	const static int SCREEN_WIDTH = 1280;	// �v�Z�Ɏg����
	const static int SCREEN_HEIGHT = 720;	// �v�Z�Ɏg������
	const static int BLOCK_MAX = 81;		// �X�e�[�W��ɔz�u����u���b�N�̍ő吔
	
	int SCREEN_WIDTH_HALF;					// �v�Z�Ɏg����ʂ̉����̔����̐��l
	int SCREEN_HEIGHT_HALF;					// �v�Z�Ɏg����ʂ̍����̔����̐��l

	void SetPauseFlg(bool value) { pauseFlg = value; }	// �|�[�Y��ʂ��J���Ă��邩�̃t���O��؂�ւ�

	void CreateInputManagerObj();				// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateFontDataObj();					// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreatePlayerObj();						// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateBulletObj();						// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateGameManagerObj();				// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateBlockObj();						// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateResultObj(int hitPNum);			// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateTitleObj();						// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateDifficultySelectSceneObj();		// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreatePauseScreenObj();				// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateUIObj();						// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���

	InputManager* inputManager;		// �����������͊Ǘ��N���X�̃|�C���^
	FontData* fontData;				// ���������t�H���g�f�[�^�Ǘ��N���X�̃|�C���^
	Player* player[2];				// ���������v���C���[�N���X�̃|�C���^
	Bullet* bullet;					// ���������e�N���X�̃|�C���^
	GameManager* gameManager;		// ���������Q�[���i�s�Ǘ��N���X�̃|�C���^
	Block* block[BLOCK_MAX];		// ���������u���b�N�N���X�̃|�C���^
	Result* result;					// �����������U���g��ʊǗ��N���X�̃|�C���^
	Title* title;					// ���������^�C�g����ʊǗ��N���X�̃|�C���^
	DifficultySelectScene* diffiSelectScene;	// ����������Փx�I����ʂ̃|�C���^
	PauseScreen* pauseScreen;		// ���������|�[�Y��ʃN���X�̃|�C���^
	UI* ui;							// ��������UI�Ǘ��N���X�̃|�C���^

private:
	bool FPSUpdate(void);		// FPS���Œ肷�邽�߂̊֐�
	void UpdateWait(void);		// FPS���Œ肷�邽�߂̊֐�
	
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
	int pausePushPLNum;			// �|�[�Y�{�^�����������v���C���[�̔ԍ� 0=RED 1=BLUE 2=�L�[�{�[�h����̓���

};
#endif // !_GAMEMAIN_H_