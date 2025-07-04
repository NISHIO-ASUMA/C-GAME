//====================================
//
// 敵の処理 [ enemy.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "enemy.h"
#include "Manager.h"
#include "debugproc.h"
#include "bullet.h"
#include "particle.h"
#include "score.h"

//**********************
// マクロ定義
//**********************
#define ENEMY_SIZE_X (40.0f)			// 敵の横幅
#define ENEMY_SIZE_Y (40.0f)			// 敵の縦幅
#define DAMAGE_COL (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)) // ダメージ色

//***********************************
// 静的メンバ変数宣言
//***********************************
int CEnemy::m_NumEnemy = 0;			// 敵の総数

//===============================
// オーバーロードコンストラクタ
//===============================
CEnemy::CEnemy(int nPriority)
{
	// 値のクリア
	m_move = VECTOR3_NULL;
	m_nInterval = 0;
	m_StateCount = 0;
	m_State = ENEMYSTATE_NONE;
	m_nIdxTexture = NULL;
}
//===============================
// デストラクタ
//===============================
CEnemy::~CEnemy()
{
	// 敵の数を減らす
}
//===============================
// 敵の生成
//===============================
CEnemy* CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,ENEMYTYPE nType)
{
	// 敵のインスタンス生成
	CEnemy* pEnemy = nullptr;

	// メモリ確保
	pEnemy = new CEnemy;

	// テクスチャセット
	pEnemy->SetTexture(nType);

	// 初期化に失敗したら
	if (FAILED(pEnemy->Init()))
	{
		// 破棄
		delete pEnemy;

		// NULLを返す
		return nullptr;
	}

	// 2Dオブジェクト設定
	pEnemy->SetSize(ENEMY_SIZE_X, ENEMY_SIZE_Y);

	pEnemy->SetRot(rot);
	pEnemy->SetPos(pos);

	// 敵の種類セット
	pEnemy->SetType(nType);

	// 敵数カウントをインクリメント
	m_NumEnemy++;

	// ポインタを返す
	return pEnemy;
}
//===============================
// 敵の種類セット
//===============================
void CEnemy::SetType(const ENEMYTYPE type)
{
	// 種類代入
	m_Type = type;
}
//===============================
// 敵の初期化
//===============================
HRESULT CEnemy::Init(void)
{
	// 親クラスの初期化
	CObject2D::Init();

	// 種類をセット
	SetObjType(TYPE_ENEMY);

	// 取得
	D3DXVECTOR3 rot = GetRot();

	// メンバ変数の初期化
	m_move = D3DXVECTOR3(sinf(rot.z + D3DX_PI) * 20.0f, cosf(rot.z + D3DX_PI) * 20.0f, 0.0f);
	m_nInterval = 0;
	m_State = ENEMYSTATE_NORMAL;

	return S_OK;
}
//===============================
// 敵の終了
//===============================
void CEnemy::Uninit(void)
{
	// オブジェクトの破棄
	CObject2D::Uninit();

	// デクリメント
	m_NumEnemy--;
}
//===============================
// 敵の更新
//===============================
void CEnemy::Update(void)
{
	// 2Dオブジェクト更新
	CObject2D::Update();

#if 0
	// 座標の取得
	D3DXVECTOR3 EnemyPos = GetPos();

	// 角度の取得
	D3DXVECTOR3 EnemyRot = GetRot();

	D3DXCOLOR col = GetCol();

	// 座標を更新
	SetRot(EnemyRot);
	SetPos(EnemyPos);

	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 画面外
	if (EnemyPos.x >= SCREEN_WIDTH ||
		EnemyPos.y >= SCREEN_HEIGHT ||
		EnemyPos.y <= 0.0f ||
		EnemyPos.x <= 0.0f)
	{
		// 破棄する
		Uninit();
	}

	// インターバルカウントを加算
	m_nInterval++;

	// カウントが上限を超えたら
	if (m_nInterval > 30)
	{
		// 弾の生成
		CBullet::Create(EnemyPos, D3DXVECTOR3(0.0f,0.0f,D3DX_PI),CBullet::BTYPE_ENEMY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		
		// 初期値に戻す
		m_nInterval = 0;
	}

	// 状態管理
	switch (m_State)
	{
	case ENEMYSTATE_NONE:
		break;

	case ENEMYSTATE_NORMAL:
		break;

	case ENEMYSTATE_DAMAGE:

		// 状態管理カウンターをデクリメント
		m_StateCount--;

		// カウンターが0以下
		if (m_StateCount <= 0)
		{
			// 敵状態変更
			m_State = ENEMYSTATE_NORMAL;

			// 頂点カラーの設定
			SetCol(col);
		}
		break;
	default:
		break;
	}
#endif
}
//===============================
// 敵の描画
//===============================
void CEnemy::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();

	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// オブジェクトの描画
	CObject2D::Draw();
}
//===============================
// 敵のダメージ処理
//===============================
void CEnemy::HitEnemy(int nDamage)
{
#if 0
	// 状態を判別
	if (m_State == ENEMYSTATE_DAMAGE)
	{
		// 処理をここで返す
		return;
	}

	// 座標を取得
	D3DXVECTOR3 pos = GetPos();

	// 体力を減らす
	nLife -= nDamage;

	// 状態処理
	if (nLife <= 0)
	{
		// パーティクル生成
		switch (m_Type)
		{
		case CEnemy::ENEMYTYPE_FACE:
			// パーティクル生成
			CParticle::Create(pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 15, 3.0f);
			// スコアを加算
			CScore::AddScore(10000);
			break;

		case CEnemy::ENEMYTYPE_GOLD:
			// パーティクル生成
			CParticle::Create(pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 15, 3.0f);
			// スコアを加算
			CScore::AddScore(10000);
			break;

		default:
			break;
		}

		// 破棄する
		Uninit();
	}
	else
	{
		m_State = ENEMYSTATE_DAMAGE;	//ダメージ状態
		m_StateCount = 5;				//ダメージ状態の時間

		// 頂点カラーの設定(ダメージ色)
		SetCol(DAMAGE_COL);
	}

	// 一番最後に現在の体力をセットする
	SetLife(nLife);
#endif
}
//===============================
// テクスチャ設定
//===============================
void CEnemy::SetTexture(ENEMYTYPE Type)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	switch (Type)
	{
	case ENEMYTYPE_FACE:
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\face000.png");
		break;

	case ENEMYTYPE_GOLD:
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\gold.jpg");
		break;

	default:
		break;
	}
}
