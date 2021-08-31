#include "main.h"
#include "background.h"
#include "input.h"

#define MAX_BACKGROUND 3		//背景の枚数

//グローバル変数
LPDIRECT3DTEXTURE9			g_pTexturebackground[MAX_BACKGROUND] = { NULL, NULL, NULL};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffbackground = NULL;									//頂点バッファのポインタ
D3DXVECTOR3					g_rotBackround;		    //向き
D3DXVECTOR3					g_moveBackground;		//移動量
//背景の初期化処理
void InitBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	int nCntbackground;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\bg01.jpg",
		&g_pTexturebackground[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_pTexturebackground[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\bg102.png",
		&g_pTexturebackground[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BACKGROUND,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffbackground,
		NULL);

	g_moveBackground = D3DXVECTOR3(0.0f, 0.01f, 0.0f);			//移動量を初期化
	g_rotBackround = D3DXVECTOR3(0.0f, 0.01f, 0.0f);			//向きを初期化

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffbackground->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntbackground = 0; nCntbackground < MAX_BACKGROUND; nCntbackground++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(GAME_WIDTH, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GAME_WIDTH*3.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GAME_WIDTH, GAME_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GAME_WIDTH*3.0f, GAME_HEIGHT, 0.0f);

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
	g_pVtxBuffbackground->Unlock();
}

//-------------------------
//背景の終了処理
//-------------------------
void UninitBackground(void)
{
	if (g_pTexturebackground[0] != NULL)
	{//テクスチャの破棄
		g_pTexturebackground[0]->Release();
		g_pTexturebackground[0] = NULL;
	}

	if (g_pTexturebackground[1] != NULL)
	{//テクスチャの破棄
		g_pTexturebackground[1]->Release();
		g_pTexturebackground[1] = NULL;
	}

	if (g_pTexturebackground[2] != NULL)
	{//テクスチャの破棄
		g_pTexturebackground[2]->Release();
		g_pTexturebackground[2] = NULL;
	}

	if (g_pVtxBuffbackground != NULL)
	{//頂点バッファの破棄
		g_pVtxBuffbackground->Release();
		g_pVtxBuffbackground = NULL;
	}
}

//-------------------------
//背景の更新処理
//-------------------------
void UpdateBackground(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	int nCntbackground;
	float MoveSpeed = 0.0007f;		//移動スピード
	float RotSpeed = 0.005f;	//回転スピード
	float DownSpeed = 0.07f;	//スピード減衰係数

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffbackground->Lock(0, 0, (void**)&pVtx, 0);

	
	//Wキーが押された
	if (GetKeyboardPress(DIK_W) == true)
	{
	
		//Wキーが押された
			g_moveBackground.x += sinf(0.0f + g_rotBackround.z) * MoveSpeed;
			g_moveBackground.y += cosf(0.0f + g_rotBackround.z) * MoveSpeed;
			//移動量を更新(増加させる)
		
	}


	//Sキーが押された
	else if (GetKeyboardPress(DIK_S) == true)
	{		
		g_moveBackground.x += sinf(0.0f + g_rotBackround.z) * MoveSpeed;
		g_moveBackground.y += cosf(0) * -MoveSpeed / 3;
		//移動量を更新(増加させる)
	}

	//移動量を更新(減衰させる)
	g_moveBackground.x += (0.0f - g_moveBackground.x) * DownSpeed;
	g_moveBackground.y += (0.005f - g_moveBackground.y) * DownSpeed;


	//頂点座標の更新
	for (nCntbackground = 0; nCntbackground < MAX_BACKGROUND; nCntbackground++)
	{
		pVtx[0].tex.x -= g_moveBackground.x / (nCntbackground + 1);
		pVtx[0].tex.y -= g_moveBackground.y / (nCntbackground + 1);

		pVtx[1].tex.x -= g_moveBackground.x / (nCntbackground + 1);
		pVtx[1].tex.y -= g_moveBackground.y / (nCntbackground + 1);

		pVtx[2].tex.x -= g_moveBackground.x / (nCntbackground + 1);
		pVtx[2].tex.y -= g_moveBackground.y / (nCntbackground + 1);

		pVtx[3].tex.x -= g_moveBackground.x / (nCntbackground + 1);
		pVtx[3].tex.y -= g_moveBackground.y / (nCntbackground + 1);


		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffbackground->Unlock();
}

//-------------------------
//背景の描画処理
//-------------------------
void DrawBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntbackground;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffbackground, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの描画
	for (nCntbackground = 0; nCntbackground < MAX_BACKGROUND; nCntbackground++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturebackground[nCntbackground]);

		//ポリゴンの描画 四角
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntbackground * 4,			//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}