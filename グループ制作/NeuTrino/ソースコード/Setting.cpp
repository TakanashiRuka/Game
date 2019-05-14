//----------------------------------------------------------------------------
//
//	ゲーム設定
//
//														 制作者：劉喆
//                                                       作成日：2018/01/18
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Polygon.h"
#include "Texture.h"
#include "debug.h"
#include "Setting.h"
#include "input.h"
#include "JoyStick.h"
#include "Note.h"
#include "JudgeSound.h"
#include "sound.h"
#include "music.h"
#include "SelectSongWaitMember.h"
#include "BtnManual.h"
#include "SelectSongFinalCheck.h"
#include "data.h"
#include "ScreenMask.h"

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------
typedef enum {
	SP_BOARD,
	SP_BOARD_TITLE_WORD,
	SP_BOARD_SPEED,
	SP_BOARD_OFFSET,
	SP_BOARD_SE,
	SP_BOARD_F_SPEED,
	SP_BOARD_F_OFFSET,
	SP_BOARD_F_SE,
	SP_MAX
}SETTING_PARTS;

typedef enum {
	OT_TEXTURE,
	OT_FONT,
	OT_MAX
}OBJECT_TYPE;

typedef enum {
	SAP_LEFT,
	SAP_RIGHT,
	SAP_MAX
}SETTING_ARROW_PARTS;

typedef enum {
	SL_SPEED,
	SL_OFFSET,
	SL_SE,
	SL_MAX
}SETTING_LIST;

typedef enum {
	SMM_MIN,
	SMM_MAX,
	SMM_MAX_NUM
}SETTING_MAX_MIN;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR2 vPos;			// 座標
	D3DXVECTOR2 vOldPos;
	D3DXVECTOR2 vAddPos;
	//int nA;					// α値
	int nCount;					// カウンター
}SETTING_BOARD;					// 設定の背景

typedef struct {
	D3DXVECTOR2 vPos;			// 座標
	int nA;						// α値
	int nU;						//
}SETTING_ARROW;					// 設定の矢印

typedef struct {
	D3DXVECTOR2 vPos;			// 座標
	int nA;						// 目標のα値
	int nOldA;					// 実際のα値
	int nAddA;					// 加算すべきα値
	int nCount;					// カウンター
	int nV;						// Vの値
}SETTING_MANUAL;				// 設定項目の説明

//---------------------------グローバル変数-----------------------------------
SETTING_BOARD g_SettingBoard[SP_MAX];

static const int gc_nSettingTextureNum[SP_MAX]=						// 設定画面のテクスチャ番号
{
	TEX_SETTING_BOARD,
	TEX_SETTING_TITLE_WORD,
	TEX_SETTING_WORD,
	TEX_SETTING_WORD,
	TEX_SETTING_WORD,
	TEX_SETTING_WORD,
	TEX_SETTING_WORD,
	TEX_SETTING_WORD,
};

static const int gc_nObjectType[SP_MAX] =							// 描画させるものの種類
{
	OT_TEXTURE,
	OT_TEXTURE,
	OT_TEXTURE,
	OT_TEXTURE,
	OT_TEXTURE,
	OT_FONT,
	OT_FONT,
	OT_FONT,
};

static const D3DXVECTOR2 gc_vSettingAddPos = { 0.0f,350.0f };		// 座標の変化値

static const float gc_fSettingListInitY = -270.0f;
static const float gc_fSettingListRangeY= 67.0f;

static const float gc_fSettingFontInitY = -255.0f;

static const D3DXVECTOR2 gc_vSettingBoardInitPos[SP_MAX] =					// 設定の背景の初期座標
{
	{ WINDOW_WIDTH / 2.0f, -176.0f },
	{ WINDOW_WIDTH / 2.0f, -320.0f },
	{ WINDOW_WIDTH / 2.0f, gc_fSettingListInitY + 0 * gc_fSettingListRangeY },
	{ WINDOW_WIDTH / 2.0f, gc_fSettingListInitY + 1 * gc_fSettingListRangeY },
	{ WINDOW_WIDTH / 2.0f, gc_fSettingListInitY + 2 * gc_fSettingListRangeY },
	{ 0.0f, gc_fSettingFontInitY + 0 * gc_fSettingListRangeY },
	{ 0.0f, gc_fSettingFontInitY + 1 * gc_fSettingListRangeY },
	{ 0.0f, gc_fSettingFontInitY + 2 * gc_fSettingListRangeY },
};

