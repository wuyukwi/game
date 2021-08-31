//--------------------------------------------------
// DirectX�̊�{����( main.cpp )
// Author: Huang QiYue
//--------------------------------------------------
#include"main.h"
#include"input.h"
#include"game.h"
#include"title.h"
#include"result.h"
//-------------------------
//�}�N����`
//-------------------------
#define ID_TIMER				(101)				//�^�C�}�[��ID
#define TIMER_INTERVAL		(1000 / 60)				//�^�C�}�[�̔����Ԋu ( �~���b )
#define CLASS_NAME		"WindowClass"			//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME		"DirectX�̊�{����"		//�E�B���h�E�̖��O


//-------------------------
//�v���g�^�C�v�錾
//-------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//-------------------------
//�O���[�o���ϐ�
//-------------------------
LPDIRECT3D9				g_pD3D = NULL;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;			//Direct3D�f�o�C�X�ւ̃|�C���^
LPDIRECTSOUND8           g_SoundInterface;									//�T�E���h�f�o�C�X

MODE g_mode = MODE_RESULT;		//���݂̃��[�h

//-------------------------
//���C���֐�
//-------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{//�E�C���h�E�N���X�̍\����
		sizeof(WNDCLASSEX),						//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,								//�E�C���h�E�̃X�^�C��
		WindowProc,								//�E�C���h�E�v���V�[�W��
		0,										//�O�ɂ��� (�ʏ�͎g�p���Ȃ�)
		0,										//�O�ɂ��� (�ʏ�͎g�p���Ȃ�)
		hInstance,								//�C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),			//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),				//�N���C�A���g�̈�̔w�i�F
		NULL,									//���j���[�o�[
		CLASS_NAME,								//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION)			//�t�@�C���̃A�C�R��
	};

	HWND hWnd;												//�E�C���h�E�n���h�� (���ʎq)
	MSG msg;												//���b�Z�[�W���i�[����ϐ�
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		//��ʃT�C�Y�̍\����
	DWORD dwCurrentTime;		//���ݎ���
	DWORD dwExecLastTime;		//�Ō�ɏ�����������

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E���쐬
	hWnd = CreateWindowEx(
		0,								//�g���E�C���h�E�X�^�C��
		CLASS_NAME,						//�E�C���h�E�X�^�C���̖��O
		WINDOW_NAME,					//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,			//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,					//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,					//�@�@�V�@�@�̍���Y���W
		(rect.right - rect.left),		//�@�@�V�@�@�̕�
		(rect.bottom - rect.top),		//�@�@�V�@�@�̍���
		NULL,							//�e�E�C���h�E�̃n���h��
		NULL,							//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,						//�C���X�^���X�n���h��
		NULL);							//�E�C���h�E�쐬�f�[�^

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ

		return -1;
	}
	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;		//����������
	dwExecLastTime = timeGetTime();		//���ݎ������擾(�ۑ�)

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);		//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);				//�N���C�A���g�̈���X�V

	//���b�Z�[�W���[�v
	while (1)
	{	//���b�Z�[�W�L���[���烁�b�Z�[�W���擾�@���b�Z�[�W���擾���Ȃ������ꍇ�O��Ԃ�
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v���ʂ���

				break;
			}
			else
			{	//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		//���z���b�Z�[�W�𕶎��̃��b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);		//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			
			dwCurrentTime = timeGetTime();		//���ݎ������擾
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60���̈�b�o��
				dwExecLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�
				
				//�X�V����
				Update();

				//�`�揈��
				Draw();
			}
			
		}
	}
	
	//�I������
	Uninit();
	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;

}

//����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)//TRUE�F�E�B���h�E/FALSE:�t���X�N���[��
{
	D3DDISPLAYMODE d3ddm;		//�f�B�X�v���[���[�h
	D3DPRESENT_PARAMETERS d3dpp;//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���[���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));		//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;	//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;	//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;	//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;				//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;	//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;//FALSE;		//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs��)
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐���(�`�揈���A���_������CPU�ōs��)
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
	//�����_�[�X�e�[�g�̐ݒ�					//�J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//�A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//
	

	// DirectSound�̐���
	if (FAILED(DirectSoundCreate8(
		NULL,			// GUID
		&g_SoundInterface,	// ��������DirectSound�ۑ���
		NULL)))			// NULL�Œ�
	{
		return false;
	}
	// �������x���̐ݒ�
	if (FAILED(g_SoundInterface->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),		// �E�B���h�E�n���h��
		DSSCL_NORMAL)))					// �������x��
	{
		return false;
	}

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	//���[�h�̐ݒ�
	SetMode(g_mode);

	//�e��I�u�W�F�N�g�̏���������
	return S_OK;

}


//�I������
void Uninit(void)
{

	//�e��I�u�W�F�N�g�̏I������

	//�^�C�g����ʂ̏I������
	UninitTitle();

	//�Q�[����ʂ̏I������
	UninitGame();

	//���T�C�N����ʂ̏I������
	UninitResult();

	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	
}

//�X�V����
void Update(void)
{

	//�e��I�u�W�F�N�g�̍X�V����
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
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

}

//�`�揈��
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		//�e��I�u�W�F�N�g�̕`�揈��

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

		

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//���[�h�̐ݒ�
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
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

	//���݂̉��(���[�h)�̏I������
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

//���[�h�̎擾
MODE GetMode(void)
{
	return g_mode;
}

//-------------------------
//�E�C���h�E�v���V�[�W��
//-------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;			//�Ԃ�l���i�[

	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };				//�E�C���h�E�̗̈� (�Z�`)

	switch (uMsg)
	{
	case WM_DESTROY:		//�E�C���h�E�j���̃��b�Z�[�W
		//WM_QOIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_CREATE:			//�E�C���h�E�����̃��b�Z�[�W
		//�^�C�}�[�̐ݒ� ( WM_TIMER���b�Z�[�W�����I�ɑ��� )
		SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, NULL);
		break;

	case WM_CLOSE:		//����{�^�������̃��b�Z�[�W

		nID = MessageBox(hWnd, "�I������H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);

		if (nID == IDYES)
		{//YES�������ꂽ
		 //�E�C���h�E��j������ (WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;

	case WM_KEYDOWN:		//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:		//[ESC]�L�[�������ꂽ

			nID = MessageBox(hWnd, "�I������H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{//YES�������ꂽ
			 //�E�C���h�E��j������ (WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;


	case WM_LBUTTONDOWN:		//�}�E�X�E�N���b�N�̃��b�Z�[�W
								//�E�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd);
		break;
	}

return DefWindowProc(hWnd, uMsg, wParam, lParam);		//�K��̏�����Ԃ�

}

//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetSoundDevice(void)
{
	return g_pD3DDevice;
}