#include "Network.h"
#include "GameMain.h"
////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
Network::Network(FontData* font, InputManager* input, GameManager* gameMNG) {
	fontData = font;
	inputManager = input;
	gameManager = gameMNG;

	send = 1;			// 送信するデータ
	totalsend = 0;		// 送信したsendの合計
	post = 0;			// 受信したデータ
	totalpost = 0;		// 受信したpostの合計

	recvSize = 0;		// 受信したデータのサイズ
	totalRecvSize = 0;	// 受信データの合計サイズ
	sendSize = 0;		// 送信したデータのサイズ
	totalSendSize = 0;	// 送信データの合計サイズ

	errorCode = 0;		// エラーコード
	IPsNumber = 0;		// IPアドレスの個数

	// UDPハンドルの作成
	UDPNetHandle = MakeUDPSocket(PORT_NUMBER);

	InitIPAddress();
	VariableInit();
}

////////////////////////////////////////////////
// 普通の変数の初期化
////////////////////////////////////////////////
void Network::VariableInit() {
	ConnectType = 0;	// ホストかゲストか
	HOST_gestSerchWaitTime = 0;
	GEST_hostSerchWaitTime = 0;
	HOST_gestReplyWaitTime = 0;

	GEST_phaseNum = 0;
	HOST_phaseNum = 0;

	matchInfo_Send.num = 0;
	matchInfo_Send.difficulty = gameManager->GetDifficulty();
	matchInfo_Send.seed = 0;

	matchInfo_Post.num = 0;
	matchInfo_Post.difficulty = 0;
	matchInfo_Post.seed = 0;

	// 送り先のIPアドレス初期化
	send_IP.d1 = 0;
	send_IP.d2 = 0;
	send_IP.d3 = 0;
	send_IP.d4 = 0;

	selectNum = 0;
}

////////////////////////////////////////////////
// 受信データ加算
////////////////////////////////////////////////
void Network::RecvDataAddition() {
	if (recvSize >= 0) {
		totalRecvSize += recvSize;
		totalpost += matchInfo_Post.num;
	}
}

////////////////////////////////////////////////
// 送信データ加算
////////////////////////////////////////////////
void Network::SendDataAddition() {
	if (sendSize >= 0) {
		totalSendSize += sendSize;
		totalsend += matchInfo_Send.num;
	}
}

////////////////////////////////////////////////
// broadCast_IPを初期化
////////////////////////////////////////////////
void Network::InitIPAddress() {
	// コンピュータの全IPアドレスを取得
	GetMyIPAddress(All_IP, ALL_IP_LENGTH, &IPsNumber);

	// 自分のIPアドレス初期化
	my_IP.d1 = 0;
	my_IP.d2 = 0;
	my_IP.d3 = 0;
	my_IP.d4 = 0;

	// 送り先のIPアドレス初期化
	send_IP.d1 = 0;
	send_IP.d2 = 0;
	send_IP.d3 = 0;
	send_IP.d4 = 0;

	// ブロードキャスト用のIPを初期化
	broadCast_IP.d1 = 0;
	broadCast_IP.d2 = 0;
	broadCast_IP.d3 = 0;
	broadCast_IP.d4 = 255;

	int i = 0;

	do {
		sendSize = NetWorkSendUDP(UDPNetHandle, All_IP[i++], PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
		recvSize = NetWorkRecvUDP(UDPNetHandle, &my_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);

		SendDataAddition();
		RecvDataAddition();
		
	}while (i < IPsNumber);

	// もし有効なIPアドレスがひとつもなかったらエラーコードに1をセットして処理を終わる
	if (my_IP.d1 == 0) {
		errorCode = 1;
		return;
	}

}

////////////////////////////////////////////////
// 使用するIPアドレスを選択させる
////////////////////////////////////////////////
void Network::IPAddressSelect() {
	if (inputManager->GetButtonDown(PAD_UP, 0) || inputManager->GetButtonHold(PAD_UP, 0, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum < 0) {
			selectNum = IPsNumber - 1;
		}
	}

	if (inputManager->GetButtonDown(PAD_DOWN, 0) || inputManager->GetButtonHold(PAD_DOWN, 0, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum > IPsNumber - 1) {
			selectNum = 0;
		}
	}

	if (inputManager->GetButtonDown(B, 0)) {
		my_IP = All_IP[selectNum];
		broadCast_IP.d1 = my_IP.d1;
		broadCast_IP.d2 = my_IP.d2;
		broadCast_IP.d3 = my_IP.d3;
		gameManager->SetPhaseStatus(GameManager::CONNECT_TYPE_SELECT);
		selectNum = 0;
		return;
	}

	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum < 0) {
			selectNum = IPsNumber - 1;
		}
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum > IPsNumber - 1) {
			selectNum = 0;
		}
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN) == 1) {
		my_IP = All_IP[selectNum];
		broadCast_IP.d1 = my_IP.d1;
		broadCast_IP.d2 = my_IP.d2;
		broadCast_IP.d3 = my_IP.d3;
		gameManager->SetPhaseStatus(GameManager::CONNECT_TYPE_SELECT);
		selectNum = 0;
	}
}

