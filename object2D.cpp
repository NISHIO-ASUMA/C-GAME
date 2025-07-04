//====================================
//
// 2Dオブジェクト処理 [ object2D.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "object2D.h"
#include "renderer.h"
#include "Manager.h"
#include "player.h"
#include "enemy.h"

//===============================
// オーバーロードコンストラクタ
//===============================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_Pos = VECTOR3_NULL;
	m_fWidth = NULL;
	m_fHeight = NULL;
	m_Rot = VECTOR3_NULL;
	m_col = COLOR_WHITE;
}
//===============================
// デストラクタ
//===============================
CObject2D::~CObject2D()
{
	// 無し
}
//===============================
// 生成処理
//===============================
CObject2D* CObject2D::Create(void)
{
	// ポインタ宣言
	CObject2D* pObject2D;

	// インスタンス生成
	pObject2D = new CObject2D;

	// 初期化処理
	pObject2D->Init();

	return pObject2D;
}
//===============================
// オブジェクト2D初期化処理
//===============================
HRESULT CObject2D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;	

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// rhwの設定(1.0fで固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//===============================
// オブジェクト2D終了処理
//===============================
void CObject2D::Uninit(void)
{
	// テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//===============================
// オブジェクト2D更新処理
//===============================
void CObject2D::Update(void)
{
	VERTEX_2D* pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_fWidth, m_Pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_fWidth, m_Pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - m_fWidth, m_Pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_fWidth, m_Pos.y + m_fHeight, 0.0f);

	// rhwの設定(1.0fで固定)
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}
//===============================
// オブジェクト2D描画処理
//===============================
void CObject2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャを戻す
	pDevice->SetTexture(0, NULL);
}
//======================================
// UV座標設定関数
//======================================
void CObject2D::SetUV(float TexU,float TexV)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;	

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, TexV);
	pVtx[3].tex = D3DXVECTOR2(TexU, TexV);

	// アンロック
	m_pVtxBuff->Unlock();
}
