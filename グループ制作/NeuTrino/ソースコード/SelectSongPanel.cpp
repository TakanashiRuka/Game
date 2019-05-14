//----------------------------------------------------------------------------
//
//	曲パネル
//
//														 制作者：劉喆
//                                                       作成日：2017/12/27
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "ReadNoteFile.h"
#include "game.h"
#include "Texture.h"
#include "Polygon.h"
#include "Polygon3D.h"
#include "Light.h"
#include "input.h"
#include "JoyStick.h"
#include "sound.h"
#include "ReadSongList.h"
#include "SongStatus.h"
#include "number.h"
#include "music.h"
#include "SelectSongPanel.h"
#include "Setting.h"
#include "TestMode.h"
#include "SelectSongFinalCheck.h"
#include "SelectSongWaitMember.h"

//---------------------------マクロ定義---------------------------------------
#define FILE_COMMON_ADDRESS				"Data/SongList/"
#define ALBUM_COMMON					"Data/SongList/"
#define NOTE_DESIGNER_COMMON			"Note Designer : "

//---------------------------列挙型定義---------------------------------------
typedef enum {
	SSD_LEFT,
	SSD_RIGHT
}SELECT_SONG_DIRECT;

//---------------------------構造体定義---------------------------------------
typedef struct {
	D3DXVECTOR3 vPos;							// 座標
	D3DXVECTOR3 vPosOld;						// 座標(描画用)
	D3DXVECTOR3 vPosMiss;						// 座標の差
	float fScaleX;								// X方向のスケーリング
	int nU;										// Uの値
	int nFadeInCount;							// フェードインカウンター
	int nChangeCount;							// 変換カウンター
	int nTexNum;								// テクスチャ番号
	TEXTURE tAlbumStatus;						// テクスチャデータ
	int nSongNum;								// 曲の番号
	int nA;										// α値
	int nOldA;									// α値
	int nAddA;									// α値
	int nCountChangeA;
}SONG_PANEL;									// 曲パネル

typedef	struct {
	D3DXVECTOR2 vPos;							// 座標
	int nA;										// α値
	int nFontSize;								// フォントサイズ
	int nFadeInCount;							// フェードインカウンター
}FONT_STRUCT;									// フォント用構造体

typedef struct {
	D3DXVECTOR2 vPos;							// 座標
	int nU;										// Uの値
	int nV;										// Vの値
	int nAEffect;								// エフェクトα値
	int nEffectCount;							// エフェクトのカウンター
}SONG_LEVEL_ICON;								// 曲のアイコン

typedef struct {
	D3DXVECTOR3 vPos;							// 座標
	float fScale;								// 大きさ
	int nCount;									// カウンター
	int nA;										// α値
	bool bStatus;								// 描画状態
}ALBUM_FRAME;									// アルバム画像のフチ

//---------------------------グローバル変数-----------------------------------
static const int gc_nMaxPanel = 7;											// パネルの最大数

static const D3DXVECTOR3 gc_vInitPos = { 0.0f,0.0f,50.0f };					// パネルの初期座標
static const D3DXVECTOR3 gc_vRangeX = { 340.0f,0.0f,0.0f };					// パネルのX座標ずらし
static const D3DXVECTOR3 gc_vRangeY = { 0.0f,-1.5f,0.0f };					// パネルのY座標ずらし

static bool g_bPanelChangeA;												// パネルのα値の変化フラグ
static const int gc_nSongPanelA[RDP_MAX] = { 255,0 };						// パネルのα値
static const int gc_nChangeACountMax = 8;									// パネルのα値の変わりスピード
static const float gc_fPanelScaleInitX = 0.0f;								// パネルの初期スケーリング(X)
static const float gc_fPanelScaleEndX = 1.0f;								// パネルの終了スケーリング(X)

static const int gc_nPanelFadeInCountMax = 8;								// パネルのフェードインカウンターの最大値
static const int gc_nPanelChangeCountMax = 6;								// パネルの変換カウンターの最大値
static const int gc_nPanelChangeCountMaxWait = 2;								// パネルの変換カウンターの最大値

static const int gc_nFontInitA = 0;											// フォントのα値(初期値)
static const int gc_nFontEndA = 255;										// フォントのα値(終了値)
static const float gc_fTitleY = 474.0f;
static const float gc_fSubTitleY = 519.0f;
static const float gc_fNoteDesignerY = 546.0f;

static const D3DXVECTOR2 gc_vLevelWordInitPos = { 640.0f,587.0f };			// 難易度(文字)の初期座標

static const int gc_nMaxDigitLevel = 2;										// レベルの最大桁数
static const D3DXVECTOR2 gc_vLevelNumInitPos = { 745.0f,587.0f };			// レベルの初期座標
static const float gc_fLevelNumRageX = 15.0f;								// レベル数字の間隔

static const D3DXVECTOR2 gc_vLevelIconInitPos = { 543.0f,587.0f };			// レベルアイコンの初期座標
static const int gc_nLevelIconEffectCountMax = 24;							// レベルアイコンのカウンターの最大値

static const int gc_nSongPanelTexNumList[gc_nMaxPanel]=						// パネルに対応しているテクスチャ番号
{
	TEX_SELECT_ALBUM_01,
	TEX_SELECT_ALBUM_02,
	TEX_SELECT_ALBUM_03,
	TEX_SELECT_ALBUM_04,
	TEX_SELECT_ALBUM_05,
	TEX_SELECT_ALBUM_06,
	TEX_SELECT_ALBUM_07,
};

static const int gc_nMaxAlbumFrame = 2;										// アルバム画像のフチの最大数
static const D3DXVECTOR3 gc_vAlbumFrameInitPos = { 0.0f,0.0f,50.0f };		// アルバム画像のフチの初期座標
static const int gc_nAlbumFrameCountMax = 20;								// アルバム画像のフチのカウンターの最大値
static const int gc_nAlbumFrameCountStopMax = 60;							// アルバム画像のフチのカウンターの止まっている最大値

static const float gc_fAlbumFrameInitScale = 0.8f;							//
static const float gc_fAlbumFrameStopScale = 1.04f;							//
static const float gc_fAlbumFrameEndScale = 1.1f;							//
static const float gc_nAlbumFrameInitA = 255;
static const float gc_nAlbumFrameStopA = 255;
static const float gc_nAlbumFrameEndA = 0;

static SSSONGSTATUS g_SSSongStatus[MAX_SONG];
static SONG_PANEL g_SongPanel[gc_nMaxPanel];
static FONT_STRUCT g_fsTitle;
static FONT_STRUCT g_fsSubTitle;
static FONT_STRUCT g_fsSongDesigner;
static SONG_LEVEL_WORD g_SongLevelWord;
static SONG_LEVEL_ICON g_SongLevelIcon;
static ALBUM_FRAME g_AlbumFrame[gc_nMaxAlbumFrame];

