//=========================================
//
// タイトル管理処理 [ titlemanager.h ]
// Author: Asuma Nishio
// 
//==========================================

//***************************
// インクルードファイル宣言
//***************************
#include "titlemanager.h"
#include "titleui.h"

//============================
// コンストラクタ
//============================
CTitleManager::CTitleManager()
{
	// 値のクリア
	m_nIdx = NULL;

	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		m_pTitleui[nCnt] = nullptr;
	}
}
//============================
// デストラクタ
//============================
CTitleManager::~CTitleManager()
{
	// 無し
}
//============================
// 初期化処理
//============================
HRESULT CTitleManager::Init(void)
{
	// 基準座標を設定
	D3DXVECTOR3 CenterPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// タイトルのuiを生成
	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		// 高さの間隔空ける
		CenterPos.y += nCnt * 180.0f;

		// uiを生成 ( 選択メニュー分 )
		m_pTitleui[nCnt] = CTitleUi::Create(CenterPos, COLOR_WHITE, 300.0f, 60.0f, nCnt);
	}

	// 初期化結果を返す
	return S_OK;
}
//============================
// 終了処理
//============================
void CTitleManager::Uninit(void)
{
	// 無し
}
//============================
// 更新処理
//============================
void CTitleManager::Update(void)
{
	// 選択に応じて

}

void CTitleManager::Draw(void)
{

}
