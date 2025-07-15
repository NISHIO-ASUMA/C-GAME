//====================================
//
// テクスチャ処理 [ texture.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "texture.h"
#include "manager.h"

//**********************
// 静的メンバ変数
//**********************
int CTexture::m_nNumAll = NULL;	// 総数管理

//===============================
// コンストラクタ
//===============================
CTexture::CTexture()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_apTexture[nCnt] = nullptr;
	}
}
//===============================
// デストラクタ
//===============================
CTexture::~CTexture()
{
	// 無し
}
//===============================
// テクスチャ読み込み
//===============================
HRESULT CTexture::Load(void)
{
#if 1
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// すべてのテクスチャ分回す
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		// ファイル名が無いならスキップ
		if (TexName[nCnt] == nullptr)
			continue;

		// 列挙型の分読み込む
		if (FAILED(D3DXCreateTextureFromFile(pDevice, TexName[nCnt], &m_apTexture[nCnt])))
		{
			// 警告表示
			MessageBox(NULL, "テクスチャパスが存在しません", "テクスチャ読み込みエラー", MB_OK);

			return E_FAIL;
		}

		// 加算する
		m_nNumAll++;
	}
#endif

	// 結果を返す
	return S_OK;

}
//===============================
// テクスチャ破棄
//===============================
void CTexture::UnLoad(void)
{
	// すべてのテクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}
//===============================
// テクスチャ番号を登録する
//===============================
int CTexture::Register(const char* pFileName)
{
	// インデックス用の変数
	int nIdx = -1;

	// インデックス番号を加算
	nIdx++;

	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		// nullptrチェック
		if (TexName[nCnt] != nullptr)
		{
			// ファイルパス名が一致していたら
			if (strcmp(pFileName, TexName[nCnt]) == 0)
			{
				// カウント番号を代入
				nIdx = nCnt;

				// for文抜ける
				break;
			}
		}
	}

	// インデックス番号を返す
	return nIdx;
}
//===============================
//テクスチャ番号取得
//===============================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	// 例外処理
	if (nIdx < 0 || nIdx >= MAX_TEXTURE) return nullptr;

	// テクスチャ番号を取得
	return m_apTexture[nIdx];
}