static bool InitStatus = false;
static bool	g_bFadeInStatus;									// パネルのフェードイン状態
static int g_nPanelChangeCount;									// パネルの変換カウンター

static const int gc_nPanelSetMusicCountMax = 24;
static int g_nPanelSetMusicCount;

static char g_cCommandSave[MAX_WORD];
static char g_cSaveChar;
static int g_nMaxFileNum;
static int g_nSelectNumOld;
static int g_nSelectNum;
static int g_nLevel;
static char g_cMusicAdd[MAX_WORD];

static float g_fBPM;

static const char g_cSStart[] = "START";
static const char g_cSBPM[] = "BPM";
static const char g_cCTitle[] = "TITLE";
static const char g_cCSubtitle[] = "SUBTITLE";
static const char g_cCWaveAdd[] = "WAVEADDRESS";
static const char g_cCSampleTiming[] = "SAMPLEBEGIN";
static const char g_cCAlbum[] = "ALBUM";
static const char g_cCLevel[] = "LEVEL";
static const char g_cCNoteDesigner[] = "NOTEDESIGNER";
static const char gc_cDemo[] = "DEMO";

static bool g_bIsDemo = false;

//---------------------------プロトタイプ宣言---------------------------------
void ResetCommandSave();
void SetPanelStatus(int nDirect);
void ChangeSample(int nNum);
void SetAlbumFrame(bool bStatus);
bool GetAllAlbumFrameStatus();

void InitSelectSongPanel()
{
	g_bFadeInStatus = false;
	g_nPanelChangeCount = 0;
	g_nPanelSetMusicCount = gc_nPanelSetMusicCountMax;
	g_bPanelChangeA = false;
	//if (!InitStatus)
	if (!InitStatus || GetTestModeMemberStatus(TMML_GUEST))
	{
		if (GetGameMode() != MODE_GAME && GetGameMode() != MODE_CONTINUE)
		{
			g_nSelectNum = 0;
			g_nSelectNumOld = g_nSelectNum;
			g_nLevel = 0;
		}
	}
	for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
	{
		g_SongPanel[nCountPanel].nChangeCount = 0;
		g_SongPanel[nCountPanel].nFadeInCount = 0;
		g_SongPanel[nCountPanel].nA = gc_nSongPanelA[RDP_INIT];
		g_SongPanel[nCountPanel].nOldA = g_SongPanel[nCountPanel].nA;
		g_SongPanel[nCountPanel].nAddA = 0;
		g_SongPanel[nCountPanel].nCountChangeA = 0;
		g_SongPanel[nCountPanel].nA = gc_nSongPanelA[RDP_INIT];
		g_SongPanel[nCountPanel].nU = g_nLevel;
		g_SongPanel[nCountPanel].vPos = gc_vInitPos + nCountPanel * gc_vRangeX;
		g_SongPanel[nCountPanel].fScaleX = gc_fPanelScaleInitX;
		g_SongPanel[nCountPanel].nTexNum = gc_nSongPanelTexNumList[nCountPanel];
		g_SongPanel[nCountPanel].tAlbumStatus = GetTextureStatus(g_SongPanel[nCountPanel].nTexNum);
		if (g_SongPanel[nCountPanel].vPos.x > gc_nMaxPanel / 2 * gc_vRangeX.x)
		{
			g_SongPanel[nCountPanel].vPos -= gc_nMaxPanel * gc_vRangeX;
		}
		if (g_SongPanel[nCountPanel].vPos.x == gc_vInitPos.x)
		{
			g_SongPanel[nCountPanel].vPos.y = gc_vInitPos.y;
		}
		else
		{
			g_SongPanel[nCountPanel].vPos += gc_vRangeY;
		}
		if (g_SongPanel[nCountPanel].vPos.x == gc_vInitPos.x)
		{
			g_SongPanel[nCountPanel].nSongNum = g_nSelectNum;
		}
		else
			if (g_SongPanel[nCountPanel].vPos.x > gc_vInitPos.x && g_SongPanel[nCountPanel].vPos.x <= gc_nMaxPanel / 2 * gc_vRangeX.x)
			{
				g_SongPanel[nCountPanel].nSongNum = g_nSelectNum + g_SongPanel[nCountPanel].vPos.x / gc_vRangeX.x;
			}
			else
				if (g_SongPanel[nCountPanel].vPos.x < gc_vInitPos.x && g_SongPanel[nCountPanel].vPos.x >= - gc_nMaxPanel / 2 * gc_vRangeX.x)
				{
					g_SongPanel[nCountPanel].nSongNum = g_nSelectNum + g_SongPanel[nCountPanel].vPos.x / gc_vRangeX.x;
				}
		while (g_SongPanel[nCountPanel].nSongNum < 0)
		{
			g_SongPanel[nCountPanel].nSongNum += g_nMaxFileNum;
		}
		while (g_SongPanel[nCountPanel].nSongNum > g_nMaxFileNum - 1)
		{
			g_SongPanel[nCountPanel].nSongNum -= g_nMaxFileNum;
		}
		g_SongPanel[nCountPanel].vPosOld = g_SongPanel[nCountPanel].vPos;
		g_SongPanel[nCountPanel].vPosMiss = g_SongPanel[nCountPanel].vPos - g_SongPanel[nCountPanel].vPosOld;
		SetTextureAddress(g_SongPanel[nCountPanel].nTexNum, &g_SSSongStatus[g_SongPanel[nCountPanel].nSongNum].c_AlbumFileAddress[0]);
	}

	SetMusicM(g_SSSongStatus[g_nSelectNum].c_SongFileAddress, 0);
	InitMusic(GethWnd());

	if (!InitStatus)
	{
		InitStatus = true;
	}
}

void InitAllSongBuffer()
{
	g_nMaxFileNum = GetMaxFileNum();
	for (int nConutMNum = 0; nConutMNum < g_nMaxFileNum; nConutMNum++)
	{
		SetMusicM(&g_SSSongStatus[nConutMNum].c_SongFileAddress[0], 0);

		InitMusic(GethWnd());
		UninitMusic();
	}
}

void UninitSelectSongPanel()
{
	SetTextureAddress(TEX_SELECT_ALBUM_00, &g_SSSongStatus[g_nSelectNum].c_AlbumFileAddress[0]);
	SetSongTitleGame(&g_SSSongStatus[g_nSelectNum].c_Title[0]);
	SetNoteFileURL(&g_SSSongStatus[g_nSelectNum].c_NoteFileAddress[0]);
	SetSongURL(&g_SSSongStatus[g_nSelectNum].c_SongFileAddress[0]);
	
	if (GetGameMode() == MODE_SELECT_SONG)
	{
		UninitMusic();
	}
}

