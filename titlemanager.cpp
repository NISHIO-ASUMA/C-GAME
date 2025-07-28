//=========================================
//
// タイトル管理処理 [ titlemanager.h ]
// Author: Asuma Nishio
// 
//==========================================

//***************************
// インクルードファイル宣言
//***************************
#include "titlemanager.h"
#include "titleui.h"
#include "manager.h"
#include "input.h"
#include "block.h"
#include "meshfield.h"
#include "game.h"
#include "tutorial.h"

//============================
// コンストラクタ
//============================
CTitleManager::CTitleManager()
{
	// 値のクリア
	m_nIdx = NULL;

	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		m_pTitleui[nCnt] = nullptr;
	}
}
//============================
// デストラクタ
//============================
CTitleManager::~CTitleManager()
{
	// 無し
}
//============================
// 初期化処理
//============================
HRESULT CTitleManager::Init(void)
{
	// 基準座標を設定
	D3DXVECTOR3 CenterPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	// タイトルのuiを生成
	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		// 高さの間隔空ける
		CenterPos.y += nCnt * 180.0f;

		// uiを生成 ( 選択メニュー分 )
		m_pTitleui[nCnt] = CTitleUi::Create(CenterPos, COLOR_WHITE, 300.0f, 60.0f, nCnt);
	}

	// ブロック生成
	CBlock::Create("data\\MODEL\\STAGEOBJ\\block000.x",VECTOR3_NULL,VECTOR3_NULL,NULL);

	// 地面生成
	CMeshField::Create(VECTOR3_NULL, 1000.0f);

	// 初期化結果を返す
	return S_OK;
}
//============================
// 終了処理
//============================
void CTitleManager::Uninit(void)
{
	// 無し
}
//============================
// 更新処理
//============================
void CTitleManager::Update(void)
{
	// 入力デバイス取得
	CInputKeyboard* pKey = CManager::GetInputKeyboard();

	// 選択インデックス範囲
	const int SELECT_BEGIN = NULL;
	const int SELECT_END = TITLE_MENU - 1;

	// 上キー入力
	if (pKey->GetTrigger(DIK_UP))
	{
		// インデックス番号を減算
		m_nIdx--;

		// 最小値以下なら最小値に設定
		if (m_nIdx < SELECT_BEGIN)
			m_nIdx = SELECT_END;
	}

	// 下キー入力
	if (pKey->GetTrigger(DIK_DOWN))
	{
		// インデックス番号を加算
		m_nIdx++;

		// 最大値以上なら最大値に設定
		if (m_nIdx > SELECT_END)
			m_nIdx = SELECT_BEGIN;
	}

	// フェード取得
	CFade* pFade = CManager::GetFade();

	// nullだったら
	if (pFade == nullptr) return;

	// 選択されているメニューのポリゴンカラーを変更
	for (int nCnt = 0; nCnt < TITLE_MENU; nCnt++)
	{
		// nullじゃなかったら
		if (m_pTitleui[nCnt] != nullptr)
		{
			// カラー変更
			if (nCnt == m_nIdx)
			{
				// 点滅
				m_pTitleui[nCnt]->SetFlash(20, 40);
			}
			else
			{
				// カラーセット
				m_pTitleui[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	// 決定処理
	if (pKey->GetTrigger(DIK_RETURN))
	{
		switch (m_nIdx)
		{
		case CTitleUi::MENU_GAME:		// ゲームモード
			if (pFade != nullptr) pFade->SetFade(new CGame());	// ゲームシーンに遷移
			break;

		case CTitleUi::MENU_TUTORIAL:	// チュートリアルモード
			if (pFade != nullptr) pFade->SetFade(new CTutorial());	// チュートリアルシーンに遷移
			break;

		default:
			break;
		}
	}
}
