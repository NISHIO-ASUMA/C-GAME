//=========================================
//
// ゲーム管理処理 [ gamemanager.cpp ]
// Author: Asuma Nishio
// 
//==========================================

//***************************
// インクルードファイル宣言
//***************************
#include "gamemanager.h"
#include "manager.h"
#include "score.h"
#include "time.h"
#include "meshdome.h"
#include "meshfield.h"
#include "player.h"
#include "meshimpact.h"
#include "playerlifegage.h"
#include "debugproc.h"
#include "bosslifegage.h"

//**************************
// 静的メンバ変数宣言
//**************************
CBlock* CGameManager::m_pBlock = nullptr;				// ブロック
CMeshCylinder* CGameManager::m_pMeshCylinder = nullptr;	// 円柱
CBoss* CGameManager::m_pBoss = nullptr;					// ボス
CTime* CGameManager::m_pTime = nullptr;					// タイマー

//========================
// コンストラクタ
//========================
CGameManager::CGameManager()
{
	// 値のクリア
}
//========================
// デストラクタ
//========================
CGameManager::~CGameManager()
{
	// 無し
}
//========================
// 初期化処理
//========================
HRESULT CGameManager::Init(void)
{
	// スコアの生成
	// CScore::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// シリンダー生成
	m_pMeshCylinder = CMeshCylinder::Create(D3DXVECTOR3(0.0f, -20.0f, 0.0f), 550.0f);

	// ドーム生成
	CMeshDome::Create(D3DXVECTOR3(0.0f, -70.0f, 0.0f), 800.0f);

	// フィールド生成
	CMeshField::Create(D3DXVECTOR3(0.0f, -150.0f, 0.0f), 2000.0f);

	// ボス生成
	m_pBoss = CBoss::Create(D3DXVECTOR3(0.0f, -600.0f, 0.0f), 60.0f, 1000);

	// プレイヤー生成
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -550.0f), VECTOR3_NULL, 10, 0, "data\\Player100motion.txt");
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 550.0f), VECTOR3_NULL, 10, 1, "data\\Player200motion.txt");

	// ブロック配置
	m_pBlock = CBlock::Create("data\\MODEL\\STAGEOBJ\\Field000.x", D3DXVECTOR3(0.0f, -90.0f, 0.0f), VECTOR3_NULL, 80.0f);

	// 体力ゲージ生成
	CPlayerLifeGage::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 450.0f, 40.0f, CPlayerLifeGage::GAGE_FRAME);
	CPlayerLifeGage::Create(D3DXVECTOR3(0.0f, 2.0f, 0.0f), 450.0f, 40.0f, CPlayerLifeGage::GAGE_BAR);

	// ボス体力ゲージ生成
	CBossLifeGage::Create(D3DXVECTOR3(770.0f, 0.0f, 0.0f), 0.0f, 0.0f, CBossLifeGage::TYPE_GAGE);
	CBossLifeGage::Create(D3DXVECTOR3(770.0f, 0.0f, 0.0f), SCREEN_WIDTH * 0.4f, 60.0f, CBossLifeGage::TYPE_FRAME);

	// タイマー生成
	m_pTime = CTime::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// 初期化結果を返す
	return S_OK;
}
//========================
// 終了処理
//========================
void CGameManager::Uninit(void)
{
	// 使用したポインタのnull初期化
	m_pBlock = nullptr;
	m_pBoss = nullptr;
	m_pMeshCylinder = nullptr;
	m_pTime = nullptr;
}
//========================
// 更新処理
//========================
void CGameManager::Update(void)
{
	// オブジェクトの更新は自動的に呼ばれる
}
