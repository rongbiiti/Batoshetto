// リザルト画面描画用クラス
#ifndef _RESULT_H_
#define _RESULT_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameManager.h"

class GameManager;
class Result {
public:
	// フォントデータ管理オブジェクトのポインタ、入力管理オブジェクトのポインタ、
	// ゲーム進行管理オブジェクトのポインタ、ヒットしたプレイヤーがREDかBLUEかを受け取る。
	Result(FontData* font, InputManager* input, GameManager* gameMNG);
	Result(FontData* font, InputManager* input, GameManager* gameMNG, int hitplayernum);
	~Result();					// デストラクタ
	void ResultControll(void);	// 処理
	void ResultControll_Net(void);	// ネット対戦用のリザルト処理
	void DrawResult(void);		// 描画
	void DrawResult_Net(void);	// 描画・ネット対戦用
	void DrawTimeOut(void);		// タイムアウトしたことを描画

	void ChangeVolume(float SEVolume);	// 音の音量変更

private:
	FontData* fontData;			// フォントデータ管理オブジェクトのポインタ
	InputManager* inputManager;	// 入力管理オブジェクトのポインタ
	GameManager* gameManager;	// ゲーム進行管理オブジェクトのポインタ
	int hitPlayerNumber;		// ヒットしたプレイヤーがREDかBLUEかの引数を保存しておく変数。

	void SceneTransition();		// シーン遷移

	void Return_to_Game();		// ポーズ画面を抜けて試合を再開する
	void ReMatch();				// ネット対戦で、同じ条件で相手と再戦する
	void Return_to_Title();		// 試合を中断してタイトル画面へ戻る

	const static int SELECT_NUM_MAX = 1;
	const static int SCENE_TRANSITION_WAITING_TIME = 60;	// シーン遷移するときのちょっとした待ち時間
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "ReMatch",
													   "Return to Title"
	};

	int waitTime;						// 項目を決定したあとのちょっとした待ち時間
	int selectNum[SELECT_NUM_MAX + 1];	// 選択中のボタン番号
	bool dicideNumFlg[2];				// 番号を決定したかのフラグ

	bool timeOutFlg;	// 通信対戦で、タイムアウトが発生してリザルト画面へ遷移してきたかのフラグ

	void LoadImages();	// 画像読み込み
	void LoadSounds();	// 音データ読み込み

	void DeleteImages();	// 画像データメモリから消去
	void DeleteSounds();	// 音データ消去

	int i_ResultImage;	// 背景画像
	int i_MenuImage[4];	// 項目名画像

	bool soundFlg;		// 勝敗SEを鳴らしたかのフラグ
	int s_WinSE;		// 普通に勝ったときの音
	int s_LoseSE;		// 自滅したときの音
};

#endif // !_RESULT_H_