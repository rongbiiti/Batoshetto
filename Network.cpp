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
	VariableInit();
	InitIPAddress();
	
}

////////////////////////////////////////////////
// 普通の変数の初期化
////////////////////////////////////////////////
void Network::VariableInit() {
	ConnectType = HOST;	// ホストかゲストか
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

	// 送り先のIPアドレス初期化
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

	// パソコンのネットワークアダプタを6つまで取得し、All_IP配列にぶちこむ
	// そうしてIPアドレスを取得する（IPv4のみ）
	do {
		sendSize = NetWorkSendUDP(UDPNetHandle, All_IP[i++], PORT_NUMBER, &send, sizeof(send));
		recvSize = NetWorkRecvUDP(UDPNetHandle, &my_IP, NULL, &post, sizeof(post), FALSE);

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
	bool dicideFlg = false;

	// ゲームパッド1の方向パッド上とキーボードの方向キー上の入力
	if (inputManager->GetButtonDown(PAD_UP, 0) ||
		inputManager->GetButtonHold(PAD_UP, 0, 4) ||
		inputManager->GetKeyDown(KEY_INPUT_UP) ||
		inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum < 0) {
			selectNum = IPsNumber - 1;
		}
	}

	// ゲームパッド1の方向パッド下とキーボードの方向キー下の入力
	if (inputManager->GetButtonDown(PAD_DOWN, 0) ||
		inputManager->GetButtonHold(PAD_DOWN, 0, 4) ||
		inputManager->GetKeyDown(KEY_INPUT_DOWN) ||
		inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum > IPsNumber - 1) {
			selectNum = 0;
		}
	}

	// ゲームパッドとキーボードの決定ボタンの入力
	if (inputManager->GetButtonDown(B, 0) ||
		inputManager->GetKeyDown(KEY_INPUT_F) ||
		inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		dicideFlg = true;
	}

	// いずれかの決定ボタンが押されていたらブロードキャストIPを設定してホストかゲストどちらになるか選択させる画面へ遷移
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
// ホストになるかゲストになるかを選択させる
////////////////////////////////////////////////
void Network::CommunicationMethodSelect() {
	bool dicideFlg = false;
	// ゲームパッド1の方向パッド上とキーボードの方向キー上の入力
	if (inputManager->GetButtonDown(PAD_UP, 0) ||
		inputManager->GetButtonHold(PAD_UP, 0, 4) ||
		inputManager->GetKeyDown(KEY_INPUT_UP) ||
		inputManager->GetKeyHold(KEY_INPUT_UP, 4)) {
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum < 0) {
			selectNum = 1;
		}
	}

	// ゲームパッド1の方向パッド下とキーボードの方向キー下の入力
	if (inputManager->GetButtonDown(PAD_DOWN, 0) ||
		inputManager->GetButtonHold(PAD_DOWN, 0, 4) ||
		inputManager->GetKeyDown(KEY_INPUT_DOWN) ||
		inputManager->GetKeyHold(KEY_INPUT_DOWN, 4)) {
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum > 1) {
			selectNum = 0;
		}
	}

	// ゲームパッドとキーボードの決定ボタンの入力
	if (inputManager->GetButtonDown(B, 0) ||
		inputManager->GetKeyDown(KEY_INPUT_F) ||
		inputManager->GetKeyDown(KEY_INPUT_RETURN)) {
		dicideFlg = true;
	}

	// いずれかの決定ボタンが押されていたらマッチング画面へ遷移
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
// 通信待機
////////////////////////////////////////////////
void Network::ConnectionWait() {
	// ホストかゲストかで処理を分ける。
	// この関数は、ゲームメインから呼ばれている
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
// 通信待機：ゲスト
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
// 通信待機中の画面
////////////////////////////////////////////////
void Network::DrawConnectionWait() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	// ホスト////////////////////////////////////////////
	if (ConnectType == HOST) {
		if (HOST_phaseNum == HOST_GEST_WAITING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ゲストを待っています...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "ゲストを待っています...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d秒経過", HOST_gestSerchWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d秒経過", HOST_gestSerchWaitTime / 60);

		}
		else if (HOST_phaseNum == HOST_GEST_REPTY_WAITING) {
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
		if (GEST_phaseNum == GEST_HOST_SEARCHING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ホストを探しています...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "ホストを探しています...");

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d秒経過", GEST_hostSerchWaitTime / 60);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "%d秒経過", GEST_hostSerchWaitTime / 60);
		}
		else if (GEST_phaseNum == GEST_MATCH_START) {
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
// 撃つ側の情報を送信する。角度、発射したかどうか、パスしたかどうかを引数に入れる
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
// 隠れる側の情報を送信する。X座標、Y座標、パスしたかどうかを引数に入れる
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
// 撃つ側の情報を受信する。バッファが0になったらtrueが返る
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
// 隠れる側の情報を受信する。バッファが0になったらtrueが返る
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
// デストラクタ
////////////////////////////////////////////////
Network::~Network() {
	DeleteUDPSocket(UDPNetHandle);
}