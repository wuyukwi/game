#include"bullet.h"
#include"explosion.h"
#include"enemy.h"

//�}�N����`
#define MAX_EXPLOSION (128)	//�e�̍ő吔

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	int nLife;				//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Bullet;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//���_�o�b�t�@�̃|�C���^
Bullet g_aBullet[MAX_EXPLOSION];		//�e�̏��
int g_nCntAnimBullet;					//�e�̃A�j���[�V�����J�E���^�[
float g_fLengthBullet;					//�Ίp���̒���
float g_fAngleBullet;					//�Ίp���̊p�x


//�e�̏���������
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^
	int nCntBullet;

	float Xbullet = 5.0f;		//�o���b�g��
	float Ybullet = 5.0f;		//�o���b�g����

	//�Ίp���̒������Z�o����
	g_fLengthBullet = sqrtf((Xbullet * 2) * (Xbullet * 2) + (Ybullet * 2) * (Ybullet * 2)) / 2.0f;

	//�Ίp���̊p�x���Z�o����
	g_fAngleBullet = atan2f((Xbullet * 2), (Ybullet * 2));


	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].bUse = false;		//�g�p���ĂȂ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//���_���W�̎w��
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�	    u     v
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffBullet->Unlock();
}

//�e�̏I������
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//�e�̍X�V����
void UpdateBullet(void)
{
	int nCntBullet;
	int nBulletSize = 8;

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			Enemy *pEnemy;		//�G�̏��ւ̃|�C���^
			int nCountEnemy;

			//�G�̎擾
			pEnemy = GetEnemy();

			for (nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{//�G���g�p����Ă���
					if (g_aBullet[nCntBullet].pos.x - nBulletSize >= pEnemy->pos.x - pEnemy->Xenemy&& 
						g_aBullet[nCntBullet].pos.x + nBulletSize <= pEnemy->pos.x + pEnemy->Xenemy&&
						g_aBullet[nCntBullet].pos.y - nBulletSize >= pEnemy->pos.y - pEnemy->Yenemy&&
						g_aBullet[nCntBullet].pos.y + nBulletSize <= pEnemy->pos.y + pEnemy->Yenemy)
					{
						//�G�̃q�b�g����
						HitEnemy(nCountEnemy, 1);

						g_aBullet[nCntBullet].bUse = false;
						
					}
				}

			}

			//�e�̈ʒu�̍X�V
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			
			if (g_aBullet[nCntBullet].pos.y >= GAME_HEIGHT)
			{//�n�_(Y���W)����ʂ̉��[�ɓ�������
				g_aBullet[nCntBullet].pos.y = GAME_HEIGHT;		//�n�_����ʂ̉��[�ɐݒ肷��
				g_aBullet[nCntBullet].move.y *= -1;				//�ړ��������t�]����
			}
			else if (g_aBullet[nCntBullet].pos.y <= 0)
			{//�n�_(Y���W)����ʂ̏�[�ɓ�������
				g_aBullet[nCntBullet].pos.y = 0;					//�n�_����ʂ̏�[�ɐݒ肷��
				g_aBullet[nCntBullet].move.y *= -1;				//�ړ��������t�]����
			}

			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;			//�����̎n�_( x���W )���ړ�����

			if (g_aBullet[nCntBullet].pos.x >= GAME_WIDTH * 3)
			{//�n�_(x���W)����ʂ̉E�[�ɓ�������
				g_aBullet[nCntBullet].pos.x = GAME_WIDTH * 3;		//�n�_����ʂ̉E�[�ɐݒ肷��
				g_aBullet[nCntBullet].move.x *= -1;				//�ړ��������t�]����
			}
			else if (g_aBullet[nCntBullet].pos.x <= GAME_WIDTH)
			{//�n�_(x���W)����ʂ̍��[�ɓ�������
				g_aBullet[nCntBullet].pos.x = GAME_WIDTH;					//�n�_����ʂ̍��[�ɐݒ肷��
				g_aBullet[nCntBullet].move.x *= -1;				//�ړ��������t�]����
			}

			////�e����ʊO�ɏo��
			//if (g_aBullet[nCntBullet].pos.y < 0)
			//{
			//	g_aBullet[nCntBullet].bUse = false;		//�g�p����Ă��Ȃ���Ԃɂ���
			//}

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

			//���_�������b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntBullet * 4);		//�Y���̈ʒu�܂Ői�߂�

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - nBulletSize, g_aBullet[nCntBullet].pos.y - nBulletSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + nBulletSize, g_aBullet[nCntBullet].pos.y - nBulletSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - nBulletSize, g_aBullet[nCntBullet].pos.y + nBulletSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + nBulletSize, g_aBullet[nCntBullet].pos.y + nBulletSize, 0.0f);

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffBullet->Unlock();

			g_aBullet[nCntBullet].nLife++;

			if (g_aBullet[nCntBullet].nLife >= 400)
			{
				//�����̐ݒ�
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aBullet[nCntBullet].bUse = false;	//�g�p����Ă��Ȃ���Ԃɂ���
			}
		}
	}
	
}

//�e�̕`�揈��
void DrawBullet(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntBullet * 4,
				2);
		}
	}
}

//�e�̐ݒ菈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	int nCntBullet;

	int nBulletSize = 20;
	g_nCntAnimBullet++;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
		{
			if (g_aBullet[nCntBullet].bUse == false)
			{//�e���g�p����Ă��Ȃ��ꍇ
				g_aBullet[nCntBullet].pos = pos;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - nBulletSize, g_aBullet[nCntBullet].pos.y - nBulletSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + nBulletSize, g_aBullet[nCntBullet].pos.y - nBulletSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - nBulletSize, g_aBullet[nCntBullet].pos.y + nBulletSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + nBulletSize, g_aBullet[nCntBullet].pos.y + nBulletSize, 0.0f);

				//���_�o�b�t�@���A�����b�N����
				g_pVtxBuffBullet->Unlock();

				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].nLife = 100;
				g_aBullet[nCntBullet].bUse = true;	//�g�p���Ă����Ԃɂ���
				break;
			}
			pVtx += 4;
		}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}
