//====================================
//
// �^�C�g������ [ title.h ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "title.h"
#include "manager.h"
#include "game.h"

CTitle::CTitle() : CScene(CScene::MODE_TITLE)
{

}

CTitle::~CTitle()
{
}

HRESULT CTitle::Init(void)
{
	return S_OK;
}

void CTitle::Uninit(void)
{
	
}

void CTitle::Update(void)
{
	int n = 0;

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y))
	{
		// �Q�[���V�[���ɑJ��
		CManager::SetScene(new CGame());
	}
}

void CTitle::Draw(void)
{

}

CTitle* CTitle::Create(void)
{
	CTitle* pTitle = new CTitle;

	pTitle->Init();

	return pTitle;
}
