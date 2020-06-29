// 入力管理クラス
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "DxLib.h"

class InputManager {
public:
	InputManager();			// コンストラクタ
	void InputKey(void);	// 入力を受け取る関数

	int k_Buf[256];			// どのキーが押されているかの結果を記憶している関数
private:
	char Key[256];
};

#endif // !_INPUTMANAGER_H_