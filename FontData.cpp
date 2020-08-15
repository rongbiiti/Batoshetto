#include "FontData.h"

////////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////////
FontData::FontData() {
	CreateFontData();	
}

////////////////////////////////////////////////
// �t�H���g�f�[�^���쐬���A�z��ɕۑ����Ă����֐�
////////////////////////////////////////////////
void FontData::CreateFontData() {
	// �t�H���g�t�@�C����ǂݍ���
	LPCSTR font_path = "fonts/SoukouMincho.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X

	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
	}

	f_FontData[0] = CreateFontToHandle("���b����", 20, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
	f_FontData[1] = CreateFontToHandle("���b����", 60, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
	f_FontData[2] = CreateFontToHandle("���b����", 90, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);	//�t�H���g�f�[�^�����炩���ߍ쐬���Ă���
}