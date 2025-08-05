//=====================================
//
// 攻撃前兆演出 [ attacksign.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "attacksign.h"
#include "manager.h"
#include "texture.h"

//==============================
// コンストラクタ
//==============================
CAttackSign::CAttackSign(int nPriority) : CBillboard(nPriority)
{
	// 値のクリア
	m_nidxTex = NULL;
}
//==============================
// デストラクタ
//==============================
CAttackSign::~CAttackSign()
{
	// 無し
}
//==============================
// 初期化処理
//==============================
HRESULT CAttackSign::Init(void)
{
	// 親クラスの初期化処理
	CBillboard::Init();

	// 初期化結果を返す
	return S_OK;
}
//==============================
// 終了処理
//==============================
void CAttackSign::Uninit(void)
{
	// 親クラスの終了処理
	CBillboard::Uninit();
}
//==============================
// 更新処理
//==============================
void CAttackSign::Update(void)
{
	// オブジェクト更新
	CBillboard::Update();
}
//==============================
// 描画処理
//==============================
void CAttackSign::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ取得
	CTexture* pTexture = CManager::GetTexture();

	// nullチェック
	if (pTexture == nullptr) return;

	// テクスチャセット
	pDevice->SetTexture(0, pTexture->GetAddress(m_nidxTex));

	// オブジェクト描画
	CBillboard::Draw();
}
//==============================
// テクスチャ割り当て
//==============================
void CAttackSign::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	// nullチェック
	if (pTexture == nullptr) return;

	// テクスチャ割りあて
	// m_nidxTex = pTexture->Register();
}
//==============================
// 生成処理
//==============================
CAttackSign* CAttackSign::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// インスタンス生成
	CAttackSign* pAttack = new CAttackSign;

	// nullチェック
	if (pAttack == nullptr) return nullptr;

	// オブジェクト設定
	pAttack->SetPos(pos);
	pAttack->SetSize(fWidth, fHeight);
	pAttack->SetTexture();

	// 初期化失敗時
	if (FAILED(pAttack->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pAttack;
}