static const int gc_nSettingBoardV[SP_MAX] =
{
	0,
	0,
	0,
	1,
	2,
	0,
	1,
	2,
};

static const int gc_nSettingFadeInOutMaxCount = 6;					// 設定のフェードイン/アウトのカウンターの最大値
static const float gc_fMusicVolume[RDP_MAX] = { 0.5f,1.0f };
static float g_fMusicVolume;
static bool g_bMusicVolumeChange;

static bool g_bSettingStatus;										// キーを押していた時の設定状態
static bool g_bSetting;												// 設定のアニメーション状態

//--------------------------------------------------------------------
// 設定の矢印
//--------------------------------------------------------------------
static SETTING_ARROW g_SettingArrow[SAP_MAX];

static const D3DXVECTOR2 gc_vSettingArrowRange = { 70.0f, 0.0f };
static const D3DXVECTOR2 gc_vSettingArrowInitPos = { WINDOW_WIDTH / 2.0f, 107.0f };
static const int gc_nSettingArrowA[RDP_MAX] = { 255, 63 };

static int g_nSettingArrowSelect;
static const int gc_nSettingArrowSelectMax = 2;
static const int gc_nSettingArrowSelectMin = 0;

static const float gc_fSettingList[SL_MAX][SMM_MAX_NUM] = 
{
	{1.0f,15.0f},
	{ -99.0f,99.0f },
	{ 0.0f,100.0f},
};

static const float gc_fSettingListChangeUnit[SL_MAX] =
{
	0.5f,1.0f,10.0f
};

//-----------------------------------------------------------------
// 設定項目の説明
//-----------------------------------------------------------------
static SETTING_MANUAL g_SettingManual;

static const D3DXVECTOR2 gc_vSettingManualInitPos = { WINDOW_WIDTH / 2.0f,415.0f };
static const float gc_fSettingManualListChangeY = -9.0f;
static const int gc_nSettingManualA[SMM_MAX_NUM] = { 0,255 };

//---------------------------プロトタイプ宣言---------------------------------
void InitSetting()
{
	g_fMusicVolume = gc_fMusicVolume[RDP_END];
	g_bMusicVolumeChange = false;
	g_bSettingStatus = false;
	g_bSetting = false;
	InitText(F_SETTING, 25, "MS コジック");
	InitSettingBoard();
	InitSettingArrow();
	InitSettingManual();
}

void UninitSetting()
{
	UninitSettingBoard();
	UninitText(F_SETTING);
	UninitSettingArrow();
	UninitSettingManual();
}

