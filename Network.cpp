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

	ConnectType = HOST;	// ホストかゲストか

	// UDPハンドルの作成
	UDPNetHandle = MakeUDPSocket(PORT_NUMBER);
	VariableInit();
	InitIPAddress();
	
}

////////////////////////////////////////////////
// 普通の変数の初期化
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

	// 送り先のIPアドレス初期化
	send_IP.d1 = 0;
	send_IP.d2 = 0;
	send_IP.d3 = 0;
	send_IP.d4 = 0;

	selectNum = 0;

	isWaitRecvCheck = FALSE;

	StructsReset();	// 構造体初期化
}

////////////////////////////////////////////////
// 試合用送受信構造体初期化！
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
// 受信データ加算
////////////////////////////////////////////////
void Network::RecvDataAddition() {
	// 受信データサイズを足す
	if (recvSize >= 0) {
		totalRecvSize += recvSize;
		totalpost += matchInfo_Post.num;
	}
}

////////////////////////////////////////////////
// 送信データ加算
////////////////////////////////////////////////
void Network::SendDataAddition() {
	// 送信データサイズを足す
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
		ConnectType = selectNum;

		// ホストとして通信することを選択していた場合、乱数の初期値を生成する。自分の乱数の初期値も、その値に設定する。
		SetSendRand();

		// 受信用構造体を初期化
		matchInfo_Post.num = 0;
		matchInfo_Post.difficulty = 0;
		matchInfo_Post.seed = 0;

		// 通信待機フェーズに遷移する
		gameManager->SetPhaseStatus(GameManager::CONNECTION_WAIT);

		// バッファをクリアしておく
		BufferClear();
	}
}

////////////////////////////////////////////////
// 乱数を生成
////////////////////////////////////////////////
void Network::SetSendRand() {
	// ホストとして通信することを選択していた場合、乱数の初期値を生成する。自分の乱数の初期値も、その値に設定する。
	if (ConnectType == HOST) {
		randSeedNum = GetRand(10000);	// 乱数を生成
		SRand(randSeedNum);		// 生成した値で乱数初期値を設定
		matchInfo_Send.seed = randSeedNum;	// 相手に送るデータに代入
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

	//////////// ホストがゲストからの応答待ち ////////////
	if (HOST_phaseNum == HOST_GEST_WAITING) {
		++HOST_gestSerchWaitTime;	// ホストの待ち時間インクリメント
		matchInfo_Post.num = 0;		// 受信した値0にリセット

		// 受信。受信相手のIPアドレスを同時に取得している。
		recvSize = NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);
		RecvDataAddition();		// 総受信量増分

		// 受信したデータのnumが1なら、ゲストがホストを探していることがわかる。ゲスト発見。
		// ゲストに、発見したことを伝えるフェーズに移行
		if (matchInfo_Post.num == 1 && matchInfo_Post.difficulty == gameManager->GetDifficulty()) {
			HOST_phaseNum = HOST_GEST_REPTY_WAITING;
			matchInfo_Send.num = 2;
		}
	}

	//////////// ホストがゲストを発見。ゲストに発見できたことを伝え、その返信待ち ////////////
	else if (HOST_phaseNum == HOST_GEST_REPTY_WAITING) {
		// 0.5秒に1回、ゲストに発見したことを伝える。
		if (++HOST_gestReplyWaitTime % 30 == 0) {
			matchInfo_Send.num = 2;	// 送信numに2を入れる。2は、ホストがゲストを見つけたことを伝えている。
			sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();
		}

		// 受信。ゲストからの返事を待つ
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);
		RecvDataAddition();		// 総受信量増分

		// ゲストからの返事が返ってきたら、ゲーム開始！！！！
		if (matchInfo_Post.num == 3 && sendSize >= 0) {
			// 初期処理フェーズに飛ぶ
			gameManager->gameMain->Init();
		}
	}
}

