// 入力管理クラス
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "DxLib.h"
#include <string>

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

class InputManager {
public:
	InputManager();			// コンストラクタ
	void InputKey(void);	// 入力を受け取る関数
	void InputButton(void);	// Xboxコントローラーの入力を受け取る関数

	int* In_Key()		{ return in_Key; }		// キー入力の結果を他から書き換えられないようにしている

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
									     "10",
									     "11",
									     "A",
									     "B",
									     "X",
									     "Y"
	};

	typedef struct PAD_INPUT	// x座標とy座標をまとめて渡せるようにするための構造体
	{
		int in_Button[16];			// どのボタンが何フレーム押されているかの結果を記憶してる関数
		float in_Stick_LX;			// 左スティックX軸
		float in_Stick_LY;			// 左スティックY軸
		float in_Stick_RX;			// 右スティックX軸
		float in_Stick_RY;			// 右スティックY軸
	}GamePad;

	bool GetButtonDown(BUTTON buttonCode, int playerNum);	// 指定したプレイヤーのパッドのボタンを押した瞬間かを受け取る
	bool GetButtonHold(BUTTON buttonCode, int playerNum);	// 指定したプレイヤーのパッドのボタンを押し続けているかを受け取る
	bool GetButtonHold(BUTTON buttonCode, int playerNum, int reduceValue);	// 押し続けているかを受け取り、ボタンの押し続け秒数を減らす。

	bool GetKeyDown(int KeyCode);	// キーボードのキーを押した瞬間かを受け取る
	bool GetKeyHold(int KeyCode);	// キーボードのキーを押し続けているかを受け取る
	bool GetKeyHold(int KeyCode, int reduceValue);	// 押し続けているかを受け取り、キーの押し続け秒数を減らす。

	GamePad* GetPadInput() { return gamePad; }	// ゲームパッドの構造体ポインタを渡す
	
private:
	const static int HOLD_FRAMECOUNT = 18;

	char Key[256];				// キーボードが押されているかをこの配列に全て入れる。
	XINPUT_STATE input[2];		// DXLibで定義されているXINPUTの入力を受け取ることができる構造体

	int in_Key[256];			// どのキーが何フレーム押されているかの結果を記憶している関数
	GamePad gamePad[2];			// 指定した番号のコントローラーの構造体を渡す
};

#endif // !_INPUTMANAGER_H_