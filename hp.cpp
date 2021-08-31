#include "hp.h"

LPDIRECT3DTEXTURE9 g_pTextureHp = NULL;	    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHp = NULL;	//頂点バッファへのポインタ

void InitHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D*pVtx;					//頂点情報へのポインタ
	Player*pPlayer;
	
	//デバイスの取得
	pDevice = GetDevice();
	pPlayer = GetPlayer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hp.png",
		&g_pTextureHp);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHp,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(300.0f, 650.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(50.0f, 670.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(300.0f, 670.0f, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffHp->Unlock();
}

void UninitHp(void)
{
	//テクスチャの破棄
	if (g_pTextureHp != NULL)
	{
		g_pTextureHp->Release();
		g_pTextureHp = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffHp != NULL)
	{
		g_pVtxBuffHp->Release();
		g_pVtxBuffHp = NULL;
	}
}

void UpdateHp(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D*pVtx;					//頂点情報へのポインタ
	Player*pPlayer;

	//デバイスの取得
	pDevice = GetDevice();
	pPlayer = GetPlayer();



	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->nLife == 100)
	{
		// テクスチャ座標の設定
		pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
		pVtx[0].tex = D3DXVECTOR2(0.45f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
		pVtx[2].tex = D3DXVECTOR2(0.45f, 1.0f);
	}
	else if (pPlayer->nLife > 0)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f - (0.005f * (100 + pPlayer->nLife)), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.51f - (0.005f * (100+ pPlayer->nLife)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f - (0.005f * (100 + pPlayer->nLife)), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.51f - (0.005f * (100 + pPlayer->nLife)), 1.0f);
	}
																   
	//頂点バッファをアンロックする
	g_pVtxBuffHp->Unlock();
}

void DrawHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHp, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureHp);


	//ポリゴンの描画 四角
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,			//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数
}