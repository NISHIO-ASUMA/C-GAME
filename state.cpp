//============================================
//
// ��ԊǗ����� [ state.cpp ]
// Author: Asuma Nishio
//
//=============================================

//***********************
// �C���N���[�h�t�@�C��
//***********************
#include "state.h"

//**********************
// �萔�錾
//**********************
constexpr int  MAX_STATECOUNT = 40; // �ő�p������

//==================================
// �R���X�g���N�^
//==================================
CState::CState()
{
	// �l�̃N���A
	m_state = STATE_NONE;
	m_StateCount = NULL;
}
//==================================
// �f�X�g���N�^
//==================================
CState::~CState()
{
	// ����
}
//==================================
// ����������
//==================================
HRESULT CState::Init(void)
{
	// ��Ԃ�����������
	m_state = STATE_NORMAL;

	// ��ԊǗ��J�E���^�[�̒l��ݒ�
	m_StateCount = MAX_STATECOUNT;

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==================================
// �I������
//==================================
void CState::Uninit(void)
{
	// ����
}
//==================================
// �X�V����
//==================================
void CState::Update(void)
{
	// ���݂̏�Ԃ��擾
	m_state = GetState();

	// ��Ԃɉ����ĕύX
	switch (m_state)
	{
	case STATE_NONE:	// �����Ȃ����
		break;

	case STATE_NORMAL:	// �ʏ���
		break;

	case STATE_DAMAGE:	// �_���[�W���

		// ��ԊǗ��J�E���^�[���f�N�������g
		m_StateCount--;

		// ��Ԃ�ύX����
		if (m_StateCount <= 0)
		{
			m_state = STATE_INVINCIBLE;
			m_StateCount = 60;
		}

		break;

	case STATE_INVINCIBLE:	// ���G���

		// ��ԊǗ��J�E���^�[���f�N�������g
		m_StateCount--;

		if (m_StateCount <= NULL)
		{
			// �ʏ��ԂɕύX
			m_state = STATE_NORMAL;
			m_StateCount = MAX_STATECOUNT; // �J�E���g��ύX
		}

		break;

	default:
		break;
	}

	// ��ԃZ�b�g
	SetState(m_state);
}
//==================================
// ��������
//==================================
CState* CState::Create(void)
{
	// �C���X�^���X����
	CState* pState = new CState;

	// ���������s��
	if (FAILED(pState->Init()))
	{
		// �|�C���^�̔j��
		delete pState;

		// nullptr���
		pState = nullptr;

		// null�|�C���^��Ԃ�
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pState;
}



//==================================
// �X�e�[�g�x�[�X�R���X�g���N�^
//==================================
CStateBase::CStateBase()
{
	// ����
}
//==================================
// �X�e�[�g�x�[�X�f�X�g���N�^
//==================================
CStateBase::~CStateBase()
{
	// ����
}


//==================================
// �X�e�[�g�}�V���R���X�g���N�^
//==================================
CStateMachine::CStateMachine()
{
	// �l�̃N���A
	m_pNowState = nullptr;
}
//==================================
// �X�e�[�g�}�V���f�X�g���N�^
//==================================
CStateMachine::~CStateMachine()
{
	// ����
}
//==================================
// �X�e�[�g�}�V���X�V����
//==================================
void CStateMachine::Update(void)
{
	// nullptr����Ȃ�������
	if (m_pNowState != nullptr)
	{
		// �X�V�J�n
		m_pNowState->OnUpdate();
	}
}
//==================================
// �X�e�[�g�ύX����
//==================================
void CStateMachine::ChangeState(CStateBase* pNewState)
{	
	// �|�C���^�����݂���Ƃ�
	if (m_pNowState != nullptr && pNewState != nullptr)
	{
		// ID �������Ȃ�
		if (m_pNowState->GetID() == pNewState->GetID())
		{
			delete pNewState;

			// �����ŏ�����Ԃ�
			return;
		}
	}

	// ���łɃX�e�[�g���Z�b�g����Ă���I������
	if (m_pNowState != nullptr)
	{
		// �I��
		m_pNowState->OnExit();

		// �|�C���^�̔j��
		delete m_pNowState;

		// nullptr�ɏ���������
		m_pNowState = nullptr;
	}

	// �V�����X�e�[�g���Z�b�g����
	m_pNowState = pNewState;

	// �V�����X�e�[�g���J�n����
	m_pNowState->OnStart();
}
//==================================
// �X�e�[�g�I������
//==================================
void CStateMachine::OnExit()
{
	// ���łɃX�e�[�g���Z�b�g����Ă���I������
	if (m_pNowState != nullptr)
	{
		// �j��
		delete m_pNowState;

		// null������
		m_pNowState = nullptr;
	}
}

