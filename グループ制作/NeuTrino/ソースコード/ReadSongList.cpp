#include "main.h"
#include "Note.h"
#include "debug.h"
#include "ReadSongList.h"
#include "SelectSongPanel.h"

#define SONGLIST_FILE            "Data/SongList/SongList.txt"
#define MAX_WORD                 (128)

static char g_cSaveNoteFileAddress[MAX_WORD];
static int g_nMaxSong;
static bool g_bStatus = false;

void InitReadSongList()
{
	if (!g_bStatus)
	{
		g_nMaxSong = 0;

		FILE *fSongList;

		fSongList = fopen(SONGLIST_FILE, "r");

		if (!fSongList)
		{
			MessageBox(NULL, "曲リストがねぇ！", "エラー", MB_OK);
		}
		else
		{
			while (fgets(g_cSaveNoteFileAddress, MAX_WORD, fSongList) != NULL)
			{

				if (g_cSaveNoteFileAddress[0] != '\n')
				{
					int len = (int)strlen(g_cSaveNoteFileAddress);

					while (g_cSaveNoteFileAddress[len - 1] == '\n' || g_cSaveNoteFileAddress[len - 1] == '\r')
					{
						g_cSaveNoteFileAddress[len - 1] = '\0';
						len--;
					}

					SetSongStatus(g_nMaxSong, &g_cSaveNoteFileAddress[0]);

					g_nMaxSong++;
					g_cSaveNoteFileAddress[0] == '\0';
				}
				else
				{
					g_cSaveNoteFileAddress[0] == '\0';
				}
			}

			fclose(fSongList);
		}
		g_bStatus = true;
	}
}

void DrawDebugRSL()
{
	SetDebugTextColor(D3DCOLOR_ARGB(255, 0, 0, 0));
	DebugText(0, 22, "MaxSong:%d", g_nMaxSong);
}

int GetMaxFileNum()
{
	return g_nMaxSong;
}