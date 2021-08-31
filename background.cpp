#include "main.h"
#include "background.h"
#include "input.h"

#define MAX_BACKGROUND 3		//�w�i�̖���

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9			g_pTexturebackground[MAX_BACKGROUND] = { NULL, NULL, NULL};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffbackground = NULL;									//���_�o�b�t�@�̃|�C���^
D3DXVECTOR3					g_rotBackround;		    //����
D3DXVECTOR3					g_moveBackground;		//�ړ���
//�w�i�̏���������
void InitBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	int nCntbackground;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\bg01.jpg",
		&g_pTexturebackground[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_pTexturebackground[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\bg102.png",
		&g_pTexturebackground[2]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BACKGROUND,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffbackground,
		NULL);

	g_moveBackground = D3DXVECTOR3(0.0f, 0.01f, 0.0f);			//�ړ��ʂ�������
	g_rotBackround = D3DXVECTOR3(0.0f, 0.01f, 0.0f);			//������������

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffbackground->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntbackground = 0; nCntbackground < MAX_BACKGROUND; nCntbackground++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(GAME_WIDTH, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GAME_WIDTH*3.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GAME_WIDTH, GAME_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GAME_WIDTH*3.0f, GAME_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffbackground->Unlock();
}

//-------------------------
//�w�i�̏I������
//-------------------------
void UninitBackground(void)
{
	if (g_pTexturebackground[0] != NULL)
	{//�e�N�X�`���̔j��
		g_pTexturebackground[0]->Release();
		g_pTexturebackground[0] = NULL;
	}

	if (g_pTexturebackground[1] != NULL)
	{//�e�N�X�`���̔j��
		g_pTexturebackground[1]->Release();
		g_pTexturebackground[1] = NULL;
	}

	if (g_pTexturebackground[2] != NULL)
	{//�e�N�X�`���̔j��
		g_pTexturebackground[2]->Release();
		g_pTexturebackground[2] = NULL;
	}

	if (g_pVtxBuffbackground != NULL)
	{//���_�o�b�t�@�̔j��
		g_pVtxBuffbackground->Release();
		g_pVtxBuffbackground = NULL;
	}
}

//-------------------------
//�w�i�̍X�V����
//-------------------------
void UpdateBackground(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	int nCntbackground;
	float MoveSpeed = 0.0007f;		//�ړ��X�s�[�h
	float RotSpeed = 0.005f;	//��]�X�s�[�h
	float DownSpeed = 0.07f;	//�X�s�[�h�����W��

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffbackground->Lock(0, 0, (void**)&pVtx, 0);

	
	//W�L�[�������ꂽ
	if (GetKeyboardPress(DIK_W) == true)
	{
	
		//W�L�[�������ꂽ
			g_moveBackground.x += sinf(0.0f + g_rotBackround.z) * MoveSpeed;
			g_moveBackground.y += cosf(0.0f + g_rotBackround.z) * MoveSpeed;
			//�ړ��ʂ��X�V(����������)
		
	}


	//S�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_S) == true)
	{		
		g_moveBackground.x += sinf(0.0f + g_rotBackround.z) * MoveSpeed;
		g_moveBackground.y += cosf(0) * -MoveSpeed / 3;
		//�ړ��ʂ��X�V(����������)
	}

	//�ړ��ʂ��X�V(����������)
	g_moveBackground.x += (0.0f - g_moveBackground.x) * DownSpeed;
	g_moveBackground.y += (0.005f - g_moveBackground.y) * DownSpeed;


	//���_���W�̍X�V
	for (nCntbackground = 0; nCntbackground < MAX_BACKGROUND; nCntbackground++)
	{
		pVtx[0].tex.x -= g_moveBackground.x / (nCntbackground + 1);
		pVtx[0].tex.y -= g_moveBackground.y / (nCntbackground + 1);

		pVtx[1].tex.x -= g_moveBackground.x / (nCntbackground + 1);
		pVtx[1].tex.y -= g_moveBackground.y / (nCntbackground + 1);

		pVtx[2].tex.x -= g_moveBackground.x / (nCntbackground + 1);
		pVtx[2].tex.y -= g_moveBackground.y / (nCntbackground + 1);

		pVtx[3].tex.x -= g_moveBackground.x / (nCntbackground + 1);
		pVtx[3].tex.y -= g_moveBackground.y / (nCntbackground + 1);


		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffbackground->Unlock();
}

//-------------------------
//�w�i�̕`�揈��
//-------------------------
void DrawBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntbackground;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffbackground, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̕`��
	for (nCntbackground = 0; nCntbackground < MAX_BACKGROUND; nCntbackground++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturebackground[nCntbackground]);

		//�|���S���̕`�� �l�p
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntbackground * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}
}