////////////////////////////////////////////////
// 通信待機：ゲスト
////////////////////////////////////////////////
void Network::ConnectionWait_TypeGEST() {

	//////////// ゲストがホストを探している ////////////
	if (GEST_phaseNum == GEST_HOST_SEARCHING) {

		// 0.5秒に1回、ブロードキャストIPを使ってホストがいないか信号を送る。
		if (++GEST_hostSerchWaitTime % 30 == 0) {
			matchInfo_Send.num = 1;		// 1の値は、ゲストがホストを探している。

			// 送信。ホストのIPがわからないので、ブロードキャストIPを使ってLANの全パソコンに送信
			sendSize = NetWorkSendUDP(UDPNetHandle, broadCast_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();		// 総送信量増分。
		}

		matchInfo_Post.num = 0;		// 受信した値0にリセット

		// 受信。ホストからの発見報告待ち。ホストのIPも受け取る
		recvSize = NetWorkRecvUDP(UDPNetHandle, &send_IP, NULL, &matchInfo_Post, sizeof(matchInfo_Post), FALSE);
		RecvDataAddition();		// 総受信量増分

		// ホストからの発見報告が来ていたら、その返事をするフェーズに移行。
		if (matchInfo_Post.num == 2) {
			GEST_phaseNum = GEST_MATCH_START;
			GEST_hostSerchWaitTime = 0;
		}
	}

	//////////// ホストがゲストを発見したことがわかった。返事をする ////////////
	else if (GEST_phaseNum == GEST_MATCH_START) {

		// 0.5秒に1回、ホストに発見報告への返事をする。
		if (++GEST_hostSerchWaitTime % 30 == 0) {
			matchInfo_Send.num = 3;		// 送信する値は3

			// 送信。ホストのIPがわかっているので、それを直接指定している
			sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &matchInfo_Send, sizeof(matchInfo_Send));
			SendDataAddition();		// 総送信量増分

			// 送信サイズが0以上なら、正しく送信できたことにして、ゲームを開始する
			if (sendSize >= 0) {
				SRand(matchInfo_Post.seed);		// ホストから送られてきた値で乱数初期値を設定。
				gameManager->gameMain->Init();	// 初期処理フェーズに飛ぶ
			}
		}
		
	}
}

////////////////////////////////////////////////
// 撃つ側の情報を送信する。角度、発射したかどうか、パスしたかどうかを引数に入れる
////////////////////////////////////////////////
void Network::SendShooterInfo(float ang, bool isShot, bool isPass) {
	shooterInfo_Send.angle = ang;
	shooterInfo_Send.shotFlg = isShot;
	shooterInfo_Send.passFlg = isPass;

	// 撃ったフラグとパスしたフラグ、どちらかがTRUEなら、返信が必要なデータとして送信する
	if (isShot || isPass) {
		shooterInfo_Send.isRecvCheck = TRUE;
		isWaitRecvCheck = TRUE;
	}
	else {
		shooterInfo_Send.isRecvCheck = FALSE;
		isWaitRecvCheck = FALSE;
	}

	// 送信
	sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &shooterInfo_Send, sizeof(shooterInfo_Send));
	SendDataAddition();		// 総送信量増分
}

////////////////////////////////////////////////
// 隠れる側の情報を送信する。X座標、Y座標、パスしたかどうかを引数に入れる
////////////////////////////////////////////////
void Network::SendHiderInfo(int px, int py, float angle, bool isPass) {
	hiderInfo_Send.x = px;
	hiderInfo_Send.y = py;
	hiderInfo_Send.angle = angle;
	hiderInfo_Send.passFlg = isPass;

	// パスしたフラグがTRUEなら、返信が必要なデータとして送信する
	if (isPass) {
		hiderInfo_Send.isRecvCheck = TRUE;
		isWaitRecvCheck = TRUE;
	}
	else {
		hiderInfo_Send.isRecvCheck = FALSE;
		isWaitRecvCheck = FALSE;
	}

	// 送信
	sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &hiderInfo_Send, sizeof(hiderInfo_Send));
	SendDataAddition();		// 総送信量増分
}

