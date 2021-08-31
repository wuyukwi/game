#include"main.h"
#include"Player.h"
#include"input.h"
#include"bullet.h"
#include"enemy.h"

//-------------------------
//�}�N����`
//-------------------------

//-------------------------
//�O���[�o���ϐ�
//-------------------------
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;		//�e�N�X�`���ւ̃|�C���^
VERTEX_2D g_aVertex[4];			//���_�����i�[
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		//���_�o�b�t�@�̃|�C���^
Player g_Player1;

float Xplayer = 18.5f;		//�v���C���[���̔���
float Yplayer = 23.5f;		//�v���C���[�����̔���	
float hX = 37.0f;		//�e�N�X�`�����̔���
float nY = 47.0f;		//�e�N�X�`�������̔���

int g_nCounterAnimPlayer;		//�A�j���[�V�����J�E���^�[
int g_nPatternAnimPlayer;		//�A�j���[�V�����p�^�[��No.

D3DXVECTOR3 g_posPlayer;		//�ʒu
D3DXVECTOR3 g_movePlayer;		//�ړ���
D3DXVECTOR3 g_rotPlayer;		//����
D3DXVECTOR3 g_moveBullet;		//�ړ���

float g_fLengthPlayer;			//�Ίp���̒���
float g_fAnglePlayer;			//�Ίp���̊p�x
float g_fmoveLength;			//�Ίp���̈ړ���



