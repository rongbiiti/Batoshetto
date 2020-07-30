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
#include "End.h"
#include  "UI.h"
#include "DifficultySelectScene.h"
#include "Option.h"
#include "Network.h"

class Bullet;
class Player;
class GameManager;
class Bullet;
class Result;
class InputManager;
class PauseScreen;
class Title;
class End;
class UI;
class Block;
class DifficultySelectScene;
class Option;
const static unsigned int COLOR_VALUE_PLAYER[2] = { 0xdc143c , 0x4169e1 };
class GameMain {
public:
	GameMain();				// �R���X�g���N�^
	int FirstInit(void);	// �Q�[�����[�v�ɓ���O�ɂ��鏉��������
	void GameLoop(void);	// �Q�[�����[�v

	void Init(void);			// �Q�[�����v���C���ȂǂɃN���X�𐶐����Ȃ���
	void MainObjDelete(void);	// �v���C���[��u���b�N�Ȃǎ�v�ȃI�u�W�F�N�g�̃f�X�g���N�^���Ă�

	const static int SCREEN_WIDTH = 1280;	// �v�Z�Ɏg����
	const static int SCREEN_HEIGHT = 720;	// �v�Z�Ɏg������
	const static int BLOCK_MAX = 81;		// �X�e�[�W��ɔz�u����u���b�N�̍ő吔
	
	int SCREEN_WIDTH_HALF;					// �v�Z�Ɏg����ʂ̉����̔����̐��l
	int SCREEN_HEIGHT_HALF;					// �v�Z�Ɏg����ʂ̍����̔����̐��l

	void SetPauseFlg(bool value) { pauseFlg = value; }	// �|�[�Y��ʂ��J���Ă��邩�̃t���O��؂�ւ�
	int GetBlockImage(int num) { return i_BlockImages[num]; }	// �u���b�N�̉摜��n��

	void CreateInputManagerObj();				// ���͊Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateFontDataObj();					// �t�H���g�f�[�^�Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreatePlayerObj();						// �v���C���[�Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateBulletObj();						// �e�Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateGameManagerObj();				// �Q�[���i�s�Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateBlockObj();						// �u���b�N�Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateResultObj(int hitPNum);			// ���U���g��ʊǗ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateTitleObj();						// �^�C�g����ʊǗ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateEndObj();						// �G���h��ʊǗ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateDifficultySelectSceneObj();		// ��Փx�I����ʊǗ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreatePauseScreenObj();				// �|�[�Y��ʊǗ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateUIObj();							// UI�Ǘ��N���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateOptionObj(int pushPLnum, int prescreennum);						// �I�v�V������ʃN���X�𐶐����A�|�C���^��ۑ����Ă���
	void CreateNetworkObj();					// �ʐM�ΐ�p�N���X�𐶐����A�|�C���^��ۑ����Ă���

	InputManager* inputManager;		// �����������͊Ǘ��N���X�̃|�C���^
	FontData* fontData;				// ���������t�H���g�f�[�^�Ǘ��N���X�̃|�C���^
	Player* player[2];				// ���������v���C���[�N���X�̃|�C���^
	Bullet* bullet;					// ���������e�N���X�̃|�C���^
	GameManager* gameManager;		// ���������Q�[���i�s�Ǘ��N���X�̃|�C���^
	Block* block[BLOCK_MAX];		// ���������u���b�N�N���X�̃|�C���^
	Result* result;					// �����������U���g��ʊǗ��N���X�̃|�C���^
	Title* title;					// ���������^�C�g����ʊǗ��N���X�̃|�C���^
	End* end;						// ���������G���h��ʊǗ��N���X�̃|�C���^
	DifficultySelectScene* diffiSelectScene;	// ����������Փx�I����ʂ̃|�C���^
	PauseScreen* pauseScreen;		// ���������|�[�Y��ʃN���X�̃|�C���^
	UI* ui;							// ��������UI�Ǘ��N���X�̃|�C���^
	Option* option;					// ���������I�v�V������ʃN���X�̃|�C���^
	Network* network;				// ���������ʐM�ΐ�p�N���X�̃|�C���^

private:
	bool FPSUpdate(void);		// FPS���Œ肷�邽�߂̊֐�
	void UpdateWait(void);		// FPS���Œ肷�邽�߂̊֐�
	
	void Update(void);			// �I�u�W�F�N�g�̏�����i�߂Ēl���X�V����
	void Output(void);			// �I�u�W�F�N�g�̕`��n�֐����Ăяo��

	bool PauseProcess(void);	// �|�[�Y��ʂɊւ��鏈�����܂Ƃ܂��Ă���
	bool IsPushPauseButton(void);	// �|�[�Y��ʂ��J����{�^���������ꂽ���`�F�b�N

	void DrawDebugInfo(void);	// �f�o�b�O����`�悷�邽�߂̊֐�

	void LoadBlockImages();			// �u���b�N�摜�ǂݍ���
	void DeleteBlockImages();	// �u���b�N�̉摜����

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

	int i_BlockImages[3];	// �u���b�N�̉摜

};
#endif // !_GAMEMAIN_H_