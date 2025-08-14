//=====================================
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
CBossStateNeutral::CBossStateNeutral(int nTime) : m_CoolTime(nTime), m_nCurrentState(0)
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

	// �N�[���^�C���Z�b�g
	m_pBoss->SetCoolTime(m_CoolTime);
}
//===========================
// �ҋ@��ԍX�V����
//===========================
void CBossStateNeutral::OnUpdate(void)
{
	// �t���O�����g
	static bool isCreating = false;

	// false�Ȃ�
	if (!isCreating)
	{
		// �����̎����x�����ݒ肷��
		srand((int)time(NULL));

		// 2��ڂɓ���Ȃ��悤�Ƀt���O��L����
		isCreating = true;
	}

	if (m_CoolTime <= 0)
	{
		// �N�[���^�C��������������
		m_CoolTime = 0;

		// �����_���Ńp�^�[����ݒ�
		if (m_nCurrentState == 0)
		{
			// �s���p�^�[���ԍ����v�Z
			m_nCurrentState = rand() % CBoss::PATTERN_MAX - 1;

			switch (m_nCurrentState)
			{
			case CBoss::PATTERN_HAND:

				// �X�e�[�g�ύX
				m_pBoss->GetMotion()->SetResetFrame(0);

				// ���[�V�����ύX
				m_pBoss->GetMotion()->SetMotion(CBoss::PATTERN_HAND);// ����U��

				return;

			case CBoss::PATTERN_IMPACT:

				// �X�e�[�g�ύX
				m_pBoss->GetMotion()->SetResetFrame(0);

				// ���[�V�����ύX
				m_pBoss->GetMotion()->SetMotion(CBoss::PATTERN_IMPACT); // �@�����U��

				return;
			}
		}
	}
	else
	{
		// �j���[�g�������[�V�������X�V
		m_pBoss->GetMotion()->SetMotion(CBoss::TYPE_NEUTRAL, false, 0);
	}
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
	m_pBoss->ChangeState(new CBossStateNeutral(60), ID_NEUTRAL);
}
//===========================
// �U����ԏI������
//===========================
void CBossStateAttack::OnExit(void)
{
	// ����
}
