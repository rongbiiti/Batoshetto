#include "Network.h"
#include "GameMain.h"
////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
Network::Network(FontData* font, InputManager* input, GameManager* gameMNG) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;

	send = 1;			// ���M����f�[�^
	totalsend = 0;		// ���M����send�̍��v
	post = 0;			// ��M�����f�[�^
	totalpost = 0;		// ��M����post�̍��v

	recvSize = 0;		// ��M�����f�[�^�̃T�C�Y
	totalRecvSize = 0;	// ��M�f�[�^�̍��v�T�C�Y
	sendSize = 0;		// ���M�����f�[�^�̃T�C�Y
	totalSendSize = 0;	// ���M�f�[�^�̍��v�T�C�Y

	errorCode = 0;		// �G���[�R�[�h
	IPsNumber = 0;		// IP�A�h���X�̌�

	ConnectType = HOST;	// �z�X�g���Q�X�g��

	// UDP�n���h���̍쐬
	UDPNetHandle = MakeUDPSocket(PORT_NUMBER);
	VariableInit();
	InitIPAddress();
	
}

////////////////////////////////////////////////
// ���ʂ̕ϐ��̏�����
////////////////////////////////////////////////
void Network::VariableInit() {
	
	HOST_gestSerchWaitTime = 0;
	GEST_hostSerchWaitTime = 0;
	HOST_gestReplyWaitTime = 0;

	GEST_phaseNum = GEST_HOST_SEARCHING;
	HOST_phaseNum = HOST_GEST_WAITING;

	matchInfo_Send.num = 0;
	matchInfo_Send.difficulty = gameManager->GetDifficulty();
	matchInfo_Send.seed = 0;

	matchInfo_Post.num = 0;
	matchInfo_Post.difficulty = 0;
	matchInfo_Post.seed = 0;

	// ������IP�A�h���X������
	send_IP.d1 = 0;
	send_IP.d2 = 0;
	send_IP.d3 = 0;
	send_IP.d4 = 0;

	selectNum = 0;

	isWaitRecvCheck = FALSE;

	StructsReset();	// �\���̏�����
}

////////////////////////////////////////////////
// �����p����M�\���̏������I
////////////////////////////////////////////////
void Network::StructsReset() {
	shooterInfo_Send.angle = 0;
	shooterInfo_Send.shotFlg = FALSE;
	shooterInfo_Send.passFlg = FALSE;
	shooterInfo_Send.isRecvCheck = FALSE;

	hiderInfo_Send.x = 0;
	hiderInfo_Send.y = 0;
	hiderInfo_Send.angle = 0;
	hiderInfo_Send.passFlg = FALSE;
	hiderInfo_Send.isRecvCheck = FALSE;

	shooterInfo_Post.angle = 0;
	shooterInfo_Post.shotFlg = FALSE;
	shooterInfo_Post.passFlg = FALSE;
	shooterInfo_Post.isRecvCheck = FALSE;

	hiderInfo_Post.x = 0;
	hiderInfo_Post.y = 0;
	hiderInfo_Post.angle = 0;
	hiderInfo_Post.passFlg = FALSE;
	hiderInfo_Post.isRecvCheck = FALSE;

	recvCheck_Post = FALSE;

}

////////////////////////////////////////////////
// ��M�f�[�^���Z
////////////////////////////////////////////////
void Network::RecvDataAddition() {
	// ��M�f�[�^�T�C�Y�𑫂�
	if (recvSize >= 0) {
		totalRecvSize += recvSize;
		totalpost += matchInfo_Post.num;
	}
}

////////////////////////////////////////////////
// ���M�f�[�^���Z
////////////////////////////////////////////////
void Network::SendDataAddition() {
	// ���M�f�[�^�T�C�Y�𑫂�
	if (sendSize >= 0) {
		totalSendSize += sendSize;
		totalsend += matchInfo_Send.num;
	}
}

