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
#include "debugproc.h"

//**********************
// �萔�錾
//**********************
constexpr float PLAYER_MOVE = 0.0095f;		// 1�t���[���̈ړ���
constexpr float PLAYER_JUMPVALUE = 18.0f;	// �W�����v��
constexpr int   NUMBER_MAIN = 0;			// ���C������v���C���[�ԍ�
constexpr int   NUMBER_SUB = 1;				// ���g����v���C���[�ԍ�
constexpr int   KeyRepeatCount = 15;		// �L�[�̃��s�[�g�J�E���g

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_move = VECTOR3_NULL;
	m_State = NULL;
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

	// ���[�V������ސ�����
	m_type = PLAYERMOTION_MAX;

	m_fAngle = NULL;

	// �t���O��ݒ�
	m_isJump = false;
	m_isLanding = false;
	m_isMoving = false;

	// ���[�V�����̓ǂݍ���
	m_pMotion = CMotion::Load(m_pFilename, MAX_MODEL, m_apModel,PLAYERMOTION_MAX);

	// ���[�V��������ݒ�
	m_pMotion->SetMotionNum(PLAYERMOTION_MAX);

	// �v���C���[���ʃ��f���t���O��ݒ�
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		// nullptr����Ȃ�������
		if (m_apModel[nCnt] != nullptr)
		{
			// �t���O��ݒ肷��
			m_apModel[nCnt]->SetIsPlayer(true);
		}
	}

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

	// �V�����_�[���W�擾
	D3DXVECTOR3 MeshPos = CManager::GetCylinder()->GetPos();

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
	D3DXMATRIX mtxWorld = {};

	// �E�r���f���̎擾
	CModel* pModelWepon = CPlayer::GetModelPartType(CModel::PARTTYPE_WEAPON);

	// null����Ȃ��Ȃ�
	if (pModelWepon != nullptr)
	{
		// ���[���h�}�g���b�N�X���Z�b�g
		mtxWorld = pModelWepon->GetMtxWorld();
	}

	// ���ݑ̗͂̎擾
	int nLife = m_pParameter->GetHp();

	// �U�����̓{�X�̕����ɑ̂�������
	if (m_isAttack)
	{
		// �{�X����v���C���[�ւ̃x�N�g������
		D3DXVECTOR3 BossDir = CManager::GetBoss()->GetPos() - m_pos;

		// y�������͈��
		BossDir.y = 0.0f;

		// ������0.0001f���傫���Ƃ�
		if (D3DXVec3LengthSq(&BossDir) > 0.0001f)
		{
			// �x�N�g���𐳋K��
			D3DXVec3Normalize(&BossDir, &BossDir);

			// �v���C���[�̊p�x�����킹��
			m_rot.y = atan2f(-BossDir.x, -BossDir.z); // Y����]�̂ݒ���
		}
	}

	// �U����Ԃ���Ȃ� ���� ���݃��[�V�������W�����v�U������Ȃ���
	if (!m_isAttack && m_pMotion->GetMotionType() != PLAYERMOTION_JUMPATTACK)
	{
		// �ړ��X�V�֐�
		UpdateMove(MeshPos,pInput);
	}

	// �U������
	UpdateNeutralAction(pInput,mtxWorld, BulletMove);
	
	// �W�����v����
	UpdateJumpAction(pInput, mtxWorld, BulletMove);

	// �ړ����Z����
	m_pos += m_move;

	// ���݂̏�Ԃ��擾
	m_State = m_pState->GetState();

	// ���݂̏�Ԃ��Z�b�g����
	m_pState->SetState(m_State);

	//=============================
	// �C���p�N�g�Ƃ̓����蔻��
	//=============================
	// �I�u�W�F�N�g�擾
	CObject* pObj = CObject::GetTop(static_cast<int>(CObject::PRIORITY::IMPACT));

	// nullptr����Ȃ��Ƃ�
	while (pObj != nullptr)
	{
		// ���b�V���^�C�v���擾
		if (pObj->GetObjType() == CObject::TYPE_MESH)
		{
			// �C���p�N�g�ɃL���X�g
			CMeshImpact* pImpact = static_cast<CMeshImpact*>(pObj);

			// �R���W�������� ���� ��Ԃ��ʏ펞
			if (pImpact->Collision(&m_pos) == true)
			{
				// ���ݏ�Ԃ�NORMAL�Ȃ�
				if (m_State == m_pState->STATE_NORMAL)
				{
					// ����������_���[�W���[�V�����ɐ؂�ւ�
					m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

					// ��ԍX�V
					m_pState->SetState(CState::STATE_DAMAGE);

					// �_���[�W����
					m_pParameter->HitDamage(1);

					// ��񓖂������甲����
					break;
				}
			}
		}

		// ���̃I�u�W�F�N�g�����o����
		pObj = pObj->GetNext();
	}

	//=============================
	// �{�X�E��̓����蔻��
	//=============================
	CBoss* pBoss = CManager::GetBoss();  // �}�l�[�W���[�o�R�Ŏ擾����

	// �����蔻��̋���
	if (pBoss->CollisionRightHand(&m_pos))
	{
		// ��Ԃ��ʏ�̎��̂�
		if (m_State == m_pState->STATE_NORMAL)
		{
			// �_���[�W���[�V����
			m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

			// ��ԍX�V
			m_pState->SetState(CState::STATE_DAMAGE);

			// �̗͂����炷
			m_pParameter->HitDamage(1);
		}
	}

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

	// ���ݑ̗͂��Z�b�g
	nLife = m_pParameter->GetHp();

	// �[���ȉ�
	if (nLife <= 0)
	{
		// �I������
		Uninit();

		// �����ŏ�����Ԃ�
		return;
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

	// �t�H���g
	CDebugproc::Print("���݂�STATE { %d } ", m_State);
	// �f�o�b�O�t�H���g�`��
	CDebugproc::Draw(0, 160);

	// ���ʕ`��
	CDebugproc::Print("MAIN�v���C���[�̍��W { %.2f,%.2f,%.2f }", CPlayer::GetIdxPlayer(0)->GetPos().x, CPlayer::GetIdxPlayer(0)->GetPos().y, CPlayer::GetIdxPlayer(0)->GetPos().z);
	// �f�o�b�O�t�H���g�`��
	CDebugproc::Draw(0, 200);

	// ���ʕ`��
	CDebugproc::Print("SUB�v���C���[�̍��W { %.2f,%.2f,%.2f }", CPlayer::GetIdxPlayer(1)->GetPos().x, CPlayer::GetIdxPlayer(1)->GetPos().y, CPlayer::GetIdxPlayer(1)->GetPos().z);
	// �f�o�b�O�t�H���g�`��
	CDebugproc::Draw(0, 220);

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
// ���f���̓��蕔���p�[�c�̎擾�֐�
//=========================================
CModel* CPlayer::GetModelPartType(CModel::PARTTYPE modelpart)
{
	// �v���C���[�������Ă��郂�f���̐��̒�����T��
	for (int nModel = 0; nModel < MAX_MODEL; nModel++)
	{
		// ���f�������� ���� �擾���������ƈ�v���Ă�����
		if (m_apModel[nModel] && m_apModel[nModel]->GetPartType() == modelpart)
		{
			// �Y�����f����idx�ԍ��̃��f����Ԃ�
			return m_apModel[nModel];
		}
	}

	// �Y���Ȃ�
	return nullptr;
}
//=========================================
// ���ʔԍ����Ƃ̍U���X�V����
//=========================================
void CPlayer::UpdateNeutralAction(CInputKeyboard* pInputKeyboard,D3DXMATRIX pMtx,const D3DXVECTOR3 DestMove)
{
	// �L�[�t���O���Z�b�g
	bool isKeyPress = false;

	//====================
	// �U������
	//====================
	if (pInputKeyboard->GetPress(DIK_RETURN) && m_pMotion->GetMotionType() != PLAYERMOTION_JUMPATTACK)
	{
		// �L�[�t���O���Z�b�g
		isKeyPress = true;

		// 15�t���[���U���L�[����͂��Ă�����
		if (pInputKeyboard->GetRepeat(DIK_RETURN, KeyRepeatCount))
		{
			// �e�𐶐�
			CBullet::Create(D3DXVECTOR3(pMtx._41, pMtx._42, pMtx._43), DestMove, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 60);
		}

		// �U����Ԃ���Ȃ��Ƃ�
		if (!m_isAttack) 	
		{
			// �U���t���O��L��������
			m_isAttack = true;

			// �n��U�����[�V�����ύX
			m_pMotion->SetMotion(PLAYERMOTION_ACTION);
		}
		else if (m_isAttack && !m_pMotion->GetFinishMotion()) // �U����� ���� ���[�V�����I�����肪false�̎�
		{
			// �U���t���O�𖳌�������
			m_isAttack = false;

			// �j���[�g�������[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);

			// �L�[���̓t���O�𖳌��ɂ���
			isKeyPress = false;
		}
	}
	else if (!isKeyPress && m_pMotion->GetMotionType() == PLAYERMOTION_ACTION)
	{// �L�[�t���O������ ���� ���݂̃��[�V�������U�����[�V�����Ȃ�

		// �ʏ탂�[�V�����ɕύX
		m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);

		// �U����Ԃ�����
		m_isAttack = false;
	}

	// �U����� ���� ���[�V�����̏�Ԃ��U������Ȃ�������
	if (m_isAttack && m_pMotion->GetMotionType() != PLAYERMOTION_ACTION)
	{
		// �U�����I��
		m_isAttack = false;
	}
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
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			// �p�x�X�V
			fAngle -= PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // �E�����ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else
		{
			// �ړ����[�V������,�j���[�g�������[�V�����ɕύX
			if (m_pMotion->GetMotionType() == PLAYERMOTION_MOVE)m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);
		}
		break;

	case NUMBER_SUB: // �Ίp����̃v���C���[

		// �L�[���͎��̊p�x�v�Z
		if (pInputKeyboard->GetPress(DIK_A)) // A�L�[
		{
			// �p�x�X�V
			fAngle += PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // �������ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D)) // D�L�[
		{
			// �p�x�X�V
			fAngle -= PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // �E�����ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else
		{
			// �ړ����[�V������,�j���[�g�������[�V�����ɕύX
			if (m_pMotion->GetMotionType() == PLAYERMOTION_MOVE)m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);
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

	// ���W�X�V����
	m_posOld = m_pos;
}
//=========================================
// ���ʔԍ����Ƃ̃W�����v�X�V����
//=========================================
void CPlayer::UpdateJumpAction(CInputKeyboard* pInputKeyboard, D3DXMATRIX pMtx, const D3DXVECTOR3 DestMove)
{
	// ���[�V�����̃t���O
	bool isJumpAttacking = (m_pMotion->GetMotionType() == PLAYERMOTION_JUMPATTACK);

	// �󒆍U����
	if (isJumpAttacking && pInputKeyboard->GetPress(DIK_RETURN))
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
	if (!m_isJump && pInputKeyboard->GetTrigger(DIK_SPACE))
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
		m_pMotion->SetMotion(PLAYERMOTION_JUMP);

		// �W�����v���ɍU���L�[����
		if (pInputKeyboard->GetPress(DIK_RETURN))
		{
			// �U���L�[��15�t���[�����������Ă�����
			if (pInputKeyboard->GetRepeat(DIK_RETURN, KeyRepeatCount))
			{
				// �e�𐶐�
				CBullet::Create(D3DXVECTOR3(pMtx._41, pMtx._42, pMtx._43), DestMove, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 45);
			}

			// �W�����v�U�����[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_JUMPATTACK);
		}

		// ���n���̏���
		if (m_isLanding)
		{
			// ���n���[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_LANDING);

			// �W�����v�\��ԂɕύX
			m_isJump = false;
		}
	}
}