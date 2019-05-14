//----------------------------------------------------------------------------
//
//	リザルトの評価
//
//														 制作者：劉喆
//                                                       作成日：2018/01/23
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "ResultAchievement.h"
#include "Gauge.h"
#include "sound.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;					// 座標
	int nA;								// α値
	int nU;								// Uの値
	float fScale;						// スケーリング
	int nCount;							// カウンター
	bool bStatus;						// 描画状態
}RESULT_ACHIEVEMENT_ICON;				// リザルトの達成項目

typedef struct {
	D3DXVECTOR2 vPos;					// 座標
	int nA;								// α値
	int nV;								// Vの値
	float fScale;						// スケーリング
	int nCount;							// カウンター
}RESULT_CLEAR_FAILED;					// リザルトの達成項目(クリア/失敗)

//---------------------------グローバル変数-----------------------------------
//----------------------------------------------------------------------------
// 達成項目(FC/AP)
//----------------------------------------------------------------------------
static RESULT_ACHIEVEMENT_ICON g_ResultAchievementIcon[RAP_MAX];

static const D3DXVECTOR2 gc_vResultAchievementInitPos = { 895.0f,570.0f };		// アイコンの初期座標
static const D3DXVECTOR2 gc_vResultAchievementRangePos = { 55.0f,0.0f };		// アイコン座標の差
static const float gc_fResultAchievementScale[RDP_MAX] =
{
	4.0f,1.0f
};

static const int gc_nResultAchievementA[RDP_MAX] =
{
	0,255
};

static const int gc_nResultAchievementMaxCount = 10;							// カウンターの最大値

static int g_nResultAchievementStartFrameCount;
static const int gc_nResultAchievementStartFrame[RAP_MAX]=
{
  0,
  36,
};

//---------------------------------------------------------------
// 達成項目(クリア/失敗)
//---------------------------------------------------------------
static RESULT_CLEAR_FAILED g_ResultClearFailed;

static const D3DXVECTOR2 gc_vResultClearFailedInitPos = { WINDOW_WIDTH / 2.0f,28.0f };		// アイコンの初期座標
static const float gc_fResultClearFailedScale[RDP_MAX] =
{
	4.0f,1.0f
};

static const int gc_nResultClearFailedA[RDP_MAX] =
{
	0,255
};

static const int gc_nResultClearFailedMaxCount = 10;							// カウンターの最大値


//---------------------------プロトタイプ宣言---------------------------------
//----------------------------------------------------------------------------
// 達成項目(FC/AP)
//----------------------------------------------------------------------------
void InitResultAchievement()
{
	g_nResultAchievementStartFrameCount = 0;

	for (int nCountResultAchievementParts = 0; nCountResultAchievementParts < RAP_MAX; nCountResultAchievementParts++)
	{
		g_ResultAchievementIcon[nCountResultAchievementParts].fScale = gc_fResultAchievementScale[RDP_INIT];
		g_ResultAchievementIcon[nCountResultAchievementParts].nCount = 0;
		g_ResultAchievementIcon[nCountResultAchievementParts].nA = gc_nResultAchievementA[RDP_INIT];
		g_ResultAchievementIcon[nCountResultAchievementParts].nU = nCountResultAchievementParts;
		g_ResultAchievementIcon[nCountResultAchievementParts].vPos = gc_vResultAchievementInitPos + nCountResultAchievementParts * gc_vResultAchievementRangePos;
	 }
}

void UninitResultAchievement()
{

}

void UpdateResultAchievement()
{
	if (g_nResultAchievementStartFrameCount < gc_nResultAchievementStartFrame[RAP_MAX - 1])
	{
		g_nResultAchievementStartFrameCount++;
	}
	for (int nCountResultAchievementParts = 0; nCountResultAchievementParts < RAP_MAX; nCountResultAchievementParts++)
	{
		if (g_ResultAchievementIcon[nCountResultAchievementParts].bStatus && 
			g_nResultAchievementStartFrameCount>=gc_nResultAchievementStartFrame[nCountResultAchievementParts])
		{
			if (g_ResultAchievementIcon[nCountResultAchievementParts].nCount < gc_nResultAchievementMaxCount)
			{
				g_ResultAchievementIcon[nCountResultAchievementParts].nCount++;
				
				// スケーリング
				g_ResultAchievementIcon[nCountResultAchievementParts].fScale += (gc_fResultAchievementScale[RDP_END] - gc_fResultAchievementScale[RDP_INIT]) /
					(gc_nResultAchievementMaxCount * 1.0f);

				// α値
				g_ResultAchievementIcon[nCountResultAchievementParts].nA += (gc_nResultAchievementA[RDP_END] - gc_nResultAchievementA[RDP_INIT]) * 1.0f /
					(gc_nResultAchievementMaxCount * 1.0f);
				if (g_ResultAchievementIcon[nCountResultAchievementParts].nCount == gc_nResultAchievementMaxCount)
				{
					g_ResultAchievementIcon[nCountResultAchievementParts].fScale = gc_fResultAchievementScale[RDP_END];
					g_ResultAchievementIcon[nCountResultAchievementParts].nA = gc_nResultAchievementA[RDP_END];
					if (GetPlayStatus(SE_FC_AP))
					{
						StopSound(SE_FC_AP);
					}
					PlaySound(SE_FC_AP);
				}
			}
		}
	}
}