void UpdateSetting()
{
	if ((GetKeyboardTrigger(DIK_SPACE) || GetJoyStickTrigger(JOY_TRI)) && !GetFadeFlag() && !GetRoomReady() && !GetErrorListStatus())
	{
		if (!g_bSettingStatus)
		{
			if (!g_bSetting)
			{
				if(GetPlayStatus(SE_SETTING_OPEN))
				{
					StopSound(SE_SETTING_OPEN);
				}
				PlaySound(SE_SETTING_OPEN);
				g_bSettingStatus = true;
				g_bSetting = true;
				SetSettingBoard(gc_vSettingAddPos);
				SetSettingManualA(SMM_MAX);
				SetBtnManualTarget(BMP_SETTING);
				SetBtnManualPos(RDP_INIT);
				SetScreenMask(gc_nSettingFadeInOutMaxCount, RDP_END);
			}
		}
		else
		{
			if (!g_bSetting)
			{
				if (GetPlayStatus(SE_SETTING_CLOSE))
				{
					StopSound(SE_SETTING_CLOSE);
				}
				PlaySound(SE_SETTING_CLOSE);
				g_bSettingStatus = false;
				g_bSetting = true;
				SetSettingBoard(-gc_vSettingAddPos);
				SetSettingManualA(SMM_MIN);
				if (!GetSelectSongFinalCheckStatus())
				{
					SetBtnManualTarget(BMP_SELECT_SONG);
				}
				else
					if (GetSelectSongFinalCheckStatus() && !GetTestModeMemberStatus(TMML_NETWORK))
					{
						SetBtnManualTarget(BMP_FINAL_CHECK_OFFLINE);
					}
					else
						if (GetSelectSongFinalCheckStatus() && GetTestModeMemberStatus(TMML_NETWORK))
						{
							if (GetRoomCreate() || GetRoomEntry())
							{
								SetBtnManualTarget(BMP_DESTROY_ROOM_GET_READY);
							}
							else
							{
								SetBtnManualTarget(BMP_FINAL_CHECK_ONLINE);
							}
						}
				SetBtnManualPos(RDP_INIT);
				SetScreenMask(gc_nSettingFadeInOutMaxCount, RDP_INIT);
			}
		}
	}
	UpdateSettingBoard();
	UpdateSettingArrow();
	UpdateSettingManual();
}

void DrawSetting()
{
	DrawSettingBoard();
	DrawSettingArrow();
	DrawSettingManual();
}

bool GetSettingStatus()
{
	bool bSetting = g_bSetting | g_bSettingStatus;
	return bSetting;
}

//----------------------------------------------------------------
// 設定の背景
//----------------------------------------------------------------
void InitSettingBoard()
{
	for (int nCountSettingParts = 0; nCountSettingParts < SP_MAX; nCountSettingParts++)
	{
		g_SettingBoard[nCountSettingParts].nCount = 0;
		g_SettingBoard[nCountSettingParts].vPos = gc_vSettingBoardInitPos[nCountSettingParts];
		g_SettingBoard[nCountSettingParts].vOldPos = gc_vSettingBoardInitPos[nCountSettingParts];
		g_SettingBoard[nCountSettingParts].vAddPos = { 0.0f,0.0f };
	}
}

void UninitSettingBoard()
{

}

void UpdateSettingBoard()
{
	g_bMusicVolumeChange = false;
	for (int nCountSettingParts = 0; nCountSettingParts < SP_MAX; nCountSettingParts++)
	{
		if (g_SettingBoard[nCountSettingParts].vOldPos != g_SettingBoard[nCountSettingParts].vPos)
		{
			if (g_SettingBoard[nCountSettingParts].nCount < gc_nSettingFadeInOutMaxCount)
			{
				g_SettingBoard[nCountSettingParts].nCount++;
				// 音量の変更
				if (!g_bMusicVolumeChange)
				{
					if (g_bSettingStatus)
					{
						g_fMusicVolume -= (gc_fMusicVolume[RDP_END] - gc_fMusicVolume[RDP_INIT]) / (gc_nSettingFadeInOutMaxCount * 1.0f);
					}
					else
					{
						g_fMusicVolume += (gc_fMusicVolume[RDP_END] - gc_fMusicVolume[RDP_INIT]) / (gc_nSettingFadeInOutMaxCount * 1.0f);
					}
					SetMusicVolume((MUSIC_LABEL)MUSIC, g_fMusicVolume);
					g_bMusicVolumeChange = true;
				}
				g_SettingBoard[nCountSettingParts].vOldPos += g_SettingBoard[nCountSettingParts].vAddPos / (gc_nSettingFadeInOutMaxCount * 1.0f);
				if (g_SettingBoard[nCountSettingParts].nCount == gc_nSettingFadeInOutMaxCount)
				{
					g_SettingBoard[nCountSettingParts].vOldPos = g_SettingBoard[nCountSettingParts].vPos;
					g_bSetting = false;
					if (g_bSettingStatus)
					{
						g_fMusicVolume = gc_fMusicVolume[RDP_INIT];
					}
					else
					{
						g_fMusicVolume = gc_fMusicVolume[RDP_END];
					}
					SetMusicVolume((MUSIC_LABEL)MUSIC, g_fMusicVolume);
				}
			}
		}
	}
}