void UpdateSelectSongPanel()
{
	for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
	{
		if (g_SongPanel[nCountPanel].nFadeInCount < gc_nPanelFadeInCountMax)
		{
			g_SongPanel[nCountPanel].nFadeInCount++;
			g_SongPanel[nCountPanel].fScaleX += (gc_fPanelScaleEndX - gc_fPanelScaleInitX) / (gc_nPanelFadeInCountMax * 1.0f);
			if (g_SongPanel[nCountPanel].nFadeInCount == gc_nPanelFadeInCountMax)
			{
				g_SongPanel[nCountPanel].fScaleX = gc_fPanelScaleEndX;
				g_bFadeInStatus = true;
				PlayMusic(MUSIC, g_SSSongStatus[g_nSelectNum].n_SampleBeginTime);
				SetMusicVolume((MUSIC_LABEL)MUSIC, GetMusicVolume());
			}
		}
	}
	if (g_bFadeInStatus)
	{
		// ここからはレベル調整
		if ((GetKeyboardTrigger(DIK_UP) || GetPOVTrigger(JOY_UP)) && !GetSettingStatus() && !GetFadeFlag() && !GetRoomReady() && !GetErrorListStatus())
		{
			if (g_nLevel < MAX_LEVEL - 1)
			{
				g_nLevel++;
				if (GetPlayStatus(SE_SELECT_LEVEL))
				{
					StopSound(SE_SELECT_LEVEL);
				}
				PlaySound(SE_SELECT_LEVEL);
			}
		}
		if ((GetKeyboardTrigger(DIK_DOWN) || GetPOVTrigger(JOY_DOWN)) && !GetSettingStatus() && !GetFadeFlag() && !GetRoomReady() && !GetErrorListStatus())
		{
			if (g_nLevel > 0)
			{
				g_nLevel--;
				if (GetPlayStatus(SE_SELECT_LEVEL))
				{
					StopSound(SE_SELECT_LEVEL);
				}
				PlaySound(SE_SELECT_LEVEL);
			}
		}
		for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
		{
			g_SongPanel[nCountPanel].nU = g_nLevel;
		}
		// レベル調整終了

		// ここからは曲選択
		if (g_nPanelChangeCount < gc_nPanelChangeCountMax + gc_nPanelChangeCountMaxWait)
		{
			g_nPanelChangeCount++;
			if (g_nPanelChangeCount < gc_nPanelChangeCountMax)
			{
				for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
				{
					g_SongPanel[nCountPanel].nChangeCount++;
				}
			}
		}
		if (g_nPanelChangeCount <= gc_nPanelChangeCountMax)
		{
			for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
			{
				if (g_SongPanel[nCountPanel].vPosOld != g_SongPanel[nCountPanel].vPos)
				{
					g_SongPanel[nCountPanel].vPosOld += g_SongPanel[nCountPanel].vPosMiss / (gc_nPanelChangeCountMax * 1.0f);
					if (g_SongPanel[nCountPanel].nChangeCount == gc_nPanelChangeCountMax)
					{
						g_SongPanel[nCountPanel].vPosOld = g_SongPanel[nCountPanel].vPos;
					}
				}
			}
		}
		if (g_nPanelChangeCount == gc_nPanelChangeCountMax + gc_nPanelChangeCountMaxWait)
		{
			if ((GetKeyboardRepeat(DIK_LEFT) || GetPOVRepeat(JOY_LEFT)) && !GetSettingStatus() && !GetSelectSongFinalCheckStatus() && !GetErrorListStatus())
			{
				g_nPanelSetMusicCount = 0;
				SetAlbumFrame(false);
				g_nSelectNum--;
				if (g_nSelectNum < 0)
				{
					g_nSelectNum += g_nMaxFileNum;
				}
				g_nPanelChangeCount = 0;
				SetPanelStatus(SSD_LEFT);
				for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
				{
					g_SongPanel[nCountPanel].nChangeCount = 0;
				}
				if (GetPlayStatus(SE_SELECT_SONG))
				{
					StopSound(SE_SELECT_SONG);
				}
				PlaySound(SE_SELECT_SONG);
			}
			else
			if ((GetKeyboardRepeat(DIK_RIGHT) || GetPOVRepeat(JOY_RIGHT)) && !GetSettingStatus() && !GetSelectSongFinalCheckStatus() && !GetErrorListStatus())
			{
				g_nPanelSetMusicCount = 0;
				SetAlbumFrame(false);
				g_nSelectNum++;
				if (g_nSelectNum == g_nMaxFileNum)
				{
					g_nSelectNum -= g_nMaxFileNum;
				}
				g_nPanelChangeCount = 0;
				SetPanelStatus(SSD_RIGHT);
				for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
				{
					g_SongPanel[nCountPanel].nChangeCount = 0;
				}
				if (GetPlayStatus(SE_SELECT_SONG))
				{
					StopSound(SE_SELECT_SONG);
				}
				PlaySound(SE_SELECT_SONG);
			}
		}
		// 曲選択終了
		if (!GetPlayStatusM(MUSIC))
		{
			PlayMusic(MUSIC, g_SSSongStatus[g_nSelectNumOld].n_SampleBeginTime);
			SetMusicVolume((MUSIC_LABEL)MUSIC, GetMusicVolume());
		}
		if (!GetAllAlbumFrameStatus())
		{
			SetAlbumFrame(true);
		}

		if (g_nPanelSetMusicCount < gc_nPanelSetMusicCountMax)
		{
			g_nPanelSetMusicCount++;
			if (g_nPanelSetMusicCount == gc_nPanelSetMusicCountMax)
			{
				if (g_nSelectNumOld != g_nSelectNum)
				{
					g_nSelectNumOld = g_nSelectNum;
					if (GetPlayStatusM(MUSIC))
					{
						StopMusic(MUSIC);
					}
					UninitMusic();
					ChangeSample(g_nSelectNum);
					PlayMusic(MUSIC, g_SSSongStatus[g_nSelectNum].n_SampleBeginTime);
					SetMusicVolume((MUSIC_LABEL)MUSIC, GetMusicVolume());
				}
			}
		}

		// α値変化
		for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
		{
			if (g_SongPanel[nCountPanel].nOldA != g_SongPanel[nCountPanel].nA)
			{
				if (g_SongPanel[nCountPanel].nCountChangeA < gc_nChangeACountMax)
				{
					g_SongPanel[nCountPanel].nCountChangeA++;
					g_SongPanel[nCountPanel].nOldA += g_SongPanel[nCountPanel].nAddA * 1.0f / (gc_nChangeACountMax * 1.0f);
					if (g_SongPanel[nCountPanel].nCountChangeA == gc_nChangeACountMax)
					{
						g_SongPanel[nCountPanel].nOldA = g_SongPanel[nCountPanel].nA;
						g_bPanelChangeA = false;
					}
				}
			}
		}
	}
}

