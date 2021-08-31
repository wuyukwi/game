//--------------------------------------------------
// DirectXの基本処理( main.cpp )
// Author: Huang QiYue
//--------------------------------------------------
#include"main.h"
#include"input.h"
#include"game.h"
#include"title.h"
#include"result.h"
//-------------------------
//マクロ定義
//-------------------------
#define ID_TIMER				(101)				//タイマーのID
#define TIMER_INTERVAL		(1000 / 60)				//タイマーの発生間隔 ( ミリ秒 )
#define CLASS_NAME		"WindowClass"			//ウィンドウクラスの名前
#define WINDOW_NAME		"DirectXの基本処理"		//ウィンドウの名前


//-------------------------
//プロトタイプ宣言
//-------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//-------------------------
//グローバル変数
//-------------------------
LPDIRECT3D9				g_pD3D = NULL;					//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;			//Direct3Dデバイスへのポインタ
LPDIRECTSOUND8           g_SoundInterface;									//サウンドデバイス

MODE g_mode = MODE_RESULT;		//現在のモード

//-------------------------
//メイン関数
//-------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{//ウインドウクラスの構造体
		sizeof(WNDCLASSEX),						//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								//ウインドウのスタイル
		WindowProc,								//ウインドウプロシージャ
		0,										//０にする (通常は使用しない)
		0,										//０にする (通常は使用しない)
		hInstance,								//インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				//クライアント領域の背景色
		NULL,									//メニューバー
		CLASS_NAME,								//ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)			//ファイルのアイコン
	};

	HWND hWnd;												//ウインドウハンドル (識別子)
	MSG msg;												//メッセージを格納する変数
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		//画面サイズの構造体
	DWORD dwCurrentTime;		//現在時刻
	DWORD dwExecLastTime;		//最後に処理した時刻

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを作成
	hWnd = CreateWindowEx(
		0,								//拡張ウインドウスタイル
		CLASS_NAME,						//ウインドウスタイルの名前
		WINDOW_NAME,					//ウインドウの名前
		WS_OVERLAPPEDWINDOW,			//ウインドウスタイル
		CW_USEDEFAULT,					//ウインドウの左上X座標
		CW_USEDEFAULT,					//　　〃　　の左上Y座標
		(rect.right - rect.left),		//　　〃　　の幅
		(rect.bottom - rect.top),		//　　〃　　の高さ
		NULL,							//親ウインドウのハンドル
		NULL,							//メニューハンドルまたは子ウインドウID
		hInstance,						//インスタンスハンドル
		NULL);							//ウインドウ作成データ

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合

		return -1;
	}
	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;		//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得(保存)

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);		//ウインドウの表示状態を設定
	UpdateWindow(hWnd);				//クライアント領域を更新

	//メッセージループ
	while (1)
	{	//メッセージキューからメッセージを取得　メッセージを取得しなかった場合０を返す
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループをぬける

				break;
			}
			else
			{	//メッセージの設定
				TranslateMessage(&msg);		//仮想メッセージを文字のメッセージへ変換
				DispatchMessage(&msg);		//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			
			dwCurrentTime = timeGetTime();		//現在時刻を取得
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の一秒経過
				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存
				
				//更新処理
				Update();

				//描画処理
				Draw();
			}
			
		}
	}
	
	//終了処理
	Uninit();
	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;

}

//初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)//TRUE：ウィンドウ/FALSE:フルスクリーン
{
	D3DDISPLAYMODE d3ddm;		//ディスプレーモード
	D3DPRESENT_PARAMETERS d3dpp;//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレーモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));		//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;	//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;	//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;	//バックバッファの形式
	d3dpp.BackBufferCount = 1;				//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;	//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;//FALSE;		//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3Dデバイスの生成(描画処理、頂点処理はCPUで行う)
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//レンダーステートの設定					//カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//
	

	// DirectSoundの生成
	if (FAILED(DirectSoundCreate8(
		NULL,			// GUID
		&g_SoundInterface,	// 生成したDirectSound保存先
		NULL)))			// NULL固定
	{
		return false;
	}
	// 協調レベルの設定
	if (FAILED(g_SoundInterface->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),		// ウィンドウハンドル
		DSSCL_NORMAL)))					// 協調レベル
	{
		return false;
	}

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	//モードの設定
	SetMode(g_mode);

	//各種オブジェクトの初期化処理
	return S_OK;

}


//終了処理
void Uninit(void)
{

	//各種オブジェクトの終了処理

	//タイトル画面の終了処理
	UninitTitle();

	//ゲーム画面の終了処理
	UninitGame();

	//リサイクル画面の終了処理
	UninitResult();

	//キーボードの終了処理
	UninitKeyboard();

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	
}

//更新処理
void Update(void)
{

	//各種オブジェクトの更新処理
	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_GAME:
		UpdateGame();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;
	default:
		break;
	}
	//キーボードの更新処理
	UpdateKeyboard();

}

//描画処理
void Draw(void)
{
	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//各種オブジェクトの描画処理

		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;
		case MODE_GAME:
			DrawGame();
			break;
		case MODE_RESULT:
			DrawResult();
			break;
		default:
			break;
		}

		

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//モードの設定
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;
	case MODE_GAME:
		UninitGame();
		break;
	case MODE_RESULT:
		UninitResult();
		break;
	default:
		break;
	}

	//現在の画面(モード)の終了処理
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;
	case MODE_GAME:
		InitGame();
		break;
	case MODE_RESULT:
		InitResult();
		break;
	default:
		break;
	}

	g_mode = mode;
}

//モードの取得
MODE GetMode(void)
{
	return g_mode;
}

//-------------------------
//ウインドウプロシージャ
//-------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;			//返り値を格納

	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };				//ウインドウの領域 (短形)

	switch (uMsg)
	{
	case WM_DESTROY:		//ウインドウ破棄のメッセージ
		//WM_QOITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_CREATE:			//ウインドウ生成のメッセージ
		//タイマーの設定 ( WM_TIMERメッセージを定期的に送る )
		SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, NULL);
		break;

	case WM_CLOSE:		//閉じるボタン押下のメッセージ

		nID = MessageBox(hWnd, "終了する？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

		if (nID == IDYES)
		{//YESを押された
		 //ウインドウを破棄する (WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0を返さないと終了してしまう
		}
		break;

	case WM_KEYDOWN:		//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		//[ESC]キーが押された

			nID = MessageBox(hWnd, "終了する？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{//YESを押された
			 //ウインドウを破棄する (WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;


	case WM_LBUTTONDOWN:		//マウス右クリックのメッセージ
								//ウインドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;
	}

return DefWindowProc(hWnd, uMsg, wParam, lParam);		//規定の処理を返す

}

//デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//デバイスの取得
LPDIRECT3DDEVICE9 GetSoundDevice(void)
{
	return g_pD3DDevice;
}