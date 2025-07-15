//====================================
//
// エディター処理 [ uiedit.cpp ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// インクルードファイル宣言
//****************************
#include "uiedit.h"
#include "input.h"
#include "manager.h"

//===============================
// コンストラクタ
//===============================
CUiedit::CUiedit()
{

}
//===============================
// デストラクタ
//===============================
CUiedit::~CUiedit()
{

}
//===============================
// 初期化処理
//===============================
HRESULT CUiedit::Init(void)
{
	return S_OK;
}
//===============================
// 終了処理
//===============================
void CUiedit::Uninit(void)
{
	// nullptrチェック
	if ( != nullptr)
}
//===============================
// 更新処理
//===============================
void CUiedit::Update(void)
{
	// キー入力用の変数
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// 配置設定
	if (pInput->GetTrigger(DIK_A))
	{

	}
	else if (pInput->GetTrigger(DIK_D))
	{

	}
	else if (pInput->GetTrigger(DIK_W))
	{

	}
	else if (pInput->GetTrigger(DIK_S))
	{

	}

}
//===============================
// 描画処理
//===============================
void CUiedit::Draw(void)
{

}
