//----------------------------------------------------------------------------
//
//	譜面ファイルの読み取り
//
//														 制作者：劉喆
//                                                       作成日：2017/12/10
//----------------------------------------------------------------------------

//---------------------------インクルード-------------------------------------
#include "main.h"
#include "Btn.h"
#include "Note.h"
#include "Timer.h"
#include "debug.h"
#include "ReadNoteFile.h"
#include "SelectSongPanel.h"

//---------------------------マクロ定義---------------------------------------
//#define MAX_WORD               (128)

#define MARK_MISSION			(35)			// #
#define MARK_COMMENT			(42)			// *

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------
static LINE g_Line[NUM_BTN][MAX_NOTE_PER_LANE];
static int g_nCountNotePerCh[NUM_BTN];				// 小節の中に含めている音符の最大数

static float g_fBPM;
static float g_fScroll;
static float g_fMessare = 1.0f;
static float g_fMessarePart[2];
static int g_nOffset;
static int g_nCountLine;
static int g_nCountCh;
static int g_nLevel;
static bool g_bSlideStartEndFlag[NUM_BTN][NJS_MAX];	// スライド音符の開始/終了フラグ
static bool g_bSlideFlag[NUM_BTN];					// スライド音符の状態フラグ
static int g_nCountSlide[NUM_BTN];					// スライド音符の番号

static char g_cSStart[] = "START";
static char g_cSEnd[] = "END";
static char g_cSScroll[] = "SCROLL";
static char g_cSBPM[] = "BPM";
static char g_cSOffset[] = "OFFSET";
static char g_cCWaveAdd[] = "WAVEADDRESS";
static char g_cCMeasure[] = "MEASURE";
static char g_cCTitle[] = "TITLE";
static char g_cCSubtitle[] = "SUBTITLE";
static char g_cCLevel[MAX_LEVEL][7] =
{
	"BASIC",
	"NORMAL",
	"HARD",
};

static char g_cSaveMusicAdd[MAX_WORD];
static char g_cMusicAdd[MAX_WORD];
static char g_cSaveChar;
static char g_cSaveMission[32];
static char g_cTitle[MAX_WORD];
static char g_cSubtitle[MAX_WORD];

static char g_cNoteFileURL[MAX_WORD];

//---------------------------プロトタイプ宣言---------------------------------
void ResetSaveMission();
void ChangeChannelAndLine();
void SetNoteFromFile();

void InitReadNote()
{
	g_fBPM = 0;
	g_nOffset = 0;
	g_nCountLine = 0;
	g_nCountCh = 0;
	g_fScroll = 1.0f;
	g_nLevel = GetLevel();

	for (int nCountBtn = 0; nCountBtn < NUM_BTN; nCountBtn++)
	{
		g_nCountNotePerCh[nCountBtn] = 0;
		g_bSlideFlag[nCountBtn] = false;
		g_nCountSlide[nCountBtn] = 0;
		for (int nCountFlag = 0; nCountFlag < NJS_MAX; nCountFlag++)
		{
			g_bSlideStartEndFlag[nCountBtn][nCountFlag] = false;
		}
		for (int nCountNote = 0; nCountNote < MAX_NOTE_PER_LANE; nCountNote++)
		{
			g_Line[nCountBtn][nCountNote].fBPM = 0.0f;
			g_Line[nCountBtn][nCountNote].fMessare = 1.0f;
			g_Line[nCountBtn][nCountNote].nMaxNumOfNote = 0;
			g_Line[nCountBtn][nCountNote].nStartTime = 0;
		}
	}
}