void DrawSelectSongPanel()
{
	SetLight(FALSE);
	SetTexture(TEX_SELECT_SONG_PANEL);
	TEXTURE TexturePanel = GetTextureStatus(TEX_SELECT_SONG_PANEL);
	for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
	{
		SetPolygonColor3D(D3DCOLOR_ARGB(g_SongPanel[nCountPanel].nOldA, 255, 255, 255));
		SetScale3D(g_SongPanel[nCountPanel].fScaleX, 1.0f, 1.0f);
		DrawPolygon3D
		(
			g_SongPanel[nCountPanel].vPosOld.x,
			g_SongPanel[nCountPanel].vPosOld.y,
			g_SongPanel[nCountPanel].vPosOld.z,
			g_SongPanel[nCountPanel].nU * TexturePanel.u,
			0,
			TexturePanel.u,
			TexturePanel.v
		);
	}
}

void DrawSongAlbum()
{
	for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
	{
		SetTexture(g_SongPanel[nCountPanel].nTexNum);
		SetPolygonColor3D(D3DCOLOR_ARGB(g_SongPanel[nCountPanel].nOldA, 255, 255, 255));
		SetScale3D(g_SongPanel[nCountPanel].fScaleX, 1.0f, 1.0f);
		DrawPolygon3D
		(
			g_SongPanel[nCountPanel].vPosOld.x,
			g_SongPanel[nCountPanel].vPosOld.y,
			g_SongPanel[nCountPanel].vPosOld.z,
			0,
			0,
			g_SongPanel[nCountPanel].tAlbumStatus.u,
			g_SongPanel[nCountPanel].tAlbumStatus.v
		);
	}
}

void DrawSongStatus()
{
	SetDebugTextColor(D3DCOLOR_ARGB(255, 0, 255, 0));
	DebugText(0, 22 * 2, "NoteFileAddress:%s", &g_SSSongStatus[g_nSelectNum].c_NoteFileAddress[0]);
	DebugText(0, 22 * 3, "Title:%s", &g_SSSongStatus[g_nSelectNum].c_Title[0]);
	DebugText(0, 22 * 4, "Subtitle:%s", &g_SSSongStatus[g_nSelectNum].c_Subtitle[0]);
	DebugText(0, 22 * 5, "SongNum:%d", g_nSelectNum);
	DebugText(0, 22 * 6, "SongLv:%d", g_SSSongStatus[g_nSelectNum].nLevel[g_nLevel]);
	DebugText(0, 22 * 7, "AlbumAddress:%s", &g_SSSongStatus[g_nSelectNum].c_AlbumFileAddress[0]);
	DebugText(0, 22 * 8, "AlbumNum: %d, %d, %d, %d, %d, %d, %d", g_SongPanel[0].nSongNum, g_SongPanel[1].nSongNum, g_SongPanel[2].nSongNum, g_SongPanel[3].nSongNum, g_SongPanel[4].nSongNum, g_SongPanel[5].nSongNum, g_SongPanel[6].nSongNum);
	DebugText(0, 22 * 9, "NoteDesigner: %s", &g_SSSongStatus[g_nSelectNum].c_NoteDesigner[0]);
}

void SetSongStatus(int nSongNum, char *cNoteAddress)
{
	g_SSSongStatus[nSongNum].c_NoteFileAddress[0] = '\0';
	strcat(g_SSSongStatus[nSongNum].c_NoteFileAddress, FILE_COMMON_ADDRESS);
	strcat(g_SSSongStatus[nSongNum].c_NoteFileAddress, cNoteAddress);

	FILE *fNoteFile;

	fNoteFile = fopen(g_SSSongStatus[nSongNum].c_NoteFileAddress, "r");

	if (!fNoteFile)
	{
		MessageBox(NULL, "譜面ファイルがねぇ！", "エラー", MB_OK);
	}
	else
	{
		while (1)
		{
			g_cSaveChar = fgetc(fNoteFile);
			if (g_cSaveChar == 35)	 // # = 35
			{
				ResetCommandSave();
				fscanf(fNoteFile, "%s", g_cCommandSave);
				if (strcmp(g_cCommandSave, g_cSBPM) == 0)
				{
					fscanf(fNoteFile, "%f", &g_fBPM);
					g_SSSongStatus[nSongNum].fBPM = g_fBPM;
				}
				if (strcmp(g_cCommandSave, g_cCSampleTiming) == 0)
				{
					fscanf(fNoteFile, "%d", &g_SSSongStatus[nSongNum].n_SampleBeginTime);
				}
				if (strcmp(g_cCommandSave, g_cCLevel) == 0)
				{
					for (int count = 0; count < MAX_LEVEL; count++)
					{
						fscanf(fNoteFile, "%d", &g_SSSongStatus[nSongNum].nLevel[count]);
					}
				}
				if (strcmp(g_cCommandSave, g_cCTitle) == 0)
				{
					g_SSSongStatus[nSongNum].c_Title[0] = '\0';

					g_cSaveChar = fgetc(fNoteFile);
					fgets(g_SSSongStatus[nSongNum].c_Title, MAX_WORD, fNoteFile);

					int len = (int)strlen(g_SSSongStatus[nSongNum].c_Title);

					while (g_SSSongStatus[nSongNum].c_Title[len - 1] == '\n' || g_SSSongStatus[nSongNum].c_Title[len - 1] == '\r')
					{
						g_SSSongStatus[nSongNum].c_Title[len - 1] = '\0';
						len--;
					}
				}
				if (strcmp(g_cCommandSave, g_cCSubtitle) == 0)
				{
					g_SSSongStatus[nSongNum].c_Subtitle[0] = '\0';

					g_cSaveChar = fgetc(fNoteFile);
					fgets(g_SSSongStatus[nSongNum].c_Subtitle, MAX_WORD, fNoteFile);

					int len = (int)strlen(g_SSSongStatus[nSongNum].c_Subtitle);

					while (g_SSSongStatus[nSongNum].c_Subtitle[len - 1] == '\n' || g_SSSongStatus[nSongNum].c_Subtitle[len - 1] == '\r')
					{
						g_SSSongStatus[nSongNum].c_Subtitle[len - 1] = '\0';
						len--;
					}
				}
				if (strcmp(g_cCommandSave, g_cCNoteDesigner) == 0)
				{
					g_SSSongStatus[nSongNum].c_NoteDesigner[0] = '\0';


					char cSaveNoteDesigner[MAX_WORD] = { "\0" };
					g_cSaveChar = fgetc(fNoteFile);
					fgets(cSaveNoteDesigner, MAX_WORD, fNoteFile);

					strcat(g_SSSongStatus[nSongNum].c_NoteDesigner, NOTE_DESIGNER_COMMON);
					strcat(g_SSSongStatus[nSongNum].c_NoteDesigner, cSaveNoteDesigner);

					int len = (int)strlen(g_SSSongStatus[nSongNum].c_NoteDesigner);

					while (g_SSSongStatus[nSongNum].c_NoteDesigner[len - 1] == '\n' || g_SSSongStatus[nSongNum].c_NoteDesigner[len - 1] == '\r')
					{
						g_SSSongStatus[nSongNum].c_NoteDesigner[len - 1] = '\0';
						len--;
					}
				}
				if (strcmp(g_cCommandSave, g_cCWaveAdd) == 0)
				{
					g_cMusicAdd[0] = '\0';

					g_cSaveChar = fgetc(fNoteFile);
					fgets(g_cMusicAdd, MAX_WORD, fNoteFile);

					int len = (int)strlen(g_cMusicAdd);

					while (g_cMusicAdd[len - 1] == '\n' || g_cMusicAdd[len - 1] == '\r')
					{
						g_cMusicAdd[len - 1] = '\0';
						len--;
					}

					g_SSSongStatus[nSongNum].c_SongFileAddress[0] = '\0';

					strcat(g_SSSongStatus[nSongNum].c_SongFileAddress, FILE_COMMON_ADDRESS);
					strcat(g_SSSongStatus[nSongNum].c_SongFileAddress, g_cMusicAdd);

				}
				if (strcmp(g_cCommandSave, g_cCAlbum) == 0)
				{
					g_cMusicAdd[0] = '\0';

					g_cSaveChar = fgetc(fNoteFile);
					fgets(g_cMusicAdd, MAX_WORD, fNoteFile);

					int len = (int)strlen(g_cMusicAdd);

					while (g_cMusicAdd[len - 1] == '\n' || g_cMusicAdd[len - 1] == '\r')
					{
						g_cMusicAdd[len - 1] = '\0';
						len--;
					}

					g_SSSongStatus[nSongNum].c_AlbumFileAddress[0] = '\0';

					strcat(g_SSSongStatus[nSongNum].c_AlbumFileAddress, ALBUM_COMMON);
					strcat(g_SSSongStatus[nSongNum].c_AlbumFileAddress, g_cMusicAdd);

				}
				if (strcmp(g_cCommandSave, gc_cDemo) == 0)
				{
					fscanf(fNoteFile, "%d", &g_SSSongStatus[nSongNum].bDemo);
					if (g_SSSongStatus[nSongNum].bDemo)
					{
						g_bIsDemo = true;
					}
				}
				if (strcmp(g_cCommandSave, g_cSStart) == 0)
				{
					break;
				}
			}
		}
		fclose(fNoteFile);
	}
}

