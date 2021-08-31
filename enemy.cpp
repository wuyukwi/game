#include"enemy.h"
#include"explosion.h"
#include"input.h"
#include"Score.h"

//マクロ定義


//グローバル変数
LPDIRECT3DTEXTURE9		g_apTextureEnemy[NUM_ENEMY] = { NULL,NULL,NULL,NULL };		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;		//頂点バッファのポインタ
Enemy g_aEnemy[MAX_ENEMY];		//敵の情報

//敵の初期化処理
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	int nCntNumEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
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



	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);


	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//敵の情報の初期化
	for (nCntNumEnemy = 0; nCntNumEnemy < MAX_ENEMY; nCntNumEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//敵のタイプを設定
		g_aEnemy[nCntNumEnemy].nType = 0;
		g_aEnemy[nCntNumEnemy].nLife = 1;						//体力の初期化
		g_aEnemy[nCntNumEnemy].Xenemy = 0.0f;						//大きさの初期化
		g_aEnemy[nCntNumEnemy].Yenemy = 0.0f;						//大きさの初期化
		g_aEnemy[nCntNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量の初期化
		g_aEnemy[nCntNumEnemy].nCounterAnim = 0;
		g_aEnemy[nCntNumEnemy].nPatternAnim = 0;
		pVtx += 4;		//頂点データのポインタを４つ分進める
	}


	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//敵の終了処理
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{//テクスチャの破棄
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	if (g_pVtxBuffEnemy != NULL)
	{//頂点バッファの破棄
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//敵の更新処理
void UpdateEnemy(void)
{
	int nCntEnemy = 0;
	int nInterval = 3;
	int nCounterAnim = 0;
	int nPatternAnim = 0;
	//頂点情報へのポインタを生成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている
			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

			if (g_aEnemy[nCntEnemy].pos.y >= GAME_HEIGHT - g_aEnemy[nCntEnemy].Yenemy)
			{//始点(Y座標)が画面の下端に当たった
				g_aEnemy[nCntEnemy].pos.y = GAME_HEIGHT - g_aEnemy[nCntEnemy].Yenemy;		//始点を画面の下端に設定する
				g_aEnemy[nCntEnemy].move.y *= -1;				//移動方向を逆転する
			}
			else if (g_aEnemy[nCntEnemy].pos.y <= 0 + g_aEnemy[nCntEnemy].Yenemy)
			{//始点(Y座標)が画面の上端に当たった
				g_aEnemy[nCntEnemy].pos.y = 0 + g_aEnemy[nCntEnemy].Yenemy;					//始点を画面の上端に設定する
				g_aEnemy[nCntEnemy].move.y *= -1;				//移動方向を逆転する
			}

			if (g_aEnemy[nCntEnemy].pos.x >= GAME_WIDTH*3.0f - g_aEnemy[nCntEnemy].Xenemy)
			{//始点(x座標)が画面の右端に当たった
				g_aEnemy[nCntEnemy].pos.x = GAME_WIDTH*3.0f - g_aEnemy[nCntEnemy].Xenemy;		//始点を画面の右端に設定する
				g_aEnemy[nCntEnemy].move.x *= -1;				//移動方向を逆転する
			}
			else if (g_aEnemy[nCntEnemy].pos.x <= GAME_WIDTH + g_aEnemy[nCntEnemy].Xenemy)
			{//始点(x座標)が画面の左端に当たった
				g_aEnemy[nCntEnemy].pos.x = GAME_WIDTH + g_aEnemy[nCntEnemy].Xenemy;					//始点を画面の左端に設定する
				g_aEnemy[nCntEnemy].move.x *= -1;				//移動方向を逆転する
			}

			//頂点座標の指定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Yenemy, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Yenemy, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Yenemy, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Yenemy, 0.0f);

			////敵のアニメーション
			//nCounterAnim++;		//カウンター進む

			//if ((nCounterAnim % nInterval) == 0)
			//{//一定時間経過
			//	nPatternAnim++;			//パターンNo.を更新

			//	if (nPatternAnim >= g_aEnemy[nCntEnemy].nPatternAnim)
			//	{
			//		nPatternAnim = 0;
			//	}

			//	//テクスチャ座標の更新
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

					//頂点カラーの設定
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
		pVtx += 4;	//頂点データのポインタを4つ分進める

		float nXRandPos = float(rand() % 640);
		float nYRandPos = float(rand() % 720);
		int nRandType = rand() % 4;

		if (g_aEnemy[nCntEnemy].nLife == 0)
		{
				SetEnemy(D3DXVECTOR3(320.0f+nXRandPos, nYRandPos, 0.0f), nRandType);
		}
		
	}
	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}


//敵の描画処理
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの描画
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//弾が使用されている場合

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//ポリゴンの描画 四角
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntEnemy * 4,			//描画する最初の頂点インデックス
				2);							//プリミティブ(ポリゴン)数
		}
	}
}

//敵の設定処理
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_aEnemy[nCntEnemy].bUse == false)
			{//敵が使用されていない場合
				g_aEnemy[nCntEnemy].pos = pos;
				g_aEnemy[nCntEnemy].nType = nType;

				if (g_aEnemy[nCntEnemy].nType == 0)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//体力の初期化
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(3.0f, 3.0f, 0.0f);			//移動量の初期化
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				if (g_aEnemy[nCntEnemy].nType == 1)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//体力の初期化
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-3.0f, -3.0f, 0.0f);			//移動量の初期化
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				if (g_aEnemy[nCntEnemy].nType == 2)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//体力の初期化
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);			//移動量の初期化
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				if (g_aEnemy[nCntEnemy].nType == 3)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//体力の初期化
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(5.0f, 5.0f, 0.0f);			//移動量の初期化
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				if (g_aEnemy[nCntEnemy].nType == 4)
				{
					g_aEnemy[nCntEnemy].nLife = 10;						//体力の初期化
					g_aEnemy[nCntEnemy].Xenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].Yenemy = 50.0f;						//大きさの初期化
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);			//移動量の初期化
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
				}


				//頂点座標の指定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Yenemy, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Yenemy, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Yenemy, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].Xenemy, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Yenemy, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_aEnemy[nCntEnemy].bUse = true;	//使用している状態にする
				break;
			}
			pVtx += 4;
		}
	
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//敵の当たり処理
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
		g_aEnemy[nCntEnemy].nCounterState = 10;		//ダメージ状態を持つ時間を設定

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCntEnemy * 4);		//該当の位置まで進める

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 1);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 1);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 1);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 1);

		//頂点バッファをアンロックする
		g_pVtxBuffEnemy->Unlock();
	}

	
}

Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}