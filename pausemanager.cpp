//====================================
//
// �|�[�Y�Ǘ����� [ pausemanager.cpp ]
// Author: Asuma Nishio
// 
// TODO : �������Ń|�[�Y�̑I�����̏����Ƃ�������
//
//=====================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "pausemanager.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "game.h"

//***************************
// �ÓI�����o�ϐ��錾
//***************************
bool CPauseManager::m_isPause = false; // �|�[�Y�t���O

//==================================
// �R���X�g���N�^
//==================================
CPauseManager::CPauseManager()
{
	// �l�̃N���A
	m_nSelectIdx = NULL;

	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		m_pPause[nCnt] = nullptr;
	}
}
//==================================
// �f�X�g���N�^
//==================================
CPauseManager::~CPauseManager()
{
	// ����
}
//==================================
// ����������
//==================================
HRESULT CPauseManager::Init(void)
{
	// �Z���N�g�ԍ��ݒ�
	m_nSelectIdx = CPause::MENU_RETRY;

	// ����W��ݒ�
	D3DXVECTOR3 Bacepos = D3DXVECTOR3(200.0f, 80.0f, 0.0f);
	
	// �󂯂�Ԋu�ݒ�
	float fSpace = 200.0f;

	// �|�[�Y����
	for (int nPause = 0; nPause < SELECT_MAX; nPause++)
	{
		// ���W�Z�b�g
		D3DXVECTOR3 pos = Bacepos;

		// �������󂯂�
		pos.y += nPause * fSpace;

		// �|�[�Y�𐶐� 
		if (nPause == 0)
		{
			// �w�i����
			m_pPause[nPause] = CPause::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,0.0f), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nPause);
		}
		else
		{
			// �I��p�|���S���̐���
			m_pPause[nPause] = CPause::Create(pos, 180.0f, 40.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nPause);
		}
	}

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==================================
// �I������
//==================================
void CPauseManager::Uninit(void)
{

}
//==================================
// �X�V����
//==================================
void CPauseManager::Update(void)
{
	// �|�[�Y������Ȃ�������
	if (m_isPause == false)
	{
		// �����ŏ�����Ԃ�
		return;
	}

	// ���̓f�o�C�X�擾
	CInputKeyboard* pKey = CManager::GetInputKeyboard();

	// �I���C���f�b�N�X�͈�
	const int SELECT_BEGIN = 1;
	const int SELECT_END = SELECT_MAX - 1;

	// ��L�[����
	if (pKey->GetTrigger(DIK_UP))
	{
		// �C���f�b�N�X�ԍ������Z
		m_nSelectIdx--;

		// �ŏ��l�ȉ��Ȃ�ŏ��l�ɐݒ�
		if (m_nSelectIdx < SELECT_BEGIN)
			m_nSelectIdx = SELECT_END;
	}

	// ���L�[����
	if (pKey->GetTrigger(DIK_DOWN))
	{
		// �C���f�b�N�X�ԍ������Z
		m_nSelectIdx++;

		// �ő�l�ȏ�Ȃ�ő�l�ɐݒ�
		if (m_nSelectIdx > SELECT_END)
			m_nSelectIdx = SELECT_BEGIN;
	}

	// �t�F�[�h�擾
	CFade* pFade = CManager::GetFade();

	// null��������
	if (pFade == nullptr) return;

	// ���菈��
	if (pKey->GetTrigger(DIK_RETURN))
	{
		switch (m_nSelectIdx)
		{
		case CPause::MENU_BACK:
			break;

		case CPause::MENU_RETRY:	// ���g���C��
			if (pFade != nullptr) pFade->SetFade(new CGame());	// �Q�[���V�[���ɑJ��
			SetEnablePause(false);		// �|�[�Y�I��
			break;

		case CPause::MENU_CONTINUE:	// �R���e�j���[��
			SetEnablePause(false);		// �|�[�Y�I��
			break;

		case CPause::MENU_QUIT:		// �ޏo��
			if (pFade != nullptr) pFade->SetFade(new CTitle());	// �^�C�g���V�[���ɑJ��
			SetEnablePause(false);		// �|�[�Y�I��
			break;
		}
	}
}
//===========================
// �|�[�Y���ǂ���
//===========================
void CPauseManager::SetEnablePause(void)
{
	//  P�L�[�������ꂽ
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P))
	{
		// �t���O�ύX
		m_isPause = m_isPause ? false : true;
	}
}
