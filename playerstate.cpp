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

	// Enter�L�[�ōU���X�e�[�g�֑J��
	if (pInput->GetPress(DIK_RETURN))
	{
		// �X�e�[�g���A�N�V������ԂɕύX����
		m_pPlayer->ChangeState(new CPlayerStateAction());

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
	m_pPlayer->SetAttack(true);
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

	// �U���t���O�������Ă��Ȃ���Ώ�����
	if (!m_pPlayer->GetAttack())
	{
		// �U����L����Ԃɂ���
		m_pPlayer->SetAttack(true);

		// �U�����[�V�����ɕύX
		m_pPlayer->GetMotion()->SetMotion(CPlayer::PLAYERMOTION_ACTION);
	}

	// �U���L�[����
	if (pInput->GetRepeat(DIK_RETURN, keyRepeatCount))
	{
		// 15�t���[���������琶��
		CBullet::Create(
			D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43),
			VecBoss, 
			CBullet::BTYPE_PLAYER, 
			5.0f, 5.0f, 60);
	}

	// ���[�V�����I���Ńj���[�g�����ɖ߂�
	if (m_pPlayer->GetMotion()->GetFinishMotion())
	{
		// �U���I��
		m_pPlayer->SetAttack(false);

		// �j���[�g�����X�e�[�g�𐶐�
		m_pPlayer->ChangeState(new CPlayerStateNeutral());
	}
}
//==================================
// �U����ԏI���֐�
//==================================
void CPlayerStateAction::OnExit()
{
	// ����
}
