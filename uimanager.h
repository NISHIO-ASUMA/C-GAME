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
#include "ui.h"
#include <vector>

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

	static CUi* Create(D3DXVECTOR3 pos,int nType,float fWidth,float fHeight);

private:
	static std::vector<CUi*>m_Uiobject; // UI�̓��I�z��
	static int m_nIdxUi;			// UI�̃C���f�b�N�X�ԍ�
};

#endif