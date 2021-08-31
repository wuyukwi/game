#include"title.h"

#define MAX_TITLE 1		//背景の枚数

//グローバル変数
LPDIRECT3DTEXTURE9			g_pTextureTitle[MAX_TITLE] = { NULL};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;									//頂点バッファのポインタ

//タイトルの初期化処理
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntTitle;


	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title.jpg",
		&g_pTextureTitle[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);


	VERTEX_2D *pVtx;		//頂点情報へのポインタ

							//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

		


		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

}

//タイトルの終了処理
void UninitTitle(void)
{
	if (g_pTextureTitle[0] != NULL)
	{//テクスチャの破棄
		g_pTextureTitle[0]->Release();
		g_pTextureTitle[0] = NULL;
	}

	if (g_pVtxBuffTitle != NULL)
	{//頂点バッファの破棄
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//タイトルの更新処理
void UpdateTitle(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//モード設定(ゲーム画面に移行)
		SetMode(MODE_GAME);
	}
	
}

//タイトルの描画処理
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntbackground;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの描画
	for (nCntbackground = 0; nCntbackground < MAX_TITLE; nCntbackground++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntbackground]);

		//ポリゴンの描画 四角
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntbackground * 4,			//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数

	
	}
}