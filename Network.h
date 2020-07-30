// �ʐM�ΐ�p�̃N���X
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "DxLib.h"
#include "FontData.h"
#include "InputManager.h"

const static int PORT_NUMBER = 9876;
class Network {
public:
	Network(FontData* font, InputManager* input);	// �R���X�g���N�^
	~Network();	// �f�X�g���N�^

	void InitIPAddress();	// IP�A�h���X��������
	void DrawNetWorkData();		// �l�b�g�n�̃f�[�^�\��
	void IPAddressSelect();		// �g�p����IP�A�h���X��I��������
	void DrawIPAddressSelect();	// �g�p����IP�A�h���X��I���������ʂ�`��

	int GetErrorCode() { return errorCode; }	// �G���[�R�[�h��Ԃ�

private:
	FontData* fontData;	// �t�H���g�f�[�^�Ǘ��N���X�̃|�C���^
	InputManager* inputManager;	// ���͊Ǘ��N���X�̃|�C���^
	const static int ALL_IP_LENGTH = 6;	// All_IP�z��̗v�f��
	int UDPNetHandle;	// UDP�l�b�g�n���h��
	int send;			// ���M����f�[�^
	int totalsend;		// ���M����send�̍��v
	int post;			// ��M�����f�[�^
	int totalpost;		// ��M����post�̍��v
	int recvSize;		// ��M�����f�[�^�̃T�C�Y
	int totalRecvSize;	// ��M�f�[�^�̍��v�T�C�Y
	int sendSize;		// ���M�����f�[�^�̃T�C�Y
	int totalSendSize;	// ���M�f�[�^�̍��v�T�C�Y
	int IPsNumber;		// IP�A�h���X�̌�
	int errorCode;		// �G���[�R�[�h
	IPDATA broadCast_IP;	// ����LAN���̑S�R���s���[�^�ɑ��M�ł���u���[�h�L���X�gIP
	IPDATA my_IP;		// �������g��IP
	IPDATA send_IP;		// ���M�����IP
	IPDATA All_IP[ALL_IP_LENGTH];	// �����̃l�b�g���[�N�A�_�v�^�[������Ǝv���̂őS���󂯎��B�ő�6�B

	int selectNum;
};

#endif // !_NETWORK_H_