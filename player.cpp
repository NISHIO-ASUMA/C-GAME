//====================================
//
// �v���C���[���� [ player.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "player.h"
#include "Manager.h"
#include "texture.h"
#include "boss.h"
#include "particle.h"
#include "shadow.h"
#include "meshimpact.h"
#include "input.h"
#include "camera.h"
#include "state.h"
#include "parameter.h"

//**********************
// �萔�錾
//**********************
constexpr float PLAYER_MOVE = 0.0095f; // 1�t���[���̈ړ���
constexpr float PLAYER_JUMPVALUE = 20.0f; // �W�����v��
constexpr int   NUMBER_MAIN = 0;       // ���C������v���C���[�ԍ�
constexpr int   NUMBER_SUB = 1;		   // ���g����v���C���[�ԍ�

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_move = VECTOR3_NULL;
	m_StateCount = NULL;
	m_nIdxTexture = NULL;
	m_rotDest = VECTOR3_NULL;
	m_nNumAll = NULL;
	m_type = NULL;
	m_posOld = VECTOR3_NULL;
	m_size = NULL;
	m_pFilename = {};
	m_nIdxPlayer = NULL;
	m_fAngle = NULL;

	// ���f���̃|�C���^�̃N���A
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}

	// �N���X�|�C���^
	m_pMotion = nullptr;
	m_pShadow = nullptr;
	m_pState = nullptr;
	m_pParameter = nullptr;

	// �t���O�����g
	m_isLanding = false;
	m_isJump = false;
	m_isAttack = false;
	m_isMoving = false;
}
//===============================
// �f�X�g���N�^
//===============================
CPlayer::~CPlayer()
{
	// ����
}
//===============================
// �v���C���[��������
//===============================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nLife,const int nIdxParson, const char* pFilename)
{
	// �v���C���[�̃C���X�^���X����
	CPlayer* pPlayer = new CPlayer;

	// �C���X�^���X��nullptr��������
	if (pPlayer == nullptr) return nullptr;

	// �I�u�W�F�N�g�ݒ�
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
	pPlayer->m_nIdxPlayer = nIdxParson;
	pPlayer->m_pFilename = pFilename;

	// �p�����[�^����
	pPlayer->m_pParameter = new CParameter;

	// null�`�F�b�N
	if (pPlayer->m_pParameter != nullptr)
	{
		// �̗͐ݒ�
		pPlayer->m_pParameter->SetHp(nLife);
	}

	// �v���C���[����������
	if (FAILED(pPlayer->Init()))
	{
		// �j��
		delete pPlayer;

		// nullptr���
		pPlayer = nullptr;

		// nullptr��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pPlayer;
}
//===============================
// �v���C���[����������
//===============================
HRESULT CPlayer::Init(void)
{
	// �I�u�W�F�N�g�̎�ނ��Z�b�g
	SetObjType(TYPE_PLAYER);

	// ���f����������
	m_nNumAll = MAX_MODEL;

	// �^�C�v���
	m_type = CMotion::TYPE_MAX;
	m_fAngle = 0.0f;

	// �t���O��ݒ�
	m_isJump = false;
	m_isLanding = false;
	m_isMoving = false;

	// ���[�V�����̓ǂݍ���
	m_pMotion = CMotion::Load(m_pFilename, MAX_MODEL, m_apModel);

	// ���[�V��������ݒ�
	m_pMotion->SetMotionNum(CMotion::TYPE_MAX);

	// ��ԊǗ��𐶐�
	m_pState = CState::Create();

	// �e�̐���
	m_pShadow = m_pShadow->Create(m_pos, m_rot);

	return S_OK;
}
//===============================
// �v���C���[�I������
//===============================
void CPlayer::Uninit(void)
{
	// ���f�������̔j��
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		// nullptr�`�F�b�N
		if (m_apModel[nCnt] != nullptr)
		{
			// �I������
			m_apModel[nCnt]->Uninit();

			// �|�C���^�̔j��
			delete m_apModel[nCnt];

			// nullptr�ɂ���
			m_apModel[nCnt] = nullptr;
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
	if (m_pState != nullptr)
	{
		// �|�C���^�̔j��
		delete m_pState;

		// nullptr�ɂ���
		m_pState = nullptr;
	}

	// nullptr�`�F�b�N
	if (m_pParameter != nullptr)
	{
		// �|�C���^�̔j��
		delete m_pParameter;

		// nullptr�ɂ���
		m_pParameter = nullptr;
	}

	// �I�u�W�F�N�g���g�̔j��
	CObject::Release();
}
//============================================================
// �v���C���[�X�V����  ( �p�x���v���C���[�̈ړ��ʂƂ��ēn�� )
//============================================================
void CPlayer::Update(void)
{
	// �W���p�x
	static float fAngle = NULL;

	// �L�[�{�[�h�̓��͎擾
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// �V�����_�[���W,���a�̎擾
	D3DXVECTOR3 MeshPos = CManager::GetCylinder()->GetPos();
	float fRadius = CManager::GetCylinder()->GetRadius();

	// �{�X���W�̎擾
	D3DXVECTOR3 BossPos = CManager::GetBoss()->GetPos();

	// �{�X�̈ʒu�ɑ΂��ăv���C���[���W����x�N�g�������
	D3DXVECTOR3 VecBoss = BossPos - m_pos;

	// ���������ɍ��킹��
	VecBoss.y = 0.0f;

	// �x�N�g���̐��K��
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// ���K�����ꂽ�l��e�̈ړ��ʂɐݒ肷��
	D3DXVECTOR3 BulletMove = VecBoss;

	// �v���C���[�̘r�̃��[���h�}�g���b�N�X���擾����
	D3DXMATRIX mtxWorld = m_apModel[18]->GetMtxWorld();

	// �U����Ԃ���Ȃ� ���� ���݃��[�V�������W�����v�U������Ȃ���
	if (!m_isAttack && m_pMotion->GetMotionType() != CMotion::TYPE_JUMPATTACK)
	{

		// �ړ��X�V�֐�
		UpdateMove(MeshPos,pInput);
#if 0
		if (pInput->GetPress(DIK_A))
		{// A�L�[����

			// �p�x�X�V
			fAngle += PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // ��

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else if (pInput->GetPress(DIK_D)) 
		{// D�L�[����

			// �p�x�X�V
			fAngle -= PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // �E

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else
		{
			// �ړ����[�V������,�j���[�g�������[�V�����ɕύX
			if (m_pMotion->GetMotionType() == CMotion::TYPE_MOVE)m_pMotion->SetMotion(CMotion::TYPE_NEUTRAL);
		}
#endif
	}

	//// �p�x�𐳋K��
	//if (m_rotDest.y - m_rot.y > D3DX_PI)
	//{
	//	m_rot.y += D3DX_PI * 2.0f;
	//}
	//else if (m_rot.y - m_rotDest.y > D3DX_PI)
	//{
	//	m_rot.y -= D3DX_PI * 2.0f;
	//}

	//// ���g�̊p�x���v�Z
	//float IdxAngle = (m_nIdxPlayer == NUMBER_MAIN) ? fAngle : fAngle + D3DX_PI;

	//// �Ίp�����W�𒆐S����v�Z
	//m_pos.x = MeshPos.x - sinf(IdxAngle) * fRadius;
	//m_pos.z = MeshPos.z - cosf(IdxAngle) * fRadius;

	//// ���݂̊p�x�ɐݒ�
	//m_rot.y += (m_rotDest.y - m_rot.y) + D3DX_PI;

	// ���[�V�����̃t���O
	bool isJumpAttacking = (m_pMotion->GetMotionType() == m_pMotion->TYPE_JUMPATTACK);

	// �L�[�t���O���Z�b�g
	bool isKeyPress = false;

	//====================
	// �U������
	//====================
	if (pInput->GetPress(DIK_RETURN) && m_pMotion->GetMotionType() != CMotion::TYPE_JUMPATTACK)
	{
		// �L�[�t���O���Z�b�g
		isKeyPress = true;

		// 15�t���[���U���L�[����͂��Ă�����
		if (pInput->GetRepeat(DIK_RETURN, 15))
		{
			// �e�𐶐�
			CBullet::Create(
				D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43),
				VecBoss,
				CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 60);
		}

		if (!m_isAttack) 	// �U����Ԃ���Ȃ��Ƃ�
		{
			// �U���t���O��L��������
			m_isAttack = true;

			// �n��U�����[�V�����ύX
			m_pMotion->SetMotion(CMotion::TYPE_ACTION);
		}
		else if (m_isAttack && !m_pMotion->GetFinishMotion()) // �U����� ���� ���[�V�����I�����肪false�̎�
		{
			// �U���t���O�𖳌�������
			m_isAttack = false;

			// �j���[�g�������[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_NEUTRAL);

			// �L�[���̓t���O�𖳌��ɂ���
			isKeyPress = false;
		}
	}
	else if (!isKeyPress && m_pMotion->GetMotionType() == CMotion::TYPE_ACTION)
	{// �L�[�t���O������ ���� ���݂̃��[�V�������U�����[�V�����Ȃ�

		// �ʏ탂�[�V�����ɕύX
		m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);

		// �U����Ԃ�����
		m_isAttack = false;
	}

	// �U����� ���� ���[�V�����̏�Ԃ��U������Ȃ�������
	if (m_isAttack && m_pMotion->GetMotionType() != CMotion::TYPE_ACTION)
	{
		// �U�����I��
		m_isAttack = false;
	}

	// �󒆍U����
	if (isJumpAttacking && pInput->GetPress(DIK_RETURN))
	{
		// ���̍����ŐÎ~����
		m_move.y = 0.0f;
	}
	else
	{
		// �d�͒l��K�p
		m_move.y -= 0.7f;
	}

	// �W�����v�L�[���� ���� �W�����v�t���O��false�̎�
	if (!m_isJump && pInput->GetTrigger(DIK_SPACE))
	{
		// �t���O��L����
		m_isJump = true;

		// �����n����ɕύX
		m_isLanding = false;

		// �㏸�l��ݒ�
		m_move.y = PLAYER_JUMPVALUE;
	}

	// �W�����v������
	if (m_isJump)
	{
		// �W�����v���[�V�����ɕύX
		m_pMotion->SetMotion(CMotion::TYPE_JUMP);

		// �W�����v���ɍU���L�[����
		if (pInput->GetPress(DIK_RETURN))
		{
			// �U���L�[��15�t���[�����������Ă�����
			if (pInput->GetRepeat(DIK_RETURN, 15))
			{
				// �e�𐶐�
				CBullet::Create(
					D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43),
					VecBoss,
					CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 30);
			}

			// �W�����v�U�����[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_JUMPATTACK);
		}

		// ���n���̏���
		if (m_isLanding)
		{
			// ���n���[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_LANDING);

			// �W�����v�\��ԂɕύX
			m_isJump = false;

			// �Ռ��g�𐶐�
			CMeshImpact::Create(m_pos, 80, 50.0f, 5.0f, 15.0f);
		}
	}
	
	// �ړ�����
	m_posOld = m_pos;
	m_pos += m_move;

	// ���݂�y���W��0.0f�ȉ��̎�
	if (m_pos.y <= 0.0f)
	{
		// y���W��n�ʂ̍��W�ɐݒ�
		m_pos.y = 0.0f;

		// ���n�����L����
		m_isLanding = true;

		// �����ւ̈ړ��ʂ�0.0f�ɐݒ�
		m_move.y = 0.0f;
	}

	// HP�`�F�b�N
	if (m_pParameter->GetHp() <= 0)
	{
		// TODO : �e�X�g����
		// �_���[�W���[�V�����ɕύX
		m_pMotion->SetMotion(CMotion::TYPE_DAMAGE);
	}

	// ��ԊǗ����X�V
	m_pState->Update();

	// �e�̍��W���X�V
	m_pShadow->UpdatePos(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));

	// ���[�V�����S�̂��X�V
	m_pMotion->Update(m_apModel, MAX_MODEL);
}
//===============================
// �v���C���[�`�揈��
//===============================
void CPlayer::Draw(void)
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
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}

	// �f�o�b�O�\��
	//CDebugproc::Print("�v���C���[���W [ %.2f,%.2f,%.2f ]", m_pos.x, m_pos.y, m_pos.z );
	//CDebugproc::Draw(0, 20);

	//CDebugproc::Print("�v���C���[���� [ %.2f,%.2f,%.2f ]", m_rot.x, m_rot.y, m_rot.z);
	//CDebugproc::Draw(0, 140);

}
//=========================================
// ���ʔԍ����Ƃ̃v���C���[�̎擾
//=========================================
CPlayer* CPlayer::GetIdxPlayer(int Idx)
{
	// �I�u�W�F�N�g�̐擪�擾
	CObject* pObj = CObject::GetTop(static_cast<int>(CObject::PRIORITY::PLAYER));

	// pObj��nullptr����Ȃ�������
	while (pObj != nullptr)
	{
		// �I�u�W�F�N�g�̃^�C�v��PLAYER�̎�
		if (pObj->GetObjType() == CObject::TYPE_PLAYER)
		{
			// �v���C���[�^�ɃL���X�g
			CPlayer* pPlayer = static_cast<CPlayer*>(pObj);

			// �ԍ�����v���Ă�����
			if (pPlayer->GetPlayerIndex() == Idx)
			{
				// �|�C���^��Ԃ�
				return pPlayer;
			}
		}

		// ���̃v���C���[����
		pObj = pObj->GetNext();
	}

	// �擾�ł��Ȃ������ꍇ
	return nullptr;
}
//=========================================
// ���ʔԍ����Ƃ̍U���X�V����
//=========================================
void CPlayer::UpdateAction(CInputKeyboard* pInputKeyboard)
{
	
}
//=========================================
// ���ʔԍ����Ƃ̈ړ��X�V����
//=========================================
void CPlayer::UpdateMove(const D3DXVECTOR3 DestPos,CInputKeyboard* pInputKeyboard)
{
	// �L�[���͎��̊p�x�v�Z
	static float fAngle = NULL;

	// �~���̔��a���擾
	float fRadius = CManager::GetCylinder()->GetRadius();

	// ���ʔԍ��ŏ����𕪕ʂ���
	switch (m_nIdxPlayer)
	{
	case NUMBER_MAIN: // ���C���v���C���[

		// �L�[���͎��̊p�x�v�Z
		if (pInputKeyboard->GetPress(DIK_A))
		{
			// �p�x�X�V
			fAngle += PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // �������ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			// �p�x�X�V
			fAngle -= PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // �E�����ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else
		{
			// �ړ����[�V������,�j���[�g�������[�V�����ɕύX
			if (m_pMotion->GetMotionType() == CMotion::TYPE_MOVE)m_pMotion->SetMotion(CMotion::TYPE_NEUTRAL);
		}
		break;

	case NUMBER_SUB: // �Ίp����̃v���C���[

		// �L�[���͎��̊p�x�v�Z
		if (pInputKeyboard->GetPress(DIK_A))
		{
			// �p�x�X�V
			fAngle += PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // �������ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			// �p�x�X�V
			fAngle -= PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // �E�����ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else
		{
			// �ړ����[�V������,�j���[�g�������[�V�����ɕύX
			if (m_pMotion->GetMotionType() == CMotion::TYPE_MOVE)m_pMotion->SetMotion(CMotion::TYPE_NEUTRAL);
		}
		break;

	default:
		break;
	}

	// �p�x�𐳋K��
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// ���g�̊p�x���v�Z
	float IdxAngle = (m_nIdxPlayer == NUMBER_MAIN) ? fAngle : fAngle + D3DX_PI;

	// �Ίp�����W�𒆐S����v�Z
	m_pos.x = DestPos.x - sinf(IdxAngle) * fRadius;
	m_pos.z = DestPos.z - cosf(IdxAngle) * fRadius;

	// ���݂̊p�x��ݒ肷��
	if (m_nIdxPlayer == NUMBER_MAIN)
	{
		m_rot.y += (m_rotDest.y - m_rot.y) + D3DX_PI;
	}
	else if (m_nIdxPlayer == NUMBER_SUB)
	{
		m_rot.y += (m_rotDest.y - m_rot.y);
	}

	// ���W�X�V����}
	m_posOld = m_pos;
	m_pos += m_move;

}
#if 0

// �L�[�{�[�h�̎擾
CInputKeyboard* pInput = CManager::GetInputKeyboard();

// ���b�V���̍��W�̎擾
D3DXVECTOR3 MeshCylinderPos = CManager::GetCylinder()->GetPos();

#if 1
// �{�X�I�u�W�F�N�g�̎擾
CObject* pObjBoss = CObject::GetTop(CObject::PRIORITY::BOSS);

// �L���X�g����
CBoss* pBoss = CManager::GetBoss();

// �{�X�̍��W�̎擾
D3DXVECTOR3 BossPos = pBoss->GetPos();
#endif

// ���b�V���̔��a�̎擾
float fRadius = CManager::GetCylinder()->GetRadius();

// ���݂̃��[�V�����^�C�v���擾
int Type = m_pMotion->GetMotionType();

// �p�x�v�Z�p
static float fAngle = NULL;

//===========================
// �ړ��L�[����
//===========================
// �U����Ԃ���Ȃ� ���� �C���f�b�N�X�ԍ������C���̃v���C���[
if (m_nIdxPlayer == NUMBER_MAIN && !m_isAttack && Type != m_pMotion->TYPE_JUMPATTACK)
{
	// A�L�[
	if (pInput->GetPress(DIK_A))
	{
		// �p�x�X�V
		fAngle += PLAYER_MOVE;

		// �ړI�p���v�Z
		m_rotDest.y = fAngle - D3DX_PI * 0.5f; // ��

		// �ړ����[�V�����ɐݒ�
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE);

	}
	// D�L�[	
	else if (pInput->GetPress(DIK_D))
	{
		// �p�x�X�V
		fAngle -= PLAYER_MOVE;

		// �ړI�p���v�Z
		m_rotDest.y = fAngle + D3DX_PI * 0.5f; // �E

		// �ړ����[�V����
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE);
	}
	else
	{
		// �ړ���������
		if (Type == m_pMotion->TYPE_MOVE)
		{
			// �j���[�g�������[�V�����ɕύX
			m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);
		}
	}
}

