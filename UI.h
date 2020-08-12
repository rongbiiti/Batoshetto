// 金城龍吾PresentsUIクラス
#ifndef _UI_H_
#define _UI_H_

#include "DxLib.h"
#include "GameMain.h"

class GameMain;	// クラスのプロトタイプ宣言
class UI {
public:
	UI(GameMain* main);	// コンストラクタ
	~UI();	// デストラクタ

	void UIControll();	// パラメータを変更処理
	bool TransitionAnimationWaiting();	// 隠れる・撃つの切替時のアニメーションの処理
	void DrawTransitionAnimation();		// 隠れる・撃つの切替時のアニメーション
	void TransitionParameterReset();	// 遷移アニメに使う変数リセット

	bool GetTransitionFlg() {return transitionFlg;}// 遷移アニメ中かのフラグを返す

	// プレイヤーの残り時間をゲージで描画する関数
	void DrawPlayerGuage(int x, int y, float maxtime, float nowtime, int playernum);
	void DrawBackGround();	// 背景画像描画
	void DeleteImages();// 画像データメモリから消去

private:
	GameMain* gameMain;		// ゲームメインクラスのポインタ
	GameManager* gameManager;	// ゲーム進行管理クラスのポインタ
	FontData* fontData;

	bool isBattleStart;	// 試合を開始した直後か
	int firstAnimWaitTime;	// 試合開始時のアニメの待ち時間
	int firstAnimX;
	int firstAnimY;
	int firstAnimAlpha;

	bool BattleStartAnim();	// 試合開始時のアニメ。演出が終わっていたらFALSEを返す
	void DrawBattleStartAnim();	// 試合開始時のアニメ描画

	int animationWaitingTime;	// アニメ待ち時間
	int transitionX;	// 遷移用X座標
	int transitionY;	// 遷移用Y座標
	bool transitionFlg;	// 遷移アニメ中か

	void LoadImages();	// 画像読み込み
	
	int i_PlayerGuage[2];	// ゲージの画像
	int i_BackGroundImage;	// 背景画像
};

#endif // !_UI_H_