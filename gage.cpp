//============================================
//
// �Q�[�W�֌W�̃N���X���� [ gage.cpp ]
// Author: Asuma Nishio
//
//=============================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "gage.h"
#include "texture.h"
#include "manager.h"

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CGage::CGage(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nTextIdx = NULL;
}
//===============================
// �f�X�g���N�^
//===============================
CGage::~CGage()
{
	// ����
}
//===============================
// ����������
//===============================
HRESULT CGage::Init(void)
{
	// �e�N���X�̏�����
	CObject2D::Init();

	return S_OK;
}
//===============================
// �I������
//===============================
void CGage::Uninit(void)
{
	// �e�N���X�̏I������
	CObject2D::Uninit();
}
//===============================
// �X�V����
//===============================
void CGage::Update(void)
{
	// �e�N���X�̍X�V
	CObject2D::Update();
}
//===============================
// �`�揈��
//===============================
void CGage::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���擾
	CTexture* pTexture = CManager::GetTexture();

	// �e�N�X�`���ǂݍ���
	pDevice->SetTexture(0, pTexture->GetAddress(m_nTextIdx));

	// �e�N���X�̕`��
	CObject2D::Draw();
}
//===============================
// �`�揈��
//===============================
CGage* CGage::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// �C���X�^���X����
	CGage* pGage = new CGage;

	// �����ł��Ȃ�������
	if (pGage == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pGage->Init()))
	{
		// null�|�C���^��Ԃ�
		return nullptr;
	}

	// 2D�I�u�W�F�N�g�ݒ�
	pGage->SetPos(pos);
	pGage->SetSize(fWidth, fHeight);
	pGage->SetTexture();

	// �|�C���^��Ԃ�
	return pGage;
}
//===================================
// �e�N�X�`���ݒ�֐�
//===================================
void CGage::SetTexture(void)
{
	// �e�N�X�`���|�C���^�擾
	CTexture* pTexture = CManager::GetTexture();

	// �e�N�X�`�����蓖��
	m_nTextIdx = pTexture->Register("data\\TEXTURE\\lifebar_gage.png");
}
