//----------------------------------------------------------------------------
//
//	ゲームの背景
//
//														 制作者：劉喆
//                                                       作成日：2017/02/01
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Polygon3D.h"
#include "Texture.h"
#include "GameBG.h"
#include "Gauge.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;			// 座標
	float fScale;				// スケーリング
	D3DXVECTOR3 vAngle;			// 角度
	float nU;						// Uの値
	float nV;						// Vの値
	int nA;
}GAME_BG_WALL;					// ゲームの背景(壁)

//---------------------------グローバル変数-----------------------------------
//----------------------------------------------------------------
// 壁
//----------------------------------------------------------------
static const int gc_nGameBGWallMax = 2;
static GAME_BG_WALL g_GameBGWall[gc_nGameBGWallMax];
static const D3DXVECTOR3 gc_vGameWallPosRange = { 1000.0f,0.0f,0.0f };
static const D3DXVECTOR3 gc_vGameWallInitPos = -0.5f * gc_vGameWallPosRange + D3DXVECTOR3(0.0f,1.0f,500.0f);
static const float gc_fGameWallInitScale = 15.0f;
static const int gc_nGameWallInitA = 255;
static const float gc_nGameWallChangeU[gc_nGameBGWallMax] = { 15.0f, -15.0f };

//----------------------------------------------------------------
// 床(線分)
//----------------------------------------------------------------
static const int gc_nGameBGFloorMax = 2;
static GAME_BG_WALL g_GameBGFloor[gc_nGameBGFloorMax];
static const D3DXVECTOR3 gc_vGameFloorPosRange = { 800.0f,0.0f,0.0f };
static const D3DXVECTOR3 gc_vGameFloorInitPos = -0.5f * gc_vGameFloorPosRange + D3DXVECTOR3(0.0f, 0.0f, 500.0f);
static const float gc_fGameFloorInitScale = 2.0f;
static const int gc_nGameFloorInitA = 127;
static const float gc_nGameFloorChangeV[gc_nGameBGFloorMax] = { -45.0f, -45.0f };

//----------------------------------------------------------------
// 床(六角形)
//----------------------------------------------------------------
static const int gc_nGameBGFloorHexMax = 2;
static GAME_BG_WALL g_GameBGFloorHex[gc_nGameBGFloorHexMax];
static const D3DXVECTOR3 gc_vGameFloorHexPosRange = { 800.0f,0.0f,0.0f };
static const D3DXVECTOR3 gc_vGameFloorHexInitPos = -0.5f * gc_vGameFloorHexPosRange + D3DXVECTOR3(0.0f,-3.0f,-150.0f);
static const float gc_fGameFloorHexInitScale = 1.0f;
static const int gc_nGameFloorHexInitA = 75;
static const float gc_nGameFloorHexChangeU[gc_nGameBGFloorHexMax] = { 0.5f, -0.5f };

//---------------------------プロトタイプ宣言---------------------------------
void InitGameBG()
{
	InitGameBGFloor();
	InitGameBGWall();
	InitGameBGLine();
}

void UninitGameBG()
{
	UninitGameBGFloor();
	UninitGameBGWall();
	UninitGameBGLine();
}

void UpdateGameBG()
{
	UpdateGameBGFloor();
	UpdateGameBGWall();
	UpdateGameBGLine();
}

void DrawGameBG()
{
	DrawGameMainBG();
	DrawGameBGFloor();
	DrawGameBGLine();
	DrawGameBGWall();
}

//----------------------------------------------------------------------------
// 背景部分
//----------------------------------------------------------------------------
void DrawGameMainBG()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_GAME_MAIN_BG);
	TEXTURE tTexture = GetTextureStatus(TEX_GAME_MAIN_BG);
	SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawPolygon
	(
		WINDOW_WIDTH / 2.0f,
		WINDOW_HEIGHT / 2.0f,
		0,
		0,
		tTexture.u,
		tTexture.v
	);
	if (GetClearStatus())
	{
		SetBlend(D3DBLEND_BLENDFACTOR);
		SetTexture(TEX_GAME_MAIN_BG);
		TEXTURE tTexture = GetTextureStatus(TEX_GAME_MAIN_BG);
		SetPolygonColor(D3DCOLOR_ARGB(127, 255, 255, 255));
		DrawPolygon
		(
			WINDOW_WIDTH / 2.0f,
			WINDOW_HEIGHT / 2.0f,
			0,
			0,
			tTexture.u,
			tTexture.v
		);
		SetBlend(D3DBLEND_INVSRCALPHA);
	}

	if (GetClearStatus())
	{
		SetBlend(D3DBLEND_BLENDFACTOR);
		SetTexture(TEX_GAME_MAIN_BG_MASK);
		tTexture = GetTextureStatus(TEX_GAME_MAIN_BG_MASK);
		SetPolygonColor(D3DCOLOR_ARGB(100, 255, 255, 255));
		DrawPolygon
		(
			WINDOW_WIDTH / 2.0f,
			WINDOW_HEIGHT / 2.0f,
			0,
			0,
			tTexture.u,
			tTexture.v
		);
		SetBlend(D3DBLEND_INVSRCALPHA);
	}
	SetZBuffer(TRUE);
}

