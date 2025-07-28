//====================================
//
// タイトル処理 [ title.h ]
// Author: Asuma Nishio
// 
// TODO : シーンの流れを管理する
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
	// インスタンス生成
	m_pTitleManager = new CTitleManager;
	m_pTitleManager->Init();

	return S_OK;
}
//=====================================
// 終了処理
//=====================================
void CTitle::Uninit(void)
{
	// nullチェック
	if (m_pTitleManager != nullptr)
	{
		// 終了処理
		m_pTitleManager->Uninit();

		// 破棄
		delete m_pTitleManager;

		// nullptrする
		m_pTitleManager = nullptr;
	}
}
//=====================================
// 更新処理
//=====================================
void CTitle::Update(void)
{
	// nullじゃなかったら
	if (m_pTitleManager != nullptr)
	{
		// マネージャーの更新処理
		m_pTitleManager->Update();
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
