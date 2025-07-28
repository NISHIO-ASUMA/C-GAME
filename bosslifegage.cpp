//=========================================
//
// �{�X�̗͏��� [ bosslifegage.h ]
// Author: Asuma Nishio
//
//=========================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "bosslifegage.h"

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CBossLifeGage::CBossLifeGage(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nIdxTex = NULL;
	m_pBoss = nullptr;
}
//===============================
// �f�X�g���N�^
//===============================
CBossLifeGage::~CBossLifeGage()
{
	// ����
}
//===============================
// ��������
//===============================
CBossLifeGage* CBossLifeGage::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	// �C���X�^���X����
	CBossLifeGage* pBossLife = new CBossLifeGage;

	// null��������
	if (pBossLife == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pBossLife->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pBossLife;
}
//===============================
// ����������
//===============================
HRESULT CBossLifeGage::Init(void)
{
	return S_OK;
}
//===============================
// �I������
//===============================
void CBossLifeGage::Uninit(void)
{

}
//===============================
// �X�V����
//===============================
void CBossLifeGage::Update(void)
{

}
//===============================
// �`�揈��
//===============================
void CBossLifeGage::Draw(void)
{

}
