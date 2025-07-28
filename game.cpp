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
#include "score.h"
#include "time.h"
#include "meshdome.h"
#include "meshfield.h"
#include "player.h"
#include "meshimpact.h"
#include "debugproc.h"
#include "title.h"
#include "bullethorming.h"
#include "playerlifegage.h"
#include "enemy.h"
#include "result.h"

//**************************
// �ÓI�����o�ϐ��錾
//**************************
CBlock* CGame::m_pBlock = nullptr;				// �u���b�N
CMeshCylinder* CGame::m_pMeshCylinder = nullptr;	// �~��
CBoss* CGame::m_pBoss = nullptr;					// �{�X
CPauseManager* CGame::m_pPausemanager = nullptr;
CTime* CGame::m_pTime = nullptr;

//==================================
// �R���X�g���N�^
//==================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// �l�̃N���A
	m_bPause = false;
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

	// ����������
	m_pPausemanager->Init();

	// �X�R�A�̐���
	CScore::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// �^�C�}�[����
	m_pTime = CTime::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// �V�����_�[����
	m_pMeshCylinder = CMeshCylinder::Create(D3DXVECTOR3(0.0f, -20.0f, 0.0f), 550.0f);

	// �h�[������
	CMeshDome::Create(D3DXVECTOR3(0.0f, -70.0f, 0.0f), 800.0f);

	// �t�B�[���h����
	CMeshField::Create(D3DXVECTOR3(0.0f, -150.0f, 0.0f), 2000.0f);

	// �{�X����
	m_pBoss = CBoss::Create(D3DXVECTOR3(0.0f, -600.0f, 0.0f), 60.0f);

	// �v���C���[����
	CPlayer::Create(D3DXVECTOR3(0.0f,0.0f,-550.0f), VECTOR3_NULL, 10, 0, "data\\Player100motion.txt");

	// �v���C���[����
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 550.0f), VECTOR3_NULL, 10, 1, "data\\Player200motion.txt");

	// �u���b�N�z�u
	m_pBlock = CBlock::Create("data\\MODEL\\STAGEOBJ\\Field000.x", D3DXVECTOR3(0.0f, -90.0f, 0.0f), VECTOR3_NULL, 80.0f);

	// �̗̓Q�[�W����
	CPlayerLifeGage::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 450.0f, 40.0f, CPlayerLifeGage::GAGE_FRAME);
	CPlayerLifeGage::Create(D3DXVECTOR3(0.0f, 2.0f, 0.0f), 450.0f, 40.0f, CPlayerLifeGage::GAGE_BAR);

	// �G����
	CEnemy::Create("data\\MODEL\\ATTACKMODEL\\SpikeEnemy000.x", D3DXVECTOR3(0.0f, 0.0f, -550.0f), VECTOR3_NULL, CEnemy::TYPE_NONE, 2);

	return S_OK;
}
//==================================
// �I������
//==================================
void CGame::Uninit(void)
{
	m_pBlock = nullptr;
	m_pBoss = nullptr;
	m_pMeshCylinder = nullptr;
	m_pTime = nullptr;

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
}
//==================================
// �X�V����
//==================================
void CGame::Update(void)
{	
	// �L�[����
	m_pPausemanager->SetEnablePause();
	
	// �|�[�Y�̍X�V����
	m_pPausemanager->Update();
	
	if (m_pPausemanager->GetPause() == false)
	{
		// ���ؗp�C���p�N�g
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_H))
		{
			// �Ռ��g�𐶐�
			CMeshImpact::Create(VECTOR3_NULL, 80, 100.0f, 40.0f, 7.0f);
		}

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

		// ���ؗp�e
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_L))
		{
			// �z�[�~���O�e�𐶐�
			CBulletHorming::Create("data\\MODEL\\ATTACKMODEL\\bulletobject000.x", D3DXVECTOR3(0.0f, 400.0f, 0.0f));
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
