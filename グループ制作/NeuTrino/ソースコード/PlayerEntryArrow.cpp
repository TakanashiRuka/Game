#include "PlayerEntryArrow.h"
#include "main.h"
#include "Polygon.h"
#include "Texture.h"

typedef struct {
	D3DXVECTOR2 vPos;			// ���W
	int nA;						// ���l
	int nFadeInCount;			// �t�F�[�h�C���J�E���^�[
}PLAYER_ENTRY_ARROW;				// �ȑI�����(�����p)

static const int gc_nInitA = 0;											// ���l(�����l)
static const int gc_nMaxA = 255;										// ���l(�I���l)
static const int gc_nMaxFadeInCount = 10;								// �t�F�[�h�C���J�E���^�[�̍ő�l
static const D3DXVECTOR2 gc_vInitPos = { WINDOW_WIDTH / 2.0f,360.0f };	// ���W(�����l)
static const float gc_fInitPos = 180.0f;
static const int gc_AnimControl = 1;

static PLAYER_ENTRY_ARROW g_PlayerEntryArrow;
static float g_fPos;
static float g_fAnimPos;
static int g_nAnimCnt;

void InitPlayerEntryArrow()
{
	g_PlayerEntryArrow.nA = gc_nInitA;
	g_PlayerEntryArrow.vPos = gc_vInitPos;
	g_PlayerEntryArrow.nFadeInCount = 0;
	g_fPos = gc_fInitPos;
	g_fAnimPos = 0.33f;
}

void UninitPlayerEntryArrow()
{

}

void UpdatePlayerEntryArrow()
{
	g_nAnimCnt++;
	if (g_PlayerEntryArrow.nFadeInCount < gc_nMaxFadeInCount)
	{
		g_PlayerEntryArrow.nFadeInCount++;
		g_PlayerEntryArrow.nA += (gc_nMaxA - gc_nInitA) * 1.0f / (gc_nMaxFadeInCount * 1.0f);
		if (g_PlayerEntryArrow.nFadeInCount == gc_nMaxFadeInCount)
		{
			g_PlayerEntryArrow.nA = gc_nMaxA;
		}
	}
	if (g_nAnimCnt % gc_AnimControl == 0)
	{
		g_fPos += g_fAnimPos;
	}
	if ((g_fPos > 180.0f) || (g_fPos < 170.0f))
	{
		g_fAnimPos *= -1.0f;
		if (g_fPos > 180.0f)
		{
			g_fPos = 180.0f;
		}
		if (g_fPos < 170.0f)
		{
			g_fPos = 170.0f;
		}
	}
}

void DrawPlayerEntryArrow()
{
	SetTexture(TEX_PLAYER_ENTRY_ARROW);
	SetPolygonColor(D3DCOLOR_ARGB(g_PlayerEntryArrow.nA, 255, 255, 255));
	DrawPolygon
	(
		g_PlayerEntryArrow.vPos.x - g_fPos,
		g_PlayerEntryArrow.vPos.y,
		0,
		0,
		GetTextureStatus(TEX_PLAYER_ENTRY_ARROW).u,
		GetTextureStatus(TEX_PLAYER_ENTRY_ARROW).v
	);
	SetPolygonColor(D3DCOLOR_ARGB(g_PlayerEntryArrow.nA, 255, 255, 255));
	DrawPolygon
	(
		g_PlayerEntryArrow.vPos.x + g_fPos,
		g_PlayerEntryArrow.vPos.y,
		GetTextureStatus(TEX_PLAYER_ENTRY_ARROW).u,
		0,
		GetTextureStatus(TEX_PLAYER_ENTRY_ARROW).u,
		GetTextureStatus(TEX_PLAYER_ENTRY_ARROW).v
	);
}