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

constexpr int  keyRepeatCount = 15;		// �L�[�̃��s�[�g�J�E���g

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

	// �V�����_�[���W�̎擾
	D3DXVECTOR3 MeshPos = CGame::GetCylinder()->GetPos();

	// �{�X�̍��W�擾
	D3DXVECTOR3 BossPos = CGame::GetBoss()->GetPos();

	// �v���C���[�ƃ{�X�ԂŃx�N�g������
	D3DXVECTOR3 VecBoss = BossPos - m_pPlayer->GetPos();
	VecBoss.y = 0.0f;

	// �ł����x�N�g���𐳋K������
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// �v���C���[�̈ړ��X�V�֐�
	m_pPlayer->UpdateMove(MeshPos, pInput);

	// ����̃��[���h�}�g���b�N�X���擾����
	CModel* pModelWepon = m_pPlayer->GetModelPartType(CModel::PARTTYPE_WEAPON);
	D3DXMATRIX mtxWorld = {};

	// null����Ȃ�������
	if (pModelWepon != nullptr) mtxWorld = pModelWepon->GetMtxWorld();

	// �L�[���͎�
	if (pInput->GetPress(DIK_RETURN))
	{
		// �X�e�[�g�ύX
		m_pPlayer->ChangeState(new CPlayerStateAction);

		// �����ŏ�����Ԃ�
		return;
	}

	// �W�����v�s��
	m_pPlayer->UpdateJumpAction(pInput, mtxWorld, VecBoss);

	// �ʒu���X�V����
	m_pPlayer->AddMove();
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
	m_pPlayer->SetAttack(false);
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

	// �{�X�̕������v�Z
	D3DXVECTOR3 VecBoss = CGame::GetBoss()->GetPos() - m_pPlayer->GetPos();
	VecBoss.y = 0.0f;

	// ���K��
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// �L�[�t���O���Z�b�g
	bool isKeyPress = false;

	//====================
	// �U������
	//====================
	if (pInput->GetPress(DIK_RETURN) && m_pPlayer->GetMotion()->GetMotionType() != CPlayer::PLAYERMOTION_JUMPATTACK)
	{
		// �L�[�t���O���Z�b�g
		isKeyPress = true;

		// 15�t���[���U���L�[����͂��Ă�����
		if (pInput->GetRepeat(DIK_RETURN, keyRepeatCount))
		{
			// �e�𐶐�
			CBullet::Create(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43), VecBoss, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 60);
		}

		// �U����Ԃ���Ȃ��Ƃ�
		if (!m_pPlayer->GetAttack())
		{
			// �U���t���O��L��������
			m_pPlayer->SetAttack(true);

			// �n��U�����[�V�����ɕύX
			m_pPlayer->GetMotion()->SetMotion(CPlayer::PLAYERMOTION_ACTION);
		}
		else if (m_pPlayer->GetAttack() && !m_pPlayer->GetMotion()->GetFinishMotion()) // �U����� ���� ���[�V�����I�����肪false�̎�
		{
			// �U���t���O�𖳌�������
			m_pPlayer->SetAttack(false);

			// �j���[�g�������[�V�����ɕύX
			m_pPlayer->ChangeState(new CPlayerStateNeutral());

			// �L�[���̓t���O�𖳌��ɂ���
			isKeyPress = false;

			return; // �����ŏ�����Ԃ�
		}
	}
	else if (!isKeyPress && m_pPlayer->GetMotion()->GetMotionType() == CPlayer::PLAYERMOTION_ACTION)
	{// �L�[�t���O������ ���� ���݂̃��[�V�������U�����[�V�����Ȃ�

		// �U����Ԃ�����
		m_pPlayer->SetAttack(false);

		// �j���[�g�������[�V�����ɕύX
		m_pPlayer->ChangeState(new CPlayerStateNeutral());

		return; // �����ŏ�����Ԃ�

	}

	// �U����� ���� ���[�V�����̏�Ԃ��U������Ȃ�������
	if (m_pPlayer->GetAttack() && m_pPlayer->GetMotion()->GetMotionType() != CPlayer::PLAYERMOTION_ACTION)
	{
		// �U����Ԃ�����
		m_pPlayer->SetAttack(false);
	}
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
	
}
//==================================
// �ړ���ԍX�V�֐�
//==================================
void CPlayerStateMove::OnUpdate()
{

}
//==================================
// �ړ���ԏI���֐�
//==================================
void CPlayerStateMove::OnExit()
{
}
