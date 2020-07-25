#include "GameManager.h"
#include <string>

// �R���X�g���N�^�BGameMain�I�u�W�F�N�g�̃|�C���^���󂯎��B
GameManager::GameManager(GameMain* main) {
	// ���������āA�t�F�[�Y���B��鑤�t�F�[�Y�Ɉڍs����
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	PhaseStatus = TITLE;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
	gameMain = main;
}

// ���Ԍn�̕ϐ�������
void GameManager::Init() {
	t_HideTime = HidePhaseTime;
	t_ShotTime = ShotPhaseTime;
	NowShooter = FirstShooter;
	NowHider = FirstHider;
}

// �����E�B��鑤����シ��֐��B
void GameManager::ShooterChange(void) {
	// ���݂̌�����RED��BLUE�����Q�Ƃ��A�����𕪊�
	switch (NowShooter)
	{
	case RED:
		gameMain->player[RED]->SetIsShooter(false);	// RED���B��鑤��
		gameMain->player[BLUE]->SetIsShooter(true);	// BLUE��������
		NowShooter = BLUE;							// ���݂̌�����BLUE��
		NowHider = RED;								// ���݂̉B��鑤��RED��
		break;
	case BLUE:
		gameMain->player[RED]->SetIsShooter(true);	// RED��������
		gameMain->player[BLUE]->SetIsShooter(false);// BLUE���B��鑤��
		NowShooter = RED;							// ���݂̌�����RED��
		NowHider = BLUE;							// ���݂̌�����BLUE��
		break;
	}
}

// �Q�[���̃t�F�[�Y���Z�b�g����B�����ɁAPHASE�񋓑̂��g���Ă���B
void GameManager::SetPhaseStatus(int value) {
	PhaseStatus = value;
}

// �I�[�o�[���[�h�ŁA���������v���C���[��RED��BLUE����ǉ����Ă���BBullet����Ă΂��B
void GameManager::SetPhaseStatus(int value, int hitPlayerNum) {
	PhaseStatus = value;

	// ���U���g��ʂ𐶐�����B
	// GameMain�I�u�W�F�N�g�̃��U���g�I�u�W�F�N�g�L���|�C���^�ɁA�����������U���g�̃|�C���^������
	gameMain->CreateResultObj(hitPlayerNum);
}

// �B��鑤�̎c�莞�Ԃ��Ǘ����Ă���֐��B
void GameManager::HideTimerControll(void) {
	// �B��鑤�c�莞�Ԃ����炷
	--t_HideTime;

	// �c�莞��0�ȉ��ɂȂ����猂���̃t�F�[�Y�Ɉڍs����B
	if (t_HideTime < 1) {
		ToShotPhase();
	}
}

// �����̎c�莞�Ԃ��Ǘ����Ă���֐��B
void GameManager::ShotTimerControll(void) {
	// �����c�莞�Ԃ����炷
	--t_ShotTime;

	//�c�莞��0�ȉ��ɂȂ�����B��鑤�̃t�F�[�Y�Ɉڍs����B
	if (t_ShotTime < 1) {
		ToHidePhase();
	}
}

// �B��鑤�̃t�F�[�Y�Ɉڍs���鏈�����܂Ƃ߂Ă���B
void GameManager::ToHidePhase(void) {
	SetHideTime();
	SetPhaseStatus(HIDE);
	ShooterChange();
	gameMain->ui->TransitionParameterReset();
}

// �����̃t�F�[�Y�Ɉڍs���鏈�����܂Ƃ߂Ă���B
void GameManager::ToShotPhase(void) {
	SetShotTime();
	SetPhaseStatus(SHOT);
	gameMain->ui->TransitionParameterReset();
}

GameManager::~GameManager() {
	
}