#ifndef _ENEMY_H_		//このマクロ定義がされなかったら
#define _ENEMY_H_		//二重インクルード防止のマクロ定義

#include"main.h"

//マクロ定義
#define MAX_ENEMY (128)	//敵の最大数
#define NUM_ENEMY (4)	//敵の種類

typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	float Xenemy;	//敵の幅の半分
	float Yenemy;	//敵の半分
	int	nCounterAnim;	//アニメーションカウンター
	int nPatternAnim;
	int nLife;				//体力
	int nType;				//種類
	ENEMYSTATE state;		//状態
	int nCounterState;		//状態管理カウンター
	bool bUse;				//使用しているどうか
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);//位置　種類
void HitEnemy(int nCntEnemy,int nDamage);
Enemy *GetEnemy(void);


#endif