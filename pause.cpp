//====================================
//
// ポーズ処理 [ pause.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "pause.h"

//================================
// コンストラクタ
//================================
CPause::CPause(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
}
//================================
// デストラクタ
//================================
CPause::~CPause()
{
	// 無し
}
//================================
// 生成処理
//================================
CPause* CPause::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col)
{
	// インスタンス生成
	CPause* pPause = new CPause;

	// nullptrだったら
	if (pPause == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pPause->Init()))
	{
		return nullptr;
	}

	// 2Dオブジェクト設定
	pPause->SetPos(pos);
	pPause->SetSize(fWidth, fHeight);
	pPause->SetCol(col);

	// 生成されたポインタを返す
	return pPause;
}
//================================
// 初期化処理
//================================
HRESULT CPause::Init(void)
{
	// オブジェクトの初期化
	CObject2D::Init();

	// 初期化結果を返す
	return S_OK;
}
//================================
// 終了処理
//================================
void CPause::Uninit(void)
{
	// オブジェクトの破棄
	CObject2D::Uninit();
}
//================================
// 更新処理
//================================
void CPause::Update(void)
{
	// オブジェクトの更新処理
	CObject2D::Update();
}
//================================
// 描画処理
//================================
void CPause::Draw(void)
{
	// オブジェクトの描画処理
	CObject2D::Draw();
}
