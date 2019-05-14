//----------------------------------------------------------------------------
//
//	曲選択の背景部分
//
//														 制作者：劉喆
//                                                       作成日：2017/12/26
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "SelectSongBG.h"
#include "Polygon.h"
#include "Texture.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	SSBP_BG,
	SSBP_BLUE,
	SSBP_RED,
	SSBP_FRAME_UP,
	SSBP_FRAME_DOWN,
	SSBP_LINE,
	SSBP_MAX,
}SELECT_SONG_BG_PARTS;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	int nFadeInCount;
	D3DXVECTOR2 vUV;
	TEXTURE tTexture;
}SELECT_SONG_BG;

//---------------------------グローバル変数-----------------------------------
static SELECT_SONG_BG g_SelectSongBG[SSBP_MAX];

static const int gc_nSelectSongBGTexNum[SSBP_MAX]=
{
	TEX_SELECT_SONG_BG,
	TEX_SELECT_SONG_BLUE,
	TEX_SELECT_SONG_RED,
	TEX_SELECT_SONG_FRAME,
	TEX_SELECT_SONG_FRAME,
	TEX_SELECT_SONG_LINE,
};

static const D3DXVECTOR2 gc_vSelectSongBGPos[SSBP_MAX][RDP_MAX]=
{
	{ { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f, -85.0f },{ WINDOW_WIDTH / 2.0f, 25.0f } },
	{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT + 80.0f },{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 30.0f } },
	{ { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f } },
};

static const int gc_nSelectSongBGA[SSBP_MAX][RDP_MAX]=
{
	{ 0,255 },
	{ 0,255 },
	{ 0,255 },
	{ 255,255 },
	{ 255,255 },
	{ 0,255 },
};

static const int gc_nSelectSongBGFadeInCountMax[SSBP_MAX] =
{
	6,
	6,
	6,
	8,
	8,
	6,
};

static const int gc_nSelectSongBGStartFrame[SSBP_MAX] =
{
	0,
	6,
	6,
	6,
	6,
	0,
};

static const D3DXVECTOR2 gc_vSelectSongBGChangeUV[SSBP_MAX] =
{
	{ 0.0f,0.0f },
	{ -0.45f,0.15f },
	{ 1.35f,0.45f },
	{ -0.75f,0.0f },
	{ -0.75f,0.0f },
	{ 0.0f,0.0f },
};

static int g_nSelectSongStartFrameCount;
static const int gc_nSelectSongStartFrameCountMax = 30;

//---------------------------プロトタイプ宣言---------------------------------
void InitSelectSongBG()
{
	g_nSelectSongStartFrameCount = 0;

	for (int nCountSelectSongBGParts = 0; nCountSelectSongBGParts < SSBP_MAX; nCountSelectSongBGParts++)
	{
		g_SelectSongBG[nCountSelectSongBGParts].nA = gc_nSelectSongBGA[nCountSelectSongBGParts][RDP_INIT];
		g_SelectSongBG[nCountSelectSongBGParts].nFadeInCount = 0;
		g_SelectSongBG[nCountSelectSongBGParts].tTexture = GetTextureStatus(gc_nSelectSongBGTexNum[nCountSelectSongBGParts]);
		g_SelectSongBG[nCountSelectSongBGParts].vPos = gc_vSelectSongBGPos[nCountSelectSongBGParts][RDP_INIT];
		g_SelectSongBG[nCountSelectSongBGParts].vUV = { 0.0f,0.0f };
	}
}

void UninitSelectSongBG()
{

}

