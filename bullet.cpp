//====================================
//
// �e�̏��� [ bullet.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "bullet.h"
#include "Manager.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"

//***********************************
// �}�N����`
//***********************************
#define BULLET_SIZE (30.0f) // �e�̃T�C�Y
#define BULLET_DAMAGE (1)	// �e�̃_���[�W

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CBullet::CBullet(int nPriority) : CBillboard(nPriority)
{
	// �l�̃N���A
	m_nLife = 0;
	m_move = VECTOR3_NULL;
	m_Type = BTYPE_NONE;
	m_nPriority = nPriority;
	m_nIdxTexture = NULL;
	m_pTarget = nullptr;
	m_isHoming = false;
}
//===============================
// �f�X�g���N�^
//===============================
CBullet::~CBullet()
{
	// ����
}
//===============================
// �e�̐���
//===============================
CBullet* CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, BTYPE nType, const float fWidth, const float fHeight,const int nLife)
{
	// �ő吔
	int nNum = CObject::GetNumAll();

	// �ő吔��������
	if (nNum >= MAX_OBJECT -1)
	{
		return nullptr;
	}

	// �e�̃C���X�^���X����
	CBullet* pBullet;

	// �������m��
	pBullet = new CBullet;

	// �I�u�W�F�N�g�ݒ�
	pBullet->SetTexture(nType);
	pBullet->SetPos(pos);
	pBullet->SetType(nType);
	pBullet->SetSize(fWidth, fHeight);
	pBullet->m_nLife = nLife;

	// �������Ɏ��s������
	if (FAILED(pBullet->Init(rot)))
	{
		// �j��
		delete pBullet;

		// NULL��Ԃ�
		return nullptr;
	}

	// �|�C���^��Ԃ�
	return pBullet;
}
//===============================
// �e�̎�ރZ�b�g
//===============================
void CBullet::SetType(BTYPE type)
{
	// ��ސݒ�
	m_Type = type;
}
//===============================
// ��ނ��Ƃ̃e�N�X�`���Z�b�g
//===============================
void CBullet::SetTexture(BTYPE type)
{
	// �e�N�X�`���|�C���^�擾
	CTexture* pTexture = CManager::GetTexture();

	// �e�N�X�`���ݒ�
	switch (type)
	{
	case BTYPE_PLAYER:
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\bullet002.png");
		break;

	case BTYPE_ENEMY:
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\gold.jpg");
		break;

	default:
		break;
	}
}
//===============================
// �e�̏�����
//===============================
HRESULT CBullet::Init(D3DXVECTOR3 rot)
{
	// �I�u�W�F�N�g2D�̏�����
	CBillboard::Init();

	// �I�u�W�F�N�g�̎�ނ�ݒ肷��
	SetObjType(TYPE_BULLET);

	// �ړ���
	m_move = D3DXVECTOR3(rot.x * 15.0f, rot.y,rot.z * 15.0f);

	return S_OK;
}
//===============================
// �e�̏I��
//===============================
void CBullet::Uninit(void)
{
	// �I�u�W�F�N�g2D�̔j��
	CBillboard::Uninit();
}
//===============================
// �e�̍X�V
//===============================
void CBullet::Update(void)
{
	// �e�̍��W���擾
	D3DXVECTOR3 BulletPos = GetPos();

	// �����J�E���g
	m_nLife--;

	// �J���[�Z�b�g
	SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	// �G�t�F�N�g����
	CEffect::Create(BulletPos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), VECTOR3_NULL, m_nLife, 10.0f);

	// �ʒu���X�V
	BulletPos += m_move;

	// ���W���X�V
	SetPos(BulletPos);

	// �G�Ƃ̓����蔻��
	bool isHit = Collision(BulletPos);

	// �̗͂�0�ȉ� ���� �G�ɓ������Ă��Ȃ�������
	if (m_nLife <= 0 && !isHit)
	{
		// ���g�p���
		Uninit();
	}
}
//===============================
// �e�̕`��
//===============================
void CBullet::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���擾
	CTexture* pTexture = CManager::GetTexture();

	// �e�N�X�`���ǂݍ���
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// �I�u�W�F�N�g�̕`��
	CBillboard::Draw();
}
//===============================
// �����蔻��֐�
//===============================
bool CBullet::Collision(D3DXVECTOR3 pos)
{
#if 0
	// �ő吔��
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		// �I�u�W�F�N�g���擾����
		CObject* pObj = CObject::GetObject(CObject::PRIORITY::PLAYER,nCnt);

		// ���݂̒e�̎�ނ��Z�b�g
		BTYPE btype= GetType();

		// NULL�`�F�b�N
		if (pObj != NULL)
		{
			// �I�u�W�F�N�g�̎�ނ��擾
			TYPE type = pObj->GetObjType();

			if (type == TYPE_ENEMY && btype == BTYPE_PLAYER)
			{// ��ނ��G�������� ���� �e�̎�ނ��v���C���[�̒e�Ȃ�

				// �G�̍��W,�p�x�̎擾
				D3DXVECTOR3 enemypos = pObj->GetPos();

				// �L���X�g����
				CEnemy* pEnemy = (CEnemy*)pObj;

				// �G�̈ʒu���Ɣ�r��,����𐶐�
				if (pos.x <= enemypos.x + 30.0f &&
					pos.y >= enemypos.y - 30.0f &&
					pos.x >= enemypos.x - 30.0f &&
					pos.y <= enemypos.y + 30.0f)
				{					
					// �G���Ƀ_���[�W
 					pEnemy->HitEnemy(BULLET_DAMAGE);

					// �e�̏I������
					CBullet::Uninit();

					// ���茋�ʂ�Ԃ�
					return true;
				}
			}
			else if (type == TYPE_PLAYER && btype == BTYPE_ENEMY)
			{// ��ނ��v���C���[��������@���� �e�̎�ނ��G�Ȃ�

				// �v���C���[�̍��W,�p�x�̎擾
				D3DXVECTOR3 pPos = pObj->GetPos();

				// �v���C���[�ɃL���X�g����
				CPlayer* pPlayer = (CPlayer*)pObj;

				// �v���C���[�̈ʒu���Ɣ�r��,����𐶐�
				if (pos.x <= pPos.x + 50.0f &&
					pos.y >= pPos.y - 50.0f &&
					pos.x >= pPos.x - 50.0f &&
					pos.y <= pPos.y + 50.0f &&
					pPlayer->GetState() == CPlayer::PLAYERSTATE_NORMAL)
				{
					// �v���C���[�Ƀ_���[�W
					// pPlayer->HitDamage(BULLET_DAMAGE);

					// �e�̏I������
					CBullet::Uninit();

					// ���茋�ʂ�Ԃ�
					return true;
				}
			}
		}
	}
#endif
	// ���茋�ʂ�����������
 	return false;
}
