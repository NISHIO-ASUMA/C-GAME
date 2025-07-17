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

//**************************
// 静的メンバ変数宣言
//**************************
CBlock* CGame::m_pBlock = nullptr;				// ブロック
CMeshCylinder* CGame::m_pMeshCylinder = nullptr;	// 円柱
CBoss* CGame::m_pBoss = nullptr;					// ボス

//==================================
// コンストラクタ
//==================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// 無し
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
	// スコアの生成
	CScore::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0f), 120.0f, 60.0f);

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
}
//==================================
// 更新処理
//==================================
void CGame::Update(void)
{
	// TODO : 状態によってゲーム終了など
	
	// 検証用インパクト
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_H))
	{
		// 衝撃波を生成
		CMeshImpact::Create(VECTOR3_NULL, 80, 100.0f, 40.0f, 7.0f);
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
	{
		// 画面遷移
		CManager::SetScene(new CTitle());
	}

}
//==================================
// 描画処理
//==================================
void CGame::Draw(void)
{
	// ない
}
//==================================
// コンストラクタ
//==================================
CGame* CGame::Create(void)
{	
	// インスタンス生成
	CGame* pGame = new CGame;

	// 初期化失敗時
	if (FAILED(pGame->Init()))
	{
		// ポインタの破棄
		delete pGame;

		// nullptr代入
		pGame = nullptr;

		// ポインタを返す
		return nullptr;
	}

	// 生成されたポインタを返す
    return pGame;
}
