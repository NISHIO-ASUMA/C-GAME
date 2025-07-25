//=========================================
//
// �^�C�g���Ǘ����� [ titlemanager.h ]
// Author: Asuma Nishio
// 
//==========================================

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "titlemanager.h"
#include "titleui.h"

//============================
// �R���X�g���N�^
//============================
CTitleManager::CTitleManager()
{
	// �l�̃N���A
	m_nIdx = NULL;

	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		m_pTitleui[nCnt] = nullptr;
	}
}
//============================
// �f�X�g���N�^
//============================
CTitleManager::~CTitleManager()
{
	// ����
}
//============================
// ����������
//============================
HRESULT CTitleManager::Init(void)
{
	// ����W��ݒ�
	D3DXVECTOR3 CenterPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// �^�C�g����ui�𐶐�
	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		// �����̊Ԋu�󂯂�
		CenterPos.y += nCnt * 180.0f;

		// ui�𐶐� ( �I�����j���[�� )
		m_pTitleui[nCnt] = CTitleUi::Create(CenterPos, COLOR_WHITE, 300.0f, 60.0f, nCnt);
	}

	// ���������ʂ�Ԃ�
	return S_OK;
}
//============================
// �I������
//============================
void CTitleManager::Uninit(void)
{
	// ����
}
//============================
// �X�V����
//============================
void CTitleManager::Update(void)
{
	// �I���ɉ�����

}

void CTitleManager::Draw(void)
{

}
