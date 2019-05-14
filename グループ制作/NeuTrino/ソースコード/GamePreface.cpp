//----------------------------------------------------------------------------
//
//	ゲームが始まる前の処理
//
//														 制作者：劉喆
//                                                       作成日：2017/02/14
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "GamePreface.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	GPP_BG_UNDER,
	GPP_BG_UP,
	GPP_READY,
	GPP_START,
	GPP_MAX
}GAME_PREFACE_PARTS;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	D3DXVECTOR2 vScale;
	D3DXVECTOR2 vUV;
	int nFadeInCount;
}GAME_PREFACE;

//---------------------------グローバル変数-----------------------------------
static GAME_PREFACE g_GamePreface[GPP_MAX];

static bool g_bGamePreface;

static const int gc_nGamePrefaceTextureNum[GPP_MAX] =
{
	TEX_GAME_PREFACE_BG_UNDER,
	TEX_GAME_PREFACE_BG_UP,
	TEX_GAME_PREFACE_READY,
	TEX_GAME_PREFACE_START,
};

static int g_nGamePrefaceCount;									// クリア/失敗のアニメーションカウンター
static const int gc_nGamePrefaceMaxCount = 230;					// クリア/失敗のアニメーションカウンターの最大値

static const D3DXVECTOR2 gc_vGamePrefacePos[GPP_MAX][AFF_MAX] =				// クリア/失敗の座標
{
	{
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f }
	},
	{
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f }
	},
	{
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f }
	},
	{
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f }
	},
};

static const int gc_nGamePrefaceA[GPP_MAX][AFF_MAX] =						// クリア/失敗のα値
{
	{ 0,255,255,0 },
	{ 0,255,255,0 },
	{ 0,255,255,0 },
	{ 0,255,255,0 },
};

static const D3DXVECTOR2 gc_vGamePrefaceScale[GPP_MAX][AFF_MAX] =
{
	{
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f }
	},
	{
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f }
	},
	{
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f }
	},
	{
		{ 0.0f, 0.0f },
		{ 0.9f, 0.9f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f }
	},
};

static const D3DXVECTOR2 gc_vGamePrefaceChangeUV[GPP_MAX] =
{
	{ -0.5f,0.0f },
	{ 0.5f,0.0f },
	{ 0.0f,0.0f },
	{ 0.0f,0.0f },
};

static const int gc_nGamePrefaceFadeInOutMaxCount[GPP_MAX][CFF_MAX] =
{
	{ 10,170,10 },
	{ 10,150,10 },
	{ 10,75,10 },
	{ 10,75,10 },
};

static const int gc_nGamePrefaceStartTime[GPP_MAX] =
{
	30 + 0,
	30 + 10,
	30 + 5,
	30 + 95,
};

//---------------------------プロトタイプ宣言---------------------------------
//---------------------------------------------------------------
// READY / START 部分
//---------------------------------------------------------------
void InitGamePreface()
{
	g_bGamePreface = false;
	g_nGamePrefaceCount = 0;
	for (int nCountPreface = 0; nCountPreface < GPP_MAX; nCountPreface++)
	{
		g_GamePreface[nCountPreface].nA = gc_nGamePrefaceA[nCountPreface][AFF_FADEIN];
		g_GamePreface[nCountPreface].nFadeInCount = 0;
		g_GamePreface[nCountPreface].vPos = gc_vGamePrefacePos[nCountPreface][AFF_FADEIN];
		g_GamePreface[nCountPreface].vScale = gc_vGamePrefaceScale[nCountPreface][AFF_FADEIN];
		g_GamePreface[nCountPreface].vUV = { 0.0f,0.0f };
	}
}

void UninitGamePreface()
{

}

