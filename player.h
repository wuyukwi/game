#ifndef _PLAYER_H_		//���̃}�N����`������Ȃ�������

#define _PLAYER_H_		//��d�C���N���[�h�h�~�̃}�N����`

//�v���C���[�\���̂̐ݒ�
typedef struct
{
	int nLife;
	bool bUse;
}Player;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

Player *GetPlayer(void);



















#endif // !_Player_H_

