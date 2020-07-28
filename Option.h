// オ　プ　シ　ョ　ン　画　面
#define _CRT_SECURE_NO_WARNINGS
#ifndef _OPTION_H_
#define _OPTION_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameManager.h"
#include <stdio.h>
#include <stdlib.h>

class GameMain;
class GameManager;

class Option {
public:
	Option(GameMain* main, int pushPLnum, int prescreennum, int prephase = 3);	// コンストラクタ
	~Option();	// デストラクタ

	enum OPTION_MENU_NUM {
		TOP,
		VOLUME
	};
	
	// 前画面がどこだったか
	enum PREVIOUS_SCREEN {
		TITLE,
		PAUSE
	};

	void OptionControll();	// オプション画面の処理関数
	void DrawOption();		// オプション画面の描画

	float GetBGMVolume() { return BGMVolume; }
	float GetSEVolume() { return SEVolume; }

private:
	GameMain* gameMain;	// ゲームメインのポインタ
	InputManager* inputManager;	// 入力管理クラスのポインタ
	FontData* fontData;	// フォントデータ管理クラスのポインタ

	// セーブ用の構造体
	typedef struct {
		float bgmVolume = 1.0f;
		float seVolume = 1.0f;
	}option_save_data_t;

	option_save_data_t saveData;

	void ControllOptionTop();	// オプショントップ画面の処理
	void ControllVolumeScreen();// 音量調節画面の処理

	void DrawOptionMain();	// オプション画面のトップを描画
	void DrawVolumeMenu();	// 音量調節画面を描画
	void BackOptionMenu();	// 各調節画面からトップに戻るための関数
	void Return_to_PauseScreen();	// ポーズ画面へ戻る
	void Return_to_Title();		// タイトル画面へ戻る

	void PlayCursorSound() {}	// カーソル音再生
	void PlayDicideSound() {}	// 決定音再生
	void PlayCanselSound() {}	// キャンセル音再生

	void ChangeBulletSoundVolume();	// 弾の音量調節

	void LoadFile();	// セーブデータ読み込み
	void SaveFile();	// セーブデータ保存

	const static int SELECT_NUM_MAX = 1;
	const std::string MenuName[SELECT_NUM_MAX + 1] = { "Volume",
													   "Back"
	};

	const static int VOLUME_SELECT_NUM_MAX = 2;
	const std::string VolumeMenuName[VOLUME_SELECT_NUM_MAX + 1] = { "BGM",
																	"SE",
																	"BACK"
	};

	int screenNum;	// 今どの画面を開いてるかの番号
	int selectNum[SELECT_NUM_MAX + 1];				// 選択中のボタン番号
	int optionPushPLNum;			// オプションを開いたプレイヤーの番号 0=RED 1=BLUE 2=キーボードからの入力
	int previousNum;	// タイトルから来たかポーズから来たかを分岐できる
	int previousGamePhase;	// ポーズ画面から入ったとき、直前のフェーズを保存しておく

	float BGMVolume;	// BGMの音量
	float SEVolume;		// SEの音量
};

#endif // !_OPTION_H_