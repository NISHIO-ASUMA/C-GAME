//====================================
//
// �^�C������ [ time.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TIME_H_ // ���̃}�N����`������ĂȂ�������
#define _TIME_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "number.h"
#include "object.h"

//**********************************
// �^�C���N���X���`
//**********************************
class CTime : public CObject
{
public:
	CTime(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CTime();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTime* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXCOLOR GetCol(void) { return m_col; }

	int GetAllTime(void) { return m_nAllTime; }
	void SetTexture(void);

private:
	static inline constexpr int DIGIT_TIME = 3;	// ����

	D3DXVECTOR3 m_pos;		// ���W
	D3DXCOLOR m_col;
	D3DXVECTOR3 m_rot;

	int m_nCurrentTime;		// �o�ߎ���
	int m_nAllTime;			// �ő厞��

	CNumber* m_pNumber[DIGIT_TIME];	// �i���o�[�^�̐ÓI�ϐ�

	float m_fHeight, m_fWidth;				// ����,����
	int m_nIdxTexture;
};
#endif