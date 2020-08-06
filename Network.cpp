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

	// UDP�n���h���̍쐬
	UDPNetHandle = MakeUDPSocket(PORT_NUMBER);
	VariableInit();
	InitIPAddress();
	
}

////////////////////////////////////////////////
// ���ʂ̕ϐ��̏�����
////////////////////////////////////////////////
void Network::VariableInit() {
	ConnectType = HOST;	// �z�X�g���Q�X�g��
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

	shooterInfo_Send.angle = 0;
	shooterInfo_Send.shotFlg = FALSE;
	shooterInfo_Send.passFlg = FALSE;
	shooterInfo_Send.isRecvCheck = FALSE;

	hiderInfo_Send.x = 0;
	hiderInfo_Send.y = 0;
	hiderInfo_Send.passFlg = FALSE;
	hiderInfo_Send.isRecvCheck = FALSE;

	shooterInfo_Post.angle = 0;
	shooterInfo_Post.shotFlg = FALSE;
	shooterInfo_Post.passFlg = FALSE;
	shooterInfo_Post.isRecvCheck = FALSE;

	hiderInfo_Post.x = 0;
	hiderInfo_Post.y = 0;
	hiderInfo_Post.passFlg = FALSE;
	hiderInfo_Post.isRecvCheck = FALSE;
}

////////////////////////////////////////////////
// ��M�f�[�^���Z
////////////////////////////////////////////////
void Network::RecvDataAddition() {
	if (recvSize >= 0) {
		totalRecvSize += recvSize;
		totalpost += matchInfo_Post.num;
	}
}

////////////////////////////////////////////////
// ���M�f�[�^���Z
////////////////////////////////////////////////
void Network::SendDataAddition() {
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
		if (selectNum == HOST) {
			randSeedNum = GetRand(10000);
			SRand(randSeedNum);
			matchInfo_Send.seed = randSeedNum;
		}
		ConnectType = selectNum;
		matchInfo_Post.num = 0;
		matchInfo_Post.difficulty = 0;
		matchInfo_Post.seed = 0;
		gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);
		selectNum = 0;
		NetWorkRecvBufferClear(UDPNetHandle);
		while (NetWorkRecvUDP(UDPNetHandle, NULL, NULL, NULL, NULL, FALSE) >= 0)
		{

		}
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
	if (HOST_phaseNum == HOST_GEST_WAITING) {
		++HOST_gestSerchWaitTime;
		matchInfo_Post.num = 0;
		recvSize = NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);

		RecvDataAddition();
		if (matchInfo_Post.num == 1 && matchInfo_Post.difficulty == gameManager->GetDifficulty()) {
			HOST_phaseNum = HOST_GEST_REPTY_WAITING;
			matchInfo_Send.num = 2;
		}
	}
	else if (HOST_phaseNum == HOST_GEST_REPTY_WAITING) {
		++HOST_gestReplyWaitTime;
		if (HOST_gestReplyWaitTime % 30 == 0) {
			matchInfo_Send.num = 2;
			sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();
		}
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);
		RecvDataAddition();
		if (matchInfo_Post.num == 3 && sendSize >= 0) {
			gameManager->gameMain->Init();
		}
	}
}

////////////////////////////////////////////////
// �ʐM�ҋ@�F�Q�X�g
////////////////////////////////////////////////
void Network::ConnectionWait_TypeGEST() {
	if (GEST_phaseNum == GEST_HOST_SEARCHING) {
		++GEST_hostSerchWaitTime;
		matchInfo_Post.num = 0;
		recvSize = NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);
		RecvDataAddition();
		if (GEST_hostSerchWaitTime % 30 == 0) {
			matchInfo_Send.num = 1;
			sendSize = NetWorkSendUDP(UDPNetHandle, broadCast_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();
		}
		if (matchInfo_Post.num == 2) {
			GEST_phaseNum = GEST_MATCH_START;
			GEST_hostSerchWaitTime = 0;
		}
	}
	else if (GEST_phaseNum == GEST_MATCH_START) {
		++GEST_hostSerchWaitTime;
		if (GEST_hostSerchWaitTime % 30 == 0) {
			matchInfo_Send.num = 3;
			sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();
			if (sendSize >= 0) {
				SRand(matchInfo_Post.seed);
				gameManager->gameMain->Init();
			}
		}
		
	}
}

////////////////////////////////////////////////
// �ʐM�ҋ@���̉��
////////////////////////////////////////////////
void Network::DrawConnectionWait() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	// �z�X�g////////////////////////////////////////////
	if (ConnectType == HOST) {
		if (HOST_phaseNum == HOST_GEST_WAITING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�Q�X�g��҂��Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�Q�X�g��҂��Ă��܂�...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d�b�o��", HOST_gestSerchWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d�b�o��", HOST_gestSerchWaitTime / 60);

		}
		else if (HOST_phaseNum == HOST_GEST_REPTY_WAITING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�Q�X�g��������܂����B");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�Q�X�g��������܂����B");
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "������҂��Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "������҂��Ă��܂�...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d�b�o��", HOST_gestReplyWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d�b�o��", HOST_gestReplyWaitTime / 60);
		}
		
	}


	// �Q�X�g/////////////////////////////////////////////
	else if (ConnectType == GEST) {
		if (GEST_phaseNum == GEST_HOST_SEARCHING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g��T���Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�z�X�g��T���Ă��܂�...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d�b�o��", GEST_hostSerchWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d�b�o��", GEST_hostSerchWaitTime / 60);
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
// �z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩��I���������ʂ̕`��
////////////////////////////////////////////////
void Network::DrawCommunicationMethodSelect() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩");
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�I�����Ă�������");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "�I�����Ă�������");

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g�ɂȂ�");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 30, 0xeeff14, fontData->f_FontData[1], "�z�X�g�ɂȂ�");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y, 0xeeff14, fontData->f_FontData[1], "�Q�X�g�ɂȂ�");


	// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
	DrawCircle(GameMain::SCREEN_WIDTH / 4, starty + y * selectNum, 10, COLOR_VALUE_PLAYER[0], 1, 1);
}

