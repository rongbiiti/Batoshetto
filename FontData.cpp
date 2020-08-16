#include "FontData.h"

////////////////////////////////////////////////
// コンストラクタ
////////////////////////////////////////////////
FontData::FontData() {
	CreateFontData();	
}

////////////////////////////////////////////////
// フォントデータを作成し、配列に保存しておく関数
////////////////////////////////////////////////
void FontData::CreateFontData() {
	// フォントファイルを読み込む
	LPCSTR font_path = "fonts/SoukouMincho.ttf"; // 読み込むフォントファイルのパス

	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}

	f_FontData[0] = CreateFontToHandle("装甲明朝", 20, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//フォントデータをあらかじめ作成しておく
	f_FontData[1] = CreateFontToHandle("装甲明朝", 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//フォントデータをあらかじめ作成しておく
	f_FontData[2] = CreateFontToHandle("装甲明朝", 90, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//フォントデータをあらかじめ作成しておく
}