//====================================
//
// タイトル処理 [ title.h ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// インクルードファイル宣言
//****************************
#include "title.h"
#include "manager.h"
#include "game.h"
#include "bg.h"

CTitle::CTitle() : CScene(CScene::MODE_TITLE)
{

}

CTitle::~CTitle()
{
}

HRESULT CTitle::Init(void)
{
	// 背景生成
	CBg::Create(1);

	return S_OK;
}

void CTitle::Uninit(void)
{
	
}

void CTitle::Update(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y))
	{
		// ゲームシーンに遷移
		CManager::GetFade()->SetFade(new CGame());
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
