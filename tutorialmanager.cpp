//=============================================
//
// �`���[�g���A���Ǘ����� [ tutorialmanager.h ]
// Author: Asuma Nishio
//
//=============================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "tutorialmanager.h"
#include "tutorialui.h"
#include "manager.h"
#include "fade.h"
#include "game.h"

//==========================
// �R���X�g���N�^
//==========================
CTutorialManager::CTutorialManager()
{
	// �l�̃N���A
	m_pTutoui = nullptr;
}
//==========================
// �f�X�g���N�^
//==========================
CTutorialManager::~CTutorialManager()
{
	// ����
}
//==========================
// ����������
//==========================
HRESULT CTutorialManager::Init(void)
{
	// ui����
	m_pTutoui = CTutorialUi::Create(D3DXVECTOR3(500.0f, 200.0f, 0.0f), 300.0f, 60.0f, 0);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==========================
// �I������
//==========================
void CTutorialManager::Uninit(void)
{
	// ����
}
//==========================
// �X�V����
//==========================
void CTutorialManager::Update(void)
{
	// ����L�[����
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		// �t�F�[�h�擾
		CFade* pFade = CManager::GetFade();

		if (pFade != nullptr) pFade->SetFade(new CGame());	// �Q�[���V�[���ɑJ��
	}

}