void ResetCommandSave()
{
	g_cCommandSave[0] = '\0';
}

int GetLevel()
{
	return g_nLevel;
}
				   
void SetLevel(int nLevel)
{
	g_nLevel = nLevel;
}

int GetSelectNum()
{
	return g_nSelectNum;
}

void SetSelectNum(int nSelectNum)
{
	g_nSelectNum = nSelectNum;
}

int GetLevelNum()
{
	return g_SSSongStatus[g_nSelectNum].nLevel[g_nLevel];
}

void SetPanelStatus(int nDirect)
{
	switch (nDirect)
	{
	case SSD_LEFT:
	{
		for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
		{
			g_SongPanel[nCountPanel].vPos += gc_vRangeX;
			if (g_SongPanel[nCountPanel].vPos.x > gc_vInitPos.x + gc_nMaxPanel / 2 * gc_vRangeX.x)
			{
				g_SongPanel[nCountPanel].vPos.x -= gc_nMaxPanel * gc_vRangeX.x;
				g_SongPanel[nCountPanel].nSongNum = g_nSelectNum - gc_nMaxPanel / 2;
				while (g_SongPanel[nCountPanel].nSongNum < 0)
				{
					g_SongPanel[nCountPanel].nSongNum += g_nMaxFileNum;
				}
				while (g_SongPanel[nCountPanel].nSongNum > g_nMaxFileNum - 1)
				{
					g_SongPanel[nCountPanel].nSongNum -= g_nMaxFileNum;
				}
				SetTextureAddress(g_SongPanel[nCountPanel].nTexNum, &g_SSSongStatus[g_SongPanel[nCountPanel].nSongNum].c_AlbumFileAddress[0]);
			}
			g_SongPanel[nCountPanel].vPosOld = g_SongPanel[nCountPanel].vPos - gc_vRangeX;
			if (g_SongPanel[nCountPanel].vPos.x == gc_vInitPos.x)
			{
				g_SongPanel[nCountPanel].vPos.y = gc_vInitPos.y;
			}
			else
			{
				g_SongPanel[nCountPanel].vPos.y = gc_vRangeY.y;
			}
			g_SongPanel[nCountPanel].vPosMiss = g_SongPanel[nCountPanel].vPos - g_SongPanel[nCountPanel].vPosOld;
		}
		break;
	}
	case SSD_RIGHT:
	{
		for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
		{
			g_SongPanel[nCountPanel].vPos -= gc_vRangeX;
			if (g_SongPanel[nCountPanel].vPos.x < gc_vInitPos.x - gc_nMaxPanel / 2 * gc_vRangeX.x)
			{
				g_SongPanel[nCountPanel].vPos.x += gc_nMaxPanel * gc_vRangeX.x;
				g_SongPanel[nCountPanel].nSongNum = g_nSelectNum + gc_nMaxPanel / 2;
				while (g_SongPanel[nCountPanel].nSongNum < 0)
				{
					g_SongPanel[nCountPanel].nSongNum += g_nMaxFileNum;
				}
				while (g_SongPanel[nCountPanel].nSongNum > g_nMaxFileNum - 1)
				{
					g_SongPanel[nCountPanel].nSongNum -= g_nMaxFileNum;
				}
				SetTextureAddress(g_SongPanel[nCountPanel].nTexNum, &g_SSSongStatus[g_SongPanel[nCountPanel].nSongNum].c_AlbumFileAddress[0]);
			}
			g_SongPanel[nCountPanel].vPosOld = g_SongPanel[nCountPanel].vPos + gc_vRangeX;
			if (g_SongPanel[nCountPanel].vPos.x == gc_vInitPos.x)
			{
				g_SongPanel[nCountPanel].vPos.y = gc_vInitPos.y;
			}
			else
			{
				g_SongPanel[nCountPanel].vPos.y = gc_vRangeY.y;
			}
			g_SongPanel[nCountPanel].vPosMiss = g_SongPanel[nCountPanel].vPos - g_SongPanel[nCountPanel].vPosOld;
		}
		break;
	}
	}
}

