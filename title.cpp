//====================================
//
// タイトル処理 [ title.h ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// インクルードファイル宣言
//****************************
#include "title.h"
#include "manager.h"
#include "game.h"
#include "ui.h"

//=====================================
// コンストラクタ
//=====================================
CTitle::CTitle() : CScene(CScene::MODE_TITLE)
{
	//無し
}
//=====================================
// デストラクタ
//=====================================
CTitle::~CTitle()
{
	// 無し
}
//=====================================
// 初期化処理
//=====================================
HRESULT CTitle::Init(void)
{
	// UI生成
	CUi::Create(VECTOR3_NULL, 0, 500.0f, 200.0f);

	return S_OK;
}
//=====================================
// 終了処理
//=====================================
void CTitle::Uninit(void)
{
	// 無し
}
//=====================================
// 更新処理
//=====================================
void CTitle::Update(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
	{
		// フェード取得
		CFade* pFade = CManager::GetFade();

		// 取得できたら
		if (pFade != nullptr) pFade->SetFade(new CGame());	// ゲームシーンに遷移
	}
}
//=====================================
// 描画処理
//=====================================
void CTitle::Draw(void)
{
	// 無し
}
//=====================================
// 生成処理
//=====================================
CTitle* CTitle::Create(void)
{
	// インスタンス生成
	CTitle* pTitle = new CTitle;

	// nullptrだったら
	if (pTitle == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pTitle->Init()))
	{
		return nullptr;
	}
	
	// 生成されたポインタを返す
	return pTitle;
}
