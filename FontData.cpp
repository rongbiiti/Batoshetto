#include "FontData.h"

// �R���X�g���N�^
FontData::FontData() {
	CreateFontData();
}

// �t�H���g�f�[�^���쐬���A�z��ɕۑ����Ă����֐�
void FontData::CreateFontData() {
	f_FontData[0] = CreateFontToHandle(NULL, 20, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
	f_FontData[1] = CreateFontToHandle(NULL, 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
}