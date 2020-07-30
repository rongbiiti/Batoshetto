#include "Network.h"
#include "GameMain.h"

// �R���X�g���N�^
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

	InitIPAddress();
	VariableInit();
}

// ���ʂ̕ϐ��̏�����
void Network::VariableInit() {
	ConnectType = 0;	// �z�X�g���Q�X�g��
	HOST_gestSerchWaitTime = 0;
	GEST_hostSerchWaitTime = 0;
	HOST_gestReplyWaitTime = 0;

	selectNum = 0;
}

// broadCast_IP��������
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

	do {
		sendSize = NetWorkSendUDP(UDPNetHandle, All_IP[i++], PORT_NUMBER, &send, sizeof(send));
		recvSize = NetWorkRecvUDP(UDPNetHandle, &my_IP, NULL, &post, sizeof(post), FALSE);

		totalSendSize += sendSize;
		totalsend += send;

		if (recvSize > 0) {
			totalRecvSize += recvSize;
			totalpost += post;
		}
	}while (i < IPsNumber);

	// �����L����IP�A�h���X���ЂƂ��Ȃ�������G���[�R�[�h��1���Z�b�g���ď������I���
	if (my_IP.d1 == 0) {
		errorCode = 1;
		return;
	}

}

// �g�p����IP�A�h���X��I��������
void Network::IPAddressSelect() {
	if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] == 1 || inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum < 0) {
			selectNum = IPsNumber - 1;
		}
		if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] >= 18) {
			inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] -= 4;
		}
	}

	if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] == 1 || inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum > IPsNumber - 1) {
			selectNum = 0;
		}
		if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] >= 18) {
			inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] -= 4;
		}
	}

	if (inputManager->GetPadInput()[0].in_Button[InputManager::B] == 1) {
		my_IP = All_IP[selectNum];
		broadCast_IP.d1 = my_IP.d1;
		broadCast_IP.d2 = my_IP.d2;
		broadCast_IP.d3 = my_IP.d3;
		gameManager->SetPhaseStatus(GameManager::CONNECT_TYPE_SELECT);
		selectNum = 0;
		return;
	}

	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->In_Key()[KEY_INPUT_UP] == 1 || inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum < 0) {
			selectNum = IPsNumber - 1;
		}
		if (inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
			inputManager->In_Key()[KEY_INPUT_UP] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_DOWN] == 1 || inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum > IPsNumber - 1) {
			selectNum = 0;
		}
		if (inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
			inputManager->In_Key()[KEY_INPUT_DOWN] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_F] == 1 || inputManager->In_Key()[KEY_INPUT_RETURN] == 1) {
		my_IP = All_IP[selectNum];
		broadCast_IP.d1 = my_IP.d1;
		broadCast_IP.d2 = my_IP.d2;
		broadCast_IP.d3 = my_IP.d3;
		gameManager->SetPhaseStatus(GameManager::CONNECT_TYPE_SELECT);
		selectNum = 0;
	}
}

// �z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩��I��������
void Network::CommunicationMethodSelect() {
	if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] == 1 || inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum < 0) {
			selectNum = 1;
		}
		if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] >= 18) {
			inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] -= 4;
		}
	}

	if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] == 1 || inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum > 1) {
			selectNum = 0;
		}
		if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] >= 18) {
			inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] -= 4;
		}
	}

	if (inputManager->GetPadInput()[0].in_Button[InputManager::B] == 1) {
		ConnectType = selectNum;
		gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);
		selectNum = 0;
		return;
	}

	// �L�[�{�[�h����̓��́B2�v���C���[�̃J�[�\���𑀍삷��B
	if (inputManager->In_Key()[KEY_INPUT_UP] == 1 || inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h��̓��́B18�t���ȏ㉟�������Ă���A���Ńf�N�������g
		// 0�����ɂȂ����獀�ڍő吔�̐����ɂ���i�J�[�\����Ɉړ��A��ԏ�̂Ƃ��ɏ���������烁�j���[�̈�ԉ��ɃJ�[�\�������킹��j
		if (--selectNum < 0) {
			selectNum = 1;
		}
		if (inputManager->In_Key()[KEY_INPUT_UP] >= 18) {
			inputManager->In_Key()[KEY_INPUT_UP] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_DOWN] == 1 || inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
		// �Q�[���p�b�h1�̕����p�b�h���̓��́B18�t���ȏ㉟�������Ă���A���ŃC���N�������g
		// ���ڍő吔�̐������傫���Ȃ�����0�ɖ߂��i�J�[�\�����Ɉړ��A��ԉ��̂Ƃ��ɉ����������烁�j���[�̈�ԏ�ɃJ�[�\�������킹��j
		if (++selectNum > 1) {
			selectNum = 0;
		}
		if (inputManager->In_Key()[KEY_INPUT_DOWN] >= 18) {
			inputManager->In_Key()[KEY_INPUT_DOWN] -= 4;
		}
	}

	if (inputManager->In_Key()[KEY_INPUT_F] == 1 || inputManager->In_Key()[KEY_INPUT_RETURN] == 1) {
		ConnectType = selectNum;
		gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);
		selectNum = 0;
	}
}

