#ifndef _PLAYER_H_		//このマクロ定義がされなかったら

#define _PLAYER_H_		//二重インクルード防止のマクロ定義

//プレイヤー構造体の設定
typedef struct
{
	int nLife;
	bool bUse;
}Player;

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

Player *GetPlayer(void);



















#endif // !_Player_H_