void ChangeSample(int nNum)
{
	SetMusicM(&g_SSSongStatus[nNum].c_SongFileAddress[0], 0);
	HWND *hWnd;
	hWnd = new HWND;

	*hWnd = GethWnd();

	InitMusic(*hWnd);
	PlayMusic(MUSIC, g_SSSongStatus[nNum].n_SampleBeginTime);
	StopMusic(MUSIC);

	delete[] hWnd;
}

//---------------------------------------------------------------
// 曲タイトル
//---------------------------------------------------------------
void InitSongTitle()
{
	g_fsTitle.nA = gc_nFontInitA;
	g_fsTitle.nFontSize = 32;
	g_fsTitle.nFadeInCount = 0;
	g_fsTitle.vPos = { 0.0f,gc_fTitleY };
	InitText(F_SELECT_SONG_TITLE, g_fsTitle.nFontSize, "MS コジック");
}

void UninitSongTitle()
{
	UninitText(F_SELECT_SONG_TITLE);
}

void UpdateSongTitle()
{
	if (g_fsTitle.nFadeInCount < gc_nPanelFadeInCountMax)
	{
		g_fsTitle.nFadeInCount++;
		g_fsTitle.nA += (gc_nFontEndA - gc_nFontInitA) * 1.0f / (gc_nPanelFadeInCountMax * 1.0f);
		if (g_fsTitle.nFadeInCount == gc_nPanelFadeInCountMax)
		{
			g_fsTitle.nA = gc_nFontEndA;
		}
	}
}

void DrawSongTitle()
{
	SetTextStatus(D3DCOLOR_ARGB(g_fsTitle.nA, 255, 255, 255), D3DCOLOR_ARGB(g_fsTitle.nA, 0, 0, 0), 0);
	SetTextStyle(DT_CENTER);
	OutputText(F_SELECT_SONG_TITLE, g_fsTitle.vPos.x, g_fsTitle.vPos.y, "%s", &g_SSSongStatus[g_nSelectNum].c_Title[0]);
}

//---------------------------------------------------------------
// 曲サブタイトル
//---------------------------------------------------------------
void InitSongSubTitle()
{
	g_fsSubTitle.nA = gc_nFontInitA;
	g_fsSubTitle.nFontSize = 15;
	g_fsSubTitle.nFadeInCount = 0;
	g_fsSubTitle.vPos = { 0.0f,gc_fSubTitleY };
	InitText(F_SELECT_SONG_SUBTITLE, g_fsSubTitle.nFontSize, "MS コジック");
}

void UninitSongSubTitle()
{
	UninitText(F_SELECT_SONG_SUBTITLE);
}

void UpdateSongSubTitle()
{
	if (g_fsSubTitle.nFadeInCount < gc_nPanelFadeInCountMax)
	{
		g_fsSubTitle.nFadeInCount++;
		g_fsSubTitle.nA += (gc_nFontEndA - gc_nFontInitA) * 1.0f / (gc_nPanelFadeInCountMax * 1.0f);
		if (g_fsSubTitle.nFadeInCount == gc_nPanelFadeInCountMax)
		{
			g_fsSubTitle.nA = gc_nFontEndA;
		}
	}
}

void DrawSongSubTitle()
{
	SetTextStatus(D3DCOLOR_ARGB(g_fsSubTitle.nA, 255, 255, 255), D3DCOLOR_ARGB(g_fsSubTitle.nA, 255, 255, 255), 0);
	SetTextStyle(DT_CENTER);
	OutputText(F_SELECT_SONG_SUBTITLE, g_fsSubTitle.vPos.x, g_fsSubTitle.vPos.y, "%s", &g_SSSongStatus[g_nSelectNum].c_Subtitle[0]);
}

//---------------------------------------------------------------
// 譜面作成者
//---------------------------------------------------------------
void InitNoteDesigner()
{
	g_fsSongDesigner.nA = gc_nFontInitA;
	g_fsSongDesigner.nFontSize = 15;
	g_fsSongDesigner.nFadeInCount = 0;
	g_fsSongDesigner.vPos = { 0.0f,gc_fNoteDesignerY };
	InitText(F_SELECT_NOTE_DESIGNER, g_fsSongDesigner.nFontSize, "MS コジック");
}

void UninitNoteDesigner()
{
	UninitText(F_SELECT_NOTE_DESIGNER);
}

void UpdateNoteDesigner()
{
	if (g_fsSongDesigner.nFadeInCount < gc_nPanelFadeInCountMax)
	{
		g_fsSongDesigner.nFadeInCount++;
		g_fsSongDesigner.nA += (gc_nFontEndA - gc_nFontInitA) * 1.0f / (gc_nPanelFadeInCountMax * 1.0f);
		if (g_fsSongDesigner.nFadeInCount == gc_nPanelFadeInCountMax)
		{
			g_fsSongDesigner.nA = gc_nFontEndA;
		}
	}
}

void DrawNoteDesigner()
{
	SetTextStatus(D3DCOLOR_ARGB(g_fsSongDesigner.nA, 255, 255, 255), D3DCOLOR_ARGB(g_fsSongDesigner.nA, 255, 255, 255), 0);
	SetTextStyle(DT_CENTER);
	OutputText(F_SELECT_NOTE_DESIGNER, g_fsSongDesigner.vPos.x, g_fsSongDesigner.vPos.y, "%s", &g_SSSongStatus[g_nSelectNum].c_NoteDesigner[0]);
}

//---------------------------------------------------------------
// 難易度(文字)
//---------------------------------------------------------------
void InitLevelWord()
{
	g_SongLevelWord.nA = gc_nFontInitA;
	g_SongLevelWord.nFadeInCount = 0;
	g_SongLevelWord.nV = g_nLevel;
	g_SongLevelWord.vPos = gc_vLevelWordInitPos;
}

void UninitLevelWord()
{

}

void UpdateLevelWord()
{
	g_SongLevelWord.nV = g_nLevel;
	if (g_SongLevelWord.nFadeInCount < gc_nPanelFadeInCountMax)
	{
		g_SongLevelWord.nFadeInCount++;
		g_SongLevelWord.nA += (gc_nFontEndA - gc_nFontInitA) * 1.0f / (gc_nPanelFadeInCountMax * 1.0f);
		if (g_SongLevelWord.nFadeInCount == gc_nPanelFadeInCountMax)
		{
			g_SongLevelWord.nA = gc_nFontEndA;
		}
	}
}

