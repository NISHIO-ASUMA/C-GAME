//====================================
//
// �{�X���� [ boss.cpp ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "boss.h"
#include "motion.h"
#include "debugproc.h"
#include "Manager.h"
#include <ctime>

//====================================
// �I�[�o�[���[�h�R���X�g���N�^
//====================================
CBoss::CBoss(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_pMotion = nullptr;
	m_type = NULL;
	m_mtxworld = {};

	for (int nCnt = 0; nCnt < m_nNumModels; nCnt++)
	{
		m_pModel[nCnt] = nullptr;
	}

	m_fSize = NULL;
}
//====================================
// �f�X�g���N�^
//====================================
CBoss::~CBoss()
{
	// ����
}
//====================================
// ��������
//====================================
CBoss* CBoss::Create(D3DXVECTOR3 pos,float fSize)
{
	// �C���X�^���X����
	CBoss* pBoss = new CBoss;

	// �Z�b�g
	pBoss->m_pos = pos;
	pBoss->m_fSize = fSize;

	// ���������s��
	if (FAILED(pBoss->Init()))
	{
		// �j��
		delete pBoss;

		// nullptr���
		pBoss = nullptr;

		// null�|�C���^��Ԃ�
		return nullptr;
	}

	// �|�C���^��Ԃ�
	return pBoss;
}
//====================================
// ����������
//====================================
HRESULT CBoss::Init(void)
{
	// �I�u�W�F�N�g�̎�ނ��Z�b�g
	SetObjType(TYPE_BOSS);

	// �^�C�v���
	m_type = TYPE_MAX;

	// ���[�V�����̓ǂݍ���
	m_pMotion = CMotion::Load("data\\MOTION\\Boss\\Boss_motion.txt", m_nNumModels, m_pModel,TYPE_MAX);

	// ���[�V��������ݒ�
	m_pMotion->SetMotionNum(m_type);

	return S_OK;
}
//====================================
// �I������
//====================================
void CBoss::Uninit(void)
{
	// ���f�������̔j��
	for (int nCnt = 0; nCnt < m_nNumModels; nCnt++)
	{
		// nullptr�`�F�b�N
		if (m_pModel[nCnt] != nullptr)
		{
			// �I������
			m_pModel[nCnt]->Uninit();

			// �|�C���^�̔j��
			delete m_pModel[nCnt];

			// nullptr�ɂ���
			m_pModel[nCnt] = nullptr;
		}
	}

	// nullptr�`�F�b�N
	if (m_pMotion != nullptr)
	{
		// �|�C���^�̔j��
		delete m_pMotion;

		// nullptr�ɂ���
		m_pMotion = nullptr;
	}

	// ���g�̔j��
	CObject::Release();
}
//====================================
// �X�V����
//====================================
void CBoss::Update(void)
{
	// �����̎��ݒ�
	srand((int)time(NULL));

	// �U���p�^�[�����������_���ݒ�
	int nAttackPattern = rand() % 3;

	// ���[�V�����Z�b�g
	m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);

	// ���[�V�����̍X�V
	m_pMotion->Update(m_pModel, m_nNumModels);
}
//====================================
// �`�揈��
//====================================
void CBoss::Draw(void)
{
#if 1
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v�Z�p�̃}�g���b�N�X��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxworld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// �S���f���p�[�c�̕`��
	for (int nCnt = 0; nCnt < m_nNumModels; nCnt++)
	{
		// �S���f���`��
		m_pModel[nCnt]->Draw();
	}
#endif
	// �f�o�b�O�t�H���g
	CDebugproc::Print("�{�X���W [ %.2f ,%.2f , %.2f]", m_pos.x,m_pos.y,m_pos.z);
	CDebugproc::Draw(0, 40);
}