//----------------------------------------------------------------------------
// 背景部分(壁)
//----------------------------------------------------------------------------
void InitGameBGWall()
{
	for (int nCountGameWall = 0; nCountGameWall < gc_nGameBGWallMax; nCountGameWall++)
	{
		g_GameBGWall[nCountGameWall].fScale = gc_fGameWallInitScale;
		g_GameBGWall[nCountGameWall].nU = 0;
		g_GameBGWall[nCountGameWall].nV = 0;
		g_GameBGWall[nCountGameWall].nA = gc_nGameWallInitA;
		g_GameBGWall[nCountGameWall].vPos = gc_vGameWallInitPos + nCountGameWall * gc_vGameWallPosRange;
	}
	g_GameBGWall[0].vAngle = { D3DXToRadian(-100.0f), D3DXToRadian(-90.0f), D3DXToRadian(0.0f) };
	g_GameBGWall[1].vAngle = { D3DXToRadian(-100.0f), D3DXToRadian(90.0f), D3DXToRadian(0.0f) };
}

void UninitGameBGWall()
{

}

void UpdateGameBGWall()
{
	TEXTURE tTexture = GetTextureStatus(TEX_GAME_BG_WALL);
	for (int nCountGameWall = 0; nCountGameWall < gc_nGameBGWallMax; nCountGameWall++)
	{
		g_GameBGWall[nCountGameWall].nU += gc_nGameWallChangeU[nCountGameWall];
		if (g_GameBGWall[nCountGameWall].nU > tTexture.u)
		{
			g_GameBGWall[nCountGameWall].nU -= tTexture.u;
		}
		if (g_GameBGWall[nCountGameWall].nU < 0)
		{
			g_GameBGWall[nCountGameWall].nU += tTexture.u;
		}
		if (!GetClearStatus())
		{
			g_GameBGWall[nCountGameWall].nV = 0;
		}
		else
		{
			g_GameBGWall[nCountGameWall].nV = 1;
		}
	}
}

void DrawGameBGWall()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_GAME_BG_WALL);
	SetBlend(D3DBLEND_BLENDFACTOR);
	TEXTURE tTexture = GetTextureStatus(TEX_GAME_BG_WALL);
	for (int nCountGameWall = 0; nCountGameWall < gc_nGameBGWallMax; nCountGameWall++)
	{
		SetPolygonColor3D(D3DCOLOR_ARGB(g_GameBGWall[nCountGameWall].nA, 255, 255, 255));
		SetScale3D(g_GameBGWall[nCountGameWall].fScale, 1.0f, 1.0f);
		SetRot3D
		(
			g_GameBGWall[nCountGameWall].vAngle.x,
			g_GameBGWall[nCountGameWall].vAngle.y,
			g_GameBGWall[nCountGameWall].vAngle.z
		);
		DrawPolygon3D
		(
			g_GameBGWall[nCountGameWall].vPos.x,
			g_GameBGWall[nCountGameWall].vPos.y,
			g_GameBGWall[nCountGameWall].vPos.z,
			g_GameBGWall[nCountGameWall].nU,
			tTexture.v * g_GameBGWall[nCountGameWall].nV,
			tTexture.u * g_GameBGWall[nCountGameWall].fScale,
			tTexture.v
		);
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}

//-----------------------------------------------------------------
// 背景部分(線分)
//-----------------------------------------------------------------
void InitGameBGLine()
{
	for (int nCountGameBGFloor = 0; nCountGameBGFloor < gc_nGameBGFloorMax; nCountGameBGFloor++)
	{
		g_GameBGFloor[nCountGameBGFloor].fScale = gc_fGameFloorInitScale;
		g_GameBGFloor[nCountGameBGFloor].nA = gc_nGameFloorInitA;
		g_GameBGFloor[nCountGameBGFloor].nU = 0;
		g_GameBGFloor[nCountGameBGFloor].nV = 0;
		g_GameBGFloor[nCountGameBGFloor].vAngle = { 0.0f,0.0f,0.0f };
		g_GameBGFloor[nCountGameBGFloor].vPos = gc_vGameFloorInitPos + nCountGameBGFloor * gc_vGameFloorPosRange;
	}
}

void UninitGameBGLine()
{

}

