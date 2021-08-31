#include"enemy.h"
#include"explosion.h"
#include"input.h"
#include"Score.h"

//�}�N����`


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTextureEnemy[NUM_ENEMY] = { NULL,NULL,NULL,NULL };		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;		//���_�o�b�t�@�̃|�C���^
Enemy g_aEnemy[MAX_ENEMY];		//�G�̏��

//�G�̏���������
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	int nCntNumEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\enemy001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\enemy003.png",
		&g_apTextureEnemy[3]);



	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);


	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//�G�̏��̏�����
	for (nCntNumEnemy = 0; nCntNumEnemy < MAX_ENEMY; nCntNumEnemy++)
	{
		//���_���W�̐ݒ�
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//�G�̃^�C�v��ݒ�
		g_aEnemy[nCntNumEnemy].nType = 0;
		g_aEnemy[nCntNumEnemy].nLife = 1;						//�̗͂̏�����
		g_aEnemy[nCntNumEnemy].Xenemy = 0.0f;						//�傫���̏�����
		g_aEnemy[nCntNumEnemy].Yenemy = 0.0f;						//�傫���̏�����
		g_aEnemy[nCntNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��ʂ̏�����
		g_aEnemy[nCntNumEnemy].nCounterAnim = 0;
		g_aEnemy[nCntNumEnemy].nPatternAnim = 0;
		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//�G�̏I������
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{//�e�N�X�`���̔j��
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	if (g_pVtxBuffEnemy != NULL)
	{//���_�o�b�t�@�̔j��
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//�G�̍X�V����
void UpdateEnemy(void)
{
	int nCntEnemy = 0;
	int nInterval = 3;
	int nCounterAnim = 0;
	int nPatternAnim = 0;
	//���_���ւ̃|�C���^�𐶐�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���
			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

			if (g_aEnemy[nCntEnemy].pos.y >= GAME_HEIGHT - g_aEnemy[nCntEnemy].Yenemy)
			{//�n�_(Y���W)����ʂ̉��[�ɓ�������
				g_aEnemy[nCntEnemy].pos.y = GAME_HEIGHT - g_aEnemy[nCntEnemy].Yenemy;		//�n�_����ʂ̉��[�ɐݒ肷��
				g_aEnemy[nCntEnemy].move.y *= -1;				//�ړ��������t�]����
			}
			else if (g_aEnemy[nCntEnemy].pos.y <= 0 + g_aEnemy[nCntEnemy].Yenemy)
			{//�n�_(Y���W)����ʂ̏�[�ɓ�������
				g_aEnemy[nCntEnemy].pos.y = 0 + g_aEnemy[nCntEnemy].Yenemy;					//�n�_����ʂ̏�[�ɐݒ肷��
				g_aEnemy[nCntEnemy].move.y *= -1;				//�ړ��������t�]����
			}

			if (g_aEnemy[nCntEnemy].pos.x >= GAME_WIDTH*3.0f - g_aEnemy[nCntEnemy].Xenemy)
			{//�n�_(x���W)����ʂ̉E�[�ɓ�������
				g_aEnemy[nCntEnemy].pos.x = GAME_WIDTH*3.0f - g_aEnemy[nCntEnemy].Xenemy;		//�n�_����ʂ̉E�[�ɐݒ肷��
				g_aEnemy[nCntEnemy].move.x *= -1;				//�ړ��������t�]����
			}
			else if (g_aEnemy[nCntEnemy].pos.x <= GAME_WIDTH + g_aEnemy[nCntEnemy].Xenemy)
			{//�n�_(x���W)����ʂ̍��[�ɓ�������
				g_aEnemy[nCntEnemy].pos.x = GAME_WIDTH + g_aEnemy[nCntEnemy].Xenemy;					//�n�_����ʂ̍��[�ɐݒ肷��
				g_aEnemy[nCntEnemy].move.x *= -1;				//�ړ��������t�]����
			}

			//���_���W�̎w��
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Yenemy, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Yenemy, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Yenemy, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Yenemy, 0.0f);

			////�G�̃A�j���[�V����
			//nCounterAnim++;		//�J�E���^�[�i��

			//if ((nCounterAnim % nInterval) == 0)
			//{//��莞�Ԍo��
			//	nPatternAnim++;			//�p�^�[��No.���X�V

			//	if (nPatternAnim >= g_aEnemy[nCntEnemy].nPatternAnim)
			//	{
			//		nPatternAnim = 0;
			//	}

			//	//�e�N�X�`�����W�̍X�V
			//	float u = 1.0f;
			//	float v = 1.0f;
			//	int Pattern = g_aEnemy[nCntEnemy].nPatternAnim;
			//	pVtx[0].tex = D3DXVECTOR2((g_aEnemy[nCntEnemy].nPatternAnim % Pattern * u), (g_aEnemy[nCntEnemy].nPatternAnim / Pattern) * v);
			//	pVtx[1].tex = D3DXVECTOR2((g_aEnemy[nCntEnemy].nPatternAnim % Pattern * u) + u, (g_aEnemy[nCntEnemy].nPatternAnim / Pattern) * v);
			//	pVtx[2].tex = D3DXVECTOR2((g_aEnemy[nCntEnemy].nPatternAnim % Pattern * u), (g_aEnemy[nCntEnemy].nPatternAnim / Pattern) * v + v);
			//	pVtx[3].tex = D3DXVECTOR2((g_aEnemy[nCntEnemy].nPatternAnim % Pattern * u) + u, (g_aEnemy[nCntEnemy].nPatternAnim / Pattern) * v + v);
			//}

			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;

			default:
				break;
			}
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�

		float nXRandPos = float(rand() % 640);
		float nYRandPos = float(rand() % 720);
		int nRandType = rand() % 4;

		if (g_aEnemy[nCntEnemy].nLife == 0)
		{
				SetEnemy(D3DXVECTOR3(320.0f+nXRandPos, nYRandPos, 0.0f), nRandType);
		}
		
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}


//�G�̕`�揈��
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̕`��
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�e���g�p����Ă���ꍇ

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//�|���S���̕`�� �l�p
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCntEnemy * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);							//�v���~�e�B�u(�|���S��)��
		}
	}
}

