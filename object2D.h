//====================================
//
// 2Dオブジェクト処理 [ object2D.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _OBJECT2D_H_ // このマクロ定義がされてなかったら
#define _OBJECT2D_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object.h"

//****************************
// オブジェクト2Dクラスを定義
//****************************
class CObject2D : public CObject
{
public:
	CObject2D(int nPriority = 3);
	~CObject2D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CObject2D* Create(void); // 生成処理

	void SetUV(float TexU, float TexV);
	void SetCol(D3DXCOLOR col) { m_col = col; }
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetSize(float fWidth, float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }

	D3DXVECTOR3 GetPos(void) { return m_Pos; } // 現在の座標を取得
	D3DXVECTOR3 GetRot(void) { return m_Rot; } // 現在の角度を取得
	D3DXCOLOR GetCol(void) { return m_col; } // 現在の色を取得

private:
	LPDIRECT3DTEXTURE9 m_pTexture;      // テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点バッファのポインタ

	D3DXVECTOR3 m_Rot;	// 角度情報
	D3DXVECTOR3 m_Pos;	// 座標情報
	D3DXCOLOR m_col;	// 色情報

	float m_fWidth;	// 横幅
	float m_fHeight;// 高さ
};

#endif