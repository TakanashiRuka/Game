//----------------------------------------------------------------------------
//
//	カメラ基礎
//
//														 制作者：劉喆
//                                                       作成日：2017/11/01
//----------------------------------------------------------------------------

#include "main.h"
#include "Camera.h"
#include "input.h"
#include <d3d9.h>
#include <d3dx9.h>

#define SPEED					(0.1f)

static D3DXMATRIX g_MtxView;
static D3DXMATRIX g_MtxProj;

static D3DXVECTOR3 eye(0.0f, 10.0f, 4.0f);	 //0.0f, -15.0f, -5.0f
static D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
static D3DXVECTOR3 up(0.0f, -2.0f, 0.0f);

static D3DXVECTOR3 g_vUp(0.0f, 10.0f, 1.0f);
static D3DXVECTOR3 g_vFront(1.0f, 1.0f, 1.0f);
static D3DXVECTOR3 g_vRight(1.0f, 1.0f, 1.0f);
static D3DXVECTOR3 g_vAt(0.0f, 4.0f, 10.0f);
static D3DXVECTOR3 g_vEye(0.0f, 2.0f, 0.0f);

static D3DXMATRIX g_MtxRotX;
static D3DXMATRIX g_MtxRotY;
static D3DXMATRIX g_MtxRotZ;

static const float g_fTransSpeed = 1.0f;
static const float g_fRotSpeed = D3DXToRadian(60);

static bool g_bModeFPS = true;

static int g_nCameraMode = 0;

static float g_fCameraAngle;
static float g_fCameraAngleTarget;
static float g_fCameraAngleDir;

static int g_nCameraAngleCountMax = 15;
static int g_nCameraAngleCount;

static const D3DXVECTOR3 g_vEyeMode[MODE_MAX]=
{
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,6.5f,0.0f },
	{ 0.0f, 4.0f, -5.0f },
	{ 0.0f, 4.0f, -5.0f },
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f },
};

static float g_fCameraY;
static float g_fCameraYTarget;
static float g_fCameraYDir;

static const int gc_nCameraYCountMax = 10;
static int g_nCameraYCount;

static const float gc_fCameraChangeZ = 0.483f;

void InitCamera()
{
	g_vAt = { 0.0f, 0.0f, 0.0f };
	g_vEye = { 0.0f, 4.0f, -5.0f };

	g_vUp = { 0.0f, 10.0f, 1.0f };
	g_vFront = { 1.0f, 1.0f, 1.0f };
	g_vRight = { 1.0f, 1.0f, 1.0f };

	g_vFront = g_vAt - g_vEye;

	//D3DXVec3Normalize(&g_vFront, &g_vFront);
	//D3DXVec3Normalize(&g_vRight, &g_vRight);
	//D3DXVec3Normalize(&g_vUp, &g_vUp);	 
}

void SetCamera(int nGameMode)
{
	g_vAt = { 0.0f, 0.0f, 0.0f };
	g_vEye = g_vEyeMode[nGameMode];
	g_vUp = { 0.0f, 10.0f, 1.0f };
	g_vFront = { 1.0f, 1.0f, 1.0f };
	g_vRight = { 1.0f, 1.0f, 1.0f };
	g_vFront = g_vAt - g_vEye;
	g_nCameraMode = nGameMode;

	g_fCameraAngle = 0.0f;
	g_fCameraAngleTarget = 0.0f;
	g_fCameraAngleDir = g_fCameraAngleTarget - g_fCameraAngle;
	g_nCameraAngleCount = 0;

	g_fCameraY = 0.0f;
	g_fCameraYTarget = 0.0f;
	g_fCameraYDir = g_fCameraYTarget - g_fCameraY;
	g_nCameraAngleCount = 0;
}

