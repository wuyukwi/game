#include"game.h"
#include"bullet.h"
#include"background.h"
#include"explosion.h"
#include"enemy.h"
#include"hp.h"
#include"Score.h"
#include"player.h"

//�Q�[���̏���������
void InitGame(void)
{
	//�w�i�̏���������
	InitBackground();

	//�e�̏���������
	InitBullet();

	//�����̏���������
	InitExplosion();

	//�X�R�A�̏���������
	InitScore();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�̏���������
	InitEnemy();

	//HP�̏���������
	InitHp();

	int nCnt = 1;
	//for (int nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	//{//�G�̐ݒ菈��
	SetEnemy(D3DXVECTOR3(320.0f + (nCnt * 20.0f), 100.0f + (nCnt * 20.0f), 0.0f), 0);
	SetEnemy(D3DXVECTOR3(960.0f - (nCnt * 20.0f), 100.0f - (nCnt * 20.0f), 0.0f), 1);
	SetEnemy(D3DXVECTOR3(320.0f - (nCnt * 20.0f), 600.0f + (nCnt * 20.0f), 0.0f), 2);
	SetEnemy(D3DXVECTOR3(960.0f - (nCnt * 20.0f), 600.0f - (nCnt * 20.0f), 0.0f), 3);
	/*}*/
}

void UninitGame(void)
{

	//�w�i�̏I������
	UninitBackground();

	//�e�̏I������
	UninitBullet();

	//�����̏I������
	UninitExplosion();

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�̏I������
	UninitEnemy();

	//HP�̏I������
	UninitHp();

	//�X�R�A�̏I������
	UninitScore();
}

//�X�V����
void UpdateGame(void)
{
	//�w�i�̍X�V����
	UpdateBackground();

	//�e�̍X�V����
	UpdateBullet();

	//�����̍X�V����
	UpdateExplosion();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�G�̍X�V����
	UpdateEnemy();

	//�X�R�A�̍X�V����
	UpdateScore();

	//HP�̍X�V����
	UpdateHp();

	Player *pPlayer;
	pPlayer = GetPlayer();
	if (pPlayer->nLife == 0)
	{
		//���[�h�ݒ�
		SetMode(MODE_RESULT);
	}
}

//�Q�[���`�揈��
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBackground();

	//�e�̕`�揈��
	DrawBullet();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	//�����̕`�揈��
	DrawExplosion();

	//HP�̕`�揈��
	DrawHp();

	//�X�R�A�̕`�揈��
	DrawScore();
}