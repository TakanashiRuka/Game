//----------------------------------------------------------------------------
//
//	ゲームの終わり時点の評価
//
//														 制作者：劉喆
//                                                       作成日：2018/01/16
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "GameAchievement.h"
#include "Gauge.h"
#include "Result.h"
#include "game.h"
#include "Judge.h"
#include "ResultAchievement.h"
#include "sound.h"
#include "ScreenMask.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	GCF_BOARD,
	GCF_WORD,
	GCF_MAX
}GAME_CLEAR_FAILED;

typedef enum {
	GFA_BOARD_01,
	GFA_BOARD_02,
	GFA_BOARD_03,
	GFA_WORD,
	GFA_MAX
}GAME_FC_AP;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	D3DXVECTOR2 vScale;
	int nFadeInCount;
}GAME_ACHIEVEMENT;

//---------------------------グローバル変数-----------------------------------
static GAME_ACHIEVEMENT g_GameAcievementClearFailed[GCF_MAX];								// クリア/失敗の背景
static int g_nGameClearFailedV;
static bool g_bGameClearFailed;

static const int gc_nGameAcievementClearFailedTextureNum[GCF_MAX]=
{
	TEX_GAME_CLEAR_FAILED_BOARD,
	TEX_GAME_CLEAR_FAILED_WORD,
};

static int g_nGameClearFailedCount;									// クリア/失敗のアニメーションカウンター
static const int gc_nGameClearFailedMaxCount = 120;					// クリア/失敗のアニメーションカウンターの最大値

static const D3DXVECTOR2 gc_vGameAchievementClearFailedPos[GCF_MAX][RDP_MAX] =				// クリア/失敗の座標
{
	{ { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f }, { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f } },
	{ { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f }, { WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f } },
};

static const int gc_nGameAchievementClearFailedA[GCF_MAX][RDP_MAX]=						// クリア/失敗のα値
{
	{ 0,255 },
	{ 0,255 },
};

static const D3DXVECTOR2 gc_vGameAchievementClearFailedScale[GCF_MAX][RDP_MAX] =
{
	{ { 0.0f, 0.0f },{ 1.0f, 1.0f } },
	{ { 2.0f, 2.0f },{ 1.0f, 1.0f } },
};

static const int gc_nGameAchievementClearFailedFadeInMaxCount[GCF_MAX] =
{
	10,
	14,
};

static const int gc_nGameAchievementClearFailedStartTime[GCF_MAX] =
{
	0,
	6,
};

//-----------------------------------------------------------------
// FULLCOMBO / ALLPERFECT
//-----------------------------------------------------------------
static GAME_ACHIEVEMENT g_GameAchievementFCAP[GFA_MAX];

static int g_nGameFCAPV;
static bool g_bGameFCAP;

static const int gc_nGameAcievementFCAPTextureNum[GFA_MAX] =
{
	TEX_GAME_FC_AP_BOARD,
	TEX_GAME_FC_AP_BOARD,
	TEX_GAME_FC_AP_BOARD,
	TEX_GAME_FC_AP_WORD,
};

static int g_nGameFCAPCount;									// クリア/失敗のアニメーションカウンター
static const int gc_nGameFCAPMaxCount = 180;					// クリア/失敗のアニメーションカウンターの最大値

static const float gc_fGameFCAPR[AFF_MAX] =
{
  180.0f,
  110.0f,
  100.0f,
  170.0f,
};

static const float gc_fGameFCAPAngle[3] =
{
	D3DXToRadian(330.0f),
	D3DXToRadian(90.0f),
	D3DXToRadian(210.0f),
};

