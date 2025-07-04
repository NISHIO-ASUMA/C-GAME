//============================================
//
// 状態管理処理 [ state.cpp ]
// Author: Asuma Nishio
//
//=============================================

//***********************
// インクルードファイル
//***********************
#include "state.h"

//**********************
// マクロ定義
//**********************
#define MAX_STATECOUNT (40) // 最大継続時間

//==================================
// コンストラクタ
//==================================
CState::CState()
{
	// 値のクリア
	m_state = STATE_NONE;
	m_StateCount = NULL;
}
//==================================
// デストラクタ
//==================================
CState::~CState()
{
	// 無し
}
//==================================
// 初期化処理
//==================================
HRESULT CState::Init(void)
{
	// 状態を初期化する
	m_state = STATE_NORMAL;

	// 状態管理カウンターの値を設定
	m_StateCount = MAX_STATECOUNT;

	// 初期化結果を返す
	return S_OK;
}
//==================================
// 終了処理
//==================================
void CState::Uninit(void)
{
	// 無し
}
//==================================
// 更新処理
//==================================
void CState::Update(void)
{
	// 状態に応じて変更
	switch (m_state)
	{
	case STATE_NONE:	// 何もない状態
		break;

	case STATE_NORMAL:	// 通常状態
		break;

	case STATE_DAMAGE:	// ダメージ状態

		// 状態管理カウンターをデクリメント
		m_StateCount--;

		// 状態を変更する
		if (m_StateCount <= 0)
		{
			m_state = STATE_INVINCIBLE;
			m_StateCount = MAX_STATECOUNT;
		}

		break;

	case STATE_INVINCIBLE:	// 無敵状態

		// 状態管理カウンターをデクリメント
		m_StateCount--;

		if (m_StateCount <= 0)
		{
			// 通常状態に変更
			m_state = STATE_NORMAL;
		}

		break;

	default:
		break;
	}
}
//==================================
// 生成処理
//==================================
CState* CState::Create(void)
{
	// インスタンス生成
	CState* pState = new CState;

	// 初期化失敗時
	if (FAILED(pState->Init()))
	{
		// ポインタの破棄
		delete pState;

		// nullptr代入
		pState = nullptr;

		// nullポインタを返す
		return nullptr;
	}

	// 生成されたポインタを返す
	return pState;
}