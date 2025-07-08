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
			m_StateCount = 5;
		}

		break;

	case STATE_INVINCIBLE:	// ���G���

		// ��ԊǗ��J�E���^�[���f�N�������g
		m_StateCount--;

		if (m_StateCount <= 0)
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