//====================================
//
// マネージャー処理 [ Manager.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**************************
// インクルードファイル宣言
//**************************
#include "Manager.h"
#include "object2D.h"
#include "enemy.h"
#include "score.h"
#include "time.h"
#include "objectX.h"
#include "billboard.h"
#include "block.h"
#include "meshdome.h"
#include "meshfield.h"
#include "player.h"

//**************************
// 静的メンバ変数宣言
//**************************
CRenderer* CManager::m_pRenderer = nullptr;				// レンダラーへのポインタ
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;	// キーボードへのポインタ
CJoyPad* CManager::m_pJoyPad = nullptr;					// ジョイパッドクラスへのポインタ
CSound* CManager::m_pSound = nullptr;					// サウンドへのポインタ
CInputMouse* CManager::m_pInputMouse = nullptr;			// マウスへのポインタ
CEnemymanager* CManager::m_pEnemyManager = nullptr;		// 敵マネージャーへのポインタ
CTexture* CManager::m_pTexture = nullptr;				// テクスチャクラスへのポインタ
CCamera* CManager::m_pCamera = nullptr;					// カメラクラスへのポインタ
CLight* CManager::m_pLight = nullptr;					// ライトクラスへのポインタ
CObject3D* CManager::m_pobj = nullptr;
CBlockManager* CManager::m_pBlockManager = nullptr;
CBlock* CManager::m_pBlock = nullptr;
CCollision* CManager::m_pCollision = nullptr;
CMeshCylinder* CManager::m_pMeshCylinder = nullptr;
CBoss* CManager::m_pBoss = nullptr;