static const D3DXVECTOR2 gc_vGameAchievementFCAPPos[GFA_MAX][AFF_MAX] =				// クリア/失敗の座標
{
	{ 
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[0]) * gc_fGameFCAPR[AFF_FADEIN],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[0]) * gc_fGameFCAPR[AFF_FADEIN] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[0]) * gc_fGameFCAPR[AFF_MIDDLE_START],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[0]) * gc_fGameFCAPR[AFF_MIDDLE_START] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[0]) * gc_fGameFCAPR[AFF_MIDDLE_END],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[0]) * gc_fGameFCAPR[AFF_MIDDLE_END] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[0]) * gc_fGameFCAPR[AFF_FADEOUT],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[0]) * gc_fGameFCAPR[AFF_FADEOUT] }
	},
	{
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[1]) * gc_fGameFCAPR[AFF_FADEIN],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[1]) * gc_fGameFCAPR[AFF_FADEIN] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[1]) * gc_fGameFCAPR[AFF_MIDDLE_START],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[1]) * gc_fGameFCAPR[AFF_MIDDLE_START] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[1]) * gc_fGameFCAPR[AFF_MIDDLE_END],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[1]) * gc_fGameFCAPR[AFF_MIDDLE_END] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[1]) * gc_fGameFCAPR[AFF_FADEOUT],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[1]) * gc_fGameFCAPR[AFF_FADEOUT] }
	},
	{
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[2]) * gc_fGameFCAPR[AFF_FADEIN],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[2]) * gc_fGameFCAPR[AFF_FADEIN] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[2]) * gc_fGameFCAPR[AFF_MIDDLE_START],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[2]) * gc_fGameFCAPR[AFF_MIDDLE_START] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[2]) * gc_fGameFCAPR[AFF_MIDDLE_END],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[2]) * gc_fGameFCAPR[AFF_MIDDLE_END] },
		{ WINDOW_WIDTH / 2.0f + cosf(gc_fGameFCAPAngle[2]) * gc_fGameFCAPR[AFF_FADEOUT],WINDOW_HEIGHT / 2.0f + sinf(gc_fGameFCAPAngle[2]) * gc_fGameFCAPR[AFF_FADEOUT] }
	},
	{ 
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f },
		{ WINDOW_WIDTH / 2.0f,WINDOW_HEIGHT / 2.0f }
	},
};

static const int gc_nGameAchievementFCAPA[GFA_MAX][AFF_MAX] =						// クリア/失敗のα値
{
	{ 0,255,255,0 },
	{ 0,255,255,0 },
	{ 0,255,255,0 },
	{ 0,255,255,0 },
};

static const D3DXVECTOR2 gc_vGameAchievementFCAPScale[GFA_MAX][AFF_MAX] =
{
	{ 
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 2.0f, 2.0f }
	},
	{ 
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 2.0f, 2.0f }
	},
	{
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 2.0f, 2.0f }
	},
	{
		{ 0.0f, 0.0f },
		{ 0.9f, 0.9f },
		{ 1.0f, 1.0f },
		{ 3.0f, 3.0f }
	},
};

static const int gc_nGameAchievementFCAPFadeInOutMaxCount[GFA_MAX][CFF_MAX] =
{
	{10,90,10},
	{10,90,10},
	{10,90,10},
	{10,90,10},
};

static const int gc_nGameAchievementFCAPStartTime[GFA_MAX] =
{
	3,
	3,
	3,
	3,
};

//---------------------------プロトタイプ宣言---------------------------------

//----------------------------------------------------------------------------
// クリア/失敗部分
//----------------------------------------------------------------------------
void InitGameAchievementClearFailed()
{
	g_bGameClearFailed = false;
	g_nGameClearFailedV = 0;
	g_nGameClearFailedCount = 0;
	for (int nCountClearFailed = 0; nCountClearFailed < GCF_MAX; nCountClearFailed++)
	{
		g_GameAcievementClearFailed[nCountClearFailed].nA = gc_nGameAchievementClearFailedA[nCountClearFailed][RDP_INIT];
		g_GameAcievementClearFailed[nCountClearFailed].nFadeInCount = 0;
		g_GameAcievementClearFailed[nCountClearFailed].vPos = gc_vGameAchievementClearFailedPos[nCountClearFailed][RDP_INIT];
		g_GameAcievementClearFailed[nCountClearFailed].vScale = gc_vGameAchievementClearFailedScale[nCountClearFailed][RDP_INIT];
	}
}

void UninitGameAchievementClearFailed()
{

}

