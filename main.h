//DirectXの初期設定
#ifndef _MAIN_H_		//このマクロ定義がされなかったら

#include<Windows.h>
#include "d3dx9.h"		//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)		//ビルドときの警告対応用マグロ
#include "dinput.h"						//入力処理に必要
#include <dsound.h>

//-------------------------
//ライブラリのリンク
//-------------------------
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ	
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
#pragma comment ( lib, "dxguid.lib" )//GUID（Global Unique ID）を定義しているライブラリ
#pragma comment ( lib, "dsound.lib" )//Direct Soundの本体のライブラリ

//-------------------------
//マクロ定義
//-------------------------
#define _MAIN_H_		//二重インクルード防止のマクロ定義

#define SCREEN_WIDTH			(1280)				//ウインドウの幅
#define SCREEN_HEIGHT			(720)				//ウインドウの高さ

#define GAME_WIDTH			(320)				//ゲームウインドウの幅
#define GAME_HEIGHT			(720)				//ゲームウインドウの高さ

								//頂点座標[2D]	頂点カラー	テクスチャ座標
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)		//頂点フォーマット[2D]

typedef enum
{
	MODE_TITLE = 0,	//タイトル画面
	MODE_GAME,		//タイトル画面
	MODE_RESULT,	//タイトル画面
	MODE_MAX
}MODE;

//-------------------------
//プロトタイプ宣言
//-------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
IDirectSound8 *pDS8;		//サウンドデバイス
void SetMode(MODE mode);
MODE GetMode(void);

//-------------------------
//頂点情報[2D]の構造体を定義
//-------------------------
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	float rhw;				//座標変換用係数(1.0fで固定)
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
}VERTEX_2D;


//DSBUFFERDESC構造体
typedef struct {

	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwBufferBytes;
	DWORD dwReserved;
	LPWAVEFORMATEX lpwfxFormat;
	GUID guid3DAlgorithm;

} DSBUFFERDESC, *LPDSBUFFERDESC;


//WAVEFORMATEX構造体
typedef struct {

	WORD  wFormatTag;
	WORD  nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD  nBlockAlign;
	WORD  wBitsPerSample;
	WORD  cbSize;

} WAVEFORMATEX;
#endif