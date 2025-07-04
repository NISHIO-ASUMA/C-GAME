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

//**********************
// �}�N����`
//**********************
#define PLAYER_MOVE (0.0095f)	 // �v���C���[�ړ���
#define PLAYER_JUMPVALUE (17.0f) // �W�����v��
#define NUMBER_MAIN (0)			 // ���C������v���C���[
#define NUMBER_SUB (1)			 // ���g����v���C���[

//**********************
// �ÓI�����o�ϐ��錾
//**********************
int CPlayer::m_nIdxPlayer = NULL; // �v���C���[���ʔԍ�

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_move = VECTOR3_NULL;
	m_State = PLAYERSTATE_NONE;
	m_StateCount = NULL;
	m_nIdxTexture = NULL;
	m_rotDest = VECTOR3_NULL;
	m_nNumAll = NULL;
	m_type = NULL;
	m_posOld = VECTOR3_NULL;
	m_size = NULL;

	m_pFilename = {};

	// ���f���̃|�C���^�̃N���A
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}

	m_pMotion = nullptr;
	m_pShadow = nullptr;
	m_pState = nullptr;

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

	// �I�u�W�F�N�g�ݒ�
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
	pPlayer->m_nIdxPlayer = nIdxParson;
	pPlayer->m_pFilename = pFilename;

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
	m_type = CPlayer::TYPE_MAX;

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

	// ���[�V�����|�C���^�̔j��
	if (m_pMotion != nullptr)
	{
		// �|�C���^�̔j��
		delete m_pMotion;

		// nullptr�ɂ���
		m_pMotion = nullptr;
	}

	// ��ԊǗ��|�C���^�̔j��
	if (m_pState != nullptr)
	{
		// �|�C���^�̔j��
		delete m_pState;

		// nullptr�ɂ���
		m_pState = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
//============================================================
// �v���C���[�X�V����  ( �p�x���v���C���[�̈ړ��ʂƂ��ēn�� )
//============================================================
void CPlayer::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// ���b�V���̍��W�̎擾
	D3DXVECTOR3 MeshCylinderPos = CManager::GetCylinder()->GetPos();

	// �{�X�̍��W�̎擾
	D3DXVECTOR3 BossPos = CManager::GetBoss()->GetPos();

	// ���b�V���̔��a�̎擾
	float fRadius = CManager::GetCylinder()->GetRadius();

	// ���݂̃��[�V�����^�C�v���擾
	int Type = m_pMotion->GetMotionType();

	// �p�x�v�Z�p
	static float fAngle = NULL;

	// �U��������Ȃ�������
	if (!m_isAttack)
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
			m_rotDest.y = fAngle + D3DX_PI * 0.5f;// �E

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

	//=====================================================
	// ���b�V���̍��W���猩�ăv���C���[�̈ʒu�����߂�
	//=====================================================
	m_pos.x = MeshCylinderPos.x - (sinf(fAngle)) * fRadius;		// X���W
	m_pos.z = MeshCylinderPos.z - (cosf(fAngle)) * fRadius;		// Z���W

	// �p�x���v�Z
	m_rot.y += (m_rotDest.y - m_rot.y) - D3DX_PI;

	//=========================
	// �U������
	//=========================
	// �L�[���̓t���O
	bool isKeyPress = false;

	// �{�X�̕����ւ̃x�N�g�����擾
	D3DXVECTOR3 VecBoss = BossPos - m_pos;

	// Y�͐��������ō��킹��i�e���^��������Ԃ悤�Ɂj
	VecBoss.y = 0.0f;

	// �x�N�g���𐳋K��
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// �e�̈ړ�����
	D3DXVECTOR3 BulletMove = VecBoss;

	// �v���C���[�̘r�̃��[���h�}�g���b�N�X���擾
	D3DXMATRIX mtxWorld = m_apModel[18]->GetMtxWorld();

	// �U���L�[ ���� �U���t���O��false�Ȃ�
	if (pInput->GetPress(DIK_RETURN))
	{
		// �L�[�t���O���Z�b�g
		isKeyPress = true;

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

	//=========================
	// �W�����v����
	//=========================
	if (m_isJump == false)
	{// �W�����v���Ă��Ȃ��Ē��n���Ă��Ȃ��Ƃ�
		if (pInput->GetTrigger(DIK_SPACE))
		{
			// ���b�V���C���p�N�g����
			CMeshImpact::Create(m_pos, 80, 50.0f, 5.0f, 15.0f);

			// �p�[�e�B�N������
			CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 10.0f, m_pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),50,150,100,500);

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

		if (m_isLanding == true)
		{
			// ���n���[�V�����ɕύX
			m_pMotion->SetMotion(m_pMotion->TYPE_LANDING);

			// �W�����v�\��Ԃɂ���
			m_isJump = false;
		}
	}

	//=========================
	// �d�͏���
	//=========================
	m_move.y -= 0.7f;
	m_posOld = m_pos;
	m_pos += m_move;

	// ���W��0�ȉ�
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_isLanding = true;
		m_move.y = 0.0f;
	}

	// �e�̍X�V����
	m_pShadow->UpdatePos(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));

	// ���[�V�����S�̂̍X�V
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
	CDebugproc::Print("�v���C���[���W [ %.2f,%.2f,%.2f ]", m_pos.x, m_pos.y, m_pos.z );
	CDebugproc::Draw(0, 20);

	CDebugproc::Print("�v���C���[���� [ %.2f,%.2f,%.2f ]", m_rot.x, m_rot.y, m_rot.z);
	CDebugproc::Draw(0, 140);

}
//=================================
// �L�[����
//=================================
void CPlayer::MoveKey(CInputKeyboard* pInputKeyBoard,CCamera * pCamera)
{
#if 0
	// �ړ�����
	if (pInputKeyBoard->GetPress(DIK_A))
	{// A�L�[��������

		if (pInputKeyBoard->GetPress(DIK_W))
		{// W�L�[��������
			m_move.x += sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * MAX_MOVE;
			m_move.z += cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
		}
		else if (pInputKeyBoard->GetPress(DIK_S))
		{// S�L�[��������
			m_move.x -= sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * MAX_MOVE;
			m_move.z -= cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
		}
		else
		{// A�L�[�݂̂�������
			m_move.x -= sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * MAX_MOVE;
			m_move.z -= cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
		}

		// �p�x�̐��K��
		if (m_rot.y > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// ���[�V�����Z�b�g
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE, m_pMotion->TYPE_NEUTRAL, false, 30);
	}
	else if (pInputKeyBoard->GetPress(DIK_D))
	{// D�L�[��������
		if (pInputKeyBoard->GetPress(DIK_W))
		{// W�L�[��������
			m_move.x += sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * MAX_MOVE;
			m_move.z += cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.75f);

		}
		else if (pInputKeyBoard->GetPress(DIK_S))
		{// S�L�[��������
			m_move.x -= sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * MAX_MOVE;
			m_move.z -= cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.25f);
		}
		else
		{// D�L�[�̂݉�����
			m_move.x += sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * MAX_MOVE;
			m_move.z += cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.5f);
		}

		// �p�x�̐��K��
		if (m_rot.y > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// ���[�V�����Z�b�g
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE, m_pMotion->TYPE_NEUTRAL, false, 40);

	}
	else if (pInputKeyBoard->GetPress(DIK_W))
	{// W�L�[��������

		m_move.x += sinf(pCamera->GetRot().y) * MAX_MOVE;
		m_move.z += cosf(pCamera->GetRot().y) * MAX_MOVE;
		m_rotDest.y = pCamera->GetRot().y - (D3DX_PI);

		// �p�x�𐳋K��
		if (m_rot.y < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			m_rot.y += D3DX_PI * 2.0f;
		}

		// ���[�V�����Z�b�g
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE, m_pMotion->TYPE_NEUTRAL, false, 40);

	}
	else if (pInputKeyBoard->GetPress(DIK_S))
	{// S�L�[��������

		m_move.x -= sinf(pCamera->GetRot().y) * MAX_MOVE;
		m_move.z -= cosf(pCamera->GetRot().y) * MAX_MOVE;
		m_rotDest.y = pCamera->GetRot().y;

		// �p�x�̐��K��
		if (m_rot.y > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// ���[�V�����Z�b�g
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE, m_pMotion->TYPE_NEUTRAL, false, 30);

	}
	else
	{
		// �ʏ펞�̓j���[�g����
		m_pMotion->SetMotion(TYPE_NEUTRAL, TYPE_MOVE, false, 0);
	}

	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{// ����]
		// �p�x
		m_rot.y = m_rot.y + D3DX_PI * 2.0f;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{// �E��]
		// �p�x
		m_rot.y = m_rot.y - D3DX_PI * 2.0f;
	}

	// Enter�L�[����
	if (pInputKeyBoard->GetTrigger(DIK_RETURN))
	{
		// �U�����[�V�����ɕύX
		m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL, m_pMotion->TYPE_NEUTRAL, true, 40);
	}

	if (bJump == false)
	{// �W�����v���Ă��Ȃ��Ē��n���Ă��Ȃ��Ƃ�
		if (pInputKeyBoard->GetTrigger(DIK_SPACE))
		{
			// �U�����[�V�����ɕύX
			m_pMotion->SetMotion(m_pMotion->TYPE_ACTION, NULL, false, 0);

			bJump = true;
			isLanding = false;

			// �W�����v�L�[�������ꂽ
			m_move.y = 15.0f;
		}
	}
	// �W�����v���g���Ă���
	if (bJump == true)
	{
		if (isLanding == true)
		{
			bJump = false;
		}
	}
#endif
}
//=========================================
// ���ʔԍ����Ƃ̃v���C���[�̍X�V����
//=========================================
void CPlayer::UpdateIdxPlayer(int nIdx,CInputKeyboard * pInputKey)
{
	// ���ʔԍ����Ƃ̃v���C���[�̓���
	switch (nIdx)
	{
	case NUMBER_MAIN:

		break;

	case NUMBER_SUB:

		break;

	default:

		break;
	}
}
