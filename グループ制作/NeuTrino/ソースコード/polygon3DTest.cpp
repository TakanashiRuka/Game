#include "main.h"
#include "polygon3Dtest.h"
#include "Texture.h"
#include <d3dx9.h>

#define MAX_NUM_POL                  (24)

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static D3DXMATRIX g_MtxWorld;
static D3DXMATRIX g_MtxWorldY;
static D3DXMATRIX g_MtxWorldZ;

static D3DXMATRIX g_MtxTrans;
static D3DXMATRIX g_MtxScale;
static D3DXMATRIX g_MtxRotX;
static D3DXMATRIX g_MtxRotY;
static D3DXMATRIX g_MtxRotZ;

static D3DMATERIAL9 g_matGrid;

static float g_fRot = 0.0f;

static float g_fTranX = 0.0f;
static float g_fTranY = 0.0f;
static float g_fTranZ = 0.0f;

static float g_fScaleX = 1.0f;
static float g_fScaleY = 1.0f;
static float g_fScaleZ = 1.0f;

static bool g_bScale = false;

static float g_fRotX = 0.0f;
static float g_fRotY = 0.0f;
static float g_fRotZ = 0.0f;

/*#include <vector>
using namespace std;  */

static int g_nTexNum = 0;
//static vector<int> nTest;


void Initpolygon3DTest()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	//ÉèÅ[ÉãÉh
	D3DXMatrixIdentity(&g_MtxWorld);

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX3DTEX) * 24,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D,
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL
	);

	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndexBuffer,
		NULL
	);

}

void Uninitpolygon3DTest()
{
	if (g_pVertexBuffer != NULL)
	{
		g_pVertexBuffer->Release();	   
		g_pVertexBuffer = NULL;
	}
	if (g_pIndexBuffer != NULL)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
}

void Updatepolygon3DTest()
{

}

void SetScale3DTest(float x, float y, float z)
{
	g_fScaleX = x;
	g_fScaleY = y;
	g_fScaleZ = z;

	g_bScale = true;
}

void SetRotate3DTest(float x, float y, float z)
{
	g_fRotX = x;
	g_fRotY = y;
	g_fRotZ = z;
}

void Drawpolygon3DCrid()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	int nMax = 11;

	for (int nCount = 0; nCount < nMax; nCount++)
	{
		int nColor = 0;
		VERTEX3D v[] =
		{
			{ D3DXVECTOR3(0.0f - 1.0f * (nMax / 2 * 1.0f),0.0f, 0.0f - 1.0f * (nMax / 2 * 1.0f) + (nCount * 1.0f)),D3DCOLOR_ARGB(255,nColor,nColor,nColor) },
			{ D3DXVECTOR3(0.0f + 1.0f * (nMax / 2 * 1.0f),0.0f, 0.0f - 1.0f * (nMax / 2 * 1.0f) + (nCount * 1.0f)),D3DCOLOR_ARGB(255,nColor,nColor,nColor) },
		};

		pDevice->SetFVF(FVF_VERTEX3D);

		//äeéÌçsóÒÇÃê›íË
		pDevice->SetTransform
		(
			D3DTS_WORLD,
			&g_MtxWorld
		);

		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->DrawPrimitiveUP
		(
			D3DPT_LINELIST,
			1,
			&v[0],
			sizeof(VERTEX3D)
		);

	}

	for (int nCount = 0; nCount < nMax; nCount++)
	{
		int nColor = 0;
		VERTEX3D v[] =
		{
			{ D3DXVECTOR3(0.0f - 1.0f * (nMax / 2 * 1.0f) + (nCount * 1.0f),0.0f, 0.0f - 1.0f * (nMax / 2 * 1.0f)),D3DCOLOR_ARGB(255,nColor,nColor,nColor) },
			{ D3DXVECTOR3(0.0f - 1.0f * (nMax / 2 * 1.0f) + (nCount * 1.0f),0.0f, 0.0f + 1.0f * (nMax / 2 * 1.0f)),D3DCOLOR_ARGB(255,nColor,nColor,nColor) },
		};

		pDevice->SetFVF(FVF_VERTEX3D);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//äeéÌçsóÒÇÃê›íË
		pDevice->SetTransform
		(
			D3DTS_WORLD,
			&g_MtxWorld
		);

		pDevice->DrawPrimitiveUP
		(
			D3DPT_LINELIST,
			1,
			&v[0],
			sizeof(VERTEX3D)
		);

	}

}

