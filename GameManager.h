// �Q�[�����[�����Ǘ�����N���X
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "GameMain.h"
const static char PlayerName[2][5] = { "RED", "BLUE" };
class GameMain;
class GameManager {
public:
	GameManager(GameMain* main);		// �R���X�g���N�^�BGameMain�I�u�W�F�N�g�̃|�C���^���󂯎��B
	~GameManager();						// �f�X�g���N�^�B

	// GameMain�ŃQ�[���̏����i�s���Ǘ����Ă���switch�����킩��₷�����邽�߂Ɏg���Ă���񋓑́B
	// GameMain�ȊO�ł��g���Ă���Ƃ��낪����B
	enum PHASE {
		TITLE,
		DIFFICULTYSELECT,
		INIT,
		HIDE,
		SHOT,
		RECOCHETWAIT,
		RESULT
	};

	// �v���C���[�̊Ǘ����z��Ȃ̂ŁA���̓Y�������킩��₷���w�肷��Ƃ��Ɏg���B
	enum PLAYERCOLOR {
		RED,
		BLUE
	};

	// ��Փx�B�u���b�N�̔z�u���@���ς��
	enum DIFFICULTY {
		BEGINNER,
		CASUAL,
		EXPERT
	};

	const static int HidePhaseTime = 240;		// �B��鑤�c�莞�Ԃ̏����l�B
	const static int ShotPhaseTime = 600;		// �����c�莞�Ԃ̏����l�B
	const static int FirstShooter = RED;		// RED��BLUE�A�ǂ����������̍ŏ��Ɍ�����
	const static int FirstHider = BLUE;		// RED��BLUE�A�ǂ����������̍ŏ��ɉB������

	void ShooterChange(void);						// �����E�B��鑤����シ��֐��B
	int GetDifficulty() { return difficulty; }		// �Q�[���̓�Փx��Ԃ��B
	int GetHideTime() { return t_HideTime; }		// �B��鑤�c�莞�Ԃ�Ԃ��B
	int GetShotTime() { return t_ShotTime; }		// �����c�莞�Ԃ�Ԃ��B
	int GetPhaseStatus() { return PhaseStatus; }	// ���݂̃Q�[���̃t�F�[�Y��Ԃ��B
	int GetNowShooter() { return NowShooter; }		// ���݂̌�����RED��BLUE����Ԃ��B
	int GetNowHider() { return NowHider; }			// ���݂̉B��鑤��RED��BLUE����Ԃ��B

	void SetDifficulty(int value) { difficulty = value; }	// ��Փx���Z�b�g����B
	void SetHideTime(void) { t_HideTime = HidePhaseTime; }	// �B��鑤�c�莞�Ԃ����Z�b�g����B
	void SetShotTime(void) { t_ShotTime = ShotPhaseTime; }	// �����c�莞�Ԃ����Z�b�g����B
	void SetPhaseStatus(int value);							// �Q�[���̃t�F�[�Y���Z�b�g����B�����ɁAPHASE�񋓑̂��g���Ă���B
	void SetPhaseStatus(int value, int hitPlayerNum);		// �I�[�o�[���[�h�ŁA���������v���C���[��RED��BLUE����ǉ����Ă���BBullet����Ă΂��B

	void HideTimerControll(void);					// �B��鑤�̎c�莞�Ԃ��Ǘ����Ă���֐��B
	void ShotTimerControll(void);					// �����̎c�莞�Ԃ��Ǘ����Ă���֐��B

	void ToHidePhase(void);							// �B��鑤�̃t�F�[�Y�Ɉڍs���鏈�����܂Ƃ߂Ă���B
	void ToShotPhase(void);							// �����̃t�F�[�Y�Ɉڍs���鏈�����܂Ƃ߂Ă���B

	GameMain* gameMain;					// GameMain�I�u�W�F�N�g�̃|�C���^��ۑ����Ă����ϐ��B

	void Init();	// �ϐ�������
	
private:
	int difficulty;		// �Q�[���̓�Փx

	int t_HideTime;		// �B��鑤�̎c�莞��
	int t_ShotTime;		// �����̎c�莞��
	int PhaseStatus;	// ���݂̃Q�[���̃t�F�[�Y
	int NowShooter;		// ���݂̌���
	int NowHider;		// ���݂̉B��鑤
	
};

#endif // !_GAMEMANAGER_H_