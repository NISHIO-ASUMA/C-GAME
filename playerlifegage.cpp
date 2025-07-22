//=========================================
//
// プレイヤー体力処理 [ playerlifegage.h ]
// Author: Asuma Nishio
//
//=========================================

//**********************
// インクルードファイル
//**********************
#include "playerlifegage.h"
#include "player.h"
#include "parameter.h"
#include "manager.h"
#include "texture.h"

//**********************
// 定数宣言
//**********************
constexpr float GAGE_WIDTH = 40.0f; // 基準値に掛ける倍率
constexpr float GAGE_HEIGHT = 30.0f; // ゲージの高さの固定値

//========================
// コンストラクタ
//========================
CPlayerLifeGage::CPlayerLifeGage(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_pPlayer = nullptr;
	m_nLifeLength = NULL;
	m_gage = GAGE_FRAME;
}
//========================
// デストラクタ
//========================
CPlayerLifeGage::~CPlayerLifeGage()
{
	// 無し
}
//========================
// 初期化処理
//========================
HRESULT CPlayerLifeGage::Init(void)
{
	// 親クラスの初期化処理
	CObject2D::Init();

	// nullだったら
	if (m_pPlayer == nullptr)
	{
		// プレイヤー取得
		m_pPlayer = CPlayer::GetIdxPlayer(1);
	}

	// 取得できたら
	if (m_pPlayer != nullptr)
	{
		// パラメーター取得
		CParameter* pParam = m_pPlayer->GetParameter();

		// 現在の体力を取得する
		m_nLifeLength = pParam->GetHp();
	}

	// 初期化結果を返す
	return S_OK;
}
//========================
// 終了処理
//========================
void CPlayerLifeGage::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//========================
// 更新処理
//========================
void CPlayerLifeGage::Update(void)
{
	// nullじゃない かつ 種類がバーの時
	if (m_pPlayer != nullptr && m_gage == GAGE_BAR)
	{
		// パラメーター取得
		CParameter* pParam = m_pPlayer->GetParameter();

		// 現在の体力を取得する
		m_nLifeLength = pParam->GetHp();

		// サイズセット
		SetSize(m_nLifeLength * 25.0f, GAGE_HEIGHT);
	}

	// 親クラスの更新処理
	CObject2D::Update();
}
//========================
// 描画処理
//========================
void CPlayerLifeGage::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャ読み込み
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// 親クラスの描画
	CObject2D::Draw();
}
//========================
// 生成処理
//========================
CPlayerLifeGage* CPlayerLifeGage::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int gagetype)
{
	// インスタンス生成
	CPlayerLifeGage* pLifeGage = new CPlayerLifeGage;

	// nullだったら
	if (pLifeGage == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pLifeGage->Init()))
	{
		// nullを返す
		return nullptr;
	}

	// オブジェクト設定
	pLifeGage->SetPos(pos);
	pLifeGage->SetSize(fWidth, fHeight);
	pLifeGage->SetGage(gagetype);
	pLifeGage->SetTexture(gagetype);
	pLifeGage->SetAnchor(CObject2D::ANCHORTYPE_LEFTSIDE);

	// 生成されたポインタを返す
	return pLifeGage;
}
//========================
// テクスチャ割り当て処理
//========================
void CPlayerLifeGage::SetTexture(int Type)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	// 種類に応じて割り当て
	switch (Type)
	{
	case GAGE_FRAME:	// フレーム
		// 割り当て
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\lifeframe.png");
		break;

	case GAGE_BAR:	// 本体ゲージ
		// 割り当て
		m_nIdxTexture = pTexture->Register("data\\TEXTURE\\lifegage.png");
		break;

	default:
		m_nIdxTexture = -1;
		break;
	}
}