////////////////////////////////////////////////
// broadCast_IP��������
////////////////////////////////////////////////
void Network::InitIPAddress() {
	// �R���s���[�^�̑SIP�A�h���X���擾
	GetMyIPAddress(All_IP, ALL_IP_LENGTH, &IPsNumber);

	// ������IP�A�h���X������
	my_IP.d1 = 0;
	my_IP.d2 = 0;
	my_IP.d3 = 0;
	my_IP.d4 = 0;

	// ������IP�A�h���X������
	send_IP.d1 = 0;
	send_IP.d2 = 0;
	send_IP.d3 = 0;
	send_IP.d4 = 0;

	// �u���[�h�L���X�g�p��IP��������
	broadCast_IP.d1 = 0;
	broadCast_IP.d2 = 0;
	broadCast_IP.d3 = 0;
	broadCast_IP.d4 = 255;

	int i = 0;

	// �p�\�R���̃l�b�g���[�N�A�_�v�^��6�܂Ŏ擾���AAll_IP�z��ɂԂ�����
	// ��������IP�A�h���X���擾����iIPv4�̂݁j
	do {
		sendSize = NetWorkSendUDP(UDPNetHandle, All_IP[i++], PORT_NUMBER, &send, sizeof(send));
		recvSize = NetWorkRecvUDP(UDPNetHandle, &my_IP, NULL, &post, sizeof(post), FALSE);

		SendDataAddition();
		RecvDataAddition();
		
	}while (i < IPsNumber);

	// �����L����IP�A�h���X���ЂƂ��Ȃ�������G���[�R�[�h��1���Z�b�g���ď������I���
	if (my_IP.d1 == 0) {
		errorCode = 1;
		return;
	}

}

////////////////////////////////////////////////
// �g�p����IP�A�h���X��I��������
////////////////////////////////////////////////
void Network::IPAddressSelect() {
	bool dicideFlg = false;

	// �Q�[���p�b�h1�̕����p�b�h��ƃL�[�{�[�h�̕����L�[��̓���
	if (inputManager->GetButtonDown(PAD_UP, 0) ||
		inputManager->GetButtonHold(PAD_UP, 0, 4) ||
		inputManager->GetKeyDown(KEY_INPUT_UP) ||
		inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum < 0) {
			selectNum = IPsNumber - 1;
		}
	}

	// �Q�[���p�b�h1�̕����p�b�h���ƃL�[�{�[�h�̕����L�[���̓���
	if (inputManager->GetButtonDown(PAD_DOWN, 0) ||
		inputManager->GetButtonHold(PAD_DOWN, 0, 4) ||
		inputManager->GetKeyDown(KEY_INPUT_DOWN) ||
		inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum > IPsNumber - 1) {
			selectNum = 0;
		}
	}

	// �Q�[���p�b�h�ƃL�[�{�[�h�̌���{�^���̓���
	if (inputManager->GetButtonDown(B, 0) ||
		inputManager->GetKeyDown(KEY_INPUT_F) ||
		inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		dicideFlg = true;
	}

	// �����ꂩ�̌���{�^����������Ă�����u���[�h�L���X�gIP��ݒ肵�ăz�X�g���Q�X�g�ǂ���ɂȂ邩�I���������ʂ֑J��
	if (dicideFlg) {
		my_IP = All_IP[selectNum];
		broadCast_IP.d1 = my_IP.d1;
		broadCast_IP.d2 = my_IP.d2;
		broadCast_IP.d3 = my_IP.d3;
		gameManager->SetPhaseStatus(GameManager::CONNECT_TYPE_SELECT);
		selectNum = 0;
		return;
	}
}

