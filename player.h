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
#include "model.h"
#include "motion.h"

//**********************
// 前方宣言
//**********************
class CShadow;
class CInputKeyboard;
class CCamera;
class CState;
class CParameter;

//*************************
// プレイヤークラスを定義
//*************************
class CPlayer : public CObject
{
public:
	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::PLAYER));
	~CPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, const int nIdxParson,const char * pFilename); // 生成処理

	D3DXVECTOR3 GetPos(void) { return m_pos; } // 現在の座標を取得
	D3DXVECTOR3 GetRot(void) { return m_rot; } // 現在の角度を取得
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }

	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }
	void MoveKey(CInputKeyboard* pInputKeyBoard, CCamera* pCamera);

	static constexpr int MAX_MODEL = 19; // プレイヤーで使うモデルの数

private:
	void UpdateIdxPlayer(int nIdx, CInputKeyboard* pInputKey);	// 番号ごとの更新関数

	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rotDest;  // 目的角

	D3DXVECTOR3 m_pos;		// 階層構造設定座標
	D3DXVECTOR3 m_posOld;	// 過去の座標情報
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxworld;	// ワールドマトリックス

	CModel* m_apModel[MAX_MODEL]; // 使うモデルのポインタ
	CMotion* m_pMotion;		// モーションのポインタ
	CShadow* m_pShadow;		// 影クラスのポインタ変数
	CState* m_pState;		// 状態管理クラスのポインタ
	CParameter* m_pParameter; // パラメータークラスポインタ

	int m_type;				// モーションの種類変数
	int m_nNumAll;		    // モデル総数
	int m_StateCount;		// 状態管理カウンター
	int m_nIdxTexture;		// テクスチャID

	bool m_isLanding;		// 着地判定
	bool m_isJump;			// ジャンプ判定
	bool m_isMoving;
	bool m_isAttack;

	float m_size;

	static int m_nIdxPlayer; // プレイヤーの識別番号
	const char* m_pFilename; // 読み込むファイル名
};

#endif