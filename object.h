//====================================
//
// オブジェクト処理 [ object.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _OBJECT_H_ // このマクロ定義がされてなかったら
#define _OBJECT_H_ // 2重インクルード防止のマクロ定義

//**********************
// マクロ定義
//**********************
#define MAX_OBJECT (512)		// 最大オブジェクト数

//***************************
// オブジェクトクラスを定義
//***************************
class CObject
{
public:
	//*************************
	// オブジェクトの種類定義
	//*************************
	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BULLET,
		TYPE_BOSS,
		TYPE_BLOCK,
		TYPE_MAX
	};

	//*************************
	// 描画番号定義
	//*************************
	enum PRIORITY
	{
		NONE, // 初期状態
		MESH, // メッシュ
		BLOCK, // ブロック
		BULLET,// 弾
		BOSS, // ボス
		SHADOW,//影
		IMPACT, // 衝撃波
		PLAYER, // プレイヤー
		UI,  // 2DUI関係
		PRIORITY_MAX // 最大数
	};

	CObject(int nPriority = NONE);
	virtual ~CObject();

	// 純粋仮想関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	void SetObjType(TYPE type);
	TYPE GetObjType(void);

	static CObject* GetObject(int nPriority,int nIdx);

	static int GetNumAll(void) { return m_nNumAll; }

protected:
	void Release(void);

private:
	static CObject* m_apObject[PRIORITY_MAX][MAX_OBJECT]; // オブジェクトのポインタ
	static int m_nNumAll;		// 総数管理
	int m_nID;					// 自分自身のID
	TYPE m_Type;				// オブジェクトの種類変数
	int m_nPriority;			// 優先順位番号
};
#endif