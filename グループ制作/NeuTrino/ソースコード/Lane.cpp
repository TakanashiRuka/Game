//----------------------------------------------------------------------------
//
//	レーン
//
//														 制作者：劉喆
//                                                       作成日：2017/11/29
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Lane.h"
#include "Texture.h"
#include "Polygon3D.h"
#include "Light.h"

//---------------------------マクロ定義---------------------------------------
#define LANE_X					(0.0f)
#define LANE_Y					(0.0f)
#define LANE_Z					(500.0f)

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;			// 座標
}LANE;							// レーン

typedef struct {
	D3DXVECTOR3 vPos;			// 座標
	float fScale;
	int nU;
	int nV;
	TEXTURE tLaneEffect;
}LANE_EFFECT;

//---------------------------グローバル変数-----------------------------------
static LANE g_Lane;

static const int gc_nMaxLaneEffect = 2;
static const D3DXVECTOR3 gc_vLaneEffectInitPos[gc_nMaxLaneEffect]=
{
	{
		-GetTextureStatus(TEX_LANE).u / 2.0f + GetTextureStatus(TEX_LANE_EFFECT).u / 2.0f,
		0.0f,
		LANE_Z
	},
	{
		GetTextureStatus(TEX_LANE).u / 2.0f - GetTextureStatus(TEX_LANE_EFFECT).u / 2.0f,
		0.0f,
		LANE_Z
	},
};

LANE_EFFECT g_LaneEffect[gc_nMaxLaneEffect];

//---------------------------プロトタイプ宣言---------------------------------
void InitLane()
{
	g_Lane.vPos = { LANE_X, LANE_Y, LANE_Z };
}

void UninitLane()
{

}

void UpdateLane()
{

}

void DrawLane()
{
	SetLight(FALSE);
	SetTexture(TEX_LANE);
	SetScale3D(1.0f, 1.0f, 3000.0f);
	SetPolygonColor3D(D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawPolygon3D
	(
		g_Lane.vPos.x,
		g_Lane.vPos.y,
		g_Lane.vPos.z,
		0,
		0,
		GetTextureStatus(TEX_LANE).u,
		GetTextureStatus(TEX_LANE).v
	);
}

void InitLaneEffect()
{
	for (int nCountLaneEffect = 0; nCountLaneEffect < gc_nMaxLaneEffect; nCountLaneEffect++)
	{
		g_LaneEffect[nCountLaneEffect].tLaneEffect = GetTextureStatus(TEX_LANE_EFFECT);
		g_LaneEffect[nCountLaneEffect].fScale = 3000.0f / (g_LaneEffect[nCountLaneEffect].tLaneEffect.v * 1.0f);
		g_LaneEffect[nCountLaneEffect].nU = 0;
		g_LaneEffect[nCountLaneEffect].nV = 0;
		g_LaneEffect[nCountLaneEffect].vPos = gc_vLaneEffectInitPos[nCountLaneEffect];
	}
}

void UninitLaneEffect()
{

}

void UpdateLaneEffect()
{
	for (int nCountLaneEffect = 0; nCountLaneEffect < gc_nMaxLaneEffect; nCountLaneEffect++)
	{
		g_LaneEffect[nCountLaneEffect].nV -= 10;
		if (g_LaneEffect[nCountLaneEffect].nV < -g_LaneEffect[nCountLaneEffect].tLaneEffect.v)
		{
			g_LaneEffect[nCountLaneEffect].nV += g_LaneEffect[nCountLaneEffect].tLaneEffect.v;
		}
	}
}

void DrawLaneEffect()
{
	SetTexture(TEX_LANE_EFFECT);
	SetBlend(D3DBLEND_BLENDFACTOR);
	for (int nCountLaneEffect = 0; nCountLaneEffect < gc_nMaxLaneEffect; nCountLaneEffect++)
	{
		SetScale3D(1.0f, 1.0f, g_LaneEffect[nCountLaneEffect].fScale);
		SetPolygonColor3D(D3DCOLOR_ARGB(127, 255, 255, 255));
		DrawPolygon3D
		(
			g_LaneEffect[nCountLaneEffect].vPos.x,
			g_LaneEffect[nCountLaneEffect].vPos.y + 0.0001f,
			g_LaneEffect[nCountLaneEffect].vPos.z,
			0,
			g_LaneEffect[nCountLaneEffect].nV,
			g_LaneEffect[nCountLaneEffect].tLaneEffect.u,
			g_LaneEffect[nCountLaneEffect].tLaneEffect.v * g_LaneEffect[nCountLaneEffect].fScale
		);
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
}