//====================================
//
// メインゲーム処理 [ game.h ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// インクルードファイル宣言
//****************************
#include "game.h"
#include "manager.h"
#include "debugproc.h"
#include "title.h"
#include "result.h"
#include "gamemanager.h"

//**************************
// 静的メンバ変数宣言
//**************************
CPauseManager* CGame::m_pPausemanager = nullptr;

//==================================
// コンストラクタ
//==================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// 値のクリア
	m_pGameManager = nullptr;
}
//==================================
// デストラクタ
//==================================
CGame::~CGame()
{
	// 無し
}
//==================================
// 初期化処理
//==================================
HRESULT CGame::Init(void)
{
	// ポーズマネージャー生成
	m_pPausemanager = new CPauseManager;

	// nullだったら
	if (m_pPausemanager == nullptr)
	{
		return E_FAIL;
	}

	// ポーズマネージャー初期化処理
	m_pPausemanager->Init();

	// ゲームマネージャー生成
	m_pGameManager = new CGameManager;

	// nullだったら
	if (m_pGameManager == nullptr)
	{
		return E_FAIL;
	}

	// ゲームマネージャー初期化処理
	m_pGameManager->Init();

	// 初期化結果を返す
	return S_OK;
}
//==================================
// 終了処理
//==================================
void CGame::Uninit(void)
{
	// nullチェック
	if (m_pPausemanager != nullptr)
	{
		// 終了処理
		m_pPausemanager->Uninit();

		// ポインタの破棄
		delete m_pPausemanager;

		// nullptrにする
		m_pPausemanager = nullptr;
	}

	// nullチェック
	if (m_pGameManager != nullptr)
	{
		// 終了処理
		m_pGameManager->Uninit();

		// ポインタの破棄
		delete m_pGameManager;

		// nullptrにする
		m_pGameManager = nullptr;
	}
}
//==================================
// 更新処理
//==================================
void CGame::Update(void)
{	
	// ポーズのキー入力判定
	m_pPausemanager->SetEnablePause();
	
	// ポーズの更新処理
	m_pPausemanager->Update();
	
	// falseの時に更新
	if (m_pPausemanager->GetPause() == false)
	{
		// ゲームマネージャー更新
		m_pGameManager->Update();

		// 検証用画面遷移
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
		{
			// フェード取得
			CFade* pFade = CManager::GetFade();

			// 取得できたら
			if (pFade != nullptr)
			{
				// 画面遷移
				pFade->SetFade(new CResult());
			}
		}

		//// 経過時間を取得
		//int Numtime = m_pTime->GetAllTime();

		//if (Numtime <= 0)
		//{
		//	// フェード取得
		//	CFade* pFade = CManager::GetFade();

		//	if (pFade != nullptr)
		//	{
		//		// リザルトシーンに遷移
		//		pFade->SetFade(new CResult());
		// 
		//		return;
		//	}
		//}
	}
}
//==================================
// 描画処理
//==================================
void CGame::Draw(void)
{
	// 無し
}
//==================================
// コンストラクタ
//==================================
CGame* CGame::Create(void)
{	
	// インスタンス生成
	CGame* pGame = new CGame;

	// nullptrだったら
	if (pGame == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pGame->Init()))
	{
		// ポインタを返す
		return nullptr;
	}

	// 生成されたポインタを返す
    return pGame;
}
