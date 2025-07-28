//====================================
//
// �^�C�g������ [ title.h ]
// Author: Asuma Nishio
// 
// TODO : �V�[���̗�����Ǘ�����
//
//=====================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "title.h"
#include "manager.h"
#include "game.h"
#include "ui.h"

//=====================================
// �R���X�g���N�^
//=====================================
CTitle::CTitle() : CScene(CScene::MODE_TITLE)
{
	//����
}
//=====================================
// �f�X�g���N�^
//=====================================
CTitle::~CTitle()
{
	// ����
}
//=====================================
// ����������
//=====================================
HRESULT CTitle::Init(void)
{
	// �C���X�^���X����
	m_pTitleManager = new CTitleManager;
	m_pTitleManager->Init();

	return S_OK;
}
//=====================================
// �I������
//=====================================
void CTitle::Uninit(void)
{
	// null�`�F�b�N
	if (m_pTitleManager != nullptr)
	{
		// �I������
		m_pTitleManager->Uninit();

		// �j��
		delete m_pTitleManager;

		// nullptr����
		m_pTitleManager = nullptr;
	}
}
//=====================================
// �X�V����
//=====================================
void CTitle::Update(void)
{
	// null����Ȃ�������
	if (m_pTitleManager != nullptr)
	{
		// �}�l�[�W���[�̍X�V����
		m_pTitleManager->Update();
	}
}
//=====================================
// �`�揈��
//=====================================
void CTitle::Draw(void)
{
	// ����
}
//=====================================
// ��������
//=====================================
CTitle* CTitle::Create(void)
{
	// �C���X�^���X����
	CTitle* pTitle = new CTitle;

	// nullptr��������
	if (pTitle == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pTitle->Init()))
	{
		return nullptr;
	}
	
	// �������ꂽ�|�C���^��Ԃ�
	return pTitle;
}
