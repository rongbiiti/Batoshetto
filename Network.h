// 通信対戦用のクラス
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"

const static int PORT_NUMBER = 9876;
class Network {
public:
	Network(FontData* font, InputManager* input);	// コンストラクタ
	~Network();	// デストラクタ

	void InitIPAddress();	// IPアドレスを初期化
	void DrawNetWorkData();		// ネット系のデータ表示
	void IPAddressSelect();		// 使用するIPアドレスを選択させる
	void DrawIPAddressSelect();	// 使用するIPアドレスを選択させる画面を描画

	int GetErrorCode() { return errorCode; }	// エラーコードを返す

private:
	FontData* fontData;	// フォントデータ管理クラスのポインタ
	InputManager* inputManager;	// 入力管理クラスのポインタ
	const static int ALL_IP_LENGTH = 6;	// All_IP配列の要素数
	int UDPNetHandle;	// UDPネットハンドル
	int send;			// 送信するデータ
	int totalsend;		// 送信したsendの合計
	int post;			// 受信したデータ
	int totalpost;		// 受信したpostの合計
	int recvSize;		// 受信したデータのサイズ
	int totalRecvSize;	// 受信データの合計サイズ
	int sendSize;		// 送信したデータのサイズ
	int totalSendSize;	// 送信データの合計サイズ
	int IPsNumber;		// IPアドレスの個数
	int errorCode;		// エラーコード
	IPDATA broadCast_IP;	// 同じLAN内の全コンピュータに送信できるブロードキャストIP
	IPDATA my_IP;		// 自分自身のIP
	IPDATA send_IP;		// 送信相手のIP
	IPDATA All_IP[ALL_IP_LENGTH];	// 複数のネットワークアダプターがあると思うので全部受け取る。最大6個。

	int selectNum;
};

#endif // !_NETWORK_H_