void UpdateGameAchievementClearFailed()
{
	if (g_bGameClearFailed)
	{
		if (g_nGameClearFailedCount < gc_nGameClearFailedMaxCount)
		{
			g_nGameClearFailedCount++;
		}
		if (g_nGameClearFailedCount == gc_nGameClearFailedMaxCount)
		{
			if (!GetFadeFlag())
			{
				switch (GetGameMode())
				{
				case MODE_TUTORIAL:
				{
					SetGameModeTarget(MODE_SELECT_SONG);
					break;
				}
				case MODE_GAME:
				{
					SetGameModeTarget(MODE_RESULT);
					break;
				}
				}
				SetFadeWaitFrame(60);
				SetFadeInOut(FIOF_FADEIN);
				//ChangeGameMode();
				g_nGameClearFailedCount++;
			}
		}
		for (int nCountClearFailed = 0; nCountClearFailed < GCF_MAX; nCountClearFailed++)
		{
			if (g_nGameClearFailedCount >= gc_nGameAchievementClearFailedStartTime[nCountClearFailed])
			{
				if (g_GameAcievementClearFailed[nCountClearFailed].nFadeInCount < gc_nGameAchievementClearFailedFadeInMaxCount[nCountClearFailed])
				{
					// カウンター
					g_GameAcievementClearFailed[nCountClearFailed].nFadeInCount++;
					
					// α値
					g_GameAcievementClearFailed[nCountClearFailed].nA += (gc_nGameAchievementClearFailedA[nCountClearFailed][RDP_END] - gc_nGameAchievementClearFailedA[nCountClearFailed][RDP_INIT]) * 1.0f /
						(gc_nGameAchievementClearFailedFadeInMaxCount[nCountClearFailed] * 1.0f);
					
					// 座標
					g_GameAcievementClearFailed[nCountClearFailed].vPos += (gc_vGameAchievementClearFailedPos[nCountClearFailed][RDP_END] - gc_vGameAchievementClearFailedPos[nCountClearFailed][RDP_INIT]) * 1.0f /
						(gc_nGameAchievementClearFailedFadeInMaxCount[nCountClearFailed] * 1.0f);
					
					// スケーリング
					g_GameAcievementClearFailed[nCountClearFailed].vScale += (gc_vGameAchievementClearFailedScale[nCountClearFailed][RDP_END] - gc_vGameAchievementClearFailedScale[nCountClearFailed][RDP_INIT]) * 1.0f /
						(gc_nGameAchievementClearFailedFadeInMaxCount[nCountClearFailed]);
					if (g_GameAcievementClearFailed[nCountClearFailed].nFadeInCount == gc_nGameAchievementClearFailedFadeInMaxCount[nCountClearFailed])
					{
						g_GameAcievementClearFailed[nCountClearFailed].nA = gc_nGameAchievementClearFailedA[nCountClearFailed][RDP_END];
						g_GameAcievementClearFailed[nCountClearFailed].vPos = gc_vGameAchievementClearFailedPos[nCountClearFailed][RDP_END];
						g_GameAcievementClearFailed[nCountClearFailed].vScale = gc_vGameAchievementClearFailedScale[nCountClearFailed][RDP_END];
					}
				}
			}
		}
	}
}

void DrawGameAchievementClearFailed()
{
	if (g_bGameClearFailed)
	{
		SetZBuffer(FALSE);
		for (int nCountClearFailed = 0; nCountClearFailed < GCF_MAX; nCountClearFailed++)
		{
			if (g_nGameClearFailedCount >= gc_nGameAchievementClearFailedStartTime[nCountClearFailed])
			{
				SetTexture(gc_nGameAcievementClearFailedTextureNum[nCountClearFailed]);
				TEXTURE tTexture = GetTextureStatus(gc_nGameAcievementClearFailedTextureNum[nCountClearFailed]);
				SetPolygonColor(D3DCOLOR_ARGB(g_GameAcievementClearFailed[nCountClearFailed].nA, 255, 255, 255));
				SetScale
				(
					g_GameAcievementClearFailed[nCountClearFailed].vScale.x,
					g_GameAcievementClearFailed[nCountClearFailed].vScale.y,
					tTexture.u / 2.0f,
					tTexture.v / 2.0f
				);
				DrawPolygon
				(
					g_GameAcievementClearFailed[nCountClearFailed].vPos.x,
					g_GameAcievementClearFailed[nCountClearFailed].vPos.y,
					0,
					tTexture.v * g_nGameClearFailedV,
					tTexture.u,
					tTexture.v
				);
			}
		}
		SetZBuffer(TRUE);
	}
}

