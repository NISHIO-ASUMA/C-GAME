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
#include "game.h"

//==================================
// �v���C���[��ԃR���X�g���N�^
//==================================
CPlayerStateBase::CPlayerStateBase()
{
	// �l�̃N���A
	m_pPlayer = nullptr;
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
	// ����
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

	// �ړ����͂�����Έړ���Ԃ�
	if (m_pPlayer->isMoveInputKey(pInput))
	{
		// ��ԕύX
		m_pPlayer->ChangeState(new CPlayerStateMove);

		// �����ŏ�����Ԃ�
		return;
	}

	// �U���L�[���͎�
	if (pInput->GetPress(DIK_RETURN))
	{
		// �X�e�[�g�ύX
		m_pPlayer->ChangeState(new CPlayerStateAction);

		// �����ŏ�����Ԃ�
		return;
	}

	//// �W�����v�L�[���͎�
	//if (pInput->GetPress(DIK_SPACE))
	//{
	//	// �X�e�[�g�ύX
	//	m_pPlayer->ChangeState(new CPlayerStateJump);

	//	// �����ŏ�����Ԃ�
	//	return;
	//}
}
//==================================
// �ҋ@��Ԏ��I���֐�
//==================================
void CPlayerStateNeutral::OnExit()
{
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

	// ����̈ʒu�擾
	CModel* pModelWeapon = m_pPlayer->GetModelPartType(CModel::PARTTYPE_WEAPON);

	// null��������
	if (!pModelWeapon) return;

	// �r�̃��[���h�}�g���b�N�X���擾
	D3DXMATRIX mtxWorld = pModelWeapon->GetMtxWorld();

	// �v���C���[�ƃ{�X�ԂŃx�N�g������
	D3DXVECTOR3 VecBoss = m_pPlayer->VecToBoss(m_pPlayer->GetPos());

	// �U���X�V
	m_pPlayer->UpdateAction(pInput, mtxWorld, VecBoss);

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

	// �V�����_�[���W�̎擾
	D3DXVECTOR3 MeshPos = CGame::GetCylinder()->GetPos();

	// �ړ��������s
	m_pPlayer->UpdateMove(MeshPos, pInput);

	// �L�[�𗣂�����j���[�g�����ɖ߂�
	if (!m_pPlayer->isMoveInputKey(pInput))
	{
		// �j���[�g�����ɑJ��
		m_pPlayer->ChangeState(new CPlayerStateNeutral);

		// �����ŏ�����Ԃ�
		return;
	}

	//// �W�����v�L�[���͎�
	//if (pInput->GetTrigger(DIK_SPACE))
	//{
	//	// �X�e�[�g�ύX
	//	m_pPlayer->ChangeState(new CPlayerStateJump);

	//	// �����ŏ�����Ԃ�
	//	return;
	//}
}
//==================================
// �ړ���ԏI���֐�
//==================================
void CPlayerStateMove::OnExit()
{

}


//==================================
// �W�����v��ԃR���X�g���N�^
//==================================
CPlayerStateJump::CPlayerStateJump()
{
	// ����
}
//==================================
// �W�����v��ԃf�X�g���N�^
//==================================
CPlayerStateJump::~CPlayerStateJump()
{
	// ����
}
//==================================
// �W�����v��ԊJ�n�֐�
//==================================
void CPlayerStateJump::OnStart()
{
	// �t���O�Z�b�g
	m_pPlayer->SetJump(false);
	m_pPlayer->StartJump();
}
//==================================
// �W�����v��ԍX�V�֐�
//==================================
void CPlayerStateJump::OnUpdate()
{
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// ����̃��[���h�}�g���b�N�X�ƃ{�X�����擾
	CModel* pModelWeapon = m_pPlayer->GetModelPartType(CModel::PARTTYPE_WEAPON);
	if (!pModelWeapon) return;

	D3DXMATRIX mtxWorld = pModelWeapon->GetMtxWorld();
	D3DXVECTOR3 vecToBoss = m_pPlayer->VecToBoss(m_pPlayer->GetPos());

	// �W�����v
	if (m_pPlayer->IsJumping())
	{
		m_pPlayer->UpdateJumpAction(pInput, mtxWorld, vecToBoss);
	}

	// ���n�� ���� ���[�V�����I�����Ă�����
	if (m_pPlayer->GetMotion()->GetMotionType() == CPlayer::PLAYERMOTION_LANDING && m_pPlayer->GetMotion()->GetFinishMotion())
	{
		// �j���[�g�����ɕύX
		m_pPlayer->ChangeState(new CPlayerStateNeutral());

		// �����ŏ�����Ԃ�
		return;
	}
}
//==================================
// �W�����v��ԏI���֐�
//==================================
void CPlayerStateJump::OnExit()
{
	// ����
}
