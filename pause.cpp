//====================================
//
// �|�[�Y���� [ pause.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "pause.h"

//================================
// �R���X�g���N�^
//================================
CPause::CPause(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
}
//================================
// �f�X�g���N�^
//================================
CPause::~CPause()
{
	// ����
}
//================================
// ��������
//================================
CPause* CPause::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col)
{
	// �C���X�^���X����
	CPause* pPause = new CPause;

	// nullptr��������
	if (pPause == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pPause->Init()))
	{
		return nullptr;
	}

	// 2D�I�u�W�F�N�g�ݒ�
	pPause->SetPos(pos);
	pPause->SetSize(fWidth, fHeight);
	pPause->SetCol(col);

	// �������ꂽ�|�C���^��Ԃ�
	return pPause;
}
//================================
// ����������
//================================
HRESULT CPause::Init(void)
{
	// �I�u�W�F�N�g�̏�����
	CObject2D::Init();

	// ���������ʂ�Ԃ�
	return S_OK;
}
//================================
// �I������
//================================
void CPause::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject2D::Uninit();
}
//================================
// �X�V����
//================================
void CPause::Update(void)
{
	// �I�u�W�F�N�g�̍X�V����
	CObject2D::Update();
}
//================================
// �`�揈��
//================================
void CPause::Draw(void)
{
	// �I�u�W�F�N�g�̕`�揈��
	CObject2D::Draw();
}
