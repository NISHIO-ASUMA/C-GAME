//====================================
//
// ���C���Q�[������ [ game.h ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "game.h"
#include "manager.h"
#include "debugproc.h"
#include "title.h"
#include "result.h"
#include "gamemanager.h"

//**************************
// �ÓI�����o�ϐ��錾
//**************************
CPauseManager* CGame::m_pPausemanager = nullptr;

//==================================
// �R���X�g���N�^
//==================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// �l�̃N���A
	m_pGameManager = nullptr;
}
//==================================
// �f�X�g���N�^
//==================================
CGame::~CGame()
{
	// ����
}
//==================================
// ����������
//==================================
HRESULT CGame::Init(void)
{
	// �|�[�Y�}�l�[�W���[����
	m_pPausemanager = new CPauseManager;

	// null��������
	if (m_pPausemanager == nullptr)
	{
		return E_FAIL;
	}

	// �|�[�Y�}�l�[�W���[����������
	m_pPausemanager->Init();

	// �Q�[���}�l�[�W���[����
	m_pGameManager = new CGameManager;

	// null��������
	if (m_pGameManager == nullptr)
	{
		return E_FAIL;
	}

	// �Q�[���}�l�[�W���[����������
	m_pGameManager->Init();

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==================================
// �I������
//==================================
void CGame::Uninit(void)
{
	// null�`�F�b�N
	if (m_pPausemanager != nullptr)
	{
		// �I������
		m_pPausemanager->Uninit();

		// �|�C���^�̔j��
		delete m_pPausemanager;

		// nullptr�ɂ���
		m_pPausemanager = nullptr;
	}

	// null�`�F�b�N
	if (m_pGameManager != nullptr)
	{
		// �I������
		m_pGameManager->Uninit();

		// �|�C���^�̔j��
		delete m_pGameManager;

		// nullptr�ɂ���
		m_pGameManager = nullptr;
	}
}
//==================================
// �X�V����
//==================================
void CGame::Update(void)
{	
	// �|�[�Y�̃L�[���͔���
	m_pPausemanager->SetEnablePause();
	
	// �|�[�Y�̍X�V����
	m_pPausemanager->Update();
	
	// false�̎��ɍX�V
	if (m_pPausemanager->GetPause() == false)
	{
		// �Q�[���}�l�[�W���[�X�V
		m_pGameManager->Update();

		// ���ؗp��ʑJ��
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
		{
			// �t�F�[�h�擾
			CFade* pFade = CManager::GetFade();

			// �擾�ł�����
			if (pFade != nullptr)
			{
				// ��ʑJ��
				pFade->SetFade(new CResult());
			}
		}

		//// �o�ߎ��Ԃ��擾
		//int Numtime = m_pTime->GetAllTime();

		//if (Numtime <= 0)
		//{
		//	// �t�F�[�h�擾
		//	CFade* pFade = CManager::GetFade();

		//	if (pFade != nullptr)
		//	{
		//		// ���U���g�V�[���ɑJ��
		//		pFade->SetFade(new CResult());
		// 
		//		return;
		//	}
		//}
	}
}
//==================================
// �`�揈��
//==================================
void CGame::Draw(void)
{
	// ����
}
//==================================
// �R���X�g���N�^
//==================================
CGame* CGame::Create(void)
{	
	// �C���X�^���X����
	CGame* pGame = new CGame;

	// nullptr��������
	if (pGame == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pGame->Init()))
	{
		// �|�C���^��Ԃ�
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
    return pGame;
}