////////////////////////////////////////////////
// 返信できたことを送信する。
////////////////////////////////////////////////
void Network::SendRecvCheck() {
	bool recvCheck_Send = TRUE;
	// 無限ループ
	while (1)
	{
		// TRUEを送る
		sendSize = NetWorkSendUDP(UDPNetHandle, send_IP, PORT_NUMBER, &recvCheck_Send, sizeof(recvCheck_Send));
		if (sendSize >= 0) {
			// 送信成功していたら、ループを抜ける
			SendDataAddition();
			break;
		}
		// ウインドウズメッセージ処理
		if (ProcessMessage() < 0) break;
	}
}

////////////////////////////////////////////////
// 撃つ側の情報を受信する。バッファが0になったらtrueが返る
////////////////////////////////////////////////
bool Network::PostShooterInfo() {
	// 無限ループ
	while (1)
	{
		// 受信バッファをすべて見る。
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &shooterInfo_Post, sizeof(shooterInfo_Post), FALSE);
		if (recvSize == -3) {
			// -3は、受信データ無しを意味する。-3なら、ループを抜ける。
			RecvDataAddition();		// 総受信量増分
			break;
		}
		else {
			RecvDataAddition();		// 総受信量増分
		}
		// ウインドウズメッセージ処理
		if (ProcessMessage() < 0) break;
	}
	return true;
}

////////////////////////////////////////////////
// 隠れる側の情報を受信する。バッファが0になったらtrueが返る
////////////////////////////////////////////////
bool Network::PostHiderInfo() {
	// 無限ループ
	while (1)
	{
		// 受信バッファをすべて見る。
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &hiderInfo_Post, sizeof(hiderInfo_Post), FALSE);
		if (recvSize == -3) {
			// -3は、受信データ無しを意味する。-3なら、ループを抜ける。
			RecvDataAddition();		// 総受信量増分
			break;
		}
		else {
			RecvDataAddition();		// 総受信量増分
		}
		// ウインドウズメッセージ処理
		if (ProcessMessage() < 0) break;
	}
	return true;
}

////////////////////////////////////////////////
// 隠れる側の情報を受信する。バッファが0になったらtrueが返る
////////////////////////////////////////////////
bool Network::PostRecvCheck() {
	// 無限ループ
	while (1)
	{
		// 受信バッファをすべて見る。
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, &recvCheck_Post, sizeof(recvCheck_Post), FALSE);
		if (recvSize == -3) {
			// -3は、受信データ無しを意味する。-3なら、ループを抜ける。
			RecvDataAddition();		// 総受信量増分
			break;
		}
		else {
			RecvDataAddition();		// 総受信量増分
		}
		// ウインドウズメッセージ処理
		if (ProcessMessage() < 0) break;
	}
	return true;
}

////////////////////////////////////////////////
// バッファークリア
////////////////////////////////////////////////
void Network::BufferClear() {
	// 無限ループ
	while (1)
	{
		// 受信バッファをすべて見る。
		recvSize = NetWorkRecvUDP(UDPNetHandle, NULL, NULL, NULL, NULL, FALSE);
		if (recvSize == -3) {
			// -3は、受信データ無しを意味する。-3なら、ループを抜ける。
			RecvDataAddition();		// 総受信量増分
			break;
		}
		else {
			RecvDataAddition();		// 総受信量増分
		}
		// ウインドウズメッセージ処理
		if (ProcessMessage() < 0) break;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////		ここから下は描画処理			//////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////
// ホストになるかゲストになるかを選択させる画面の描画
////////////////////////////////////////////////
void Network::DrawCommunicationMethodSelect() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ホストになるかゲストになるか");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "ホストになるかゲストになるか");
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "選択してください");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xFFFFFF, fontData->f_FontData[1], "選択してください");

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ホストになる");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 30, 0xFFFFFF, fontData->f_FontData[1], "ホストになる");
	DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y, 0xFFFFFF, fontData->f_FontData[1], "ゲストになる");


	// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
	DrawRotaGraph(GameMain::SCREEN_WIDTH / 4, starty + y * selectNum, 1, 0, gameManager->gameMain->GetCursorImage(0), TRUE);
}

