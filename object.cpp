//====================================
//
// オブジェクト処理 [ object.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "object.h"
#include "object2D.h"
#include "Manager.h"
#include "debugproc.h"

//**********************
// 静的メンバ変数宣言
//**********************
CObject* CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT] = {}; // オブジェクトのポインタ
int CObject::m_nNumAll = 0;									 // オブジェクト総数

//===============================
// オーバーロードコンストラクタ
//===============================
CObject::CObject(int nPriority)
{
	// 全オブジェクト分回す
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		// NULLチェック
		if (m_apObject[nPriority][nCntObj] == nullptr)
		{
			m_apObject[nPriority][nCntObj] = this; // 自分自身を代入
			m_nID = nCntObj;			// ID保持
			m_nNumAll++;				// 総数を加算
			m_nPriority = nPriority;	// プライオリティを保持
			break;
		}
	}

	// 値のクリア
	m_Type = TYPE_NONE;
}
//===============================
// デストラクタ
//===============================
CObject::~CObject()
{
	// 無し
}
//===============================
// 全解放処理
//===============================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				// nullptrじゃなかったら
				m_apObject[nCntPri][nCnt]->Uninit();
			}
		}
	}
}
//===============================
// 全更新処理
//===============================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				// nullptrじゃなかったら
				m_apObject[nCntPri][nCnt]->Update();
			}
		}
	}
}
//===============================
// 全描画処理
//===============================
void CObject::DrawAll(void)
{
	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラのセット
	pCamera->SetCamera();

	// 最大分回す
	for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				// nullptrじゃなかったら
				m_apObject[nCntPri][nCnt]->Draw();
			}
		}
	}

	// デバッグフォント
	CDebugproc::Print("現在のオブジェクト数 : %d", m_nNumAll);
	CDebugproc::Draw(0, 120);
}
//===============================
// 種類をセット
//===============================
void CObject::SetObjType(TYPE type)
{
	m_Type = type;
}
//===============================
// 種類を取得
//===============================
CObject::TYPE CObject::GetObjType(void)
{
	return m_Type;
}
//===============================
// オブジェクトを取得
//===============================
CObject* CObject::GetObject(int nPriority,int nIdx)
{
	// nullptrチェック
	if (nIdx < 0 || nIdx >= m_nNumAll)
	{
		// ここで結果を返す
		return nullptr;
	}

	// nullptrじゃなかったら
	return m_apObject[nPriority][nIdx];
}
//===============================
// オブジェクトの破棄
//===============================
void CObject::Release(void)
{
	// 自分自身のIDを代入
	int nId = this->m_nID;

	// 自分自身のプライオリティ番号を代入
	int nPrio = this->m_nPriority;

	// nullptrじゃなかったら
	if (m_apObject[nPrio][nId] != nullptr)
	{
		// 破棄
		delete m_apObject[nPrio][nId];

		// nullptrにする
		m_apObject[nPrio][nId] = nullptr;

		// 総数をデクリメント
		m_nNumAll--;
	}
}
