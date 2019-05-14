#include "main.h"
#include "Polygon.h"
#include "game.h"
#include "SelectSong.h"
#include "Timer.h"
#include "title.h"
#include "JoyStick.h"
#include "music.h"
#include "sound.h"
#include "Result.h"
#include "Camera.h"
#include "polygon3Dtest.h"
#include "Polygon3D.h"
#include "XModel.h"
#include "Texture.h"
#include "Light.h"
#include "Btn.h"
#include "EditMode.h"
#include "ReadNoteFile.h"
#include "Judge.h"
#include "BillBoard.h"
#include "Setting.h"
#include "TestMode.h"
#include "ReadSongList.h"
#include "SelectSongPanel.h"
#include "SelectSongWaitMember.h"
#include "Demo.h"
#include "Preface.h"
#include"network.h"		//追加
#include"data.h"		//追加
#include "name.h"
#include "GamePreface.h"
#include "Tutorial.h"
#include "Thanks.h"
#include "PlayerEntry.h"	//追加
#include "Continue.h"
#include <process.h>


#define CLASS_NAME      "Sample"
#define WINDOW_NAME     "NeuTrino"

// プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
unsigned WINAPI UpdateGameSP(void *lpx);
void Uninit(void);
void Update(void);
void Draw(void);

void ChangeTestMode();

// グローバル変数
DWORD TimeNow;
DWORD TimeOld;
DWORD TimeFrame;

DWORD thID;
HANDLE g_Handle;
MSG msg;

static LPDIRECT3D9        g_pD3D = NULL;
static LPDIRECT3DDEVICE9  g_pD3DDevice = NULL;
static int g_nGameMode;
static int g_nGameModeTarget;
static int CountFrame;
static float g_fFPS;
static HWND hWnd;
static bool g_bAuto = false;
static bool g_bSetAuto = false;
static bool g_bFullScreen = false;
static bool g_bShowWindow = true;
static bool g_bTestMode = false;
static bool g_bEscGame = false;
static bool g_bEscGameMultiPlay = false;
static bool g_bEscTutorial = false;
static bool g_bChangeTest = false;
static float g_fStartWidth;
static float g_fStartHeight;
static TEXTURE g_tex_network_status;		//追加
static bool g_bMultiPlayCancel;
static bool g_bOnline;

#ifdef _DEBUG
	//LPD3DXFONT	  
#endif