// ��ԕύX
if (pInput->GetTrigger(DIK_K))
{
	// ��ԕύX
	m_pState->SetState(m_pState->STATE_DAMAGE);
}

//=========================
// �p�x�𐳋K������
//=========================
if (m_rotDest.y - m_rot.y > D3DX_PI)
{
	m_rot.y += D3DX_PI * 2.0f;
}
else if (m_rot.y - m_rotDest.y > D3DX_PI)
{
	m_rot.y -= D3DX_PI * 2.0f;
}

//=============================================
// �Ή�����p�x�ƍ��W�E�������v�Z
//=============================================
float finalAngle = (m_nIdxPlayer == NUMBER_MAIN) ? fAngle : fAngle + D3DX_PI;

//=====================================================
// ���b�V���̍��W���猩�ăv���C���[�̈ʒu�����߂�
//=====================================================
m_pos.x = MeshCylinderPos.x - (sinf(fAngle)) * fRadius;		// X���W
m_pos.z = MeshCylinderPos.z - (cosf(fAngle)) * fRadius;		// Z���W

// �ړI�p��ݒ�
m_rotDest.y = finalAngle;

// �p�x���v�Z
m_rot.y += (m_rotDest.y - m_rot.y) + D3DX_PI;

//=========================
// �U������
//=========================
// �L�[���̓t���O
bool isKeyPress = false;