void SetReadNote()
{
	// ファイルの読み取り
	FILE *pNoteFile;

	pNoteFile = fopen(g_cNoteFileURL, "r");

	if (!pNoteFile)
	{
		MessageBox(NULL, "譜面ファイルがねぇ！", "エラー", MB_OK);
	}
	else
	{
		// 音符を読むための前準備
		while (1)
		{
			g_cSaveChar = fgetc(pNoteFile);
			if (g_cSaveChar == MARK_COMMENT)	 // * = 42
			{
				ResetSaveMission();
				fscanf(pNoteFile, "%s", g_cSaveMission);
				ResetSaveMission();
			}
			if (g_cSaveChar == MARK_MISSION)	 // # = 35
			{
				ResetSaveMission();
				fscanf(pNoteFile, "%s", g_cSaveMission);
				if (strcmp(g_cSaveMission, g_cSBPM) == 0)
				{
					fscanf(pNoteFile, "%f", &g_fBPM);
				}
				if (strcmp(g_cSaveMission, g_cSScroll) == 0)
				{
					fscanf(pNoteFile, "%f", &g_fScroll);
				}
				if (strcmp(g_cSaveMission, g_cCMeasure) == 0)
				{
					fscanf(pNoteFile, "%f", &g_fMessarePart[0]);
					fscanf(pNoteFile, "%f", &g_fMessarePart[1]);

					g_fMessare = (float)(g_fMessarePart[0] / g_fMessarePart[1]);
				}
				if (strcmp(g_cSaveMission, g_cSOffset) == 0)
				{
					fscanf(pNoteFile, "%d", &g_nOffset);
				}
				if (strcmp(g_cSaveMission, g_cCTitle) == 0)
				{
					g_cTitle[0] = '\0';

					g_cSaveChar = fgetc(pNoteFile);
					fgets(g_cTitle, MAX_WORD, pNoteFile);

					int len = (int)strlen(g_cTitle);

					while (g_cTitle[len - 1] == '\n' || g_cTitle[len - 1] == '\r')
					{
						g_cTitle[len - 1] = '\0';
						len--;
					}
				}
				if (strcmp(g_cSaveMission, g_cCSubtitle) == 0)
				{
					g_cSubtitle[0] = '\0';

					g_cSaveChar = fgetc(pNoteFile);
					fgets(g_cSubtitle, MAX_WORD, pNoteFile);

					int len = (int)strlen(g_cSubtitle);

					while (g_cSubtitle[len - 1] == '\n' || g_cSubtitle[len - 1] == '\r')
					{
						g_cSubtitle[len - 1] = '\0';
						len--;
					}
				}
				if (strcmp(g_cSaveMission, g_cCWaveAdd) == 0)
				{
					g_cSaveChar = fgetc(pNoteFile);
					g_cSaveMusicAdd[0] = '\0';
					fgets(g_cSaveMusicAdd, 128, pNoteFile);

					int len = (int)strlen(g_cSaveMusicAdd);

					while (g_cSaveMusicAdd[len - 1] == '\n' || g_cSaveMusicAdd[len - 1] == '\r')
					{
						g_cSaveMusicAdd[len - 1] = '\0';
						len--;
					}

					//g_cMusicAdd[0] = '\0';

					//strcat(g_cMusicAdd, FILEADD);
					//strcat(g_cMusicAdd, g_cSaveMusicAdd);

					//SetMusicM(&g_cMusicAdd[0]);
				}
			}

			if (g_cSaveChar == 47)	// / = 47
			{
				ResetSaveMission();
				fscanf(pNoteFile, "%s", g_cSaveMission);

				if (strcmp(g_cSaveMission, g_cCLevel[g_nLevel]) == 0)
				{
					break;
				}
			}
		}

		// スタート命令が見つかる前に文字をスキップする
		while (1)
		{
			g_cSaveChar = fgetc(pNoteFile);
			if (g_cSaveChar == MARK_COMMENT)	 // * = 42
			{
				ResetSaveMission();
				fscanf(pNoteFile, "%s", g_cSaveMission);
				ResetSaveMission();
			}
			if (g_cSaveChar == MARK_MISSION)	 // # = 35
			{
				ResetSaveMission();
				fscanf(pNoteFile, "%s", g_cSaveMission);
				if (strcmp(g_cSaveMission, g_cSStart) == 0)
				{
					break;
				}
			}
		}

		// 音符の読み取り
		ResetSaveMission();
		while (1)
		{
			g_cSaveChar = fgetc(pNoteFile);
			if (g_cSaveChar == MARK_COMMENT)	 // * = 42
			{
				ResetSaveMission();
				fscanf(pNoteFile, "%s", g_cSaveMission);
				ResetSaveMission();
			}
			if (g_nCountCh < NUM_BTN_NORMAL)
			{
				switch (g_cSaveChar)
				{
				case 48:	 // 0 = 48
				{
					g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote++;
					break;
				}
				case 49:	 // 1 = 49
				{
					SetNoteLineAndNum
					(
						g_nCountCh,
						g_nCountNotePerCh[g_nCountCh],
						g_nCountLine,
						g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
						NJS_START
					);
					//SetNoteScroll(g_nCountCh, g_nCountNotePerCh[g_nCountCh], g_fScroll);

					SetNoteType(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NT_TAP);
					g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote++;
					g_nCountNotePerCh[g_nCountCh]++;
					break;
				}
				case 50:	 // 2 = 50
				{
					SetNoteLineAndNum
					(
						g_nCountCh,
						g_nCountNotePerCh[g_nCountCh],
						g_nCountLine,
						g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
						NJS_START
					);
					//SetNoteScroll(g_nCountCh, g_nCountNotePerCh[g_nCountCh], g_fScroll);

					SetNoteType(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NT_BREAK);
					g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote++;
					g_nCountNotePerCh[g_nCountCh]++;
					break;
				}
				case 51:	 // 3 = 51
				{
					SetNoteLineAndNum
					(
						g_nCountCh,
						g_nCountNotePerCh[g_nCountCh],
						g_nCountLine,
						g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
						NJS_START
					);
					//SetNoteScroll(g_nCountCh, g_nCountNotePerCh[g_nCountCh], g_fScroll);

					SetNoteType(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NT_HOLD);
					g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote++;

					break;
				}
				case 52:	 // 4 = 52
				{
					SetNoteLineAndNum
					(
						g_nCountCh,
						g_nCountNotePerCh[g_nCountCh],
						g_nCountLine,
						g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
						NJS_END
					);
					g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote++;
					g_nCountNotePerCh[g_nCountCh]++;

					break;
				}
				case 44:	 // , = 44
				{
					ChangeChannelAndLine();
					break;
				}
				default:
				{
					break;
				}
				}
			}
			else
			{
				switch (g_cSaveChar)
				{
				case 48:	 // 0 = 48
				{
					g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote++;
					break;
				}
				case 91:	 // [ = 91
				{
					g_bSlideFlag[g_nCountCh] = true;
					g_bSlideStartEndFlag[g_nCountCh][NJS_START] = true;
					break;
				}
				case 93:	 // ] = 91
				{
					g_bSlideFlag[g_nCountCh] = false;
					g_bSlideStartEndFlag[g_nCountCh][NJS_END] = true;
					break;
				}
				case 49:	 // 1 = 49
				case 50:	 // 2 = 50
				case 51:	 // 3 = 51
				case 52:	 // 4 = 52
				{
					if (!g_bSlideStartEndFlag[g_nCountCh][NJS_START] && !g_bSlideStartEndFlag[g_nCountCh][NJS_END] &&
						g_bSlideFlag[g_nCountCh])
					{
						SetSlideStartEndFlag(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, false, NJS_END);
						SetNoteSlideNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, g_nCountSlide[g_nCountCh]);
						SetNoteLineAndNum
						(
							g_nCountCh,
							g_nCountNotePerCh[g_nCountCh] - 1,
							g_nCountLine,
							g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote - 1,
							NJS_END
						);

						SetSlideStartEndFlag(g_nCountCh, g_nCountNotePerCh[g_nCountCh], false, NJS_START);
						SetNoteLineAndNum
						(
							g_nCountCh,
							g_nCountNotePerCh[g_nCountCh],
							g_nCountLine,
							g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote - 1,
							NJS_START
						);
						SetNoteType(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NT_SLIDE);

						NOTE Note = GetNote(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1);
						switch (g_cSaveChar)
						{
						case 49:	 // 5 = 53
						{
							switch (Note.nSlidePointPosNum[NJS_START])
							{
							case NSP_LEFT_DOWN:
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_UP, NJS_END);
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_LEFT_UP, NJS_START);
								break;
							}
							case NSP_RIGHT_DOWN:
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_UP, NJS_END);
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_RIGHT_UP, NJS_START);
								break;
							}
							}
							break;
						}
						case 50:	 // 6 = 54
						{
							switch (Note.nSlidePointPosNum[NJS_START])
							{
							case NSP_LEFT_UP:
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_DOWN, NJS_END);
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_LEFT_DOWN, NJS_START);
								break;
							}
							case NSP_RIGHT_UP:
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_DOWN, NJS_END);
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_RIGHT_DOWN, NJS_START);
								break;
							}
							}
							break;
						}
						case 51:	 // 7 = 55
						{
							switch (Note.nSlidePointPosNum[NJS_START])
							{
							case NSP_RIGHT_UP:
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_UP, NJS_END);
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_LEFT_UP, NJS_START);
								break;
							}
							case NSP_RIGHT_DOWN:
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_DOWN, NJS_END);
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_LEFT_DOWN, NJS_START);
								break;
							}
							}
							break;
						}
						case 52:	 // 8 = 56
						{
							switch (Note.nSlidePointPosNum[NJS_START])
							{
							case NSP_LEFT_UP:
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_UP, NJS_END);
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_RIGHT_UP, NJS_START);
								break;
							}
							case NSP_LEFT_DOWN:
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_DOWN, NJS_END);
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_RIGHT_DOWN, NJS_START);
								break;
							}
							}
							break;
						}
						}
					}
					else
						if (!g_bSlideStartEndFlag[g_nCountCh][NJS_START] && g_bSlideStartEndFlag[g_nCountCh][NJS_END])
						{
							SetSlideStartEndFlag(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, true, NJS_END);
							SetNoteSlideNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, g_nCountSlide[g_nCountCh]);
							g_nCountSlide[g_nCountCh]++;
							g_bSlideStartEndFlag[g_nCountCh][NJS_END] = false;
							SetNoteLineAndNum
							(
								g_nCountCh,
								g_nCountNotePerCh[g_nCountCh] - 1,
								g_nCountLine,
								g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote - 1,
								NJS_END
							);

							NOTE Note = GetNote(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1);
							switch (g_cSaveChar)
							{
							case 49:	 // 5 = 53
							{
								switch (Note.nSlidePointPosNum[NJS_START])
								{
								case NSP_LEFT_DOWN:
								{
									SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_UP, NJS_END);
									break;
								}
								case NSP_RIGHT_DOWN:
								{
									SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_UP, NJS_END);
									break;
								}
								}
								break;
							}
							case 50:	 // 6 = 54
							{
								switch (Note.nSlidePointPosNum[NJS_START])
								{
								case NSP_LEFT_UP:
								{
									SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_DOWN, NJS_END);
									break;
								}
								case NSP_RIGHT_UP:
								{
									SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_DOWN, NJS_END);
									break;
								}
								}
								break;
							}
							case 51:	 // 7 = 55
							{
								switch (Note.nSlidePointPosNum[NJS_START])
								{
								case NSP_RIGHT_UP:
								{
									SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_UP, NJS_END);
									break;
								}
								case NSP_RIGHT_DOWN:
								{
									SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_DOWN, NJS_END);
									break;
								}
								}
								break;
							}
							case 52:	 // 8 = 56
							{
								switch (Note.nSlidePointPosNum[NJS_START])
								{
								case NSP_LEFT_UP:
								{
									SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_UP, NJS_END);
									break;
								}
								case NSP_LEFT_DOWN:
								{
									SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_DOWN, NJS_END);
									break;
								}
								}
								break;
							}
							}
							break;
						}
						else
						if (!g_bSlideStartEndFlag[g_nCountCh][NJS_START] && !g_bSlideStartEndFlag[g_nCountCh][NJS_END] &&
							!g_bSlideFlag[g_nCountCh])
						{
							SetNoteLineAndNum
							(
								g_nCountCh,
								g_nCountNotePerCh[g_nCountCh],
								g_nCountLine,
								g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
								NJS_START
							);
							SetNoteType(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NT_FLICK);
							switch (g_cSaveChar)
							{
							case 49:
							{
								SetNoteDirect(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NFD_UP);
								break;
							}
							case 50:
							{
								SetNoteDirect(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NFD_DOWN);
								break;
							}
							case 51:
							{
								SetNoteDirect(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NFD_LEFT);
								break;
							}
							case 52:
							{
								SetNoteDirect(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NFD_RIGHT);
								break;
							}
							}
							g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote++;
						}
						g_nCountNotePerCh[g_nCountCh]++;
						break;
				}

				case 53:	 // 5 = 53
				case 54:	 // 6 = 54
				case 55:	 // 7 = 55
				case 56:	 // 8 = 56
				{
					if (g_bSlideStartEndFlag[g_nCountCh][NJS_START] && !g_bSlideStartEndFlag[g_nCountCh][NJS_END])
					{
						g_bSlideStartEndFlag[g_nCountCh][NJS_START] = false;
						SetSlideStartEndFlag(g_nCountCh, g_nCountNotePerCh[g_nCountCh], true, NJS_START);
						SetNoteSlideNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], g_nCountSlide[g_nCountCh]);
						SetNoteLineAndNum
						(
							g_nCountCh,
							g_nCountNotePerCh[g_nCountCh],
							g_nCountLine,
							g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
							NJS_START
						);
						SetNoteType(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NT_SLIDE);
						switch (g_cSaveChar)
						{
						case 53:	 // 5 = 53
						{
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_LEFT_UP, NJS_START);
							break;
						}
						case 54:	 // 6 = 54
						{
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_RIGHT_UP, NJS_START);
							break;
						}
						case 55:	 // 7 = 55
						{
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_LEFT_DOWN, NJS_START);
							break;
						}
						case 56:	 // 8 = 56
						{
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_RIGHT_DOWN, NJS_START);
							break;
						}
						}
						g_nCountNotePerCh[g_nCountCh]++;
					}
					else
					if (!g_bSlideStartEndFlag[g_nCountCh][NJS_START] && !g_bSlideStartEndFlag[g_nCountCh][NJS_END]&&
						g_bSlideFlag[g_nCountCh])
					{
						SetSlideStartEndFlag(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, false, NJS_END);
						SetNoteLineAndNum
						(
							g_nCountCh,
							g_nCountNotePerCh[g_nCountCh] - 1,
							g_nCountLine,
							g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
							NJS_END
						);

						SetSlideStartEndFlag(g_nCountCh, g_nCountNotePerCh[g_nCountCh], false, NJS_START);
						SetNoteLineAndNum
						(
							g_nCountCh,
							g_nCountNotePerCh[g_nCountCh],
							g_nCountLine,
							g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
							NJS_START
						);
						SetNoteType(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NT_SLIDE);
						SetNoteSlideNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, g_nCountSlide[g_nCountCh]);
						SetNoteSlideNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], g_nCountSlide[g_nCountCh]);

						switch (g_cSaveChar)
						{
						case 53:	 // 5 = 53
						{
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_UP, NJS_END);
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_LEFT_UP, NJS_START);
							break;
						}
						case 54:	 // 6 = 54
						{
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_UP, NJS_END);
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_RIGHT_UP, NJS_START);
							break;
						}
						case 55:	 // 7 = 55
						{
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_DOWN, NJS_END);
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_LEFT_DOWN, NJS_START);
							break;
						}
						case 56:	 // 8 = 56
						{
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_DOWN, NJS_END);
							SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh], NSP_RIGHT_DOWN, NJS_START);
							break;
						}
						}
						g_nCountNotePerCh[g_nCountCh]++;
					}
					else
						if (!g_bSlideStartEndFlag[g_nCountCh][NJS_START] && g_bSlideStartEndFlag[g_nCountCh][NJS_END])
						{
							g_bSlideStartEndFlag[g_nCountCh][NJS_END] = false;
							SetSlideStartEndFlag(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, true, NJS_END);
							SetNoteLineAndNum
							(
								g_nCountCh,
								g_nCountNotePerCh[g_nCountCh] - 1,
								g_nCountLine,
								g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote,
								NJS_END
							);
							SetNoteSlideNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, g_nCountSlide[g_nCountCh]);
							g_nCountSlide[g_nCountCh]++;
							switch (g_cSaveChar)
							{
							case 53:	 // 5 = 53
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_UP, NJS_END);
								break;
							}
							case 54:	 // 6 = 54
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_UP, NJS_END);
								break;
							}
							case 55:	 // 7 = 55
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_LEFT_DOWN, NJS_END);
								break;
							}
							case 56:	 // 8 = 56
							{
								SetSlidePointPosNum(g_nCountCh, g_nCountNotePerCh[g_nCountCh] - 1, NSP_RIGHT_DOWN, NJS_END);
								break;
							}
							}
						}
					g_Line[g_nCountCh][g_nCountLine].nMaxNumOfNote++;
					break;
				}


				case 44:	 // , = 44
				{
					ChangeChannelAndLine();
					break;
				}
				default:
				{
					break;
				}
				}
			}

			if (g_cSaveChar == MARK_MISSION)	 // # = 35
			{
				fscanf(pNoteFile, "%s", g_cSaveMission);
				if (strcmp(g_cSaveMission, g_cSEnd) == 0)
				{
					ResetSaveMission();
					break;
				}
				if (strcmp(g_cSaveMission, g_cSBPM) == 0)
				{
					fscanf(pNoteFile, "%f", &g_fBPM);
					ResetSaveMission();
				}
				if (strcmp(g_cSaveMission, g_cSScroll) == 0)
				{
					fscanf(pNoteFile, "%f", &g_fScroll);
					ResetSaveMission();
				}
				if (strcmp(g_cSaveMission, g_cCMeasure) == 0)
				{
					fscanf(pNoteFile, "%f", &g_fMessarePart[0]);
					fscanf(pNoteFile, "%f", &g_fMessarePart[1]);

					g_fMessare = (float)(g_fMessarePart[0] / g_fMessarePart[1]);
					ResetSaveMission();
				}
			}
		}
		fclose(pNoteFile);

		SetNoteFromFile();
	}
}

