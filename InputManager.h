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
	float ThumbLX;
	float ThumbLY;
	float ThumbRX;
	float ThumbRY;
	XINPUT_STATE input;
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
	char Key[256];
};

#endif // !_INPUTMANAGER_H_