// ウィンドウプロシージャ、ウィンドウに対するメッセージ処理を行う
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
	// ウィンドウが破棄されたとき
	case WM_DESTROY:
		PostQuitMessage(0);	// WM_QUITメッセージをメッセージキューに送る
		return 0;
	//case WM_CLOSE:
	//{
	//	int nID;
	//	nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION);

	//	if (nID == IDYES)
	//	{
	//		DestroyWindow(hWnd);
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//	break;
	//}
	case WM_SETCURSOR:
	{		
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(NULL);
			g_pD3DDevice->ShowCursor(false);

			return true;
		}
		break;
	}
	}
	// デフォルトのメッセージ処理を行う
	return DefWindowProc(hWnd, msg, wParam, lParam);
}  
			 //GetAsyncKeyState('A')
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrewInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex = 
	{
		sizeof(WNDCLASSEX),
		CS_VREDRAW | CS_HREDRAW, 
		WndProc,
		0,
		0, 
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&wcex);

	RECT wr =
	{
		0,
		0,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	};

	RECT dr;

	GetWindowRect(GetDesktopWindow(), &dr);

	g_fStartWidth = (dr.right - wr.right) / 2;
	g_fStartHeight = (dr.bottom - wr.bottom) / 2;

	hWnd = CreateWindowEx
	(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_MAXIMIZEBOX ^ WS_SIZEBOX ^ (WS_OVERLAPPEDWINDOW),
		g_fStartWidth,
		g_fStartHeight,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	if (!Init(hInstance, hWnd, TRUE))
	{
		return -1;
	}

	// 分解能の設定
	timeBeginPeriod(1);

	TimeNow = TimeOld = TimeFrame = timeGetTime();

	while (1)
	{
		if (g_bFullScreen)
		{
			if (g_bShowWindow)
			{
				SetWindowLong(hWnd, GWL_STYLE, (WS_VISIBLE | WS_POPUP));
				ShowWindow(hWnd, SW_MAXIMIZE);
				g_bShowWindow = false;
			}
		}
		else
		{
			if (g_bShowWindow)
			{
				SetWindowLong(hWnd, GWL_STYLE, WS_MAXIMIZEBOX ^ WS_SIZEBOX ^ (WS_OVERLAPPEDWINDOW));
				SetWindowPos(hWnd, HWND_NOTOPMOST, g_fStartWidth, g_fStartHeight, WINDOW_WIDTH, WINDOW_HEIGHT, SWP_SHOWWINDOW);
				ShowWindow(hWnd, nCmdShow);
				g_bShowWindow = false;
			}
		}
		UpdateWindow(hWnd);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセージ処理
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// ゲーム処理
			TimeNow = timeGetTime();

			switch (g_nGameMode)
			{
			case MODE_GAME:
			{
				if (!g_bTestMode)
				{
					ChangeTestMode();
					if (g_bEscGame || g_bEscGameMultiPlay)
					{
						switch (g_nGameMode)
						{
						case MODE_GAME:
						{
							StopMusic(MUSIC);
							SetGameModeTarget(MODE_SELECT_SONG);
							SetFadeWaitFrame(60);
							SetFadeInOut(FIOF_FADEIN);
							//ChangeGameMode();
							break;
						}
						}
						g_bEscGame = false;
						g_bEscGameMultiPlay = false;
						
					}
				}
				break;
			}
			case MODE_TUTORIAL:
			{
				if (!g_bTestMode)
				{
					ChangeTestMode();
					if (g_bEscTutorial)
					{
						switch (g_nGameMode)
						{
						case MODE_TUTORIAL:
						{
							StopMusic(MUSIC);
							SetGameModeTarget(MODE_SELECT_SONG);
							SetFadeWaitFrame(60);
							SetFadeInOut(FIOF_FADEIN);
							//ChangeGameMode();
							break;
						}
						}
						g_bEscTutorial = false;
					}
				}
				break;
			}
			}

			//if ((TimeNow - TimeOld) * 60.0f >= (1000.0f))
			{	
				int n_Time = (TimeNow - TimeOld);
				g_fFPS = (float)CountFrame / ((n_Time)) * 1020.0f;
				CountFrame = 0;
				// 更新
				Update();

				// 描画
				Draw();

				TimeOld = TimeNow;
				CountFrame++;
			}
		}
	}

	timeEndPeriod(1);

	Uninit();

	return (int)msg.wParam;
}

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// Direct3Dオブジェクトの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	D3DDISPLAYMODE d3ddm;

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	FILE *fConfig; 
	int nAnti = 0;
	fConfig = fopen("Data/Config.ini", "r");
	if (!fConfig)
	{
		MessageBox(NULL, "設定ファイルがねぇ！", "エラー", MB_OK);
	}
	else
	{
		fscanf(fConfig, "%d", &nAnti);
		fclose(fConfig);
	}

	// デバイスのプレゼンテーションパラメータ
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = WINDOW_WIDTH;
	d3dpp.BackBufferHeight           = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat           = d3ddm.Format;
	d3dpp.BackBufferCount            = 1;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil     = TRUE;
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;
	d3dpp.Windowed                   = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_ONE/*D3DPRESENT_INTERVAL_DEFAULT*/;
	if (SUCCEEDED(g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8B8G8R8, TRUE, (_D3DMULTISAMPLE_TYPE)nAnti, (DWORD)NULL)))
	{
		d3dpp.MultiSampleType = (_D3DMULTISAMPLE_TYPE)nAnti;
	}

	// DirectXデバイスの作成
	if (FAILED(g_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{
		// 上記の設定が失敗したら
		MessageBox(hWnd, "DirectXの初期化が失敗しました", "エラー", MB_OK);
		return false;
	}

	// レンダーステートの設定					 SRC_RGB * SRC_a + DEST_RGB * (1 - SRC_a)
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	  // 今から描くもの、ポリゴンテクスチャ
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  // すでに描画した   D3DBLEND_INVSRCALPHA

	g_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);  //

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// フィルタリング
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	

	InitTexture();

	if (!InitPolygon())
	{
		MessageBox(hWnd, "テクスチャが読み込めなかった", "エラー", MB_OK);
		return false;
	}

	srand(time(NULL));

	InitKeyboard(hInstance, hWnd);
	InitJoyStick(hInstance, hWnd);
	//UninitJoyStick();	  
	//UninitJoyStick();	  
	//InitJoyStick(hInstance, hWnd);
	
	g_Handle = (HANDLE)_beginthreadex(NULL, 0, &UpdateGameSP, NULL, 0, (unsigned int*)&thID);
	InitDebugText();
	InitCamera();
	//Initpolygon3DTest();
	InitPolygon3D();
	//InitXModel();
	InitSound(hWnd);
	InitLight();
	InitBillboard();
	
	// 読み込みをもっと快適させるため
	InitReadSongList();
	InitAllSongBuffer();
	
	// デモモードを準備するため
	InitSelectSong();
	UninitSelectSong();

	g_tex_network_status = GetTextureStatus(TEXTURE_NUM::TEX_NETWORK_STATUS);	//追加

	g_nGameMode = MODE_PREFACE;
	g_nGameModeTarget = g_nGameMode;
	InitPreface();
	InitFadeInOut();
	InitErrorList();
	CountFrame = 0;
	g_fFPS = 0;

	return true;
}