void ChangeCameraByKey()
{
#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_1))
	{
		if (g_bModeFPS)
		{
			g_bModeFPS = false;
		}
		else
		{
			g_bModeFPS = true;
		}
	}

	if (GetKeyboardPress(DIK_LEFT))
	{
		D3DXMatrixRotationZ(&g_MtxRotY, -0.01f);
		D3DXVec3TransformNormal(&g_vFront, &g_vFront, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vRight, &g_vRight, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vUp, &g_vUp, &g_MtxRotY);
		if (g_bModeFPS)
		{
			g_vAt = g_vEye + g_vFront;
		}
		else
		{
			g_vEye = g_vAt - g_vFront;
		}
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		D3DXMatrixRotationZ(&g_MtxRotY, 0.01f);
		D3DXVec3TransformNormal(&g_vFront, &g_vFront, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vRight, &g_vRight, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vUp, &g_vUp, &g_MtxRotY);
		if (g_bModeFPS)
		{
			g_vAt = g_vEye + g_vFront;
		}
		else
		{
			g_vEye = g_vAt - g_vFront;
		}
	}

	if (GetKeyboardPress(DIK_UP))
	{
		D3DXMatrixRotationX(&g_MtxRotY, 0.01f);
		D3DXVec3TransformNormal(&g_vFront, &g_vFront, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vRight, &g_vRight, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vUp, &g_vUp, &g_MtxRotY);
		if (g_bModeFPS)
		{
			g_vAt = g_vEye + g_vFront;
		}
		else
		{
			g_vEye = g_vAt - g_vFront;
		}
	}

	if (GetKeyboardPress(DIK_DOWN))
	{
		D3DXMatrixRotationX(&g_MtxRotY, -0.01f);
		D3DXVec3TransformNormal(&g_vFront, &g_vFront, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vRight, &g_vRight, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vUp, &g_vUp, &g_MtxRotY);
		if (g_bModeFPS)
		{
			g_vAt = g_vEye + g_vFront;
		}
		else
		{
			g_vEye = g_vAt - g_vFront;
		}
	}

	D3DXVECTOR3 vec;

	if (GetKeyboardPress(DIK_NUMPAD8))
	{
		vec = g_vFront;
		vec.y = 0.0f;
		D3DXVec3Normalize(&vec, &vec);
		vec *= SPEED;
		g_vAt += vec;
		g_vEye += vec;
	}

	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		vec = g_vFront;
		vec.y = 0.0f;
		D3DXVec3Normalize(&vec, &vec);
		vec *= SPEED;
		g_vAt -= vec;
		g_vEye -= vec;
	}

	if (GetKeyboardPress(DIK_NUMPAD4))
	{
		vec = g_vRight;
		vec.y = 0.0f;
		D3DXVec3Normalize(&vec, &vec);
		vec *= SPEED;
		g_vAt -= vec;
		g_vEye -= vec;
	}

	if (GetKeyboardPress(DIK_NUMPAD6))
	{
		vec = g_vRight;
		vec.y = 0.0f;
		D3DXVec3Normalize(&vec, &vec);
		vec *= SPEED;
		g_vAt += vec;
		g_vEye += vec;
	}

	if (GetKeyboardPress(DIK_NUMPAD7))
	{
		vec = g_vUp;
		vec.x = 0.0f;
		vec.z = 0.0f;
		D3DXVec3Normalize(&vec, &vec);

		vec *= SPEED;
		//g_vAt += vec;
		g_vEye += vec;
	}

	if (GetKeyboardPress(DIK_NUMPAD9))
	{
		vec = g_vUp;
		vec.x = 0.0f;
		vec.z = 0.0f;
		D3DXVec3Normalize(&vec, &vec);
		vec *= SPEED;
		//g_vAt -= vec;
		g_vEye -= vec;
	}

	D3DXVec3Cross(&g_vRight, &g_vUp, &g_vFront);

	//float a = D3DXVec3Dot(&g_vFront, &g_vFront);

#endif // _DEBUG
}

void UpdateCamera()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	//ビュー
	D3DXMatrixLookAtLH
	(
		&g_MtxView,
		&g_vEye,
		&g_vAt,
		&g_vUp
	);

	D3DXMatrixPerspectiveFovLH
	(
		&g_MtxProj,
		D3DXToRadian(60),
		(float)WINDOW_WIDTH / WINDOW_HEIGHT,
		0.1f,
		25.0f
	);

	pDevice->SetTransform
	(
		D3DTS_VIEW,
		&g_MtxView
	);

	pDevice->SetTransform
	(
		D3DTS_PROJECTION,
		&g_MtxProj
	);
}