////////////////////////////////////////////////
// ホストになるかゲストになるかを選択させる
////////////////////////////////////////////////
void Network::CommunicationMethodSelect() {
	if (inputManager->GetButtonDown(PAD_UP, 0) || inputManager->GetButtonHold(PAD_UP, 0, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum < 0) {
			selectNum = 1;
		}
	}

	if (inputManager->GetButtonDown(PAD_DOWN, 0) || inputManager->GetButtonHold(PAD_DOWN, 0, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum > 1) {
			selectNum = 0;
		}
	}

	if (inputManager->GetButtonDown(B, 0)) {
		if (selectNum == HOST) {
			randSeedNum = GetRand(10000);
			SRand(randSeedNum);
			matchInfo_Send.seed = randSeedNum;
			gameManager->SetRandSeedNum(randSeedNum);
		}
		ConnectType = selectNum;
		matchInfo_Post.num = 0;
		matchInfo_Post.difficulty = 0;
		matchInfo_Post.seed = 0;
		gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);
		selectNum = 0;
		NetWorkRecvBufferClear(UDPNetHandle);
		while (NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE) >= 0)
		{

		}
	}

	// キーボードからの入力。2プレイヤーのカーソルを操作する。
	if (inputManager->GetKeyDown(KEY_INPUT_UP) || inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum < 0) {
			selectNum = 1;
		}
	}

	if (inputManager->GetKeyDown(KEY_INPUT_DOWN) || inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum > 1) {
			selectNum = 0;
		}
	}

	if (inputManager->GetKeyDown(KEY_INPUT_F) || inputManager->GetKeyDown(KEY_INPUT_RETURN) == 1) {
		if (selectNum == HOST) {
			randSeedNum = GetRand(10000);
			SRand(randSeedNum);
			matchInfo_Send.seed = randSeedNum;
			gameManager->SetRandSeedNum(randSeedNum);
		}
		ConnectType = selectNum;
		matchInfo_Post.num = 0;
		matchInfo_Post.difficulty = 0;
		matchInfo_Post.seed = 0;
		gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);
		NetWorkRecvBufferClear(UDPNetHandle);
		selectNum = 0;
		while (NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE) >= 0)
		{

		}
	}
}

////////////////////////////////////////////////
// 通信待機
////////////////////////////////////////////////
void Network::ConnectionWait() {
	if (ConnectType == HOST) {
		ConnectionWait_TypeHOST();
	}
	else if (ConnectType == GEST) {
		ConnectionWait_TypeGEST();
	}
}

