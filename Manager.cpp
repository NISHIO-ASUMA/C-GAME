//====================================
//
// �}�l�[�W���[���� [ Manager.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "Manager.h"
#include "object2D.h"
#include "enemy.h"
#include "score.h"
#include "time.h"
#include "objectX.h"
#include "billboard.h"
#include "block.h"
#include "meshdome.h"
#include "meshfield.h"
#include "player.h"

//**************************
// �ÓI�����o�ϐ��錾
//**************************
CRenderer* CManager::m_pRenderer = nullptr;				// �����_���[�ւ̃|�C���^
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;	// �L�[�{�[�h�ւ̃|�C���^
CJoyPad* CManager::m_pJoyPad = nullptr;					// �W���C�p�b�h�N���X�ւ̃|�C���^
CSound* CManager::m_pSound = nullptr;					// �T�E���h�ւ̃|�C���^
CInputMouse* CManager::m_pInputMouse = nullptr;			// �}�E�X�ւ̃|�C���^
CEnemymanager* CManager::m_pEnemyManager = nullptr;		// �G�}�l�[�W���[�ւ̃|�C���^
CTexture* CManager::m_pTexture = nullptr;				// �e�N�X�`���N���X�ւ̃|�C���^
CCamera* CManager::m_pCamera = nullptr;					// �J�����N���X�ւ̃|�C���^
CLight* CManager::m_pLight = nullptr;					// ���C�g�N���X�ւ̃|�C���^
CObject3D* CManager::m_pobj = nullptr;
CBlockManager* CManager::m_pBlockManager = nullptr;
CBlock* CManager::m_pBlock = nullptr;
CCollision* CManager::m_pCollision = nullptr;
CMeshCylinder* CManager::m_pMeshCylinder = nullptr;
CBoss* CManager::m_pBoss = nullptr;

