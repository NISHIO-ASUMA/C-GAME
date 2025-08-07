//====================================
//
// �{�X��ԊǗ����� [ bossstate.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "bossstate.h"

//===========================
// �R���X�g���N�^
//===========================
CBossStateBace::CBossStateBace()
{
	// �l�̃N���A
	m_ID = ID_NEUTRAL;
	m_pBoss = nullptr;
}
//===========================
// �f�X�g���N�^
//===========================
CBossStateBace::~CBossStateBace()
{
	// ����
}


//===========================
// �ҋ@��ԃR���X�g���N�^
//===========================
CBossStateNeutral::CBossStateNeutral()
{
	// ID�Z�b�g
	SetID(ID_NEUTRAL);
}
//===========================
// �ҋ@��ԃf�X�g���N�^
//===========================
CBossStateNeutral::~CBossStateNeutral()
{
	// ����
}
//===========================
// �ҋ@��ԊJ�n����
//===========================
void CBossStateNeutral::OnStart(void)
{
	// ���[�V�����Z�b�g
	m_pBoss->GetMotion()->SetMotion(CBoss::TYPE_NEUTRAL);
}
//===========================
// �ҋ@��ԍX�V����
//===========================
void CBossStateNeutral::OnUpdate(void)
{
	// �����ŃN�[���^�C����0�ȉ��Ȃ�X�e�[�g�ڍs�̏���

}
//===========================
// �ҋ@��ԏI������
//===========================
void CBossStateNeutral::OnExit(void)
{
	// ����
}


//===========================
// �U����ԃR���X�g���N�^
//===========================
CBossStateAttack::CBossStateAttack()
{
	// ID�Z�b�g
	SetID(ID_ACTION);
}
//===========================
// �U����ԃf�X�g���N�^
//===========================
CBossStateAttack::~CBossStateAttack()
{
	// ����
}
//===========================
// �U����ԊJ�n����
//===========================
void CBossStateAttack::OnStart(void)
{
	// �����Ń����_���̒l���g���čU���ԍ����[�V�����ɐݒ�

}
//===========================
// �U����ԍX�V����
//===========================
void CBossStateAttack::OnUpdate(void)
{
	// ���[�V�����̏I�����ł�����

	// �j���[�g�����ɂ���
	m_pBoss->ChangeState(new CBossStateNeutral, ID_NEUTRAL);
}
//===========================
// �U����ԏI������
//===========================
void CBossStateAttack::OnExit(void)
{
	// ����
}
