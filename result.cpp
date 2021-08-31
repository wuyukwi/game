#include"result.h"

//グローバル変数
LPDIRECT3DTEXTURE9			g_pTextureResult[MAX_RESULT] = { NULL,NULL };		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;									//頂点バッファのポインタ

																					//タイトルの初期化処理
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntResult;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\result.jpg",
		&g_pTextureResult[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gameover.png",
		&g_pTextureResult[1]);
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);


	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	float gameoverX = 421.0f;
	float gameoverY = 78.0f;

	for (nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		if (nCntResult == 0)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		else if (nCntResult == 1)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(gameoverX, 200.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(gameoverX * 2, 200.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(gameoverX, 200.0f + gameoverY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(gameoverX * 2, 200.0f + gameoverY, 0.0f);
		}

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
	g_pVtxBuffResult->Unlock();
}

//タイトルの終了処理
void UninitResult(void)
{
	int nCntResult;

	for (nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{//テクスチャの破棄
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	if (g_pVtxBuffResult != NULL)
	{//頂点バッファの破棄
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//タイトルの更新処理
void UpdateResult(void)
{

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//モード設定(ゲーム画面に移行)
		SetMode(MODE_TITLE);
	}
}

//タイトルの描画処理
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntbackground;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの描画
	for (nCntbackground = 0; nCntbackground < MAX_RESULT; nCntbackground++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[nCntbackground]);

		//ポリゴンの描画 四角
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntbackground * 4,			//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}