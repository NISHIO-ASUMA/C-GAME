//====================================
//
// モデル処理 [ model.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _MODEL_H_ // このマクロ定義がされてなかったら
#define _MODEL_H_ // 2重インクルード防止のマクロ定義

//**********************
// マクロ定義
//**********************
#define MAX_PART (25)

//**********************
// モデルクラスの定義
//**********************
class CModel
{
public:
	CModel();
	~CModel();

	static CModel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParent(CModel* pModel);
	D3DXMATRIX GetMtxWorld(void) { return m_mtxworld; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void OffSetPos(D3DXVECTOR3 offpos) { m_offPos = offpos; }
	void OffSetRot(D3DXVECTOR3 offrot) { m_offRot = offrot; }

private:
	//***************************
	// モーション列挙型宣言
	//***************************
	enum TYPE
	{
		TYPE_NEUTRAL = 0,	// ニュートラル
		TYPE_MOVE,			// 移動
		TYPE_ACTION,		// アクション
		TYPE_JUMP,			// ジャンプ
		TYPE_LANDING,		// 着地
		TYPE_MAX
	};

	//***************************
	// キー構造体宣言
	//***************************
	struct KEY
	{
		float fPosX;	// 位置X
		float fPosY;	// 位置Y
		float fPosZ;	// 位置Z
		float fRotX;	// 角度X
		float fRotY;	// 角度Y
		float fRotZ;	// 角度Z
	};

	//***************************
	// キー情報の構造体宣言
	//***************************
	struct KEY_INFO
	{
		int nFrame;				// フレーム数
		KEY aKey[MAX_PART]; 	// 各パーツのキーの要素
	};

	//***************************
	// モーション情報の構造体宣言
	//***************************
	struct MOTION_INFO
	{
		bool bLoop;						// ループするかしないか
		int nNumKey;					// キーの総数
		KEY_INFO aKeyInfo[MAX_PART];	// キー情報
		TYPE Motiontype;				// モーションの種類
	};

	MOTION_INFO m_aMotionInfo[TYPE_MAX];// モーション情報
	int m_nNumMotion;					// モーションの総数
	bool m_isLoopMotion;				// ループするかどうか
	int m_nNumKey;						// キーの総数
	int m_nKey;							// 現在のキーNo
	int m_nCounterMotion;				// モーションのカウンター
	TYPE m_type;

	D3DXVECTOR3 m_pos, m_rot,m_offPos,m_offRot; // 座標,角度

	D3DXMATRIX m_mtxworld;	  // ワールドマトリックス
	CModel* m_pParent;	  // 親モデル

	LPD3DXMESH m_pMesh;	   // メッシュ情報
	LPD3DXBUFFER m_pBuffMat; // マテリアル情報
	DWORD m_dwNumMat;		// マテリアル数
	int* m_pTexture;		// テクスチャポインタ
};

#endif