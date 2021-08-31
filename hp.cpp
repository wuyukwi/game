#include "hp.h"

LPDIRECT3DTEXTURE9 g_pTextureHp = NULL;	    //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHp = NULL;	//���_�o�b�t�@�ւ̃|�C���^

void InitHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D*pVtx;					//���_���ւ̃|�C���^
	Player*pPlayer;
	
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	pPlayer = GetPlayer();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hp.png",
		&g_pTextureHp);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHp,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(300.0f, 650.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(50.0f, 670.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(300.0f, 670.0f, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHp->Unlock();
}

void UninitHp(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureHp != NULL)
	{
		g_pTextureHp->Release();
		g_pTextureHp = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHp != NULL)
	{
		g_pVtxBuffHp->Release();
		g_pVtxBuffHp = NULL;
	}
}

void UpdateHp(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D*pVtx;					//���_���ւ̃|�C���^
	Player*pPlayer;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	pPlayer = GetPlayer();



	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->nLife == 100)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
		pVtx[0].tex = D3DXVECTOR2(0.45f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
		pVtx[2].tex = D3DXVECTOR2(0.45f, 1.0f);
	}
	else if (pPlayer->nLife > 0)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f - (0.005f * (100 + pPlayer->nLife)), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.51f - (0.005f * (100+ pPlayer->nLife)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f - (0.005f * (100 + pPlayer->nLife)), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.51f - (0.005f * (100 + pPlayer->nLife)), 1.0f);
	}
																   
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHp->Unlock();
}

void DrawHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHp, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureHp);


	//�|���S���̕`�� �l�p
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,			//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��
}