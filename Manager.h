//====================================
//
// マネージャー処理 [ Manager.h ]
// Author: Asuma Nishio
//
//=====================================

//**************************
// 二重インクルードガード
//**************************
#ifndef _MANAGER_H_ 
#define _MANAGER_H_ 

//**************************  
// インクルードファイル宣言
//**************************
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "bullet.h"
#include "enemymanager.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "object3D.h"
#include "blockManager.h"
#include "collision.h"
#include "meshcylinder.h"
#include "boss.h"

//**************************
// マネージャークラス宣言
//**************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CObject3D* GetObj3D(void);
	static CRenderer* GetRenderer(void);
	static CInputKeyboard* GetInputKeyboard(void);
	static CJoyPad* GetJoyPad(void);
	static CSound* GetSound(void);
	static CInputMouse* GetMouse(void);
	static CEnemymanager* GetEnemyManager(void);
	static CTexture* GetTexture(void);
	static CCamera* GetCamera(void);
	static CLight* GetLight(void);
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CBlockManager* GetBlockManager(void) {return m_pBlockManager;}
	static CBlock* GetBlock(void) { return m_pBlock; }
	static CCollision* GetCollision(void) { return m_pCollision; }
	static CMeshCylinder* GetCylinder(void) {return m_pMeshCylinder;}
	static CBoss* GetBoss(void) { return m_pBoss; }
private:
	static CRenderer* m_pRenderer;			 // レンダラークラスのポインタ
	static CInputKeyboard* m_pInputKeyboard; // キーボードクラスのポインタ
	static CJoyPad* m_pJoyPad;				 // ジョイパッドクラスのポインタ
	static CSound* m_pSound;				 // サウンドオブジェクトのポインタ
	static CInputMouse* m_pInputMouse;		 // マウスクラスのポインタ
	static CEnemymanager* m_pEnemyManager;	 // 敵管理クラス
	static CTexture* m_pTexture;			 // テクスチャクラス
	static CCamera* m_pCamera;				 // カメラクラス
	static CLight* m_pLight;				 // ライトクラス
	static CPlayer* m_pPlayer;				 // プレイヤークラス
	static CObject3D* m_pobj;
	static CBlockManager* m_pBlockManager;
	static CBlock* m_pBlock;
	static CCollision* m_pCollision;
	static CMeshCylinder* m_pMeshCylinder;		// シリンダー
	static CBoss* m_pBoss;
};
#endif