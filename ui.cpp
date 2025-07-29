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

	// ���������ʂ�Ԃ�
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
	// ���W�擾
	D3DXVECTOR3 Pos = GetPos();

	// 0.0f��菬�����ʒu�ɂ���
	if (Pos.y <= 150.0f)
	{
		Pos.y += 5.0f;
	}

	// ���W�Z�b�g
	SetPos(Pos);

	if (m_type == UITYPE_NONE)
	{
		SetFlash(10, 20);
	}

	// �I�u�W�F�N�g�̍X�V
	CObject2D::Update();
}
//===============================
// �`�揈��
//===============================
void CUi::Draw(void)
{
	// �t���O�̎擾
	CCamera* pCamera = CManager::GetCamera();

	// �擾���s��
	if (pCamera == nullptr) return;

	// �t���O���Q�b�g����
	bool isKey = pCamera->GetIsRotation();

	if (!isKey)
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
	pUi->SetAnchor(CObject2D::ANCHORTYPE_CENTER);
	pUi->m_type = nType;

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
	case UITYPE_NONE:
		m_nTexIdxType = pTexture->Register("data\\TEXTURE\\tutorial_menu.png");
		break;

	case UITYPE_MOVE:
		m_nTexIdxType = pTexture->Register("data\\TEXTURE\\karititle.png");
		break;

	default:
		break;
	}
}
