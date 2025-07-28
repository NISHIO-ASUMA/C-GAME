//=========================================
//
// ボス体力処理 [ bosslifegage.h ]
// Author: Asuma Nishio
//
//=========================================

//**********************
// インクルードファイル
//**********************
#include "bosslifegage.h"

//===============================
// オーバーロードコンストラクタ
//===============================
CBossLifeGage::CBossLifeGage(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_nIdxTex = NULL;
	m_pBoss = nullptr;
}
//===============================
// デストラクタ
//===============================
CBossLifeGage::~CBossLifeGage()
{
	// 無し
}
//===============================
// 生成処理
//===============================
CBossLifeGage* CBossLifeGage::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	// インスタンス生成
	CBossLifeGage* pBossLife = new CBossLifeGage;

	// nullだったら
	if (pBossLife == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pBossLife->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pBossLife;
}
//===============================
// 初期化処理
//===============================
HRESULT CBossLifeGage::Init(void)
{
	return S_OK;
}
//===============================
// 終了処理
//===============================
void CBossLifeGage::Uninit(void)
{

}
//===============================
// 更新処理
//===============================
void CBossLifeGage::Update(void)
{

}
//===============================
// 描画処理
//===============================
void CBossLifeGage::Draw(void)
{

}
