#include "Network.h"
#include "GameMain.h"

// コンストラクタ
Network::Network(FontData* font, InputManager* input) {
	fontData = font;
	inputManager = input;

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
}

// broadCast_IPを初期化
void Network::InitIPAddress() {
	// コンピュータの全IPアドレスを取得
	GetMyIPAddress(All_IP, ALL_IP_LENGTH, &IPsNumber);

	my_IP.d1 = 0;
	my_IP.d2 = 0;
	my_IP.d3 = 0;
	my_IP.d4 = 0;

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
		sendSize = NetWorkSendUDP(UDPNetHandle, All_IP[i++], PORT_NUMBER, &send, sizeof(send));
		recvSize = NetWorkRecvUDP(UDPNetHandle, &my_IP, NULL, &post, sizeof(post), FALSE);

		totalSendSize += sendSize;
		totalsend += send;

		if (recvSize > 0) {
			totalRecvSize += recvSize;
			totalpost += post;
		}
	}while (i < IPsNumber);

	// もし有効なIPアドレスがひとつもなかったらエラーコードに1をセットして処理を終わる
	if (my_IP.d1 == 0) {
		errorCode = 1;
		return;
	}

}

// 使用するIPアドレスを選択させる
void Network::IPAddressSelect() {
	if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] == 1 || inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] >= 18) {
		// ゲームパッド1の方向パッド上の入力。18フレ以上押し続けてたら連続でデクリメント
		// 0未満になったら項目最大数の数字にする（カーソル上に移動、一番上のときに上を押したらメニューの一番下にカーソルをあわせる）
		if (--selectNum < 0) {
			selectNum = IPsNumber - 1;
		}
		if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] >= 18) {
			inputManager->GetPadInput()[0].in_Button[InputManager::PAD_UP] -= 4;
		}
	}

	if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] == 1 || inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] >= 18) {
		// ゲームパッド1の方向パッド下の入力。18フレ以上押し続けてたら連続でインクリメント
		// 項目最大数の数字より大きくなったら0に戻す（カーソル下に移動、一番下のときに下を押したらメニューの一番上にカーソルをあわせる）
		if (++selectNum > IPsNumber - 1) {
			selectNum = 0;
		}
		if (inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] >= 18) {
			inputManager->GetPadInput()[0].in_Button[InputManager::PAD_DOWN] -= 4;
		}
	}

	if (inputManager->GetPadInput()[0].in_Button[InputManager::B] == 1) {
		my_IP = All_IP[selectNum];
	}
}

// 使用するIPアドレスを選択させる画面を描画
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

// デストラクタ
Network::~Network() {

}