void Drawpolygon3DTest(float x, float y ,float z)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	D3DXVECTOR3 vPos[] =
	{
		{ -0.5f,-0.5f,0.5f },
		{ 0.5f,-0.5f,0.5f },
		{ 0.5f,0.5f,0.5f },
		{ -0.5f,0.5f,0.5f },
		{ -0.5f,-0.5f,-0.5f },
		{ 0.5f,-0.5f,-0.5f },
		{ 0.5f,0.5f,-0.5f },
		{ -0.5f,0.5f,-0.5f }
	};

	D3DXVECTOR3 vPosIndex[] =
	{
		vPos[0],
		vPos[1],
		vPos[2],
		vPos[3],
		
		vPos[1],
		vPos[5],
		vPos[6],
		vPos[2],
		
		vPos[5],
		vPos[4],
		vPos[7],
		vPos[6],

		vPos[4],
		vPos[0],
		vPos[3],
		vPos[7],
		
		vPos[3],
		vPos[2],
		vPos[6],
		vPos[7],

		vPos[4],
		vPos[5],
		vPos[1],
		vPos[0],
	};

	WORD Index[] =
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,
		
		8, 9, 10,
		8, 10, 11,
		
		12, 13, 14,
		12, 14, 15,
		
		16, 17, 18,
		16, 18, 19,
		
		20, 21, 22,
		20, 22, 23,
	};

	D3DXMatrixRotationZ(&g_MtxWorldZ, 0.0f);

	if (g_fRotX != 0.0f)
	{
		D3DXMatrixRotationX(&g_MtxRotX, g_fRotX);
		D3DXMatrixMultiply(&g_MtxWorldZ, &g_MtxWorldZ, &g_MtxRotX);
		g_fRotX = 0.0f;
	}
	if (g_fRotY != 0.0f)
	{
		D3DXMatrixRotationY(&g_MtxRotY, g_fRotY);
		D3DXMatrixMultiply(&g_MtxWorldZ, &g_MtxWorldZ, &g_MtxRotY);
		g_fRotY = 0.0f;
	}
	if (g_fRotZ != 0.0f)
	{
		D3DXMatrixRotationZ(&g_MtxRotZ, g_fRotZ);
		D3DXMatrixMultiply(&g_MtxWorldZ, &g_MtxWorldZ, &g_MtxRotZ);
		g_fRotZ = 0.0f;
	}  

	//D3DXMatrixScaling()        ägëÂèkè¨
	//D3DXMatrixRotationAxis()	 îCà”é≤ÇÃâÒì]

	if (g_bScale)
	{
		D3DXMatrixScaling(&g_MtxScale, g_fScaleX, g_fScaleY, g_fScaleZ);
		D3DXMatrixMultiply(&g_MtxWorldZ, &g_MtxWorldZ, &g_MtxScale);

		g_bScale = false;
	}
	else
	{
		g_fScaleX = g_fScaleY = g_fScaleZ = 1.0f;
	}

	D3DXMatrixTranslation(&g_MtxTrans, x, y, z);

	D3DXMatrixMultiply(&g_MtxWorldZ, &g_MtxWorldZ, &g_MtxTrans);


	VERTEX3DTEX v[MAX_NUM_POL];

	D3DXVECTOR2 UV[] =
	{
		{ 0.0f,0.0f },
		{ 0.25f,0.0f },
		{ 0.5f,0.0f },
		{ 0.75f,0.0f },
		{ 0.0f,0.25f },
		{ 0.25f,0.25f }
	};

	D3DXVECTOR3 vNormalList[] =
	{
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 0.0, -1.0 },

		{ 1.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0 },

		{ 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0 },

		{ -1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 },

		{ 0.0, -1.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, -1.0, 0.0 },

		{ 0.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },

	};


	for (int nCount = 0; nCount < MAX_NUM_POL; nCount++)
	{
		D3DXVECTOR2 vTexcList[] =
		{
			{ 0.0f,0.0f },
			{ 0.25f,0.0f },
			{ 0.25f,0.25f },
			{ 0.0f,0.25f }
		};

		D3DXVECTOR2 vTexcoord[] =
		{
			vTexcList[0] + UV[nCount / 4],
			vTexcList[1] + UV[nCount / 4],
			vTexcList[2] + UV[nCount / 4],
			vTexcList[3] + UV[nCount / 4]
		};

		v[nCount].vPos = vPosIndex[nCount];
		v[nCount].vNormal = vNormalList[nCount];
		v[nCount].color = D3DCOLOR_ARGB(255, 255, 255, 255);
		v[nCount].texcoord = vTexcoord[nCount % 4];
	}

	VERTEX3DTEX* pV;

	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
	memcpy(&pV[0], &v[0], sizeof(VERTEX3DTEX) * 24);
	g_pVertexBuffer->Unlock();

	LPWORD pIndex;

	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
	memcpy(&pIndex[0], &Index[0], sizeof(WORD) * 36);
	g_pIndexBuffer->Unlock();

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3DTEX));
	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->SetFVF(FVF_VERTEX3D_TEX);

	//äeéÌçsóÒÇÃê›íË
	pDevice->SetTransform
	(
		D3DTS_WORLD,
		&g_MtxWorldZ
	);

	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLELIST,
		0,
		0,
		24,
		0,
		12
	);

}