void Uninit(void)
{
	CloseHandle(g_Handle);
	UninitKeyboard();
	UninitJoyStick();
	UninitPolygon();
	UninitPolygon3D();
	UninitTexture();
	UninitDebugText();
	//Uninitpolygon3DTest();
	//UninitXModel();
	UninitLight();
	UninitSound();
	UninitBillboard();
	UninitFadeInOut();
	UninitErrorList();
	UninitUDP();		//追加

	if (g_pD3DDevice != NULL)
	{
		// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{
		// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

void Update(void)
{
	UpdateFadeInOut();
	if ((g_nGameMode != MODE_GAME && g_nGameMode != MODE_TUTORIAL) || GetEditMode())
	{
		UpdateKeyboard();
		UpdateJoyStick();
		if (!g_bTestMode)
		{

			if (GetKeyboardTrigger(DIK_2))
			{
				if (GetEditMode())
				{
					SetEditMode(false);
				}
			}

			if (GetKeyboardTrigger(DIK_F11))
			{
				if (!g_bShowWindow)
				{
					g_bShowWindow = true;
				}
				if (g_bFullScreen)
				{
					g_bFullScreen = false;
				}
				else
				{
					g_bFullScreen = true;
				}
			}
		}
		ChangeTestMode();
	}
	UpdateCamera();
	//if (!g_bTestMode)
	{
		//ChangeCameraByKey();

		switch (g_nGameMode)
		{
		case MODE_PREFACE:
		{
			UpdatePreface();
			break;
		}
		case MODE_TITLE:
		{
			UpdateTitle();
			break;
		}
		case MODE_DEMO:
		{
			UpdateDemo();
			break;
		}
		case MODE_PLAYER_ENTRY:
		{
			UpdatePlayerEntry();
			break;
		}
		case MODE_SELECT_SONG:
		{
			UpdateSelectSong();
			break;
		}
		case MODE_TUTORIAL:
		{
			UpdateTutorial();
			break;
		}
		case MODE_GAME:
		{
			UpdateGame();
			break;
		}
		case MODE_RESULT:
		{
			UpdateResult();
			break;
		}
		case MODE_CONTINUE:
		{
			UpdateContinue();
			break;
		}
		case MODE_THANKS:
		{
			UpdateThanks();
			break;
		}
		case MODE_TEST:
		{
			UpdateTestMode();
			break;
		}
		}
		//if (GetKeyboardTrigger(DIK_E))
		//{
		//	SetErrorList(0, RDP_END);
		//}
		UpdateErrorList();
	}
}

void Draw(void)
{
	g_pD3DDevice->Clear
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255),
		1.0f,										   // Zバッファのクリア値
		0
	);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
			switch (g_nGameMode)
			{
			case MODE_PREFACE:
			{
				DrawPreface();
				break;
			}
			case MODE_TITLE:
			{
				DrawTitle();
				break;
			}
			case MODE_DEMO:
			{
				DrawDemo();
				break;
			}
			case MODE_PLAYER_ENTRY:
			{
				DrawPlayerEntry();
				break;
			}
			case MODE_SELECT_SONG:
			{
				DrawSelectSong();
				break;
			}
			case MODE_TUTORIAL:
			{
				DrawTutorial();
				break;
			}
			case MODE_GAME:
			{
				DrawGame();
				break;
			}
			case MODE_RESULT:
			{
				DrawResult();
				break;
			}
			case MODE_CONTINUE:
			{
				DrawContinue();
				break;
			}
			case MODE_THANKS:
			{
				DrawThanks();
				break;
			}
			case MODE_TEST:
			{
				DrawTestMode();
				break;
			}
			}
			DrawFadeInOut();
			switch (g_nGameMode)
			{
			case MODE_TITLE:
			case MODE_DEMO:
			case MODE_PLAYER_ENTRY:
			case MODE_SELECT_SONG:
			case MODE_TUTORIAL:
			case MODE_GAME:
			case MODE_RESULT:
			case MODE_THANKS:
			{
				SetZBuffer(FALSE);
				SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				SetTexture(TEXTURE_NUM::TEX_NETWORK_STATUS);
				if (GetTestModeMemberStatus(TMML_NETWORK))
				{
					DrawPolygon(1250.0f, 690.0f, 0, g_tex_network_status.v, g_tex_network_status.u, g_tex_network_status.v, false);
				}
				else
				{
					DrawPolygon(1250.0f, 690.0f, 0, 0, g_tex_network_status.u, g_tex_network_status.v, false);
				}
				SetZBuffer(TRUE);
				break;
			}
			}
			DrawErrorList();
			SetDebugTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			DebugText(0, 0, "FPS:%.3f", g_fFPS);
			DebugText(500, 200, "JoyDev:%d", GetJSDev());
			DrawDebugKey();
		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present
	(
		NULL, NULL, NULL, NULL
	);
}