void UninitReadNote()
{

}

void DrawReadNoteDebug()
{
	//DebugText(0, 22 * 13, "BPM:%.2f", g_fBPM);
	//DebugText(0, 22 * 14, "Offset:%d", g_nOffset);
	//DebugText(0, 22 * 15, "Mission:%s", &g_cSaveMission);
	//DebugText(0, 22 * 16, "Channel:%d", g_nCountCh);
	//DebugText(0, 22 * 17, "LineNum:%d", g_nCountLine);
	//DebugText(0, 22 * 18, "MaxNoteNum:%d", g_Line[0][0].nMaxNumOfNote);
	//DebugText(0, 22 * 19, "LineTime:%f", g_Line[0][g_nCountLine - 1].nStartTime);
	//DebugText(0, 22 * 23, "LineBPM:%f", g_Line[0][0].fBPM);

	for (int nCountBtn = 0; nCountBtn < NUM_BTN; nCountBtn++)
	{
		DebugText(0, 22 * 24 + 22 * nCountBtn, "MN:%d", g_nCountNotePerCh[nCountBtn]);
	}

	DebugText(200, 22 * 4, "FileAddress:%s", &g_cSaveMusicAdd[0]);
	DebugText(200, 22 * 5, "Measure:%.2f", g_fMessare);
	DebugText(200, 22 * 6, "Title:%s", &g_cTitle[0]);
}

