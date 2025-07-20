//======================================================
//
// 弾のオブジェクトホーミング処理 [ bullethorming.cpp ]
// Author: Asuma Nishio
//
//======================================================

//**************************
// インクルードファイル宣言
//**************************
#include "bullethorming.h"
#include "manager.h"
#include "player.h"
#include "playerstate.h"

//**************************
// 定数宣言
//**************************
constexpr float MIN_RANGE = 40.0f;	// 最少距離
constexpr float MOVESPEED = 5.0f;	// 追従スピード
constexpr float HITRANGE = 70.0f;	// 当たり判定距離

//==================================
// コンストラクタ
//==================================
CBulletHorming::CBulletHorming()
{
	// 値のクリア
	m_fRange = NULL;
}
//==================================
// デストラクタ
//==================================
CBulletHorming::~CBulletHorming()
{
	// 無し
}
//==================================
// 生成処理
//==================================
CBulletHorming* CBulletHorming::Create(const char * pFileName,D3DXVECTOR3 pos)
{
	// インスタンス生成
	CBulletHorming* pBulletHorming = new CBulletHorming;

	// nullチェック
	if (pBulletHorming == nullptr)
		return nullptr;

	// オブジェクト設定
	pBulletHorming->SetFilePass(pFileName);
	pBulletHorming->SetPos(pos);

	// 初期化失敗時
	if (FAILED(pBulletHorming->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pBulletHorming;
}
//==================================
// 初期化処理
//==================================
HRESULT CBulletHorming::Init(void)
{
	// オブジェクト初期化処理
	CObjectX::Init();

	// 初期化結果を返す
	return S_OK;
}
//==================================
// 終了処理
//==================================
void CBulletHorming::Uninit(void)
{
	// オブジェクト終了処理
	CObjectX::Uninit();
}
//==================================
// 更新処理
//==================================
void CBulletHorming::Update(void)
{
	// 現在の座標を取得
	D3DXVECTOR3 NowPos = GetPos();

	// プレイヤー取得
	CPlayer* pPlayer = CPlayer::GetIdxPlayer(0);
	if (pPlayer == nullptr) return;			// nullだったら処理をとおさない

	// 座標を取得
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	// 衝突判定
	if (Collision(PlayerPos))
	{
		// ダメージ変更
		pPlayer->GetMotion()->SetMotion(CPlayer::PLAYERMOTION_DAMAGE);

		// ステート変更
		pPlayer->ChangeState(new CPlayerStateDamage(1), CPlayerStateBase::ID_DAMAGE);

		// ここで抜ける
		return;
	}

	// プレイヤーと弾のベクトルを生成
	D3DXVECTOR3 VecPlayer = PlayerPos - NowPos;

	// ベクトルの長さ取得
	float fLength = D3DXVec3Length(&VecPlayer);

	// 離れすぎていたら追従しないようにする
	m_fRange = MIN_RANGE;

	// 追従距離上限より小さくなったら追従しない
	if (fLength < m_fRange) return;

	// ベクトルを正規化する
	D3DXVec3Normalize(&VecPlayer, &VecPlayer);

	// 弾の移動速度を設定する
	float fMove = MOVESPEED;

	// 移動ベクトルを加算
	NowPos += VecPlayer * fMove;

	// 地面以下にならないようにする
	if (NowPos.y <= PlayerPos.y + 50.0f)
	{
		NowPos.y = PlayerPos.y + 50.0f;
	}

	// 現在の座標にセットする
	SetPos(NowPos);
}
//==================================
// 描画処理
//==================================
void CBulletHorming::Draw(void)
{
	// オブジェクト描画処理
	CObjectX::Draw();
}
//==================================
// 当たり判定処理
//==================================
bool CBulletHorming::Collision(D3DXVECTOR3 DestPos)
{
	// 弾の現在位置を取得
	D3DXVECTOR3 BulletPos = GetPos();

	// ベクトルを計算
	D3DXVECTOR3 vec = DestPos - BulletPos;

	// 距離を求める
	float fDistance = D3DXVec3Length(&vec);

	// ヒット判定半径
	const float fHitRadius = HITRANGE;

	// 距離がヒット半径以内なら当たり
	if (fDistance <= fHitRadius)
	{
		// オブジェクトを消す
		Uninit();

		// 当たり
		return true; 
	}

	return false; // 当たらない
}

