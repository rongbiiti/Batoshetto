// �\�ߍ���Ă������t�H���g�̃f�[�^���L�����Ă����N���X
#ifndef _FONTDATA_H_
#define _FONTDATA_H_

#include "DxLib.h"

class FontData {
public:
	int f_FontData[2];			// �t�H���g�f�[�^��ۑ����Ă����z��
	FontData();					// �R���X�g���N�^
private:
	void CreateFontData(void);	// �t�H���g�f�[�^���쐬���A�z��ɕۑ�����֐�
};

#endif // !_FONTDATA_H_