void ChangeChannelAndLine()
{
	g_Line[g_nCountCh][g_nCountLine].fBPM = g_fBPM;
	g_Line[g_nCountCh][g_nCountLine].fMessare = g_fMessare;
	if (g_nCountLine > 0)
	{
		g_Line[g_nCountCh][g_nCountLine].nStartTime = g_Line[g_nCountCh][g_nCountLine - 1].nStartTime + GetBPMTime(g_Line[g_nCountCh][g_nCountLine - 1].fMessare,g_Line[g_nCountCh][g_nCountLine - 1].fBPM);
	}
	if (g_nCountCh < NUM_BTN)
	{
		g_nCountCh++;
	}
	if (g_nCountCh == NUM_BTN)
	{
		g_nCountCh = 0;
		g_nCountLine++;
	}
}

float GetBPMTime(float fMeasure, float fBPM)
{
	float fTime = 240000.000f / (fBPM)* fMeasure;
	return fTime;
}

void ResetSaveMission()
{
	g_cSaveMission[0] = '\0';
}

void SetNoteFromFile()
{
	for (int CountCh = 0; CountCh < NUM_BTN; CountCh++)
	{
		SetMaxNotePerCh(CountCh, g_nCountNotePerCh[CountCh]);
		for (int CountNote = 0; CountNote < g_nCountNotePerCh[CountCh]; CountNote++)
		{
			NOTE Note = GetNote(CountCh, CountNote);

			switch (Note.nType)
			{
			case NT_TAP:
			case NT_BREAK:
			case NT_FLICK:
			{
				float fTiming = (float)g_nOffset + GetBPMTime
				(g_Line[CountCh][Note.nNumOfLine[NJS_START]].fMessare, g_Line[CountCh][Note.nNumOfLine[NJS_START]].fBPM)*
					(Note.nNumOfNote[NJS_START] * 1.0f / g_Line[CountCh][Note.nNumOfLine[NJS_START]].nMaxNumOfNote * 1.0f);
				if (Note.nNumOfLine[NJS_START] > 0)
				{
					fTiming += g_Line[CountCh][Note.nNumOfLine[NJS_START]].nStartTime;
				}
				SetNoteTime(CountCh, CountNote, fTiming, NJS_START);
				break;
			}
			case NT_HOLD:
			case NT_SLIDE:
			{
				float fTimingS = (float)g_nOffset + GetBPMTime
				(g_Line[CountCh][Note.nNumOfLine[NJS_START]].fMessare, g_Line[CountCh][Note.nNumOfLine[NJS_START]].fBPM)*
					(Note.nNumOfNote[NJS_START] * 1.0f / g_Line[CountCh][Note.nNumOfLine[NJS_START]].nMaxNumOfNote * 1.0f);
				if (Note.nNumOfLine[NJS_START] > 0)
				{
					fTimingS += g_Line[CountCh][Note.nNumOfLine[NJS_START]].nStartTime;
				}
				float fTimingE = (float)g_nOffset + GetBPMTime
				(g_Line[CountCh][Note.nNumOfLine[NJS_END]].fMessare, g_Line[CountCh][Note.nNumOfLine[NJS_END]].fBPM)*
					(Note.nNumOfNote[NJS_END] * 1.0f / g_Line[CountCh][Note.nNumOfLine[NJS_END]].nMaxNumOfNote * 1.0f);
				if (Note.nNumOfLine[NJS_END] > 0)
				{
					fTimingE += g_Line[CountCh][Note.nNumOfLine[NJS_END]].nStartTime;
				}
				SetNoteTime(CountCh, CountNote, fTimingS, NJS_START);
				SetNoteTime(CountCh, CountNote, fTimingE, NJS_END);
				break;
			}
			}
		}
	}
}

int GetMaxLineNum()
{
	return g_nCountLine;
}

LINE GetLine(int nNum)
{
	return g_Line[0][nNum];
}

int GetRecordLine(int nTime)
{
	int count = 0;

	for (count = 0; count < g_nCountLine; count++)
	{
		if (g_Line[0][count].nStartTime + g_nOffset >= nTime)
		{
			break;
		}
	}
	return count;
}

int GetNoteOffsetFromFile()
{
	return g_nOffset;
}

void SetLineOffset(int nOffset)
{
	for (int nCountBtn = 0; nCountBtn < NUM_BTN; nCountBtn++)
	{
		for (int count = 0; count < g_nCountLine; count++)
		{
			g_Line[nCountBtn][count].nStartTime += nOffset;
		}
	}
}

void SetNoteFileURL(char *cURL)
{
	g_cNoteFileURL[0] = '\0';
	strcat(g_cNoteFileURL, cURL);
}