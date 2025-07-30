//============================================
//
// �v���C���[��ԊǗ����� [ playerstate.cpp ]
// Author: Asuma Nishio
//
//=============================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "playerstate.h"
#include "state.h"
#include "manager.h"
#include "gamemanager.h"
#include "parameter.h"

//******************************
// �萔�錾
//******************************
constexpr int DAMAGECOUNT = 80;	// �ő�X�e�[�g�J�E���g

//==================================
// �v���C���[��ԃR���X�g���N�^
//==================================
CPlayerStateBase::CPlayerStateBase()
{
	// �l�̃N���A
	m_pPlayer = nullptr;
	m_ID = ID_NONE;
}
//==================================
// �v���C���[��ԃf�X�g���N�^
//==================================
CPlayerStateBase::~CPlayerStateBase()
{
	// ����
}


//==================================
// �ҋ@��Ԏ��R���X�g���N�^
//==================================
CPlayerStateNeutral::CPlayerStateNeutral()
{
	// �Z�b�g
	SetID(ID_NEUTRAL);
}
//==================================
// �ҋ@��Ԏ��f�X�g���N�^
//==================================
CPlayerStateNeutral::~CPlayerStateNeutral()
{
	// ����
}
//==================================
// �ҋ@��ԊJ�n�֐�
//==================================
void CPlayerStateNeutral::OnStart()
{
	// �j���[�g�������[�V�����ɐݒ�
	m_pPlayer->GetMotion()->SetMotion(CPlayer::PLAYERMOTION_NEUTRAL);
}
//==================================
// �ҋ@��ԍX�V�֐�
//==================================
void CPlayerStateNeutral::OnUpdate()
{
	// �L�[���͏��̎擾
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	CJoyPad* pPad = CManager::GetJoyPad();

	// �ړ����͂�����Έړ���Ԃ�
	if ((m_pPlayer->isMoveInputKey(pInput) || m_pPlayer->isMovePadButton(pPad)) &&
		m_pPlayer->GetNowMotion() != CPlayer::PLAYERMOTION_DAMAGE)
	{
		// ��ԕύX
		m_pPlayer->ChangeState(new CPlayerStateMove,ID_MOVE);

		// �����ŏ�����Ԃ�
		return;
	}

	// �U���L�[���͎�
	if ((pInput->GetPress(DIK_RETURN) || pPad->GetPress(CJoyPad::JOYKEY_X)) &&
		m_pPlayer->GetNowMotion() != CPlayer::PLAYERMOTION_DAMAGE) 
	{
		// �X�e�[�g�ύX
		m_pPlayer->ChangeState(new CPlayerStateAction,ID_ACTION);

		// �����ŏ�����Ԃ�
		return;
	}
}
//==================================
// �ҋ@��Ԏ��I���֐�
//==================================
void CPlayerStateNeutral::OnExit()
{
	// ����
}


//==================================
// �R���X�g���N�^
//==================================
CPlayerStateAction::CPlayerStateAction()
{
	// ����
}
//==================================
// �f�X�g���N�^
//==================================
CPlayerStateAction::~CPlayerStateAction()
{
	// ����
}
//==================================
// �U����ԊJ�n�֐�
//==================================
void CPlayerStateAction::OnStart()
{
	// �U�����[�V�����ɕύX
	m_pPlayer->GetMotion()->SetMotion(CPlayer::PLAYERMOTION_ACTION);
}
//==================================
// �U����ԍX�V�֐�
//==================================
void CPlayerStateAction::OnUpdate()
{
	// ���͏��̎擾
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	CJoyPad * pPad = CManager::GetJoyPad();

	// ����̈ʒu�擾
	CModel* pModelWeapon = m_pPlayer->GetModelPartType(CModel::PARTTYPE_WEAPON);

	// null��������
	if (!pModelWeapon) return;

	// �r�̃��[���h�}�g���b�N�X���擾
	D3DXMATRIX mtxWorld = pModelWeapon->GetMtxWorld();

	// �v���C���[�ƃ{�X�ԂŃx�N�g������
	D3DXVECTOR3 VecBoss = m_pPlayer->VecToBoss(m_pPlayer->GetPos());

	// �U���X�V
	m_pPlayer->UpdateAction(pInput, mtxWorld, VecBoss, pPad);
}
//==================================
// �U����ԏI���֐�
//==================================
void CPlayerStateAction::OnExit()
{
	// ����
}


//==================================
// �ړ���ԃR���X�g���N�^
//==================================
CPlayerStateMove::CPlayerStateMove()
{
	// ����
}
//==================================
// �ړ���ԃf�X�g���N�^
//==================================
CPlayerStateMove::~CPlayerStateMove()
{
	// ����
}
//==================================
// �ړ���ԊJ�n�֐�
//==================================
void CPlayerStateMove::OnStart()
{
	// ����
}
//==================================
// �ړ���ԍX�V�֐�
//==================================
void CPlayerStateMove::OnUpdate()
{
	// �L�[���͂��擾	
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	CJoyPad* pPad = CManager::GetJoyPad();

	// �V�����_�[���W�̎擾
	D3DXVECTOR3 MeshPos = CGameManager::GetCylinder()->GetPos();

	// �ړ��������s
	m_pPlayer->UpdateMove(MeshPos, pInput, pPad);

	if (!m_pPlayer->isMoveInputKey(pInput) && !m_pPlayer->isMovePadButton(pPad))
	{
		// �j���[�g�����ɑJ��
		m_pPlayer->ChangeState(new CPlayerStateNeutral, ID_NEUTRAL);
		return;
	}
}
//==================================
// �ړ���ԏI���֐�
//==================================
void CPlayerStateMove::OnExit()
{
	// ����
}

//==================================
// �_���[�W��ԃR���X�g���N�^
//==================================
CPlayerStateDamage::CPlayerStateDamage(int nDamage)
{
	// �l�̃N���A
	m_nStateCount = NULL;
	m_nDamage = nDamage;
}
//==================================
// �_���[�W��ԃf�X�g���N�^
//==================================
CPlayerStateDamage::~CPlayerStateDamage()
{
	// ����
}
//==================================
// �_���[�W��ԊJ�n�֐�
//==================================
void CPlayerStateDamage::OnStart()
{
	// ��̖ڂ̃v���C���[�̎�
	if (m_pPlayer->GetPlayerIndex() == 0)
	{
		// �̗͂����炷
		m_pPlayer->HitDamage(m_nDamage);
	}

	// ��ԕύX
	m_nStateCount = DAMAGECOUNT;
}
//==================================
// �_���[�W��ԍX�V�֐�
//==================================
void CPlayerStateDamage::OnUpdate()
{
	// ��ԊǗ��J�E���^�[���f�N�������g
	m_nStateCount--;

	if (m_nStateCount <= NULL)
	{
		// �J�E���g��ύX
		m_nStateCount = DAMAGECOUNT; 

		// ��ԕύX
		m_pPlayer->ChangeState(new CPlayerStateNeutral(), ID_NEUTRAL);

		// �����ŏ�����Ԃ�
		return;
	}
}
//==================================
// �_���[�W��ԍX�V�֐�
//==================================
void CPlayerStateDamage::OnExit()
{
	// ����
}