#if 1
// �{�X�̕����ւ̃x�N�g�����擾
D3DXVECTOR3 VecBoss = BossPos - m_pos;

// ���������ō��킹��
VecBoss.y = NULL;

// �x�N�g���𐳋K��
D3DXVec3Normalize(&VecBoss, &VecBoss);

// �e�̈ړ�������ݒ�
D3DXVECTOR3 BulletMove = VecBoss;

#endif
// �v���C���[�̘r�̃��[���h�}�g���b�N�X���擾
D3DXMATRIX mtxWorld = m_apModel[18]->GetMtxWorld();

// �U���L�[ ���� ���[�V�������W�����v�U���łȂ����
if (pInput->GetPress(DIK_RETURN) && Type != m_pMotion->TYPE_JUMPATTACK)
{
	// �L�[�t���O���Z�b�g
	isKeyPress = true;

	// 15�t���[�����Ƃɒe�𔭎�
	if (pInput->GetRepeat(DIK_RETURN, 15))
	{
		// �r�̕���̕�������e�𔭎˂���
		CBullet::Create(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43), BulletMove, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 60);
	}

	// �U�����ĂȂ���
	if (!m_isAttack)
	{
		// �t���O��L����
		m_isAttack = true;

		// �U�����[�V�����ɕύX
		m_pMotion->SetMotion(m_pMotion->TYPE_ACTION);
	}
	else if (m_isAttack && m_pMotion->GetFinishMotion() == false)
	{
		// �A�^�b�N��false�ɂ���
		m_isAttack = false;

		// �ʏ탂�[�V�����ɕύX
		m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);

		// �t���O��false�ɂ���
		isKeyPress = false;
	}
}
else if (isKeyPress == false && Type == m_pMotion->TYPE_ACTION)
{
	// �ʏ탂�[�V�����ɕύX
	m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);

	// �U����Ԃ�����
	m_isAttack = false;
}