////////////////////////////////////////////////
// �g�p����IP�A�h���X��I���������ʂ�`��
////////////////////////////////////////////////
void Network::DrawIPAddressSelect() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "���g���ɂȂ�");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "���g���ɂȂ�");
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "IP�A�h���X��I�����Ă�������");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "IP�A�h���X��I�����Ă�������");
	// �e���ږ��`��
	for (int i = 0; i < IPsNumber; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d.%d.%d.%d", All_IP[i].d1, All_IP[i].d2, All_IP[i].d3, All_IP[i].d4);
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xeeff14, fontData->f_FontData[1], "%d.%d.%d.%d", All_IP[i].d1, All_IP[i].d2, All_IP[i].d3, All_IP[i].d4);
	}
	// �v���C���[�̑I�𒆂̃J�[�\���ʒu�Ƀv���C���[�F�̊ۂ�`��
	DrawCircle(GameMain::SCREEN_WIDTH / 4, starty + y * selectNum, 10, COLOR_VALUE_PLAYER[0], 1, 1);
}

////////////////////////////////////////////////
// �����̏��𑗐M����B�p�x�A���˂������ǂ����A�p�X�������ǂ����������ɓ����
////////////////////////////////////////////////
void Network::SendShooterInfo(float ang, bool isShot, bool isPass) {
	NetWorkRecvBufferClear(UDPNetHandle);

	shooterInfo_Send.angle = ang;
	shooterInfo_Send.shotFlg = isShot;
	shooterInfo_Send.passFlg = isPass;
	if (isShot || isPass) {
		shooterInfo_Send.isRecvCheck = TRUE;
	}
	else {
		shooterInfo_Send.isRecvCheck = FALSE;
	}
	sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &shooterInfo_Send, sizeof(shooterInfo_Send));
	SendDataAddition();
}

////////////////////////////////////////////////
// �B��鑤�̏��𑗐M����BX���W�AY���W�A�p�X�������ǂ����������ɓ����
////////////////////////////////////////////////
void Network::SendHiderInfo(int px, int py, bool isPass) {
	NetWorkRecvBufferClear(UDPNetHandle);
	hiderInfo_Send.x = px;
	hiderInfo_Send.y = py;
	hiderInfo_Send.passFlg = isPass;
	if (isPass) {
		hiderInfo_Send.isRecvCheck = TRUE;
	}
	else {
		hiderInfo_Send.isRecvCheck = FALSE;
	}
	sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &hiderInfo_Send, sizeof(hiderInfo_Send));
	SendDataAddition();
}

////////////////////////////////////////////////
// �����̏�����M����B�o�b�t�@��0�ɂȂ�����true���Ԃ�
////////////////////////////////////////////////
bool Network::PostShooterInfo() {
	NetWorkRecvBufferClear(UDPNetHandle);
	while (1)
	{
		if (recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &shooterInfo_Post, sizeof(shooterInfo_Post), FALSE) < 0) {
			RecvDataAddition();
			break;
		}
		else {
			RecvDataAddition();
		}
	}
	return true;
}

////////////////////////////////////////////////
// �B��鑤�̏�����M����B�o�b�t�@��0�ɂȂ�����true���Ԃ�
////////////////////////////////////////////////
bool Network::PostHiderInfo() {
	NetWorkRecvBufferClear(UDPNetHandle);
	while (1)
	{
		if (recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &hiderInfo_Post, sizeof(hiderInfo_Post), FALSE) < 0) {
			RecvDataAddition();
			break;
		}
		else {
			RecvDataAddition();
		}
	}
	return true;
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
	DrawFormatStringToHandle(0, 380, c, handle, "shooterInfo_Post.angle:%d", shooterInfo_Send.angle);
	DrawFormatStringToHandle(0, 400, c, handle, "shooterInfo_Post.shotFlg:%d", shooterInfo_Send.shotFlg);
	DrawFormatStringToHandle(0, 420, c, handle, "shooterInfo_Post.passFlg:%d", shooterInfo_Send.passFlg);
	DrawFormatStringToHandle(0, 440, c, handle, "shooterInfo_Post.isRecvCheck:%d", shooterInfo_Send.isRecvCheck);
	DrawFormatStringToHandle(0, 460, c, handle, "hiderInfo_Post.x:%d", hiderInfo_Post.x);
	DrawFormatStringToHandle(0, 480, c, handle, "hiderInfo_Post.y:%d", hiderInfo_Post.y);
	DrawFormatStringToHandle(0, 500, c, handle, "hiderInfo_Post.passFlg:%d", hiderInfo_Post.passFlg);
	DrawFormatStringToHandle(0, 520, c, handle, "hiderInfo_Post.isRecvCheck:%d", hiderInfo_Post.isRecvCheck);
}

////////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////////
Network::~Network() {
	DeleteUDPSocket(UDPNetHandle);
}