void DrawLevelWord()
{
	SetTexture(TEX_SELECT_LEVEL_WORD);
	TEXTURE tLevelWord = GetTextureStatus(TEX_SELECT_LEVEL_WORD);
	SetPolygonColor(D3DCOLOR_ARGB(g_SongLevelWord.nA, 255, 255, 255));
	DrawPolygon
	(
		g_SongLevelWord.vPos.x,
		g_SongLevelWord.vPos.y,
		0,
		tLevelWord.v * g_SongLevelWord.nV,
		tLevelWord.u,
		tLevelWord.v
	);
}

//---------------------------------------------------------------
// 難易度(数字)
//---------------------------------------------------------------
void DrawLevelNum()
{
	TEXTURE tLevelNum = GetTextureStatus(TEX_SELECT_LEVEL_NUM);
	int n_LvBackUp = g_SSSongStatus[g_nSelectNum].nLevel[g_nLevel];
	int nDigit = 0;

	do
	{
		n_LvBackUp /= 10;
		nDigit++;
	} while (n_LvBackUp != 0);

	n_LvBackUp = g_SSSongStatus[g_nSelectNum].nLevel[GetLevel()];
	int count;
	for (count = 0; count < nDigit; count++)
	{
		int n = n_LvBackUp % 10;
		DrawNumber(TEX_SELECT_LEVEL_NUM, tLevelNum.u, tLevelNum.v, n, gc_vLevelNumInitPos.x - (count) * gc_fLevelNumRageX, gc_vLevelNumInitPos.y, g_SongLevelWord.nA);
		n_LvBackUp /= 10;
	}
	for (count; count < gc_nMaxDigitLevel; count++)
	{
		DrawNumber(TEX_SELECT_LEVEL_NUM, tLevelNum.u, tLevelNum.v, 0, gc_vLevelNumInitPos.x - (count) * gc_fLevelNumRageX, gc_vLevelNumInitPos.y, g_SongLevelWord.nA);
	}
}

//---------------------------------------------------------------
// 曲のアイコン
//---------------------------------------------------------------
void InitLevelIcon()
{
	g_SongLevelIcon.nU = 0;
	g_SongLevelIcon.nV = g_nLevel;
	g_SongLevelIcon.vPos = gc_vLevelIconInitPos;
	g_SongLevelIcon.nAEffect = gc_nFontInitA;
	g_SongLevelIcon.nEffectCount = 0;
}

void UninitLevelIcon()
{

}

void UpdateLevelIcon()
{
	g_SongLevelIcon.nV = g_nLevel;
	g_SongLevelIcon.nEffectCount++;
	if (g_SongLevelIcon.nEffectCount < gc_nLevelIconEffectCountMax)
	{
		g_SongLevelIcon.nAEffect += (gc_nFontEndA - gc_nFontInitA) * 1.0f / (gc_nLevelIconEffectCountMax * 1.0f);
	}
	else
	if (g_SongLevelIcon.nEffectCount == gc_nLevelIconEffectCountMax)
	{
		g_SongLevelIcon.nAEffect = gc_nFontEndA;
	}
	else
	if (g_SongLevelIcon.nEffectCount < gc_nLevelIconEffectCountMax * 2)
	{
		g_SongLevelIcon.nAEffect += (-gc_nFontEndA + gc_nFontInitA) * 1.0f / (gc_nLevelIconEffectCountMax * 1.0f);
	}
	else
	if (g_SongLevelIcon.nEffectCount == gc_nLevelIconEffectCountMax * 2)
	{
		g_SongLevelIcon.nAEffect = gc_nFontInitA;
		g_SongLevelIcon.nEffectCount = 0;
	}
	if (g_SongLevelIcon.nAEffect < gc_nFontInitA)
	{
		g_SongLevelIcon.nAEffect = gc_nFontInitA;
	}
	if (g_SongLevelIcon.nAEffect > gc_nFontEndA)
	{
		g_SongLevelIcon.nAEffect = gc_nFontEndA;
	}
}

void DrawLevelIcon()
{
	SetTexture(TEX_SELECT_LEVEL_ICON);
	TEXTURE tLevelIconWord = GetTextureStatus(TEX_SELECT_LEVEL_ICON);
	SetPolygonColor(D3DCOLOR_ARGB(g_SongLevelWord.nA, 255, 255, 255));
	DrawPolygon
	(
		g_SongLevelIcon.vPos.x,
		g_SongLevelIcon.vPos.y,
		0,
		tLevelIconWord.v * g_SongLevelIcon.nV,
		tLevelIconWord.u,
		tLevelIconWord.v
	);
	SetBlend(D3DBLEND_BLENDFACTOR);
	SetPolygonColor(D3DCOLOR_ARGB(g_SongLevelIcon.nAEffect, 255, 255, 255));
	DrawPolygon
	(
		g_SongLevelIcon.vPos.x,
		g_SongLevelIcon.vPos.y,
		tLevelIconWord.u,
		tLevelIconWord.v * g_SongLevelIcon.nV,
		tLevelIconWord.u,
		tLevelIconWord.v
	);
	SetBlend(D3DBLEND_INVSRCALPHA);
}

//---------------------------------------------------------------
// アルバム画像のフチ
//---------------------------------------------------------------
void InitAlbumFrame()
{
	for (int nCountAlbumFrame = 0; nCountAlbumFrame < gc_nMaxAlbumFrame; nCountAlbumFrame++)
	{
		g_AlbumFrame[nCountAlbumFrame].bStatus = false;
		g_AlbumFrame[nCountAlbumFrame].nA = gc_nAlbumFrameInitA;
		g_AlbumFrame[nCountAlbumFrame].nCount = 0;
		g_AlbumFrame[nCountAlbumFrame].vPos = gc_vAlbumFrameInitPos;
		g_AlbumFrame[nCountAlbumFrame].fScale = gc_fAlbumFrameInitScale;
	}
	SetAlbumFrame(true);
}

void UninitAlbumFrame()
{

}

