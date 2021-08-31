#ifndef _ENEMY_H_		//���̃}�N����`������Ȃ�������
#define _ENEMY_H_		//��d�C���N���[�h�h�~�̃}�N����`

#include"main.h"

//�}�N����`
#define MAX_ENEMY (128)	//�G�̍ő吔
#define NUM_ENEMY (4)	//�G�̎��

typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	float Xenemy;	//�G�̕��̔���
	float Yenemy;	//�G�̔���
	int	nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int nPatternAnim;
	int nLife;				//�̗�
	int nType;				//���
	ENEMYSTATE state;		//���
	int nCounterState;		//��ԊǗ��J�E���^�[
	bool bUse;				//�g�p���Ă���ǂ���
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);//�ʒu�@���
void HitEnemy(int nCntEnemy,int nDamage);
Enemy *GetEnemy(void);


#endif