//===========================
// �R���X�g���N�^
//===========================
CManager::CManager()
{
	// ����
}
//===========================
// �f�X�g���N�^
//===========================
CManager::~CManager()
{
	// ����
}
//===========================
// �}�l�[�W���[�̏���������
//===========================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �L�[�{�[�h�I�u�W�F�N�g�̐�������
	m_pInputKeyboard = new CInputKeyboard;

	// �������Ɏ��s������
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		// -1��Ԃ�
		return -1;
	}

	// �W���C�p�b�h�I�u�W�F�N�g�̐�������
	m_pJoyPad = new CJoyPad;

	// �������Ɏ��s������
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd)))
	{
		// -1��Ԃ�
		return -1;
	}

	// �}�E�X�I�u�W�F�N�g�̐�������
	m_pInputMouse = new CInputMouse;

	// �������Ɏ��s������
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		// -1��Ԃ�
		return -1;
	}

	// �T�E���h�̐�������
	m_pSound = new CSound;

	if (FAILED(m_pSound->Init(hWnd)))
	{
		// -1��Ԃ�
		return -1;
	}

	// �����_���[�̐�������
	m_pRenderer = new CRenderer;

	// �������Ɏ��s������
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		// -1��Ԃ�
		return -1;
	}

	// �J��������
	m_pCamera = new CCamera;

	// �������Ɏ��s������
	if (FAILED(m_pCamera->Init()))
	{
		// -1��Ԃ�
		return -1;
	}

	// ���C�g����
	m_pLight = new CLight;

	// �������Ɏ��s������
	if (FAILED(m_pLight->Init()))
	{
		// -1��Ԃ�
		return -1;
	}

	// �e�N�X�`������
	m_pTexture = new CTexture;

	// �e�N�X�`���S�ǂݍ���
	m_pTexture->Load();

	// �X�R�A�̐���
	CScore::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// �V�����_�[����
	m_pMeshCylinder = m_pMeshCylinder->Create(D3DXVECTOR3(0.0f, -20.0f, 0.0f), 550.0f);

	// �h�[������
	CMeshDome::Create(D3DXVECTOR3(0.0f,-70.0f,0.0f), 800.0f);

	// �t�B�[���h����
	CMeshField::Create(D3DXVECTOR3(0.0f, -150.0f, 0.0f), 2000.0f);

	// �{�X����
	m_pBoss = m_pBoss->Create(D3DXVECTOR3(0.0f, -600.0f, 0.0f),60.0f);

	// �v���C���[����
	CPlayer::Create(VECTOR3_NULL, VECTOR3_NULL, 20, 0, "data\\Player100motion.txt");

	// �v���C���[����
	CPlayer::Create(VECTOR3_NULL, VECTOR3_NULL, 20, 0, "data\\Player200motion.txt");

	// �u���b�N�z�u
	m_pBlock = m_pBlock->Create("data\\MODEL\\STAGEOBJ\\Field000.x", D3DXVECTOR3(0.0f, -90.0f, 0.0f), VECTOR3_NULL,80.0f);

	return S_OK;
}
//===========================
// �}�l�[�W���[�̏I������
//===========================
void CManager::Uninit(void)
{
	// �S�I�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	// NULL�`�F�b�N
	if (m_pEnemyManager != nullptr)
	{
		// �G�}�l�[�W���[�̏I������
		m_pEnemyManager->Uninit();

		// �G�}�l�[�W���[�̔j��
		delete m_pEnemyManager;

		// NULL�ɂ���
		m_pEnemyManager = nullptr;
	}

	// NULL�`�F�b�N
	if (m_pInputKeyboard != nullptr)
	{
		// �L�[�{�[�h�̏I������
		m_pInputKeyboard->Uninit();
		
		// �L�[�{�[�h�̔j��
		delete m_pInputKeyboard;

		// NULL�ɂ���
		m_pInputKeyboard = nullptr;
	}

	// NULL�`�F�b�N
	if (m_pJoyPad != nullptr)
	{
		// �W���C�p�b�h�̏I������
		m_pJoyPad->Uninit();
		
		// �W���C�p�b�h�̔j��
		delete m_pJoyPad;

		// NULL�ɂ���
		m_pJoyPad = nullptr;
	}

	// NULL�`�F�b�N
	if (m_pInputMouse != nullptr)
	{
		// �}�E�X�̏I������
		m_pInputMouse->Uninit();

		// �}�E�X�̊�
		delete m_pInputMouse;

		// NULL�ɂ���
		m_pInputMouse = nullptr;
	}


	// NULL�`�F�b�N
	if (m_pSound != nullptr)
	{
		// �T�E���h�̏I������
		m_pSound->Uninit();

		// �T�E���h�̔j��
		delete m_pSound;

		// NULL�ɂ���
		m_pSound = nullptr;
	}

	// �J�����̔j��
	if (m_pCamera != nullptr)
	{
		// �I������
		m_pCamera->Uninit();

		// �j��
		delete m_pCamera;

		// NULL�ɂ���
		m_pCamera = nullptr;

	}

	//  ���C�g�̔j��
	if (m_pLight != nullptr)
	{
		// �I������
		m_pLight->Uninit();

		// �j��
		delete m_pLight;

		// NULL�ɂ���
		m_pLight = nullptr;

	}

	// �e�N�X�`���j��
	if (m_pTexture != nullptr)
	{
		// ���ׂẴe�N�X�`���̔j��
		m_pTexture->UnLoad();

		// �|�C���^�̔j��
		delete m_pTexture;

		// nullptr���
		m_pTexture = nullptr;
	}

	// NULL�`�F�b�N
	if (m_pRenderer != nullptr)
	{
		// �����_���[�̏I������
		m_pRenderer->Uninit();

		// �����_���[�̔j��
		delete m_pRenderer;

		// NULL�ɂ���
		m_pRenderer = nullptr;
	}
}
//===========================
// �}�l�[�W���[�̍X�V����
//===========================
void CManager::Update()
{
	// �L�[�{�[�h�̍X�V����
	m_pInputKeyboard->Update();

	// �W���C�p�b�h�̍X�V����
	m_pJoyPad->Update();

	// �}�E�X�̍X�V����
	m_pInputMouse->Update();

	//�J�����X�V
	m_pCamera->Update();

	// �����_���[�̍X�V����
	m_pRenderer->Update();

#ifdef _DEBUG
	// �u���b�N�}�l�[�W���[�X�V
	m_pBlockManager->Update();
#endif
}
//===========================
// �}�l�[�W���[�̕`�揈��
//===========================
void CManager::Draw(void)
{
	// �����_���[�̕`�揈��
	m_pRenderer->Draw();
}
//===========================
// �����_���[�̎擾
//===========================
CRenderer* CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//===========================
// �L�[�{�[�h�̎擾
//===========================
CInputKeyboard* CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//===========================
// �W���C�p�b�h�̎擾
//===========================
CJoyPad* CManager::GetJoyPad(void)
{
	return m_pJoyPad;
}
//===========================
// �T�E���h�̎擾
//===========================
CSound* CManager::GetSound(void)
{
	return m_pSound;
}
//===========================
// �}�E�X�̎擾
//===========================
CInputMouse* CManager::GetMouse(void)
{
	return m_pInputMouse;
}
//===========================
// �G�}�l�[�W���[�̎擾
//===========================
CEnemymanager* CManager::GetEnemyManager(void)
{
	return m_pEnemyManager;
}
//===========================
// �e�N�X�`���̎擾
//===========================
CTexture* CManager::GetTexture(void)
{
	return m_pTexture;
}
//===========================
// �J�����̎擾
//===========================
CCamera* CManager::GetCamera(void)
{
	return m_pCamera;
}
//===========================
// ���C�g�̎擾
//===========================
CLight* CManager::GetLight(void)
{
	return m_pLight;
}
//===========================
// 3D�I�u�W�F�N�g�̎擾
//===========================
CObject3D* CManager::GetObj3D(void)
{
	return m_pobj;
}