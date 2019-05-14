//----------------------------------------------------------------------------
//
//	コンボ
//
//														 制作者：劉喆
//                                                       作成日：2017/12/25
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Texture.h"
#include "Light.h"
#include "debug.h"
#include "Btn.h"
#include "Note.h"
#include "Judge.h"
#include "Combo.h"
#include "Polygon.h"
#include "Polygon3D.h"
#include "number.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------
typedef struct {
	int nCombo;				// 今のコンボ
	int nComboMax;			// 最大コンボ
	D3DXVECTOR3 vPos;		// 座標
	int nCountEffect;		// エフェクトのカウンター
}COMBO;						// コンボ

typedef struct {
	D3DXVECTOR3 vPos;		// 座標
	int nA;					// α値
	TEXTURE tTextureStatus;	// テクスチャデータ
}COMBO_WORD;				// コンボ文字

typedef struct {
	D3DXVECTOR2 vPos;
	int nA;
	TEXTURE tTextureStatus;	// テクスチャデータ
}MAX_COMBO;

//---------------------------グローバル変数-----------------------------------
static COMBO g_Combo;
static COMBO_WORD g_ComboWord;
static MAX_COMBO g_MaxComboWord;
static MAX_COMBO g_MaxComboNum;

static const D3DXVECTOR3 gc_vComboNumInitPos = { 0.0f,0.0f,0.0f };			//
static const D3DXVECTOR3 gc_vComboNumEndPos = { 0.0f,0.1f,0.0f };			//
static const D3DXVECTOR3 gc_vComboWordInitPos = { 0.0f,0.0f,100.0f };		//
static const int gc_nComboEffectCountMax = 4;								//

static const int gc_nDrawMinCombo = 5;										// コンボを表示させる最小値
static const int gc_nComboA = 127;											// コンボのα値
static const float gc_fComboRangeX = 100.0f;								// 数字の距離

static const int gc_nMaxComboWordInitA = 255;								//
static const D3DXVECTOR2 gc_vMaxComboWordInitPos = { 1175.0f,95.0f };		//
static const D3DXVECTOR2 gc_vMaxComboNumInitPos = { 1247.0f,95.0f };		//
static const float gc_fMaxComboRangeX = 8.0f;								// 数字の距離

//---------------------------プロトタイプ宣言---------------------------------
void SetComboEffect();

void InitCombo()
{
	g_Combo.nCombo = 0;
	g_Combo.nComboMax = 0;
	g_Combo.nCountEffect = 0;
	g_Combo.vPos = gc_vComboNumInitPos;
}

void UninitCombo()
{

}

void UpdateCombo()
{
	if (g_Combo.nCombo > g_Combo.nComboMax)
	{
		g_Combo.nComboMax = g_Combo.nCombo;
	}
	if (g_Combo.nCountEffect < gc_nComboEffectCountMax * 2)
	{
		g_Combo.nCountEffect++;

		if (g_Combo.nCountEffect < gc_nComboEffectCountMax)
		{
			g_Combo.vPos += (gc_vComboNumEndPos - gc_vComboNumInitPos) / (gc_nComboEffectCountMax * 1.0f);
		}
		else
			if (g_Combo.nCountEffect == gc_nComboEffectCountMax)
			{
				g_Combo.vPos = gc_vComboNumEndPos;
			}
			else
				if (g_Combo.nCountEffect < gc_nComboEffectCountMax * 2)
				{
					g_Combo.vPos += (-gc_vComboNumEndPos + gc_vComboNumInitPos) / (gc_nComboEffectCountMax * 1.0f);
				}
				else
					if (g_Combo.nCountEffect == gc_nComboEffectCountMax * 2)
					{
						g_Combo.vPos = gc_vComboNumInitPos;
					}
	}
}

void DrawCombo()
{
	DebugText(0, 412, "Combo: %d, %d", g_Combo.nCombo, g_Combo.nComboMax);
	if (g_Combo.nCombo >= gc_nDrawMinCombo)
	{
		SetZBuffer(FALSE);
		SetLight(FALSE);
		SetBlend(D3DBLEND_BLENDFACTOR);
		TEXTURE tComboNum = GetTextureStatus(TEX_GAME_COMBO_NUM);
		int n_ComboBackUp = g_Combo.nCombo;
		int nDigit = 0;

		do
		{
			n_ComboBackUp /= 10;
			nDigit++;
		} while (n_ComboBackUp != 0);

		float fInitX = (nDigit - 1) * gc_fComboRangeX * 0.5;

		n_ComboBackUp = g_Combo.nCombo;
		int count;
		for (count = 0; count < nDigit; count++)
		{
			int n = n_ComboBackUp % 10;
			DrawNumber3D(TEX_GAME_COMBO_NUM, tComboNum.u, tComboNum.v, n, fInitX - (count)* gc_fComboRangeX, g_Combo.vPos.y, g_Combo.vPos.z, gc_nComboA);
			n_ComboBackUp /= 10;
		}
		SetZBuffer(TRUE);
		SetBlend(D3DBLEND_INVSRCALPHA);
	}
}