void SetClearFailedStatus()
{
	g_bGameClearFailed = true;
	if (GetClearStatus())
	{
		g_nGameClearFailedV = 1;
		PlaySound(SE_GAME_CLEAR);
	}
	else
	{
		g_nGameClearFailedV = 0;
		PlaySound(SE_GAME_FAILED);
	}
}

bool GetClearFailedStatus()
{
	return g_bGameClearFailed;
}

//---------------------------------------------------------------
// FULL COMBO / ALL PERFECT 部分
//---------------------------------------------------------------
void InitGameAchievementFCAP()
{
	g_bGameFCAP = false;
	g_nGameFCAPCount = 0;
	g_nGameFCAPV = 1;
	for (int nCountFCAP = 0; nCountFCAP < GFA_MAX; nCountFCAP++)
	{
		g_GameAchievementFCAP[nCountFCAP].nA = gc_nGameAchievementFCAPA[nCountFCAP][AFF_FADEIN];
		g_GameAchievementFCAP[nCountFCAP].nFadeInCount = 0;
		g_GameAchievementFCAP[nCountFCAP].vPos = gc_vGameAchievementFCAPPos[nCountFCAP][AFF_FADEIN];
		g_GameAchievementFCAP[nCountFCAP].vScale = gc_vGameAchievementFCAPScale[nCountFCAP][AFF_FADEIN];
	}
}

void UninitGameAchievementFCAP()
{

}

void UpdateGameAchievementFCAP()
{
	if (g_bGameFCAP)
	{
		if (g_nGameFCAPCount < gc_nGameFCAPMaxCount)
		{
			g_nGameFCAPCount++;
		}
		if (g_nGameFCAPCount == gc_nGameFCAPMaxCount)
		{
			g_bGameFCAP = false;
		}
		for (int nCountFCAP = 0; nCountFCAP < GFA_MAX; nCountFCAP++)
		{
			if (g_nGameFCAPCount >= gc_nGameAchievementFCAPStartTime[nCountFCAP])
			{
				// アニメーションを終わるまでの全フレーム数
				if (g_GameAchievementFCAP[nCountFCAP].nFadeInCount < gc_nGameAchievementFCAPFadeInOutMaxCount[nCountFCAP][CFF_FADEIN]
					+ gc_nGameAchievementFCAPFadeInOutMaxCount[nCountFCAP][CFF_MIDDLE] + gc_nGameAchievementFCAPFadeInOutMaxCount[nCountFCAP][CFF_FADEOUT])
				{
					g_GameAchievementFCAP[nCountFCAP].nFadeInCount++;
					if (g_GameAchievementFCAP[nCountFCAP].nFadeInCount == gc_nGameAchievementFCAPFadeInOutMaxCount[nCountFCAP][CFF_FADEIN] * 0 + 1)
					{
						PlaySound(SE_GAME_FC_AP);
					}
					for (int nCountAnimationPart = 0; nCountAnimationPart < CFF_MAX; nCountAnimationPart++)
					{
						int nCounter = 0;
						for (int nCountAPSave = 0; nCountAPSave <= nCountAnimationPart; nCountAPSave++)
						{
							nCounter += gc_nGameAchievementFCAPFadeInOutMaxCount[nCountFCAP][nCountAPSave];
						}
						if (g_GameAchievementFCAP[nCountFCAP].nFadeInCount < nCounter)
						{
							g_GameAchievementFCAP[nCountFCAP].nA += 
								(gc_nGameAchievementFCAPA[nCountFCAP][nCountAnimationPart + 1] - 
									gc_nGameAchievementFCAPA[nCountFCAP][nCountAnimationPart]) * 1.0f /
									(gc_nGameAchievementFCAPFadeInOutMaxCount[nCountFCAP][nCountAnimationPart] * 1.0f);
							
							g_GameAchievementFCAP[nCountFCAP].vPos += 
								(gc_vGameAchievementFCAPPos[nCountFCAP][nCountAnimationPart + 1] - 
									gc_vGameAchievementFCAPPos[nCountFCAP][nCountAnimationPart]) * 1.0f / 
									(gc_nGameAchievementFCAPFadeInOutMaxCount[nCountFCAP][nCountAnimationPart] * 1.0f);
							
							g_GameAchievementFCAP[nCountFCAP].vScale += 
								(gc_vGameAchievementFCAPScale[nCountFCAP][nCountAnimationPart + 1] - 
									gc_vGameAchievementFCAPScale[nCountFCAP][nCountAnimationPart]) * 1.0f / 
									(gc_nGameAchievementFCAPFadeInOutMaxCount[nCountFCAP][nCountAnimationPart] * 1.0f);
							break;
						}
						if (g_GameAchievementFCAP[nCountFCAP].nFadeInCount == nCounter)
						{
							g_GameAchievementFCAP[nCountFCAP].nA = gc_nGameAchievementFCAPA[nCountFCAP][nCountAnimationPart + 1];
							g_GameAchievementFCAP[nCountFCAP].vPos = gc_vGameAchievementFCAPPos[nCountFCAP][nCountAnimationPart + 1];
							g_GameAchievementFCAP[nCountFCAP].vScale = gc_vGameAchievementFCAPScale[nCountFCAP][nCountAnimationPart + 1];
							if (nCountAnimationPart == CFF_MIDDLE)
							{
								SetScreenMask(10, RDP_INIT);
							}
							break;
						}
					}
				}
			}
		}
	}
}