void UpdateAlbumFrame()
{
	for (int nCountAlbumFrame = 0; nCountAlbumFrame < gc_nMaxAlbumFrame; nCountAlbumFrame++)
	{
		if (g_AlbumFrame[nCountAlbumFrame].bStatus)
		{
			g_AlbumFrame[nCountAlbumFrame].nCount++;
			if (g_AlbumFrame[nCountAlbumFrame].nCount < gc_nAlbumFrameCountMax)
			{
				g_AlbumFrame[nCountAlbumFrame].fScale += (gc_fAlbumFrameStopScale - gc_fAlbumFrameInitScale) / (gc_nAlbumFrameCountMax * 1.0f);
			}
			if (g_AlbumFrame[nCountAlbumFrame].nCount == gc_nAlbumFrameCountMax)
			{
				g_AlbumFrame[nCountAlbumFrame].fScale = gc_fAlbumFrameStopScale;
			}
			if (g_AlbumFrame[nCountAlbumFrame].nCount == gc_nAlbumFrameCountMax + gc_nAlbumFrameCountStopMax)
			{
				SetAlbumFrame(true);
			}
			if (g_AlbumFrame[nCountAlbumFrame].nCount > gc_nAlbumFrameCountMax + gc_nAlbumFrameCountStopMax &&
				g_AlbumFrame[nCountAlbumFrame].nCount < gc_nAlbumFrameCountMax * 2 + gc_nAlbumFrameCountStopMax)
			{
				g_AlbumFrame[nCountAlbumFrame].fScale += (gc_fAlbumFrameEndScale - gc_fAlbumFrameStopScale) / (gc_nAlbumFrameCountMax * 1.0f);
				g_AlbumFrame[nCountAlbumFrame].nA += (gc_nAlbumFrameEndA - gc_nAlbumFrameStopA) * 1.0f / (gc_nAlbumFrameCountMax * 1.0f);
			}
			if (g_AlbumFrame[nCountAlbumFrame].nCount == gc_nAlbumFrameCountMax * 2 + gc_nAlbumFrameCountStopMax)
			{
				g_AlbumFrame[nCountAlbumFrame].bStatus = false;
			}
		}
	}
}

void DrawAlbumFrame()
{
	SetLight(FALSE);
	SetZBuffer(FALSE);
	SetBlend(D3DBLEND_BLENDFACTOR);
	SetTexture(TEX_SELECT_ALBUM_FRAME);
	TEXTURE tAlbumFrame = GetTextureStatus(TEX_SELECT_ALBUM_FRAME);
	for (int nCountAlbumFrame = 0; nCountAlbumFrame < gc_nMaxAlbumFrame; nCountAlbumFrame++)
	{
		if (g_AlbumFrame[nCountAlbumFrame].bStatus)
		{
			SetPolygonColor3D(D3DCOLOR_ARGB(g_AlbumFrame[nCountAlbumFrame].nA, 255, 255, 255));
			SetScale3D(g_AlbumFrame[nCountAlbumFrame].fScale, 1.0f, g_AlbumFrame[nCountAlbumFrame].fScale);
			DrawPolygon3D
			(
				g_AlbumFrame[nCountAlbumFrame].vPos.x,
				g_AlbumFrame[nCountAlbumFrame].vPos.y,
				g_AlbumFrame[nCountAlbumFrame].vPos.z,
				0,
				0,
				tAlbumFrame.u,
				tAlbumFrame.v
			);
		}
	}
	SetBlend(D3DBLEND_INVSRCALPHA);
	SetZBuffer(TRUE);
}

void SetAlbumFrame(bool bStatus)
{
	if (bStatus)
	{
		for (int nCountAlbumFrame = 0; nCountAlbumFrame < gc_nMaxAlbumFrame; nCountAlbumFrame++)
		{		
			if (!g_AlbumFrame[nCountAlbumFrame].bStatus)
			{
				g_AlbumFrame[nCountAlbumFrame].bStatus = true;
				g_AlbumFrame[nCountAlbumFrame].nA = gc_nAlbumFrameInitA;
				g_AlbumFrame[nCountAlbumFrame].nCount = 0;
				g_AlbumFrame[nCountAlbumFrame].vPos = gc_vAlbumFrameInitPos;
				g_AlbumFrame[nCountAlbumFrame].fScale = gc_fAlbumFrameInitScale;
				break;
			}
		}
	}
	else
	{
		for (int nCountAlbumFrame = 0; nCountAlbumFrame < gc_nMaxAlbumFrame; nCountAlbumFrame++)
		{
			g_AlbumFrame[nCountAlbumFrame].bStatus = false;
		}
	}
}

bool GetAllAlbumFrameStatus()
{
	bool bStatus = false;
	for (int nCountAlbumFrame = 0; nCountAlbumFrame < gc_nMaxAlbumFrame; nCountAlbumFrame++)
	{
		if (g_AlbumFrame[nCountAlbumFrame].bStatus)
		{
			bStatus = true;
			break;
		}
	}
	return bStatus;
}

void SetSongPanelA(int nParts)
{
	g_bPanelChangeA = true;
	for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
	{
		if (g_SongPanel[nCountPanel].vPos.x != gc_vInitPos.x)
		{
			g_SongPanel[nCountPanel].nOldA = g_SongPanel[nCountPanel].nA;
			g_SongPanel[nCountPanel].nA = gc_nSongPanelA[nParts];
			g_SongPanel[nCountPanel].nAddA = g_SongPanel[nCountPanel].nA - g_SongPanel[nCountPanel].nOldA;
			g_SongPanel[nCountPanel].nCountChangeA = 0;
		}
	}
}

bool GetSongPanelChangeAStatus()
{
	return g_bPanelChangeA;
}

void ChangeSelectNum(int nNum)
{
	g_nSelectNum = nNum;

	for (int nCountPanel = 0; nCountPanel < gc_nMaxPanel; nCountPanel++)
	{
		if (g_SongPanel[nCountPanel].vPos.x == gc_vInitPos.x)
		{
			g_SongPanel[nCountPanel].nSongNum = g_nSelectNum;
		}
		else
			if (g_SongPanel[nCountPanel].vPos.x > gc_vInitPos.x && g_SongPanel[nCountPanel].vPos.x <= gc_nMaxPanel / 2 * gc_vRangeX.x)
			{
				g_SongPanel[nCountPanel].nSongNum = g_nSelectNum + g_SongPanel[nCountPanel].vPos.x / gc_vRangeX.x;
			}
			else
				if (g_SongPanel[nCountPanel].vPos.x < gc_vInitPos.x && g_SongPanel[nCountPanel].vPos.x >= -gc_nMaxPanel / 2 * gc_vRangeX.x)
				{
					g_SongPanel[nCountPanel].nSongNum = g_nSelectNum + g_SongPanel[nCountPanel].vPos.x / gc_vRangeX.x;
				}
		while (g_SongPanel[nCountPanel].nSongNum < 0)
		{
			g_SongPanel[nCountPanel].nSongNum += g_nMaxFileNum;
		}
		while (g_SongPanel[nCountPanel].nSongNum > g_nMaxFileNum - 1)
		{
			g_SongPanel[nCountPanel].nSongNum -= g_nMaxFileNum;
		}
		SetTextureAddress(g_SongPanel[nCountPanel].nTexNum, &g_SSSongStatus[g_SongPanel[nCountPanel].nSongNum].c_AlbumFileAddress[0]);
	}
	if (GetPlayStatusM(MUSIC))
	{
		StopMusic(MUSIC);
	}
	g_nPanelSetMusicCount = gc_nPanelSetMusicCountMax - 1;
}

SSSONGSTATUS GetSSSongStatus(int nSongNum)
{
	return g_SSSongStatus[nSongNum];
}

bool GetIsDemo()
{
	return g_bIsDemo;
}