void SetCombo(int nComboCommend)
{
	switch (nComboCommend)
	{
	case CC_ADD:
	{
		g_Combo.nCombo += 1;
		SetComboEffect();
		break;
	}
	case CC_RESET:
	{
		g_Combo.nCombo = 0;
		break;
	}
	}	
}

//---------------------------------------------------------------
// コンボ文字
//---------------------------------------------------------------
void InitComboWord()
{
	g_ComboWord.nA = gc_nComboA;
	g_ComboWord.tTextureStatus = GetTextureStatus(TEX_GAME_COMBO_WORD);
	g_ComboWord.vPos = gc_vComboWordInitPos;
}

void UninitComboWord()
{

}

void UpdateComboWord()
{

}

void DrawComboWord()
{
	if (g_Combo.nCombo >= gc_nDrawMinCombo)
	{
		SetZBuffer(FALSE);
		SetLight(FALSE);
		SetBlend(D3DBLEND_BLENDFACTOR);
		SetTexture(TEX_GAME_COMBO_WORD);
		SetPolygonColor3D(D3DCOLOR_ARGB(g_ComboWord.nA, 255, 255, 255));
		DrawPolygon3D
		(
			g_ComboWord.vPos.x,
			g_ComboWord.vPos.y,
			g_ComboWord.vPos.z,
			0,
			0,
			g_ComboWord.tTextureStatus.u,
			g_ComboWord.tTextureStatus.v
		);
		SetZBuffer(TRUE);
		SetBlend(D3DBLEND_INVSRCALPHA);
	}
}

void SetComboEffect()
{
	g_Combo.nCountEffect = 0;
	g_Combo.vPos = gc_vComboNumInitPos;
}

//---------------------------------------------------------------
// 最大コンボ数文字
//---------------------------------------------------------------
void InitMaxComboWord()
{
	g_MaxComboWord.nA = gc_nMaxComboWordInitA;
	g_MaxComboWord.tTextureStatus = GetTextureStatus(TEX_GAME_MAX_COMBO_WORD);
	g_MaxComboWord.vPos = gc_vMaxComboWordInitPos;
}

void UninitMaxComboWord()
{

}

void UpdateMaxComboWord()
{

}

void DrawMaxComboWord()
{
	SetTexture(TEX_GAME_MAX_COMBO_WORD);
	SetPolygonColor(D3DCOLOR_ARGB(g_MaxComboWord.nA, 255, 255, 255));
	DrawPolygon
	(
		g_MaxComboWord.vPos.x,
		g_MaxComboWord.vPos.y,
		0,
		0,
		g_MaxComboWord.tTextureStatus.u,
		g_MaxComboWord.tTextureStatus.v
	);
}

//---------------------------------------------------------------
// 最大コンボ数字
//---------------------------------------------------------------
void InitMaxComboNum()
{
	g_MaxComboNum.nA = gc_nMaxComboWordInitA;
	g_MaxComboNum.tTextureStatus = GetTextureStatus(TEX_GAME_MAX_COMBO_NUM);
	g_MaxComboNum.vPos = gc_vMaxComboNumInitPos;
}

void UninitMaxComboNum()
{

}

void UpdateMaxComboNum()
{

}

void DrawMaxComboNum()
{
	int n_ComboMaxBackUp = g_Combo.nComboMax;
	int nDigit = 0;

	do
	{
		n_ComboMaxBackUp /= 10;
		nDigit++;
	} while (n_ComboMaxBackUp != 0);

	float fInitX = gc_vMaxComboNumInitPos.x + (nDigit - 1) * gc_fMaxComboRangeX * 0.5f;

	n_ComboMaxBackUp = g_Combo.nComboMax;
	int count;
	for (count = 0; count < nDigit; count++)
	{
		int n = n_ComboMaxBackUp % 10;
		DrawNumber(TEX_GAME_MAX_COMBO_NUM, g_MaxComboNum.tTextureStatus.u, g_MaxComboNum.tTextureStatus.v, n, fInitX - (count)* gc_fMaxComboRangeX, g_MaxComboNum.vPos.y, gc_nMaxComboWordInitA);
		n_ComboMaxBackUp /= 10;
	}
}

int GetMaxCombo()
{
	return g_Combo.nComboMax;
}