////////////////////////////////////////////////
// �z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩��I��������
////////////////////////////////////////////////
void Network::CommunicationMethodSelect() {
	bool dicideFlg = false;
	// �Q�[���p�b�h1�̕����p�b�h��ƃL�[�{�[�h�̕����L�[��̓���
	if (inputManager->GetButtonDown(PAD_UP, 0) ||
		inputManager->GetButtonHold(PAD_UP, 0, 4) ||
		inputManager->GetKeyDown(KEY_INPUT_UP) ||
		inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum < 0) {
			selectNum = 1;
		}
	}

	// �Q�[���p�b�h1�̕����p�b�h���ƃL�[�{�[�h�̕����L�[���̓���
	if (inputManager->GetButtonDown(PAD_DOWN, 0) ||
		inputManager->GetButtonHold(PAD_DOWN, 0, 4) ||
		inputManager->GetKeyDown(KEY_INPUT_DOWN) ||
		inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum > 1) {
			selectNum = 0;
		}
	}

	// �Q�[���p�b�h�ƃL�[�{�[�h�̌���{�^���̓���
	if (inputManager->GetButtonDown(B, 0) ||
		inputManager->GetKeyDown(KEY_INPUT_F) ||
		inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		dicideFlg = true;
	}

	// �����ꂩ�̌���{�^����������Ă�����}�b�`���O��ʂ֑J��
	if (dicideFlg) {
		ConnectType = selectNum;

		// �z�X�g�Ƃ��ĒʐM���邱�Ƃ�I�����Ă����ꍇ�A�����̏����l�𐶐�����B�����̗����̏����l���A���̒l�ɐݒ肷��B
		SetSendRand();

		// ��M�p�\���̂�������
		matchInfo_Post.num = 0;
		matchInfo_Post.difficulty = 0;
		matchInfo_Post.seed = 0;

		// �ʐM�ҋ@�t�F�[�Y�ɑJ�ڂ���
		gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);

		// �o�b�t�@���N���A���Ă���
		BufferClear();
	}
}

////////////////////////////////////////////////
// �����𐶐�
////////////////////////////////////////////////
void Network::SetSendRand() {
	// �z�X�g�Ƃ��ĒʐM���邱�Ƃ�I�����Ă����ꍇ�A�����̏����l�𐶐�����B�����̗����̏����l���A���̒l�ɐݒ肷��B
	if (ConnectType == HOST) {
		randSeedNum = GetRand(10000);	// �����𐶐�
		SRand(randSeedNum);		// ���������l�ŗ��������l��ݒ�
		matchInfo_Send.seed = randSeedNum;	// ����ɑ���f�[�^�ɑ��
	}
}

////////////////////////////////////////////////
// �ʐM�ҋ@
////////////////////////////////////////////////
void Network::ConnectionWait() {
	// �z�X�g���Q�X�g���ŏ����𕪂���B
	// ���̊֐��́A�Q�[�����C������Ă΂�Ă���
	if (ConnectType == HOST) {
		ConnectionWait_TypeHOST();
	}
	else if (ConnectType == GEST) {
		ConnectionWait_TypeGEST();
	}
}

////////////////////////////////////////////////
// �ʐM�ҋ@�F�z�X�g
////////////////////////////////////////////////
void Network::ConnectionWait_TypeHOST() {

	//////////// �z�X�g���Q�X�g����̉����҂� ////////////
	if (HOST_phaseNum == HOST_GEST_WAITING) {
		++HOST_gestSerchWaitTime;	// �z�X�g�̑҂����ԃC���N�������g
		matchInfo_Post.num = 0;		// ��M�����l0�Ƀ��Z�b�g

		// ��M�B��M�����IP�A�h���X�𓯎��Ɏ擾���Ă���B
		recvSize = NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);
		RecvDataAddition();		// ����M�ʑ���

		// ��M�����f�[�^��num��1�Ȃ�A�Q�X�g���z�X�g��T���Ă��邱�Ƃ��킩��B�Q�X�g�����B
		// �Q�X�g�ɁA�����������Ƃ�`����t�F�[�Y�Ɉڍs
		if (matchInfo_Post.num == 1 && matchInfo_Post.difficulty == gameManager->GetDifficulty()) {
			HOST_phaseNum = HOST_GEST_REPTY_WAITING;
			matchInfo_Send.num = 2;
		}
	}

	//////////// �z�X�g���Q�X�g�𔭌��B�Q�X�g�ɔ����ł������Ƃ�`���A���̕ԐM�҂� ////////////
	else if (HOST_phaseNum == HOST_GEST_REPTY_WAITING) {
		// 0.5�b��1��A�Q�X�g�ɔ����������Ƃ�`����B
		if (++HOST_gestReplyWaitTime % 30 == 0) {
			matchInfo_Send.num = 2;	// ���Mnum��2������B2�́A�z�X�g���Q�X�g�����������Ƃ�`���Ă���B
			sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();
		}

		// ��M�B�Q�X�g����̕Ԏ���҂�
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);
		RecvDataAddition();		// ����M�ʑ���

		// �Q�X�g����̕Ԏ����Ԃ��Ă�����A�Q�[���J�n�I�I�I�I
		if (matchInfo_Post.num == 3 && sendSize >= 0) {
			// ���������t�F�[�Y�ɔ��
			gameManager->gameMain->Init();
		}
	}
}

