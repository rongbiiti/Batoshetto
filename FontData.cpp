#include "FontData.h"

FontData::FontData() {
	CreateFontData();
}

void FontData::CreateFontData() {
	f_FontData[0] = CreateFontToHandle(NULL, 20, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
	f_FontData[1] = CreateFontToHandle(NULL, 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
}