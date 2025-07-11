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
#include "debugproc.h"

//**********************
// 定数宣言
//**********************
constexpr float PLAYER_MOVE = 0.0095f;		// 1フレームの移動量
constexpr float PLAYER_JUMPVALUE = 18.0f;	// ジャンプ量
constexpr int   NUMBER_MAIN = 0;			// メイン操作プレイヤー番号
constexpr int   NUMBER_SUB = 1;				// 分身操作プレイヤー番号
constexpr int   KeyRepeatCount = 15;		// キーのリピートカウント

//===============================
// オーバーロードコンストラクタ
//===============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_move = VECTOR3_NULL;
	m_State = NULL;
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

	// モーション種類数を代入
	m_type = PLAYERMOTION_MAX;

	m_fAngle = NULL;

	// フラグを設定
	m_isJump = false;
	m_isLanding = false;
	m_isMoving = false;

	// モーションの読み込み
	m_pMotion = CMotion::Load(m_pFilename, MAX_MODEL, m_apModel,PLAYERMOTION_MAX);

	// モーション数を設定
	m_pMotion->SetMotionNum(PLAYERMOTION_MAX);

	// プレイヤー識別モデルフラグを設定
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		// nullptrじゃなかったら
		if (m_apModel[nCnt] != nullptr)
		{
			// フラグを設定する
			m_apModel[nCnt]->SetIsPlayer(true);
		}
	}

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

	// シリンダー座標取得
	D3DXVECTOR3 MeshPos = CManager::GetCylinder()->GetPos();

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
	D3DXMATRIX mtxWorld = {};

	// 右腕モデルの取得
	CModel* pModelWepon = CPlayer::GetModelPartType(CModel::PARTTYPE_WEAPON);

	// nullじゃないなら
	if (pModelWepon != nullptr)
	{
		// ワールドマトリックスをセット
		mtxWorld = pModelWepon->GetMtxWorld();
	}

	// 現在体力の取得
	int nLife = m_pParameter->GetHp();

	// 攻撃中はボスの方向に体を向ける
	if (m_isAttack)
	{
		// ボスからプレイヤーへのベクトル生成
		D3DXVECTOR3 BossDir = CManager::GetBoss()->GetPos() - m_pos;

		// y軸方向は一定
		BossDir.y = 0.0f;

		// 長さが0.0001fより大きいとき
		if (D3DXVec3LengthSq(&BossDir) > 0.0001f)
		{
			// ベクトルを正規化
			D3DXVec3Normalize(&BossDir, &BossDir);

			// プレイヤーの角度を合わせる
			m_rot.y = atan2f(-BossDir.x, -BossDir.z); // Y軸回転のみ調整
		}
	}

	// 攻撃状態じゃない かつ 現在モーションがジャンプ攻撃じゃない時
	if (!m_isAttack && m_pMotion->GetMotionType() != PLAYERMOTION_JUMPATTACK)
	{
		// 移動更新関数
		UpdateMove(MeshPos,pInput);
	}

	// 攻撃処理
	UpdateNeutralAction(pInput,mtxWorld, BulletMove);
	
	// ジャンプ処理
	UpdateJumpAction(pInput, mtxWorld, BulletMove);

	// 移動加算処理
	m_pos += m_move;

	// 現在の状態を取得
	m_State = m_pState->GetState();

	// 現在の状態をセットする
	m_pState->SetState(m_State);

	//=============================
	// インパクトとの当たり判定
	//=============================
	// オブジェクト取得
	CObject* pObj = CObject::GetTop(static_cast<int>(CObject::PRIORITY::IMPACT));

	// nullptrじゃないとき
	while (pObj != nullptr)
	{
		// メッシュタイプを取得
		if (pObj->GetObjType() == CObject::TYPE_MESH)
		{
			// インパクトにキャスト
			CMeshImpact* pImpact = static_cast<CMeshImpact*>(pObj);

			// コリジョンした かつ 状態が通常時
			if (pImpact->Collision(&m_pos) == true)
			{
				// 現在状態がNORMALなら
				if (m_State == m_pState->STATE_NORMAL)
				{
					// 当たったらダメージモーションに切り替え
					m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

					// 状態更新
					m_pState->SetState(CState::STATE_DAMAGE);

					// ダメージ処理
					m_pParameter->HitDamage(1);

					// 一回当たったら抜ける
					break;
				}
			}
		}

		// 次のオブジェクトを検出する
		pObj = pObj->GetNext();
	}

	//=============================
	// ボス右手の当たり判定
	//=============================
	CBoss* pBoss = CManager::GetBoss();  // マネージャー経由で取得する

	// 当たり判定の距離
	if (pBoss->CollisionRightHand(&m_pos))
	{
		// 状態が通常の時のみ
		if (m_State == m_pState->STATE_NORMAL)
		{
			// ダメージモーション
			m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

			// 状態更新
			m_pState->SetState(CState::STATE_DAMAGE);

			// 体力を減らす
			m_pParameter->HitDamage(1);
		}
	}

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

	// 現在体力をセット
	nLife = m_pParameter->GetHp();

	// ゼロ以下
	if (nLife <= 0)
	{
		// 終了処理
		Uninit();

		// ここで処理を返す
		return;
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

	// フォント
	CDebugproc::Print("現在のSTATE { %d } ", m_State);
	// デバッグフォント描画
	CDebugproc::Draw(0, 160);

	// 識別描画
	CDebugproc::Print("MAINプレイヤーの座標 { %.2f,%.2f,%.2f }", CPlayer::GetIdxPlayer(0)->GetPos().x, CPlayer::GetIdxPlayer(0)->GetPos().y, CPlayer::GetIdxPlayer(0)->GetPos().z);
	// デバッグフォント描画
	CDebugproc::Draw(0, 200);

	// 識別描画
	CDebugproc::Print("SUBプレイヤーの座標 { %.2f,%.2f,%.2f }", CPlayer::GetIdxPlayer(1)->GetPos().x, CPlayer::GetIdxPlayer(1)->GetPos().y, CPlayer::GetIdxPlayer(1)->GetPos().z);
	// デバッグフォント描画
	CDebugproc::Draw(0, 220);

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
// モデルの特定部分パーツの取得関数
//=========================================
CModel* CPlayer::GetModelPartType(CModel::PARTTYPE modelpart)
{
	// プレイヤーが持っているモデルの数の中から探す
	for (int nModel = 0; nModel < MAX_MODEL; nModel++)
	{
		// モデルがある かつ 取得したい物と一致していたら
		if (m_apModel[nModel] && m_apModel[nModel]->GetPartType() == modelpart)
		{
			// 該当モデルのidx番号のモデルを返す
			return m_apModel[nModel];
		}
	}

	// 該当なし
	return nullptr;
}
//=========================================
// 識別番号ごとの攻撃更新処理
//=========================================
void CPlayer::UpdateNeutralAction(CInputKeyboard* pInputKeyboard,D3DXMATRIX pMtx,const D3DXVECTOR3 DestMove)
{
	// キーフラグをセット
	bool isKeyPress = false;

	//====================
	// 攻撃処理
	//====================
	if (pInputKeyboard->GetPress(DIK_RETURN) && m_pMotion->GetMotionType() != PLAYERMOTION_JUMPATTACK)
	{
		// キーフラグをセット
		isKeyPress = true;

		// 15フレーム攻撃キーを入力していたら
		if (pInputKeyboard->GetRepeat(DIK_RETURN, KeyRepeatCount))
		{
			// 弾を生成
			CBullet::Create(D3DXVECTOR3(pMtx._41, pMtx._42, pMtx._43), DestMove, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 60);
		}

		// 攻撃状態じゃないとき
		if (!m_isAttack) 	
		{
			// 攻撃フラグを有効化する
			m_isAttack = true;

			// 地上攻撃モーション変更
			m_pMotion->SetMotion(PLAYERMOTION_ACTION);
		}
		else if (m_isAttack && !m_pMotion->GetFinishMotion()) // 攻撃状態 かつ モーション終了判定がfalseの時
		{
			// 攻撃フラグを無効化する
			m_isAttack = false;

			// ニュートラルモーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);

			// キー入力フラグを無効にする
			isKeyPress = false;
		}
	}
	else if (!isKeyPress && m_pMotion->GetMotionType() == PLAYERMOTION_ACTION)
	{// キーフラグが無効 かつ 現在のモーションが攻撃モーションなら

		// 通常モーションに変更
		m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);

		// 攻撃状態を解除
		m_isAttack = false;
	}

	// 攻撃状態 かつ モーションの状態が攻撃じゃなかったら
	if (m_isAttack && m_pMotion->GetMotionType() != PLAYERMOTION_ACTION)
	{
		// 攻撃を終了
		m_isAttack = false;
	}
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
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			// 角度更新
			fAngle -= PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // 右向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else
		{
			// 移動モーション時,ニュートラルモーションに変更
			if (m_pMotion->GetMotionType() == PLAYERMOTION_MOVE)m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);
		}
		break;

	case NUMBER_SUB: // 対角線上のプレイヤー

		// キー入力時の角度計算
		if (pInputKeyboard->GetPress(DIK_A)) // Aキー
		{
			// 角度更新
			fAngle += PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle - D3DX_PI * 0.5f; // 左向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D)) // Dキー
		{
			// 角度更新
			fAngle -= PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = fAngle + D3DX_PI * 0.5f; // 右向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else
		{
			// 移動モーション時,ニュートラルモーションに変更
			if (m_pMotion->GetMotionType() == PLAYERMOTION_MOVE)m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);
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

	// 座標更新処理
	m_posOld = m_pos;
}
//=========================================
// 識別番号ごとのジャンプ更新処理
//=========================================
void CPlayer::UpdateJumpAction(CInputKeyboard* pInputKeyboard, D3DXMATRIX pMtx, const D3DXVECTOR3 DestMove)
{
	// モーションのフラグ
	bool isJumpAttacking = (m_pMotion->GetMotionType() == PLAYERMOTION_JUMPATTACK);

	// 空中攻撃中
	if (isJumpAttacking && pInputKeyboard->GetPress(DIK_RETURN))
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
	if (!m_isJump && pInputKeyboard->GetTrigger(DIK_SPACE))
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
		m_pMotion->SetMotion(PLAYERMOTION_JUMP);

		// ジャンプ中に攻撃キー入力
		if (pInputKeyboard->GetPress(DIK_RETURN))
		{
			// 攻撃キーを15フレーム押し続けていたら
			if (pInputKeyboard->GetRepeat(DIK_RETURN, KeyRepeatCount))
			{
				// 弾を生成
				CBullet::Create(D3DXVECTOR3(pMtx._41, pMtx._42, pMtx._43), DestMove, CBullet::BTYPE_PLAYER, 5.0f, 5.0f, 45);
			}

			// ジャンプ攻撃モーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_JUMPATTACK);
		}

		// 着地時の処理
		if (m_isLanding)
		{
			// 着地モーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_LANDING);

			// ジャンプ可能状態に変更
			m_isJump = false;
		}
	}
}