////////////////////////////////////////////////
// �ʐM�ҋ@�F�Q�X�g
////////////////////////////////////////////////
void Network::ConnectionWait_TypeGEST() {

	//////////// �Q�X�g���z�X�g��T���Ă��� ////////////
	if (GEST_phaseNum == GEST_HOST_SEARCHING) {

		// 0.5�b��1��A�u���[�h�L���X�gIP���g���ăz�X�g�����Ȃ����M���𑗂�B
		if (++GEST_hostSerchWaitTime % 30 == 0) {
			matchInfo_Send.num = 1;		// 1�̒l�́A�Q�X�g���z�X�g��T���Ă���B

			// ���M�B�z�X�g��IP���킩��Ȃ��̂ŁA�u���[�h�L���X�gIP���g����LAN�̑S�p�\�R���ɑ��M
			sendSize = NetWorkSendUDP(UDPNetHandle, broadCast_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();		// �����M�ʑ����B
		}

		matchInfo_Post.num = 0;		// ��M�����l0�Ƀ��Z�b�g

		// ��M�B�z�X�g����̔����񍐑҂��B�z�X�g��IP���󂯎��
		recvSize = NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);
		RecvDataAddition();		// ����M�ʑ���

		// �z�X�g����̔����񍐂����Ă�����A���̕Ԏ�������t�F�[�Y�Ɉڍs�B
		if (matchInfo_Post.num == 2) {
			GEST_phaseNum = GEST_MATCH_START;
			GEST_hostSerchWaitTime = 0;
		}
	}

	//////////// �z�X�g���Q�X�g�𔭌��������Ƃ��킩�����B�Ԏ������� ////////////
	else if (GEST_phaseNum == GEST_MATCH_START) {

		// 0.5�b��1��A�z�X�g�ɔ����񍐂ւ̕Ԏ�������B
		if (++GEST_hostSerchWaitTime % 30 == 0) {
			matchInfo_Send.num = 3;		// ���M����l��3

			// ���M�B�z�X�g��IP���킩���Ă���̂ŁA����𒼐ڎw�肵�Ă���
			sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();		// �����M�ʑ���

			// ���M�T�C�Y��0�ȏ�Ȃ�A���������M�ł������Ƃɂ��āA�Q�[�����J�n����
			if (sendSize >= 0) {
				SRand(matchInfo_Post.seed);		// �z�X�g���瑗���Ă����l�ŗ��������l��ݒ�B
				gameManager->gameMain->Init();	// ���������t�F�[�Y�ɔ��
			}
		}
		
	}
}

////////////////////////////////////////////////
// �����̏��𑗐M����B�p�x�A���˂������ǂ����A�p�X�������ǂ����������ɓ����
////////////////////////////////////////////////
void Network::SendShooterInfo(float ang, bool isShot, bool isPass) {
	shooterInfo_Send.angle = ang;
	shooterInfo_Send.shotFlg = isShot;
	shooterInfo_Send.passFlg = isPass;

	// �������t���O�ƃp�X�����t���O�A�ǂ��炩��TRUE�Ȃ�A�ԐM���K�v�ȃf�[�^�Ƃ��đ��M����
	if (isShot || isPass) {
		shooterInfo_Send.isRecvCheck = TRUE;
		isWaitRecvCheck = TRUE;
	}
	else {
		shooterInfo_Send.isRecvCheck = FALSE;
		isWaitRecvCheck = FALSE;
	}

	// ���M
	sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &shooterInfo_Send, sizeof(shooterInfo_Send));
	SendDataAddition();		// �����M�ʑ���
}