LPDIRECT3DDEVICE9 GetD3DDevice(void)
{
	// Direct3Dデバイスの取得
	return g_pD3DDevice;
}

void SetMode(int nMode)
{
	g_nGameMode = nMode;
}

int GetGameMode()
{
	return g_nGameMode;
}

HWND GethWnd()
{
	return hWnd;
}

void ChangeTestMode()
{
	if ((GetKeyboardTrigger(DIK_0) || g_bChangeTest) && !GetFadeFlag())
	{
		g_bChangeTest = false;
		if (!g_bTestMode)
		{
			g_bTestMode = true;
			switch (g_nGameMode)
			{
			case MODE_PREFACE:
			{
				UninitPreface();
				break;
			}
			case MODE_TITLE:
			{
				UninitTitle();
				break;
			}
			case MODE_DEMO:
			{
				UninitDemo();
				break;
			}
			case MODE_PLAYER_ENTRY:
			{
				UninitPlayerEntry();
				break;
			}
			case MODE_SELECT_SONG:
			{
				UninitSelectSong();
				break;
			}
			case MODE_TUTORIAL:
			{
				UninitTutorial();
				break;
			}
			case MODE_GAME:
			{
				UninitGame();
				break;
			}
			case MODE_RESULT:
			{
				UninitResult();
				break;
			}
			case MODE_CONTINUE:
			{
				UninitContinue();
				break;
			}
			case MODE_THANKS:
			{
				UninitThanks();
				break;
			}
			}
			ResetData();
			SetMode(MODE_TEST);
			InitTestMode();
		}
		else
		{
			g_bTestMode = false;
			UninitTestMode();
			InitPreface();
			SetMode(MODE_PREFACE);
		}
	}
}

