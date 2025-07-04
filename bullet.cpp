//====================================
//
// 弾の処理 [ bullet.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "bullet.h"
#include "Manager.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"

//***********************************
// マクロ定義
//***********************************
#define BULLET_SIZE (30.0f) // 弾のサイズ
#define BULLET_DAMAGE (1)	// 弾のダメージ

//===============================
// オーバーロードコンストラクタ
//===============================
CBullet::CBullet(int nPriority) : CBillboard(nPriority)
{
	// 値のクリア
	m_nLife = 0;
	m_move = VECTOR3_NULL;
	m_Type = BTYPE_NONE;
	m_nPriority = nPriority;
	m_nIdxTexture = NULL;
	m_pTarget = nullptr;
	m_isHoming = false;
}
//===============================
// デストラクタ
//===============================
CBullet::~CBullet()
{
	// 無し
}
//===============================
// 弾の生成
//===============================
CBullet* CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, BTYPE nType, const float fWidth, const float fHeight,const int nLife)
{
	// 最大数
	int nNum = CObject::GetNumAll();

	// 最大数超えたら
	if (nNum >= MAX_OBJECT -1)
	{
		return nullptr;
	}

	// 弾のインスタンス生成
	CBullet* pBullet;

	// メモリ確保
	pBullet = new CBullet;

	// オブジェクト設定
	pBullet->SetTexture(nType);
	pBullet->SetPos(pos);
	pBullet->SetType(nType);
	pBullet->SetSize(fWidth, fHeight);
	pBullet->m_nLife = nLife;

	// 初期化に失敗したら
	if (FAILED(pBullet->Init(rot)))
	{
		// 破棄
		delete pBullet;

		// NULLを返す
		return nullptr;
	}

	// ポインタを返す
	return pBullet;
}
//===============================
// 弾の種類セット
//===============================
void CBullet::SetType(BTYPE type)
{
	// 種類設定
	m_Type = type;
}
//===============================
// 種類ごとのテクスチャセット
//===============================
void CBullet::SetTexture(BTYPE type)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャ設定
	switch (type)
	{
	case BTYPE_PLAYER:
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\bullet002.png");
		break;

	case BTYPE_ENEMY:
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\gold.jpg");
		break;

	default:
		break;
	}
}
//===============================
// 弾の初期化
//===============================
HRESULT CBullet::Init(D3DXVECTOR3 rot)
{
	// オブジェクト2Dの初期化
	CBillboard::Init();

	// オブジェクトの種類を設定する
	SetObjType(TYPE_BULLET);

	// 移動量
	m_move = D3DXVECTOR3(rot.x * 15.0f, rot.y,rot.z * 15.0f);

	return S_OK;
}
//===============================
// 弾の終了
//===============================
void CBullet::Uninit(void)
{
	// オブジェクト2Dの破棄
	CBillboard::Uninit();
}
//===============================
// 弾の更新
//===============================
void CBullet::Update(void)
{
	// 弾の座標を取得
	D3DXVECTOR3 BulletPos = GetPos();

	// 寿命カウント
	m_nLife--;

	// カラーセット
	SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	// エフェクト生成
	CEffect::Create(BulletPos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), VECTOR3_NULL, m_nLife, 10.0f);

	// 位置を更新
	BulletPos += m_move;

	// 座標を更新
	SetPos(BulletPos);

	// 敵との当たり判定
	bool isHit = Collision(BulletPos);

	// 体力が0以下 かつ 敵に当たっていなかったら
	if (m_nLife <= 0 && !isHit)
	{
		// 未使用状態
		Uninit();
	}
}
//===============================
// 弾の描画
//===============================
void CBullet::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャ読み込み
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// オブジェクトの描画
	CBillboard::Draw();
}
//===============================
// 当たり判定関数
//===============================
bool CBullet::Collision(D3DXVECTOR3 pos)
{
#if 0
	// 最大数回す
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		// オブジェクトを取得する
		CObject* pObj = CObject::GetObject(CObject::PRIORITY::PLAYER,nCnt);

		// 現在の弾の種類をセット
		BTYPE btype= GetType();

		// NULLチェック
		if (pObj != NULL)
		{
			// オブジェクトの種類を取得
			TYPE type = pObj->GetObjType();

			if (type == TYPE_ENEMY && btype == BTYPE_PLAYER)
			{// 種類が敵だったら かつ 弾の種類がプレイヤーの弾なら

				// 敵の座標,角度の取得
				D3DXVECTOR3 enemypos = pObj->GetPos();

				// キャストする
				CEnemy* pEnemy = (CEnemy*)pObj;

				// 敵の位置をと比較し,判定を生成
				if (pos.x <= enemypos.x + 30.0f &&
					pos.y >= enemypos.y - 30.0f &&
					pos.x >= enemypos.x - 30.0f &&
					pos.y <= enemypos.y + 30.0f)
				{					
					// 敵をにダメージ
 					pEnemy->HitEnemy(BULLET_DAMAGE);

					// 弾の終了処理
					CBullet::Uninit();

					// 判定結果を返す
					return true;
				}
			}
			else if (type == TYPE_PLAYER && btype == BTYPE_ENEMY)
			{// 種類がプレイヤーだったら　かつ 弾の種類が敵なら

				// プレイヤーの座標,角度の取得
				D3DXVECTOR3 pPos = pObj->GetPos();

				// プレイヤーにキャストする
				CPlayer* pPlayer = (CPlayer*)pObj;

				// プレイヤーの位置をと比較し,判定を生成
				if (pos.x <= pPos.x + 50.0f &&
					pos.y >= pPos.y - 50.0f &&
					pos.x >= pPos.x - 50.0f &&
					pos.y <= pPos.y + 50.0f &&
					pPlayer->GetState() == CPlayer::PLAYERSTATE_NORMAL)
				{
					// プレイヤーにダメージ
					// pPlayer->HitDamage(BULLET_DAMAGE);

					// 弾の終了処理
					CBullet::Uninit();

					// 判定結果を返す
					return true;
				}
			}
		}
	}
#endif
	// 判定結果が無かったら
 	return false;
}