//===========================
// コンストラクタ
//===========================
CManager::CManager()
{
	// 無し
}
//===========================
// デストラクタ
//===========================
CManager::~CManager()
{
	// 無し
}
//===========================
// マネージャーの初期化処理
//===========================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// キーボードオブジェクトの生成処理
	m_pInputKeyboard = new CInputKeyboard;

	// 初期化に失敗したら
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		// -1を返す
		return -1;
	}

	// ジョイパッドオブジェクトの生成処理
	m_pJoyPad = new CJoyPad;

	// 初期化に失敗したら
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd)))
	{
		// -1を返す
		return -1;
	}

	// マウスオブジェクトの生成処理
	m_pInputMouse = new CInputMouse;

	// 初期化に失敗したら
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		// -1を返す
		return -1;
	}

	// サウンドの生成処理
	m_pSound = new CSound;

	if (FAILED(m_pSound->Init(hWnd)))
	{
		// -1を返す
		return -1;
	}

	// レンダラーの生成処理
	m_pRenderer = new CRenderer;

	// 初期化に失敗したら
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		// -1を返す
		return -1;
	}

	// カメラ生成
	m_pCamera = new CCamera;

	// 初期化に失敗したら
	if (FAILED(m_pCamera->Init()))
	{
		// -1を返す
		return -1;
	}

	// ライト生成
	m_pLight = new CLight;

	// 初期化に失敗したら
	if (FAILED(m_pLight->Init()))
	{
		// -1を返す
		return -1;
	}

	// テクスチャ生成
	m_pTexture = new CTexture;

	// テクスチャ全読み込み
	m_pTexture->Load();

	// スコアの生成
	CScore::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0f), 120.0f, 60.0f);

	// シリンダー生成
	m_pMeshCylinder = m_pMeshCylinder->Create(D3DXVECTOR3(0.0f, -20.0f, 0.0f), 550.0f);

	// ドーム生成
	CMeshDome::Create(D3DXVECTOR3(0.0f,-70.0f,0.0f), 800.0f);

	// フィールド生成
	CMeshField::Create(D3DXVECTOR3(0.0f, -150.0f, 0.0f), 2000.0f);

	// ボス生成
	m_pBoss = m_pBoss->Create(D3DXVECTOR3(0.0f, -600.0f, 0.0f),60.0f);

	// プレイヤー生成
	CPlayer::Create(VECTOR3_NULL, VECTOR3_NULL, 20, 0, "data\\Player100motion.txt");

	// プレイヤー生成
	CPlayer::Create(VECTOR3_NULL, VECTOR3_NULL, 20, 0, "data\\Player200motion.txt");

	// ブロック配置
	m_pBlock = m_pBlock->Create("data\\MODEL\\STAGEOBJ\\Field000.x", D3DXVECTOR3(0.0f, -90.0f, 0.0f), VECTOR3_NULL,80.0f);

	return S_OK;
}
//===========================
// マネージャーの終了処理
//===========================
void CManager::Uninit(void)
{
	// 全オブジェクトの破棄
	CObject::ReleaseAll();

	// NULLチェック
	if (m_pEnemyManager != nullptr)
	{
		// 敵マネージャーの終了処理
		m_pEnemyManager->Uninit();

		// 敵マネージャーの破棄
		delete m_pEnemyManager;

		// NULLにする
		m_pEnemyManager = nullptr;
	}

	// NULLチェック
	if (m_pInputKeyboard != nullptr)
	{
		// キーボードの終了処理
		m_pInputKeyboard->Uninit();
		
		// キーボードの破棄
		delete m_pInputKeyboard;

		// NULLにする
		m_pInputKeyboard = nullptr;
	}

	// NULLチェック
	if (m_pJoyPad != nullptr)
	{
		// ジョイパッドの終了処理
		m_pJoyPad->Uninit();
		
		// ジョイパッドの破棄
		delete m_pJoyPad;

		// NULLにする
		m_pJoyPad = nullptr;
	}

	// NULLチェック
	if (m_pInputMouse != nullptr)
	{
		// マウスの終了処理
		m_pInputMouse->Uninit();

		// マウスの棄
		delete m_pInputMouse;

		// NULLにする
		m_pInputMouse = nullptr;
	}


	// NULLチェック
	if (m_pSound != nullptr)
	{
		// サウンドの終了処理
		m_pSound->Uninit();

		// サウンドの破棄
		delete m_pSound;

		// NULLにする
		m_pSound = nullptr;
	}

	// カメラの破棄
	if (m_pCamera != nullptr)
	{
		// 終了処理
		m_pCamera->Uninit();

		// 破棄
		delete m_pCamera;

		// NULLにする
		m_pCamera = nullptr;

	}

	//  ライトの破棄
	if (m_pLight != nullptr)
	{
		// 終了処理
		m_pLight->Uninit();

		// 破棄
		delete m_pLight;

		// NULLにする
		m_pLight = nullptr;

	}

	// テクスチャ破棄
	if (m_pTexture != nullptr)
	{
		// すべてのテクスチャの破棄
		m_pTexture->UnLoad();

		// ポインタの破棄
		delete m_pTexture;

		// nullptr代入
		m_pTexture = nullptr;
	}

	// NULLチェック
	if (m_pRenderer != nullptr)
	{
		// レンダラーの終了処理
		m_pRenderer->Uninit();

		// レンダラーの破棄
		delete m_pRenderer;

		// NULLにする
		m_pRenderer = nullptr;
	}
}
//===========================
// マネージャーの更新処理
//===========================
void CManager::Update()
{
	// キーボードの更新処理
	m_pInputKeyboard->Update();

	// ジョイパッドの更新処理
	m_pJoyPad->Update();

	// マウスの更新処理
	m_pInputMouse->Update();

	//カメラ更新
	m_pCamera->Update();

	// レンダラーの更新処理
	m_pRenderer->Update();

#ifdef _DEBUG
	// ブロックマネージャー更新
	m_pBlockManager->Update();
#endif
}
//===========================
// マネージャーの描画処理
//===========================
void CManager::Draw(void)
{
	// レンダラーの描画処理
	m_pRenderer->Draw();
}
//===========================
// レンダラーの取得
//===========================
CRenderer* CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//===========================
// キーボードの取得
//===========================
CInputKeyboard* CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//===========================
// ジョイパッドの取得
//===========================
CJoyPad* CManager::GetJoyPad(void)
{
	return m_pJoyPad;
}
//===========================
// サウンドの取得
//===========================
CSound* CManager::GetSound(void)
{
	return m_pSound;
}
//===========================
// マウスの取得
//===========================
CInputMouse* CManager::GetMouse(void)
{
	return m_pInputMouse;
}
//===========================
// 敵マネージャーの取得
//===========================
CEnemymanager* CManager::GetEnemyManager(void)
{
	return m_pEnemyManager;
}
//===========================
// テクスチャの取得
//===========================
CTexture* CManager::GetTexture(void)
{
	return m_pTexture;
}
//===========================
// カメラの取得
//===========================
CCamera* CManager::GetCamera(void)
{
	return m_pCamera;
}
//===========================
// ライトの取得
//===========================
CLight* CManager::GetLight(void)
{
	return m_pLight;
}
//===========================
// 3Dオブジェクトの取得
//===========================
CObject3D* CManager::GetObj3D(void)
{
	return m_pobj;
}