void DrawResultAchievement()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_RESULT_ACHIEVEMENT_ICON);
	TEXTURE tTexture = GetTextureStatus(TEX_RESULT_ACHIEVEMENT_ICON);
	for (int nCountResultAchievementParts = 0; nCountResultAchievementParts < RAP_MAX; nCountResultAchievementParts++)
	{
		if (g_ResultAchievementIcon[nCountResultAchievementParts].bStatus)
		{
			SetPolygonColor(D3DCOLOR_ARGB(g_ResultAchievementIcon[nCountResultAchievementParts].nA, 255, 255, 255));
			SetScale
			(
				g_ResultAchievementIcon[nCountResultAchievementParts].fScale,
				g_ResultAchievementIcon[nCountResultAchievementParts].fScale,
				tTexture.u / 2.0f,
				tTexture.v / 2.0f
			);
			DrawPolygon
			(
				g_ResultAchievementIcon[nCountResultAchievementParts].vPos.x,
				g_ResultAchievementIcon[nCountResultAchievementParts].vPos.y,
				tTexture.u * g_ResultAchievementIcon[nCountResultAchievementParts].nU,
				0,
				tTexture.u,
				tTexture.v
			);
		}
	}
	SetZBuffer(TRUE);
}

void SetResultAchievement(int nParts, bool bStatus)
{
	g_ResultAchievementIcon[nParts].bStatus = bStatus;
}

//---------------------------------------------------------------
// 達成項目(クリア/失敗)
//---------------------------------------------------------------
void InitResultClearFailed()
{
	g_ResultClearFailed.fScale = gc_fResultClearFailedScale[RDP_INIT];
	g_ResultClearFailed.nA = gc_nResultClearFailedA[RDP_INIT];
	g_ResultClearFailed.nCount = 0;
	g_ResultClearFailed.vPos = gc_vResultClearFailedInitPos;
	if (!GetClearStatus())
	{
		g_ResultClearFailed.nV = 0;
	}
	else
	{
		g_ResultClearFailed.nV = 1;
	}
}

void UninitResultClearFailed()
{

}

void UpdateResultClearFailed()
{
	if (g_ResultClearFailed.nCount < gc_nResultClearFailedMaxCount)
	{
		g_ResultClearFailed.nCount++;

		// スケーリング
		g_ResultClearFailed.fScale += (gc_fResultClearFailedScale[RDP_END] - gc_fResultClearFailedScale[RDP_INIT]) /
			(gc_nResultClearFailedMaxCount * 1.0f);

		// α値
		g_ResultClearFailed.nA += (gc_nResultClearFailedA[RDP_END] - gc_nResultClearFailedA[RDP_INIT]) * 1.0f /
			(gc_nResultClearFailedMaxCount * 1.0f);
		if (g_ResultClearFailed.nCount == gc_nResultClearFailedMaxCount)
		{
			g_ResultClearFailed.fScale = gc_fResultClearFailedScale[RDP_END];
			g_ResultClearFailed.nA = gc_nResultClearFailedA[RDP_END];
		}
	}
}

void DrawResultClearFailed()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_RESULT_CLEAR_FAILED);
	TEXTURE tTexture = GetTextureStatus(TEX_RESULT_CLEAR_FAILED);
	SetPolygonColor(D3DCOLOR_ARGB(g_ResultClearFailed.nA, 255, 255, 255));
	SetScale
	(
		g_ResultClearFailed.fScale,
		g_ResultClearFailed.fScale,
		tTexture.u / 2.0f,
		tTexture.v / 2.0f
	);
	DrawPolygon
	(
		g_ResultClearFailed.vPos.x,
		g_ResultClearFailed.vPos.y,
		0,
		tTexture.v * g_ResultClearFailed.nV,
		tTexture.u,
		tTexture.v
	);
	SetZBuffer(TRUE);
}