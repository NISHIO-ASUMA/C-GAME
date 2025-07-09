//====================================
//
// ボス処理 [ boss.cpp ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// インクルードファイル宣言
//****************************
#include "boss.h"
#include "motion.h"
#include "debugproc.h"
#include "Manager.h"
#include <ctime>

//====================================
// オーバーロードコンストラクタ
//====================================
CBoss::CBoss(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_pMotion = nullptr;
	m_type = NULL;
	m_mtxworld = {};

	for (int nCnt = 0; nCnt < m_nNumModels; nCnt++)
	{
		m_pModel[nCnt] = nullptr;
	}

	m_fSize = NULL;
}
//====================================
// デストラクタ
//====================================
CBoss::~CBoss()
{
	// 無し
}
//====================================
// 生成処理
//====================================
CBoss* CBoss::Create(D3DXVECTOR3 pos,float fSize)
{
	// インスタンス生成
	CBoss* pBoss = new CBoss;

	// セット
	pBoss->m_pos = pos;
	pBoss->m_fSize = fSize;

	// 初期化失敗時
	if (FAILED(pBoss->Init()))
	{
		// 破棄
		delete pBoss;

		// nullptr代入
		pBoss = nullptr;

		// nullポインタを返す
		return nullptr;
	}

	// ポインタを返す
	return pBoss;
}
//====================================
// 初期化処理
//====================================
HRESULT CBoss::Init(void)
{
	// オブジェクトの種類をセット
	SetObjType(TYPE_BOSS);

	// タイプ代入
	m_type = TYPE_MAX;

	// モーションの読み込み
	m_pMotion = CMotion::Load("data\\MOTION\\Boss\\Boss_motion.txt", m_nNumModels, m_pModel,TYPE_MAX);

	// モーション数を設定
	m_pMotion->SetMotionNum(m_type);

	return S_OK;
}
//====================================
// 終了処理
//====================================
void CBoss::Uninit(void)
{
	// モデル数分の破棄
	for (int nCnt = 0; nCnt < m_nNumModels; nCnt++)
	{
		// nullptrチェック
		if (m_pModel[nCnt] != nullptr)
		{
			// 終了処理
			m_pModel[nCnt]->Uninit();

			// ポインタの破棄
			delete m_pModel[nCnt];

			// nullptrにする
			m_pModel[nCnt] = nullptr;
		}
	}

	// nullptrチェック
	if (m_pMotion != nullptr)
	{
		// ポインタの破棄
		delete m_pMotion;

		// nullptrにする
		m_pMotion = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//====================================
// 更新処理
//====================================
void CBoss::Update(void)
{
	// 乱数の種を設定
	srand((int)time(NULL));

	// 攻撃パターン数をランダム設定
	int nAttackPattern = rand() % 3;

	// モーションセット
	m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);

	// モーションの更新
	m_pMotion->Update(m_pModel, m_nNumModels);
}
//====================================
// 描画処理
//====================================
void CBoss::Draw(void)
{
#if 1
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxworld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// 全モデルパーツの描画
	for (int nCnt = 0; nCnt < m_nNumModels; nCnt++)
	{
		// 全モデル描画
		m_pModel[nCnt]->Draw();
	}
#endif
	// デバッグフォント
	CDebugproc::Print("ボス座標 [ %.2f ,%.2f , %.2f]", m_pos.x,m_pos.y,m_pos.z);
	CDebugproc::Draw(0, 40);
}
