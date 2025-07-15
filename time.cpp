//====================================
//
// タイム処理 [ time.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "time.h"
#include "manager.h"

//===============================
// 生成処理
//===============================
CTime* CTime::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// タイムクラスのインスタンス生成
	CTime* pTime = new CTime;

	// セット
	pTime->m_fHeight = fHeight;
	pTime->m_fWidth = fWidth;
	pTime->m_pos = pos;

	// 初期化失敗時
	if (FAILED(pTime->Init()))
	{
		// 破棄
		delete pTime;

		// nullptr代入
		pTime = nullptr;
	}

	// 結果を返す
	return pTime;
}
//===============================
// テクスチャセット
//===============================
void CTime::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\score001.png");
}
//===============================
// 初期化処理
//===============================
HRESULT CTime::Init(void)
{
	// メンバ変数の初期化
	m_nAllTime = 100;
	m_nCurrentTime = 0;

	// 横幅計算
	float fTexPos = m_fWidth / NUM_TIME;

	// 桁数分回す
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		// インスタンス生成
		m_pNumber[nCnt] = new CNumber;

		// ナンバー変数の初期化
		m_pNumber[nCnt]->SetSize(fTexPos, m_fHeight);
		m_pNumber[nCnt]->SetPos(m_pos);
		m_pNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x - (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);

	}

	// テクスチャセット
	SetTexture();

	// 結果を返す
	return S_OK;
}
//===============================
// 終了処理
//===============================
void CTime::Uninit(void)
{
	// 使った分破棄
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		// nullptrチェック
		if (m_pNumber[nCnt] != nullptr)
		{
			// 終了
			m_pNumber[nCnt]->Uninit();

			// 破棄
			delete m_pNumber[nCnt];

			// ポインタ初期化
			m_pNumber[nCnt] = nullptr;
		}
	}

	// オブジェクトの破棄
	CObject::Release();
}
//===============================
// 更新処理
//===============================
void CTime::Update(void)
{
	// カウントを加算
	m_nCurrentTime++;

	// 一秒経過後
	if (m_nCurrentTime >= 60)
	{
		// 総時間を減らす
		m_nAllTime--;

		if (m_nAllTime <= 0) m_nAllTime = 0;

		// カウンターを初期化する
		m_nCurrentTime = 0;

	}

	// 最大時間を格納
	int time = m_nAllTime;

	// 一の位から百の位まで順に分解
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		// 桁数計算
		int digit = time % 10;
		time /= 10;

		// ナンバー更新と桁設定
		if (m_pNumber[nCnt] != nullptr)
		{
			m_pNumber[nCnt]->Update();
			m_pNumber[nCnt]->SetDidgit(digit);
		}
	}
}
//===============================
// 描画処理
//===============================
void CTime::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();

	// 桁数分描画
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

		m_pNumber[nCnt]->Draw();
	}
}
//===============================
// オーバーロードコンストラクタ
//===============================
CTime::CTime(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_pos = VECTOR3_NULL;
	m_fHeight = 0;
	m_fWidth = 0;
	m_nAllTime = 0;
	m_nCurrentTime = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = VECTOR3_NULL;
	m_nIdxTexture = NULL;

	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		m_pNumber[nCnt] = nullptr;
	}
}
//===============================
// デストラクタ
//===============================
CTime::~CTime()
{
	// 無し
}