void UpdateSelectSongBG()
{
	if (g_nSelectSongStartFrameCount < gc_nSelectSongStartFrameCountMax)
	{
		g_nSelectSongStartFrameCount++;
	}
	for (int nCountSelectSongBGParts = 0; nCountSelectSongBGParts < SSBP_MAX; nCountSelectSongBGParts++)
	{
		if (g_nSelectSongStartFrameCount >= gc_nSelectSongBGStartFrame[nCountSelectSongBGParts])
		{
			if (g_SelectSongBG[nCountSelectSongBGParts].nFadeInCount < gc_nSelectSongBGFadeInCountMax[nCountSelectSongBGParts])
			{
				g_SelectSongBG[nCountSelectSongBGParts].nFadeInCount++;
				g_SelectSongBG[nCountSelectSongBGParts].nA += 
				(gc_nSelectSongBGA[nCountSelectSongBGParts][RDP_END] - gc_nSelectSongBGA[nCountSelectSongBGParts][RDP_INIT]) * 1.0f / 
					(gc_nSelectSongBGFadeInCountMax[nCountSelectSongBGParts] * 1.0f);

				g_SelectSongBG[nCountSelectSongBGParts].vPos += 
					(gc_vSelectSongBGPos[nCountSelectSongBGParts][RDP_END] - gc_vSelectSongBGPos[nCountSelectSongBGParts][RDP_INIT]) / 
					(gc_nSelectSongBGFadeInCountMax[nCountSelectSongBGParts] * 1.0f);

				if (g_SelectSongBG[nCountSelectSongBGParts].nFadeInCount == gc_nSelectSongBGFadeInCountMax[nCountSelectSongBGParts])
				{
					g_SelectSongBG[nCountSelectSongBGParts].nA = gc_nSelectSongBGA[nCountSelectSongBGParts][RDP_END];
					g_SelectSongBG[nCountSelectSongBGParts].vPos = gc_vSelectSongBGPos[nCountSelectSongBGParts][RDP_END];
				}
			}
		}
		if (g_nSelectSongStartFrameCount == gc_nSelectSongStartFrameCountMax)
		{
			g_SelectSongBG[nCountSelectSongBGParts].vUV += gc_vSelectSongBGChangeUV[nCountSelectSongBGParts];
			if (g_SelectSongBG[nCountSelectSongBGParts].vUV.x > g_SelectSongBG[nCountSelectSongBGParts].tTexture.u)
			{
				g_SelectSongBG[nCountSelectSongBGParts].vUV.x -= g_SelectSongBG[nCountSelectSongBGParts].tTexture.u;
			}
			if (g_SelectSongBG[nCountSelectSongBGParts].vUV.x < g_SelectSongBG[nCountSelectSongBGParts].tTexture.u)
			{
				g_SelectSongBG[nCountSelectSongBGParts].vUV.x += g_SelectSongBG[nCountSelectSongBGParts].tTexture.u;
			}
			if (g_SelectSongBG[nCountSelectSongBGParts].vUV.y > g_SelectSongBG[nCountSelectSongBGParts].tTexture.v)
			{
				g_SelectSongBG[nCountSelectSongBGParts].vUV.y -= g_SelectSongBG[nCountSelectSongBGParts].tTexture.v;
			}
			if (g_SelectSongBG[nCountSelectSongBGParts].vUV.y < g_SelectSongBG[nCountSelectSongBGParts].tTexture.v)
			{
				g_SelectSongBG[nCountSelectSongBGParts].vUV.y += g_SelectSongBG[nCountSelectSongBGParts].tTexture.v;
			}
		}
	}
}

void DrawSelectSongBG()
{
	SetZBuffer(FALSE);
	for (int nCountSelectSongBGParts = 0; nCountSelectSongBGParts < SSBP_MAX; nCountSelectSongBGParts++)
	{
		if (g_nSelectSongStartFrameCount >= gc_nSelectSongBGStartFrame[nCountSelectSongBGParts])
		{
			if (nCountSelectSongBGParts == SSBP_LINE)
			{
				SetBlend(D3DBLEND_BLENDFACTOR);
			}
			else
			{
				SetBlend(D3DBLEND_INVSRCALPHA);
			}
			SetTexture(gc_nSelectSongBGTexNum[nCountSelectSongBGParts]);
			SetPolygonColor(D3DCOLOR_ARGB(g_SelectSongBG[nCountSelectSongBGParts].nA, 200, 200, 200));
			DrawPolygon
			(
				g_SelectSongBG[nCountSelectSongBGParts].vPos.x,
				g_SelectSongBG[nCountSelectSongBGParts].vPos.y,
				g_SelectSongBG[nCountSelectSongBGParts].vUV.x,
				g_SelectSongBG[nCountSelectSongBGParts].vUV.y,
				g_SelectSongBG[nCountSelectSongBGParts].tTexture.u,
				g_SelectSongBG[nCountSelectSongBGParts].tTexture.v
			);
		}
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}