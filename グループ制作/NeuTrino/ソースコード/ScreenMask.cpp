//----------------------------------------------------------------------------
//
//	�S��ʃ}�X�N
//
//														 ����ҁF����
//                                                       �쐬���F2018/02/12
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "ScreenMask.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	int nA;
	int nAOld;
	int nADir;
	int nCount;
	bool bStatus;
}SCREEN_MASK;

//---------------------------�O���[�o���ϐ�-----------------------------------
static SCREEN_MASK g_ScreenMask;

static const int gc_nScreenMaskA[RDP_MAX] = { 0,127 };
static int g_nScreenMaskCountMax;

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitScreenMask()
{
	g_nScreenMaskCountMax = 10;
	g_ScreenMask.bStatus = false;
	g_ScreenMask.nA = gc_nScreenMaskA[RDP_INIT];
	g_ScreenMask.nAOld = g_ScreenMask.nA;
	g_ScreenMask.nADir = g_ScreenMask.nA - g_ScreenMask.nAOld;
	g_ScreenMask.nCount = 0;
}

void UninitScreenMask()
{

}

void UpdateScreenMask()
{
	if (g_ScreenMask.nAOld != g_ScreenMask.nA)
	{
		g_ScreenMask.nCount++;
		g_ScreenMask.nAOld += (g_ScreenMask.nADir * 1.0f) / (g_nScreenMaskCountMax * 1.0f);
		if (g_ScreenMask.nAOld < gc_nScreenMaskA[RDP_INIT])
		{
			g_ScreenMask.nAOld = gc_nScreenMaskA[RDP_INIT];
		}
		if (g_ScreenMask.nAOld > gc_nScreenMaskA[RDP_END])
		{
			g_ScreenMask.nAOld = gc_nScreenMaskA[RDP_END];
		}
		if (g_ScreenMask.nCount == g_nScreenMaskCountMax)
		{
			g_ScreenMask.nAOld = g_ScreenMask.nA;
			if (g_ScreenMask.bStatus)
			{
				g_ScreenMask.bStatus = false;
			}
		}
	}
}

void DrawScreenMask()
{
	if (g_ScreenMask.nAOld != gc_nScreenMaskA[RDP_INIT])
	{
		SetZBuffer(FALSE);

		SetTexture(TEX_SLIDE_SHADOW);
		TEXTURE tTexture = GetTextureStatus(TEX_SLIDE_SHADOW);
		SetPolygonColor(D3DCOLOR_ARGB(g_ScreenMask.nAOld, 0, 0, 0));
		SetScale(WINDOW_WIDTH, WINDOW_HEIGHT);
		DrawPolygon
		(
			0,
			0,
			0,
			0,
			tTexture.u,
			tTexture.v
		);

		SetZBuffer(TRUE);
	}
}

void SetScreenMask(int nMaxCount, int nType)
{
	g_nScreenMaskCountMax = nMaxCount;
	g_ScreenMask.nA = gc_nScreenMaskA[nType];
	g_ScreenMask.nADir = g_ScreenMask.nA - g_ScreenMask.nAOld;
	g_ScreenMask.nCount = 0;
	g_ScreenMask.bStatus = true;
}