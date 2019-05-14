#include "main.h"
#include "JoyStick.h"
#include "debug.h"
#include <math.h>

#define DIDEVICE_BUFFERSIZE       	    (100)			// デバイスに設定するバッファ・サイズ
#define NUM_KEY_MAX                     (32)
#define NUM_POV_MAX                     (4)
#define	LIMIT_COUNT_REPEAT	            (30)	// リピートカウントリミッター

// グローバル変数
static LPDIRECTINPUT8 g_pInput = NULL;
static LPDIRECTINPUTDEVICE8 g_pDevJoyStick = NULL;
static DIDEVCAPS g_diDevCaps;
static 	DIJOYSTATE2 js;

static BYTE					g_aJoyState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
static BYTE					g_aJoyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報ワーク
static BYTE					g_aJoyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報ワーク
static BYTE					g_aJoyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート情報ワーク
static int					g_aJoyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ
static int 					g_nPOV = -1;
static BYTE                 g_bPOVStateOld[NUM_POV_MAX];
static BYTE                 g_bPOVState[NUM_POV_MAX];
static BYTE                 g_bPOVTrigger[NUM_POV_MAX];
static BYTE                 g_bPOVRelease[NUM_POV_MAX];
static BYTE                 g_bPOVRepeat[NUM_POV_MAX];
static int					g_bPOVRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

// プロトタイプ宣言
HRESULT InitJSInput(HINSTANCE hInstance, HWND hWnd);


