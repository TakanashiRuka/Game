//----------------------------------------------------------------------------
//
//	前書き
//
//														 制作者：劉喆
//                                                       作成日：2017/01/28
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "title.h"
#include "Preface.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	PP_CONTROLLER,
	PP_TEAM_LOGO,
	PP_MAX,
}PREFACE_PARTS;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	int nCount;
	int nPrefacePartsNum;
	TEXTURE tTexture;
}PREFACE;

//---------------------------グローバル変数-----------------------------------
static const int gc_nCountPrefaceStartFrame = 30;		// 前書きアニメーションの始まりのフレーム

static int g_nCountPrefaceFrame;					// 前書きのカウンター

static PREFACE g_Preface;

static const int gc_nPrefaceTexNum[PP_MAX] =
{
	TEX_PREFACE_CONTROLLER,
	TEX_PREFACE_TEAM_LOGO,
};

static const int gc_nPrefaceA[AFF_MAX] = { 0, 255, 255, 0 };
static const int gc_nPrefaceMaxCount[CFF_MAX] = { 40,120,40 };

//---------------------------プロトタイプ宣言---------------------------------
void InitPreface()
{
	g_nCountPrefaceFrame = 0;

	g_Preface.nA = gc_nPrefaceA[AFF_FADEIN];
	g_Preface.nCount = 0;
	g_Preface.nPrefacePartsNum = 0;
	g_Preface.vPos = { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f };
	g_Preface.tTexture = GetTextureStatus(gc_nPrefaceTexNum[g_Preface.nPrefacePartsNum]);
}

void UninitPreface()
{

}

void UpdatePreface()
{
	if (g_nCountPrefaceFrame < gc_nCountPrefaceStartFrame)
	{
		g_nCountPrefaceFrame++;
	}
	else
	{
		g_Preface.nCount++;
		for (int nCountAnimationPart = 0; nCountAnimationPart < CFF_MAX; nCountAnimationPart++)
		{
			int nCounter = 0;
			for (int nCountAPSave = 0; nCountAPSave <= nCountAnimationPart; nCountAPSave++)
			{
				nCounter += gc_nPrefaceMaxCount[nCountAPSave];
			}
			if (g_Preface.nCount < nCounter)
			{
				g_Preface.nA +=
					(gc_nPrefaceA[nCountAnimationPart + 1] -
						gc_nPrefaceA[nCountAnimationPart]) * 1.0f /
						(gc_nPrefaceMaxCount[nCountAnimationPart] * 1.0f);

				if (g_Preface.nA < gc_nPrefaceA[AFF_FADEIN])
				{
					g_Preface.nA = gc_nPrefaceA[AFF_FADEIN];
				}
				if (g_Preface.nA > gc_nPrefaceA[AFF_MIDDLE_START])
				{
					g_Preface.nA = gc_nPrefaceA[AFF_MIDDLE_START];
				}
				break;
			}
			if (g_Preface.nCount == nCounter)
			{
				g_Preface.nA = gc_nPrefaceA[nCountAnimationPart + 1];
				if (nCountAnimationPart == CFF_MAX - 1)
				{
					if (g_Preface.nPrefacePartsNum < PP_MAX - 1)
					{
						g_Preface.nPrefacePartsNum++;
						g_Preface.nCount = 0;
						g_Preface.nA = gc_nPrefaceA[AFF_FADEIN];
						g_Preface.tTexture = GetTextureStatus(gc_nPrefaceTexNum[g_Preface.nPrefacePartsNum]);
					}
					else
					{
						SetGameModeTarget(MODE_TITLE);
						ChangeGameMode();
					}
				}
				break;
			}
		}
	}
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoyStickTrigger(JOY_SPHERE))
	{
		if (g_Preface.nCount < gc_nPrefaceMaxCount[CFF_FADEIN])
		{
			g_Preface.nCount = gc_nPrefaceMaxCount[CFF_FADEIN];
			g_Preface.nA = gc_nPrefaceA[AFF_MIDDLE_START];
		}
		else
			if (g_Preface.nCount < gc_nPrefaceMaxCount[CFF_FADEIN] + gc_nPrefaceMaxCount[CFF_MIDDLE])
			{
				g_Preface.nCount = gc_nPrefaceMaxCount[CFF_FADEIN] + gc_nPrefaceMaxCount[CFF_MIDDLE];
				g_Preface.nA = gc_nPrefaceA[AFF_MIDDLE_END];
			}
	}
}

void DrawPreface()
{
	SetZBuffer(FALSE);

	SetTexture(gc_nPrefaceTexNum[g_Preface.nPrefacePartsNum]);
	SetPolygonColor(D3DCOLOR_ARGB(g_Preface.nA, 255, 255, 255));
	DrawPolygon
	(
		g_Preface.vPos.x,
		g_Preface.vPos.y,
		0,
		0,
		g_Preface.tTexture.u,
		g_Preface.tTexture.v
	);

	SetZBuffer(TRUE);
}