//====================================
//
// �G�f�B�^�[���� [ uiedit.cpp ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "uiedit.h"
#include "input.h"
#include "manager.h"

//===============================
// �R���X�g���N�^
//===============================
CUiedit::CUiedit()
{

}
//===============================
// �f�X�g���N�^
//===============================
CUiedit::~CUiedit()
{

}
//===============================
// ����������
//===============================
HRESULT CUiedit::Init(void)
{
	return S_OK;
}
//===============================
// �I������
//===============================
void CUiedit::Uninit(void)
{
	// nullptr�`�F�b�N
	if ( != nullptr)
}
//===============================
// �X�V����
//===============================
void CUiedit::Update(void)
{
	// �L�[���͗p�̕ϐ�
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// �z�u�ݒ�
	if (pInput->GetTrigger(DIK_A))
	{

	}
	else if (pInput->GetTrigger(DIK_D))
	{

	}
	else if (pInput->GetTrigger(DIK_W))
	{

	}
	else if (pInput->GetTrigger(DIK_S))
	{

	}

}
//===============================
// �`�揈��
//===============================
void CUiedit::Draw(void)
{

}