//�G�̐ݒ菈��
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_aEnemy[nCntEnemy].bUse == false)
			{//�G���g�p����Ă��Ȃ��ꍇ
				g_aEnemy[nCntEnemy].pos = pos;
				g_aEnemy[nCntEnemy].nType = nType;

				if (g_aEnemy[nCntEnemy].nType == 0)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//�̗͂̏�����
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(3.0f, 3.0f, 0.0f);			//�ړ��ʂ̏�����
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				if (g_aEnemy[nCntEnemy].nType == 1)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//�̗͂̏�����
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-3.0f, -3.0f, 0.0f);			//�ړ��ʂ̏�����
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				if (g_aEnemy[nCntEnemy].nType == 2)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//�̗͂̏�����
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);			//�ړ��ʂ̏�����
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				if (g_aEnemy[nCntEnemy].nType == 3)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//�̗͂̏�����
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(5.0f, 5.0f, 0.0f);			//�ړ��ʂ̏�����
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				if (g_aEnemy[nCntEnemy].nType == 4)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//�̗͂̏�����
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//�傫���̏�����
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);			//�ړ��ʂ̏�����
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
				}


				//���_���W�̎w��
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Yenemy, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Yenemy, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Yenemy, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Yenemy, 0.0f);

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_aEnemy[nCntEnemy].bUse = true;	//�g�p���Ă����Ԃɂ���
				break;
			}
			pVtx += 4;
		}
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//�G�̓����菈��
void HitEnemy(int nCntEnemy, int nDamage)
{

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		SetExplosion(g_aEnemy->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		AddScore(g_aEnemy[nCntEnemy].nType*100);
		g_aEnemy[nCntEnemy].bUse = false;
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 10;		//�_���[�W��Ԃ������Ԃ�ݒ�

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCntEnemy * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 1);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 1);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 1);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 1);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffEnemy->Unlock();
	}

	
}

Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}