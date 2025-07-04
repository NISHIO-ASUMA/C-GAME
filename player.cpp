//====================================
//
// プレイヤー処理 [ player.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "player.h"
#include "Manager.h"
#include "texture.h"
#include "boss.h"
#include "particle.h"
#include "shadow.h"
#include "meshimpact.h"
#include "input.h"
#include "camera.h"

//**********************
// マクロ定義
//**********************
#define PLAYER_MOVE (0.0095f)	 // プレイヤー移動量
#define PLAYER_JUMPVALUE (17.0f) // ジャンプ量
#define NUMBER_MAIN (0)			 // メイン操作プレイヤー
#define NUMBER_SUB (1)			 // 分身操作プレイヤー

//**********************
// 静的メンバ変数宣言
//**********************
int CPlayer::m_nIdxPlayer = NULL; // プレイヤー識別番号

//===============================
// オーバーロードコンストラクタ
//===============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_move = VECTOR3_NULL;
	m_State = PLAYERSTATE_NONE;
	m_StateCount = NULL;
	m_nIdxTexture = NULL;
	m_rotDest = VECTOR3_NULL;
	m_nNumAll = NULL;
	m_type = NULL;
	m_posOld = VECTOR3_NULL;
	m_size = NULL;

	m_pFilename = {};

	// モデルのポインタのクリア
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}

	m_pMotion = nullptr;
	m_pShadow = nullptr;
	m_pState = nullptr;

	m_isLanding = false;
	m_isJump = false;
	m_isAttack = false;
	m_isMoving = false;
}
//===============================
// デストラクタ
//===============================
CPlayer::~CPlayer()
{
	// 無し
}
//===============================
// プレイヤー生成処理
//===============================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nLife,const int nIdxParson, const char* pFilename)
{
	// プレイヤーのインスタンス生成
	CPlayer* pPlayer = new CPlayer;

	// オブジェクト設定
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
	pPlayer->m_nIdxPlayer = nIdxParson;
	pPlayer->m_pFilename = pFilename;

	// プレイヤー初期化処理
	if (FAILED(pPlayer->Init()))
	{
		// 破棄
		delete pPlayer;

		// nullptr代入
		pPlayer = nullptr;

		// nullptrを返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pPlayer;
}
//===============================
// プレイヤー初期化処理
//===============================
HRESULT CPlayer::Init(void)
{
	// オブジェクトの種類をセット
	SetObjType(TYPE_PLAYER);

	// モデル総数を代入
	m_nNumAll = MAX_MODEL;

	// タイプ代入
	m_type = CPlayer::TYPE_MAX;

	// フラグを設定
	m_isJump = false;
	m_isLanding = false;
	m_isMoving = false;

	// モーションの読み込み
	m_pMotion = CMotion::Load(m_pFilename, MAX_MODEL, m_apModel);

	// モーション数を設定
	m_pMotion->SetMotionNum(CMotion::TYPE_MAX);

	// 状態管理を生成
	m_pState = CState::Create();

	// 影の生成
	m_pShadow = m_pShadow->Create(m_pos, m_rot);

	return S_OK;
}
//===============================
// プレイヤー終了処理
//===============================
void CPlayer::Uninit(void)
{
	// モデル数分の破棄
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		// nullptrチェック
		if (m_apModel[nCnt] != nullptr)
		{
			// 終了処理
			m_apModel[nCnt]->Uninit();

			// ポインタの破棄
			delete m_apModel[nCnt];

			// nullptrにする
			m_apModel[nCnt] = nullptr;
		}
	}

	// モーションポインタの破棄
	if (m_pMotion != nullptr)
	{
		// ポインタの破棄
		delete m_pMotion;

		// nullptrにする
		m_pMotion = nullptr;
	}

	// 状態管理ポインタの破棄
	if (m_pState != nullptr)
	{
		// ポインタの破棄
		delete m_pState;

		// nullptrにする
		m_pState = nullptr;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//============================================================
// プレイヤー更新処理  ( 角度をプレイヤーの移動量として渡す )
//============================================================
void CPlayer::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// メッシュの座標の取得
	D3DXVECTOR3 MeshCylinderPos = CManager::GetCylinder()->GetPos();

	// ボスの座標の取得
	D3DXVECTOR3 BossPos = CManager::GetBoss()->GetPos();

	// メッシュの半径の取得
	float fRadius = CManager::GetCylinder()->GetRadius();

	// 現在のモーションタイプを取得
	int Type = m_pMotion->GetMotionType();

	// 角度計算用
	static float fAngle = NULL;

	// 攻撃中じゃなかったら
	if (!m_isAttack)
	{
		// Aキー
		if (pInput->GetPress(DIK_A))
		{
			// 角度更新
			fAngle += PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // 左

			// 移動モーションに設定
			m_pMotion->SetMotion(m_pMotion->TYPE_MOVE);

		}
		// Dキー	
		else if (pInput->GetPress(DIK_D))
		{
			// 角度更新
			fAngle -= PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle + D3DX_PI * 0.5f;// 右

			// 移動モーション
			m_pMotion->SetMotion(m_pMotion->TYPE_MOVE);
		}
		else
		{
			// 移動だったら
			if (Type == m_pMotion->TYPE_MOVE)
			{
				// ニュートラルモーションに変更
				m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);
			}
		}
	}

	//=========================
	// 角度を正規化する
	//=========================
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	//=====================================================
	// メッシュの座標から見てプレイヤーの位置を求める
	//=====================================================
	m_pos.x = MeshCylinderPos.x - (sinf(fAngle)) * fRadius;		// X座標
	m_pos.z = MeshCylinderPos.z - (cosf(fAngle)) * fRadius;		// Z座標

	// 角度を計算
	m_rot.y += (m_rotDest.y - m_rot.y) - D3DX_PI;

	//=========================
	// 攻撃処理
	//=========================
	// キー入力フラグ
	bool isKeyPress = false;

	// ボスの方向へのベクトルを取得
	D3DXVECTOR3 VecBoss = BossPos - m_pos;

	// Yは水平方向で合わせる（弾が真っすぐ飛ぶように）
	VecBoss.y = 0.0f;

	// ベクトルを正規化
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// 弾の移動成分
	D3DXVECTOR3 BulletMove = VecBoss;

	// プレイヤーの腕のワールドマトリックスを取得
	D3DXMATRIX mtxWorld = m_apModel[18]->GetMtxWorld();

	// 攻撃キー かつ 攻撃フラグがfalseなら
	if (pInput->GetPress(DIK_RETURN))
	{
		// キーフラグをセット
		isKeyPress = true;

		if (pInput->GetRepeat(DIK_RETURN, 15))
		{
			// 腕の武器の部分から弾を発射する
			CBullet::Create(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43), BulletMove, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 60);
		}

		// 攻撃してない時
		if (!m_isAttack)
		{
			// フラグを有効化
			m_isAttack = true;

			// 攻撃モーションに変更
			m_pMotion->SetMotion(m_pMotion->TYPE_ACTION);
		}
		else if (m_isAttack && m_pMotion->GetFinishMotion() == false)
		{
			// アタックをfalseにする
			m_isAttack = false;

			// 通常モーションに変更
			m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);

			// フラグをfalseにする
			isKeyPress = false;
		}
	}
	else if (isKeyPress == false && Type == m_pMotion->TYPE_ACTION)
	{
		// 通常モーションに変更
		m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);

		// 攻撃状態を解除
		m_isAttack = false;
	}

	//=========================
	// ジャンプ処理
	//=========================
	if (m_isJump == false)
	{// ジャンプしていなくて着地していないとき
		if (pInput->GetTrigger(DIK_SPACE))
		{
			// メッシュインパクト生成
			CMeshImpact::Create(m_pos, 80, 50.0f, 5.0f, 15.0f);

			// パーティクル生成
			CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 10.0f, m_pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),50,150,100,500);

			// フラグを変更
			m_isJump = true;
			m_isLanding = false;

			// ジャンプキーが押された
			m_move.y = PLAYER_JUMPVALUE;
		}
	}

	// ジャンプ可能時
	if (m_isJump == true)
	{
		// ジャンプモーションに変更
		m_pMotion->SetMotion(m_pMotion->TYPE_JUMP);

		if (m_isLanding == true)
		{
			// 着地モーションに変更
			m_pMotion->SetMotion(m_pMotion->TYPE_LANDING);

			// ジャンプ可能状態にする
			m_isJump = false;
		}
	}

	//=========================
	// 重力処理
	//=========================
	m_move.y -= 0.7f;
	m_posOld = m_pos;
	m_pos += m_move;

	// 座標が0以下
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_isLanding = true;
		m_move.y = 0.0f;
	}

	// 影の更新処理
	m_pShadow->UpdatePos(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));

	// モーション全体の更新
	m_pMotion->Update(m_apModel, MAX_MODEL);
}
//===============================
// プレイヤー描画処理
//===============================
void CPlayer::Draw(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxworld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// 全モデルパーツの描画
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}

	// デバッグ表示
	CDebugproc::Print("プレイヤー座標 [ %.2f,%.2f,%.2f ]", m_pos.x, m_pos.y, m_pos.z );
	CDebugproc::Draw(0, 20);

	CDebugproc::Print("プレイヤー向き [ %.2f,%.2f,%.2f ]", m_rot.x, m_rot.y, m_rot.z);
	CDebugproc::Draw(0, 140);

}
//=================================
// キー操作
//=================================
void CPlayer::MoveKey(CInputKeyboard* pInputKeyBoard,CCamera * pCamera)
{
#if 0
	// 移動処理
	if (pInputKeyBoard->GetPress(DIK_A))
	{// Aキーを押した

		if (pInputKeyBoard->GetPress(DIK_W))
		{// Wキーを押した
			m_move.x += sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * MAX_MOVE;
			m_move.z += cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
		}
		else if (pInputKeyBoard->GetPress(DIK_S))
		{// Sキーを押した
			m_move.x -= sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * MAX_MOVE;
			m_move.z -= cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
		}
		else
		{// Aキーのみを押した
			m_move.x -= sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * MAX_MOVE;
			m_move.z -= cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
		}

		// 角度の正規化
		if (m_rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// モーションセット
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE, m_pMotion->TYPE_NEUTRAL, false, 30);
	}
	else if (pInputKeyBoard->GetPress(DIK_D))
	{// Dキーを押した
		if (pInputKeyBoard->GetPress(DIK_W))
		{// Wキーを押した
			m_move.x += sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * MAX_MOVE;
			m_move.z += cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.75f);

		}
		else if (pInputKeyBoard->GetPress(DIK_S))
		{// Sキーを押した
			m_move.x -= sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * MAX_MOVE;
			m_move.z -= cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.25f);
		}
		else
		{// Dキーのみ押した
			m_move.x += sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * MAX_MOVE;
			m_move.z += cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * MAX_MOVE;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.5f);
		}

		// 角度の正規化
		if (m_rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// モーションセット
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE, m_pMotion->TYPE_NEUTRAL, false, 40);

	}
	else if (pInputKeyBoard->GetPress(DIK_W))
	{// Wキーを押した

		m_move.x += sinf(pCamera->GetRot().y) * MAX_MOVE;
		m_move.z += cosf(pCamera->GetRot().y) * MAX_MOVE;
		m_rotDest.y = pCamera->GetRot().y - (D3DX_PI);

		// 角度を正規化
		if (m_rot.y < -D3DX_PI)
		{// D3DX_PIより小さくなったら
			m_rot.y += D3DX_PI * 2.0f;
		}

		// モーションセット
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE, m_pMotion->TYPE_NEUTRAL, false, 40);

	}
	else if (pInputKeyBoard->GetPress(DIK_S))
	{// Sキーを押した

		m_move.x -= sinf(pCamera->GetRot().y) * MAX_MOVE;
		m_move.z -= cosf(pCamera->GetRot().y) * MAX_MOVE;
		m_rotDest.y = pCamera->GetRot().y;

		// 角度の正規化
		if (m_rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// モーションセット
		m_pMotion->SetMotion(m_pMotion->TYPE_MOVE, m_pMotion->TYPE_NEUTRAL, false, 30);

	}
	else
	{
		// 通常時はニュートラル
		m_pMotion->SetMotion(TYPE_NEUTRAL, TYPE_MOVE, false, 0);
	}

	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{// 左回転
		// 角度
		m_rot.y = m_rot.y + D3DX_PI * 2.0f;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{// 右回転
		// 角度
		m_rot.y = m_rot.y - D3DX_PI * 2.0f;
	}

	// Enterキー入力
	if (pInputKeyBoard->GetTrigger(DIK_RETURN))
	{
		// 攻撃モーションに変更
		m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL, m_pMotion->TYPE_NEUTRAL, true, 40);
	}

	if (bJump == false)
	{// ジャンプしていなくて着地していないとき
		if (pInputKeyBoard->GetTrigger(DIK_SPACE))
		{
			// 攻撃モーションに変更
			m_pMotion->SetMotion(m_pMotion->TYPE_ACTION, NULL, false, 0);

			bJump = true;
			isLanding = false;

			// ジャンプキーが押された
			m_move.y = 15.0f;
		}
	}
	// ジャンプが使われている
	if (bJump == true)
	{
		if (isLanding == true)
		{
			bJump = false;
		}
	}
#endif
}
//=========================================
// 識別番号ごとのプレイヤーの更新処理
//=========================================
void CPlayer::UpdateIdxPlayer(int nIdx,CInputKeyboard * pInputKey)
{
	// 識別番号ごとのプレイヤーの動き
	switch (nIdx)
	{
	case NUMBER_MAIN:

		break;

	case NUMBER_SUB:

		break;

	default:

		break;
	}
}
