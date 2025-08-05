//=====================================
//
// �U���O�����o [ attacksign.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "attacksign.h"
#include "manager.h"
#include "texture.h"

//==============================
// �R���X�g���N�^
//==============================
CAttackSign::CAttackSign(int nPriority) : CBillboard(nPriority)
{
	// �l�̃N���A
	m_nidxTex = NULL;
}
//==============================
// �f�X�g���N�^
//==============================
CAttackSign::~CAttackSign()
{
	// ����
}
//==============================
// ����������
//==============================
HRESULT CAttackSign::Init(void)
{
	// �e�N���X�̏���������
	CBillboard::Init();

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==============================
// �I������
//==============================
void CAttackSign::Uninit(void)
{
	// �e�N���X�̏I������
	CBillboard::Uninit();
}
//==============================
// �X�V����
//==============================
void CAttackSign::Update(void)
{
	// �I�u�W�F�N�g�X�V
	CBillboard::Update();
}
//==============================
// �`�揈��
//==============================
void CAttackSign::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���擾
	CTexture* pTexture = CManager::GetTexture();

	// null�`�F�b�N
	if (pTexture == nullptr) return;

	// �e�N�X�`���Z�b�g
	pDevice->SetTexture(0, pTexture->GetAddress(m_nidxTex));

	// �I�u�W�F�N�g�`��
	CBillboard::Draw();
}
//==============================
// �e�N�X�`�����蓖��
//==============================
void CAttackSign::SetTexture(void)
{
	// �e�N�X�`���|�C���^�擾
	CTexture* pTexture = CManager::GetTexture();

	// null�`�F�b�N
	if (pTexture == nullptr) return;

	// �e�N�X�`�����肠��
	// m_nidxTex = pTexture->Register();
}
//==============================
// ��������
//==============================
CAttackSign* CAttackSign::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// �C���X�^���X����
	CAttackSign* pAttack = new CAttackSign;

	// null�`�F�b�N
	if (pAttack == nullptr) return nullptr;

	// �I�u�W�F�N�g�ݒ�
	pAttack->SetPos(pos);
	pAttack->SetSize(fWidth, fHeight);
	pAttack->SetTexture();

	// ���������s��
	if (FAILED(pAttack->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pAttack;
}