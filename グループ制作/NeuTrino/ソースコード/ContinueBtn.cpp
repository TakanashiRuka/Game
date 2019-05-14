//----------------------------------------------------------------------------
//
//	コンティニュー
//
//														 制作者：劉喆
//                                                       作成日：2017/03/05
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "ContinueBG.h"
#include "Polygon.h"
#include "Texture.h"
#include "input.h"
#include "JoyStick.h"
#include "FadeInOut.h"
#include "sound.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	int nCount;
	int nV;
	TEXTURE tTexture;
}CONTINUE_BTN;

//---------------------------グローバル変数-----------------------------------
static CONTINUE_BTN g_ContinueBtn;
static const int gc_nContinueBtnA[RDP_MAX] = { 0,255 };
static const int gc_nContinueBtnCountMax = 8;

//---------------------------プロトタイプ宣言---------------------------------
void InitContinueBtn()
{
	g_ContinueBtn.vPos = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 150.0f };
	g_ContinueBtn.nA = gc_nContinueBtnA[RDP_INIT];
	g_ContinueBtn.nCount = 0;
	g_ContinueBtn.tTexture = GetTextureStatus(TEX_CONTINUE_BTN);
	if (!GetJSDev())
	{
		g_ContinueBtn.nV = 0;
	}
	else
	{
		g_ContinueBtn.nV = 1;
	}
}

void UninitContinueBtn()
{

}

void UpdateContinueBtn()
{
	if (g_ContinueBtn.nCount < gc_nContinueBtnCountMax)
	{
		g_ContinueBtn.nCount++;
		g_ContinueBtn.nA += (gc_nContinueBtnA[RDP_END] - gc_nContinueBtnA[RDP_INIT]) * 1.0f / (gc_nContinueBtnCountMax * 1.0f);

		if (g_ContinueBtn.nCount == gc_nContinueBtnCountMax)
		{
			g_ContinueBtn.nA = gc_nContinueBtnA[RDP_END];
		}
	}
	if (g_ContinueBtn.nCount == gc_nContinueBtnCountMax)
	{
		if ((GetKeyboardTrigger(DIK_RETURN) || GetJoyStickTrigger(JOY_SPHERE)) && !GetFadeFlag())
		{
			if (GetPlayStatus(SE_ENTER))
			{
				StopSound(SE_ENTER);
			}
			PlaySound(SE_ENTER);
			SetGameModeTarget(MODE_SELECT_SONG);
			SetFadeWaitFrame(0);
			SetFadeInOut(FIOF_FADEIN);
		}
		if ((GetKeyboardTrigger(DIK_ESCAPE) || GetJoyStickTrigger(JOY_CROSS)) && !GetFadeFlag())
		{
			if (GetPlayStatus(SE_ENTER))
			{
				StopSound(SE_ENTER);
			}
			PlaySound(SE_ENTER);
			SetGameModeTarget(MODE_THANKS);
			SetFadeWaitFrame(30);
			SetFadeInOut(FIOF_FADEIN);
		}
	}
}

void DrawContinueBtn()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_CONTINUE_BTN);
	SetPolygonColor(D3DCOLOR_ARGB(g_ContinueBtn.nA, 255, 255, 255));
	DrawPolygon
	(
		g_ContinueBtn.vPos.x,
		g_ContinueBtn.vPos.y,
		0,
		g_ContinueBtn.nV * g_ContinueBtn.tTexture.v,
		g_ContinueBtn.tTexture.u,
		g_ContinueBtn.tTexture.v
	);
	SetZBuffer(TRUE);
}