////////////////////////////////////////////////
// 使用するIPアドレスを選択させる画面を描画
////////////////////////////////////////////////
void Network::DrawIPAddressSelect() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;

	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "お使いになる");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "お使いになる");
	fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "IPアドレスを選択してください");
	DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xFFFFFF, fontData->f_FontData[1], "IPアドレスを選択してください");
	// 各項目名描画
	for (int i = 0; i < IPsNumber; i++) {
		fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "%d.%d.%d.%d", All_IP[i].d1, All_IP[i].d2, All_IP[i].d3, All_IP[i].d4);
		DrawFormatStringToHandle(x - fontwidth / 2, starty - 30 + y * i, 0xFFFFFF, fontData->f_FontData[1], "%d.%d.%d.%d", All_IP[i].d1, All_IP[i].d2, All_IP[i].d3, All_IP[i].d4);
	}
	// プレイヤーの選択中のカーソル位置にプレイヤー色の丸を描画
	DrawRotaGraph(GameMain::SCREEN_WIDTH / 4, starty + y * selectNum, 1, 0, gameManager->gameMain->GetCursorImage(0), TRUE);
}

////////////////////////////////////////////////
// 通信待機中の画面
////////////////////////////////////////////////
void Network::DrawConnectionWait() {
	// 文字の幅、			画面の横中心、　　　　　　　Y軸の増加量、　初期Yの位置
	int fontwidth = 0, x = GameMain::SCREEN_WIDTH / 2, y = 70, starty = 300;
	int min = 0, sec = 0;

	// ホスト////////////////////////////////////////////
	if (ConnectType == HOST) {
		if (HOST_phaseNum == HOST_GEST_WAITING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ゲストを待っています...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "ゲストを待っています...");

			sec = HOST_gestSerchWaitTime / 60;
			min = sec / 60;
			if (60 <= sec) sec -= 60;

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "経過時間：%d:%02d", min, sec);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xFFFFFF, fontData->f_FontData[1], "経過時間：%d:%02d", min, sec);

		}
		else if (HOST_phaseNum == HOST_GEST_REPTY_WAITING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ゲストが見つかりました。");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xeeff14, fontData->f_FontData[1], "ゲストが見つかりました。");
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "返答を待っています...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 140, 0xeeff14, fontData->f_FontData[1], "返答を待っています...");

			sec = HOST_gestReplyWaitTime / 60;
			min = sec / 60;
			if (60 <= sec) sec -= 60;

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "経過時間：%d:%02d", min, sec);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xeeff14, fontData->f_FontData[1], "経過時間：%d:%02d", min, sec);
		}

	}


	// ゲスト/////////////////////////////////////////////
	else if (ConnectType == GEST) {
		if (GEST_phaseNum == GEST_HOST_SEARCHING) {
			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "ホストを探しています...");
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty - 200, 0xFFFFFF, fontData->f_FontData[1], "ホストを探しています...");

			sec = GEST_hostSerchWaitTime / 60;
			min = sec / 60;
			if (60 <= sec) sec -= 60;

			fontwidth = GetDrawFormatStringWidthToHandle(fontData->f_FontData[1], "経過時間：%d:%02d", min, sec);
			DrawFormatStringToHandle(GameMain::SCREEN_WIDTH / 2 - fontwidth / 2, starty, 0xFFFFFF, fontData->f_FontData[1], "経過時間：%d:%02d", min, sec);
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
// デストラクタ
////////////////////////////////////////////////
Network::~Network() {
	DeleteUDPSocket(UDPNetHandle);
}