//============================================
//
// ゲージ関係のクラス処理 [ gage.cpp ]
// Author: Asuma Nishio
//
//=============================================

//**********************
// インクルードファイル
//**********************
#include "gage.h"
#include "texture.h"
#include "manager.h"

//===============================
// オーバーロードコンストラクタ
//===============================
CGage::CGage(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_nTextIdx = NULL;
}
//===============================
// デストラクタ
//===============================
CGage::~CGage()
{
	// 無し
}
//===============================
// 初期化処理
//===============================
HRESULT CGage::Init(void)
{
	// 親クラスの初期化
	CObject2D::Init();

	return S_OK;
}
//===============================
// 終了処理
//===============================
void CGage::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//===============================
// 更新処理
//===============================
void CGage::Update(void)
{
	// 親クラスの更新
	CObject2D::Update();
}
//===============================
// 描画処理
//===============================
void CGage::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャ読み込み
	pDevice->SetTexture(0, pTexture->GetAddress(m_nTextIdx));

	// 親クラスの描画
	CObject2D::Draw();
}
//===============================
// 描画処理
//===============================
CGage* CGage::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// インスタンス生成
	CGage* pGage = new CGage;

	// 生成できなかったら
	if (pGage == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pGage->Init()))
	{
		// nullポインタを返す
		return nullptr;
	}

	// 2Dオブジェクト設定
	pGage->SetPos(pos);
	pGage->SetSize(fWidth, fHeight);
	pGage->SetTexture();

	// ポインタを返す
	return pGage;
}
//===================================
// テクスチャ設定関数
//===================================
void CGage::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャ割り当て
	m_nTextIdx = pTexture->Register("data\\TEXTURE\\lifebar_gage.png");
}
