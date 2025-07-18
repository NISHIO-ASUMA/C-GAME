//============================================
//
// プレイヤー状態管理処理 [ playerstate.cpp ]
// Author: Asuma Nishio
//
//=============================================

//**************************
// インクルードファイル宣言
//**************************
#include "playerstate.h"
#include "state.h"
#include "manager.h"
#include "game.h"
#include "parameter.h"

constexpr int DAMAGECOUNT = 60;

//==================================
// プレイヤー状態コンストラクタ
//==================================
CPlayerStateBase::CPlayerStateBase()
{
	// 値のクリア
	m_pPlayer = nullptr;
	m_ID = ID_NONE;
}
//==================================
// プレイヤー状態デストラクタ
//==================================
CPlayerStateBase::~CPlayerStateBase()
{
	// 無し
}


//==================================
// 待機状態時コンストラクタ
//==================================
CPlayerStateNeutral::CPlayerStateNeutral()
{
	// セット
	SetID(ID_NEUTRAL);
}
//==================================
// 待機状態時デストラクタ
//==================================
CPlayerStateNeutral::~CPlayerStateNeutral()
{
	// 無し
}
//==================================
// 待機状態開始関数
//==================================
void CPlayerStateNeutral::OnStart()
{
	// ニュートラルモーションに設定
	m_pPlayer->GetMotion()->SetMotion(CPlayer::PLAYERMOTION_NEUTRAL);
}
//==================================
// 待機状態更新関数
//==================================
void CPlayerStateNeutral::OnUpdate()
{
	// キー入力情報の取得
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// 移動入力があれば移動状態へ
	if (m_pPlayer->isMoveInputKey(pInput))
	{
		// 状態変更
		m_pPlayer->ChangeState(new CPlayerStateMove,ID_MOVE);

		// ここで処理を返す
		return;
	}

	// 攻撃キー入力時
	if (pInput->GetPress(DIK_RETURN))
	{
		// ステート変更
		m_pPlayer->ChangeState(new CPlayerStateAction,ID_ACTION);

		// ここで処理を返す
		return;
	}

	//// ジャンプキー入力時
	//if (pInput->GetPress(DIK_SPACE))
	//{
	//	// ステート変更
	//	m_pPlayer->ChangeState(new CPlayerStateJump);

	//	// ここで処理を返す
	//	return;
	//}
}
//==================================
// 待機状態時終了関数
//==================================
void CPlayerStateNeutral::OnExit()
{
}


//==================================
// コンストラクタ
//==================================
CPlayerStateAction::CPlayerStateAction()
{
	// 無し
}
//==================================
// デストラクタ
//==================================
CPlayerStateAction::~CPlayerStateAction()
{
	// 無し
}
//==================================
// 攻撃状態開始関数
//==================================
void CPlayerStateAction::OnStart()
{
	// 攻撃モーションに変更
	m_pPlayer->GetMotion()->SetMotion(CPlayer::PLAYERMOTION_ACTION);
}
//==================================
// 攻撃状態更新関数
//==================================
void CPlayerStateAction::OnUpdate()
{
	// 入力情報の取得
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// 武器の位置取得
	CModel* pModelWeapon = m_pPlayer->GetModelPartType(CModel::PARTTYPE_WEAPON);

	// nullだったら
	if (!pModelWeapon) return;

	// 腕のワールドマトリックスを取得
	D3DXMATRIX mtxWorld = pModelWeapon->GetMtxWorld();

	// プレイヤーとボス間でベクトル生成
	D3DXVECTOR3 VecBoss = m_pPlayer->VecToBoss(m_pPlayer->GetPos());

	// 攻撃更新
	m_pPlayer->UpdateAction(pInput, mtxWorld, VecBoss);

}
//==================================
// 攻撃状態終了関数
//==================================
void CPlayerStateAction::OnExit()
{
	// 無し
}


//==================================
// 移動状態コンストラクタ
//==================================
CPlayerStateMove::CPlayerStateMove()
{
	// 無し
}
//==================================
// 移動状態デストラクタ
//==================================
CPlayerStateMove::~CPlayerStateMove()
{
	// 無し
}
//==================================
// 移動状態開始関数
//==================================
void CPlayerStateMove::OnStart()
{
	// 無し
}
//==================================
// 移動状態更新関数
//==================================
void CPlayerStateMove::OnUpdate()
{
	// キー入力を取得	
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// シリンダー座標の取得
	D3DXVECTOR3 MeshPos = CGame::GetCylinder()->GetPos();

	// 移動処理実行
	m_pPlayer->UpdateMove(MeshPos, pInput);

	// キーを離したらニュートラルに戻る
	if (!m_pPlayer->isMoveInputKey(pInput))
	{
		// ニュートラルに遷移
		m_pPlayer->ChangeState(new CPlayerStateNeutral,ID_NEUTRAL);

		// ここで処理を返す
		return;
	}
}
//==================================
// 移動状態終了関数
//==================================
void CPlayerStateMove::OnExit()
{
	// 無し
}


//==================================
// ダメージ状態コンストラクタ
//==================================
CPlayerStateDamage::CPlayerStateDamage(int nDamage)
{
	// 値のクリア
	m_nStateCount = NULL;
	m_nDamage = nDamage;
}
//==================================
// ダメージ状態デストラクタ
//==================================
CPlayerStateDamage::~CPlayerStateDamage()
{
	// 無し
}
//==================================
// ダメージ状態開始関数
//==================================
void CPlayerStateDamage::OnStart()
{
	// 一体目のプレイヤーの時
	if (m_pPlayer->GetPlayerIndex() == 0)
	{
		// パラメーター取得
		CParameter* pParamet = m_pPlayer->GetParameter();

		// 体力を減らす
		pParamet->HitDamage(m_nDamage);
	}

	// 状態変更
	m_nStateCount = DAMAGECOUNT;
}
//==================================
// ダメージ状態更新関数
//==================================
void CPlayerStateDamage::OnUpdate()
{
	// 状態管理カウンターをデクリメント
	m_nStateCount--;

	if (m_nStateCount <= NULL)
	{
		// カウントを変更
		m_nStateCount = DAMAGECOUNT; 

		// 状態変更
		m_pPlayer->ChangeState(new CPlayerStateNeutral(), ID_NEUTRAL);
	}
}
//==================================
// ダメージ状態更新関数
//==================================
void CPlayerStateDamage::OnExit()
{
	// 無し
}