////////////////////////////////////////////////
// �B��鑤�̏��𑗐M����BX���W�AY���W�A�p�X�������ǂ����������ɓ����
////////////////////////////////////////////////
void Network::SendHiderInfo(int px, int py, float angle, bool isPass) {
	hiderInfo_Send.x = px;
	hiderInfo_Send.y = py;
	hiderInfo_Send.angle = angle;
	hiderInfo_Send.passFlg = isPass;

	// �p�X�����t���O��TRUE�Ȃ�A�ԐM���K�v�ȃf�[�^�Ƃ��đ��M����
	if (isPass) {
		hiderInfo_Send.isRecvCheck = TRUE;
		isWaitRecvCheck = TRUE;
	}
	else {
		hiderInfo_Send.isRecvCheck = FALSE;
		isWaitRecvCheck = FALSE;
	}

	// ���M
	sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &hiderInfo_Send, sizeof(hiderInfo_Send));
	SendDataAddition();		// �����M�ʑ���
}

////////////////////////////////////////////////
// �ԐM�ł������Ƃ𑗐M����B
////////////////////////////////////////////////
void Network::SendRecvCheck() {
	bool recvCheck_Send = TRUE;
	// �������[�v
	while (1)
	{
		// TRUE�𑗂�
		sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &recvCheck_Send, sizeof(recvCheck_Send));
		if (sendSize >= 0) {
			// ���M�������Ă�����A���[�v�𔲂���
			SendDataAddition();
			break;
		}
		// �E�C���h�E�Y���b�Z�[�W����
		if (ProcessMessage() < 0) break;
	}
}

////////////////////////////////////////////////
// �����̏�����M����B�o�b�t�@��0�ɂȂ�����true���Ԃ�
////////////////////////////////////////////////
bool Network::PostShooterInfo() {
	// �������[�v
	while (1)
	{
		// ��M�o�b�t�@�����ׂČ���B
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &shooterInfo_Post, sizeof(shooterInfo_Post), FALSE);
		if (recvSize == -3) {
			// -3�́A��M�f�[�^�������Ӗ�����B-3�Ȃ�A���[�v�𔲂���B
			RecvDataAddition();		// ����M�ʑ���
			break;
		}
		else {
			RecvDataAddition();		// ����M�ʑ���
		}
		// �E�C���h�E�Y���b�Z�[�W����
		if (ProcessMessage() < 0) break;
	}
	return true;
}

////////////////////////////////////////////////
// �B��鑤�̏�����M����B�o�b�t�@��0�ɂȂ�����true���Ԃ�
////////////////////////////////////////////////
bool Network::PostHiderInfo() {
	// �������[�v
	while (1)
	{
		// ��M�o�b�t�@�����ׂČ���B
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &hiderInfo_Post, sizeof(hiderInfo_Post), FALSE);
		if (recvSize == -3) {
			// -3�́A��M�f�[�^�������Ӗ�����B-3�Ȃ�A���[�v�𔲂���B
			RecvDataAddition();		// ����M�ʑ���
			break;
		}
		else {
			RecvDataAddition();		// ����M�ʑ���
		}
		// �E�C���h�E�Y���b�Z�[�W����
		if (ProcessMessage() < 0) break;
	}
	return true;
}

////////////////////////////////////////////////
// �B��鑤�̏�����M����B�o�b�t�@��0�ɂȂ�����true���Ԃ�
////////////////////////////////////////////////
bool Network::PostRecvCheck() {
	// �������[�v
	while (1)
	{
		// ��M�o�b�t�@�����ׂČ���B
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &recvCheck_Post, sizeof(recvCheck_Post), FALSE);
		if (recvSize == -3) {
			// -3�́A��M�f�[�^�������Ӗ�����B-3�Ȃ�A���[�v�𔲂���B
			RecvDataAddition();		// ����M�ʑ���
			break;
		}
		else {
			RecvDataAddition();		// ����M�ʑ���
		}
		// �E�C���h�E�Y���b�Z�[�W����
		if (ProcessMessage() < 0) break;
	}
	return true;
}

