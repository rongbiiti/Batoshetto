// 入力管理クラス
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "DxLib.h"
#include <string>

class InputManager {
public:
	InputManager();			// コンストラクタ
	void InputKey(void);	// 入力を受け取る関数
	void InputButton(void);	// Xboxコントローラーの入力を受け取る関数

	int k_Buf[256];			// どのキーが押されているかの結果を記憶している関数
	int k_Button[16];		// どのボタンが押されているかの結果を記憶してる関数
	float ThumbLX;			// 左スティックX軸
	float ThumbLY;			// 左スティックY軸
	float ThumbRX;			// 右スティックX軸
	float ThumbRY;			// 右スティックY軸

	XINPUT_STATE input;		//DXLibで定義されているXINPUTの入力を受け取ることができる構造体

	// ボタンの名前
	const std::string buttonName[16] = { "方向パッド上",
									     "方向パッド下",
									     "方向パッド左",
									     "方向パッド右",
									     "START",
									     "BACK",
									     "左スティック押し込み",
									     "右スティック押し込み",
									     "LB",
									     "RB",
									     "",
									     "",
									     "A",
									     "B",
									     "X",
									     "Y"
	};
private:
	char Key[256];		// キーボードが押されているかをこの配列に全て入れる。
};

#endif // !_INPUTMANAGER_H_