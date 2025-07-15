//====================================
//
// UI���� [ ui.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "ui.h"
#include "texture.h"
#include "manager.h"

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CUi::CUi(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nTexIdxType = NULL;
}
//===============================
// �f�X�g���N�^
//===============================
CUi::~CUi()
{
	// ����
}
//===============================
// ����������
//===============================
HRESULT CUi::Init(void)
{
	// �I�u�W�F�N�g�̏�����
	CObject2D::Init();

	return S_OK;
}
//===============================
// �I������
//===============================
void CUi::Uninit(void)
{
	// �I�u�W�F�N�g�̏I��
	CObject2D::Uninit();
}
//===============================
// �X�V����
//===============================
void CUi::Update(void)
{
	// �I�u�W�F�N�g�̍X�V
	CObject2D::Update();
}
//===============================
// �`�揈��
//===============================
void CUi::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���Z�b�g
	CTexture* pTexture = CManager::GetTexture();

	// ���蓖��
	pDevice->SetTexture(0, pTexture->GetAddress(m_nTexIdxType));

	// �I�u�W�F�N�g�̕`��
	CObject2D::Draw();
}
//===============================
// ��������
//===============================
CUi* CUi::Create(D3DXVECTOR3 pos,int nType,float fWidth,float fHeight)
{
	// �C���X�^���X����
	CUi* pUi = new CUi;

	// �e�N�X�`���ݒ�
	pUi->SetTexture(nType);

	// ���������s��
	if (FAILED(pUi->Init()))
	{
		// �|�C���^�̔j��
		delete pUi;

		// nullptr���
		pUi = nullptr;

		// null�|�C���^��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g�ݒ�
	pUi->SetPos(pos);
	pUi->SetSize(fWidth, fHeight);

	// �������ꂽ�|�C���^��Ԃ�
	return pUi;
}
//===============================
// �e�N�X�`���Z�b�g����
//===============================
void CUi::SetTexture(int nType)
{
	// �e�N�X�`���|�C���^�擾
	CTexture* pTexture = CManager::GetTexture();

	// �e�N�X�`���ݒ�
	switch (nType)
	{
	case SCENETYPE_NONE:
		m_nTexIdxType = pTexture->Register("data\\TEXTURE\\billboard_wepon.png");
		break;

	default:
		break;
	}
}
