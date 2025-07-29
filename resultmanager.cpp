//==============================================
//
// リザルトシーン管理処理 [ resultmanager.cpp ]
// Author: Asuma Nishio
// 
//==============================================

//**************************
// インクルードファイル宣言
//**************************
#include "resultmanager.h"
#include "manager.h"
#include "input.h"
#include "title.h"

//=================================
// コンストラクタ
//=================================
CResultManager::CResultManager()
{
	// 値のクリア
}
//=================================
// デストラクタ
//=================================
CResultManager::~CResultManager()
{
	// 無し
}
//=================================
// 初期化処理
//=================================
HRESULT CResultManager::Init(void)
{
	// 初期化結果を返す
	return S_OK;
}
//=================================
// 終了処理
//=================================
void CResultManager::Uninit(void)
{
	// nullチェック
}
//=================================
// 更新処理
//=================================
void CResultManager::Update(void)
{
	// 入力デバイス
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// 決定キーが押された
	if (pInput->GetTrigger(DIK_RETURN))
	{
		// ポインタ取得
		CFade* pFade = CManager::GetFade();

		// nullじゃないとき
		if (pFade != nullptr)
		{
			// シーン遷移
			pFade->SetFade(new CTitle(false));
		}
	}
}
