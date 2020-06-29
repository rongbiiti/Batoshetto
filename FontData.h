// 予め作っておいたフォントのデータを記憶しておくクラス
#ifndef _FONTDATA_H_
#define _FONTDATA_H_

#include "DxLib.h"

class FontData {
public:
	int f_FontData[2];			// フォントデータを保存しておく配列
	FontData();					// コンストラクタ
private:
	void CreateFontData(void);	// フォントデータを作成し、配列に保存する関数
};

#endif // !_FONTDATA_H_