//====================================
//
// �e�N�X�`������ [ texture.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TEXTURE_H_ // ���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************


//**********************
// �}�N����`
//**********************
#define MAX_TEXTURE (128)	// �ő�e�N�X�`������

//*****************************
// �e�N�X�`���Ǘ��N���X���`
//*****************************
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT Load(void);
	void UnLoad(void);
	int Register(const char* pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:
	// �e�N�X�`���ǂݍ��ݔz����쐬
	const char* TexName[MAX_TEXTURE] =
	{
		"data\\TEXTURE\\gold.jpg",
		"data\\TEXTURE\\explosion000.png",
		"data\\TEXTURE\\effect000.jpg",
		"data\\TEXTURE\\bullet002.png",
		"data\\TEXTURE\\num001.png",
		"data\\TEXTURE\\number000.png",
		"data\\TEXTURE\\field100.jpg",
		"data\\TEXTURE\\trash.png",
		"data\\TEXTURE\\billboard_wepon.png",
		"data\\TEXTURE\\field000.jpeg",
		"data\\TEXTURE\\bg.jpeg",
		"data\\TEXTURE\\DomeTex.jpg",
		"data\\TEXTURE\\shadow000.jpg",
	};

	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];	// �e�N�X�`���|�C���^
	static int m_nNumAll;							// �e�N�X�`���J�E���g�ϐ�
};

#endif