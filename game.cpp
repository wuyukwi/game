#include"game.h"
#include"bullet.h"
#include"background.h"
#include"explosion.h"
#include"enemy.h"
#include"hp.h"
#include"Score.h"
#include"player.h"

//ゲームの初期化処理
void InitGame(void)
{
	//背景の初期化処理
	InitBackground();

	//弾の初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	//スコアの初期化処理
	InitScore();

	//プレイヤーの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();

	//HPの初期化処理
	InitHp();

	int nCnt = 1;
	//for (int nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	//{//敵の設定処理
	SetEnemy(D3DXVECTOR3(320.0f + (nCnt * 20.0f), 100.0f + (nCnt * 20.0f), 0.0f), 0);
	SetEnemy(D3DXVECTOR3(960.0f - (nCnt * 20.0f), 100.0f - (nCnt * 20.0f), 0.0f), 1);
	SetEnemy(D3DXVECTOR3(320.0f - (nCnt * 20.0f), 600.0f + (nCnt * 20.0f), 0.0f), 2);
	SetEnemy(D3DXVECTOR3(960.0f - (nCnt * 20.0f), 600.0f - (nCnt * 20.0f), 0.0f), 3);
	/*}*/
}

void UninitGame(void)
{

	//背景の終了処理
	UninitBackground();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();

	//プレイヤーの終了処理
	UninitPlayer();

	//敵の終了処理
	UninitEnemy();

	//HPの終了処理
	UninitHp();

	//スコアの終了処理
	UninitScore();
}

//更新処理
void UpdateGame(void)
{
	//背景の更新処理
	UpdateBackground();

	//弾の更新処理
	UpdateBullet();

	//爆発の更新処理
	UpdateExplosion();

	//プレイヤーの更新処理
	UpdatePlayer();

	//敵の更新処理
	UpdateEnemy();

	//スコアの更新処理
	UpdateScore();

	//HPの更新処理
	UpdateHp();

	Player *pPlayer;
	pPlayer = GetPlayer();
	if (pPlayer->nLife == 0)
	{
		//モード設定
		SetMode(MODE_RESULT);
	}
}

//ゲーム描画処理
void DrawGame(void)
{
	//背景の描画処理
	DrawBackground();

	//弾の描画処理
	DrawBullet();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//爆発の描画処理
	DrawExplosion();

	//HPの描画処理
	DrawHp();

	//スコアの描画処理
	DrawScore();
}