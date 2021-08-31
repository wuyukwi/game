#include"bullet.h"
#include"explosion.h"
#include"enemy.h"

//マクロ定義
#define MAX_EXPLOSION (128)	//弾の最大数

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	int nLife;				//寿命
	bool bUse;				//使用しているかどうか
}Bullet;

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//頂点バッファのポインタ
Bullet g_aBullet[MAX_EXPLOSION];		//弾の情報
int g_nCntAnimBullet;					//弾のアニメーションカウンター
float g_fLengthBullet;					//対角線の長さ
float g_fAngleBullet;					//対角線の角度


//弾の初期化処理
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ
	int nCntBullet;

	float Xbullet = 5.0f;		//バレット幅
	float Ybullet = 5.0f;		//バレット長さ

	//対角線の長さを算出する
	g_fLengthBullet = sqrtf((Xbullet * 2) * (Xbullet * 2) + (Ybullet * 2) * (Ybullet * 2)) / 2.0f;

	//対角線の角度を算出する
	g_fAngleBullet = atan2f((Xbullet * 2), (Ybullet * 2));


	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].bUse = false;		//使用してない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//頂点座標の指定
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定	    u     v
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
		//頂点バッファをアンロックする
		g_pVtxBuffBullet->Unlock();
}

//弾の終了処理
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//弾の更新処理
void UpdateBullet(void)
{
	int nCntBullet;
	int nBulletSize = 8;

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			Enemy *pEnemy;		//敵の情報へのポインタ
			int nCountEnemy;

			//敵の取得
			pEnemy = GetEnemy();

			for (nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{//敵が使用されている
					if (g_aBullet[nCntBullet].pos.x - nBulletSize >= pEnemy->pos.x - pEnemy->Xenemy&& 
						g_aBullet[nCntBullet].pos.x + nBulletSize <= pEnemy->pos.x + pEnemy->Xenemy&&
						g_aBullet[nCntBullet].pos.y - nBulletSize >= pEnemy->pos.y - pEnemy->Yenemy&&
						g_aBullet[nCntBullet].pos.y + nBulletSize <= pEnemy->pos.y + pEnemy->Yenemy)
					{
						//敵のヒット処理
						HitEnemy(nCountEnemy, 1);

						g_aBullet[nCntBullet].bUse = false;
						
					}
				}

			}

			//弾の位置の更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			
			if (g_aBullet[nCntBullet].pos.y >= GAME_HEIGHT)
			{//始点(Y座標)が画面の下端に当たった
				g_aBullet[nCntBullet].pos.y = GAME_HEIGHT;		//始点を画面の下端に設定する
				g_aBullet[nCntBullet].move.y *= -1;				//移動方向を逆転する
			}
			else if (g_aBullet[nCntBullet].pos.y <= 0)
			{//始点(Y座標)が画面の上端に当たった
				g_aBullet[nCntBullet].pos.y = 0;					//始点を画面の上端に設定する
				g_aBullet[nCntBullet].move.y *= -1;				//移動方向を逆転する
			}

			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;			//線分の始点( x座標 )を移動する

			if (g_aBullet[nCntBullet].pos.x >= GAME_WIDTH * 3)
			{//始点(x座標)が画面の右端に当たった
				g_aBullet[nCntBullet].pos.x = GAME_WIDTH * 3;		//始点を画面の右端に設定する
				g_aBullet[nCntBullet].move.x *= -1;				//移動方向を逆転する
			}
			else if (g_aBullet[nCntBullet].pos.x <= GAME_WIDTH)
			{//始点(x座標)が画面の左端に当たった
				g_aBullet[nCntBullet].pos.x = GAME_WIDTH;					//始点を画面の左端に設定する
				g_aBullet[nCntBullet].move.x *= -1;				//移動方向を逆転する
			}

			////弾が画面外に出た
			//if (g_aBullet[nCntBullet].pos.y < 0)
			//{
			//	g_aBullet[nCntBullet].bUse = false;		//使用されていない状態にする
			//}

			VERTEX_2D *pVtx;		//頂点情報へのポインタ

			//頂点情報をロックし、頂点情報へのポインタを取得
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntBullet * 4);		//該当の位置まで進める

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - nBulletSize, g_aBullet[nCntBullet].pos.y - nBulletSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + nBulletSize, g_aBullet[nCntBullet].pos.y - nBulletSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - nBulletSize, g_aBullet[nCntBullet].pos.y + nBulletSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + nBulletSize, g_aBullet[nCntBullet].pos.y + nBulletSize, 0.0f);

			//頂点バッファをアンロックする
			g_pVtxBuffBullet->Unlock();

			g_aBullet[nCntBullet].nLife++;

			if (g_aBullet[nCntBullet].nLife >= 400)
			{
				//爆発の設定
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aBullet[nCntBullet].bUse = false;	//使用されていない状態にする
			}
		}
	}
	
}

//弾の描画処理
void DrawBullet(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntBullet * 4,
				2);
		}
	}
}

//弾の設定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	int nCntBullet;

	int nBulletSize = 20;
	g_nCntAnimBullet++;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
		{
			if (g_aBullet[nCntBullet].bUse == false)
			{//弾が使用されていない場合
				g_aBullet[nCntBullet].pos = pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - nBulletSize, g_aBullet[nCntBullet].pos.y - nBulletSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + nBulletSize, g_aBullet[nCntBullet].pos.y - nBulletSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - nBulletSize, g_aBullet[nCntBullet].pos.y + nBulletSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + nBulletSize, g_aBullet[nCntBullet].pos.y + nBulletSize, 0.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffBullet->Unlock();

				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].nLife = 100;
				g_aBullet[nCntBullet].bUse = true;	//使用している状態にする
				break;
			}
			pVtx += 4;
		}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}