void DrawSettingBoard()
{
	SetZBuffer(FALSE);
	for (int nCountSettingParts = 0; nCountSettingParts < SP_MAX; nCountSettingParts++)
	{
		if (gc_nObjectType[nCountSettingParts] == OT_TEXTURE)
		{
			SetTexture(gc_nSettingTextureNum[nCountSettingParts]);
			TEXTURE tSettingBoard = GetTextureStatus(gc_nSettingTextureNum[nCountSettingParts]);
			SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			DrawPolygon
			(
				g_SettingBoard[nCountSettingParts].vOldPos.x,
				g_SettingBoard[nCountSettingParts].vOldPos.y,
				0,
				tSettingBoard.v * gc_nSettingBoardV[nCountSettingParts],
				tSettingBoard.u,
				tSettingBoard.v
			);
		}
		if (gc_nObjectType[nCountSettingParts] == OT_FONT)
		{
			SetTextStatus(D3DCOLOR_ARGB(200, 255, 255, 255), D3DCOLOR_ARGB(200, 255, 255, 255), 0);
			SetTextStyle(DT_CENTER);
			switch (nCountSettingParts)
			{
			case SP_BOARD_F_SPEED:
			{
				OutputText
				(
					F_SETTING,
					g_SettingBoard[nCountSettingParts].vOldPos.x,
					g_SettingBoard[nCountSettingParts].vOldPos.y,
					"%.1f",GetHiSpeed()
				);
				break;
			}
			case SP_BOARD_F_OFFSET:
			{
				int nOffset = GetOffsetNote();
				if (nOffset < 0)
				{
					OutputText
					(
						F_SETTING,
						g_SettingBoard[nCountSettingParts].vOldPos.x,
						g_SettingBoard[nCountSettingParts].vOldPos.y,
						"%d", nOffset
					);
				}
				else
				{
					OutputText
					(
						F_SETTING,
						g_SettingBoard[nCountSettingParts].vOldPos.x,
						g_SettingBoard[nCountSettingParts].vOldPos.y,
						"+%d", nOffset
					);
				}
				break;
			}
			case SP_BOARD_F_SE:
			{
				OutputText
				(
					F_SETTING,
					g_SettingBoard[nCountSettingParts].vOldPos.x,
					g_SettingBoard[nCountSettingParts].vOldPos.y,
					"%d%%", GetJudgeSoundVolume()
				);
				break;
			}
			}
		}
	}
	SetZBuffer(TRUE);
}

void SetSettingBoard(D3DXVECTOR2 vAddPos)
{
	for (int nCountSettingParts = 0; nCountSettingParts < SP_MAX; nCountSettingParts++)
	{
		g_SettingBoard[nCountSettingParts].vPos += vAddPos;
		g_SettingBoard[nCountSettingParts].vAddPos = vAddPos;
		g_SettingBoard[nCountSettingParts].nCount = 0;
	}
}

//--------------------------------------------------------------------
// 設定の矢印
//--------------------------------------------------------------------
void InitSettingArrow()
{
	g_nSettingArrowSelect = gc_nSettingArrowSelectMin;
	for (int nCountSettingArrow = 0; nCountSettingArrow < SAP_MAX; nCountSettingArrow++)
	{
		g_SettingArrow[nCountSettingArrow].nA = gc_nSettingArrowA[RDP_INIT];
		g_SettingArrow[nCountSettingArrow].vPos = gc_vSettingArrowInitPos + (nCountSettingArrow * 2 - 1) * gc_vSettingArrowRange;
		g_SettingArrow[nCountSettingArrow].nU = nCountSettingArrow;
	}
}

void UninitSettingArrow()
{

}

