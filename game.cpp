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
#include "score.h"
#include "time.h"
#include "meshdome.h"
#include "meshfield.h"
#include "player.h"
#include "meshimpact.h"
#include "debugproc.h"
#include "title.h"
#include "bullethorming.h"
#include "playerlifegage.h"
#include "enemy.h"
#include "result.h"

//**************************
// 静的メンバ変数宣言
//**************************
CBlock* CGame::m_pBlock = nullptr;				// ブロック
CMeshCylinder* CGame::m_pMeshCylinder = nullptr;	// 円柱
CBoss* CGame::m_pBoss = nullptr;					// ボス
CPauseManager* CGame::m_pPausemanager = nullptr;
CTime* CGame::m_pTime = nullptr;

//==================================
// コンストラクタ
//==================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// 値のクリア
	m_bPause = false;
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

	// 初期化処理
	m_pPausemanager->Init();

	// スコアの生成
	CScore::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// タイマー生成
	m_pTime = CTime::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// シリンダー生成
	m_pMeshCylinder = CMeshCylinder::Create(D3DXVECTOR3(0.0f, -20.0f, 0.0f), 550.0f);

	// ドーム生成
	CMeshDome::Create(D3DXVECTOR3(0.0f, -70.0f, 0.0f), 800.0f);

	// フィールド生成
	CMeshField::Create(D3DXVECTOR3(0.0f, -150.0f, 0.0f), 2000.0f);

	// ボス生成
	m_pBoss = CBoss::Create(D3DXVECTOR3(0.0f, -600.0f, 0.0f), 60.0f);

	// プレイヤー生成
	CPlayer::Create(D3DXVECTOR3(0.0f,0.0f,-550.0f), VECTOR3_NULL, 10, 0, "data\\Player100motion.txt");

	// プレイヤー生成
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 550.0f), VECTOR3_NULL, 10, 1, "data\\Player200motion.txt");

	// ブロック配置
	m_pBlock = CBlock::Create("data\\MODEL\\STAGEOBJ\\Field000.x", D3DXVECTOR3(0.0f, -90.0f, 0.0f), VECTOR3_NULL, 80.0f);

	// 体力ゲージ生成
	CPlayerLifeGage::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 450.0f, 40.0f, CPlayerLifeGage::GAGE_FRAME);
	CPlayerLifeGage::Create(D3DXVECTOR3(0.0f, 2.0f, 0.0f), 450.0f, 40.0f, CPlayerLifeGage::GAGE_BAR);

	// 敵生成
	CEnemy::Create("data\\MODEL\\ATTACKMODEL\\SpikeEnemy000.x", D3DXVECTOR3(0.0f, 0.0f, -550.0f), VECTOR3_NULL, CEnemy::TYPE_NONE, 2);

	return S_OK;
}
//==================================
// 終了処理
//==================================
void CGame::Uninit(void)
{
	m_pBlock = nullptr;
	m_pBoss = nullptr;
	m_pMeshCylinder = nullptr;
	m_pTime = nullptr;

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
}
//==================================
// 更新処理
//==================================
void CGame::Update(void)
{	
	// キー入力
	m_pPausemanager->SetEnablePause();
	
	// ポーズの更新処理
	m_pPausemanager->Update();
	
	if (m_pPausemanager->GetPause() == false)
	{
		// 検証用インパクト
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_H))
		{
			// 衝撃波を生成
			CMeshImpact::Create(VECTOR3_NULL, 80, 100.0f, 40.0f, 7.0f);
		}

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

		// 検証用弾
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_L))
		{
			// ホーミング弾を生成
			CBulletHorming::Create("data\\MODEL\\ATTACKMODEL\\bulletobject000.x", D3DXVECTOR3(0.0f, 400.0f, 0.0f));
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
