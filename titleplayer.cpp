#include "titleplayer.h"

//============================
// コンストラクタ
//============================
CTitlePlayer::CTitlePlayer(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_move = VECTOR3_NULL;
	m_rotDest = VECTOR3_NULL;
	m_nIdxPlayer = NULL;

	// モデルのポインタのクリア
	for (int nCnt = 0; nCnt < TITLE_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}

	// クラスポインタ
	m_pMotion = nullptr;
	m_pFilename = {};
}
//============================
// デストラクタ
//============================
CTitlePlayer::~CTitlePlayer()
{
	// 無い
}
//============================
// 生成処理
//============================
CTitlePlayer* CTitlePlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, const int nIdxParson, const char* pFilename)
{
	// インスタンス生成
	CTitlePlayer* pTitlePlayer = new CTitlePlayer;

	// nullptrだったら
	if (pTitlePlayer == nullptr) return nullptr;

	// オブジェクト設定
	pTitlePlayer->m_pos = pos;
	pTitlePlayer->m_rot = rot;
	pTitlePlayer->m_nIdxPlayer = nIdxParson;
	pTitlePlayer->m_pFilename = pFilename;

	// 初期化失敗時
	if (FAILED(pTitlePlayer->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pTitlePlayer;
}
//============================
// 初期化処理
//============================
HRESULT CTitlePlayer::Init(void)
{
	// オブジェクトの種類をセット
	SetObjType(TYPE_PLAYER);

	// モーションの読み込み
	m_pMotion = CMotion::Load(m_pFilename, TITLE_MODEL, m_apModel, TITLEMOTION_MAX);

	// モーション数を設定
	m_pMotion->SetMotionNum(TITLEMOTION_MAX);

	// プレイヤー識別モデルフラグを設定
	for (int nCnt = 0; nCnt < TITLE_MODEL; nCnt++)
	{
		// nullptrじゃなかったら
		if (m_apModel[nCnt] != nullptr)
		{
			// フラグを設定する
			m_apModel[nCnt]->SetIsPlayer(true);
		}
	}

	// 初期化結果を返す
	return S_OK;
}
//============================
// 終了処理
//============================
void CTitlePlayer::Uninit(void)
{
	// 自身の破棄
	CObject::Release();
}
//============================
// 更新処理
//============================
void CTitlePlayer::Update(void)
{

}
//============================
// 描画処理
//============================
void CTitlePlayer::Draw(void)
{

}