void DrawGameAchievementFCAP()
{
	if (g_bGameFCAP)
	{
		SetZBuffer(FALSE);
		for (int nCountFCAP = 0; nCountFCAP < GFA_MAX; nCountFCAP++)
		{
			if (g_nGameFCAPCount >= gc_nGameAchievementFCAPStartTime[nCountFCAP])
			{
				SetTexture(gc_nGameAcievementFCAPTextureNum[nCountFCAP]);
				TEXTURE tTexture = GetTextureStatus(gc_nGameAcievementFCAPTextureNum[nCountFCAP]);
				SetPolygonColor(D3DCOLOR_ARGB(g_GameAchievementFCAP[nCountFCAP].nA, 255, 255, 255));
				SetScale
				(
					g_GameAchievementFCAP[nCountFCAP].vScale.x,
					g_GameAchievementFCAP[nCountFCAP].vScale.y,
					tTexture.u / 2.0f,
					tTexture.v / 2.0f
				);
				DrawPolygon
				(
					g_GameAchievementFCAP[nCountFCAP].vPos.x,
					g_GameAchievementFCAP[nCountFCAP].vPos.y,
					0,
					tTexture.v * g_nGameFCAPV,
					tTexture.u,
					tTexture.v
				);
			}
		}
		SetZBuffer(TRUE);
	}
}

void SetFCAP()
{
	if (GetJudgeLevelDetail(JL_BAD) == 0 && GetJudgeLevelDetail(JL_GOOD) == 0)
	{
		g_bGameFCAP = true;
		g_nGameFCAPV = 1;
		SetResultAchievement(RAP_ALL_PERFECT, true);
		SetResultAchievement(RAP_FULL_COMBO, true);
		SetScreenMask(10, RDP_END);
	}
	else
		if (GetJudgeLevelDetail(JL_BAD) == 0 && GetJudgeLevelDetail(JL_GOOD) != 0)
		{
			g_bGameFCAP = true;
			g_nGameFCAPV = 0;
			SetResultAchievement(RAP_ALL_PERFECT, false);
			SetResultAchievement(RAP_FULL_COMBO, true);
			SetScreenMask(10, RDP_END);
		}
		else
		{
			SetResultAchievement(RAP_ALL_PERFECT, false);
			SetResultAchievement(RAP_FULL_COMBO, false);
		}
}

bool GetFCAP()
{
	return g_bGameFCAP;
}