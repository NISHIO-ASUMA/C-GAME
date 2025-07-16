//====================================
//
// �}�l�[�W���[���� [ manager.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "manager.h"
#include "score.h"
#include "time.h"
#include "objectX.h"
#include "billboard.h"
#include "block.h"
#include "meshdome.h"
#include "meshfield.h"
#include "player.h"
#include "meshimpact.h"
#include "gage.h"
#include "ui.h"
#include "title.h"

//**************************
// �ÓI�����o�ϐ��錾
//**************************
CRenderer* CManager::m_pRenderer = nullptr;				// �����_���[�ւ̃|�C���^
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;	// �L�[�{�[�h�ւ̃|�C���^
CJoyPad* CManager::m_pJoyPad = nullptr;					// �W���C�p�b�h�N���X�ւ̃|�C���^
CSound* CManager::m_pSound = nullptr;					// �T�E���h�ւ̃|�C���^
CInputMouse* CManager::m_pInputMouse = nullptr;			// �}�E�X�ւ̃|�C���^
CTexture* CManager::m_pTexture = nullptr;				// �e�N�X�`���N���X�ւ̃|�C���^
CCamera* CManager::m_pCamera = nullptr;					// �J�����N���X�ւ̃|�C���^
CLight* CManager::m_pLight = nullptr;					// ���C�g�N���X�ւ̃|�C���^
CScene* CManager::m_pScene = nullptr;

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

	// �V�[���Z�b�g
	SetScene(new CTitle());

	//// �X�R�A�̐���
	//CScore::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	//// �V�����_�[����
	//m_pMeshCylinder = CMeshCylinder::Create(D3DXVECTOR3(0.0f, -20.0f, 0.0f), 550.0f);

	//// �h�[������
	//CMeshDome::Create(D3DXVECTOR3(0.0f,-70.0f,0.0f), 800.0f);

	//// �t�B�[���h����
	//CMeshField::Create(D3DXVECTOR3(0.0f, -150.0f, 0.0f), 2000.0f);

	//// �{�X����
	//m_pBoss = CBoss::Create(D3DXVECTOR3(0.0f, -600.0f, 0.0f),60.0f);

	//// �v���C���[����
	//CPlayer::Create(VECTOR3_NULL, VECTOR3_NULL, 10, 0, "data\\Player100motion.txt");

	//// �v���C���[����
	//CPlayer::Create(VECTOR3_NULL, VECTOR3_NULL, 10, 1, "data\\Player200motion.txt");

	//// �u���b�N�z�u
	//m_pBlock = CBlock::Create("data\\MODEL\\STAGEOBJ\\Field000.x", D3DXVECTOR3(0.0f, -90.0f, 0.0f), VECTOR3_NULL,80.0f);

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

	// nullptr����Ȃ�
	if (m_pScene)
	{
		// �I������
		m_pScene->Uninit();

		// �|�C���^�̔j��
		delete m_pScene;

		// nullptr�ɂ���
		m_pScene = nullptr;
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

	// �J�����X�V
	m_pCamera->Update();

	// �V�[���̍X�V
	m_pScene->Update();

	// �����_���[�̍X�V����
	m_pRenderer->Update();

	// TODO : ���ؗp�C���p�N�g
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_H))
	{
		// �Ռ��g�𐶐�
		CMeshImpact::Create(VECTOR3_NULL, 80, 100.0f, 40.0f, 7.0f);
	}
}
//===========================
// �}�l�[�W���[�̕`�揈��
//===========================
void CManager::Draw(void)
{
	// �V�[���̕`��
	m_pScene->Draw();

	// �����_���[�̕`�揈��
	m_pRenderer->Draw();
}

//===========================
// �V�[���̃Z�b�g
//===========================
void CManager::SetScene(CScene * pNewscene)
{// TODO : �����Ƀt�F�[�h���������邩��

	// null����Ȃ�
	if (m_pSound)
	{
		// �T�E���h�̒�~
		m_pSound->StopSound();
	}

	// nullptr����Ȃ�
	if (m_pScene)
	{
		// �I������
		m_pScene->Uninit();

		// �|�C���^�̔j��
		delete m_pScene;

		// nullptr�ɂ���
		m_pScene = nullptr;
	}
	
	// �S�I�u�W�F�N�g�j��
	CObject::ReleaseAll();

	// �V�����V�[�����Z�b�g����
	m_pScene = pNewscene;

	// �������s��
	if (m_pScene == nullptr)
	{
		MessageBox(GetActiveWindow(), "�V�[���������s (Cmanager::SetMode)", "��O�X���[", MB_OK);

		// �E�B���h�E�I��
		PostQuitMessage(0);

		return;
	}

	// �V�[���̏�����
	if (FAILED(m_pScene->Init()))
	{
		// ���s��
		MessageBox(GetActiveWindow(), "�V�[�����������s", "��O�X���[", MB_OK);

		// �j��
		delete m_pScene;

		// nullptr���
		m_pScene = nullptr;
	}
}
//===========================
// ���݃V�[���擾
//===========================
CScene::MODE CManager::GetScene(void)
{
	// nullptr����Ȃ�
	if (m_pScene)
		return m_pScene->GetScene(); // ���݃V�[��
	else
		return CScene::MODE_NONE;	 // �����Ȃ��V�[��
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