HRESULT InitJSInput(HINSTANCE hInstance, HWND hWnd)
{
	hWnd = hWnd;
	if (g_pInput == NULL)
	{
		// DirectInputオブジェクトの作成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT InitJoyStick(HINSTANCE hInstance, HWND hWnd)
{
	InitJSInput(hInstance, hWnd);

	g_pDevJoyStick = NULL;

	if (FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoySticksCallback, NULL,
		DIEDFL_ATTACHEDONLY)))
	{
		MessageBox(hWnd, "JOYPADがありません！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}
	if (g_pDevJoyStick != NULL)
	{
		if (FAILED(g_pDevJoyStick->SetDataFormat(&c_dfDIJoystick2)))
		{
			MessageBox(hWnd, "JOYPADのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}

		if (FAILED(g_pDevJoyStick->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			MessageBox(hWnd, "JOYPADの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}

		if (FAILED(g_pDevJoyStick->EnumObjects(EnumAxesCallback, NULL, DIDFT_AXIS)))
		{
			MessageBox(hWnd, "軸モードの設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}

		//軸モードの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_ABS;

		if (FAILED(g_pDevJoyStick->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
		{
			MessageBox(hWnd, "軸モードの設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}
		// バッファリング・データを取得するため、バッファ・サイズを設定
		diprop.dwData = DIDEVICE_BUFFERSIZE;
		if (FAILED(g_pDevJoyStick->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph)))
		{
			MessageBox(hWnd, "軸モードの設定できませんでした。", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}
		//入力制御開始
		g_pDevJoyStick->Acquire();
	}
	return S_OK;
}

// コールバック関数
BOOL CALLBACK EnumJoySticksCallback(const DIDEVICEINSTANCE *pdidinstance, VOID *pContext)
{
	HRESULT hr;
										 
	hr = g_pInput->CreateDevice(pdidinstance->guidInstance, &g_pDevJoyStick, NULL);

	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;
	hr = g_pDevJoyStick->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

void UninitJoyStick()
{
	if (g_pDevJoyStick != NULL)
	{
		g_pDevJoyStick->Unacquire();
		g_pDevJoyStick->Release();
		g_pDevJoyStick = NULL;
	}
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

void UpdateJoyStick()
{
	if (g_pDevJoyStick != NULL)
	{
		HRESULT hr;

		hr = g_pDevJoyStick->Poll();

		if (FAILED(hr))
		{
			hr = g_pDevJoyStick->Acquire();
			while (hr == DIERR_INPUTLOST)
			{
				hr = g_pDevJoyStick->Acquire();
			}
		}


		if (SUCCEEDED(g_pDevJoyStick->GetDeviceState(sizeof(DIJOYSTATE2), &js)))
		{
			switch (js.rgdwPOV[0])
			{
			case JOY_NUM_UP:
			{
				g_bPOVState[JOY_UP + JOY_MISS] = 0x80;
				g_bPOVState[JOY_DOWN + JOY_MISS] = 0x00;
				g_bPOVState[JOY_LEFT + JOY_MISS] = 0x00;
				g_bPOVState[JOY_RIGHT + JOY_MISS] = 0x00;
				break;
			}
			case JOY_NUM_DOWN:			   
			{
				g_bPOVState[JOY_DOWN + JOY_MISS] = 0x80;
				g_bPOVState[JOY_UP + JOY_MISS] = 0x00;
				g_bPOVState[JOY_LEFT + JOY_MISS] = 0x00;
				g_bPOVState[JOY_RIGHT + JOY_MISS] = 0x00;
				break;
			}
			case JOY_NUM_LEFT:
			{
				g_bPOVState[JOY_LEFT + JOY_MISS] = 0x80;
				g_bPOVState[JOY_DOWN + JOY_MISS] = 0x00;
				g_bPOVState[JOY_UP + JOY_MISS] = 0x00;
				g_bPOVState[JOY_RIGHT + JOY_MISS] = 0x00;
				break;
			}
			case JOY_NUM_RIGHT:
			{
				g_bPOVState[JOY_RIGHT + JOY_MISS] = 0x80;
				g_bPOVState[JOY_DOWN + JOY_MISS] = 0x00;
				g_bPOVState[JOY_LEFT + JOY_MISS] = 0x00;
				g_bPOVState[JOY_UP + JOY_MISS] = 0x00;
				break;
			}
			case JOY_NUM_UP_LEFT:
			{
				g_bPOVState[JOY_UP + JOY_MISS] = 0x80;
				g_bPOVState[JOY_LEFT + JOY_MISS] = 0x80;
				g_bPOVState[JOY_RIGHT + JOY_MISS] = 0x00;
				g_bPOVState[JOY_DOWN + JOY_MISS] = 0x00;
				break;
			}
			case JOY_NUM_UP_RIGHT:
			{
				g_bPOVState[JOY_UP + JOY_MISS] = 0x80;
				g_bPOVState[JOY_RIGHT + JOY_MISS] = 0x80;
				g_bPOVState[JOY_LEFT + JOY_MISS] = 0x00;
				g_bPOVState[JOY_DOWN + JOY_MISS] = 0x00;
				break;
			}
			case JOY_NUM_DOWN_LEFT:
			{
				g_bPOVState[JOY_DOWN + JOY_MISS] = 0x80;
				g_bPOVState[JOY_LEFT + JOY_MISS] = 0x80;
				g_bPOVState[JOY_RIGHT + JOY_MISS] = 0x00;
				g_bPOVState[JOY_UP + JOY_MISS] = 0x00;
				break;
			}
			case JOY_NUM_DOWN_RIGHT:
			{
				g_bPOVState[JOY_DOWN + JOY_MISS] = 0x80;
				g_bPOVState[JOY_RIGHT + JOY_MISS] = 0x80;
				g_bPOVState[JOY_UP + JOY_MISS] = 0x00;
				g_bPOVState[JOY_LEFT + JOY_MISS] = 0x00;
				break;
			}
			case -1:
			{
				for (int nCntKey = 0; nCntKey < NUM_POV_MAX; nCntKey++)
				{
					g_bPOVState[nCntKey] = 0x00;
				}
				break;
			}
			}

			for (int nCnKey = 0; nCnKey < NUM_POV_MAX; nCnKey++)
			{
				// キートリガー・リリース情報を生成
				g_bPOVTrigger[nCnKey] = (g_bPOVStateOld[nCnKey] ^ g_bPOVState[nCnKey]) & g_bPOVState[nCnKey];
				g_bPOVRelease[nCnKey] = (g_bPOVStateOld[nCnKey] ^ g_bPOVState[nCnKey]) & ~g_bPOVState[nCnKey];

				// キーリピート情報を生成
				if (g_bPOVState[nCnKey])
				{
					if (g_bPOVRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
					{
						g_bPOVRepeatCnt[nCnKey]++;
						if (g_bPOVRepeatCnt[nCnKey] == 1
							|| g_bPOVRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
						{// キーを押し始めた最初のフレーム、または一定時間経過したらキーリピート情報ON
							g_bPOVRepeat[nCnKey] = g_bPOVState[nCnKey];
						}
						else
						{
							g_bPOVRepeat[nCnKey] = 0;
						}
					}
				}
				else
				{
					g_bPOVRepeatCnt[nCnKey] = 0;
					g_bPOVRepeat[nCnKey] = 0;
				}
				g_bPOVStateOld[nCnKey] = g_bPOVState[nCnKey];
			}

			for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
			{
				// キートリガー・リリース情報を生成
				g_aJoyStateTrigger[nCnKey] = (g_aJoyState[nCnKey] ^ js.rgbButtons[nCnKey]) & js.rgbButtons[nCnKey];
				g_aJoyStateRelease[nCnKey] = (g_aJoyState[nCnKey] ^ js.rgbButtons[nCnKey]) & ~js.rgbButtons[nCnKey];

				// キーリピート情報を生成
				if (js.rgbButtons[nCnKey])
				{
					if (g_aJoyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
					{
						g_aJoyStateRepeatCnt[nCnKey]++;
						if (g_aJoyStateRepeatCnt[nCnKey] == 1
							|| g_aJoyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
						{// キーを押し始めた最初のフレーム、または一定時間経過したらキーリピート情報ON
							g_aJoyStateRepeat[nCnKey] = js.rgbButtons[nCnKey];
						}
						else
						{
							g_aJoyStateRepeat[nCnKey] = 0;
						}
					}
				}
				else
				{
					g_aJoyStateRepeatCnt[nCnKey] = 0;
					g_aJoyStateRepeat[nCnKey] = 0;
				}  
				g_aJoyState[nCnKey] = js.rgbButtons[nCnKey];
			}
		}
		else
		if (hr == DIERR_INPUTLOST)
		{
			g_pDevJoyStick->Acquire();
		}
	}
}

//=============================================================================
// コントローラーのプレス状態を取得
//=============================================================================
bool GetJoyStickPress(int nKey)
{
	return (g_aJoyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// コントローラーのトリガー状態を取得
//=============================================================================
bool GetJoyStickTrigger(int nKey)
{
	return (g_aJoyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// コントローラーのリピート状態を取得
//=============================================================================
bool GetJoyStickRepeat(int nKey)
{
	return (g_aJoyStateRepeat[nKey] & 0x80) ? true : false;
}

//=============================================================================
// コントローラーのリリ－ス状態を取得
//=============================================================================
bool GetJoyStickRelease(int nKey)
{
	return (g_aJoyStateRelease[nKey] & 0x80) ? true : false;
}

bool GetPOVPress(int nKey)
{
	return (g_bPOVState[nKey + JOY_MISS] & 0x80) ? true : false;
}

bool GetPOVTrigger(int nKey)
{
	return (g_bPOVTrigger[nKey + JOY_MISS] & 0x80) ? true : false;
}

bool GetPOVRelease(int nKey)
{
	return (g_bPOVRelease[nKey + JOY_MISS] & 0x80) ? true : false;
}

bool GetPOVRepeat(int nKey)
{
	return (g_bPOVRepeat[nKey + JOY_MISS] & 0x80) ? true : false;
}

float GetStickLength(int nNum)
{
	float x, y;
	float fLength;
	if (nNum == AS_LEFT)
	{
		x = js.lX * 0.001f;
		y = js.lY * 0.001f;
	}
	if (nNum == AS_RIGHT)
	{
		if (GetTestModeMemberStatus(TMML_XINPUT))
		{
			x = js.lRx * 0.001f;
			y = js.lRy * 0.001f;
		}
		else
		{
			x = js.lZ * 0.001f;
			y = js.lRz * 0.001f;
		}
	}

	fLength = sqrt(x * x + y * y);

	return fLength;
}

float GetStickAngle(int nNum)
{
	float x, y;
	float fAngle = 0;
	float fLength = GetStickLength(nNum);
	if (nNum == AS_LEFT)
	{
		x = js.lX;
		y = js.lY;
	}
	if (nNum == AS_RIGHT)
	{
		if (GetTestModeMemberStatus(TMML_XINPUT))
		{
			x = js.lRx;
			y = js.lRy;
		}
		else
		{
			x = js.lZ;
			y = js.lRz;
		}
	}

	if (x != 0 && y != 0)
	{
		fAngle = x / fLength * 0.001f;
	}

	if (x >= 0 && y < 0)
	{
		fAngle = asinf(fAngle)*180.0f / 3.14f;
	}
	if (y >= 0)
	{
		fAngle = 180.0f - asinf(fAngle)*180.0f / 3.14f;
	}
	if (x < 0 && y < 0)
	{
		fAngle = 360 + asinf(fAngle)*180.0f / 3.14f;
	}

	if (x == 0 && y == 0)
	{
		fAngle = 0;
	}

	if (x == 1000 && y == 0)
	{
		fAngle = 90.0f;
	}
	if (x == -1000 && y == 0)
	{
		fAngle = 270.0f;
	}

	return fAngle;
}

void DrawDebugKey()
{					   
	DebugText(0, 430, "AnalogX: %d", js.lX);
	DebugText(0, 460, "AnalogY: %d", js.lY);
	DebugText(0, 490, "AnalogRightX: %d", js.lZ);
	DebugText(0, 510, "AnalogRightY: %d", js.lRz);
	DebugText(0, 530, "AnalogLeftLength: %.2f", GetStickLength(AS_LEFT));
	DebugText(0, 550, "AnalogRightLength: %.2f", GetStickLength(AS_RIGHT));
	DebugText(0, 570, "ASLeftAngle: %.2f", GetStickAngle(AS_LEFT));
	DebugText(0, 590, "ASRightAngle: %.2f", GetStickAngle(AS_RIGHT));


	for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
	{
		if (GetJoyStickPress(nCnKey))
		{
			DebugText(0, 520, "Key: %d", nCnKey);
		}
		if (GetJoyStickTrigger(nCnKey))
		{
			DebugText(0, 550, "Key: %d", nCnKey);
		}
		if (GetJoyStickRelease(nCnKey))
		{
			DebugText(0, 580, "Key: %d", nCnKey);
		}
		if (GetJoyStickRepeat(nCnKey))
		{
			DebugText(0, 610, "Key: %d", nCnKey);
		}
	}
}

LPDIRECTINPUTDEVICE8 GetJSDev()
{
	return g_pDevJoyStick;
}