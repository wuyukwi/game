#include "main.h"
#include"Score.h"

LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;	    //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	//���_�o�b�t�@�ւ̃|�C���^
int	g_nScore;				//�X�R�A�̒l
D3DXVECTOR3 g_posScore;		//�ʒu
float nXnumTex = 35.0f;
float nYnumTex = 70.0f;

void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D*pVtx;					//���_���ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number000.png",
		&g_pTextureScore);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_KETA,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	int nCntScore; 

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_KETA; nCntScore++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[0].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);

		g_posScore = D3DXVECTOR3(1235.0f, 20.0f, 0.0f);					//�ʒu������������
		g_nScore = 0;		//�X�R�A�̏�����

		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

void UpdateScore(void)
{
	SetScore(g_nScore);

}

void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̕`��
	for (nCntScore = 0; nCntScore < MAX_KETA; nCntScore++)
	{

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);

		//�|���S���̕`�� �l�p
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntScore * 4,							//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}
}

void SetScore(int nScore)
{
	int aPosTexU[MAX_KETA] = {0};		//�e���̐������i�[
	int nCntScore=0;

	while (nScore !=0)
	{
		aPosTexU[nCntScore] = nScore % 10;
		nScore /= 10;
		nCntScore++;
	}

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_KETA; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - nCntScore* nXnumTex, g_posScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + nXnumTex - nCntScore* nXnumTex, g_posScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - nCntScore* nXnumTex, g_posScore.y + nYnumTex, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + nXnumTex - nCntScore* nXnumTex, g_posScore.y + nYnumTex, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntScore] *0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aPosTexU[nCntScore] *0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntScore] *0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aPosTexU[nCntScore] *0.1f + 0.1f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffScore->Unlock();

		pVtx += 4;
	}
		
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
	/*itoa(g_nScore, aPosTexU, 1);*/
}

void AddScore(int nValue)
{
	g_nScore += nValue;
}