//�v���C���[�̏���������
void InitPlayer(void)
{
	float x = 650.0f;		    //�ʒuX���W
	float y = 600.0f;		    //�ʒuy���W
	g_Player1.nLife = 100;		//�v���C���[�̗̑͂�����������
	g_Player1.bUse = true;		//�v���C���[���g�p����Ă����Ԃɂ���

	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\hero.png",
		&g_pTexturePlayer);


	g_nCounterAnimPlayer = 0;								//�J�E���^�[������������
	g_nPatternAnimPlayer = 0;								//�p�^�[��No.������������
	g_posPlayer = D3DXVECTOR3(x, y, 0.0f);					//�ʒu������������
	g_movePlayer = D3DXVECTOR3(0.0f,0.0f,0.0f);				//�ړ��ʂ�����������
	g_moveBullet = D3DXVECTOR3(0.0f, -10.0f, 0.0f);				//�ړ��ʂ�����������
	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
	g_fmoveLength = 0.0f;									//�Ίp���̈ړ��ʂ�������
	
	//�Ίp���̒������Z�o����
	g_fLengthPlayer = sqrtf((Xplayer*2) * (Xplayer * 2) + (Yplayer * 2) * (Yplayer * 2)) / 2.0f;

	//�Ίp���̊p�x���Z�o����
	g_fAnglePlayer = atan2f((Xplayer * 2), (Yplayer * 2));

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(g_aVertex),	//�K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	
	pVtx[0].pos = D3DXVECTOR3(x - Xplayer, y - Yplayer, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(x + Xplayer , y - Yplayer, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(x - Xplayer, y + Yplayer, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(x + Xplayer, y + Yplayer, 0.0f);
	
	

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

	
	
	//�e�N�X�`�����W�̐ݒ�	   u     v
	pVtx[0].tex = D3DXVECTOR2(0.4f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.4f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);

	


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//�v���C���[�̏I������
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//�v���C���[�̍X�V����
void UpdatePlayer(void)
{
	int MaxPattern = 10;
	float MoveSpeed = 0.5f;		//�ړ��X�s�[�h
	float RotSpeed = 0.005f;	//��]�X�s�[�h
	float DownSpeed = 0.07f;	//�X�s�[�h�����W��

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//A�L�[�������ꂽ
	if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ
			g_movePlayer.x += sinf(-D3DX_PI * 0.75f + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(-D3DX_PI * 0.75f + g_rotPlayer.z) * MoveSpeed;
			//�ړ��ʂ��X�V(����������)
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ
			g_movePlayer.x += sinf(-D3DX_PI * 0.25f + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(-D3DX_PI * 0.25f + g_rotPlayer.z) * MoveSpeed;
			//�ړ��ʂ��X�V(����������)
		}
		else
		{//A�L�[�������ꂽ
		 //�ړ��ʂ��X�V(����������)
			g_movePlayer.x += sinf(-D3DX_PI / 2 + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(-D3DX_PI / 2 + g_rotPlayer.z) * MoveSpeed;
			
			//�e�N�X�`�����W�̐ݒ�	   u     v
			pVtx[0].tex = D3DXVECTOR2(0.2f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.4f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.2f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);

		}
	}
	
	//W�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[�������ꂽ
			g_movePlayer.x += sinf(D3DX_PI * 0.75f + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(D3DX_PI * 0.75f + g_rotPlayer.z) * MoveSpeed;
			//�ړ��ʂ��X�V(����������)

		}
		else
		{//W�L�[�������ꂽ
			g_movePlayer.x += sinf(D3DX_PI + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(D3DX_PI + g_rotPlayer.z) * MoveSpeed;
			//�ړ��ʂ��X�V(����������)
			//�e�N�X�`�����W�̐ݒ�	   u     v
			pVtx[0].tex = D3DXVECTOR2(0.4f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.4f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
		}
	}

	//D�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ
			g_movePlayer.x += sinf(D3DX_PI * 0.25f + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(D3DX_PI * 0.25f + g_rotPlayer.z) * MoveSpeed;
			//�ړ��ʂ��X�V(����������)
		}
		else
		{//D�L�[�������ꂽ
			g_movePlayer.x += sinf(D3DX_PI / 2 + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(D3DX_PI / 2 + g_rotPlayer.z) * MoveSpeed;
			//�ړ��ʂ��X�V(����������)
			//�e�N�X�`�����W�̐ݒ�	   u     v
			pVtx[0].tex = D3DXVECTOR2(0.6f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.8f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.6f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.8f, 1.0f);
		}
	}

	//S�L�[�������ꂽ
	else if (GetKeyboardPress(DIK_S) == true)
	{		
		g_movePlayer.x += sinf(0.0f + g_rotPlayer.z) * MoveSpeed;
		g_movePlayer.y += cosf(0.0f + g_rotPlayer.z) * MoveSpeed;
		//�ړ��ʂ��X�V(����������)
		//�e�N�X�`�����W�̐ݒ�	   u     v
		pVtx[0].tex = D3DXVECTOR2(0.4f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.4f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
	}

	//SPACE�L�[�������ꂽ
	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		//�e�̐ݒ�
			/*SetBullet(g_posPlayer, g_moveBullet);
			SetBullet(g_posPlayer, D3DXVECTOR3(-10.0f, -10.0f, 0.0f) );
			SetBullet(g_posPlayer, D3DXVECTOR3(10.0f, -10.0f, 0.0f) );*/
			SetBullet(g_posPlayer, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 8.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * 8.0f, cosf(D3DX_PI * 0.75f) * 8.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * 8.0f, cosf(-D3DX_PI * 0.75f) * 8.0f, 0.0f));
			/*SetBullet(g_posPlayer, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 10.0f, 0.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 10.0f, 0.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(0.0f, cosf(0.0f) * 10.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * 10.0f, cosf(-D3DX_PI * 0.25f) * 10.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 10.0f, cosf(D3DX_PI * 0.25f) * 10.0f, 0.0f));*/
	}


	//�ʒu���X�V
	g_posPlayer.x += g_movePlayer.x;
	g_posPlayer.y += g_movePlayer.y;

	//�ړ��ʂ��X�V(����������)
	g_movePlayer.x += (0.0f - g_movePlayer.x) * DownSpeed;
	g_movePlayer.y += (0.0f - g_movePlayer.y) * DownSpeed;

	if (g_posPlayer.x <= GAME_WIDTH + Xplayer)
	{//�n�_(Y���W)����ʂ̍��[�ɓ�������
		g_posPlayer.x = GAME_WIDTH + Xplayer;					//�n�_����ʂ̍��[�ɐݒ肷
	}
	if (g_posPlayer.y <= nY)
	{//�n�_(Y���W)����ʂ̏�[�ɓ�������
		g_posPlayer.y = nY;					//�n�_����ʂ̏�[�ɐݒ肷
	}
	if (g_posPlayer.y >= GAME_HEIGHT - nY)
	{//�n�_(Y���W)����ʂ̉��[�ɓ�������
		g_posPlayer.y = GAME_HEIGHT - nY;					//�n�_����ʂ̉��[�ɐݒ肷
	}
	if (g_posPlayer.x >= GAME_WIDTH * 3 - hX)
	{//�n�_(Y���W)����ʂ̉E�[�ɓ�������
		g_posPlayer.x = GAME_WIDTH * 3 - hX;					//�n�_����ʂ̉E�[�ɐݒ肷
	}


	////���_���W�̍X�V
	//pVtx[0].pos.x = g_posPlayer.x - hX;
	//pVtx[0].pos.y = g_posPlayer.y - nY;

	//pVtx[1].pos.x = g_posPlayer.x + hX;
	//pVtx[1].pos.y = g_posPlayer.y - nY;

	//pVtx[2].pos.x = g_posPlayer.x - hX;
	//pVtx[2].pos.y = g_posPlayer.y + nY;

	//pVtx[3].pos.x = g_posPlayer.x + hX;
	//pVtx[3].pos.y = g_posPlayer.y + nY;
	
	if (GetKeyboardPress(DIK_LEFT) == true)
	{//���L�[�������ꂽ
	 //�ړ��ʂ��X�V (����������)
		g_movePlayer.z += tanf(-D3DX_PI * g_fAnglePlayer) * RotSpeed;
	}

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{//���L�[�������ꂽ
		g_movePlayer.z += tanf(D3DX_PI * g_fAnglePlayer) * RotSpeed;
	}

	//�������X�V
	g_rotPlayer.z += g_movePlayer.z;

	//�ړ��ʂ��X�V (����������)
	g_movePlayer.z += (0.0f - g_movePlayer.z) * DownSpeed;

	if (GetKeyboardPress(DIK_UP) == true)
	{//���L�[�������ꂽ
		if (g_fLengthPlayer <= 100)
		{
			g_fmoveLength += tanf(-D3DX_PI * g_fAnglePlayer) * 0.05f;
		}
	}

	if (GetKeyboardPress(DIK_DOWN) == true)
	{//���L�[�������ꂽ
		if (g_fLengthPlayer >= 50)
		{
			g_fmoveLength += tanf(D3DX_PI * g_fAnglePlayer) * 0.05f;
		}
	}
	//�g��E�k�����X�V
	g_fLengthPlayer += g_fmoveLength;

	//�����ʂ��X�V (����������)
	g_fmoveLength += (0.0f - g_fmoveLength) * DownSpeed;
	
	//���_���W�̍X�V
	pVtx[0].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;



	if (g_Player1.bUse == true)
	{//�e���g�p����Ă���
		Enemy *pEnemy;		//�G�̏��ւ̃|�C���^
		int nCountEnemy;

		//�G�̎擾
		pEnemy = GetEnemy();

		for (nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{//�G���g�p����Ă���
				if (g_posPlayer.x - Xplayer >= pEnemy->pos.x - pEnemy->Xenemy&&
					g_posPlayer.x + Xplayer <= pEnemy->pos.x + pEnemy->Xenemy&&
					g_posPlayer.y - Yplayer >= pEnemy->pos.y - pEnemy->Yenemy&&
					g_posPlayer.y + Yplayer <= pEnemy->pos.y + pEnemy->Yenemy)
				{
					g_Player1.nLife--;
				}
			}
		}
}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
	//g_nCounterAnimPlayer++;		//�J�E���^�[�����Z
	//if ((g_nCounterAnimPlayer%6)==0)
	//{//��莞�Ԍo��
	//	
	//	g_nCounterAnimPlayer = 0;		//�J�E���^�[�������l�ɖ߂�	

	//	if (g_nPatternAnimPlayer > MaxPattern)
	//	{
	//		g_nCounterAnimPlayer = 0;		//�p�^�[��No.�������l�ɖ߂�
	//	}

	//	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//	//�e�N�X�`�����W�̍X�V
	//	float u=0.2f;
	//	float v=0.5f;
	//	int Pattern=5;
	//	pVtx[0].tex = D3DXVECTOR2((g_nPatternAnimPlayer % Pattern * u), (g_nPatternAnimPlayer/ Pattern) * v);
	//	pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimPlayer % Pattern * u + u, (g_nPatternAnimPlayer / Pattern) * v);
	//	pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimPlayer % Pattern * u, (g_nPatternAnimPlayer / Pattern) * v + v);
	//	pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimPlayer % Pattern * u + u, (g_nPatternAnimPlayer / Pattern) * v + v);
	//	
	//	//�ʒu���X�V����
	//	//g_posPlayer.x -= g_movePlaye.x;
	//	//g_posPlayer.y -= g_movePlaye.y;
	//	//if (g_posPlayer.x >= SCREEN_WIDTH)
	//	//{//�n�_(x���W)����ʂ̉E�[�ɓ�������
	//	//	g_posPlayer.x = SCREEN_WIDTH;		//�n�_����ʂ̉E�[�ɐݒ肷��
	//	//	g_movePlaye.x *= -1;				//�ړ��������t�]����
	//	//}
	//	//else if (g_posPlayer.x <= 0)
	//	//{//�n�_(x���W)����ʂ̍��[�ɓ�������
	//	//	g_posPlayer.x = 0;					//�n�_����ʂ̍��[�ɐݒ肷��
	//	//	g_movePlaye.x *= -1;				//�ړ��������t�]����
	//	//}

	//	////���_���W�̍X�V

	//	//pVtx[0].pos = D3DXVECTOR3(g_posPlayer.x , g_posPlayer.y, 0.0f);
	//	//pVtx[1].pos = D3DXVECTOR3(g_posPlayer.x + hX, g_posPlayer.y, 0.0f);
	//	//pVtx[2].pos = D3DXVECTOR3(g_posPlayer.x, g_posPlayer.y + nY, 0.0f);
	//	//pVtx[3].pos = D3DXVECTOR3(g_posPlayer.x + hX, g_posPlayer.y + nY, 0.0f);

	//	//�p�^�[��No.���X�V����
	//	g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % MaxPattern;
	//}
	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffPlayer->Unlock();
}

//�|���S���̕`�揈��
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));
	

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);
	

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		//D3DPT_TRIANGLELIST,		//�v���~�e�B�u�̎�� �O�p�`
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎�� �l�p�`
		0,
		2);							//�`�悷��v���~�e�B�u(�|���S��)��
			
}

//�v���C���[�̎擾
Player *GetPlayer(void)
{
	return &g_Player1;
}