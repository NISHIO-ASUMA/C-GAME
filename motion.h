//====================================
//
// モーション処理 [ motion.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _MOTION_H_ // このマクロ定義がされてなかったら
#define _MOTION_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "model.h"
#include <vector>

//*****************************
// モーションクラスを定義
//*****************************
class CMotion
{
public:

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
		TYPE_JUMPATTACK,	// SP攻撃
		TYPE_DAMAGE,		// 吹き飛ばし
		TYPE_WAKEUP,		// 起き上がり
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
		std::vector<KEY> aKey; // 動的なキー数
	};

	//***************************
	// モーション情報の構造体宣言
	//***************************
	struct INFO
	{
		bool bLoop;						// ループするかしないか
		int nNumKey;					// キーの総数
		std::vector<KEY_INFO> aKeyInfo; // モーションの動的キーフレーム
		TYPE Motiontype;				// モーションの種類
	};

	CMotion();
	~CMotion();

	static CMotion* Load(const char* pFilename, const int nMaxParts,CModel ** pModel,int nDestMotions);
	void SetMotion(int motiontype);
	void Update(CModel** ppModel, const int nMaxPart);

	void UpdateCurrentMotion(CModel** ppModel, int nModelCount);
	void UpdateBlend(CModel** ppModel, int nModelCount);
	void SetMotionNum(int nMotion) { m_nNumMotion = nMotion; }

	void SetModels(std::istringstream& iss, int& nModel, int nMaxParts);
	void SetModelFile(std::istringstream& iss, CModel** pModel, int nCnt);
	void SetParts(std::ifstream& file, CModel** pModel);
	void SetPartsMotion(std::ifstream& file, CMotion* pMotion, int nCntMotion);
	void SetKey(std::ifstream& file, CMotion* pMotion, int nCntMotion, int nCntKey);
	void SetKeyDate(std::istringstream& ss, const std::string& param, CMotion* pMotion, int nCntMotion, int nCntKey, int& posKeyIndex, int& rotKeyIndex);

	int GetMotionType(void) { return m_motiontype; }
	bool GetFinishMotion(void) { return m_isFinishMotion; }

private: 
	std::vector<INFO> m_aMotionInfo; // モーション情報を動的確保

	int m_nNumMotion;					// モーションの総数
	bool m_isLoopMotion;				// ループするかどうか
	int m_nNumKey;						// キーの総数
	int m_nKey;							// 現在のキーNo
	int m_nCounterMotion;				// モーションのカウンター
	int m_nNextKey;						// 次のキー
	int m_motiontype;					// モーションタイプ
	int m_type;
//----------------------------------------------------------------------
	bool m_isBlendMotion;				// ブレンドがあるかどうか
	bool m_isFinishMotion;				// モーションが終わったかどうか
	bool m_isFirstMotion;				// モーションが始まったフラグ

	int m_motiontypeBlend;				// ブレンドの種類
	int m_nFrameBlend;					// ブレンドのフレーム数
	int m_nCounterBlend;				// ブレンドカウンター
	int m_nKeyBlend;					// ブレンドモーションの現在のキー
	int m_nNextKeyBlend;				// ブレンドモーションの次のキー
//----------------------------------------------------------------------

	int m_nNumModels;
};

#endif

