//=====================================
//
// チュートリアル処理 [ tutorial.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "tutorial.h"
#include "tutorialmanager.h"

//===============================
// オーバーロードコンストラクタ
//===============================
CTutorial::CTutorial() : CScene(CScene::MODE_TUTORIAL)
{
	// 値のクリア
	m_pTutoManager = nullptr;
}
//===============================
// デストラクタ
//===============================
CTutorial::~CTutorial()
{
	// 無し
}
//===============================
// 初期化処理
//===============================
HRESULT CTutorial::Init(void)
{
	// マネージャーのインスタンス生成
	m_pTutoManager = new CTutorialManager;

	// nullではなかったら
	if (m_pTutoManager != nullptr) m_pTutoManager->Init();

	// 初期化結果を返す
	return S_OK;
}
//===============================
// 終了処理
//===============================
void CTutorial::Uninit(void)
{
	// nullチェック
	if (m_pTutoManager != nullptr)
	{
		// 終了処理
		m_pTutoManager->Uninit();

		// ポインタの破棄
		delete m_pTutoManager;

		// nullptrにする
		m_pTutoManager = nullptr;
	}
}
//===============================
// 更新処理
//===============================
void CTutorial::Update(void)
{
	// ポインタが存在するなら
	if (m_pTutoManager != nullptr)
	{
		// マネージャーの更新処理
		m_pTutoManager->Update();
	}
}
//===============================
// 描画処理
//===============================
void CTutorial::Draw(void)
{
	// 無し
}
//===============================
// 生成処理
//===============================
CTutorial* CTutorial::Create(void)
{
	// インスタンス生成
	CTutorial* pTutorial = new CTutorial;

	// 生成失敗時
	if (pTutorial == nullptr) return nullptr;

	// もし初期化に失敗したら
	if (FAILED(pTutorial->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pTutorial;
}
