//====================================
//
// Xファイル処理 [ objectX.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _OBJECTX_H_ // このマクロ定義がされてなかったら
#define _OBJECTX_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object.h"

//******************************
// オブジェクトXクラスを定義
//******************************
class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = static_cast<int>(CObject::PRIORITY::BASENUMBER));
	~CObjectX();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CObjectX* Create(const char* pFileName,D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	
	void SetFilePass(const char* pFilePass) { m_pFileName = pFilePass; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

	void SetUseQuat(bool isFlags) { m_isUseQaut = isFlags; }

private:
	D3DXMATRIX m_mtxWorld; // ワールドマトリックス
	LPD3DXMESH m_pMesh;	   // メッシュ情報
	LPD3DXBUFFER m_pBuffMat; // マテリアル情報
	DWORD m_dwNumMat;		// マテリアル数

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度
	int m_nIdxTexture;		// テクスチャインデックス
	int* m_pTexture;		// テクスチャポインタ

	const char* m_pFileName; // ファイル名

	bool m_isUseQaut;		 // クォータニオンかどうか

	D3DXMATRIX m_mtxRot;		// 回転マトリックス
	D3DXQUATERNION m_quat;	// クォータニオン
	D3DXVECTOR3 m_VecAxis;	// 回転軸
	float fValueRot;		// 回転角度


	D3DXVECTOR3 m_posOld;   // 前フレームの位置
	float m_theta;          // 現在の角度
	float m_radius;         // 円の半径

};

#endif