D3DXMATRIX GetCameraView()
{
	return g_MtxView;
}

int GetCameraMode()
{
	return g_nCameraMode;
}

void ResetCameraAngleTarget()
{
	g_fCameraAngleTarget = 0.0f;
	g_fCameraAngleDir = g_fCameraAngleTarget - g_fCameraAngle;
	g_nCameraAngleCount = 0;
}

void SetCameraAngleTarget(float fAngle)
{

	g_fCameraAngleTarget += fAngle;

	g_fCameraAngleDir = g_fCameraAngleTarget - g_fCameraAngle;
	g_nCameraAngleCount = 0;
}

void ChangeCameraAngle()
{
	if (g_nCameraAngleCount < g_nCameraAngleCountMax)
	{
		g_nCameraAngleCount++;

		g_fCameraAngle += g_fCameraAngleDir / (g_nCameraAngleCountMax * 1.0f);
		D3DXMatrixRotationZ(&g_MtxRotY, g_fCameraAngleDir / (g_nCameraAngleCountMax * 1.0f));
		D3DXVec3TransformNormal(&g_vFront, &g_vFront, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vRight, &g_vRight, &g_MtxRotY);
		D3DXVec3TransformNormal(&g_vUp, &g_vUp, &g_MtxRotY);
		if (g_bModeFPS)
		{
			g_vAt = g_vEye + g_vFront;
		}
		else
		{
			g_vEye = g_vAt - g_vFront;
		}

		if (g_nCameraAngleCount == g_nCameraAngleCountMax)
		{
			//g_vAt = { 0.0f, 0.0f, 0.0f };
			//g_vEye = g_vEyeMode[GetGameMode()];
			//g_vUp = { 0.0f, 10.0f, 1.0f };
			//g_vFront = { 1.0f, 1.0f, 1.0f };
			//g_vRight = { 1.0f, 1.0f, 1.0f };
			//g_vFront = g_vAt - g_vEye;

			//D3DXMatrixRotationZ(&g_MtxRotY, g_fCameraAngleTarget);
			//D3DXVec3TransformNormal(&g_vFront, &g_vFront, &g_MtxRotY);
			//D3DXVec3TransformNormal(&g_vRight, &g_vRight, &g_MtxRotY);
			//D3DXVec3TransformNormal(&g_vUp, &g_vUp, &g_MtxRotY);
			//if (g_bModeFPS)
			//{
			//	g_vAt = g_vEye + g_vFront;
			//}
			//else
			//{
			//	g_vEye = g_vAt - g_vFront;
			//}
			g_fCameraAngle = g_fCameraAngleTarget;
		}
		D3DXVec3Cross(&g_vRight, &g_vUp, &g_vFront);
	}
}

void ResetCameraY()
{
	g_fCameraYTarget = 0.0f;
	g_fCameraYDir = g_fCameraYTarget - g_fCameraY;
	g_nCameraYCount = 0;
}

void SetCameraYTarget(float fY)
{
	g_fCameraYTarget += fY;
	g_fCameraYDir = g_fCameraYTarget - g_fCameraY;
	g_nCameraYCount = 0;
}

void ChangeCameraY()
{
	if (g_nCameraYCount < gc_nCameraYCountMax)
	{
		g_nCameraYCount++;
		g_vEye.y += g_fCameraYDir / (gc_nCameraYCountMax * 1.0f);
		g_vEye.z -= (g_fCameraYDir / (gc_nCameraYCountMax * 1.0f)) * gc_fCameraChangeZ;
		g_fCameraY += g_fCameraYDir / (gc_nCameraYCountMax * 1.0f);
		if (g_nCameraYCount == gc_nCameraYCountMax)
		{
			g_fCameraY = g_fCameraYTarget;
		}
	}
}

void SetChangeAngleCountMax(int nNum)
{
	g_nCameraAngleCountMax = nNum;
}