#include"main.h"
#include"Player.h"
#include"input.h"
#include"bullet.h"
#include"enemy.h"

//-------------------------
//マクロ定義
//-------------------------

//-------------------------
//グローバル変数
//-------------------------
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;		//テクスチャへのポインタ
VERTEX_2D g_aVertex[4];			//頂点情報を格納
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		//頂点バッファのポインタ
Player g_Player1;

float Xplayer = 18.5f;		//プレイヤー幅の半分
float Yplayer = 23.5f;		//プレイヤー長さの半分	
float hX = 37.0f;		//テクスチャ幅の半分
float nY = 47.0f;		//テクスチャ長さの半分

int g_nCounterAnimPlayer;		//アニメーションカウンター
int g_nPatternAnimPlayer;		//アニメーションパターンNo.

D3DXVECTOR3 g_posPlayer;		//位置
D3DXVECTOR3 g_movePlayer;		//移動量
D3DXVECTOR3 g_rotPlayer;		//向き
D3DXVECTOR3 g_moveBullet;		//移動量

float g_fLengthPlayer;			//対角線の長さ
float g_fAnglePlayer;			//対角線の角度
float g_fmoveLength;			//対角線の移動量



//プレイヤーの初期化処理
void InitPlayer(void)
{
	float x = 650.0f;		    //位置X座標
	float y = 600.0f;		    //位置y座標
	g_Player1.nLife = 100;		//プレイヤーの体力を初期化する
	g_Player1.bUse = true;		//プレイヤーを使用されている状態にする

	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\hero.png",
		&g_pTexturePlayer);


	g_nCounterAnimPlayer = 0;								//カウンターを初期化する
	g_nPatternAnimPlayer = 0;								//パターンNo.を初期化する
	g_posPlayer = D3DXVECTOR3(x, y, 0.0f);					//位置を初期化する
	g_movePlayer = D3DXVECTOR3(0.0f,0.0f,0.0f);				//移動量を初期化する
	g_moveBullet = D3DXVECTOR3(0.0f, -10.0f, 0.0f);				//移動量を初期化する
	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
	g_fmoveLength = 0.0f;									//対角線の移動量を初期化
	
	//対角線の長さを算出する
	g_fLengthPlayer = sqrtf((Xplayer*2) * (Xplayer * 2) + (Yplayer * 2) * (Yplayer * 2)) / 2.0f;

	//対角線の角度を算出する
	g_fAnglePlayer = atan2f((Xplayer * 2), (Yplayer * 2));

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(g_aVertex),	//必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	
	pVtx[0].pos = D3DXVECTOR3(x - Xplayer, y - Yplayer, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(x + Xplayer , y - Yplayer, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(x - Xplayer, y + Yplayer, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(x + Xplayer, y + Yplayer, 0.0f);
	
	

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

	
	
	//テクスチャ座標の設定	   u     v
	pVtx[0].tex = D3DXVECTOR2(0.4f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.4f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);

	


	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//プレイヤーの終了処理
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//プレイヤーの更新処理
void UpdatePlayer(void)
{
	int MaxPattern = 10;
	float MoveSpeed = 0.5f;		//移動スピード
	float RotSpeed = 0.005f;	//回転スピード
	float DownSpeed = 0.07f;	//スピード減衰係数

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//Aキーが押された
	if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された
			g_movePlayer.x += sinf(-D3DX_PI * 0.75f + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(-D3DX_PI * 0.75f + g_rotPlayer.z) * MoveSpeed;
			//移動量を更新(増加させる)
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された
			g_movePlayer.x += sinf(-D3DX_PI * 0.25f + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(-D3DX_PI * 0.25f + g_rotPlayer.z) * MoveSpeed;
			//移動量を更新(増加させる)
		}
		else
		{//Aキーが押された
		 //移動量を更新(増加させる)
			g_movePlayer.x += sinf(-D3DX_PI / 2 + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(-D3DX_PI / 2 + g_rotPlayer.z) * MoveSpeed;
			
			//テクスチャ座標の設定	   u     v
			pVtx[0].tex = D3DXVECTOR2(0.2f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.4f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.2f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);

		}
	}
	
	//Wキーが押された
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された
			g_movePlayer.x += sinf(D3DX_PI * 0.75f + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(D3DX_PI * 0.75f + g_rotPlayer.z) * MoveSpeed;
			//移動量を更新(増加させる)

		}
		else
		{//Wキーが押された
			g_movePlayer.x += sinf(D3DX_PI + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(D3DX_PI + g_rotPlayer.z) * MoveSpeed;
			//移動量を更新(増加させる)
			//テクスチャ座標の設定	   u     v
			pVtx[0].tex = D3DXVECTOR2(0.4f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.4f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
		}
	}

	//Dキーが押された
	else if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された
			g_movePlayer.x += sinf(D3DX_PI * 0.25f + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(D3DX_PI * 0.25f + g_rotPlayer.z) * MoveSpeed;
			//移動量を更新(増加させる)
		}
		else
		{//Dキーが押された
			g_movePlayer.x += sinf(D3DX_PI / 2 + g_rotPlayer.z) * MoveSpeed;
			g_movePlayer.y += cosf(D3DX_PI / 2 + g_rotPlayer.z) * MoveSpeed;
			//移動量を更新(増加させる)
			//テクスチャ座標の設定	   u     v
			pVtx[0].tex = D3DXVECTOR2(0.6f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.8f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.6f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.8f, 1.0f);
		}
	}

	//Sキーが押された
	else if (GetKeyboardPress(DIK_S) == true)
	{		
		g_movePlayer.x += sinf(0.0f + g_rotPlayer.z) * MoveSpeed;
		g_movePlayer.y += cosf(0.0f + g_rotPlayer.z) * MoveSpeed;
		//移動量を更新(増加させる)
		//テクスチャ座標の設定	   u     v
		pVtx[0].tex = D3DXVECTOR2(0.4f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.4f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
	}

	//SPACEキーが押された
	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		//弾の設定
			/*SetBullet(g_posPlayer, g_moveBullet);
			SetBullet(g_posPlayer, D3DXVECTOR3(-10.0f, -10.0f, 0.0f) );
			SetBullet(g_posPlayer, D3DXVECTOR3(10.0f, -10.0f, 0.0f) );*/
			SetBullet(g_posPlayer, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 8.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * 8.0f, cosf(D3DX_PI * 0.75f) * 8.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * 8.0f, cosf(-D3DX_PI * 0.75f) * 8.0f, 0.0f));
			/*SetBullet(g_posPlayer, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 10.0f, 0.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 10.0f, 0.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(0.0f, cosf(0.0f) * 10.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * 10.0f, cosf(-D3DX_PI * 0.25f) * 10.0f, 0.0f));
			SetBullet(g_posPlayer, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 10.0f, cosf(D3DX_PI * 0.25f) * 10.0f, 0.0f));*/
	}


	//位置を更新
	g_posPlayer.x += g_movePlayer.x;
	g_posPlayer.y += g_movePlayer.y;

	//移動量を更新(減衰させる)
	g_movePlayer.x += (0.0f - g_movePlayer.x) * DownSpeed;
	g_movePlayer.y += (0.0f - g_movePlayer.y) * DownSpeed;

	if (g_posPlayer.x <= GAME_WIDTH + Xplayer)
	{//始点(Y座標)が画面の左端に当たった
		g_posPlayer.x = GAME_WIDTH + Xplayer;					//始点を画面の左端に設定す
	}
	if (g_posPlayer.y <= nY)
	{//始点(Y座標)が画面の上端に当たった
		g_posPlayer.y = nY;					//始点を画面の上端に設定す
	}
	if (g_posPlayer.y >= GAME_HEIGHT - nY)
	{//始点(Y座標)が画面の下端に当たった
		g_posPlayer.y = GAME_HEIGHT - nY;					//始点を画面の下端に設定す
	}
	if (g_posPlayer.x >= GAME_WIDTH * 3 - hX)
	{//始点(Y座標)が画面の右端に当たった
		g_posPlayer.x = GAME_WIDTH * 3 - hX;					//始点を画面の右端に設定す
	}


	////頂点座標の更新
	//pVtx[0].pos.x = g_posPlayer.x - hX;
	//pVtx[0].pos.y = g_posPlayer.y - nY;

	//pVtx[1].pos.x = g_posPlayer.x + hX;
	//pVtx[1].pos.y = g_posPlayer.y - nY;

	//pVtx[2].pos.x = g_posPlayer.x - hX;
	//pVtx[2].pos.y = g_posPlayer.y + nY;

	//pVtx[3].pos.x = g_posPlayer.x + hX;
	//pVtx[3].pos.y = g_posPlayer.y + nY;
	
	if (GetKeyboardPress(DIK_LEFT) == true)
	{//←キーが押された
	 //移動量を更新 (増加させる)
		g_movePlayer.z += tanf(-D3DX_PI * g_fAnglePlayer) * RotSpeed;
	}

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{//→キーが押された
		g_movePlayer.z += tanf(D3DX_PI * g_fAnglePlayer) * RotSpeed;
	}

	//向きを更新
	g_rotPlayer.z += g_movePlayer.z;

	//移動量を更新 (減衰させる)
	g_movePlayer.z += (0.0f - g_movePlayer.z) * DownSpeed;

	if (GetKeyboardPress(DIK_UP) == true)
	{//↑キーが押された
		if (g_fLengthPlayer <= 100)
		{
			g_fmoveLength += tanf(-D3DX_PI * g_fAnglePlayer) * 0.05f;
		}
	}

	if (GetKeyboardPress(DIK_DOWN) == true)
	{//↓キーが押された
		if (g_fLengthPlayer >= 50)
		{
			g_fmoveLength += tanf(D3DX_PI * g_fAnglePlayer) * 0.05f;
		}
	}
	//拡大・縮小を更新
	g_fLengthPlayer += g_fmoveLength;

	//増加量を更新 (減衰させる)
	g_fmoveLength += (0.0f - g_fmoveLength) * DownSpeed;
	
	//頂点座標の更新
	pVtx[0].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;



	if (g_Player1.bUse == true)
	{//弾が使用されている
		Enemy *pEnemy;		//敵の情報へのポインタ
		int nCountEnemy;

		//敵の取得
		pEnemy = GetEnemy();

		for (nCountEnemy = 0; nCountEnemy < MAX_ENEMY; nCountEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{//敵が使用されている
				if (g_posPlayer.x - Xplayer >= pEnemy->pos.x - pEnemy->Xenemy&&
					g_posPlayer.x + Xplayer <= pEnemy->pos.x + pEnemy->Xenemy&&
					g_posPlayer.y - Yplayer >= pEnemy->pos.y - pEnemy->Yenemy&&
					g_posPlayer.y + Yplayer <= pEnemy->pos.y + pEnemy->Yenemy)
				{
					g_Player1.nLife--;
				}
			}
		}
}
	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
	//g_nCounterAnimPlayer++;		//カウンターを加算
	//if ((g_nCounterAnimPlayer%6)==0)
	//{//一定時間経過
	//	
	//	g_nCounterAnimPlayer = 0;		//カウンターを初期値に戻す	

	//	if (g_nPatternAnimPlayer > MaxPattern)
	//	{
	//		g_nCounterAnimPlayer = 0;		//パターンNo.を初期値に戻す
	//	}

	//	//頂点バッファをロックし、頂点情報へのポインタを取得
	//	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//	//テクスチャ座標の更新
	//	float u=0.2f;
	//	float v=0.5f;
	//	int Pattern=5;
	//	pVtx[0].tex = D3DXVECTOR2((g_nPatternAnimPlayer % Pattern * u), (g_nPatternAnimPlayer/ Pattern) * v);
	//	pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimPlayer % Pattern * u + u, (g_nPatternAnimPlayer / Pattern) * v);
	//	pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimPlayer % Pattern * u, (g_nPatternAnimPlayer / Pattern) * v + v);
	//	pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimPlayer % Pattern * u + u, (g_nPatternAnimPlayer / Pattern) * v + v);
	//	
	//	//位置を更新する
	//	//g_posPlayer.x -= g_movePlaye.x;
	//	//g_posPlayer.y -= g_movePlaye.y;
	//	//if (g_posPlayer.x >= SCREEN_WIDTH)
	//	//{//始点(x座標)が画面の右端に当たった
	//	//	g_posPlayer.x = SCREEN_WIDTH;		//始点を画面の右端に設定する
	//	//	g_movePlaye.x *= -1;				//移動方向を逆転する
	//	//}
	//	//else if (g_posPlayer.x <= 0)
	//	//{//始点(x座標)が画面の左端に当たった
	//	//	g_posPlayer.x = 0;					//始点を画面の左端に設定する
	//	//	g_movePlaye.x *= -1;				//移動方向を逆転する
	//	//}

	//	////頂点座標の更新

	//	//pVtx[0].pos = D3DXVECTOR3(g_posPlayer.x , g_posPlayer.y, 0.0f);
	//	//pVtx[1].pos = D3DXVECTOR3(g_posPlayer.x + hX, g_posPlayer.y, 0.0f);
	//	//pVtx[2].pos = D3DXVECTOR3(g_posPlayer.x, g_posPlayer.y + nY, 0.0f);
	//	//pVtx[3].pos = D3DXVECTOR3(g_posPlayer.x + hX, g_posPlayer.y + nY, 0.0f);

	//	//パターンNo.を更新する
	//	g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % MaxPattern;
	//}
	////頂点バッファをアンロックする
	//g_pVtxBuffPlayer->Unlock();
}

//ポリゴンの描画処理
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));
	

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);
	

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		//D3DPT_TRIANGLELIST,		//プリミティブの種類 三角形
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類 四角形
		0,
		2);							//描画するプリミティブ(ポリゴン)数
			
}

//プレイヤーの取得
Player *GetPlayer(void)
{
	return &g_Player1;
}