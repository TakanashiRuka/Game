//----------------------------------------------------------------------------
//
//	小節ライン
//
//														 制作者：劉喆
//                                                       作成日：2017/12/12
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Line.h"
#include "Texture.h"
#include "Polygon3D.h"
#include "Light.h"
#include "Timer.h"
#include "debug.h"
#include "ReadNoteFile.h"
#include "Note.h"
#include "JudgeLine.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;		// 座標
	int nTime;				// 時間			  .
	bool bDrawStatus;		// 描画状態
	D3DXVECTOR3 vScale;		// スケール
}LINE_IN_GAME;				// 小節ライン(ゲーム)

//---------------------------グローバル変数-----------------------------------
static LINE_IN_GAME g_LineInGame[MAX_NOTE_PER_LANE];
static int g_nMaxLine;									// 小節ラインの最大本数
static int g_nStartLine;							// 更新/描画し始め小節ライン

static const D3DXVECTOR3 g_vLinePos = { 0.0f,0.0f,0.0f };

//---------------------------プロトタイプ宣言---------------------------------
void InitLine()
{
	g_nStartLine = 0;
	for (int nCountLine = 0; nCountLine < MAX_NOTE_PER_LANE; nCountLine++)
	{
		g_LineInGame[nCountLine].bDrawStatus = false;
		g_LineInGame[nCountLine].nTime = 0;
		g_LineInGame[nCountLine].vPos = g_vLinePos;
		g_LineInGame[nCountLine].vScale = { 406.0f,1.0f,10.0f };
	}
}

void SetLine()
{
	g_nMaxLine = GetMaxLineNum();
	int nOffset = GetNoteOffsetFromFile();
	for (int nCountLine = 0; nCountLine < g_nMaxLine; nCountLine++)
	{
		g_LineInGame[nCountLine].nTime = GetLine(nCountLine).nStartTime + nOffset;
	}
}

void UninitLine()
{

}

void UpdateLine(int nTime)
{
	//int nTime = GetTimer();
	float fHiSpeed = GetHiSpeed();
	g_nStartLine = GetRecordLine(nTime);
	for (int nCountLine = g_nStartLine; nCountLine < g_nMaxLine; nCountLine++)
	{
		g_LineInGame[nCountLine].vPos.z =
			JUDGE_LINE_Z + (-nTime + g_LineInGame[nCountLine].nTime)
			* fHiSpeed * NOTE_SPEED_N;

		if (g_LineInGame[nCountLine].vPos.z <= DRAW_NOTE_START
			&&	g_LineInGame[nCountLine].vPos.z > DRAW_NOTE_END)
		{
			g_LineInGame[nCountLine].bDrawStatus = true;
		}
		else
		{
			g_LineInGame[nCountLine].bDrawStatus = false;
		}
		if (g_LineInGame[nCountLine].vPos.z > DRAW_NOTE_START)
		{
			break;
		}
	}
}

void DrawLine()
{
	SetLight(FALSE);
	SetTexture(TEX_LINE);
	SetZBuffer(FALSE);
	for (int nCountLine = g_nStartLine; nCountLine < g_nMaxLine; nCountLine++)
	{
		if (g_LineInGame[nCountLine].bDrawStatus)
		{
				SetPolygonColor3D(D3DCOLOR_ARGB(127, 255, 255, 255));
				SetScale3D
				(
					g_LineInGame[nCountLine].vScale.x,
					g_LineInGame[nCountLine].vScale.y,
					g_LineInGame[nCountLine].vScale.z
				);
				DrawPolygon3D
				(
					g_LineInGame[nCountLine].vPos.x,
					g_LineInGame[nCountLine].vPos.y,
					g_LineInGame[nCountLine].vPos.z,
					0,
					0,
					GetTextureStatus(TEX_LINE).u,
					GetTextureStatus(TEX_LINE).v
				);
		}
	}
	SetZBuffer(TRUE);
}