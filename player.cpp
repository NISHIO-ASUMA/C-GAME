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
#include "manager.h"
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
#include "shadowS.h"
#include "gamemanager.h"
#include "playerstate.h"
#include "state.h"

//**********************
// 定数宣言
//**********************
constexpr float PLAYER_MOVE = 0.0095f;		// 1フレームの移動量
constexpr float PLAYER_JUMPVALUE = 17.0f;	// ジャンプ量
constexpr int   NUMBER_MAIN = 0;			// メイン操作プレイヤー番号
constexpr int   NUMBER_SUB = 1;				// 分身操作プレイヤー番号
constexpr int   KeyRepeatCount = 15;		// キーのリピートカウント

//**********************
// 静的メンバ変数宣言
//**********************
bool CPlayer::m_isDeath = false;  // 死亡フラグ

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
	m_pParameter = nullptr;
	m_pShadowS = nullptr;
	m_pStateMachine = nullptr;

	// フラグメント
	m_isLanding = false;
	m_isJump = false;
	m_isAttack = false;
	m_isMoving = false;
	m_isShadow = false;
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

	// 体力は2体で共通
	if (nIdxParson == NUMBER_MAIN)
	{
		// ポインタ生成
		pPlayer->m_pParameter = new CParameter;

		// nullチェック
		if (pPlayer->m_pParameter != nullptr)
		{
			// 体力パラメーターを設定
			pPlayer->m_pParameter->SetHp(nLife);
		}
	}
	else
	{
		// メイン側の情報をセット
		CPlayer* pMain = CPlayer::GetIdxPlayer(NUMBER_MAIN);

		// nullptrチェック
		if (pMain != nullptr)
		{
			// ポインタを共有
			pPlayer->m_pParameter = pMain->m_pParameter; 
		}
	}

	// プレイヤー初期化処理
	if (FAILED(pPlayer->Init()))
	{
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

	// 角度初期化
	m_fAngle = NULL;

	// フラグを設定
	m_isDeath = false;
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
	
	// 一人目のプレイヤーなら
	if (m_nIdxPlayer == NUMBER_MAIN)
	{
		// ステンシルシャドウ生成
		m_pShadowS = CShadowS::Create("data\\MODEL\\STAGEOBJ\\Shadowmodel.x", CPlayer::GetIdxPlayer(0)->GetPos(), CPlayer::GetIdxPlayer(0)->GetRot());
	}

	// 初期座標の向きを設定
	InitPos(NULL);

	// ステートマシンを生成
	m_pStateMachine = new CStateMachine;	

	// 初期状態をセット
	ChangeState(new CPlayerStateNeutral,CPlayerStateBase::ID_NEUTRAL); 

	// 結果を返す
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
	if (m_pParameter != nullptr && m_nIdxPlayer == NUMBER_MAIN)
	{
		// ポインタの破棄
		delete m_pParameter;

		// nullptrにする
		m_pParameter = nullptr;
	}

	// nullptrチェック
	if (m_pStateMachine != nullptr)
	{
		// 終了処理
		m_pStateMachine->OnExit();

		// ポインタの破棄
		delete m_pStateMachine;

		// nullptrにする
		m_pStateMachine = nullptr;
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//============================================================
// プレイヤー更新処理 ( 角度をプレイヤーの移動量として渡す )
//============================================================
void CPlayer::Update(void)
{
	if (m_isDeath) return;

	// 攻撃中はボスの方向に体を向ける
	if (m_isAttack)
	{
		D3DXVECTOR3 BossDir = CGameManager::GetBoss()->GetPos() - m_pos;
		BossDir.y = 0.0f;

		if (D3DXVec3LengthSq(&BossDir) > 0.0001f)
		{
			D3DXVec3Normalize(&BossDir, &BossDir);
			m_rot.y = atan2f(-BossDir.x, -BossDir.z);
		}
	}

	// nullptrじゃないとき
	if (m_pStateMachine != nullptr)
	{
		// ステート更新
		m_pStateMachine->Update();
	}

	// 入力取得
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// 武器のワールドマトリックスとボス方向取得
	CModel* pModelWeapon = GetModelPartType(CModel::PARTTYPE_WEAPON);
	if (!pModelWeapon) return;

	D3DXMATRIX mtxWorld = pModelWeapon->GetMtxWorld();
	D3DXVECTOR3 vecToBoss = VecToBoss(m_pos);

	// ジャンプ状態時の更新関数
	UpdateJumpAction(pInput, mtxWorld, vecToBoss);

	// 当たり判定処理関数
	Collision();

	// SUBプレイヤーだけ処理
	if (m_nIdxPlayer == NUMBER_SUB)
	{
		// MAINプレイヤー取得
		CPlayer* pMain = CPlayer::GetIdxPlayer(NUMBER_MAIN);

		// モーションを統一する
		m_pMotion->SetMotion(pMain->GetNowMotion());

		// ステートを統一
		int mainStateID = pMain->m_pStateMachine->GetNowStateID();
		int subStateID = m_pStateMachine->GetNowStateID();

		// もし一致していなかったら
		if (mainStateID != subStateID)
		{
			switch (mainStateID)
			{
			case CPlayerStateBase::ID_DAMAGE: //ダメージ
				ChangeState(new CPlayerStateDamage(0), CPlayerStateBase::ID_DAMAGE);
				break;

			case CPlayerStateBase::ID_MOVE: // 移動
				ChangeState(new CPlayerStateMove(), CPlayerStateBase::ID_MOVE);
				break;

			case CPlayerStateBase::ID_ACTION: //アクション
				ChangeState(new CPlayerStateAction(), CPlayerStateBase::ID_ACTION);
				break;

			case CPlayerStateBase::ID_NEUTRAL: // ニュートラル
				ChangeState(new CPlayerStateNeutral(), CPlayerStateBase::ID_NEUTRAL);
				break;

			default:
				break;
			}
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

	// ステンシルシャドウが存在 かつ MAINプレイヤー
	if (m_pShadowS && m_nIdxPlayer == NUMBER_MAIN)
	{
		// 影座標をMAINプレイヤー座標に設定
		D3DXVECTOR3 ShadowPos = GetIdxPlayer(0)->GetPos();

		// オブジェクト設定
		m_pShadowS->SetPos(ShadowPos);
		m_pShadowS->SetRot(GetIdxPlayer(0)->GetRot()); 
	}

	// 現在体力の取得
	int nHp = m_pParameter->GetHp();

	// 生きているなら
	if (nHp >= 0) m_pMotion->Update(m_apModel, MAX_MODEL); // モーションの全体更新
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

	// 状態
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

	// 体力描画
	CDebugproc::Print("プレイヤーの体力 { %d } ", m_pParameter->GetHp());
	// デバッグフォント描画
	CDebugproc::Draw(0, 340);
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
// ステート変更
//=========================================
void CPlayer::ChangeState(CPlayerStateBase* pNewState,int id)
{
	// 自分自身を代入
	pNewState->SetOwner(this);

	// ステート変更
	m_pStateMachine->ChangeState(pNewState);
}

//=========================================
// 識別番号ごとの攻撃更新処理
//=========================================
void CPlayer::UpdateAction(CInputKeyboard* pInputKeyboard,D3DXMATRIX pMtx,const D3DXVECTOR3 DestMove)
{
	// キーフラグをセット
	bool isKeyPress = false;

	//====================
	// 攻撃処理
	//====================
	if (pInputKeyboard->GetPress(DIK_RETURN) && m_pMotion->GetMotionType() != CPlayer::PLAYERMOTION_JUMPATTACK)
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

			// 地上攻撃モーションに変更
			m_pMotion->SetMotion(CPlayer::PLAYERMOTION_ACTION);
		}
		else if (m_isAttack && !m_pMotion->GetFinishMotion()) // 攻撃状態 かつ モーション終了判定がfalseの時
		{
			// 攻撃フラグを無効化する
			m_isAttack = false;

			// キー入力フラグを無効にする
			isKeyPress = false;

			// ニュートラルモーションに変更
			ChangeState(new CPlayerStateNeutral(), CPlayerStateBase::ID_NEUTRAL);

			// ここで処理を返す
			return; 
		}
	}
	else if (!isKeyPress && m_pMotion->GetMotionType() == CPlayer::PLAYERMOTION_ACTION)
	{// キーフラグが無効 かつ 現在のモーションが攻撃モーションなら

		// 攻撃状態を解除
		m_isAttack = false;

		// ニュートラルモーションに変更
		ChangeState(new CPlayerStateNeutral(), CPlayerStateBase::ID_NEUTRAL);

		// ここで処理を返す
		return;

	}

	// 攻撃状態 かつ モーションの状態が攻撃じゃなかったら
	if (m_isAttack &&m_pMotion->GetMotionType() != CPlayer::PLAYERMOTION_ACTION)
	{
		// 攻撃状態を解除
		m_isAttack = false;
	}
}

//=========================================
// 識別番号ごとの移動更新処理
//=========================================
void CPlayer::UpdateMove(const D3DXVECTOR3 DestPos,CInputKeyboard* pInputKeyboard)
{
	// ジャンプ中かつ攻撃中なら移動処理を禁止
	if (m_pMotion->GetMotionType() == PLAYERMOTION_JUMPATTACK)
	{
		return; // この時は移動や方向変更なし
	}

	// キー入力時の角度計算
	static float fAngle = NULL;

	// 円柱の半径を取得
	float fRadius = CGameManager::GetCylinder()->GetRadius();

	// 識別番号で処理を分別する
	switch (m_nIdxPlayer)
	{
	case NUMBER_MAIN: // メインプレイヤー

		// キー入力時の角度計算
		if (pInputKeyboard->GetPress(DIK_A))
		{
			// 角度更新
			m_fAngle += PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = m_fAngle - D3DX_PI * 0.5f; // 左向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{
			// 角度更新
			m_fAngle -= PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = m_fAngle + D3DX_PI * 0.5f; // 右向きに設定

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
			m_fAngle += PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = m_fAngle - D3DX_PI * 0.5f; // 左向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else if (pInputKeyboard->GetPress(DIK_D)) // Dキー
		{
			// 角度更新
			m_fAngle -= PLAYER_MOVE;

			// 目的角を計算
			m_rotDest.y = m_fAngle + D3DX_PI * 0.5f; // 右向きに設定

			// 移動モーションに変更
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else
		{
			// 移動モーション時,ニュートラルモーションに変更
			if (m_pMotion->GetMotionType() == PLAYERMOTION_MOVE) m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL);
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
	float IdxAngle = (m_nIdxPlayer == NUMBER_MAIN) ? m_fAngle : m_fAngle + D3DX_PI;

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

	// 現在座標を更新
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
	if (!m_isJump)
	{
		if (pInputKeyboard->GetTrigger(DIK_SPACE))
		{
			// フラグを有効化
			m_isJump = true;

			// 未着地判定に変更
			m_isLanding = false;

			// 上昇値を設定
			m_move.y = PLAYER_JUMPVALUE;
		}
	}

	// 高さ更新
	AddMove();

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

			// 方向をボスに向かせる
			D3DXVECTOR3 BossDir = CGameManager::GetBoss()->GetPos() - m_pos;
			BossDir.y = 0.0f;

			if (D3DXVec3LengthSq(&BossDir) > 0.0001f)
			{
				D3DXVec3Normalize(&BossDir, &BossDir);
				m_rot.y = atan2f(-BossDir.x, -BossDir.z);
			}

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

	// モーション終了時　かつ 種類が着地モーション
	if (m_pMotion->GetMotionType() == PLAYERMOTION_LANDING && m_pMotion->GetFinishMotion())
	{
		// ニュートラルに変更
		ChangeState(new CPlayerStateNeutral(), CPlayerStateBase::ID_NEUTRAL);

		// ここで処理を返す
		return;
	}
}
//=============================
// コリジョン処理関数
//=============================
void CPlayer::Collision(void)
{
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

			// コリジョンした時 かつ IDがダメージ以外
			if (pImpact->Collision(&m_pos) == true )
			{
				// 当たったらダメージモーションに切り替え
				m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

				// ステート変更
				ChangeState(new CPlayerStateDamage(1), CPlayerStateBase::ID_DAMAGE);

				// 一回当たったら抜ける
				break;
			}
		}

		// 次のオブジェクトを検出する
		pObj = pObj->GetNext();
	}

	//=============================
	// ボス右手の当たり判定
	//=============================
	CBoss* pBoss = CGameManager::GetBoss();  // マネージャー経由でボスを取得する

	// nullだったら
	if (pBoss->IsDaeth())
	{
		return;
	}

	// 当たり判定の距離
	if (pBoss->CollisionRightHand(&m_pos) && pBoss->IsDaeth() == false)
	{
		// 当たったらダメージモーションに切り替え
		m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

		// ステート変更
		ChangeState(new CPlayerStateDamage(1), CPlayerStateBase::ID_DAMAGE);
	}

	// 当たり判定の距離
	if (pBoss->CollisionImpactScal(&m_pos) && pBoss->IsDaeth() == false)
	{
		// 当たったらダメージモーションに切り替え
		m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

		// ステート変更
		ChangeState(new CPlayerStateDamage(1), CPlayerStateBase::ID_DAMAGE);
	}

	//=============================
	// 敵との当たり判定
	//=============================
	// オブジェクト取得
	CObject* pObjEnemy = CObject::GetTop(static_cast<int>(CObject::PRIORITY::ENEMY));

	// nullptrじゃないとき
	while (pObjEnemy != nullptr)
	{
		// 敵のオブジェクトタイプを取得
		if (pObjEnemy->GetObjType() == CObject::TYPE_ENEMY)
		{
			// 敵にキャスト
			CEnemy* pEnemy = static_cast<CEnemy*>(pObjEnemy);

			if (m_nIdxPlayer != NUMBER_MAIN) break;

			// コリジョンしたとき
			if (pEnemy->Collision(&m_pos) == true )
			{
				// 当たったらダメージモーションに切り替え
				m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

				// ステート変更
				ChangeState(new CPlayerStateDamage(1), CPlayerStateBase::ID_DAMAGE);

				// 一回当たったら抜ける
				break;
			}
		}

		// 次のオブジェクトを検出する
		pObjEnemy = pObjEnemy->GetNext();
	}
}
//===============================
// プレイヤーとボス間のベクトル
//===============================
D3DXVECTOR3 CPlayer::VecToBoss(const D3DXVECTOR3& pPos)
{
	// ボスの座標取得
	D3DXVECTOR3 BossPos = CGameManager::GetBoss()->GetPos();

	// プレイヤーとボス間でベクトル生成
	D3DXVECTOR3 VecBoss = BossPos - pPos;
	VecBoss.y = 0.0f;

	// できたベクトルを正規化する
	D3DXVec3Normalize(&VecBoss, &VecBoss);

	// ベクトルを返す
	return VecBoss;
}
//===============================
// キー押下時の入力取得
//===============================
bool CPlayer::isMoveInputKey(CInputKeyboard* pKeyInput)
{
	// いずれかの移動キー入力
	return (pKeyInput->GetPress(DIK_A) || pKeyInput->GetPress(DIK_D) ||
			pKeyInput->GetPress(DIK_W) || pKeyInput->GetPress(DIK_S));
	
}
//===============================
// 初期座標計算関数
//===============================
void CPlayer::InitPos(float fAngle)
{
	// 円柱半径を取得
	float fRadius = CGameManager::GetCylinder()->GetRadius();

	// 角度の初期設定
	m_fAngle = fAngle;

	// 自身の角度を計算
	float IdxAngle = (m_nIdxPlayer == NUMBER_MAIN) ? m_fAngle : m_fAngle + D3DX_PI;

	// 円周上の初期位置を計算
	m_pos.x = CGameManager::GetCylinder()->GetPos().x - sinf(IdxAngle) * fRadius;
	m_pos.z = CGameManager::GetCylinder()->GetPos().z - cosf(IdxAngle) * fRadius;
	m_pos.y = 0.0f;

	// 現在の角度を設定する
	if (m_nIdxPlayer == NUMBER_MAIN)
	{
		m_rot.y += (m_rotDest.y - m_rot.y) + D3DX_PI;
	}
	else if (m_nIdxPlayer == NUMBER_SUB)
	{
		m_rot.y += (m_rotDest.y - m_rot.y);
	}

	// 回転をセット
	m_rotDest.y = m_rot.y;

	// 前フレーム座標をセット
	m_posOld = m_pos;
}
//===============================
// ジャンプ制御関数
//===============================
void CPlayer::StartJump(void)
{
	// ジャンプキー入力 かつ ジャンプフラグがfalseの時
	if (!m_isJump)
	{
		// フラグを有効化
		m_isJump = true;

		// 未着地判定に変更
		m_isLanding = false;

		// 上昇値を設定
		m_move.y = PLAYER_JUMPVALUE;
	}
}
//===============================
// 現在のモーション種類取得
//===============================
CPlayer::PLAYERMOTION CPlayer::GetNowMotion() const
{
	// nullじゃなかったら
	if (m_pMotion)
	{
		return static_cast<CPlayer::PLAYERMOTION>(m_pMotion->GetMotionType());
	}

	return PLAYERMOTION_NEUTRAL; // デフォルト
}
//===============================
// ヒット処理
//===============================
void CPlayer::HitDamage(int nDamage)
{
	// パラメーター取得
	int nHp = m_pParameter->GetHp();

	// 体力を減らす
	nHp -= nDamage;

	// 現在体力が0以下
	if (nHp <= 0)
	{
		// 現在体力をセット
		m_pParameter->SetHp(nHp);

		// 死亡フラグをセット
		m_isDeath = true;

		// モーションセット
		m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);

		// ここで処理を返す
		return;
	}
	else
	{
		// 現在体力をセット
		m_pParameter->SetHp(nHp);

		// モーションセット
		m_pMotion->SetMotion(PLAYERMOTION_DAMAGE);
	}
}