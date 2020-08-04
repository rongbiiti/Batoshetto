// 通信対戦用のクラス
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"

const static int PORT_NUMBER = 9876;
class Network {
public:
	enum CONNECT_TYPE {
		HOST,
		GEST
	};

	Network(FontData* font, InputManager* input, GameManager* gameMNG);	// コンストラクタ
	~Network();	// デストラクタ

	void InitIPAddress();	// IPアドレスを初期化
	void VariableInit();	// 普通の変数を初期化

	void DrawNetWorkData();		// ネット系のデータ表示

	void RecvDataAddition();	// 受信データ加算
	void SendDataAddition();	// 送信データ加算

	void IPAddressSelect();		// 使用するIPアドレスを選択させる
	void DrawIPAddressSelect();	// 使用するIPアドレスを選択させる画面を描画

	void CommunicationMethodSelect();	// ホストになるかゲストになるかを選択させる
	void DrawCommunicationMethodSelect();	// ホストになるかゲストになるかを選択させる画面の描画

	void ConnectionWait();				// 通信待機
	void ConnectionWait_TypeHOST();		// 通信待機：ホスト側
	void ConnectionWait_TypeGEST();		// 通信待機：ゲスト側
	void DrawConnectionWait();			// 通信待機中の描画

	int GetErrorCode() { return errorCode; }	// エラーコードを返す

private:
	FontData* fontData;	// フォントデータ管理クラスのポインタ
	InputManager* inputManager;	// 入力管理クラスのポインタ
	GameManager* gameManager;	// ゲーム進行管理クラスのポインタ

	typedef struct MATCHING_INFO	// sendと選択した難易度を同時に送る
	{
		int num;		// 送信する値
		int difficulty;	// 選択した難易度
		int seed;		// 乱数のシード値
	}MatchingInfo;

	MatchingInfo matchInfo_Send;	// 構造体生成
	MatchingInfo matchInfo_Post;	// 受信用構造体

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

	int ConnectType;	// ホストかゲストか 0 = ホスト。1 = ゲスト。

	int GEST_hostSerchWaitTime;	// ゲストの、ホストを探している時間
	int HOST_gestSerchWaitTime;	// ホストの、ゲストを待機している時間
	int HOST_gestReplyWaitTime;	// ホストの、ゲストからの返信待ち時間
	
	int GEST_phaseNum;			// ゲストの通信確立までの過程の変数
	int HOST_phaseNum;			// ホストの通信確立までの過程の変数

	IPDATA broadCast_IP;	// 同じLAN内の全コンピュータに送信できるブロードキャストIP
	IPDATA my_IP;		// 自分自身のIP
	IPDATA send_IP;		// 送信相手のIP
	IPDATA All_IP[ALL_IP_LENGTH];	// 複数のネットワークアダプターがあると思うので全部受け取る。最大6個。

	int selectNum;

	int randSeedNum;	// 乱数のシード値
};

#endif // !_NETWORK_H_