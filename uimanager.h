//======================================
//
// UI�S�̂��Ǘ����鏈�� [ uimanager.h ]
// Author: Asuma Nishio
//
//======================================

#ifndef _UIMANAGER_H_ // ���̃}�N����`������ĂȂ�������
#define _UIMANAGER_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//****************************
// �C���N���[�h�t�@�C���錾
//****************************

//****************************
// UI�Ǘ��N���X���`
//****************************
class CUimanager
{
public:
	CUimanager();
	~CUimanager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	static int m_nIdxUi;			// UI�̃C���f�b�N�X�ԍ�

	// NOTE : �o��ui��z��Ŏ����ă��[�h�œK�p���鏈�����쐬

};

#endif