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
#include "manager.h"
#include <ctime>

//****************************
// 定数宣言
//****************************
constexpr float HITRANGE = 12.0f; // コリジョンサイズ

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
	m_nCoolTime = NULL;
	m_mtxworld = {};

	for (int nCnt = 0; nCnt < NUMMODELS; nCnt++)
	{
		m_pModel[nCnt] = nullptr;
	}

	m_fSize = NULL;

	m_isAttacked = false;
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
//=========================================
// モデルの特定部分パーツの取得関数
//=========================================
CModel* CBoss::GetModelPartType(CModel::PARTTYPE modelpart)
{
	// ボスが持っているモデルの数の中から探す
	for (int nModel = 0; nModel < NUMMODELS; nModel++)
	{
		// モデルがある かつ 取得したい物と一致していたら
		if (m_pModel[nModel] && m_pModel[nModel]->GetPartType() == modelpart)
		{
			// 該当モデルのidx番号のモデルを返す
			return m_pModel[nModel];
		}
	}

	// 該当なし
	return nullptr;
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

	m_nCoolTime = 120;

	// モーションの読み込み
	m_pMotion = CMotion::Load("data\\Boss\\Bossmotion.txt", NUMMODELS, m_pModel,TYPE_MAX);

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
	for (int nCnt = 0; nCnt < NUMMODELS; nCnt++)
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
	// フラグメント
	static bool isCreating = false;

	if (!isCreating)
	{
		// 乱数の種を一度だけ設定する
		srand((int)time(NULL));
		isCreating = true;
	}

	// クールタイム中なら待機モーション更新だけ
	if (m_nCoolTime > 0)
	{
		// クールタイムを減らす
		m_nCoolTime--;

		// もし現在攻撃モーションが終わっていればニュートラルに戻す
		if (m_pMotion->GetFinishMotion())
		{
			m_pMotion->SetMotion(TYPE_NEUTRAL);
		}

		// モーションの更新だけ行う
		m_pMotion->Update(m_pModel, NUMMODELS);

		return;
	}

	// モーション中か判別
	if (!m_pMotion->GetFinishMotion() && m_pMotion->GetMotionType() != CBoss::PATTERN_NONE)
	{
		// 攻撃モーション中なので、続ける
		m_pMotion->Update(m_pModel, NUMMODELS);

		// ここでかえす
		return;
	}

	// ランダムに行動パターンを決定する
	int nAttackPattern = rand() % 3;

	switch (nAttackPattern)
	{
	case PATTERN_NONE:
		m_pMotion->SetMotion(TYPE_NEUTRAL);
		break;

	case PATTERN_HAND:
		m_pMotion->SetMotion(TYPE_ACTION);
		m_nCoolTime = 60; 
		break;

	case PATTERN_BULLET:
		// 弾追尾モーション
		m_nCoolTime = 180;
		break;

	case PATTERN_CIRCLE:
		// 薙ぎ払いモーション
		m_nCoolTime = 150;
		break;

	default:
		break;
	}

	// モーション全体更新
	m_pMotion->Update(m_pModel, NUMMODELS);
}
//====================================
// 描画処理
//====================================
void CBoss::Draw(void)
{
#if 0
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
	for (int nCnt = 0; nCnt < NUMMODELS; nCnt++)
	{
		// 全モデル描画
		m_pModel[nCnt]->Draw();
	}
#endif
	// デバッグフォント
	CDebugproc::Print("ボス座標 [ %.2f ,%.2f , %.2f]", m_pos.x,m_pos.y,m_pos.z);
	CDebugproc::Draw(0, 40);

	CDebugproc::Print("ボスモーション数 { %d }", m_type);
	CDebugproc::Draw(0, 180);

	CDebugproc::Print("ボス右手座標 { %.2f,%.2f,%.2f }", GetModelPartType(CModel::PARTTYPE_RIGHT_HAND)->GetMtxWorld()._41, GetModelPartType(CModel::PARTTYPE_RIGHT_HAND)->GetMtxWorld()._42, GetModelPartType(CModel::PARTTYPE_RIGHT_HAND)->GetMtxWorld()._43);
	CDebugproc::Draw(0, 300);

	m_pMotion->Debug();
}
//====================================
// 右手とプレイヤーの当たり判定
//====================================
bool CBoss::CollisionRightHand(D3DXVECTOR3* pPos)
{
	// 一定フレーム内
	if (m_pMotion->CheckFrame(100, 150, PATTERN_HAND))
	{
		// モデルのパーツ取得
		CModel* pRightHand = GetModelPartType(CModel::PARTTYPE_RIGHT_HAND); // 右手

		// nullだったら
		if (!pRightHand) return false;

		// 右手のワールドマトリックスを取得
		D3DXMATRIX mtxWorld = pRightHand->GetMtxWorld();

		// 差分計算
		D3DXVECTOR3 diff = *pPos - D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

		// 計算した差分の長さ取得
		float fDist = D3DXVec3Length(&diff);

		// 距離を返す
		return fDist <= (HITRANGE * HITRANGE);
	}
	else
	{
		// 当たらないとき
		return false;
	}
}