void UpdateSettingArrow()
{
	if (g_bSettingStatus && !g_bSetting)
	{
		bool bTrigger = false;
		if (GetKeyboardTrigger(DIK_UP) || GetPOVTrigger(JOY_UP))
		{
			g_nSettingArrowSelect--;
			if (g_nSettingArrowSelect < gc_nSettingArrowSelectMin)
			{
				g_nSettingArrowSelect = gc_nSettingArrowSelectMin;
			}
			else
			{
				if (GetPlayStatus(SE_SELECT_SONG))
				{
					StopSound(SE_SELECT_SONG);
				}
				PlaySound(SE_SELECT_SONG);
			}
			bTrigger = true;
		}
		if (GetKeyboardTrigger(DIK_DOWN) || GetPOVTrigger(JOY_DOWN))
		{
			g_nSettingArrowSelect++;
			if (g_nSettingArrowSelect > gc_nSettingArrowSelectMax)
			{
				g_nSettingArrowSelect = gc_nSettingArrowSelectMax;
			}
			else
			{
				if (GetPlayStatus(SE_SELECT_SONG))
				{
					StopSound(SE_SELECT_SONG);
				}
				PlaySound(SE_SELECT_SONG);
			}
			bTrigger = true;
		}
		float fDir = 0.0f;

		if (GetKeyboardRepeat(DIK_LEFT) || GetPOVRepeat(JOY_LEFT))
		{
			fDir = -1.0f;
		}
		if (GetKeyboardRepeat(DIK_RIGHT) || GetPOVRepeat(JOY_RIGHT))
		{
			fDir = 1.0f;
		}

		if (fDir != 0.0f)
		{
			float fSave;
			switch (g_nSettingArrowSelect)
			{
			case SL_SPEED:
			{
				fSave = GetHiSpeed();
				break;
			}
			case SL_OFFSET:
			{
				fSave = GetOffsetNote();
				break;
			}
			case SL_SE:
			{
				fSave = GetJudgeSoundVolume();
				break;
			}
			}
			if (fSave >= gc_fSettingList[g_nSettingArrowSelect][SMM_MIN] &&
				fSave <= gc_fSettingList[g_nSettingArrowSelect][SMM_MAX])
			{
				fSave += fDir * gc_fSettingListChangeUnit[g_nSettingArrowSelect];
				if (fSave < gc_fSettingList[g_nSettingArrowSelect][SMM_MIN])
				{
					fSave = gc_fSettingList[g_nSettingArrowSelect][SMM_MIN];
				}
				if (fSave > gc_fSettingList[g_nSettingArrowSelect][SMM_MAX])
				{
					fSave = gc_fSettingList[g_nSettingArrowSelect][SMM_MAX];
				}
			}
			switch (g_nSettingArrowSelect)
			{
			case SL_SPEED:
			{
				SetHiSpeed(fSave);
				break;
			}
			case SL_OFFSET:
			{
				SetOffsetNote(fSave);
				break;
			}
			case SL_SE:
			{
				SetJudgeSoundVolume(fSave);
				break;
			}
			}
		}

		// データを取得してから矢印のα値を設定
		float fSave;
		switch (g_nSettingArrowSelect)
		{
		case SL_SPEED:
		{
			fSave = GetHiSpeed();
			break;
		}
		case SL_OFFSET:
		{
			fSave = GetOffsetNote();
			break;
		}
		case SL_SE:
		{
			fSave = GetJudgeSoundVolume();
			break;
		}
		}
		if (fSave <= gc_fSettingList[g_nSettingArrowSelect][SMM_MIN])
		{
			g_SettingArrow[SAP_LEFT].nA = gc_nSettingArrowA[RDP_END];
		}
		else
		{
			g_SettingArrow[SAP_LEFT].nA = gc_nSettingArrowA[RDP_INIT];
		}
		if (fSave >= gc_fSettingList[g_nSettingArrowSelect][SMM_MAX])
		{
			g_SettingArrow[SAP_RIGHT].nA = gc_nSettingArrowA[RDP_END];
		}
		else
		{
			g_SettingArrow[SAP_RIGHT].nA = gc_nSettingArrowA[RDP_INIT];
		}

		if (bTrigger)
		{
			for (int nCountSettingArrow = 0; nCountSettingArrow < SAP_MAX; nCountSettingArrow++)
			{
				g_SettingArrow[nCountSettingArrow].vPos.y = gc_vSettingArrowInitPos.y + g_nSettingArrowSelect * gc_fSettingListRangeY;
			}
		}
	}
}