// �ʐM�ҋ@
void Network::ConnectionWait() {
	if (ConnectType == 0) {
		ConnectionWait_TypeHOST();
	}
	else {
		ConnectionWait_TypeGEST();
	}
}

// �ʐM�ҋ@�F�z�X�g
void Network::ConnectionWait_TypeHOST() {
	if (HOST_phaseNum == 0) {
		++HOST_gestSerchWaitTime;
		post = 0;
		recvSize = NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &post, sizeof(post), FALSE);
		if (post == 1) {
			HOST_phaseNum = 1;
			send = 2;
		}
	}
	else if (HOST_phaseNum == 1) {
		++HOST_gestReplyWaitTime;
		if (HOST_gestReplyWaitTime % 60 == 0) {
			sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &send, sizeof(send));
		}
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &post, sizeof(post), FALSE);
		if (post == 3 && sendSize >= 0) {
			gameManager->SetPhaseStatus(GameManager::INIT);
		}
	}
}

// �ʐM�ҋ@�F�Q�X�g
void Network::ConnectionWait_TypeGEST() {
	if (GEST_phaseNum == 0) {
		++GEST_hostSerchWaitTime;
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &post, sizeof(post), FALSE);
		if (GEST_hostSerchWaitTime % 60 == 0) {
			send = 1;
			sendSize = NetWorkSendUDP(UDPNetHandle, broadCast_IP, PORT_NUMBER, &send, sizeof(send));
		}
		if (post == 1) {

		}
	}
	else if (GEST_phaseNum == 1) {

	}
}

// �ʐM�ҋ@���̉��
void Network::DrawConnectionWait() {
	// �����̕��A			��ʂ̉����S�A�@�@�@�@�@�@�@Y���̑����ʁA�@����Y�̈ʒu
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	// �z�X�g////////////////////////////////////////////
	if (ConnectType == 0) {
		if (HOST_phaseNum == 0) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�Q�X�g��҂��Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�Q�X�g��҂��Ă��܂�...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d�b�o��", HOST_gestSerchWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d�b�o��", HOST_gestSerchWaitTime / 60);

		}
		else if (HOST_phaseNum == 1) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�Q�X�g��������܂����B");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�Q�X�g��������܂����B");
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "������҂��Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "������҂��Ă��܂�...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d�b�o��", HOST_gestReplyWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d�b�o��", HOST_gestReplyWaitTime / 60);
		}
		
	}


	// �Q�X�g/////////////////////////////////////////////
	else {
		if (GEST_phaseNum == 0) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g��T���Ă��܂�...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�z�X�g��T���Ă��܂�...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d�b�o��", GEST_hostSerchWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d�b�o��", GEST_hostSerchWaitTime / 60);
		}
		else if (GEST_phaseNum == 1) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�z�X�g��������܂����B");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "�Q�X�g��������܂����B");
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "�ΐ���J�n���܂��I");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "�ΐ���J�n���܂��I");
		}
		
	}
	
}

// �z�X�g�ɂȂ邩�Q�X�g�ɂȂ邩��I���������ʂ̕`��
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

// �g�p����IP�A�h���X��I���������ʂ�`��
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

// �l�b�g�n�̃f�[�^�\��
void Network::DrawNetWorkData() {
	int handle = fontData->f_FontData[0];
	unsigned int c = 0xFFFFFF;
	DrawFormatStringToHandle(0, 40, c, handle, "send:%d", send);
	DrawFormatStringToHandle(0, 60, c, handle, "totalSend:%d", totalsend);
	DrawFormatStringToHandle(0, 80, c, handle, "post:%d", post);
	DrawFormatStringToHandle(0, 100, c, handle, "totalPost%d", totalpost);

	DrawFormatStringToHandle(0, 120, c, handle, "recvSize:%d", recvSize);
	DrawFormatStringToHandle(0, 140, c, handle, "totalRecvSize:%d", totalRecvSize);
	DrawFormatStringToHandle(0, 160, c, handle, "sendSize:%d", sendSize);
	DrawFormatStringToHandle(0, 180, c, handle, "totalSendSize%d", totalSendSize);

	DrawFormatStringToHandle(0, 200, 0xFABBBB, handle, "errorCode%d", errorCode);

	DrawFormatStringToHandle(0, 220, c, handle, "broadCast_IP:%d.%d.%d.%d", broadCast_IP.d1,broadCast_IP.d2,broadCast_IP.d3,broadCast_IP.d4);
	DrawFormatStringToHandle(0, 240, c, handle, "my_IP:%d.%d.%d.%d", my_IP.d1, my_IP.d2, my_IP.d3, my_IP.d4);
	DrawFormatStringToHandle(0, 260, c, handle, "send_IP:%d.%d.%d.%d", send_IP.d1, send_IP.d2, send_IP.d3, send_IP.d4);
	DrawFormatStringToHandle(0, 280, c, handle, "IpsNumber%d", IPsNumber);
}

// �f�X�g���N�^
Network::~Network() {

}