#include "main.h"
#include "3DMeshField.h"
#include "Texture.h"
#include "polygon3Dtest.h"
#include "debug.h"

#define NUM_W						(20)
#define NUM_H						(20)
#define NUM_POINT					((NUM_W + 1) * (NUM_H + 1))
#define NUM_INDEX					(NUM_POINT + ((NUM_W + 1) * (NUM_H - 1)) + (NUM_H - 1) * 2)
#define NUM_TRI						(NUM_INDEX - 2)
#define MESH_LENGTH					(1.0f)

static D3DXMATRIX g_MtxWorld;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
static VERTEX3DTEX g_v[NUM_POINT];
static D3DXVECTOR3 g_vPosInit[NUM_POINT];
static WORD g_wIndex[NUM_INDEX];
static int g_nCounter;

void Init3DMeshField()
{
	g_nCounter = 0;
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	//ワールド
	D3DXMatrixIdentity(&g_MtxWorld);

	int nCountPoint = 0;

	for (int nCountH = 0; nCountH < NUM_H + 1; nCountH++)
	{
		for (int nCountW = 0; nCountW < NUM_W + 1; nCountW++)
		{
			g_vPosInit[nCountPoint] = g_v[nCountPoint].vPos =
			{
				(-NUM_W * MESH_LENGTH / 2.0f) + MESH_LENGTH * nCountW,
				0.0f,
				(NUM_H * MESH_LENGTH / 2.0f) - MESH_LENGTH * nCountH
			};

			if (NUM_W % 2 == 0)
			{
				g_v[nCountPoint].texcoord.x = (g_v[nCountPoint].vPos.x) / MESH_LENGTH;
			}
			else
			{
				g_v[nCountPoint].texcoord.x = (g_v[nCountPoint].vPos.x - MESH_LENGTH / 2.0f) / MESH_LENGTH;
			}
			if (NUM_H % 2 == 0)
			{
				g_v[nCountPoint].texcoord.y = (-g_v[nCountPoint].vPos.z) / MESH_LENGTH;
			}
			else
			{
				g_v[nCountPoint].texcoord.y = (-g_v[nCountPoint].vPos.z - MESH_LENGTH / 2.0f) / MESH_LENGTH;
			}

			nCountPoint += 1;
		}
	}

	int nW = 0;
	int nH = 0;
	int nCountIndexOld = 0;
	for (int nCountIndex = 0; nCountIndex < NUM_INDEX; nCountIndex++)
	{
		if (nCountIndex - nCountIndexOld == 2)
		{
			nCountIndexOld = nCountIndex;
			nW++;
		}

		if (nW == NUM_W + 1)
		{
			g_wIndex[nCountIndex] = nW - 1 + (NUM_W + 1) * (nH);
			nW = 0;
			nH++;
			g_wIndex[nCountIndex + 1] = nW + (NUM_W + 1) * (nH + 1);
			nCountIndex += 2;
			nCountIndexOld += 2;
		}
		if (nCountIndex % 2 == 0)
		{
			g_wIndex[nCountIndex] = nW + (NUM_W + 1) * (nH + 1);
		}
		else
		{
			g_wIndex[nCountIndex] = nW + (NUM_W + 1) * (nH);
		}

	}

	//float fMaxLength = g_v[0].vPos.x * g_v[0].vPos.x + g_v[0].vPos.z * g_v[0].vPos.z;


	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX3DTEX) * NUM_POINT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D,
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL
	);

	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * NUM_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndexBuffer,
		NULL
	);
}

void Uninit3DMeshField()
{

}

void Update3DMeshField()
{
	g_nCounter++;
}

void Draw3DMeshField()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	for (int nCountPoint = 0; nCountPoint < NUM_INDEX; nCountPoint++)
	{
		DebugText(0, nCountPoint * 20.0f, "%d: %d", nCountPoint, g_wIndex[nCountPoint]);
	}

	for (int nCountPoint = 0; nCountPoint < NUM_POINT; nCountPoint++)
	{
		g_v[nCountPoint].color = D3DCOLOR_ARGB
		(
			255,
			255,
			255,
			255
		);

		g_v[nCountPoint].vNormal = { 0.0f,-1.0f,0.0f };
	}

	VERTEX3DTEX* pV;

	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
	memcpy(&pV[0], &g_v[0], sizeof(VERTEX3DTEX) * NUM_POINT);
	g_pVertexBuffer->Unlock();

	LPWORD pIndex;

	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
	memcpy(&pIndex[0], &g_wIndex[0], sizeof(WORD) * NUM_INDEX);
	g_pIndexBuffer->Unlock();

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3DTEX));
	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->SetFVF(FVF_VERTEX3D_TEX);
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMatrixRotationZ(&g_MtxWorld, 0.0f);

	//各種行列の設定
	pDevice->SetTransform
	(
		D3DTS_WORLD,
		&g_MtxWorld
	);

	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		NUM_POINT,
		0,
		NUM_TRI
	);

}
