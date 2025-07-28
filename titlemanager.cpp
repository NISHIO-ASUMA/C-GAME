//=========================================
//
// �^�C�g���Ǘ����� [ titlemanager.h ]
// Author: Asuma Nishio
// 
//==========================================

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "titlemanager.h"
#include "titleui.h"
#include "manager.h"
#include "input.h"
#include "block.h"
#include "meshfield.h"
#include "game.h"
#include "tutorial.h"

//============================
// �R���X�g���N�^
//============================
CTitleManager::CTitleManager()
{
	// �l�̃N���A
	m_nIdx = NULL;

	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		m_pTitleui[nCnt] = nullptr;
	}
}
//============================
// �f�X�g���N�^
//============================
CTitleManager::~CTitleManager()
{
	// ����
}
//============================
// ����������
//============================
HRESULT CTitleManager::Init(void)
{
	// ����W��ݒ�
	D3DXVECTOR3 CenterPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// �^�C�g����ui�𐶐�
	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		// �����̊Ԋu�󂯂�
		CenterPos.y += nCnt * 180.0f;

		// ui�𐶐� ( �I�����j���[�� )
		m_pTitleui[nCnt] = CTitleUi::Create(CenterPos, COLOR_WHITE, 300.0f, 60.0f, nCnt);
	}

	// �u���b�N����
	CBlock::Create("data\\MODEL\\STAGEOBJ\\block000.x",VECTOR3_NULL,VECTOR3_NULL,NULL);

	// �n�ʐ���
	CMeshField::Create(VECTOR3_NULL, 1000.0f);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//============================
// �I������
//============================
void CTitleManager::Uninit(void)
{
	// ����
}
//============================
// �X�V����
//============================
void CTitleManager::Update(void)
{
	// ���̓f�o�C�X�擾
	CInputKeyboard* pKey = CManager::GetInputKeyboard();

	// �I���C���f�b�N�X�͈�
	const int SELECT_BEGIN = NULL;
	const int SELECT_END = TITLE_MENU - 1;

	// ��L�[����
	if (pKey->GetTrigger(DIK_UP))
	{
		// �C���f�b�N�X�ԍ������Z
		m_nIdx--;

		// �ŏ��l�ȉ��Ȃ�ŏ��l�ɐݒ�
		if (m_nIdx < SELECT_BEGIN)
			m_nIdx = SELECT_END;
	}

	// ���L�[����
	if (pKey->GetTrigger(DIK_DOWN))
	{
		// �C���f�b�N�X�ԍ������Z
		m_nIdx++;

		// �ő�l�ȏ�Ȃ�ő�l�ɐݒ�
		if (m_nIdx > SELECT_END)
			m_nIdx = SELECT_BEGIN;
	}

	// �t�F�[�h�擾
	CFade* pFade = CManager::GetFade();

	// null��������
	if (pFade == nullptr) return;

	// �I������Ă��郁�j���[�̃|���S���J���[��ύX
	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		// null����Ȃ�������
		if (m_pTitleui[nCnt] != nullptr)
		{
			// �J���[�ύX
			if (nCnt == m_nIdx)
			{
				// �_��
				m_pTitleui[nCnt]->SetFlash(20, 40);
			}
			else
			{
				// �J���[�Z�b�g
				m_pTitleui[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	// ���菈��
	if (pKey->GetTrigger(DIK_RETURN))
	{
		switch (m_nIdx)
		{
		case CTitleUi::MENU_GAME:		// �Q�[�����[�h
			if (pFade != nullptr) pFade->SetFade(new CGame());	// �Q�[���V�[���ɑJ��
			break;

		case CTitleUi::MENU_TUTORIAL:	// �`���[�g���A�����[�h
			if (pFade != nullptr) pFade->SetFade(new CTutorial());	// �`���[�g���A���V�[���ɑJ��
			break;

		default:
			break;
		}
	}
}
