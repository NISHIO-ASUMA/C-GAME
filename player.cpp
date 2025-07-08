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
#include "state.h"
#include "parameter.h"

//**********************
// 定数宣言
//**********************
constexpr float PLAYER_MOVE = 0.0095f; // 1フレームの移動量
constexpr float PLAYER_JUMPVALUE = 20.0f; // ジャンプ量
constexpr int   NUMBER_MAIN = 0;       // メイン操作プレイヤー番号
constexpr int   NUMBER_SUB = 1;		   // 分身操作プレイヤー番号

//===============================
// オーバーロードコンストラクタ
//===============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_move = VECTOR3_NULL;
	m_StateCount = NULL;
	m_nIdxTexture = NULL;
	m_rotDest = VECTOR3_NULL;
	m_nNumAll = NULL;
	m_type = NULL;
	m_posOld = VECTOR3_NULL;
	m_size = NULL;
	m_pFilename = {};
	m_nIdxPlayer = NULL;
	m_fAngle = NULL;

	// モデルのポインタのクリア
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}

	// クラスポインタ
	m_pMotion = nullptr;
	m_pShadow = nullptr;
	m_pState = nullptr;
	m_pParameter = nullptr;

	// フラグメント
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

	// インスタンスがnullptrだったら
	if (pPlayer == nullptr) return nullptr;

	// オブジェクト設定
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
	pPlayer->m_nIdxPlayer = nIdxParson;
	pPlayer->m_pFilename = pFilename;

	// パラメータ生成
	pPlayer->m_pParameter = new CParameter;

	// nullチェック
	if (pPlayer->m_pParameter != nullptr)
	{
		// 体力設定
		pPlayer->m_pParameter->SetHp(nLife);
	}

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
	m_type = CMotion::TYPE_MAX;
	m_fAngle = 0.0f;

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

	// nullptrチェック
	if (m_pMotion != nullptr)
	{
		// ポインタの破棄
		delete m_pMotion;

		// nullptrにする
		m_pMotion = nullptr;
	}

	// nullptrチェック
	if (m_pState != nullptr)
	{
		// ポインタの破棄
		delete m_pState;

		// nullptrにする
		m_pState = nullptr;
	}

	// nullptrチェック
	if (m_pParameter != nullptr)
	{
		// ポインタの破棄
		delete m_pParameter;

		// nullptrにする
		m_pParameter = nullptr;
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//============================================================
// プレイヤー更新処理  ( 角度をプレイヤーの移動量として渡す )
//============================================================
void CPlayer::Update(void)
{
	// 標準角度
	static float fAngle = NULL;

	// キーボードの入力取得
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// シリンダー座標,半径の取得
	D3DXVECTOR3 MeshPos = CManager::GetCylinder()->GetPos();
	float fRadius = CManager::GetCylinder()->GetRadius();

	// ボス座標の取得
	D3DXVECTOR3 BossPos = CManager::GetBoss()->GetPos();

	// ボスの位置に対してプレイヤー座標からベクトルを取る
	D3DXVECTOR3 VecBoss = BossPos - m_pos;

	// 水平方向に合わせる
	VecBoss.y = 0.0f;

	// ベクトルの正規化
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// 正規化された値を弾の移動量に設定する
	D3DXVECTOR3 BulletMove = VecBoss;

	// プレイヤーの腕のワールドマトリックスを取得する
	D3DXMATRIX mtxWorld = m_apModel[18]->GetMtxWorld();

	// 攻撃状態じゃない かつ 現在モーションがジャンプ攻撃じゃない時
	if (!m_isAttack && m_pMotion->GetMotionType() != CMotion::TYPE_JUMPATTACK)
	{

		// 移動更新関数
		UpdateMove(MeshPos,pInput);
#if 0
		if (pInput->GetPress(DIK_A))
		{// Aキー入力

			// 角度更新
			fAngle += PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // 左

			// 移動モーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else if (pInput->GetPress(DIK_D)) 
		{// Dキー入力

			// 角度更新
			fAngle -= PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // 右

			// 移動モーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else
		{
			// 移動モーション時,ニュートラルモーションに変更
			if (m_pMotion->GetMotionType() == CMotion::TYPE_MOVE)m_pMotion->SetMotion(CMotion::TYPE_NEUTRAL);
		}
#endif
	}

	//// 角度を正規化
	//if (m_rotDest.y - m_rot.y > D3DX_PI)
	//{
	//	m_rot.y += D3DX_PI * 2.0f;
	//}
	//else if (m_rot.y - m_rotDest.y > D3DX_PI)
	//{
	//	m_rot.y -= D3DX_PI * 2.0f;
	//}

	//// 自身の角度を計算
	//float IdxAngle = (m_nIdxPlayer == NUMBER_MAIN) ? fAngle : fAngle + D3DX_PI;

	//// 対角線座標を中心から計算
	//m_pos.x = MeshPos.x - sinf(IdxAngle) * fRadius;
	//m_pos.z = MeshPos.z - cosf(IdxAngle) * fRadius;

	//// 現在の角度に設定
	//m_rot.y += (m_rotDest.y - m_rot.y) + D3DX_PI;

	// モーションのフラグ
	bool isJumpAttacking = (m_pMotion->GetMotionType() == m_pMotion->TYPE_JUMPATTACK);

	// キーフラグをセット
	bool isKeyPress = false;

	//====================
	// 攻撃処理
	//====================
	if (pInput->GetPress(DIK_RETURN) && m_pMotion->GetMotionType() != CMotion::TYPE_JUMPATTACK)
	{
		// キーフラグをセット
		isKeyPress = true;

		// 15フレーム攻撃キーを入力していたら
		if (pInput->GetRepeat(DIK_RETURN, 15))
		{
			// 弾を生成
			CBullet::Create(
				D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43),
				VecBoss,
				CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 60);
		}

		if (!m_isAttack) 	// 攻撃状態じゃないとき
		{
			// 攻撃フラグを有効化する
			m_isAttack = true;

			// 地上攻撃モーション変更
			m_pMotion->SetMotion(CMotion::TYPE_ACTION);
		}
		else if (m_isAttack && !m_pMotion->GetFinishMotion()) // 攻撃状態 かつ モーション終了判定がfalseの時
		{
			// 攻撃フラグを無効化する
			m_isAttack = false;

			// ニュートラルモーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_NEUTRAL);

			// キー入力フラグを無効にする
			isKeyPress = false;
		}
	}
	else if (!isKeyPress && m_pMotion->GetMotionType() == CMotion::TYPE_ACTION)
	{// キーフラグが無効 かつ 現在のモーションが攻撃モーションなら

		// 通常モーションに変更
		m_pMotion->SetMotion(m_pMotion->TYPE_NEUTRAL);

		// 攻撃状態を解除
		m_isAttack = false;
	}

	// 攻撃状態 かつ モーションの状態が攻撃じゃなかったら
	if (m_isAttack && m_pMotion->GetMotionType() != CMotion::TYPE_ACTION)
	{
		// 攻撃を終了
		m_isAttack = false;
	}

	// 空中攻撃中
	if (isJumpAttacking && pInput->GetPress(DIK_RETURN))
	{
		// 一定の高さで静止する
		m_move.y = 0.0f;
	}
	else
	{
		// 重力値を適用
		m_move.y -= 0.7f;
	}

	// ジャンプキー入力 かつ ジャンプフラグがfalseの時
	if (!m_isJump && pInput->GetTrigger(DIK_SPACE))
	{
		// フラグを有効化
		m_isJump = true;

		// 未着地判定に変更
		m_isLanding = false;

		// 上昇値を設定
		m_move.y = PLAYER_JUMPVALUE;
	}

	// ジャンプ中処理
	if (m_isJump)
	{
		// ジャンプモーションに変更
		m_pMotion->SetMotion(CMotion::TYPE_JUMP);

		// ジャンプ中に攻撃キー入力
		if (pInput->GetPress(DIK_RETURN))
		{
			// 攻撃キーを15フレーム押し続けていたら
			if (pInput->GetRepeat(DIK_RETURN, 15))
			{
				// 弾を生成
				CBullet::Create(
					D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43),
					VecBoss,
					CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 30);
			}

			// ジャンプ攻撃モーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_JUMPATTACK);
		}

		// 着地時の処理
		if (m_isLanding)
		{
			// 着地モーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_LANDING);

			// ジャンプ可能状態に変更
			m_isJump = false;

			// 衝撃波を生成
			CMeshImpact::Create(m_pos, 80, 50.0f, 5.0f, 15.0f);
		}
	}
	
	// 移動処理
	m_posOld = m_pos;
	m_pos += m_move;

	// 現在のy座標が0.0f以下の時
	if (m_pos.y <= 0.0f)
	{
		// y座標を地面の座標に設定
		m_pos.y = 0.0f;

		// 着地判定を有効化
		m_isLanding = true;

		// 高さへの移動量を0.0fに設定
		m_move.y = 0.0f;
	}

	// HPチェック
	if (m_pParameter->GetHp() <= 0)
	{
		// TODO : テスト検証
		// ダメージモーションに変更
		m_pMotion->SetMotion(CMotion::TYPE_DAMAGE);
	}

	// 状態管理を更新
	m_pState->Update();

	// 影の座標を更新
	m_pShadow->UpdatePos(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));

	// モーション全体を更新
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
	//CDebugproc::Print("プレイヤー座標 [ %.2f,%.2f,%.2f ]", m_pos.x, m_pos.y, m_pos.z );
	//CDebugproc::Draw(0, 20);

	//CDebugproc::Print("プレイヤー向き [ %.2f,%.2f,%.2f ]", m_rot.x, m_rot.y, m_rot.z);
	//CDebugproc::Draw(0, 140);

}
//=========================================
// 識別番号ごとのプレイヤーの取得
//=========================================
CPlayer* CPlayer::GetIdxPlayer(int Idx)
{
	// オブジェクトの先頭取得
	CObject* pObj = CObject::GetTop(static_cast<int>(CObject::PRIORITY::PLAYER));

	// pObjがnullptrじゃなかったら
	while (pObj != nullptr)
	{
		// オブジェクトのタイプがPLAYERの時
		if (pObj->GetObjType() == CObject::TYPE_PLAYER)
		{
			// プレイヤー型にキャスト
			CPlayer* pPlayer = static_cast<CPlayer*>(pObj);

			// 番号が一致していたら
			if (pPlayer->GetPlayerIndex() == Idx)
			{
				// ポインタを返す
				return pPlayer;
			}
		}

		// 次のプレイヤーを代入
		pObj = pObj->GetNext();
	}

	// 取得できなかった場合
	return nullptr;
}
//=========================================
// 識別番号ごとの攻撃更新処理
//=========================================
void CPlayer::UpdateAction(CInputKeyboard* pInputKeyboard)
{
	
}
//=========================================
// 識別番号ごとの移動更新処理
//=========================================
void CPlayer::UpdateMove(const D3DXVECTOR3 DestPos,CInputKeyboard* pInputKeyboard)
{
	// キー入力時の角度計算
	static float fAngle = NULL;

	// 円柱の半径を取得
	float fRadius = CManager::GetCylinder()->GetRadius();

	// 識別番号で処理を分別する
	switch (m_nIdxPlayer)
	{
	case NUMBER_MAIN: // メインプレイヤー

		// キー入力時の角度計算
		if (pInputKeyboard->GetPress(DIK_A))
		{
			// 角度更新
			fAngle += PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // 左向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			// 角度更新
			fAngle -= PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // 右向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else
		{
			// 移動モーション時,ニュートラルモーションに変更
			if (m_pMotion->GetMotionType() == CMotion::TYPE_MOVE)m_pMotion->SetMotion(CMotion::TYPE_NEUTRAL);
		}
		break;

	case NUMBER_SUB: // 対角線上のプレイヤー

		// キー入力時の角度計算
		if (pInputKeyboard->GetPress(DIK_A))
		{
			// 角度更新
			fAngle += PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // 左向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			// 角度更新
			fAngle -= PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // 右向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(CMotion::TYPE_MOVE);
		}
		else
		{
			// 移動モーション時,ニュートラルモーションに変更
			if (m_pMotion->GetMotionType() == CMotion::TYPE_MOVE)m_pMotion->SetMotion(CMotion::TYPE_NEUTRAL);
		}
		break;

	default:
		break;
	}

	// 角度を正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// 自身の角度を計算
	float IdxAngle = (m_nIdxPlayer == NUMBER_MAIN) ? fAngle : fAngle + D3DX_PI;

	// 対角線座標を中心から計算
	m_pos.x = DestPos.x - sinf(IdxAngle) * fRadius;
	m_pos.z = DestPos.z - cosf(IdxAngle) * fRadius;

	// 現在の角度を設定する
	if (m_nIdxPlayer == NUMBER_MAIN)
	{
		m_rot.y += (m_rotDest.y - m_rot.y) + D3DX_PI;
	}
	else if (m_nIdxPlayer == NUMBER_SUB)
	{
		m_rot.y += (m_rotDest.y - m_rot.y);
	}

	// 座標更新処理}
	m_posOld = m_pos;
	m_pos += m_move;

}
#if 0

// キーボードの取得
CInputKeyboard* pInput = CManager::GetInputKeyboard();

// メッシュの座標の取得
D3DXVECTOR3 MeshCylinderPos = CManager::GetCylinder()->GetPos();

#if 1
// ボスオブジェクトの取得
CObject* pObjBoss = CObject::GetTop(CObject::PRIORITY::BOSS);

// キャストする
CBoss* pBoss = CManager::GetBoss();

// ボスの座標の取得
D3DXVECTOR3 BossPos = pBoss->GetPos();
#endif

// メッシュの半径の取得
float fRadius = CManager::GetCylinder()->GetRadius();

// 現在のモーションタイプを取得
int Type = m_pMotion->GetMotionType();

// 角度計算用
static float fAngle = NULL;

//===========================
// 移動キー処理
//===========================
// 攻撃状態じゃない かつ インデックス番号がメインのプレイヤー
if (m_nIdxPlayer == NUMBER_MAIN && !m_isAttack && Type != m_pMotion->TYPE_JUMPATTACK)
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
		m_rotDest.y = fAngle + D3DX_PI * 0.5f; // 右

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

// 状態変更
if (pInput->GetTrigger(DIK_K))
{
	// 状態変更
	m_pState->SetState(m_pState->STATE_DAMAGE);
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

//=============================================
// 対応する角度と座標・向きを計算
//=============================================
float finalAngle = (m_nIdxPlayer == NUMBER_MAIN) ? fAngle : fAngle + D3DX_PI;

//=====================================================
// メッシュの座標から見てプレイヤーの位置を求める
//=====================================================
m_pos.x = MeshCylinderPos.x - (sinf(fAngle)) * fRadius;		// X座標
m_pos.z = MeshCylinderPos.z - (cosf(fAngle)) * fRadius;		// Z座標

// 目的角を設定
m_rotDest.y = finalAngle;

// 角度を計算
m_rot.y += (m_rotDest.y - m_rot.y) + D3DX_PI;

//=========================
// 攻撃処理
//=========================
// キー入力フラグ
bool isKeyPress = false;

#if 1
// ボスの方向へのベクトルを取得
D3DXVECTOR3 VecBoss = BossPos - m_pos;

// 水平方向で合わせる
VecBoss.y = NULL;

// ベクトルを正規化
D3DXVec3Normalize(&VecBoss, &VecBoss);

// 弾の移動成分を設定
D3DXVECTOR3 BulletMove = VecBoss;

#endif
// プレイヤーの腕のワールドマトリックスを取得
D3DXMATRIX mtxWorld = m_apModel[18]->GetMtxWorld();

// 攻撃キー かつ モーションがジャンプ攻撃でなければ
if (pInput->GetPress(DIK_RETURN) && Type != m_pMotion->TYPE_JUMPATTACK)
{
	// キーフラグをセット
	isKeyPress = true;

	// 15フレームごとに弾を発射
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

// 攻撃フラグがオン かつ アクションモーションじゃなければ
if (m_isAttack && m_pMotion->GetMotionType() != m_pMotion->TYPE_ACTION)
{
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

	// ジャンプしているなら
	if (pInput->GetPress(DIK_RETURN))
	{
		// 弾の発射間隔を調整する
		if (pInput->GetRepeat(DIK_RETURN, 15))
		{
			// 腕の武器の部分から弾を発射する
			CBullet::Create(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43), BulletMove, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 30);
		}

		// ジャンプ攻撃モーションに変更
		m_pMotion->SetMotion(m_pMotion->TYPE_JUMPATTACK);

	}

	// 着地時
	if (m_isLanding == true)
	{
		// 着地モーションに変更
		m_pMotion->SetMotion(m_pMotion->TYPE_LANDING);

		// ジャンプ可能状態にする
		m_isJump = false;

		// メッシュインパクト生成
		CMeshImpact::Create(m_pos, 80, 50.0f, 5.0f, 15.0f);
	}
}

// モーションのフラグ
bool isJumpAttacking = (m_pMotion->GetMotionType() == m_pMotion->TYPE_JUMPATTACK);

// 攻撃キー押してる間は静止
if (isJumpAttacking && pInput->GetPress(DIK_RETURN))
{
	m_move.y = 0.0f; // 高さキープ
}
else
{
	m_move.y -= 0.7f; // 重力適用
}

//=========================
// 位置更新
//=========================
m_posOld = m_pos;
m_pos += m_move;

// 座標が0以下
if (m_pos.y <= 0.0f)
{
	m_pos.y = 0.0f;
	m_isLanding = true;
	m_move.y = 0.0f;
}

// 現在体力の取得
int nLife = m_pParameter->GetHp();

// 体力が0以下
if (nLife <= 0)
{
	// 死亡モーションに変更
	m_pMotion->SetMotion(m_pMotion->TYPE_DAMAGE);
}

// 状態管理クラスの更新
m_pState->Update();

// 影の更新処理
m_pShadow->UpdatePos(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));

// モーション全体の更新
m_pMotion->Update(m_apModel, MAX_MODEL);

#endif
