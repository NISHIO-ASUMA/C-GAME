//====================================
//
// プレイヤー処理 [ player.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _PLAYER_H_ // このマクロ定義がされてなかったら
#define _PLAYER_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object.h"
#include "character.h"
#include "model.h"
#include "motion.h"
#include "state.h"

//**********************
// マクロ定義
//**********************
#define MAX_MODEL (19) // プレイヤーモデル数

//**********************
// 前方宣言
//**********************
class CShadow;
class CInputKeyboard;
class CCamera;

//*************************
// プレイヤークラスを定義
//*************************
class CPlayer : public CObject
{
public:
	// プレイヤー状態
	enum PLAYERSTATE
	{
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_NORMAL,
		PLAYERSTATE_APPER,
		PLAYERSTATE_DEATH,
		PLAYERSTATE_MAX
	};

	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::PLAYER));
	~CPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, const int nIdxParson,const char * pFilename); // 生成処理
	PLAYERSTATE GetState(void) { return m_State; }

	D3DXVECTOR3 GetPos(void) { return m_pos; } // 現在の座標を取得
	D3DXVECTOR3 GetRot(void) { return m_rot; } // 現在の角度を取得
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }

	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }
	void MoveKey(CInputKeyboard* pInputKeyBoard, CCamera* pCamera);

private:
	void UpdateIdxPlayer(int nIdx, CInputKeyboard* pInputKey);	// 番号ごとの更新関数

	int m_StateCount;		// 状態管理カウンター
	int m_nIdxTexture;		// テクスチャID

	D3DXVECTOR3 m_move;		// 移動量
	PLAYERSTATE m_State;	// 状態管理
	D3DXVECTOR3 m_rotDest;  // 目的角

	D3DXVECTOR3 m_pos;		// 階層構造やるため用
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxworld;	// マトリックス

	CModel* m_apModel[MAX_MODEL]; // 使うモデルのポインタ
	int m_nNumAll;		   // モデル総数
	CMotion* m_pMotion;		// モーションのポインタ
	int m_type;				// モーションの種類変数
	bool m_isLanding;		// 着地判定
	bool m_isJump;			// ジャンプ判定
	bool m_isMoving;
	float m_size;
	static int m_nIdxPlayer; // プレイヤーの識別番号
	const char* m_pFilename;

	CShadow* m_pShadow;
	CState* m_pState;   // 状態管理クラスのポインタ

	bool m_isAttack;
};

#endif