// ゲーム処理(並行)
unsigned WINAPI UpdateGameSP(void *lpx)
{
	while (1)
	{
		if (msg.message != WM_QUIT)
		{
			switch (g_nGameMode)
			{
			case MODE_GAME:
			{
				if (!g_bTestMode)
				{
					if (!GetEditMode())
					{
						if (GetKeyboardDev() != NULL)
						{
							UpdateKeyboard();
						}
						if (GetJSDev() != NULL)
						{
							UpdateJoyStick();
						}
						if (!((GetRoomCreate() && Data::g_player.room_entry) || (GetRoomEntry() && Data::g_player.room_create)))
						{
							if (GetFadeFlag() || GetGamePreface())
							{
								switch (GetGameModeTarget())
								{
								case MODE_GAME:
								{
									SetTimer(0);
									break;
								}
								case MODE_SELECT_SONG:
								{
									break;
								}
								}
							}
							else
							{
								UpdateTimer();
							}
						}
						if (((GetRoomCreate() && Data::g_player.room_entry) || (GetRoomEntry() && Data::g_player.room_create)))
						{
							if (!Data::g_player.init_game || GetFadeFlag() || GetGamePreface() || Data::g_player.game_preface)
							{
								switch (GetGameModeTarget())
								{
								case MODE_GAME:
								{
									SetTimer(0);
									break;
								}
								case MODE_SELECT_SONG:
								{
									break;
								}
								}
							}
							else
							{
								UpdateTimer();
							}
						}
					}
					if (GetTestModeMemberStatus(TMML_AUTO_PLAY))
					{
						if (GetKeyboardTrigger(DIK_1))
						{
							if (GetAutoPlay())
							{
								SetAutoPlay(false);
							}
							else
							{
								SetAutoPlay(true);
							}
						}
					}
					if (GetTestModeMemberStatus(TMML_EDIT_MODE))
					{
						if (GetKeyboardTrigger(DIK_2))
						{
							if (!GetEditMode() && GetTimer() >= GetNoteOffsetFromFile() && ((GetTimer() - GetPlayMusicOffset()) * 1.0f / GetMusicLength(MUSIC)) < 1.0f)
							{
								SetEditMode(true);
							}
						}
					}

					if (!GetAutoPlay())
					{
						if (!GetFadeFlag())
						{
							UpdateBtn();
						}
					}
					else
					{
						if (!GetFadeFlag())
						{
							UpdateBtnAuto();
							if(!g_bSetAuto)
							{
								g_bSetAuto = true;
							}
						}
					}
					if (GetKeyboardTrigger(DIK_F11))
					{
						if (!g_bShowWindow)
						{
							g_bShowWindow = true;
						}
						if (g_bFullScreen)
						{
							g_bFullScreen = false;
						}
						else
						{
							g_bFullScreen = true;
						}
					}

					if (!(GetRoomCreate() && Data::g_player.room_entry) && !(Data::g_player.room_create && GetRoomEntry()))
					{
						if (GetKeyboardTrigger(DIK_ESCAPE) || GetJoyStickTrigger(JOY_PS))
						{
							switch (g_nGameMode)
							{
							case MODE_GAME:
							{
								if (!g_bEscGame && !GetFadeFlag() && !((GetRoomCreate() && Data::g_player.room_entry) || (GetRoomEntry() && Data::g_player.room_create)))
								{
									g_bEscGame = true;
								}
								break;
							}
							}
						}
					}
					else
					if ((GetRoomCreate() && Data::g_player.room_entry) || (Data::g_player.room_create && GetRoomEntry()))
					{
						if (!g_bOnline)
						{
							g_bOnline = true;
						}
						if (GetKeyboardTrigger(DIK_ESCAPE) || GetJoyStickTrigger(JOY_PS))
						{
							if (!g_bMultiPlayCancel)
							{
								g_bMultiPlayCancel = true;
							}
							//else
							//{
							//	g_bMultiPlayCancel = false;
							//}
						}
					}
					if (g_bMultiPlayCancel && Data::g_player.multiplay_cancel)
					{
						switch (g_nGameMode)
						{
						case MODE_GAME:
						{
							if (!g_bEscGameMultiPlay && !GetFadeFlag())
							{
								g_bEscGameMultiPlay = true;
							}
							break;
						}
						}
					}
				}
				if (GetKeyboardTrigger(DIK_0))
				{
					if (!g_bChangeTest && !GetFadeFlag())
					{
						g_bChangeTest = true;
					}
				}
				break;
			}
			case MODE_TUTORIAL:
			{
				if (!g_bTestMode)
				{
					if (!GetEditMode())
					{
						if (GetKeyboardDev() != NULL)
						{
							UpdateKeyboard();
						}
						if (GetJSDev() != NULL)
						{
							UpdateJoyStick();
						}
						if (GetFadeFlag() || GetGamePreface())
						{
							switch (GetGameModeTarget())
							{
							case MODE_TUTORIAL:
							{
								SetTimer(0);
								break;
							}
							case MODE_SELECT_SONG:
							{
								break;
							}
							}
						}
						else
						{
							UpdateTimer();
						}
					}
					if (!GetAutoPlay())
					{
						if (!GetFadeFlag())
						{
							UpdateBtn();
						}
					}
					else
					{
						if (!GetFadeFlag())
						{
							UpdateBtnAuto();
						}
					}
					if (GetKeyboardTrigger(DIK_F11))
					{
						if (!g_bShowWindow)
						{
							g_bShowWindow = true;
						}
						if (g_bFullScreen)
						{
							g_bFullScreen = false;
						}
						else
						{
							g_bFullScreen = true;
						}
					}

					if (GetKeyboardTrigger(DIK_T) || GetJoyStickTrigger(JOY_OPTION))
					{
						switch (g_nGameMode)
						{
						case MODE_TUTORIAL:
						{
							if (!g_bEscTutorial && !GetFadeFlag() && !((GetRoomCreate() && Data::g_player.room_entry) || (GetRoomEntry() && Data::g_player.room_create)))
							{
								g_bEscTutorial = true;
							}
							break;
						}
						}

					}
				}
				if (GetKeyboardTrigger(DIK_0))
				{
					if (!g_bChangeTest && !GetFadeFlag())
					{
						g_bChangeTest = true;
					}
				}
				break;
			}
			case MODE_DEMO:
			{
				if (GetFadeFlag() || GetGamePreface())
				{
					SetTimer(0);
				}
				UpdateBtnAuto();
				break;
			}
			}
			Sleep(1);
		}
	}
	return 0;
}

