#include "GameManager.h"
#include <string>

////////////////////////////////////////////////
// �R���X�g���N�^�BGameMain�I�u�W�F�N�g�̃|�C���^���󂯎��B
////////////////////////////////////////////////
GameManager::GameManager(GameMain* main) {
	// ���������āA�t�F�[�Y���B��鑤�t�F�[�Y�Ɉڍs����
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	PhaseStatus = TITLE;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
	gameMain = main;

	LoadSounds();
}

////////////////////////////////////////////////
// ���Ԍn�̕ϐ�������
////////////////////////////////////////////////
void GameManager::Init() {
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
	PhaseStatus = HIDE;
}

////////////////////////////////////////////////
// �����E�B��鑤����シ��֐��B
////////////////////////////////////////////////
void GameManager::ShooterChange(void) {
	// ���݂̌�����RED��BLUE�����Q�Ƃ��A�����𕪊�
	switch (NowShooter)
	{
	case RED:
		NowShooter = BLUE;							// ���݂̌�����BLUE��
		NowHider = RED;								// ���݂̉B��鑤��RED��
		break;
	case BLUE:
		NowShooter = RED;							// ���݂̌�����RED��
		NowHider = BLUE;							// ���݂̌�����BLUE��
		break;
	}
}

////////////////////////////////////////////////
// �����E�B��鑤�̃t���O��ς���
// allchangeflg��true�Ȃ�ARED��BLUE�ǂ���̃t���O���ς���
////////////////////////////////////////////////
void GameManager::ShooterFlgChange(bool allchangeflg) {
	switch (NowShooter)
	{
	case RED:
		if (allchangeflg == false) {
			gameMain->player[RED]->SetIsShooter(false);		// RED���B��鑤��
			gameMain->player[BLUE]->SetIsShooter(false);	// BLUE��������
		}
		else {
			gameMain->player[RED]->SetIsShooter(true);		// RED���B��鑤��
		}
		
		break;
	case BLUE:
		if (allchangeflg == false) {
			gameMain->player[RED]->SetIsShooter(false);		// RED��������
			gameMain->player[BLUE]->SetIsShooter(false);		// BLUE���B��鑤��
		}
		else {
			gameMain->player[BLUE]->SetIsShooter(true);		// RED���B��鑤��
		}
		
		break;
	}
}

////////////////////////////////////////////////
// �Q�[���̃t�F�[�Y���Z�b�g����B�����ɁAPHASE�񋓑̂��g���Ă���B
////////////////////////////////////////////////
void GameManager::SetPhaseStatus(int value) {

	PhaseStatus = value;
}

////////////////////////////////////////////////
// �I�[�o�[���[�h�ŁA���������v���C���[��RED��BLUE����ǉ����Ă���BBullet����Ă΂��B
////////////////////////////////////////////////
void GameManager::SetPhaseStatus(int value, int hitPlayerNum) {
	PhaseStatus = value;

	// ���U���g��ʂ𐶐�����B
	// GameMain�I�u�W�F�N�g�̃��U���g�I�u�W�F�N�g�L���|�C���^�ɁA�����������U���g�̃|�C���^������
	gameMain->CreateResultObj(hitPlayerNum);
}

////////////////////////////////////////////////
// �B��鑤�̎c�莞�Ԃ��Ǘ����Ă���֐��B
////////////////////////////////////////////////
void GameManager::HideTimerControll(void) {
	// �B��鑤�c�莞�Ԃ����炷
	--t_HideTime;

	// �c�莞��0�ȉ��ɂȂ����猂���̃t�F�[�Y�Ɉڍs����B
	if (t_HideTime < 1) {
		ToShotPhase();
	}

	// �c�莞�Ԃ������Ă����Ƃ��̉�
	if (t_HideTime <= HidePhaseTime / 4) {
		if (t_HideTime % 12 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	else if (t_HideTime <= HidePhaseTime / 2) {
		if (t_HideTime % 24 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	else if (t_HideTime <= HidePhaseTime) {
		if (t_HideTime % 30 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
}

////////////////////////////////////////////////
// �����̎c�莞�Ԃ��Ǘ����Ă���֐��B
////////////////////////////////////////////////
void GameManager::ShotTimerControll(void) {
	// �����c�莞�Ԃ����炷
	--t_ShotTime;

	// �c�莞�Ԃ������Ă����Ƃ��̉�
	if (t_ShotTime <= ShotPhaseTime / 4) {
		if (t_ShotTime % 12 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	else if (t_ShotTime <= ShotPhaseTime / 2) {
		if (t_ShotTime % 24 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	else if (t_ShotTime <= ShotPhaseTime) {
		if (t_ShotTime % 30 == 0) PlaySoundMem(s_TimeLimitSE, DX_PLAYTYPE_BACK);
	}
	
}

////////////////////////////////////////////////
// �B��鑤�̎c�莞�Ԃ��Ǘ����Ă���֐��B�l�b�g�p
////////////////////////////////////////////////
void GameManager::HideTimerControll_Net(void) {
	// �B��鑤�c�莞�Ԃ����炷
	--t_HideTime;

	if (!gameMain->network->GetIsWaitRecvCheck() && t_HideTime < 0) {
		gameMain->network->SetIsWaitRecvCheck(TRUE);
	}

	if (t_HideTime < -600 && gameMain->network->GetIsWaitRecvCheck()) {
		SetPhaseStatus(RESULT);
		gameMain->CreateResultObj_TimeOut();
	}
}

////////////////////////////////////////////////
// �����̎c�莞�Ԃ��Ǘ����Ă���֐��B�l�b�g�p
////////////////////////////////////////////////
void GameManager::ShotTimerControll_Net(void) {
	// �����c�莞�Ԃ����炷
	--t_ShotTime;

	if (!gameMain->network->GetIsWaitRecvCheck() && t_ShotTime < 0) {
		gameMain->network->SetIsWaitRecvCheck(TRUE);
	}

	if (t_ShotTime < -600 && gameMain->network->GetIsWaitRecvCheck()) {
		SetPhaseStatus(RESULT);
		gameMain->CreateResultObj_TimeOut();
	}
}

////////////////////////////////////////////////
// �B��鑤�̃t�F�[�Y�Ɉڍs���鏈�����܂Ƃ߂Ă���B
////////////////////////////////////////////////
void GameManager::ToHidePhase(void) {
	SetHideTime();
	SetPhaseStatus(HIDE);
	ShooterChange();
	gameMain->ui->TransitionParameterReset();
}

////////////////////////////////////////////////
// �����̃t�F�[�Y�Ɉڍs���鏈�����܂Ƃ߂Ă���B
////////////////////////////////////////////////
void GameManager::ToShotPhase(void) {
	SetShotTime();
	SetPhaseStatus(SHOT);
	gameMain->ui->TransitionParameterReset();
}

////////////////////////////////////////////////
// ���f�[�^�ǂݍ���
////////////////////////////////////////////////
void GameManager::LoadSounds() {
	if ((s_TimeLimitSE = LoadSoundMem("sounds/limitgauge.mp3")) == -1) return;
}

////////////////////////////////////////////////
// ���̉��ʕύX
////////////////////////////////////////////////
void GameManager::ChangeVolume(float SEVolume) {
	int volume = 255.0f * SEVolume;

	ChangeVolumeSoundMem(volume, s_TimeLimitSE);
}

GameManager::~GameManager() {
	
}