// �U���t���O���I�� ���� �A�N�V�������[�V��������Ȃ����
if (m_isAttack && m_pMotion->GetMotionType() != m_pMotion->TYPE_ACTION)
{
	// �U����Ԃ�����
	m_isAttack = false;
}

//=========================
// �W�����v����
//=========================
if (m_isJump == false)
{// �W�����v���Ă��Ȃ��Ē��n���Ă��Ȃ��Ƃ�
	if (pInput->GetTrigger(DIK_SPACE))
	{
		// �t���O��ύX
		m_isJump = true;
		m_isLanding = false;

		// �W�����v�L�[�������ꂽ
		m_move.y = PLAYER_JUMPVALUE;
	}
}

// �W�����v�\��
if (m_isJump == true)
{
	// �W�����v���[�V�����ɕύX
	m_pMotion->SetMotion(m_pMotion->TYPE_JUMP);

	// �W�����v���Ă���Ȃ�
	if (pInput->GetPress(DIK_RETURN))
	{
		// �e�̔��ˊԊu�𒲐�����
		if (pInput->GetRepeat(DIK_RETURN, 15))
		{
			// �r�̕���̕�������e�𔭎˂���
			CBullet::Create(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43), BulletMove, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 30);
		}

		// �W�����v�U�����[�V�����ɕύX
		m_pMotion->SetMotion(m_pMotion->TYPE_JUMPATTACK);

	}

	// ���n��
	if (m_isLanding == true)
	{
		// ���n���[�V�����ɕύX
		m_pMotion->SetMotion(m_pMotion->TYPE_LANDING);

		// �W�����v�\��Ԃɂ���
		m_isJump = false;

		// ���b�V���C���p�N�g����
		CMeshImpact::Create(m_pos, 80, 50.0f, 5.0f, 15.0f);
	}
}

// ���[�V�����̃t���O
bool isJumpAttacking = (m_pMotion->GetMotionType() == m_pMotion->TYPE_JUMPATTACK);

// �U���L�[�����Ă�Ԃ͐Î~
if (isJumpAttacking && pInput->GetPress(DIK_RETURN))
{
	m_move.y = 0.0f; // �����L�[�v
}
else
{
	m_move.y -= 0.7f; // �d�͓K�p
}

//=========================
// �ʒu�X�V
//=========================
m_posOld = m_pos;
m_pos += m_move;

// ���W��0�ȉ�
if (m_pos.y <= 0.0f)
{
	m_pos.y = 0.0f;
	m_isLanding = true;
	m_move.y = 0.0f;
}

// ���ݑ̗͂̎擾
int nLife = m_pParameter->GetHp();

// �̗͂�0�ȉ�
if (nLife <= 0)
{
	// ���S���[�V�����ɕύX
	m_pMotion->SetMotion(m_pMotion->TYPE_DAMAGE);
}

// ��ԊǗ��N���X�̍X�V
m_pState->Update();

// �e�̍X�V����
m_pShadow->UpdatePos(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));

// ���[�V�����S�̂̍X�V
m_pMotion->Update(m_apModel, MAX_MODEL);

#endif
