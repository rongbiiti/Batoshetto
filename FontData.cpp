#include "FontData.h"

FontData::FontData() {
	CreateFontData();
}

void FontData::CreateFontData() {
	f_FontData[0] = CreateFontToHandle(NULL, 20, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//フォントデータをあらかじめ作成しておく
	f_FontData[1] = CreateFontToHandle(NULL, 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//フォントデータをあらかじめ作成しておく
}