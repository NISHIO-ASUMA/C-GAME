//==============================================
//
// ���U���g�V�[���Ǘ����� [ resultmanager.cpp ]
// Author: Asuma Nishio
// 
//==============================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "resultmanager.h"
#include "manager.h"
#include "input.h"
#include "title.h"

//=================================
// �R���X�g���N�^
//=================================
CResultManager::CResultManager()
{
	// �l�̃N���A
}
//=================================
// �f�X�g���N�^
//=================================
CResultManager::~CResultManager()
{
	// ����
}
//=================================
// ����������
//=================================
HRESULT CResultManager::Init(void)
{
	// ���������ʂ�Ԃ�
	return S_OK;
}
//=================================
// �I������
//=================================
void CResultManager::Uninit(void)
{
	// null�`�F�b�N
}
//=================================
// �X�V����
//=================================
void CResultManager::Update(void)
{
	// ���̓f�o�C�X
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// ����L�[�������ꂽ
	if (pInput->GetTrigger(DIK_RETURN))
	{
		// �|�C���^�擾
		CFade* pFade = CManager::GetFade();

		// null����Ȃ��Ƃ�
		if (pFade != nullptr)
		{
			// �V�[���J��
			pFade->SetFade(new CTitle(false));
		}
	}
}
