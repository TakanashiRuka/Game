#include "main.h"
#include <stdio.h>
#include "debug.h"

#define MAX_FRAME               (8)

#ifdef _DEBUG
static LPD3DXFONT g_pFont = NULL;
#endif

static LPD3DXFONT g_pText[F_MAX] = { NULL };
static D3DCOLOR g_FontColor = D3DCOLOR_ARGB(255, 255, 255, 255);
static D3DCOLOR g_FontOutColor = D3DCOLOR_ARGB(255, 255, 255, 255);
static D3DCOLOR g_DebugTextColor = D3DCOLOR_ARGB(255, 255, 255, 255);
static int g_FrameSize = 0;
static int g_nFontSizeOld;
static int g_nTitleSizeOld;
static int g_nTextStyle = DT_LEFT;

void InitDebugText()
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if (pDevice == NULL)
	{
		return;
	}

	D3DXCreateFont
	(
		pDevice,
		22,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"MS コジック",
		&g_pFont
	);
#endif
}

void InitText(int nNumFont, int nFontSize, const char* pFormat)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if (pDevice == NULL)
	{
		return;
	}

	D3DXCreateFont
	(
		pDevice,
		nFontSize,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,		//ANTIALIASED_QUALITY
		DEFAULT_PITCH,
		pFormat,
		&g_pText[nNumFont]
	);
}

void UninitDebugText()
{
#ifdef _DEBUG
	if (g_pFont)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
#endif
}

void UninitText(int nNumFont)
{
	if (g_pText[nNumFont])
	{
		g_pText[nNumFont]->Release();
		g_pText[nNumFont] = NULL;
	}
}

void SetText(int nNumFont, int nSize, const char* pFormat)
{
	if (g_nFontSizeOld != nSize)
	{
		g_nFontSizeOld = nSize;

		UninitText(nNumFont);
		InitText(nNumFont, nSize, pFormat);
	}
}

void SetDebugTextColor(D3DCOLOR color)
{
	g_DebugTextColor = color;
}

void DebugText(int x, int y, const char* pFormat, ...)
{
#ifdef _DEBUG
	va_list argp;
	char strBuf[512];
	va_start(argp, pFormat);
	vsprintf_s(strBuf, 512, pFormat, argp);
	va_end(argp);

	RECT rect =
	{
	  x,
	  y,
	  WINDOW_WIDTH,
	  WINDOW_HEIGHT
	};

	g_pFont->DrawText
	(
		NULL,
		strBuf,
		-1,
		&rect,
		DT_LEFT,
		g_DebugTextColor
	);
#endif
}

void SetTextStatus(D3DCOLOR FontColor, D3DCOLOR FontOutColor, int nSize)
{
	g_FontColor = FontColor;
	g_FontOutColor = FontOutColor;
	g_FrameSize = nSize;
}

void SetTextStyle(int nStyle)
{
	g_nTextStyle = nStyle;
}

void OutputText(int nNumFont, int x, int y, const char* pFormat, ...)
{
	if (g_pText[nNumFont] != NULL)
	{
		va_list argp;
		char strBuf[512];
		va_start(argp, pFormat);
		vsprintf_s(strBuf, 512, pFormat, argp);
		va_end(argp);

		if (g_FrameSize > 0)
		{
			RECT rectFrame[MAX_FRAME];

			rectFrame[0] = { x + g_FrameSize, y, WINDOW_WIDTH, WINDOW_HEIGHT };
			rectFrame[1] = { x - g_FrameSize, y, WINDOW_WIDTH, WINDOW_HEIGHT };
			rectFrame[2] = { x, y + g_FrameSize, WINDOW_WIDTH, WINDOW_HEIGHT };
			rectFrame[3] = { x, y - g_FrameSize, WINDOW_WIDTH, WINDOW_HEIGHT };
			rectFrame[4] = { x + g_FrameSize, y + g_FrameSize, WINDOW_WIDTH, WINDOW_HEIGHT };
			rectFrame[5] = { x + g_FrameSize, y - g_FrameSize, WINDOW_WIDTH, WINDOW_HEIGHT };
			rectFrame[6] = { x - g_FrameSize, y - g_FrameSize, WINDOW_WIDTH, WINDOW_HEIGHT };
			rectFrame[7] = { x - g_FrameSize, y + g_FrameSize, WINDOW_WIDTH, WINDOW_HEIGHT };

			for (int count = 0; count < MAX_FRAME; count++)
			{
				g_pText[nNumFont]->DrawText
				(
					NULL,
					strBuf,
					-1,
					&rectFrame[count],
					g_nTextStyle,
					g_FontOutColor
				);
			}

		}

		RECT rect =
		{
			x,
			y,
			WINDOW_WIDTH,
			WINDOW_HEIGHT
		};

		g_pText[nNumFont]->DrawText
		(
			NULL,
			strBuf,
			-1,
			&rect,
			g_nTextStyle,
			g_FontColor
		);
	}
}
