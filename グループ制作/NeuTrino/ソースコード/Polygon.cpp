//--------------------------------------------------------------------------------------------------
//
// ポリゴンの描画
//																				    2017.06.05 RYU
//--------------------------------------------------------------------------------------------------

#include "main.h"
#include "Polygon.h"   
#include "Texture.h"   

// 構造体定義
typedef struct {
	D3DXVECTOR4 pos;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}VERTEX_2D;

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static float g_fScale = 1.0f;
static float g_fScaleAdd = 0.01f;
static D3DCOLOR color = 0xffffffff;
static float ScaleX = 1.0f;
static float ScaleY = 1.0f;
static bool ScaleStatus = false;
static int g_ncx;
static int g_ncy;

static float g_fCenterX;
static float g_fCenterY;
static float g_fPolyAngle;

static void CreateVertex(float dx, float dy, int tcx, int tcy, int tcw, int tch);
static void CreateVertexAffine(float dx, float dy, int tcx, int tcy, int tcw, int tch);

// ポリゴンの初期化
bool InitPolygon()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if (pDevice == NULL)
	{
		return false;
	}

	HRESULT hr;

	hr = pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファが作れなかった", "エラー", MB_OK);
		return false;
	}
	return true;
}

// ポリゴンの終了
void UninitPolygon(void)
{
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}


// ポリゴンの更新
void UpdatePolygon(void)
{

}

void SetPolygonColor(D3DCOLOR nColor)
{
	color = nColor;
}

void SetScale(float x, float y, int cx, int cy)
{
	g_ncx = cx;
	g_ncy = cy;
	ScaleX = x;
	ScaleY = y;
	ScaleStatus = true;
}

// ポリゴンの描画
void DrawPolygon(float dx, float dy,int tcx, int tcy, int tcw, int tch, bool bAffine)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if (pDevice == NULL)
	{
		return;
	}

	if (!bAffine)
	{
		CreateVertex(dx, dy, tcx, tcy, tcw, tch);
	}
	else
	{
		CreateVertexAffine(dx, dy, tcx, tcy, tcw, tch);
	}

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX_2D));

	// FVF(今から使用する頂点情報)の設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLEFAN,
		0,
		2
	); 
}

void CreateVertex(float dx, float dy, int tcx, int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	if (!ScaleStatus)
	{
		ScaleX = 1.0f;
		ScaleY = 1.0f;
		g_ncx = 0;
		g_ncy = 0;
	}
	else
	{
		ScaleStatus = false;
	}

	float wd = (float)(GetTextureStatus(GetTextureNum()).u) * (ScaleX);
	float wh = (float)(GetTextureStatus(GetTextureNum()).v) * (ScaleY);
	int tw = GetTextureStatus(GetTextureNum()).width;
	int th = GetTextureStatus(GetTextureNum()).height;

	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;

	float OffsetX = (g_ncx - GetTextureStatus(GetTextureNum()).u / 2.0f) * (ScaleX - 1.0f);
	float OffsetY = (g_ncy - GetTextureStatus(GetTextureNum()).v / 2.0f) * (ScaleY - 1.0f);

	pV[0].pos = D3DXVECTOR4{ dx - wd / 2.0f - OffsetX + 0.5f,dy - wh / 2.0f - OffsetY + 0.5f,1.0f,1.0f };
	pV[1].pos = D3DXVECTOR4{ dx + wd / 2.0f + 0.5f - OffsetX,dy - wh / 2.0f - OffsetY + 0.5f,1.0f,1.0f };
	pV[2].pos = D3DXVECTOR4{ dx + wd / 2.0f + 0.5f - OffsetX,dy + wh / 2.0f + 0.5f - OffsetY,1.0f,1.0f };
	pV[3].pos = D3DXVECTOR4{ dx - wd / 2.0f - OffsetX + 0.5f,dy + wh / 2.0f + 0.5f - OffsetY,1.0f,1.0f };

	pV[0].color =
	pV[1].color =
	pV[2].color =
	pV[3].color = color;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u1, v1);
	pV[3].texcoord = D3DXVECTOR2(u0, v1);

	g_pVertexBuffer->Unlock();
}

void CreateVertexAffine(float dx, float dy, int tcx, int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	if (!ScaleStatus)
	{
		ScaleX = 1.0f;
		ScaleY = 1.0f;
		g_ncx = 0;
		g_ncy = 0;
	}
	else
	{
		ScaleStatus = false;
	}

	float wd = GetTextureStatus(GetTextureNum()).u * ScaleX;
	float wh = GetTextureStatus(GetTextureNum()).v * ScaleY;
	int tw = GetTextureStatus(GetTextureNum()).width;
	int th = GetTextureStatus(GetTextureNum()).height;

	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;

	float OffsetX = (g_ncx - GetTextureStatus(GetTextureNum()).u / 2.0f) * ScaleX;
	float OffsetY = (g_ncy - GetTextureStatus(GetTextureNum()).v / 2.0f) * ScaleY;

	float x = -(g_fCenterX / GetTextureStatus(GetTextureNum()).width) * wd;
	float x_w = wd - (g_fCenterX / GetTextureStatus(GetTextureNum()).width) * wd;
	float y = -(g_fCenterY / GetTextureStatus(GetTextureNum()).height) * wh;
	float y_h = wh - (g_fCenterY / GetTextureStatus(GetTextureNum()).height) * wh;

	pV[0].pos = D3DXVECTOR4
	{
		x * cosf(g_fPolyAngle) - y * sinf(g_fPolyAngle) + dx + (wd / 2.0f) - OffsetX,
		x * sinf(g_fPolyAngle) + y * cosf(g_fPolyAngle) + dy + (wh / 2.0f) - OffsetY,
		1.0f,
		1.0f
	};
	pV[1].pos = D3DXVECTOR4
	{
		x_w * cosf(g_fPolyAngle) - y * sinf(g_fPolyAngle) + dx + (wd / 2.0f) - OffsetX,
		x_w * sinf(g_fPolyAngle) + y * cosf(g_fPolyAngle) + dy + (wh / 2.0f) - OffsetY,
		1.0f,
		1.0f
	};
	pV[2].pos = D3DXVECTOR4
	{
		x_w * cosf(g_fPolyAngle) - y_h * sinf(g_fPolyAngle) + dx + (wd / 2.0f) - OffsetX,
		x_w * sinf(g_fPolyAngle) + y_h * cosf(g_fPolyAngle) + dy + (wh / 2.0f) - OffsetY,
		1.0f,
		1.0f
	};
	pV[3].pos = D3DXVECTOR4
	{
		x * cosf(g_fPolyAngle) - y_h * sinf(g_fPolyAngle) + dx + (wd / 2.0f) - OffsetX,
		x * sinf(g_fPolyAngle) + y_h * cosf(g_fPolyAngle) + dy + (wh / 2.0f) - OffsetY,
		1.0f,
		1.0f
	};

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = color;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u1, v1);
	pV[3].texcoord = D3DXVECTOR2(u0, v1);

	g_pVertexBuffer->Unlock();
}
