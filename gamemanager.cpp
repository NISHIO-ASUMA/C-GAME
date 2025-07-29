//=========================================
//
// �Q�[���Ǘ����� [ gamemanager.cpp ]
// Author: Asuma Nishio
// 
//==========================================

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "gamemanager.h"
#include "manager.h"
#include "score.h"
#include "time.h"
#include "meshdome.h"
#include "meshfield.h"
#include "player.h"
#include "meshimpact.h"
#include "playerlifegage.h"
#include "debugproc.h"
#include "bosslifegage.h"

//**************************
// �ÓI�����o�ϐ��錾
//**************************
CBlock* CGameManager::m_pBlock = nullptr;				// �u���b�N
CMeshCylinder* CGameManager::m_pMeshCylinder = nullptr;	// �~��
CBoss* CGameManager::m_pBoss = nullptr;					// �{�X
CTime* CGameManager::m_pTime = nullptr;					// �^�C�}�[

//========================
// �R���X�g���N�^
//========================
CGameManager::CGameManager()
{
	// �l�̃N���A
}
//========================
// �f�X�g���N�^
//========================
CGameManager::~CGameManager()
{
	// ����
}
//========================
// ����������
//========================
HRESULT CGameManager::Init(void)
{
	// �X�R�A�̐���
	// CScore::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// �V�����_�[����
	m_pMeshCylinder = CMeshCylinder::Create(D3DXVECTOR3(0.0f, -20.0f, 0.0f), 550.0f);

	// �h�[������
	CMeshDome::Create(D3DXVECTOR3(0.0f, -70.0f, 0.0f), 800.0f);

	// �t�B�[���h����
	CMeshField::Create(D3DXVECTOR3(0.0f, -150.0f, 0.0f), 2000.0f);

	// �{�X����
	m_pBoss = CBoss::Create(D3DXVECTOR3(0.0f, -600.0f, 0.0f), 60.0f, 1000);

	// �v���C���[����
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -550.0f), VECTOR3_NULL, 10, 0, "data\\Player100motion.txt");
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 550.0f), VECTOR3_NULL, 10, 1, "data\\Player200motion.txt");

	// �u���b�N�z�u
	m_pBlock = CBlock::Create("data\\MODEL\\STAGEOBJ\\Field000.x", D3DXVECTOR3(0.0f, -90.0f, 0.0f), VECTOR3_NULL, 80.0f);

	// �̗̓Q�[�W����
	CPlayerLifeGage::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 450.0f, 40.0f, CPlayerLifeGage::GAGE_FRAME);
	CPlayerLifeGage::Create(D3DXVECTOR3(0.0f, 2.0f, 0.0f), 450.0f, 40.0f, CPlayerLifeGage::GAGE_BAR);

	// �{�X�̗̓Q�[�W����
	CBossLifeGage::Create(D3DXVECTOR3(770.0f, 0.0f, 0.0f), 0.0f, 0.0f, CBossLifeGage::TYPE_GAGE);
	CBossLifeGage::Create(D3DXVECTOR3(770.0f, 0.0f, 0.0f), SCREEN_WIDTH * 0.4f, 60.0f, CBossLifeGage::TYPE_FRAME);

	// �^�C�}�[����
	m_pTime = CTime::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//========================
// �I������
//========================
void CGameManager::Uninit(void)
{
	// �g�p�����|�C���^��null������
	m_pBlock = nullptr;
	m_pBoss = nullptr;
	m_pMeshCylinder = nullptr;
	m_pTime = nullptr;
}
//========================
// �X�V����
//========================
void CGameManager::Update(void)
{
	// �I�u�W�F�N�g�̍X�V�͎����I�ɌĂ΂��
}
