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

//==================================
// プレイヤー状態コンストラクタ
//==================================
CPlayerStateBase::CPlayerStateBase()
{
	// 値のクリア
	m_pPlayer = nullptr;
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
	// 無し
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
		m_pPlayer->ChangeState(new CPlayerStateMove);

		// ここで処理を返す
		return;
	}

	// 攻撃キー入力時
	if (pInput->GetPress(DIK_RETURN))
	{
		// ステート変更
		m_pPlayer->ChangeState(new CPlayerStateAction);

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
		m_pPlayer->ChangeState(new CPlayerStateNeutral);

		// ここで処理を返す
		return;
	}

	//// ジャンプキー入力時
	//if (pInput->GetTrigger(DIK_SPACE))
	//{
	//	// ステート変更
	//	m_pPlayer->ChangeState(new CPlayerStateJump);

	//	// ここで処理を返す
	//	return;
	//}
}
//==================================
// 移動状態終了関数
//==================================
void CPlayerStateMove::OnExit()
{

}


//==================================
// ジャンプ状態コンストラクタ
//==================================
CPlayerStateJump::CPlayerStateJump()
{
	// 無し
}
//==================================
// ジャンプ状態デストラクタ
//==================================
CPlayerStateJump::~CPlayerStateJump()
{
	// 無し
}
//==================================
// ジャンプ状態開始関数
//==================================
void CPlayerStateJump::OnStart()
{
	// フラグセット
	m_pPlayer->SetJump(false);
	m_pPlayer->StartJump();
}
//==================================
// ジャンプ状態更新関数
//==================================
void CPlayerStateJump::OnUpdate()
{
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// 武器のワールドマトリックスとボス方向取得
	CModel* pModelWeapon = m_pPlayer->GetModelPartType(CModel::PARTTYPE_WEAPON);
	if (!pModelWeapon) return;

	D3DXMATRIX mtxWorld = pModelWeapon->GetMtxWorld();
	D3DXVECTOR3 vecToBoss = m_pPlayer->VecToBoss(m_pPlayer->GetPos());

	// ジャンプ
	if (m_pPlayer->IsJumping())
	{
		m_pPlayer->UpdateJumpAction(pInput, mtxWorld, vecToBoss);
	}

	// 着地時 かつ モーション終了していたら
	if (m_pPlayer->GetMotion()->GetMotionType() == CPlayer::PLAYERMOTION_LANDING && m_pPlayer->GetMotion()->GetFinishMotion())
	{
		// ニュートラルに変更
		m_pPlayer->ChangeState(new CPlayerStateNeutral());

		// ここで処理を返す
		return;
	}
}
//==================================
// ジャンプ状態終了関数
//==================================
void CPlayerStateJump::OnExit()
{
	// 無し
}
