//======================================
//
// UI全体を管理する処理 [ uimanager.cpp ]
// Author: Asuma Nishio
//
//======================================

//****************************
// インクルードファイル宣言
//****************************
#include "uimanager.h"

//****************************
// 静的メンバ変数宣言
//****************************
std::vector<CUi*>CUimanager::m_Uiobject = {};
int CUimanager::m_nIdxUi = NULL;

//===============================
// コンストラクタ
//===============================
CUimanager::CUimanager()
{
	m_Uiobject.clear();
}
//===============================
// デストラクタ
//===============================
CUimanager::~CUimanager()
{
	// 無い
}
//===============================
// 生成処理
//===============================
CUi* CUimanager::Create(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight)
{
	// ポインタ生成
	CUi* pNewUi = CUi::Create(pos, nType, fWidth, fHeight);

	if (pNewUi)
	{
		// 配列追加
		m_Uiobject.push_back(pNewUi);

		// インデックス加算
		m_nIdxUi++;

		// 生成されたポインタを返す
		return pNewUi;
	}
	else
	{
		return nullptr;
	}
}
//===============================
// 初期化処理
//===============================
HRESULT CUimanager::Init(void)
{
	return S_OK;
}
//===============================
// 終了処理
//===============================
void CUimanager::Uninit(void)
{
	// 配列クリア
	m_Uiobject.clear();
}
//===============================
// 更新処理
//===============================
void CUimanager::Update(void)
{

}
