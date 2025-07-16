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

constexpr int  keyRepeatCount = 15;		// キーのリピートカウント

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

	// シリンダー座標の取得
	D3DXVECTOR3 MeshPos = CGame::GetCylinder()->GetPos();

	// ボスの座標取得
	D3DXVECTOR3 BossPos = CGame::GetBoss()->GetPos();

	// プレイヤーとボス間でベクトル生成
	D3DXVECTOR3 VecBoss = BossPos - m_pPlayer->GetPos();
	VecBoss.y = 0.0f;

	// できたベクトルを正規化する
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// プレイヤーの移動更新関数
	m_pPlayer->UpdateMove(MeshPos, pInput);

	// 武器のワールドマトリックスを取得する
	CModel* pModelWepon = m_pPlayer->GetModelPartType(CModel::PARTTYPE_WEAPON);
	D3DXMATRIX mtxWorld = {};

	// nullじゃなかったら
	if (pModelWepon != nullptr) mtxWorld = pModelWepon->GetMtxWorld();

	// Enterキーで攻撃ステートへ遷移
	if (pInput->GetPress(DIK_RETURN))
	{
		// ステートをアクション状態に変更する
		m_pPlayer->ChangeState(new CPlayerStateAction());

		// ここで処理を返す
		return;
	}

	// ジャンプ行動
	m_pPlayer->UpdateJumpAction(pInput, mtxWorld, VecBoss);

	// 位置を更新する
	m_pPlayer->AddMove();
}
//==================================
// 待機状態時終了関数
//==================================
void CPlayerStateNeutral::OnExit()
{
	// 無し
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
	m_pPlayer->SetAttack(true);
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

	// ボスの方向を計算
	D3DXVECTOR3 VecBoss = CGame::GetBoss()->GetPos() - m_pPlayer->GetPos();
	VecBoss.y = 0.0f;

	// 正規化
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// 攻撃フラグが立っていなければ初期化
	if (!m_pPlayer->GetAttack())
	{
		// 攻撃を有効状態にする
		m_pPlayer->SetAttack(true);

		// 攻撃モーションに変更
		m_pPlayer->GetMotion()->SetMotion(CPlayer::PLAYERMOTION_ACTION);
	}

	// 攻撃キー入力
	if (pInput->GetRepeat(DIK_RETURN, keyRepeatCount))
	{
		// 15フレームたったら生成
		CBullet::Create(
			D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43),
			VecBoss, 
			CBullet::BTYPE_PLAYER, 
			5.0f, 5.0f, 60);
	}

	// モーション終了でニュートラルに戻す
	if (m_pPlayer->GetMotion()->GetFinishMotion())
	{
		// 攻撃終了
		m_pPlayer->SetAttack(false);

		// ニュートラルステートを生成
		m_pPlayer->ChangeState(new CPlayerStateNeutral());
	}
}
//==================================
// 攻撃状態終了関数
//==================================
void CPlayerStateAction::OnExit()
{
	// 無し
}
