//====================================
//
// カメラ処理 [ camera.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "camera.h"
#include "Manager.h"
#include "input.h"
#include "player.h"
#include "debugproc.h"
#include "object.h"
#include "boss.h"

//**********************
// マクロ定義
//**********************
#define MAX_VIEWUP (3.0f)   // カメラの制限
#define MAX_VIEWDOWN (0.1f) // カメラの制限

//=================================
// コンストラクタ
//=================================
CCamera::CCamera()
{
	// 値のクリア
	m_pCamera.mtxprojection = {};
	m_pCamera.mtxView = {};
	m_pCamera.posR = VECTOR3_NULL;
	m_pCamera.posV = VECTOR3_NULL;
	m_pCamera.rot = VECTOR3_NULL;
	m_pCamera.vecU = VECTOR3_NULL;
	m_pCamera.posRDest = VECTOR3_NULL;
	m_pCamera.fDistance = NULL;
	m_pCamera.nMode = MODE_NONE;
}
//=================================
// デストラクタ
//=================================
CCamera::~CCamera()
{
	// 無し
}
//=================================
// 初期化処理
//=================================
HRESULT CCamera::Init(void)
{
	// 初期値を設定する
	m_pCamera.posV = D3DXVECTOR3(0.0f, 500.0f, -250.0f);		// カメラの位置
	m_pCamera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの見ている位置
	m_pCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
	m_pCamera.rot = D3DXVECTOR3(D3DX_PI * 0.55f, 0.0f, 0.0f);	// 角度

	// 距離を計算
	float fRotx = m_pCamera.posV.x - m_pCamera.posR.x;
	float fRoty = m_pCamera.posV.y - m_pCamera.posR.y;
	float fRotz = m_pCamera.posV.z - m_pCamera.posR.z;

	// 視点から注視点までの距離
	m_pCamera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));

	// 初期モード
	m_pCamera.nMode = MODE_NONE;

	return S_OK;
}
//=================================
// 終了処理
//=================================
void CCamera::Uninit(void)
{
	// 無し
}
//=================================
// 更新処理
//=================================
void CCamera::Update(void)
{
	// 入力情報を取得
	CInputMouse* pMouse = CManager::GetMouse();
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// プレイヤー取得
	CObject* pObjPlayer = CObject::GetTop(static_cast<int>(CObject::PRIORITY::PLAYER));

	// 取得失敗時
	if (pObjPlayer == nullptr)
	{
		return;
	}

	// プレイヤー型にキャストする
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObjPlayer);

	// ボス取得
	CBoss* pBoss = CManager::GetBoss();

#if 0
	// プレイヤー取得
	CObject* pObjPlayer = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER), 0);

	// 取得失敗時
	if (pObjPlayer == nullptr)
	{
		return;
	}

	// プレイヤー型にキャストする
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObjPlayer);

	// ボス取得
	CBoss* pBoss = CManager::GetBoss();

#endif

#ifdef _DEBUG
	// カメラモード変更
	if (pInput->GetTrigger(DIK_N))
	{
		m_pCamera.nMode = MODE_LOCKON;
	}
	if (pInput->GetTrigger(DIK_M))
	{
		m_pCamera.nMode = MODE_PLAYER;
	}
	if (pInput->GetTrigger(DIK_B))
	{
		m_pCamera.nMode = MODE_NONE;
	}
