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
#include "manager.h"
#include "parameter.h"
#include "result.h"
#include <ctime>

//****************************
// �萔�錾
//****************************
constexpr float HITRANGE = 12.0f; // �R���W�����T�C�Y
constexpr int COOLTIME = 60;	  // �����N�[���^�C��

//====================================
// �I�[�o�[���[�h�R���X�g���N�^
//====================================
CBoss::CBoss(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;

	m_pMotion = nullptr;
	m_pParam = nullptr;

	m_type = NULL;
	m_nCoolTime = NULL;
	m_mtxworld = {};

	for (int nCnt = 0; nCnt < NUMMODELS; nCnt++)
	{
		m_pModel[nCnt] = nullptr;
	}

	m_fSize = NULL;
	m_isAttacked = false;
	m_isdaeth = false;

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
CBoss* CBoss::Create(D3DXVECTOR3 pos,float fSize,int nLife)
{
	// �C���X�^���X����
	CBoss* pBoss = new CBoss;

	// �������s��
	if (pBoss == nullptr) return nullptr;

	// �I�u�W�F�N�g�Z�b�g
	pBoss->m_pos = pos;
	pBoss->m_fSize = fSize;

	// ���������s��
	if (FAILED(pBoss->Init()))
	{
		// null�|�C���^��Ԃ�
		return nullptr;
	}

	// �p�����[�^�[�ݒ�
	pBoss->m_pParam = new CParameter;

	// null����Ȃ�������
	if (pBoss->m_pParam != nullptr) pBoss->m_pParam->SetHp(nLife);

	// �|�C���^��Ԃ�
	return pBoss;
}
//=========================================
// ���f���̓��蕔���p�[�c�̎擾�֐�
//=========================================
CModel* CBoss::GetModelPartType(CModel::PARTTYPE modelpart)
{
	// �{�X�������Ă��郂�f���̐��̒�����T��
	for (int nModel = 0; nModel < NUMMODELS; nModel++)
	{
		// ���f�������� ���� �擾���������ƈ�v���Ă�����
		if (m_pModel[nModel] && m_pModel[nModel]->GetPartType() == modelpart)
		{
			// �Y�����f����idx�ԍ��̃��f����Ԃ�
			return m_pModel[nModel];
		}
	}

	// �Y���Ȃ��̏ꍇ
	return nullptr;
}
//====================================
// ����������
//====================================
HRESULT CBoss::Init(void)
{
	// �I�u�W�F�N�g�̎�ނ��Z�b�g
	SetObjType(TYPE_BOSS);

	// �^�C�v���
	m_type = CBoss::TYPE_MAX;

	// �����N�[���^�C����ݒ�
	m_nCoolTime = COOLTIME;

	// ���[�V�����̓ǂݍ���
	m_pMotion = CMotion::Load("data\\Boss\\Bossmotion.txt", NUMMODELS, m_pModel, CBoss::TYPE_MAX);

	// ���[�V��������ݒ�
	m_pMotion->SetMotionNum(m_type);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//====================================
// �I������
//====================================
void CBoss::Uninit(void)
{
	// ���f�������̔j��
	for (int nCnt = 0; nCnt < NUMMODELS; nCnt++)
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

	// nullptr�`�F�b�N
	if (m_pParam != nullptr)
	{
		// �|�C���^�̔j��
		delete m_pParam;

		// nullptr�ɂ���
		m_pParam = nullptr;
	}

	m_isdaeth = true;

	// ���g�̔j��
	CObject::Release();
}
//====================================
// �X�V����
//====================================
void CBoss::Update(void)
{
	// ����ł�����
	if (m_isdaeth) return;

	// �t���O�����g
	static bool isCreating = false;

	if (!isCreating)
	{
		// �����̎����x�����ݒ肷��
		srand((int)time(NULL));
		isCreating = true;
	}

	// �N�[���^�C�����Ȃ�ҋ@���[�V�����X�V����
	if (m_nCoolTime > 0)
	{
		// �N�[���^�C�������炷
		m_nCoolTime--;

		// �������ݍU�����[�V�������I����Ă���΃j���[�g�����ɖ߂�
		if (m_pMotion->GetFinishMotion())
		{
			m_pMotion->SetMotion(TYPE_NEUTRAL);
		}

		// ���[�V�����̍X�V�����s��
		m_pMotion->Update(m_pModel, NUMMODELS);

		// �����ŏ�����Ԃ�
		return;
	}

	// ���[�V������������
	if (!m_pMotion->GetFinishMotion() && m_pMotion->GetMotionType() != CBoss::PATTERN_NONE)
	{
		// �U�����[�V�������Ȃ̂ŁA������
		m_pMotion->Update(m_pModel, NUMMODELS);

		// �����ł�����
		return;
	}

	// �����_���ɍs���p�^�[�������肷��
	int nAttackPattern = rand() % PATTERN_MAX - 1;

	// ���l�ɂ���čs���ω�
	switch (nAttackPattern)
	{
	case PATTERN_NONE:
		m_pMotion->SetMotion(TYPE_NEUTRAL);
		break;

	case PATTERN_HAND:
		m_pMotion->SetMotion(TYPE_ACTION);
		m_nCoolTime = 120;
		break;

	case PATTERN_BULLET:
		// �e�ǔ����[�V����
		m_nCoolTime = 180;
		break;

	case PATTERN_CIRCLE:
		// �ガ�������[�V����
		m_nCoolTime = 150;
		break;

	default:
		break;
	}

	// ���[�V�����S�̍X�V
	m_pMotion->Update(m_pModel, NUMMODELS);
}
//====================================
// �`�揈��
//====================================
void CBoss::Draw(void)
{
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
	for (int nCnt = 0; nCnt < NUMMODELS; nCnt++)
	{
		// �S���f���`��
		m_pModel[nCnt]->Draw();
	}

	// �f�o�b�O�t�H���g
	CDebugproc::Print("�{�X���W [ %.2f ,%.2f , %.2f]", m_pos.x,m_pos.y,m_pos.z);
	CDebugproc::Draw(0, 40);

	CDebugproc::Print("�{�X���[�V������ { %d }", m_type);
	CDebugproc::Draw(0, 180);

	CDebugproc::Print("�{�X�E����W { %.2f,%.2f,%.2f }", GetModelPartType(CModel::PARTTYPE_RIGHT_HAND)->GetMtxWorld()._41, GetModelPartType(CModel::PARTTYPE_RIGHT_HAND)->GetMtxWorld()._42, GetModelPartType(CModel::PARTTYPE_RIGHT_HAND)->GetMtxWorld()._43);
	CDebugproc::Draw(0, 300);

	CDebugproc::Print("�{�X�̗� { %d }",m_pParam->GetHp());
	CDebugproc::Draw(0, 400);

	// �f�o�b�O�t�H���g
	m_pMotion->Debug();
}
//====================================
// �E��ƃv���C���[�̓����蔻��
//====================================
bool CBoss::CollisionRightHand(D3DXVECTOR3* pPos)
{
	if (m_isdaeth) return false;

	// ���t���[����
	if (m_pMotion->CheckFrame(100, 150, PATTERN_HAND))
	{
		// ���f���̃p�[�c�擾
		CModel* pRightHand = GetModelPartType(CModel::PARTTYPE_RIGHT_HAND); // �E��

		// null��������
		if (!pRightHand) return false;

		// �E��̃��[���h�}�g���b�N�X���擾
		D3DXMATRIX mtxWorld = pRightHand->GetMtxWorld();

		// �����v�Z
		D3DXVECTOR3 diff = *pPos - D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

		// �v�Z���������̒����擾
		float fDist = D3DXVec3Length(&diff);

		// ������Ԃ�
		return fDist <= (HITRANGE * HITRANGE);
	}
	else
	{
		// ������Ȃ��Ƃ�
		return false;
	}
}
//====================================
// �q�b�g����
//====================================
void CBoss::Hit(int nDamage)
{
	// �̗͂��擾
	int nHp = m_pParam->GetHp();

	// ���ݑ̗͂����炷
	nHp -= nDamage;

	if (nHp <= 0)
	{
		// ���S����
		m_isdaeth = true;

		// �{�X��j��
		Uninit();
	}
	else
	{
		// �Z�b�g����
		m_pParam->SetHp(nHp);
	}
}
