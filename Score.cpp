#include "main.h"
#include"Score.h"

LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;	    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	//頂点バッファへのポインタ
int	g_nScore;				//スコアの値
D3DXVECTOR3 g_posScore;		//位置
float nXnumTex = 35.0f;
float nYnumTex = 70.0f;

void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D*pVtx;					//頂点情報へのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number000.png",
		&g_pTextureScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_KETA,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	int nCntScore; 

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_KETA; nCntScore++)
	{
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
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[0].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);

		g_posScore = D3DXVECTOR3(1235.0f, 20.0f, 0.0f);					//位置を初期化する
		g_nScore = 0;		//スコアの初期化

		pVtx += 4;
	}
	
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

void UpdateScore(void)
{
	SetScore(g_nScore);

}

void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの描画
	for (nCntScore = 0; nCntScore < MAX_KETA; nCntScore++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		//ポリゴンの描画 四角
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntScore * 4,							//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}

void SetScore(int nScore)
{
	int aPosTexU[MAX_KETA] = {0};		//各桁の数字を格納
	int nCntScore=0;

	while (nScore !=0)
	{
		aPosTexU[nCntScore] = nScore % 10;
		nScore /= 10;
		nCntScore++;
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_KETA; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - nCntScore* nXnumTex, g_posScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + nXnumTex - nCntScore* nXnumTex, g_posScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - nCntScore* nXnumTex, g_posScore.y + nYnumTex, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + nXnumTex - nCntScore* nXnumTex, g_posScore.y + nYnumTex, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCntScore] *0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aPosTexU[nCntScore] *0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCntScore] *0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aPosTexU[nCntScore] *0.1f + 0.1f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffScore->Unlock();

		pVtx += 4;
	}
		
	
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
	/*itoa(g_nScore, aPosTexU, 1);*/
}

void AddScore(int nValue)
{
	g_nScore += nValue;
}