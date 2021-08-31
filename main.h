//DirectX�̏����ݒ�
#ifndef _MAIN_H_		//���̃}�N����`������Ȃ�������

#include<Windows.h>
#include "d3dx9.h"		//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)		//�r���h�Ƃ��̌x���Ή��p�}�O��
#include "dinput.h"						//���͏����ɕK�v
#include <dsound.h>

//-------------------------
//���C�u�����̃����N
//-------------------------
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����	
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v
#pragma comment ( lib, "dxguid.lib" )//GUID�iGlobal Unique ID�j���`���Ă��郉�C�u����
#pragma comment ( lib, "dsound.lib" )//Direct Sound�̖{�̂̃��C�u����

//-------------------------
//�}�N����`
//-------------------------
#define _MAIN_H_		//��d�C���N���[�h�h�~�̃}�N����`

#define SCREEN_WIDTH			(1280)				//�E�C���h�E�̕�
#define SCREEN_HEIGHT			(720)				//�E�C���h�E�̍���

#define GAME_WIDTH			(320)				//�Q�[���E�C���h�E�̕�
#define GAME_HEIGHT			(720)				//�Q�[���E�C���h�E�̍���

								//���_���W[2D]	���_�J���[	�e�N�X�`�����W
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)		//���_�t�H�[�}�b�g[2D]

typedef enum
{
	MODE_TITLE = 0,	//�^�C�g�����
	MODE_GAME,		//�^�C�g�����
	MODE_RESULT,	//�^�C�g�����
	MODE_MAX
}MODE;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
IDirectSound8 *pDS8;		//�T�E���h�f�o�C�X
void SetMode(MODE mode);
MODE GetMode(void);

//-------------------------
//���_���[2D]�̍\���̂��`
//-------------------------
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	float rhw;				//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
}VERTEX_2D;


//DSBUFFERDESC�\����
typedef struct {

	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwBufferBytes;
	DWORD dwReserved;
	LPWAVEFORMATEX lpwfxFormat;
	GUID guid3DAlgorithm;

} DSBUFFERDESC, *LPDSBUFFERDESC;


//WAVEFORMATEX�\����
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