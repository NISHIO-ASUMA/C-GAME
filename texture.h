//====================================
//
// �e�N�X�`������ [ texture.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TEXTURE_H_ // ���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

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

	static inline constexpr int NUM_TEXTURE = 128;	// �ő�e�N�X�`����

private:
	// �e�N�X�`���ǂݍ��ݔz����쐬
	const char* TexName[NUM_TEXTURE] =
	{
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
		"data\\TEXTURE\\lifegage.png",
		"data\\TEXTURE\\lifeframe.png",
		"data\\TEXTURE\\score001.png",
		"data\\TEXTURE\\pause_retry.png",
		"data\\TEXTURE\\pause_continue.png",
		"data\\TEXTURE\\pause_quit.png",
		"data\\TEXTURE\\PauseBack.png",
		"data\\TEXTURE\\title000.png",
		"data\\TEXTURE\\title001.png",
		"data\\TEXTURE\\tutorial_menu.png",
	};

	LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEXTURE];	// �e�N�X�`���|�C���^
	static int m_nNumAll;							// �e�N�X�`���J�E���g�ϐ�
};

#endif