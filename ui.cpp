//====================================
//
// UI処理 [ ui.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "ui.h"
#include "texture.h"
#include "manager.h"

//===============================
// オーバーロードコンストラクタ
//===============================
CUi::CUi(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_nTexIdxType = NULL;
}
//===============================
// デストラクタ
//===============================
CUi::~CUi()
{
	// 無し
}
//===============================
// 初期化処理
//===============================
HRESULT CUi::Init(void)
{
	// オブジェクトの初期化
	CObject2D::Init();

	return S_OK;
}
//===============================
// 終了処理
//===============================
void CUi::Uninit(void)
{
	// オブジェクトの終了
	CObject2D::Uninit();
}
//===============================
// 更新処理
//===============================
void CUi::Update(void)
{
	// オブジェクトの更新
	CObject2D::Update();
}
//===============================
// 描画処理
//===============================
void CUi::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャセット
	CTexture* pTexture = CManager::GetTexture();

	// 割り当て
	pDevice->SetTexture(0, pTexture->GetAddress(m_nTexIdxType));

	// オブジェクトの描画
	CObject2D::Draw();
}
//===============================
// 生成処理
//===============================
CUi* CUi::Create(D3DXVECTOR3 pos,int nType,float fWidth,float fHeight)
{
	// インスタンス生成
	CUi* pUi = new CUi;

	// テクスチャ設定
	pUi->SetTexture(nType);

	// 初期化失敗時
	if (FAILED(pUi->Init()))
	{
		// ポインタの破棄
		delete pUi;

		// nullptr代入
		pUi = nullptr;

		// nullポインタを返す
		return nullptr;
	}

	// オブジェクト設定
	pUi->SetPos(pos);
	pUi->SetSize(fWidth, fHeight);

	// 生成されたポインタを返す
	return pUi;
}
//===============================
// テクスチャセット処理
//===============================
void CUi::SetTexture(int nType)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャ設定
	switch (nType)
	{
	case SCENETYPE_NONE:
		m_nTexIdxType = pTexture->Register("data\\TEXTURE\\billboard_wepon.png");
		break;

	default:
		break;
	}
}