void UpdateGameBGLine()
{
	TEXTURE tTexture = GetTextureStatus(TEX_GAME_BG_LINE);
	for (int nCountGameBGFloor = 0; nCountGameBGFloor < gc_nGameBGFloorMax; nCountGameBGFloor++)
	{
		g_GameBGFloor[nCountGameBGFloor].nV += gc_nGameFloorChangeV[nCountGameBGFloor];
		if (g_GameBGFloor[nCountGameBGFloor].nV > tTexture.v)
		{									  
			g_GameBGFloor[nCountGameBGFloor].nV -= tTexture.v;
		}									  
		if (g_GameBGFloor[nCountGameBGFloor].nV < 0)
		{									  
			g_GameBGFloor[nCountGameBGFloor].nV += tTexture.v;
		}
	}
}

void DrawGameBGLine()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_GAME_BG_LINE);
	SetBlend(D3DBLEND_BLENDFACTOR);
	TEXTURE tTexture = GetTextureStatus(TEX_GAME_BG_LINE);
	for (int nCountGameBGFloor = 0; nCountGameBGFloor < gc_nGameBGFloorMax; nCountGameBGFloor++)
	{
		SetPolygonColor3D(D3DCOLOR_ARGB(g_GameBGFloor[nCountGameBGFloor].nA, 255, 255, 255));
		SetScale3D(1.0f, 1.0f, g_GameBGFloor[nCountGameBGFloor].fScale);
		DrawPolygon3D
		(
			g_GameBGFloor[nCountGameBGFloor].vPos.x,
			g_GameBGFloor[nCountGameBGFloor].vPos.y,
			g_GameBGFloor[nCountGameBGFloor].vPos.z,
			g_GameBGFloor[nCountGameBGFloor].nU,
			g_GameBGFloor[nCountGameBGFloor].nV,
			tTexture.u,
			tTexture.v * g_GameBGFloor[nCountGameBGFloor].fScale
		);
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}

//---------------------------------------------------------------
// 背景部分(床)
//---------------------------------------------------------------
void InitGameBGFloor()
{
	for (int nCountGameBGFloor = 0; nCountGameBGFloor < gc_nGameBGFloorMax; nCountGameBGFloor++)
	{
		g_GameBGFloorHex[nCountGameBGFloor].fScale = gc_fGameFloorHexInitScale;
		g_GameBGFloorHex[nCountGameBGFloor].nA = gc_nGameFloorHexInitA;
		g_GameBGFloorHex[nCountGameBGFloor].nU = 0;
		g_GameBGFloorHex[nCountGameBGFloor].nV = 0;
		g_GameBGFloorHex[nCountGameBGFloor].vAngle = { 0.0f,0.0f,0.0f };
		g_GameBGFloorHex[nCountGameBGFloor].vPos = gc_vGameFloorHexInitPos + nCountGameBGFloor * gc_vGameFloorHexPosRange;
	}
}

void UninitGameBGFloor()
{

}

void UpdateGameBGFloor()
{
	TEXTURE tTexture = GetTextureStatus(TEX_GAME_BG_FLOOR);
	for (int nCountGameBGFloor = 0; nCountGameBGFloor < gc_nGameBGFloorMax; nCountGameBGFloor++)
	{
		g_GameBGFloorHex[nCountGameBGFloor].nU += gc_nGameFloorHexChangeU[nCountGameBGFloor];
		if (g_GameBGFloorHex[nCountGameBGFloor].nU > tTexture.u)
		{
			g_GameBGFloorHex[nCountGameBGFloor].nU -= tTexture.u;
		}
		if (g_GameBGFloorHex[nCountGameBGFloor].nU < 0)
		{
			g_GameBGFloorHex[nCountGameBGFloor].nU += tTexture.u;
		}
	}
}

void DrawGameBGFloor()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_GAME_BG_FLOOR);
	SetBlend(D3DBLEND_BLENDFACTOR);
	TEXTURE tTexture = GetTextureStatus(TEX_GAME_BG_FLOOR);
	for (int nCountGameBGFloor = 0; nCountGameBGFloor < gc_nGameBGFloorMax; nCountGameBGFloor++)
	{
		SetPolygonColor3D(D3DCOLOR_ARGB(g_GameBGFloorHex[nCountGameBGFloor].nA, 255, 255, 255));
		SetScale3D(g_GameBGFloorHex[nCountGameBGFloor].fScale, 1.0f, 1.0f);
		DrawPolygon3D
		(
			g_GameBGFloorHex[nCountGameBGFloor].vPos.x,
			g_GameBGFloorHex[nCountGameBGFloor].vPos.y,
			g_GameBGFloorHex[nCountGameBGFloor].vPos.z,
			g_GameBGFloorHex[nCountGameBGFloor].nU,
			g_GameBGFloorHex[nCountGameBGFloor].nV,
			tTexture.u * g_GameBGFloorHex[nCountGameBGFloor].fScale,
			tTexture.v
		);
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}