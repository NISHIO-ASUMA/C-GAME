//==============================================
//
// リザルトシーン管理処理 [ resultmanager.cpp ]
// Author: Asuma Nishio
// 
//==============================================

//**************************
// インクルードファイル宣言
//**************************
#include "resultmanager.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "title.h"

//=================================
// コンストラクタ
//=================================
CResultManager::CResultManager()
{
	// 値のクリア
}
//=================================
// デストラクタ
//=================================
CResultManager::~CResultManager()
{
	// 無し
}
//=================================
// 初期化処理
//=================================
HRESULT CResultManager::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetSound();

	// nullだったら
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->PlaySound(CSound::SOUND_LABEL_RESULTBGM);

	// 初期化結果を返す
	return S_OK;
}
//=================================
// 終了処理
//=================================
void CResultManager::Uninit(void)
{
	// nullチェック

}
//=================================
// 更新処理
//=================================
void CResultManager::Update(void)
{
	// 入力デバイス
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// 決定キーが押された
	if (pInput->GetTrigger(DIK_RETURN))
	{
		// ポインタ取得
		CFade* pFade = CManager::GetFade();

		// nullじゃないとき
		if (pFade != nullptr)
		{
			// 旋回off
			pCamera->SetIsRotation(false);
			pCamera->SetFinishRotation(false);

			// シーン遷移
			pFade->SetFade(new CTitle(true));
		}
	}
}
