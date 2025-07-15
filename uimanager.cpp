//======================================
//
// UI�S�̂��Ǘ����鏈�� [ uimanager.cpp ]
// Author: Asuma Nishio
//
//======================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "uimanager.h"

//****************************
// �ÓI�����o�ϐ��錾
//****************************
std::vector<CUi*>CUimanager::m_Uiobject = {};
int CUimanager::m_nIdxUi = NULL;

//===============================
// �R���X�g���N�^
//===============================
CUimanager::CUimanager()
{
	m_Uiobject.clear();
}
//===============================
// �f�X�g���N�^
//===============================
CUimanager::~CUimanager()
{
	// ����
}
//===============================
// ��������
//===============================
CUi* CUimanager::Create(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight)
{
	// �|�C���^����
	CUi* pNewUi = CUi::Create(pos, nType, fWidth, fHeight);

	if (pNewUi)
	{
		// �z��ǉ�
		m_Uiobject.push_back(pNewUi);

		// �C���f�b�N�X���Z
		m_nIdxUi++;

		// �������ꂽ�|�C���^��Ԃ�
		return pNewUi;
	}
	else
	{
		return nullptr;
	}
}
//===============================
// ����������
//===============================
HRESULT CUimanager::Init(void)
{
	return S_OK;
}
//===============================
// �I������
//===============================
void CUimanager::Uninit(void)
{
	// �z��N���A
	m_Uiobject.clear();
}
//===============================
// �X�V����
//===============================
void CUimanager::Update(void)
{

}