////////////////////////////////////////////////
// 通信待機：ホスト
////////////////////////////////////////////////
void Network::ConnectionWait_TypeHOST() {
	if (HOST_phaseNum == 0) {
		++HOST_gestSerchWaitTime;
		matchInfo_Post.num = 0;
		recvSize = NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);

		RecvDataAddition();
		if (matchInfo_Post.num == 1 && matchInfo_Post.difficulty == gameManager->GetDifficulty()) {
			HOST_phaseNum = 1;
			matchInfo_Send.num = 2;
		}
	}
	else if (HOST_phaseNum == 1) {
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
// 通信待機：ゲスト
////////////////////////////////////////////////
void Network::ConnectionWait_TypeGEST() {
	if (GEST_phaseNum == 0) {
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
			GEST_phaseNum = 1;
			GEST_hostSerchWaitTime = 0;
		}
	}
	else if (GEST_phaseNum == 1) {
		++GEST_hostSerchWaitTime;
		if (GEST_hostSerchWaitTime % 30 == 0) {
			matchInfo_Send.num = 3;
			sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();
			if (sendSize >= 0) {
				gameManager->SetRandSeedNum(matchInfo_Post.seed);
				SRand(matchInfo_Post.seed);
				gameManager->gameMain->Init();
			}
		}
		
	}
}

////////////////////////////////////////////////
// 通信待機中の画面
////////////////////////////////////////////////
void Network::DrawConnectionWait() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	// ホスト////////////////////////////////////////////
	if (ConnectType == HOST) {
		if (HOST_phaseNum == 0) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ゲストを待っています...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "ゲストを待っています...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d秒経過", HOST_gestSerchWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d秒経過", HOST_gestSerchWaitTime / 60);

		}
		else if (HOST_phaseNum == 1) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ゲストが見つかりました。");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "ゲストが見つかりました。");
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "応答を待っています...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "応答を待っています...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d秒経過", HOST_gestReplyWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d秒経過", HOST_gestReplyWaitTime / 60);
		}
		
	}


	// ゲスト/////////////////////////////////////////////
	else if (ConnectType == GEST) {
		if (GEST_phaseNum == 0) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ホストを探しています...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "ホストを探しています...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d秒経過", GEST_hostSerchWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d秒経過", GEST_hostSerchWaitTime / 60);
		}
		else if (GEST_phaseNum == 1) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ホストが見つかりました。");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "ゲストが見つかりました。");
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "対戦を開始します！");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "対戦を開始します！");
		}
		
	}
	
}

////////////////////////////////////////////////
// ホストになるかゲストになるかを選択させる画面の描画
////////////////////////////////////////////////
void Network::DrawCommunicationMethodSelect() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ホストになるかゲストになるか");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "ホストになるかゲストになるか");
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "選択してください");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "選択してください");

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ホストになる");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 30, 0xeeff14, fontData->f_FontData[1], "ホストになる");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y, 0xeeff14, fontData->f_FontData[1], "ゲストになる");


	// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
	DrawCircle(GameMain::SCREEN_WIDTH / 4, starty + y * selectNum, 10, COLOR_VALUE_PLAYER[0], 1, 1);
}

////////////////////////////////////////////////
// 使用するIPアドレスを選択させる画面を描画
////////////////////////////////////////////////
void Network::DrawIPAddressSelect() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "お使いになる");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "お使いになる");
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "IPアドレスを選択してください");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "IPアドレスを選択してください");
	// 各項目名描画
	for (int i = 0; i < IPsNumber; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d.%d.%d.%d", All_IP[i].d1, All_IP[i].d2, All_IP[i].d3, All_IP[i].d4);
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xeeff14, fontData->f_FontData[1], "%d.%d.%d.%d", All_IP[i].d1, All_IP[i].d2, All_IP[i].d3, All_IP[i].d4);
	}
	// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
	DrawCircle(GameMain::SCREEN_WIDTH / 4, starty + y * selectNum, 10, COLOR_VALUE_PLAYER[0], 1, 1);
}

////////////////////////////////////////////////
// ネット系のデータ表示
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
}

////////////////////////////////////////////////
// デストラクタ
////////////////////////////////////////////////
Network::~Network() {
	DeleteUDPSocket(UDPNetHandle);
}