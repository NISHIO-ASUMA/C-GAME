//====================================
//
// ステンシルシャドウ処理 [ shadowS.h ]
// Author: Asuma Nishio
//
//=====================================
#ifndef _SHADOWS_H_ // このマクロ定義がされてなかったら
#define _SHADOWS_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "objectX.h"
#include "object.h"
#include "object2D.h"

//**********************************
// シャドウステンシルクラスを定義
//**********************************
class CShadowS :public CObjectX
{
public:
	CShadowS(int nPriority = static_cast<int>(CObject::PRIORITY::SHADOW));
	~CShadowS();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadowS* Create(const char * pFileName,D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;

};

#endif
