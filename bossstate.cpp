//====================================
//
// ボス状態管理処理 [ bossstate.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "bossstate.h"

//===========================
// コンストラクタ
//===========================
CBossStateBace::CBossStateBace()
{
	// 値のクリア
	m_ID = ID_NEUTRAL;
	m_pBoss = nullptr;
}
//===========================
// デストラクタ
//===========================
CBossStateBace::~CBossStateBace()
{
	// 無し
}


//===========================
// 待機状態コンストラクタ
//===========================
CBossStateNeutral::CBossStateNeutral()
{
	// IDセット
	SetID(ID_NEUTRAL);
}
//===========================
// 待機状態デストラクタ
//===========================
CBossStateNeutral::~CBossStateNeutral()
{
	// 無い
}
//===========================
// 待機状態開始処理
//===========================
void CBossStateNeutral::OnStart(void)
{
	// モーションセット
	m_pBoss->GetMotion()->SetMotion(CBoss::TYPE_NEUTRAL);
}
//===========================
// 待機状態更新処理
//===========================
void CBossStateNeutral::OnUpdate(void)
{
	// ここでクールタイムが0以下ならステート移行の処理

}
//===========================
// 待機状態終了処理
//===========================
void CBossStateNeutral::OnExit(void)
{
	// 無し
}


//===========================
// 攻撃状態コンストラクタ
//===========================
CBossStateAttack::CBossStateAttack()
{
	// IDセット
	SetID(ID_ACTION);
}
//===========================
// 攻撃状態デストラクタ
//===========================
CBossStateAttack::~CBossStateAttack()
{
	// 無し
}
//===========================
// 攻撃状態開始処理
//===========================
void CBossStateAttack::OnStart(void)
{
	// ここでランダムの値を使って攻撃番号モーションに設定

}
//===========================
// 攻撃状態更新処理
//===========================
void CBossStateAttack::OnUpdate(void)
{
	// モーションの終了ができたら

	// ニュートラルにする
	m_pBoss->ChangeState(new CBossStateNeutral, ID_NEUTRAL);
}
//===========================
// 攻撃状態終了処理
//===========================
void CBossStateAttack::OnExit(void)
{
	// 無し
}
