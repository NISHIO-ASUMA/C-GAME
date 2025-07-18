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
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

//*****************************
// モーションクラスを定義
//*****************************
class CMotion
{
public:

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
	};

	CMotion();
	~CMotion();

	static CMotion* Load(const char* pFilename, const int nMaxParts,CModel ** pModel,int nDestMotions);

	// 更新関数
	void Update(CModel** ppModel, const int nMaxPart);
	void UpdateCurrentMotion(CModel** ppModel, int nModelCount);
	void UpdateBlend(CModel** ppModel, int nModelCount);

	void Debug(void);

	// セッター
	void SetMotion(int motiontype);
	void SetMotionNum(int nMotion) { m_nNumMotion = nMotion; }
	void SetModels(std::istringstream& iss, int& nModel, int nMaxParts);
	void SetModelFile(std::istringstream& iss, CModel** pModel, int nCnt);
	void SetParts(std::ifstream& file, CModel** pModel);
	void SetPartsMotion(std::ifstream& file, CMotion* pMotion, int nCntMotion);
	void SetKey(std::ifstream& file, CMotion* pMotion, int nCntMotion, int nCntKey);
	void SetKeyDate(std::istringstream& ss, const std::string& param, CMotion* pMotion, int nCntMotion, int nCntKey, int& posKeyIndex, int& rotKeyIndex);

	// ゲッター
	int GetMotionType(void) { return m_motiontype; }
	bool GetFinishMotion(void) { return m_isFinishMotion; }

	bool CheckFrame(int nStartMotion, int nEndMotion, int nMotionType);

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
	int m_nNumAllFrame;   // 全体フレーム数
	int m_nAllFrameCount;
	bool m_isStopAction;
};

#endif

