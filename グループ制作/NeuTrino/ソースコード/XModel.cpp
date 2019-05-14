#include "main.h"
#include "XModel.h"
#include "polygon3Dtest.h"
#include "debug.h"

#define MAX_TEX								(10)

typedef struct {
	char cFlieAddress[128];
	LPD3DXBUFFER pAdjacency;
	LPD3DXBUFFER pMaterialBuffer;
	LPD3DXMATERIAL pMaterial;
	DWORD nMaterialNum;
	LPD3DXMESH pMesh;
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEX];
}XMODEL;

static LPD3DXMESH g_pTempMesh;	  // Global Memory
static D3DXMATRIX g_MtxWorld;
static D3DXMATRIX g_MtxTrans;
static D3DXMATRIX g_MtxScale;
static D3DXMATRIX g_MtxRotX;
static D3DXMATRIX g_MtxRotY;
static D3DXMATRIX g_MtxRotZ;

static int g_nXMNum = 0;

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

static XMODEL g_XModel[XM_MAX]=
{
	{ "Data/XModel/cart/cart.x"} ,
};

void InitXModel()
{
	//ワールド
	D3DXMatrixIdentity(&g_MtxWorld);

	HRESULT hr;	// Heap Memory

	for (int i = 0; i < XM_MAX; i++)
	{
		hr = D3DXLoadMeshFromX
		(
			g_XModel[i].cFlieAddress,
			D3DXMESH_SYSTEMMEM,
			GetD3DDevice(),
			&g_XModel[i].pAdjacency,
			&g_XModel[i].pMaterialBuffer,
			NULL,
			&g_XModel[i].nMaterialNum,
			&g_XModel[i].pMesh
		);

		hr = g_XModel[i].pMesh->OptimizeInplace
		(
			D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
			(DWORD*)g_XModel[i].pAdjacency->GetBufferPointer(),
			NULL,
			NULL,
			NULL
		);

		if (g_XModel[i].pAdjacency)
		{
			g_XModel[i].pAdjacency->Release();
		}

		D3DVERTEXELEMENT9 pElement[MAXD3DDECLLENGTH + 1];
		g_XModel[i].pMesh->GetDeclaration(pElement);
		hr = g_XModel[i].pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, pElement, GetD3DDevice(), &g_pTempMesh);

		if (g_XModel[i].pMesh)
		{
			g_XModel[i].pMesh->Release();
		}
		g_XModel[i].pMesh = g_pTempMesh;

		//マテリアルの取得
		g_XModel[i].pMaterial = (LPD3DXMATERIAL)g_XModel[i].pMaterialBuffer->GetBufferPointer();

		for (int j = 0; j < g_XModel[i].nMaterialNum; j++)
		{
			hr = D3DXCreateTextureFromFile
			(
				GetD3DDevice(),
				g_XModel[i].pMaterial[j].pTextureFilename,
				&g_XModel[i].pTexture[j]
			);

			if (FAILED(hr))
			{
				MessageBox(NULL, "テクスチャが読み込めなかった", "エラー", MB_OK);
			}
		}

	}
}

void UninitXModel()
{
	for (int i = 0; i < XM_MAX; i++)
	{
		if (g_XModel[i].pMesh)
		{
			g_XModel[i].pMesh->Release();
			g_XModel[i].pMesh = NULL;
		}
		for (unsigned int j = 0; j < g_XModel[i].nMaterialNum; j++)
		{
			if (g_XModel[i].pTexture[j] != NULL)
			{
				g_XModel[i].pTexture[j]->Release();
				g_XModel[i].pTexture[j] = NULL;
			}
		} 
	}
}

void SetXModel(int nXMNum)
{
	g_nXMNum = nXMNum;
}

void SetXMRot(float x, float y, float z)
{
	g_fRotX = x;
	g_fRotY = y;
	g_fRotZ = z;
}

void SetXMScale(float x, float y, float z)
{
	g_fScaleX = x;
	g_fScaleY = y;
	g_fScaleZ = z;

	g_bScale = true;
}

void DrawXModel(float x, float y, float z)
{
	D3DXMatrixRotationZ(&g_MtxWorld, 0.0f);

	if (g_fRotX != 0.0f)
	{
		D3DXMatrixRotationX(&g_MtxRotX, g_fRotX);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxRotX);
		g_fRotX = 0.0f;
	}
	if (g_fRotY != 0.0f)
	{
		D3DXMatrixRotationY(&g_MtxRotY, g_fRotY);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxRotY);
		g_fRotY = 0.0f;
	}
	if (g_fRotZ != 0.0f)
	{
		D3DXMatrixRotationZ(&g_MtxRotZ, g_fRotZ);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxRotZ);
		g_fRotZ = 0.0f;
	}


	D3DXMatrixTranslation(&g_MtxTrans, x, y, z);

	D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxTrans);

	if (g_bScale)
	{
		D3DXMatrixScaling(&g_MtxScale, g_fScaleX, g_fScaleY, g_fScaleZ);
		D3DXMatrixMultiply(&g_MtxWorld, &g_MtxWorld, &g_MtxScale);

		g_bScale = false;
	}
	else
	{
		g_fScaleX = g_fScaleY = g_fScaleZ = 1.0f;
	}

	for (int i = 0; i < g_XModel[g_nXMNum].nMaterialNum; i++)
	{

		//各種行列の設定
		GetD3DDevice()->SetTransform
		(
			D3DTS_WORLD,
			&g_MtxWorld
		);

		GetD3DDevice()->SetMaterial(&g_XModel[g_nXMNum].pMaterial[i].MatD3D);
		GetD3DDevice()->SetTexture(0, g_XModel[g_nXMNum].pTexture[i]);
		g_XModel[g_nXMNum].pMesh->DrawSubset(i);
	}
}