void SetGameModeTarget(int nMode)
{
	g_nGameModeTarget = nMode;
}

int GetGameModeTarget()
{
	return g_nGameModeTarget;
}

void ChangeGameMode()
{
	if (g_nGameMode != g_nGameModeTarget)
	{
		switch (g_nGameMode)
		{
		case MODE_PREFACE:
		{
			UninitPreface();
			break;
		}
		case MODE_TITLE:
		{
			UninitTitle();
			break;
		}
		case MODE_DEMO:
		{
			UninitDemo();
			break;
		}
		case MODE_PLAYER_ENTRY:
		{
			UninitPlayerEntry();
			break;
		}
		case MODE_SELECT_SONG:
		{
			UninitSelectSong();
			break;
		}
		case MODE_TUTORIAL:
		{
			UninitTutorial();
			break;
		}
		case MODE_GAME:
		{
			UninitGame();
			break;
		}
		case MODE_RESULT:
		{
			UninitResult();
			break;
		}
		case MODE_CONTINUE:
		{
			UninitContinue();
			break;
		}
		case MODE_THANKS:
		{
			UninitThanks();
			break;
		}
		}

		switch (g_nGameModeTarget)
		{
		case MODE_PREFACE:
		{
			InitPreface();
			break;
		}
		case MODE_TITLE:
		{
			InitTitle();
			break;
		}
		case MODE_DEMO:
		{
			InitDemo();
			break;
		}
		case MODE_PLAYER_ENTRY:
		{
			InitPlayerEntry();
			break;
		}
		case MODE_SELECT_SONG:
		{
			InitSelectSong();
			break;
		}
		case MODE_TUTORIAL:
		{
			InitTutorial();
			break;
		}
		case MODE_GAME:
		{
			g_bMultiPlayCancel = false;
			g_bOnline = false;
			g_bSetAuto = false;
			InitGame();
			break;
		}
		case MODE_RESULT:
		{
			InitResult();
			break;
		}
		case MODE_CONTINUE:
		{
			InitContinue();
			break;
		}
		case MODE_THANKS:
		{
			InitThanks();
			break;
		}
		}
		g_nGameMode = g_nGameModeTarget;
		SetMode(g_nGameMode);
	}
}

bool GetMultiPlayCancel()
{
	return g_bMultiPlayCancel;
}

bool GetOnline()
{
	return g_bOnline;
}

bool GetSetAuto()
{
	return g_bSetAuto;
}