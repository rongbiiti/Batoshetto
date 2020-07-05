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

	int* In_Key()		{ return in_Key; }		// キー入力の結果を他から書き換えられないようにしている
	int* In_Button()	{ return in_Button; }	// ボタン入力の結果を他から書き換えられないようにしている
	float In_Stick_LX() { return in_Stick_LX; }	// 左スティックX軸の値を他から書き換えられないようにしている
	float In_Stick_LY() { return in_Stick_LY; }	// 左スティックY軸Get
	float In_Stick_RX() { return in_Stick_RX; }	// 右スティックX軸Get
	float IN_Stick_RY() { return in_Stick_RY; }	// 右スティックY軸Get

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

	enum BUTTON {
		PAD_UP,
		PAD_DOWN,
		PAD_LEFT,
		PAD_RIGHT,
		START,
		BACK,
		L_PUSH,
		R_PUSH,
		LB,
		RB,
		A = 12,
		B,
		X,
		Y
	};

	// ボタンの名前列挙体
	
private:
	char Key[256];		// キーボードが押されているかをこの配列に全て入れる。
	XINPUT_STATE input;		//DXLibで定義されているXINPUTの入力を受け取ることができる構造体

	int in_Key[256];			// どのキーが何フレーム押されているかの結果を記憶している関数
	int in_Button[16];			// どのボタンが何フレーム押されているかの結果を記憶してる関数
	float in_Stick_LX;			// 左スティックX軸
	float in_Stick_LY;			// 左スティックY軸
	float in_Stick_RX;			// 右スティックX軸
	float in_Stick_RY;			// 右スティックY軸
};

#endif // !_INPUTMANAGER_H_