////////////////////////////////////////////////
// �o�b�t�@�[�N���A
////////////////////////////////////////////////
void Network::BufferClear() {
	// �������[�v
	while (1)
	{
		// ��M�o�b�t�@�����ׂČ���B
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, NULL, NULL, FALSE);
		if (recvSize == -3) {
			// -3�́A��M�f�[�^�������Ӗ�����B-3�Ȃ�A���[�v�𔲂���B
			RecvDataAddition();		// ����M�ʑ���
			break;
		}
		else {
			RecvDataAddition();		// ����M�ʑ���
		}
		// �E�C���h�E�Y���b�Z�[�W����
		if (ProcessMessage() < 0) break;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////		�������牺�͕`�揈��			//////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////
// �z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩��I���������ʂ̕`��
////////////////////////////////////////////////
void Network::DrawCommunicationMethodSelect() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "�z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩");
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�I�����Ă�������");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xFFFFFF, fontData->f_FontData[1], "�I�����Ă�������");

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g�ɂȂ�");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 30, 0xFFFFFF, fontData->f_FontData[1], "�z�X�g�ɂȂ�");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y, 0xFFFFFF, fontData->f_FontData[1], "�Q�X�g�ɂȂ�");


	// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
	DrawRotaGraph(GameMain::SCREEN_WIDTH / 4, starty + y * selectNum, 1, 0, gameManager->gameMain->GetCursorImage(0), TRUE);
}

////////////////////////////////////////////////
// �g�p����IP�A�h���X��I���������ʂ�`��
////////////////////////////////////////////////
void Network::DrawIPAddressSelect() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "���g���ɂȂ�");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "���g���ɂȂ�");
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "IP�A�h���X��I�����Ă�������");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xFFFFFF, fontData->f_FontData[1], "IP�A�h���X��I�����Ă�������");
	// �e���ږ��`��
	for (int i = 0; i < IPsNumber; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d.%d.%d.%d", All_IP[i].d1, All_IP[i].d2, All_IP[i].d3, All_IP[i].d4);
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%d.%d.%d.%d", All_IP[i].d1, All_IP[i].d2, All_IP[i].d3, All_IP[i].d4);
	}
	// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
	DrawRotaGraph(GameMain::SCREEN_WIDTH / 4, starty + y * selectNum, 1, 0, gameManager->gameMain->GetCursorImage(0), TRUE);
}

////////////////////////////////////////////////
// �ʐM�ҋ@���̉��
////////////////////////////////////////////////
void Network::DrawConnectionWait() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;
	int min = 0, sec = 0;

	// �z�X�g////////////////////////////////////////////
	if (ConnectType == HOST) {
		if (HOST_phaseNum == HOST_GEST_WAITING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�Q�X�g��҂��Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "�Q�X�g��҂��Ă��܂�...");

			sec = HOST_gestSerchWaitTime / 60;
			min = sec / 60;
			if (60 <= sec) sec -= 60;

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�o�ߎ��ԁF%d:%02d", min, sec);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xFFFFFF, fontData->f_FontData[1], "�o�ߎ��ԁF%d:%02d", min, sec);

		}
		else if (HOST_phaseNum == HOST_GEST_REPTY_WAITING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�Q�X�g��������܂����B");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�Q�X�g��������܂����B");
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�ԓ���҂��Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "�ԓ���҂��Ă��܂�...");

			sec = HOST_gestReplyWaitTime / 60;
			min = sec / 60;
			if (60 <= sec) sec -= 60;

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�o�ߎ��ԁF%d:%02d", min, sec);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "�o�ߎ��ԁF%d:%02d", min, sec);
		}

	}


	// �Q�X�g/////////////////////////////////////////////
	else if (ConnectType == GEST) {
		if (GEST_phaseNum == GEST_HOST_SEARCHING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g��T���Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "�z�X�g��T���Ă��܂�...");

			sec = GEST_hostSerchWaitTime / 60;
			min = sec / 60;
			if (60 <= sec) sec -= 60;

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�o�ߎ��ԁF%d:%02d", min, sec);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xFFFFFF, fontData->f_FontData[1], "�o�ߎ��ԁF%d:%02d", min, sec);
		}
		else if (GEST_phaseNum == GEST_MATCH_START) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g��������܂����B");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�Q�X�g��������܂����B");
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�ΐ���J�n���܂��I");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "�ΐ���J�n���܂��I");
		}

	}

}

