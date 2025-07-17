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
#include "manager.h"
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
#include "shadowS.h"
#include "game.h"
#include "playerstate.h"
#include "state.h"

//**********************
// �萔�錾
//**********************
constexpr float PLAYER_MOVE = 0.0095f;		// 1�t���[���̈ړ���
constexpr float PLAYER_JUMPVALUE = 17.0f;	// �W�����v��
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
	m_pParameter = nullptr;
	m_pShadowS = nullptr;
	m_pStateMachine = nullptr;

	// �t���O�����g
	m_isLanding = false;
	m_isJump = false;
	m_isAttack = false;
	m_isMoving = false;
	m_isShadow = false;
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

	// �̗͂�2�̂ŋ���
	if (nIdxParson == NUMBER_MAIN)
	{
		// �|�C���^����
		pPlayer->m_pParameter = new CParameter;

		// null�`�F�b�N
		if (pPlayer->m_pParameter != nullptr)
		{
			// �̗̓p�����[�^�[��ݒ�
			pPlayer->m_pParameter->SetHp(nLife);
		}
	}
	else
	{
		// ���C�����̏����Z�b�g
		CPlayer* pMain = CPlayer::GetIdxPlayer(NUMBER_MAIN);

		// nullptr�`�F�b�N
		if (pMain != nullptr)
		{
			// �|�C���^�����L
			pPlayer->m_pParameter = pMain->m_pParameter; 
		}
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
	
	// ��l�� ���� �t���O�����g�p����Ȃ�
	if (m_nIdxPlayer == NUMBER_MAIN)
	{
		// �X�e���V���V���h�E����
		m_pShadowS = CShadowS::Create("data\\MODEL\\STAGEOBJ\\Shadowmodel.x", CPlayer::GetIdxPlayer(0)->GetPos(), CPlayer::GetIdxPlayer(0)->GetRot());
	}

	InitPos(0.0f);

	// �X�e�[�g�}�V���𐶐�
	m_pStateMachine = new CStateMachine;	

	// ������Ԃ��Z�b�g
	ChangeState(new CPlayerStateNeutral); 

	// ���ʂ�Ԃ�
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
	if (m_pParameter != nullptr && m_nIdxPlayer == NUMBER_MAIN)
	{
		// �|�C���^�̔j��
		delete m_pParameter;

		// nullptr�ɂ���
		m_pParameter = nullptr;
	}

	// nullptr�`�F�b�N
	if (m_pStateMachine != nullptr)
	{
		// �I������
		m_pStateMachine->OnExit();

		// �|�C���^�̔j��
		delete m_pStateMachine;

		// nullptr�ɂ���
		m_pStateMachine = nullptr;
	}

	// �I�u�W�F�N�g���g�̔j��
	CObject::Release();
}
//============================================================
// �v���C���[�X�V����  ( �p�x���v���C���[�̈ړ��ʂƂ��ēn�� )
//============================================================
void CPlayer::Update(void)
{
	// �U�����̓{�X�̕����ɑ̂�������
	if (m_isAttack)
	{
		D3DXVECTOR3 BossDir = CGame::GetBoss()->GetPos() - m_pos;
		BossDir.y = 0.0f;

		if (D3DXVec3LengthSq(&BossDir) > 0.0001f)
		{
			D3DXVec3Normalize(&BossDir, &BossDir);
			m_rot.y = atan2f(-BossDir.x, -BossDir.z);
		}
	}

	// ���ݑ̗͂̎擾
	int nLife = m_pParameter->GetHp();

	// nullptr����Ȃ��Ƃ�
	if (m_pStateMachine != nullptr)
	{
		// �X�e�[�g�X�V
		m_pStateMachine->Update();
	}

	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// ����̃��[���h�}�g���b�N�X�ƃ{�X�����擾
	CModel* pModelWeapon = GetModelPartType(CModel::PARTTYPE_WEAPON);
	if (!pModelWeapon) return;

	D3DXMATRIX mtxWorld = pModelWeapon->GetMtxWorld();
	D3DXVECTOR3 vecToBoss = VecToBoss(m_pos);

	UpdateJumpAction(pInput, mtxWorld, vecToBoss);

	// �����蔻�菈���֐�
	Collision();

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
	else
	{
		// ���[�V�����S�̂��X�V
		m_pMotion->Update(m_apModel, MAX_MODEL);
	}

	// �X�e���V���V���h�E������ ���� MAIN�v���C���[
	if (m_pShadowS && m_nIdxPlayer == NUMBER_MAIN)
	{
		// �e���W��MAIN�v���C���[���W�ɐݒ�
		D3DXVECTOR3 ShadowPos = GetIdxPlayer(0)->GetPos();

		// �I�u�W�F�N�g�ݒ�
		m_pShadowS->SetPos(ShadowPos);
		m_pShadowS->SetRot(GetIdxPlayer(0)->GetRot()); 
	}
}
//===============================
// �v���C���[�`�揈��
//===============================
void CPlayer::Draw(void)
{
	// �L�[�{�[�h�̓��͎擾
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

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

	// ���ʕ`��
	CDebugproc::Print("�v���C���[�̗̑� { %d } ", m_pParameter->GetHp());
	// �f�o�b�O�t�H���g�`��
	CDebugproc::Draw(0, 340);

	if (pInput->GetPress(DIK_F9) && m_nIdxPlayer == NUMBER_MAIN)
	{
		CDebugproc::Print("�X�e���V���e { %.2f,%.2f,%.2f }", m_pShadowS->GetPos().x, m_pShadowS->GetPos().y, m_pShadowS->GetPos().z);
		CDebugproc::Draw(0, 360);
	}
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
// �X�e�[�g�ύX
//=========================================
void CPlayer::ChangeState(CPlayerStateBase* pNewState)
{
	// �������g����
	pNewState->SetOwner(this);

	// �X�e�[�g�ύX
	m_pStateMachine->ChangeState(pNewState);
}

//=========================================
// ���ʔԍ����Ƃ̍U���X�V����
//=========================================
void CPlayer::UpdateAction(CInputKeyboard* pInputKeyboard,D3DXMATRIX pMtx,const D3DXVECTOR3 DestMove)
{
	// �L�[�t���O���Z�b�g
	bool isKeyPress = false;

	//====================
	// �U������
	//====================
	if (pInputKeyboard->GetPress(DIK_RETURN) && m_pMotion->GetMotionType() != CPlayer::PLAYERMOTION_JUMPATTACK)
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

			// �n��U�����[�V�����ɕύX
			m_pMotion->SetMotion(CPlayer::PLAYERMOTION_ACTION);
		}
		else if (m_isAttack && !m_pMotion->GetFinishMotion()) // �U����� ���� ���[�V�����I�����肪false�̎�
		{
			// �U���t���O�𖳌�������
			m_isAttack = false;

			// �L�[���̓t���O�𖳌��ɂ���
			isKeyPress = false;

			// �j���[�g�������[�V�����ɕύX
			ChangeState(new CPlayerStateNeutral());

			// �����ŏ�����Ԃ�
			return; 
		}
	}
	else if (!isKeyPress && m_pMotion->GetMotionType() == CPlayer::PLAYERMOTION_ACTION)
	{// �L�[�t���O������ ���� ���݂̃��[�V�������U�����[�V�����Ȃ�

		// �U����Ԃ�����
		m_isAttack = false;

		// �j���[�g�������[�V�����ɕύX
		ChangeState(new CPlayerStateNeutral());

		// �����ŏ�����Ԃ�
		return;

	}

	// �U����� ���� ���[�V�����̏�Ԃ��U������Ȃ�������
	if (m_isAttack &&m_pMotion->GetMotionType() != CPlayer::PLAYERMOTION_ACTION)
	{
		// �U����Ԃ�����
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
	float fRadius = CGame::GetCylinder()->GetRadius();

	// ���ʔԍ��ŏ����𕪕ʂ���
	switch (m_nIdxPlayer)
	{
	case NUMBER_MAIN: // ���C���v���C���[

		// �L�[���͎��̊p�x�v�Z
		if (pInputKeyboard->GetPress(DIK_A))
		{
			// �p�x�X�V
			m_fAngle += PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = m_fAngle - D3DX_PI * 0.5f; // �������ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			// �p�x�X�V
			m_fAngle -= PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = m_fAngle + D3DX_PI * 0.5f; // �E�����ɐݒ�

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
			m_fAngle += PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = m_fAngle - D3DX_PI * 0.5f; // �������ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D)) // D�L�[
		{
			// �p�x�X�V
			m_fAngle -= PLAYER_MOVE;

			// �ړI�p���v�Z
			m_rotDest.y = m_fAngle + D3DX_PI * 0.5f; // �E�����ɐݒ�

			// �ړ����[�V�����ɕύX
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else
		{
			// �ړ����[�V������,�j���[�g�������[�V�����ɕύX
			if (m_pMotion->GetMotionType() == PLAYERMOTION_MOVE) m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);
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
	float IdxAngle = (m_nIdxPlayer == NUMBER_MAIN) ? m_fAngle : m_fAngle + D3DX_PI;

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

	// ���ݍ��W���X�V
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
	if (!m_isJump)
	{
		if (pInputKeyboard->GetTrigger(DIK_SPACE))
		{
			// �t���O��L����
			m_isJump = true;

			// �����n����ɕύX
			m_isLanding = false;

			// �㏸�l��ݒ�
			m_move.y = PLAYER_JUMPVALUE;

		}
	}

	// �����X�V
	AddMove();

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


	if (m_pMotion->GetMotionType() == PLAYERMOTION_LANDING && m_pMotion->GetFinishMotion())
	{
		// �j���[�g�����ɕύX
		ChangeState(new CPlayerStateNeutral());

		// �����ŏ�����Ԃ�
		return;
	}

}
//=============================
// �R���W���������֐�
//=============================
void CPlayer::Collision(void)
{
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

			// �R���W����������
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
	CBoss* pBoss = CGame::GetBoss();  // �}�l�[�W���[�o�R�Ń{�X���擾����

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
}
//===============================
// �v���C���[�ƃ{�X�Ԃ̃x�N�g��
//===============================
D3DXVECTOR3 CPlayer::VecToBoss(const D3DXVECTOR3& pPos)
{
	// �{�X�̍��W�擾
	D3DXVECTOR3 BossPos = CGame::GetBoss()->GetPos();

	// �v���C���[�ƃ{�X�ԂŃx�N�g������
	D3DXVECTOR3 VecBoss = BossPos - pPos;
	VecBoss.y = 0.0f;

	// �ł����x�N�g���𐳋K������
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	return VecBoss;
}
//===============================
// �L�[�������̓��͎擾
//===============================
bool CPlayer::isMoveInputKey(CInputKeyboard* pKeyInput)
{
	// �����ꂩ�̈ړ��L�[����
	return (pKeyInput->GetPress(DIK_A) || pKeyInput->GetPress(DIK_D) ||
			pKeyInput->GetPress(DIK_W) || pKeyInput->GetPress(DIK_S));
	
}
//===============================
// �������W�v�Z�֐�
//===============================
void CPlayer::InitPos(float fAngle)
{
	// �~�����a���擾
	float fRadius = CGame::GetCylinder()->GetRadius();

	// �p�x�̏����ݒ�
	m_fAngle = fAngle;

	// ���g�̊p�x���v�Z
	float IdxAngle = (m_nIdxPlayer == NUMBER_MAIN) ? m_fAngle : m_fAngle + D3DX_PI;

	// �~����̏����ʒu���v�Z
	m_pos.x = CGame::GetCylinder()->GetPos().x - sinf(IdxAngle) * fRadius;
	m_pos.z = CGame::GetCylinder()->GetPos().z - cosf(IdxAngle) * fRadius;
	m_pos.y = 0.0f;

	// ���݂̊p�x��ݒ肷��
	if (m_nIdxPlayer == NUMBER_MAIN)
	{
		m_rot.y += (m_rotDest.y - m_rot.y) + D3DX_PI;
	}
	else if (m_nIdxPlayer == NUMBER_SUB)
	{
		m_rot.y += (m_rotDest.y - m_rot.y);
	}

	// �ڕW��]����v
	m_rotDest.y = m_rot.y;

	// �O�t���[�����W���Z�b�g
	m_posOld = m_pos;
}
void CPlayer::StartJump(void)
{
	// �W�����v�L�[���� ���� �W�����v�t���O��false�̎�
	if (!m_isJump)
	{
		// �t���O��L����
		m_isJump = true;

		// �����n����ɕύX
		m_isLanding = false;

		// �㏸�l��ݒ�
		m_move.y = PLAYER_JUMPVALUE;
	}
}