void DrawSettingArrow()
{
	if (g_bSettingStatus && !g_bSetting)
	{
		SetZBuffer(FALSE);
		SetTexture(TEX_SETTING_ARROW);
		TEXTURE tSettingArrow = GetTextureStatus(TEX_SETTING_ARROW);
		for (int nCountSettingArrow = 0; nCountSettingArrow < SAP_MAX; nCountSettingArrow++)
		{
			SetPolygonColor(D3DCOLOR_ARGB(g_SettingArrow[nCountSettingArrow].nA, 255, 255, 255));
			DrawPolygon
			(
				g_SettingArrow[nCountSettingArrow].vPos.x,
				g_SettingArrow[nCountSettingArrow].vPos.y,
				tSettingArrow.u * g_SettingArrow[nCountSettingArrow].nU,
				0,
				tSettingArrow.u,
				tSettingArrow.v
			);
		}
		SetZBuffer(TRUE);
	}
}

//---------------------------------------------------------------
// 設定項目の説明
//---------------------------------------------------------------
void InitSettingManual()
{
	g_SettingManual.nA = gc_nSettingManualA[SMM_MIN];
	g_SettingManual.nOldA = gc_nSettingManualA[SMM_MIN];
	g_SettingManual.nAddA = 0;
	g_SettingManual.nCount = 0;
	g_SettingManual.nV = 0;
	g_SettingManual.vPos = gc_vSettingManualInitPos;
}

void UninitSettingManual()
{

}

void UpdateSettingManual()
{
	if (g_SettingManual.nOldA != g_SettingManual.nA)
	{
		if (g_SettingManual.nCount < gc_nSettingFadeInOutMaxCount)
		{
			g_SettingManual.nCount++;
			g_SettingManual.nOldA += g_SettingManual.nAddA*1.0f / (gc_nSettingFadeInOutMaxCount * 1.0f);
			if (g_SettingManual.nCount == gc_nSettingFadeInOutMaxCount)
			{
				g_SettingManual.nOldA = g_SettingManual.nA;
			}
		}
	}
	g_SettingManual.nV = g_nSettingArrowSelect;
}

void DrawSettingManual()
{
	SetZBuffer(FALSE);

	// 背景
	SetTexture(TEX_SETTING_MANUAL_BOARD);
	TEXTURE tTexture = GetTextureStatus(TEX_SETTING_MANUAL_BOARD);
	SetPolygonColor(D3DCOLOR_ARGB(g_SettingManual.nOldA, 255, 255, 255));
	DrawPolygon
	(
		g_SettingManual.vPos.x,
		g_SettingManual.vPos.y,
		0,
		0,
		tTexture.u,
		tTexture.v
	);

	// 文字
	SetTexture(TEX_SETTING_MANUAL_LIST);
	tTexture = GetTextureStatus(TEX_SETTING_MANUAL_LIST);
	SetPolygonColor(D3DCOLOR_ARGB(g_SettingManual.nOldA, 255, 255, 255));
	DrawPolygon
	(
		g_SettingManual.vPos.x,
		g_SettingManual.vPos.y + gc_fSettingManualListChangeY,
		0,
		tTexture.v * g_SettingManual.nV,
		tTexture.u,
		tTexture.v
	);

	SetZBuffer(TRUE);
}

void SetSettingManualA(int nType)
{
	g_SettingManual.nA = gc_nSettingManualA[nType];
	g_SettingManual.nAddA = g_SettingManual.nA - g_SettingManual.nOldA;
	g_SettingManual.nCount = 0;
}
					
float GetMusicVolume()
{
	return g_fMusicVolume;
}