void UpdateGamePreface()
{
	if (g_bGamePreface)
	{
		if (g_nGamePrefaceCount < gc_nGamePrefaceMaxCount)
		{
			g_nGamePrefaceCount++;
		}
		if (g_nGamePrefaceCount == gc_nGamePrefaceMaxCount)
		{
			g_bGamePreface = false;
		}
		for (int nCountPreface = 0; nCountPreface < GPP_MAX; nCountPreface++)
		{
			if (g_nGamePrefaceCount >= gc_nGamePrefaceStartTime[nCountPreface])
			{
				// アニメーションを終わるまでの全フレーム数
				if (g_GamePreface[nCountPreface].nFadeInCount < gc_nGamePrefaceFadeInOutMaxCount[nCountPreface][CFF_FADEIN]
					+ gc_nGamePrefaceFadeInOutMaxCount[nCountPreface][CFF_MIDDLE] + gc_nGamePrefaceFadeInOutMaxCount[nCountPreface][CFF_FADEOUT])
				{
					g_GamePreface[nCountPreface].nFadeInCount++;
					g_GamePreface[nCountPreface].vUV += gc_vGamePrefaceChangeUV[nCountPreface];
					for (int nCountAnimationPart = 0; nCountAnimationPart < CFF_MAX; nCountAnimationPart++)
					{
						int nCounter = 0;
						for (int nCountAPSave = 0; nCountAPSave <= nCountAnimationPart; nCountAPSave++)
						{
							nCounter += gc_nGamePrefaceFadeInOutMaxCount[nCountPreface][nCountAPSave];
						}
						if (g_GamePreface[nCountPreface].nFadeInCount < nCounter)
						{
							g_GamePreface[nCountPreface].nA +=
								(gc_nGamePrefaceA[nCountPreface][nCountAnimationPart + 1] -
									gc_nGamePrefaceA[nCountPreface][nCountAnimationPart]) * 1.0f /
									(gc_nGamePrefaceFadeInOutMaxCount[nCountPreface][nCountAnimationPart] * 1.0f);

							g_GamePreface[nCountPreface].vPos +=
								(gc_vGamePrefacePos[nCountPreface][nCountAnimationPart + 1] -
									gc_vGamePrefacePos[nCountPreface][nCountAnimationPart]) * 1.0f /
									(gc_nGamePrefaceFadeInOutMaxCount[nCountPreface][nCountAnimationPart] * 1.0f);

							g_GamePreface[nCountPreface].vScale +=
								(gc_vGamePrefaceScale[nCountPreface][nCountAnimationPart + 1] -
									gc_vGamePrefaceScale[nCountPreface][nCountAnimationPart]) * 1.0f /
									(gc_nGamePrefaceFadeInOutMaxCount[nCountPreface][nCountAnimationPart] * 1.0f);
							break;
						}
						if (g_GamePreface[nCountPreface].nFadeInCount == nCounter)
						{
							g_GamePreface[nCountPreface].nA = gc_nGamePrefaceA[nCountPreface][nCountAnimationPart + 1];
							g_GamePreface[nCountPreface].vPos = gc_vGamePrefacePos[nCountPreface][nCountAnimationPart + 1];
							g_GamePreface[nCountPreface].vScale = gc_vGamePrefaceScale[nCountPreface][nCountAnimationPart + 1];
							break;
						}
					}
				}
			}
		}
	}
}

void DrawGamePreface()
{
	if (g_bGamePreface)
	{
		SetZBuffer(FALSE);
		for (int nCountPreface = 0; nCountPreface < GPP_MAX; nCountPreface++)
		{
			if (g_nGamePrefaceCount >= gc_nGamePrefaceStartTime[nCountPreface])
			{
				SetTexture(gc_nGamePrefaceTextureNum[nCountPreface]);
				TEXTURE tTexture = GetTextureStatus(gc_nGamePrefaceTextureNum[nCountPreface]);
				SetPolygonColor(D3DCOLOR_ARGB(g_GamePreface[nCountPreface].nA, 255, 255, 255));
				SetScale
				(
					g_GamePreface[nCountPreface].vScale.x,
					g_GamePreface[nCountPreface].vScale.y,
					tTexture.u / 2.0f,
					tTexture.v / 2.0f
				);
				DrawPolygon
				(
					g_GamePreface[nCountPreface].vPos.x,
					g_GamePreface[nCountPreface].vPos.y,
					g_GamePreface[nCountPreface].vUV.x,
					g_GamePreface[nCountPreface].vUV.y,
					tTexture.u,
					tTexture.v
				);
			}
		}
		SetZBuffer(TRUE);
	}
}

void SetGamePreface()
{
	g_bGamePreface = true;
	g_nGamePrefaceCount = 0;
	for (int nCountPreface = 0; nCountPreface < GPP_MAX; nCountPreface++)
	{
		g_GamePreface[nCountPreface].nA = gc_nGamePrefaceA[nCountPreface][AFF_FADEIN];
		g_GamePreface[nCountPreface].nFadeInCount = 0;
		g_GamePreface[nCountPreface].vPos = gc_vGamePrefacePos[nCountPreface][AFF_FADEIN];
		g_GamePreface[nCountPreface].vScale = gc_vGamePrefaceScale[nCountPreface][AFF_FADEIN];
	}
}

bool GetGamePreface()
{
	return g_bGamePreface;
}