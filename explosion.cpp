#include"explosion.h"

//マクロ定義
#define MAX_EXPLOSION (128)	//爆発の最大数

//爆発構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXCOLOR	col;				//色
	int			nCounterAnim;		//アニメーションカウンター
	int			nPatternAnim;		//アニメーションパターンNo.
	bool		bUse;				//使用しているかどうか
}Explosion;

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;			//頂点バッファのポインタ
Explosion				g_aExplosion[MAX_EXPLOSION];		//爆発の情報


//爆発の初期化処理
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ
	int nCntExplosion;
	int nSize = 50;					//爆発の大きさ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\explosion000.png",
		&g_pTextureExplosion);

	//爆発の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;		//使用してない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
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
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

		pVtx += 4;
	}
		//頂点バッファをアンロックする
		g_pVtxBuffExplosion->Unlock();
}

//爆発の終了処理
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//爆発の更新処理
void UpdateExplosion(void)
{
	int nCntExplosion;
	int nInterval = 3;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている

			g_aExplosion[nCntExplosion].nCounterAnim++;		//カウンター進む

			if ((g_aExplosion[nCntExplosion].nCounterAnim % nInterval) == 0)
			{//一定時間経過
				g_aExplosion[nCntExplosion].nPatternAnim++;			//パターンNo.を更新

				if (g_aExplosion[nCntExplosion].nPatternAnim == 8)
				{
					g_aExplosion[nCntExplosion].bUse = false;		//使用されていない状態にする
				}

				//テクスチャ座標の更新
				float u = 0.125f;
				float v = 1.0f;
				int Pattern = 8;
				pVtx[0].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % Pattern * u), (g_aExplosion[nCntExplosion].nPatternAnim / Pattern) * v);
				pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % Pattern * u) + u, (g_aExplosion[nCntExplosion].nPatternAnim / Pattern) * v);
				pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % Pattern * u), (g_aExplosion[nCntExplosion].nPatternAnim / Pattern) * v + v);
				pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % Pattern * u) + u, (g_aExplosion[nCntExplosion].nPatternAnim / Pattern) * v+ v);
			}	
		}
		pVtx += 4;		//頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//爆発の描画処理
void DrawExplosion(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ
	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntExplosion * 4,
				2);
		}
	}
}

//爆発の設定処理
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;
	int nSizeBullet = 20;					//爆発の大きさ

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//弾が使用されていない場合
			g_aExplosion[nCntExplosion].pos = pos;

			VERTEX_2D *pVtx;		//頂点情報へのポインタ

			//頂点情報をロックし、頂点情報へのポインタを取得
			g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntExplosion * 4);		//該当の位置まで進める

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - nSizeBullet, g_aExplosion[nCntExplosion].pos.y - nSizeBullet, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + nSizeBullet, g_aExplosion[nCntExplosion].pos.y - nSizeBullet, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - nSizeBullet, g_aExplosion[nCntExplosion].pos.y + nSizeBullet, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + nSizeBullet, g_aExplosion[nCntExplosion].pos.y + nSizeBullet, 0.0f);

			//テクスチャ座標の設定	    u     v
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

			g_pVtxBuffExplosion->Unlock();		//頂点バッファをアンロックする

			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;	//使用している状態にする

			break;
		}
	}
}