#endif

	switch (m_pCamera.nMode)
	{
	case MODE_NONE:

		// マウス視点移動
		MouseView(pMouse);

		break;

	case MODE_PLAYER:
		// 追従カメラ用に設定
		m_pCamera.posRDest.x = pPlayer->GetPos().x + sinf(pPlayer->GetRotDest().y) * 1.0f;
		m_pCamera.posRDest.y = pPlayer->GetPos().y + cosf(pPlayer->GetRotDest().y) * 1.0f;
		m_pCamera.posRDest.z = pPlayer->GetPos().z + cosf(pPlayer->GetRotDest().y) * 1.0f;

		m_pCamera.posR.x += ((m_pCamera.posRDest.x - m_pCamera.posR.x) * 0.3f);
		m_pCamera.posR.y += ((m_pCamera.posRDest.y - m_pCamera.posR.y) * 0.3f);
		m_pCamera.posR.z += ((m_pCamera.posRDest.z - m_pCamera.posR.z) * 0.3f);

		// カメラの視点の情報
		m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;

		break;

	case MODE_LOCKON:
	{
		
		// プレイヤーとボスの座標取得
		D3DXVECTOR3 playerPos = pPlayer->GetPos();
		D3DXVECTOR3 bossPos = pBoss->GetPos();

		// ボスへの向きベクトルを作成
		D3DXVECTOR3 vecToBoss = bossPos - playerPos;
		vecToBoss.y = 0.0f; // 高さは無視

		// 正規化
		D3DXVec3Normalize(&vecToBoss, &vecToBoss);

		// プレイヤーの回転角をボスの方向に向ける
		float angleToBoss = atan2f(vecToBoss.x, vecToBoss.z);
		pPlayer->SetRotDest(D3DXVECTOR3(0, angleToBoss, 0));

		// カメラ座標計算
		D3DXVECTOR3 camOffset = -vecToBoss * 280.0f; // プレイヤーの後方へ距離をとる
		camOffset.y = 160.0f; // カメラの高さ

		// カメラの座標をプレイヤー位置＋オフセットに設定
		D3DXVECTOR3 desiredPosV = playerPos + camOffset;

		// 注視点はボスにセット
		D3DXVECTOR3 targetBoss = bossPos;
		targetBoss.y = playerPos.y + 50.0f;

		// カメラ座標にセット
		m_pCamera.posV += (desiredPosV - m_pCamera.posV) * 0.3f; // 追従
		m_pCamera.posR += (targetBoss - m_pCamera.posR) * 0.3f;  // 注視点補間
	}
		break;

	default:
		break;
	}


	// 角度の正規化
	if (m_pCamera.rot.y > D3DX_PI)
	{// D3DX_PIより大きくなったら
		m_pCamera.rot.y -= D3DX_PI * 2.0f;
	}

	// 角度の正規化
	if (m_pCamera.rot.y < -D3DX_PI)
	{// D3DX_PIより小さくなったら
		m_pCamera.rot.y += D3DX_PI * 2.0f;
	}
}
//=================================
// カメラをセット
//=================================
void CCamera::SetCamera(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_pCamera.mtxView,
		&m_pCamera.posV,
		&m_pCamera.posR,
		&m_pCamera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera.mtxprojection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_pCamera.mtxprojection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		3000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera.mtxprojection);

	CDebugproc::Print("Camera : PosV [ %.2f, %.2f, %.2f ]", m_pCamera.posV.x, m_pCamera.posV.y, m_pCamera.posV.z);
	CDebugproc::Draw(0, 100);
}
//======================================
// マウス操作の視点移動
//======================================
void CCamera::MouseView(CInputMouse * pMouse)
{
	// 右クリック
	if (pMouse->GetPress(CInputMouse::MOUSE_LEFT))
	{
		D3DXVECTOR2 Move = pMouse->GetMouseVelocity();
		D3DXVECTOR2 MoveOld = pMouse->GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		// 回転量を更新
		m_pCamera.rot.y += fAngle.x * 0.01f;
		m_pCamera.rot.x += fAngle.y * 0.01f;

		// 回転量を制限
		if (m_pCamera.rot.x > MAX_VIEWUP)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}
		else if (m_pCamera.rot.x < MAX_VIEWDOWN)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}

		// カメラ座標を更新
		m_pCamera.posR.x = m_pCamera.posV.x + sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		m_pCamera.posR.y = m_pCamera.posV.y + cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		m_pCamera.posR.z = m_pCamera.posV.z + sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
	}
	// 左クリック
	else if (pMouse->GetPress(CInputMouse::MOUSE_RIGHT))
	{
		D3DXVECTOR2 Move = pMouse->GetMouseVelocity();
		D3DXVECTOR2 MoveOld = pMouse->GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		// 回転量を更新
		m_pCamera.rot.y += fAngle.x * 0.01f;
		m_pCamera.rot.x += fAngle.y * 0.01f;

		// 回転量を制限
		if (m_pCamera.rot.x > MAX_VIEWUP)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}
		else if (m_pCamera.rot.x < MAX_VIEWDOWN)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}

		// カメラの視点の情報
		m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
	}

	// 正規化
	if (m_pCamera.rot.y < -D3DX_PI)
	{
		m_pCamera.rot.y += D3DX_PI * 2.0f;
	}
	else if (m_pCamera.rot.y > D3DX_PI)
	{
		m_pCamera.rot.y += -D3DX_PI * 2.0f;
	}
	if (m_pCamera.rot.x < -D3DX_PI)
	{
		m_pCamera.rot.x += D3DX_PI * 2.0f;
	}
	else if (m_pCamera.rot.x > D3DX_PI)
	{
		m_pCamera.rot.x += -D3DX_PI * 2.0f;
	}

}
