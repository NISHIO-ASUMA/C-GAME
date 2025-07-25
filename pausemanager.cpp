//====================================
//
// ポーズ管理処理 [ pausemanager.cpp ]
// Author: Asuma Nishio
// 
// TODO : こっちでポーズの選択時の処理とかをする
//
//=====================================

//**************************
// インクルードファイル宣言
//**************************
#include "pausemanager.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "game.h"

//***************************
// 静的メンバ変数宣言
//***************************
bool CPauseManager::m_isPause = false; // ポーズフラグ

//==================================
// コンストラクタ
//==================================
CPauseManager::CPauseManager()
{
	// 値のクリア
	m_nSelectIdx = NULL;

	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		m_pPause[nCnt] = nullptr;
	}
}
//==================================
// デストラクタ
//==================================
CPauseManager::~CPauseManager()
{
	// 無し
}
//==================================
// 初期化処理
//==================================
HRESULT CPauseManager::Init(void)
{
	// セレクト番号設定
	m_nSelectIdx = CPause::MENU_RETRY;

	// 基準座標を設定
	D3DXVECTOR3 Bacepos = D3DXVECTOR3(200.0f, 80.0f, 0.0f);
	
	// 空ける間隔設定
	float fSpace = 200.0f;

	// ポーズ生成
	for (int nPause = 0; nPause < SELECT_MAX; nPause++)
	{
		// 座標セット
		D3DXVECTOR3 pos = Bacepos;

		// 高さを空ける
		pos.y += nPause * fSpace;

		// ポーズを生成 
		if (nPause == 0)
		{
			// 背景生成
			m_pPause[nPause] = CPause::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,0.0f), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nPause);
		}
		else
		{
			// 選択用ポリゴンの生成
			m_pPause[nPause] = CPause::Create(pos, 180.0f, 40.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nPause);
		}
	}

	// 初期化結果を返す
	return S_OK;
}
//==================================
// 終了処理
//==================================
void CPauseManager::Uninit(void)
{

}
//==================================
// 更新処理
//==================================
void CPauseManager::Update(void)
{
	// ポーズ中じゃなかったら
	if (m_isPause == false)
	{
		// ここで処理を返す
		return;
	}

	// 入力デバイス取得
	CInputKeyboard* pKey = CManager::GetInputKeyboard();

	// 選択インデックス範囲
	const int SELECT_BEGIN = 1;
	const int SELECT_END = SELECT_MAX - 1;

	// 上キー入力
	if (pKey->GetTrigger(DIK_UP))
	{
		// インデックス番号を減算
		m_nSelectIdx--;

		// 最小値以下なら最小値に設定
		if (m_nSelectIdx < SELECT_BEGIN)
			m_nSelectIdx = SELECT_END;
	}

	// 下キー入力
	if (pKey->GetTrigger(DIK_DOWN))
	{
		// インデックス番号を加算
		m_nSelectIdx++;

		// 最大値以上なら最大値に設定
		if (m_nSelectIdx > SELECT_END)
			m_nSelectIdx = SELECT_BEGIN;
	}

	// フェード取得
	CFade* pFade = CManager::GetFade();

	// nullだったら
	if (pFade == nullptr) return;

	// 決定処理
	if (pKey->GetTrigger(DIK_RETURN))
	{
		switch (m_nSelectIdx)
		{
		case CPause::MENU_BACK:
			break;

		case CPause::MENU_RETRY:	// リトライ時
			if (pFade != nullptr) pFade->SetFade(new CGame());	// ゲームシーンに遷移
			SetEnablePause(false);		// ポーズ終了
			break;

		case CPause::MENU_CONTINUE:	// コンテニュー時
			SetEnablePause(false);		// ポーズ終了
			break;

		case CPause::MENU_QUIT:		// 退出時
			if (pFade != nullptr) pFade->SetFade(new CTitle());	// タイトルシーンに遷移
			SetEnablePause(false);		// ポーズ終了
			break;
		}
	}
}
//===========================
// ポーズかどうか
//===========================
void CPauseManager::SetEnablePause(void)
{
	//  Pキーが押された
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P))
	{
		// フラグ変更
		m_isPause = m_isPause ? false : true;
	}
}
