//----------------------------------------------------------------------------
//
//	エラーリスト
//
//														 制作者：劉喆
//                                                       作成日：2018/02/13
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "debug.h"
#include "ErrorList.h"
#include "ScreenMask.h"
#include "sound.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	float fScale;
	float fScaleOld;
	float fScaleDir;
	int nCount;
	bool bStatus;
	int nWordA;
	int nWordAOld;
	int nWordADir;
	int nErrorNum;
	int nTarget;
}ERROR_LIST_BG;

//---------------------------グローバル変数-----------------------------------
static const int gc_nErrorListWordMaxWord = 128;
static const char gc_cErrorListWord[ELW_MAX][gc_nErrorListWordMaxWord] =
{
	"相手はルームを解散したため、\n参加できませんでした。",
	"これ以上入力できません\n(1～7文字まで)",
	"名前が入力されていません\n(1～7文字まで)",
};

static ERROR_LIST_BG g_ErrorListBG;

static const D3DXVECTOR2 gc_vErrorListBGInitPos=
{
	WINDOW_WIDTH / 2.0f,
	WINDOW_HEIGHT / 2.0f
};

static const float gc_fErrorListBGScale[RDP_MAX] = { 0.0f,1.0f };
static const int gc_nErrorListWordA[RDP_MAX] = { 0,255 };

static const int gc_nErrorListFadeMaxFrame = 4;
static const int gc_nErrorListMaxWaitFrame = 150;
static int g_nErrorListWaitFrameCount;

//---------------------------プロトタイプ宣言---------------------------------
void InitErrorList()
{
	g_nErrorListWaitFrameCount = 0;
	g_ErrorListBG.nTarget = RDP_INIT;
	g_ErrorListBG.fScale = gc_fErrorListBGScale[RDP_INIT];
	g_ErrorListBG.fScaleOld = gc_fErrorListBGScale[RDP_INIT];
	g_ErrorListBG.fScaleDir = g_ErrorListBG.fScale - g_ErrorListBG.fScaleOld;
	g_ErrorListBG.nCount = 0;
	g_ErrorListBG.nWordA = gc_nErrorListWordA[RDP_INIT];
	g_ErrorListBG.nWordAOld = gc_nErrorListWordA[RDP_INIT];
	g_ErrorListBG.nWordADir = g_ErrorListBG.nWordA - g_ErrorListBG.nWordAOld;
	g_ErrorListBG.vPos = gc_vErrorListBGInitPos;
	g_ErrorListBG.bStatus = false;
	g_ErrorListBG.nErrorNum = ELW_ROOM_NOT_EXIST;
	InitText(F_ERROR_LIST, 23, "MS コジック");
}

void UninitErrorList()
{
	UninitText(F_ERROR_LIST);
}

void UpdateErrorList()
{
	if (g_ErrorListBG.bStatus)
	{
		if (g_ErrorListBG.nCount == gc_nErrorListFadeMaxFrame)
		{
			if (g_ErrorListBG.nTarget == RDP_END)
			{
				if (g_nErrorListWaitFrameCount < gc_nErrorListMaxWaitFrame)
				{
					g_nErrorListWaitFrameCount++;
				}
				else
				{
					SetErrorList(g_ErrorListBG.nErrorNum, RDP_INIT);
				}
			}
		}
		if (g_ErrorListBG.nCount < gc_nErrorListFadeMaxFrame)
		{
			g_ErrorListBG.nCount++;
			g_ErrorListBG.fScaleOld += g_ErrorListBG.fScaleDir / (gc_nErrorListFadeMaxFrame * 1.0f);
			g_ErrorListBG.nWordAOld += (g_ErrorListBG.nWordADir * 1.0f) / (gc_nErrorListFadeMaxFrame * 1.0f);
			if (g_ErrorListBG.nCount == gc_nErrorListFadeMaxFrame)
			{
				g_ErrorListBG.fScaleOld = g_ErrorListBG.fScale;
				g_ErrorListBG.nWordAOld = g_ErrorListBG.nWordA;
				if (g_ErrorListBG.nTarget == RDP_INIT)
				{
					g_ErrorListBG.bStatus = false;
				}
			}
		}
	}
}

void DrawErrorList()
{
	if (g_ErrorListBG.bStatus)
	{
		SetZBuffer(FALSE);

		SetTexture(TEX_ERROR_LIST_BG);
		TEXTURE tTexture = GetTextureStatus(TEX_ERROR_LIST_BG);
		SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		SetScale(1.0f, g_ErrorListBG.fScaleOld, tTexture.u / 2.0f, tTexture.v / 2.0f);
		DrawPolygon
		(
			g_ErrorListBG.vPos.x,
			g_ErrorListBG.vPos.y,
			0,
			0,
			tTexture.u,
			tTexture.v
		);
		SetTextStatus(D3DCOLOR_ARGB(g_ErrorListBG.nWordAOld, 0, 0, 0), D3DCOLOR_ARGB(g_ErrorListBG.nWordAOld, 0, 0, 0), 0);
		SetTextStyle(DT_CENTER);
		OutputText(F_ERROR_LIST, 0.0f, g_ErrorListBG.vPos.y, "%s", &gc_cErrorListWord[g_ErrorListBG.nErrorNum]);

		SetZBuffer(TRUE);
	}
}

void SetErrorList(int nErrorNum, int nType)
{
	if (nType == RDP_END)
	{
		g_ErrorListBG.bStatus = true;
		g_nErrorListWaitFrameCount = 0;
		SetScreenMask(gc_nErrorListFadeMaxFrame, RDP_END);
		if (GetPlayStatus(SE_ERROR))
		{
			StopSound(SE_ERROR);
		}
		PlaySound(SE_ERROR);
	}
	else
	{
		SetScreenMask(gc_nErrorListFadeMaxFrame, RDP_INIT);
	}
	g_ErrorListBG.fScale = gc_fErrorListBGScale[nType];
	g_ErrorListBG.fScaleDir = g_ErrorListBG.fScale - g_ErrorListBG.fScaleOld;
	g_ErrorListBG.nWordA = gc_nErrorListWordA[nType];
	g_ErrorListBG.nWordADir = g_ErrorListBG.nWordA - g_ErrorListBG.nWordAOld;
	g_ErrorListBG.nCount = 0;
	g_ErrorListBG.nErrorNum = nErrorNum;
	g_ErrorListBG.nTarget = nType;
}

bool GetErrorListStatus()
{
	return g_ErrorListBG.bStatus;
}