////////////////////////////////////////////////
// �l�b�g�n�̃f�[�^�\��
////////////////////////////////////////////////
void Network::DrawNetWorkData() {
	int handle = fontData->f_FontData[0];
	unsigned int c = 0xFFFFFF;
	DrawFormatStringToHandle(0, 40, c, handle, "send:%d", matchInfo_Send.num);
	DrawFormatStringToHandle(0, 60, c, handle, "totalSend:%d", totalsend);
	DrawFormatStringToHandle(0, 80, c, handle, "post:%d", matchInfo_Post.num);
	DrawFormatStringToHandle(0, 100, c, handle, "totalPost%d", totalpost);

	DrawFormatStringToHandle(0, 120, c, handle, "recvSize:%d", recvSize);
	DrawFormatStringToHandle(0, 140, c, handle, "totalRecvSize:%d", totalRecvSize);
	DrawFormatStringToHandle(0, 160, c, handle, "sendSize:%d", sendSize);
	DrawFormatStringToHandle(0, 180, c, handle, "totalSendSize%d", totalSendSize);

	DrawFormatStringToHandle(0, 200, 0xFABBBB, handle, "errorCode%d", errorCode);

	DrawFormatStringToHandle(0, 220, c, handle, "broadCast_IP:%d.%d.%d.%d", broadCast_IP.d1,broadCast_IP.d2,broadCast_IP.d3,broadCast_IP.d4);
	DrawFormatStringToHandle(0, 240, c, handle, "my_IP:%d.%d.%d.%d", my_IP.d1, my_IP.d2, my_IP.d3, my_IP.d4);
	DrawFormatStringToHandle(0, 260, c, handle, "send_IP:%d.%d.%d.%d", send_IP.d1, send_IP.d2, send_IP.d3, send_IP.d4);
	DrawFormatStringToHandle(0, 280, c, handle, "IpsNumber:%d", IPsNumber);
	DrawFormatStringToHandle(0, 300, c, handle, "matchInfo_Send.seed:%d", matchInfo_Send.seed);
	DrawFormatStringToHandle(0, 320, c, handle, "matchInfo_Post.seed:%d", matchInfo_Post.seed);
	DrawFormatStringToHandle(0, 340, c, handle, "matchInfo_Send.Diff:%d", matchInfo_Send.difficulty);
	DrawFormatStringToHandle(0, 360, c, handle, "matchInfo_Post.Diff:%d", matchInfo_Post.difficulty);
	DrawFormatStringToHandle(0, 380, c, handle, "shooterInfo_Post.angle:%.3f", shooterInfo_Post.angle);
	DrawFormatStringToHandle(0, 400, c, handle, "shooterInfo_Post.shotFlg:%d", shooterInfo_Post.shotFlg);
	DrawFormatStringToHandle(0, 420, c, handle, "shooterInfo_Post.passFlg:%d", shooterInfo_Post.passFlg);
	DrawFormatStringToHandle(0, 440, c, handle, "shooterInfo_Post.isRecvCheck:%d", shooterInfo_Post.isRecvCheck);
	DrawFormatStringToHandle(0, 460, c, handle, "hiderInfo_Post.x:%d", hiderInfo_Post.x);
	DrawFormatStringToHandle(0, 480, c, handle, "hiderInfo_Post.y:%d", hiderInfo_Post.y);
	DrawFormatStringToHandle(0, 500, c, handle, "hiderInfo_Post.angle:%d", hiderInfo_Post.angle);
	DrawFormatStringToHandle(0, 520, c, handle, "hiderInfo_Post.passFlg:%d", hiderInfo_Post.passFlg);
	DrawFormatStringToHandle(0, 540, c, handle, "hiderInfo_Post.isRecvCheck:%d", hiderInfo_Post.isRecvCheck);
}

////////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////////
Network::~Network() {
	DeleteUDPSocket(UDPNetHandle);
}