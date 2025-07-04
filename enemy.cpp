//====================================
//
// �G�̏��� [ enemy.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "enemy.h"
#include "Manager.h"
#include "debugproc.h"
#include "bullet.h"
#include "particle.h"
#include "score.h"

//**********************
// �}�N����`
//**********************
#define ENEMY_SIZE_X (40.0f)			// �G�̉���
#define ENEMY_SIZE_Y (40.0f)			// �G�̏c��
#define DAMAGE_COL (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)) // �_���[�W�F

//***********************************
// �ÓI�����o�ϐ��錾
//***********************************
int CEnemy::m_NumEnemy = 0;			// �G�̑���

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CEnemy::CEnemy(int nPriority)
{
	// �l�̃N���A
	m_move = VECTOR3_NULL;
	m_nInterval = 0;
	m_StateCount = 0;
	m_State = ENEMYSTATE_NONE;
	m_nIdxTexture = NULL;
}
//===============================
// �f�X�g���N�^
//===============================
CEnemy::~CEnemy()
{
	// �G�̐������炷
}
//===============================
// �G�̐���
//===============================
CEnemy* CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,ENEMYTYPE nType)
{
	// �G�̃C���X�^���X����
	CEnemy* pEnemy = nullptr;

	// �������m��
	pEnemy = new CEnemy;

	// �e�N�X�`���Z�b�g
	pEnemy->SetTexture(nType);

	// �������Ɏ��s������
	if (FAILED(pEnemy->Init()))
	{
		// �j��
		delete pEnemy;

		// NULL��Ԃ�
		return nullptr;
	}

	// 2D�I�u�W�F�N�g�ݒ�
	pEnemy->SetSize(ENEMY_SIZE_X, ENEMY_SIZE_Y);

	pEnemy->SetRot(rot);
	pEnemy->SetPos(pos);

	// �G�̎�ރZ�b�g
	pEnemy->SetType(nType);

	// �G���J�E���g���C���N�������g
	m_NumEnemy++;

	// �|�C���^��Ԃ�
	return pEnemy;
}
//===============================
// �G�̎�ރZ�b�g
//===============================
void CEnemy::SetType(const ENEMYTYPE type)
{
	// ��ޑ��
	m_Type = type;
}
//===============================
// �G�̏�����
//===============================
HRESULT CEnemy::Init(void)
{
	// �e�N���X�̏�����
	CObject2D::Init();

	// ��ނ��Z�b�g
	SetObjType(TYPE_ENEMY);

	// �擾
	D3DXVECTOR3 rot = GetRot();

	// �����o�ϐ��̏�����
	m_move = D3DXVECTOR3(sinf(rot.z + D3DX_PI) * 20.0f, cosf(rot.z + D3DX_PI) * 20.0f, 0.0f);
	m_nInterval = 0;
	m_State = ENEMYSTATE_NORMAL;

	return S_OK;
}
//===============================
// �G�̏I��
//===============================
void CEnemy::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject2D::Uninit();

	// �f�N�������g
	m_NumEnemy--;
}
//===============================
// �G�̍X�V
//===============================
void CEnemy::Update(void)
{
	// 2D�I�u�W�F�N�g�X�V
	CObject2D::Update();

#if 0
	// ���W�̎擾
	D3DXVECTOR3 EnemyPos = GetPos();

	// �p�x�̎擾
	D3DXVECTOR3 EnemyRot = GetRot();

	D3DXCOLOR col = GetCol();

	// ���W���X�V
	SetRot(EnemyRot);
	SetPos(EnemyPos);

	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ��ʊO
	if (EnemyPos.x >= SCREEN_WIDTH ||
		EnemyPos.y >= SCREEN_HEIGHT ||
		EnemyPos.y <= 0.0f ||
		EnemyPos.x <= 0.0f)
	{
		// �j������
		Uninit();
	}

	// �C���^�[�o���J�E���g�����Z
	m_nInterval++;

	// �J�E���g������𒴂�����
	if (m_nInterval > 30)
	{
		// �e�̐���
		CBullet::Create(EnemyPos, D3DXVECTOR3(0.0f,0.0f,D3DX_PI),CBullet::BTYPE_ENEMY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		
		// �����l�ɖ߂�
		m_nInterval = 0;
	}

	// ��ԊǗ�
	switch (m_State)
	{
	case ENEMYSTATE_NONE:
		break;

	case ENEMYSTATE_NORMAL:
		break;

	case ENEMYSTATE_DAMAGE:

		// ��ԊǗ��J�E���^�[���f�N�������g
		m_StateCount--;

		// �J�E���^�[��0�ȉ�
		if (m_StateCount <= 0)
		{
			// �G��ԕύX
			m_State = ENEMYSTATE_NORMAL;

			// ���_�J���[�̐ݒ�
			SetCol(col);
		}
		break;
	default:
		break;
	}
#endif
}
//===============================
// �G�̕`��
//===============================
void CEnemy::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���ǂݍ���
	CTexture* pTexture = CManager::GetTexture();

	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// �I�u�W�F�N�g�̕`��
	CObject2D::Draw();
}
//===============================
// �G�̃_���[�W����
//===============================
void CEnemy::HitEnemy(int nDamage)
{
#if 0
	// ��Ԃ𔻕�
	if (m_State == ENEMYSTATE_DAMAGE)
	{
		// �����������ŕԂ�
		return;
	}

	// ���W���擾
	D3DXVECTOR3 pos = GetPos();

	// �̗͂����炷
	nLife -= nDamage;

	// ��ԏ���
	if (nLife <= 0)
	{
		// �p�[�e�B�N������
		switch (m_Type)
		{
		case CEnemy::ENEMYTYPE_FACE:
			// �p�[�e�B�N������
			CParticle::Create(pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 15, 3.0f);
			// �X�R�A�����Z
			CScore::AddScore(10000);
			break;

		case CEnemy::ENEMYTYPE_GOLD:
			// �p�[�e�B�N������
			CParticle::Create(pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 15, 3.0f);
			// �X�R�A�����Z
			CScore::AddScore(10000);
			break;

		default:
			break;
		}

		// �j������
		Uninit();
	}
	else
	{
		m_State = ENEMYSTATE_DAMAGE;	//�_���[�W���
		m_StateCount = 5;				//�_���[�W��Ԃ̎���

		// ���_�J���[�̐ݒ�(�_���[�W�F)
		SetCol(DAMAGE_COL);
	}

	// ��ԍŌ�Ɍ��݂̗̑͂��Z�b�g����
	SetLife(nLife);
#endif
}
//===============================
// �e�N�X�`���ݒ�
//===============================
void CEnemy::SetTexture(ENEMYTYPE Type)
{
	// �e�N�X�`���|�C���^�擾
	CTexture* pTexture = CManager::GetTexture();

	switch (Type)
	{
	case ENEMYTYPE_FACE:
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\face000.png");
		break;

	case ENEMYTYPE_GOLD:
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\gold.jpg");
		break;

	default:
		break;
	}
}
