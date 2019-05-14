//----------------------------------------------------------------------------
//
//	�Q�[�W
//
//														 ����ҁF����
//                                                       �쐬���F2017/12/23
//----------------------------------------------------------------------------

//---------------------------�C���N���[�h-------------------------------------
#include "main.h"
#include "Texture.h"
#include "Light.h"
#include "debug.h"
#include "Note.h"
#include "Judge.h"
#include "Gauge.h"
#include "ReadNoteFile.h"
#include "SelectSongPanel.h"
#include "Polygon.h"

//---------------------------�}�N����`---------------------------------------

//---------------------------�񋓌^��`---------------------------------------

//---------------------------�\���̒�`---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;
	float fGaugeNum;							// �Q�[�W�̒l
	float fGaugeNumPerNote;						// �����̃Q�[�W�̒l
	float fUScale;								//
	int nV;										//
	TEXTURE tTextureStatus;						//
}GAUGE;											// �Q�[�W


typedef struct {								
	D3DXVECTOR2 vPos;							//
	int nA;										//
	TEXTURE tTextureStatus;						//
}GAUGE_BOARD;									//

typedef struct {
	D3DXVECTOR2 vPos;							//
	int nA;										//
	TEXTURE tTextureStatus;						//
}GAUGE_BG;										//

//---------------------------�O���[�o���ϐ�-----------------------------------
static GAUGE g_Gauge;
static GAUGE_BOARD g_GaugeBoard;
static GAUGE_BG g_GaugeBG;
static GAUGE_BG g_GaugeClearLine;

static const float gc_fMaxGuage = 10000.0f;						// �Q�[�W�̍ő�l
static const float gc_fGaugeClear = 7000.0f;					// �Q�[�W�̃N���A�l
static const float gc_fMaxGaugePerLevel[MAX_LEVEL]=				// ��Փx���ƃQ�[�W�̍ő�l
{
	23500.0f,
	20500.0f,	
	17500.0f,
};

static const float gc_fGaugeScalePerJudgeLevel[JL_MAX] =		// ���背�x�����Ƃ̃Q�[�W���
{
	1.0f,
	1.0f,
	0.5f,
	0.0f
};

static const float gc_fGaugeAdjustmentBad[NT_MAX] =				// ���肪�o�b�g�̎��̃Q�[�W����
{
	200.0f,
	200.0f,
	100.0f,
	200.0f,
	100.0f
};

//---------------------------�v���g�^�C�v�錾---------------------------------
void InitGauge()
{
	g_Gauge.fGaugeNum = 0;
	g_Gauge.fGaugeNumPerNote = gc_fMaxGaugePerLevel[GetLevel()] / GetMaxNote();
	g_Gauge.fUScale = 0.0f;
	g_Gauge.nV = 0;
	g_Gauge.tTextureStatus = GetTextureStatus(TEX_GAME_GAUGE_INSIDE);
	g_Gauge.vPos =
	{
		WINDOW_WIDTH / 2.0f,
		27.0f
	};
}

void UninitGauge()
{

}

void UpdateGauge()
{
	g_Gauge.fUScale = g_Gauge.fGaugeNum / gc_fMaxGuage;
	if (g_Gauge.fGaugeNum >= gc_fGaugeClear)
	{
		g_Gauge.nV = 1;
	}
	else
	{
		g_Gauge.nV = 0;
	}
}

void DrawGauge()
{
	DebugText(0, 396, "Gauge: %f, %f", g_Gauge.fGaugeNum, g_Gauge.fGaugeNumPerNote);
	SetZBuffer(FALSE);
	SetUVSTA(D3DTADDRESS_BORDER);
	SetTexture(TEX_GAME_GAUGE_INSIDE);
	SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	SetScale(g_Gauge.fUScale, 1.0f, 0.0f, 0.5f);
	DrawPolygon
	(
		g_Gauge.vPos.x,
		g_Gauge.vPos.y,
		0,
		g_Gauge.tTextureStatus.v * g_Gauge.nV,
		g_Gauge.tTextureStatus.u * g_Gauge.fUScale,
		g_Gauge.tTextureStatus.v
	);
	SetUVSTA(D3DTADDRESS_WRAP);
	SetZBuffer(TRUE);
}

