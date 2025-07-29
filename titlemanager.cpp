//=========================================
//
// �^�C�g���Ǘ����� [ titlemanager.cpp ]
// Author: Asuma Nishio
// 
//==========================================

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "titlemanager.h"
#include "titleui.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "input.h"
#include "block.h"
#include "meshfield.h"
#include "meshdome.h"
#include "titleplayer.h"
#include "ui.h"

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
	// �^�C�g���L���v�V��������
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, -60.0f, 0.0f), CUi::UITYPE_MOVE, 300.0f, 100.0f);
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), CUi::UITYPE_NONE, 300.0f, 100.0f);

	// �n�ʐ���
	CMeshField::Create(VECTOR3_NULL, FIELDWIDTH);

	// ���󃁃b�V���𐶐�
	CMeshDome::Create(D3DXVECTOR3(0.0f, -70.0f, 0.0f), 500.0f);

	// �^�C�g���v���C���[�𐶐�
	CTitlePlayer::Create(D3DXVECTOR3(180.0f,0.0f,0.0f), VECTOR3_NULL, 0, "data\\TitlePlayer100.txt");
	CTitlePlayer::Create(D3DXVECTOR3(260.0f,0.0f,0.0f), VECTOR3_NULL, 1, "data\\TitlePlayer200.txt");

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
	CJoyPad* pJoyPad = CManager::GetJoyPad();

	// �擾���s��
	if (pKey == nullptr) return;
	if (pJoyPad == nullptr) return;

	// �J�����擾
	CCamera* pCamera = CManager::GetCamera();

	// �L�[���̓t���O
	bool bKeyTrigger = false;

	// �L�[���͎�
	if ((pKey->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A)) && !bKeyTrigger)
	{
		// ����W��ݒ�
		D3DXVECTOR3 CenterPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 480.0f, 0.0f);

		// �^�C�g����ui�𐶐�
		for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
		{
			// �����̊Ԋu�󂯂�
			CenterPos.y += nCnt * DIGITPOS;

			// ui�𐶐� ( �I�����j���[�� )
			m_pTitleui[nCnt] = CTitleUi::Create(CenterPos, COLOR_WHITE, UIWIDTH, UIHEIGHT, nCnt);
		}

		bKeyTrigger = true;
	}


	// �I���C���f�b�N�X�͈�
	const int SELECT_BEGIN = NULL;
	const int SELECT_END = TITLE_MENU;

	// ��L�[����
	if (pKey->GetTrigger(DIK_UP) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_UP))
	{
		// �C���f�b�N�X�ԍ������Z
		m_nIdx--;

		// �ŏ��l�ȉ��Ȃ�ŏ��l�ɐݒ�
		if (m_nIdx < SELECT_BEGIN)
			m_nIdx = SELECT_END;
	}

	// ���L�[����
	if (pKey->GetTrigger(DIK_DOWN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_DOWN))
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
				// �J���[�Z�b�g
				m_pTitleui[nCnt]->SetCol(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
			}
			else
			{
				// �J���[�Z�b�g
				m_pTitleui[nCnt]->SetCol(COLOR_WHITE);
			}
		}
	}

	// ���菈��
	if ((pKey->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A)) && pCamera->GetFinishRotation())
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

#if 0

// ����W��ݒ�
D3DXVECTOR3 CenterPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 480.0f, 0.0f);

// �^�C�g����ui�𐶐�
for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
{
	// �����̊Ԋu�󂯂�
	CenterPos.y += nCnt * DIGITPOS;

	// ui�𐶐� ( �I�����j���[�� )
	m_pTitleui[nCnt] = CTitleUi::Create(CenterPos, COLOR_WHITE, UIWIDTH, UIHEIGHT, nCnt);
}

#endif