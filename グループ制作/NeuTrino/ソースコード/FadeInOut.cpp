//----------------------------------------------------------------------------
//
//	フェードイン&アウト
//
//														 制作者：劉喆
//                                                       作成日：2017/02/03
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "FadeInOut.h"
#include "sound.h"
#include "data.h"
#include "SelectSongWaitMember.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	FIOT_DOOR,
	FIOT_MAX
}FADEIN_OUT_TYPE;

typedef enum {
	FIOP_CENTER,
	FIOP_LEFT,
	FIOP_RIGHT,
	FIOP_LEFT_UP,
	FIOP_RIGHT_UP,
	FIOP_LEFT_DOWN,
	FIOP_RIGHT_DOWN,
	FIOP_MAX
}FADEIN_OUT_PARTS;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	D3DXVECTOR2 vPosOld;
	D3DXVECTOR2 vPosDir;
	int nA;
	int nAOld;
	int nADir;
	float fScale;
	float fScaleOld;
	float fScaleDir;
	int nCount;
	TEXTURE tTexture;
}FADEIN_OUT;

//---------------------------グローバル変数-----------------------------------
static const float gc_fFadeInOutR = 450.0f;
static const D3DXVECTOR2 gc_vFadeInOutInitPos[FIOP_MAX]=
{
	{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
	{ GetTextureStatus(TEX_FADEINOUT_LEFT).u * 0.5f - 1.0f,WINDOW_HEIGHT / 2.0f },
	{ WINDOW_WIDTH - GetTextureStatus(TEX_FADEINOUT_RIGHT).u * 0.5f + 1.0f,WINDOW_HEIGHT / 2.0f },
	{ GetTextureStatus(TEX_FADEINOUT_LEFT_UP).u * 0.5f + 14.0f,GetTextureStatus(TEX_FADEINOUT_LEFT_UP).v * 0.5f - 2.0f },
	{ WINDOW_WIDTH - GetTextureStatus(TEX_FADEINOUT_RIGHT_UP).u * 0.5f - 14.0f,GetTextureStatus(TEX_FADEINOUT_RIGHT_UP).v * 0.5f - 2.0f },
	{ GetTextureStatus(TEX_FADEINOUT_LEFT_DOWN).u * 0.5f + 14.0f,WINDOW_HEIGHT - GetTextureStatus(TEX_FADEINOUT_LEFT_DOWN).v * 0.5f - 0.0f },
	{ WINDOW_WIDTH - GetTextureStatus(TEX_FADEINOUT_RIGHT_DOWN).u * 0.5f - 14.0f,WINDOW_HEIGHT - GetTextureStatus(TEX_FADEINOUT_RIGHT_DOWN).v * 0.5f - 0.0f },
};

static const float gc_fFadeInOutAngle[FIOP_MAX]=
{
	D3DXToRadian(0.0f),
	D3DXToRadian(180.0f),
	D3DXToRadian(0.0f),
	D3DXToRadian(240.0f),
	D3DXToRadian(300.0f),
	D3DXToRadian(120.0f),
	D3DXToRadian(60.0f),
};

static FADEIN_OUT g_FadeInOut[FIOP_MAX];

static const int gc_nFadeInOutTexNum[FIOP_MAX]=
{
	TEX_FADEINOUT_CENTER,
	TEX_FADEINOUT_LEFT,
	TEX_FADEINOUT_RIGHT,
	TEX_FADEINOUT_LEFT_UP,
	TEX_FADEINOUT_RIGHT_UP,
	TEX_FADEINOUT_LEFT_DOWN,
	TEX_FADEINOUT_RIGHT_DOWN,
};

static const D3DXVECTOR2 gc_vFadeInOutPos[FIOP_MAX][RDP_MAX]=
{
	{{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f }},
	{ { gc_vFadeInOutInitPos[1].x + cosf(gc_fFadeInOutAngle[1])*gc_fFadeInOutR,gc_vFadeInOutInitPos[1].y + sinf(gc_fFadeInOutAngle[1])*gc_fFadeInOutR },gc_vFadeInOutInitPos[1] },
	{ { gc_vFadeInOutInitPos[2].x + cosf(gc_fFadeInOutAngle[2])*gc_fFadeInOutR,gc_vFadeInOutInitPos[2].y + sinf(gc_fFadeInOutAngle[2])*gc_fFadeInOutR },gc_vFadeInOutInitPos[2] },
	{ { gc_vFadeInOutInitPos[3].x + cosf(gc_fFadeInOutAngle[3])*gc_fFadeInOutR,gc_vFadeInOutInitPos[3].y + sinf(gc_fFadeInOutAngle[3])*gc_fFadeInOutR },gc_vFadeInOutInitPos[3] },
	{ { gc_vFadeInOutInitPos[4].x + cosf(gc_fFadeInOutAngle[4])*gc_fFadeInOutR,gc_vFadeInOutInitPos[4].y + sinf(gc_fFadeInOutAngle[4])*gc_fFadeInOutR },gc_vFadeInOutInitPos[4] },
	{ { gc_vFadeInOutInitPos[5].x + cosf(gc_fFadeInOutAngle[5])*gc_fFadeInOutR,gc_vFadeInOutInitPos[5].y + sinf(gc_fFadeInOutAngle[5])*gc_fFadeInOutR },gc_vFadeInOutInitPos[5] },
	{ { gc_vFadeInOutInitPos[6].x + cosf(gc_fFadeInOutAngle[6])*gc_fFadeInOutR,gc_vFadeInOutInitPos[6].y + sinf(gc_fFadeInOutAngle[6])*gc_fFadeInOutR },gc_vFadeInOutInitPos[6] },
};

static const int gc_nFadeInOutA[FIOP_MAX][RDP_MAX] =
{
	{0,255},
	{ 255,255 },
	{ 255,255 },
	{ 255,255 },
	{ 255,255 },
	{ 255,255 },
	{ 255,255 },
};

static const float gc_fFadeInOutScale[FIOP_MAX][RDP_MAX] =
{
	{0.0f,1.0f},
	{ 1.01f,1.01f },
	{ 1.01f,1.01f },
	{ 1.01f,1.01f },
	{ 1.01f,1.01f },
	{ 1.01f,1.01f },
	{ 1.01f,1.01f },
};

static const int gc_nFadeInOutCountMax[FIOP_MAX] =
{
	10,
	10,
	10,
	10,
	10,
	10,
	10,
};

static const int gc_nFadeInOutStartFrame[FIOP_MAX] =
{
	0,
	10,
	10,
	10,
	10,
	10,
	10,
};

static bool g_bFadeFlag[FIOF_MAX];
static const int gc_nFadeStartFrameCountMax = 20;
static int g_nFadeStartFrameCount;

static int g_nFadeWaitMax;
static int g_nFadeWait;

static bool g_bChangeMode;

//---------------------------プロトタイプ宣言---------------------------------
void InitFadeInOut()
{
	g_bChangeMode = false;
	g_bFadeFlag[FIOF_FADEIN] = false;
	g_bFadeFlag[FIOF_FADEOUT] = false;
	g_nFadeStartFrameCount = 0;

	g_nFadeWait = 0;
	g_nFadeWaitMax = 0;

	for (int nCountFadeInOut = 0; nCountFadeInOut < FIOP_MAX; nCountFadeInOut++)
	{
		g_FadeInOut[nCountFadeInOut].tTexture = GetTextureStatus(gc_nFadeInOutTexNum[nCountFadeInOut]);
		
		g_FadeInOut[nCountFadeInOut].vPos = gc_vFadeInOutPos[nCountFadeInOut][RDP_INIT];
		g_FadeInOut[nCountFadeInOut].vPosOld = g_FadeInOut[nCountFadeInOut].vPos;
		g_FadeInOut[nCountFadeInOut].vPosDir = g_FadeInOut[nCountFadeInOut].vPos - g_FadeInOut[nCountFadeInOut].vPosOld;
		
		g_FadeInOut[nCountFadeInOut].nA = gc_nFadeInOutA[nCountFadeInOut][RDP_INIT];
		g_FadeInOut[nCountFadeInOut].nAOld = g_FadeInOut[nCountFadeInOut].nA;
		g_FadeInOut[nCountFadeInOut].nADir = g_FadeInOut[nCountFadeInOut].nA - g_FadeInOut[nCountFadeInOut].nAOld;
		
		g_FadeInOut[nCountFadeInOut].fScale = gc_fFadeInOutScale[nCountFadeInOut][RDP_INIT];
		g_FadeInOut[nCountFadeInOut].fScaleOld = g_FadeInOut[nCountFadeInOut].fScale;
		g_FadeInOut[nCountFadeInOut].fScaleDir = g_FadeInOut[nCountFadeInOut].fScale - g_FadeInOut[nCountFadeInOut].fScaleOld;
	}
}

void UninitFadeInOut()
{

}

void UpdateFadeInOut()
{
	if (g_bFadeFlag[FIOF_FADEIN] || g_bFadeFlag[FIOF_FADEOUT])
	{
		if (g_nFadeStartFrameCount < gc_nFadeStartFrameCountMax)
		{
			g_nFadeStartFrameCount++;
		}
		else
		{
			if (g_nFadeStartFrameCount == gc_nFadeStartFrameCountMax)
			{
				if (g_bFadeFlag[FIOF_FADEIN])
				{
					PlaySound(SE_FADEIN_CLOSE);
				}
				g_nFadeStartFrameCount++;
			}
			if (g_nFadeWait < g_nFadeWaitMax)
			{
				g_nFadeWait++;
			}
			else
			{
				if (g_bFadeFlag[FIOF_FADEIN])
				{
					if (GetGameModeTarget() != MODE_GAME)
					{
						g_bFadeFlag[FIOF_FADEIN] = false;
						SetFadeInOut(FIOF_FADEOUT);
						ChangeGameMode();
					}
					else
					{
						if (Data::g_player.init_game || (!GetRoomCreate() && !GetRoomEntry()))
						{
							g_bFadeFlag[FIOF_FADEIN] = false;
							SetFadeInOut(FIOF_FADEOUT);
						}
						if (!g_bChangeMode)
						{
							ChangeGameMode();
							g_bChangeMode = true;
						}
					}
				}
				else
				{
					if (g_bFadeFlag[FIOF_FADEOUT])
					{
						g_bFadeFlag[FIOF_FADEOUT] = false;
					}
				}
			}
		}
		for (int nCountFadeInOut = 0; nCountFadeInOut < FIOP_MAX; nCountFadeInOut++)
		{
			if (g_nFadeStartFrameCount >= gc_nFadeInOutStartFrame[nCountFadeInOut])
			{
				if (g_FadeInOut[nCountFadeInOut].nCount < gc_nFadeInOutCountMax[nCountFadeInOut])
				{
					if (g_FadeInOut[nCountFadeInOut].nCount == 0 && g_bFadeFlag[FIOF_FADEIN])
					{
						PlaySound(SE_FADEIN_START);
					}
					g_FadeInOut[nCountFadeInOut].nCount++;

					g_FadeInOut[nCountFadeInOut].fScaleOld += g_FadeInOut[nCountFadeInOut].fScaleDir / (gc_nFadeInOutCountMax[nCountFadeInOut] * 1.0f);

					g_FadeInOut[nCountFadeInOut].nAOld += (g_FadeInOut[nCountFadeInOut].nADir * 1.0f) / (gc_nFadeInOutCountMax[nCountFadeInOut] * 1.0f);

					g_FadeInOut[nCountFadeInOut].vPosOld += g_FadeInOut[nCountFadeInOut].vPosDir / (gc_nFadeInOutCountMax[nCountFadeInOut] * 1.0f);

					if (g_FadeInOut[nCountFadeInOut].nCount == gc_nFadeInOutCountMax[nCountFadeInOut])
					{
						g_FadeInOut[nCountFadeInOut].fScaleOld = g_FadeInOut[nCountFadeInOut].fScale;
						g_FadeInOut[nCountFadeInOut].nAOld = g_FadeInOut[nCountFadeInOut].nA;
						g_FadeInOut[nCountFadeInOut].vPosOld = g_FadeInOut[nCountFadeInOut].vPos;
					}
				}
			}
		}
	}
}

void DrawFadeInOut()
{
	if (g_bFadeFlag[FIOF_FADEIN] || g_bFadeFlag[FIOF_FADEOUT])
	{
		SetZBuffer(FALSE);
		for (int nCountFadeInOut = 0; nCountFadeInOut < FIOP_MAX; nCountFadeInOut++)
		{
			SetTexture(gc_nFadeInOutTexNum[nCountFadeInOut]);
			SetPolygonColor(D3DCOLOR_ARGB(g_FadeInOut[nCountFadeInOut].nAOld, 222, 222, 222));
			SetScale
			(
				g_FadeInOut[nCountFadeInOut].fScaleOld,
				g_FadeInOut[nCountFadeInOut].fScaleOld,
				g_FadeInOut[nCountFadeInOut].tTexture.u / 2.0f,
				g_FadeInOut[nCountFadeInOut].tTexture.v / 2.0f
			);
			DrawPolygon
			(
				g_FadeInOut[nCountFadeInOut].vPosOld.x,
				g_FadeInOut[nCountFadeInOut].vPosOld.y,
				0,
				0,
				g_FadeInOut[nCountFadeInOut].tTexture.u,
				g_FadeInOut[nCountFadeInOut].tTexture.v
			);
		}
		SetZBuffer(TRUE);
	}
}

void SetFadeInOut(int nFadeInOut)
{
	g_bFadeFlag[nFadeInOut] = true;
	g_nFadeStartFrameCount = 0;

	switch (nFadeInOut)
	{
	case FIOF_FADEIN:
	{
		g_bChangeMode = false;
		g_nFadeWait = 0;
		for (int nCountFadeInOut = 0; nCountFadeInOut < FIOP_MAX; nCountFadeInOut++)
		{
			g_FadeInOut[nCountFadeInOut].nCount = 0;
			g_FadeInOut[nCountFadeInOut].vPos = gc_vFadeInOutPos[nCountFadeInOut][RDP_END];
			g_FadeInOut[nCountFadeInOut].nA = gc_nFadeInOutA[nCountFadeInOut][RDP_END];
			g_FadeInOut[nCountFadeInOut].fScale = gc_fFadeInOutScale[nCountFadeInOut][RDP_END];
		}

		break;
	}
	case FIOF_FADEOUT:
	{
		for (int nCountFadeInOut = 0; nCountFadeInOut < FIOP_MAX; nCountFadeInOut++)
		{
			g_FadeInOut[nCountFadeInOut].nCount = 0;
			g_FadeInOut[nCountFadeInOut].vPos = gc_vFadeInOutPos[nCountFadeInOut][RDP_INIT];
			g_FadeInOut[nCountFadeInOut].nA = gc_nFadeInOutA[nCountFadeInOut][RDP_INIT];
			g_FadeInOut[nCountFadeInOut].fScale = gc_fFadeInOutScale[nCountFadeInOut][RDP_INIT];
		}
		break;
	}
	}
	for (int nCountFadeInOut = 0; nCountFadeInOut < FIOP_MAX; nCountFadeInOut++)
	{
		g_FadeInOut[nCountFadeInOut].vPosDir = g_FadeInOut[nCountFadeInOut].vPos - g_FadeInOut[nCountFadeInOut].vPosOld;
		g_FadeInOut[nCountFadeInOut].nADir = g_FadeInOut[nCountFadeInOut].nA - g_FadeInOut[nCountFadeInOut].nAOld;
		g_FadeInOut[nCountFadeInOut].fScaleDir = g_FadeInOut[nCountFadeInOut].fScale - g_FadeInOut[nCountFadeInOut].fScaleOld;
	}
}

void SetFadeWaitFrame(int nNum)
{
	g_nFadeWaitMax = nNum;
}

bool GetFadeFlag()
{
	return g_bFadeFlag[FIOF_FADEIN] | g_bFadeFlag[FIOF_FADEOUT];
}