void SetGaugeNum(int nNoteType, int nJudgeLevel)
{
	if (nJudgeLevel != JL_BAD)
	{
		g_Gauge.fGaugeNum += g_Gauge.fGaugeNumPerNote * gc_fGaugeScalePerJudgeLevel[nJudgeLevel];
	}
	else
	{
		g_Gauge.fGaugeNum -= gc_fGaugeAdjustmentBad[nNoteType];
	}
	if (g_Gauge.fGaugeNum > gc_fMaxGuage)
	{
		g_Gauge.fGaugeNum = gc_fMaxGuage;
	}
	else
		if (g_Gauge.fGaugeNum < 0)
		{
			g_Gauge.fGaugeNum = 0;
		}
}

//----------------------------------------------------------------
// �Q�[�W�{�[�h
//----------------------------------------------------------------
void InitGaugeBoard()
{
	g_GaugeBoard.nA = 255;
	g_GaugeBoard.tTextureStatus = GetTextureStatus(TEX_GAME_GAUGE_BOARD);
	g_GaugeBoard.vPos =
	{
		WINDOW_WIDTH / 2.0f,
		g_GaugeBoard.tTextureStatus.v / 2.0f - 2.0f
	};
}

void UninitGaugeBoard()
{

}

void UpdateGaugeBoard()
{

}

void DrawGaugeBoard()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_GAME_GAUGE_BOARD);
	SetPolygonColor(D3DCOLOR_ARGB(g_GaugeBoard.nA, 255, 255, 255));
	DrawPolygon
	(
		g_GaugeBoard.vPos.x,
		g_GaugeBoard.vPos.y,
		0,
		0,
		g_GaugeBoard.tTextureStatus.u,
		g_GaugeBoard.tTextureStatus.v
	);
	SetZBuffer(TRUE);
}

//---------------------------------------------------------------
// �Q�[�W�w�i
//---------------------------------------------------------------
void InitGaugeBG()
{
	g_GaugeBG.nA = 255;
	g_GaugeBG.tTextureStatus = GetTextureStatus(TEX_GAME_GAUGE_BG);
	g_GaugeBG.vPos =
	{
		WINDOW_WIDTH / 2.0f,
		27.0f
	};
}

void UninitGaugeBG()
{

}

void UpdateGaugeBG()
{

}

void DrawGaugeBG()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_GAME_GAUGE_BG);
	SetPolygonColor(D3DCOLOR_ARGB(g_GaugeBG.nA, 255, 255, 255));
	DrawPolygon
	(
		g_GaugeBG.vPos.x,
		g_GaugeBG.vPos.y,
		0,
		0,
		g_GaugeBG.tTextureStatus.u,
		g_GaugeBG.tTextureStatus.v
	);
	SetZBuffer(TRUE);
}

//----------------------------------------------------------------
// �Q�[�W�̍��i���C��
//----------------------------------------------------------------
void InitGaugeClearLine()
{
	g_GaugeClearLine.nA = 255;
	g_GaugeClearLine.tTextureStatus = GetTextureStatus(TEX_GAME_GAUGE_CLEAR_LINE);
	g_GaugeClearLine.vPos =
	{
		WINDOW_WIDTH / 2.0f - (GetTextureStatus(TEX_GAME_GAUGE_BG).u * 0.5f) + (GetTextureStatus(TEX_GAME_GAUGE_BG).u * gc_fGaugeClear / gc_fMaxGuage),
		27.0f
	};

}

void UninitGaugeClearLine()
{

}

void UpdateGaugeClearLine()
{

}

void DrawGaugeClearLine()
{
	SetZBuffer(FALSE);
	SetTexture(TEX_GAME_GAUGE_CLEAR_LINE);
	SetPolygonColor(D3DCOLOR_ARGB(g_GaugeClearLine.nA, 255, 255, 255));
	DrawPolygon
	(
		g_GaugeClearLine.vPos.x,
		g_GaugeClearLine.vPos.y,
		0,
		0,
		g_GaugeClearLine.tTextureStatus.u,
		g_GaugeClearLine.tTextureStatus.v
	);
	SetZBuffer(TRUE);
}

bool GetClearStatus()
{
	if (g_Gauge.fGaugeNum >= gc_fGaugeClear)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ResetGauge()
{
	g_Gauge.fGaugeNum = 0;
}