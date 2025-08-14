//=====================================
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
CBossStateNeutral::CBossStateNeutral(int nTime) : m_CoolTime(nTime), m_nCurrentState(0)
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

	// クールタイムセット
	m_pBoss->SetCoolTime(m_CoolTime);
}
//===========================
// 待機状態更新処理
//===========================
void CBossStateNeutral::OnUpdate(void)
{
	// フラグメント
	static bool isCreating = false;

	// falseなら
	if (!isCreating)
	{
		// 乱数の種を一度だけ設定する
		srand((int)time(NULL));

		// 2回目に入らないようにフラグを有効化
		isCreating = true;
	}

	if (m_CoolTime <= 0)
	{
		// クールタイムを初期化する
		m_CoolTime = 0;

		// ランダムでパターンを設定
		if (m_nCurrentState == 0)
		{
			// 行動パターン番号を計算
			m_nCurrentState = rand() % CBoss::PATTERN_MAX - 1;

			switch (m_nCurrentState)
			{
			case CBoss::PATTERN_HAND:

				// ステート変更
				m_pBoss->GetMotion()->SetResetFrame(0);

				// モーション変更
				m_pBoss->GetMotion()->SetMotion(CBoss::PATTERN_HAND);// 殴り攻撃

				return;

			case CBoss::PATTERN_IMPACT:

				// ステート変更
				m_pBoss->GetMotion()->SetResetFrame(0);

				// モーション変更
				m_pBoss->GetMotion()->SetMotion(CBoss::PATTERN_IMPACT); // 叩きつけ攻撃

				return;
			}
		}
	}
	else
	{
		// ニュートラルモーションを更新
		m_pBoss->GetMotion()->SetMotion(CBoss::TYPE_NEUTRAL, false, 0);
	}
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
	m_pBoss->ChangeState(new CBossStateNeutral(60), ID_NEUTRAL);
}
//===========================
// 攻撃状態終了処理
//===========================
void CBossStateAttack::OnExit(void)
{
	// 無し
}
