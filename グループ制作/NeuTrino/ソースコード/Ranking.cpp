#include "Ranking.h"
#include "main.h"
#include "ReadSongList.h"
#include "SelectSongPanel.h"
#include "SelectSongFinalCheck.h"
#include "SelectSongWaitMember.h"
#include "game.h"
#include "Setting.h"
#include "ErrorList.h"
#include "score.h"
#include "Texture.h"
#include "Polygon.h"
#include "input.h"
#include "JoyStick.h"
#include "Light.h"
#include "debug.h"
#include "name.h"
#include "data.h"

#include <vector>
#include <fstream>
#include <string>

#define RANKING_FILE			("Data/Ranking/R")
#define SONGLIST_FILE			("Data/SongList/SongList.txt")
#define FILE_EXTENSION			(".ner")
#define RANKING_SELECT_INIT_X	(915.0f)
#define RANKING_SELECT_INIT_Y	(240.0f)
#define RANKING_RESULT_INIT_X	(840.0f)
#define RANKING_RESULT_INIT_Y	(170.0f)
#define RANKING_MAX				(3)
#define RANKING_FONT_SIZE		(28)
#define DRAW_TIME				(60)							//何フレーム未入力で描画
#define DRAW_FADE				(6)
#define DRAW_WAIT				(DRAW_FADE + 120)
#define DRAW_MY_RANK_ANIM		(32)

static TEXTURE g_tex_bg;
static TEXTURE g_tex_header;
static TEXTURE g_tex_rank;
static TEXTURE g_tex_score_num;
static TEXTURE g_tex_result_ranking_filter;
static D3DXVECTOR2 g_ranking;
static bool g_bDrawRanking;
static int g_nDrawCnt;
static int g_nDrawRanking;
static int g_nMyRank;
static std::string g_sRankingName[RANKING_MAX];
static int g_nRankingScore[RANKING_MAX];
static float g_fAlfaBg;
static float g_fAlfa;
static float g_fAlfaMyRank;
static int g_nMyRankAnimCnt;
static int g_nGetLevelOld;
static int g_nGetLevel;

std::string GetSongName(int song_num);

void InitRanking()
{
	g_tex_bg = GetTextureStatus(TEXTURE_NUM::TEX_RANKING_BG);
	g_tex_header = GetTextureStatus(TEXTURE_NUM::TEX_RANKING_HEADER);
	g_tex_rank = GetTextureStatus(TEXTURE_NUM::TEX_RANKING_RANK);
	g_tex_score_num = GetTextureStatus(TEXTURE_NUM::TEX_RANKING_SCORE_NUM);
	g_tex_result_ranking_filter = GetTextureStatus(TEXTURE_NUM::TEX_RESULT_RANKING_FILTER);
	if (GetGameModeTarget() == GAMEMODE::MODE_RESULT)
	{
		g_ranking = D3DXVECTOR2(RANKING_RESULT_INIT_X, RANKING_RESULT_INIT_Y);
		g_nDrawRanking = g_nMyRank;
	}
	else
	{
		g_ranking = D3DXVECTOR2(RANKING_SELECT_INIT_X, RANKING_SELECT_INIT_Y);
		g_nMyRank = -1;
		g_nDrawRanking = 0;
	}
	g_bDrawRanking = false;
	g_nMyRankAnimCnt = g_nDrawCnt = 0;
	g_fAlfaMyRank = g_fAlfaBg = g_fAlfa = 0.0f;
	InitText(FONTLIST::F_RANKING_NAME, RANKING_FONT_SIZE, "MS コジック");
	g_nGetLevel = GetLevel();
	g_nGetLevelOld = g_nGetLevel;
}

void UninitRanking()
{
	UninitText(FONTLIST::F_RANKING_NAME);
}

void UpdateRanking()
{
	switch (GetGameMode())
	{
	case MODE_SELECT_SONG:
	{
		if (!GetSelectSongFinalCheckStatus()/* && !GetSettingStatus() */ &&
			!GetKeyboardRepeat(DIK_LEFT) && !GetPOVRepeat(JOY_LEFT) &&
			!GetKeyboardRepeat(DIK_RIGHT) && !GetPOVRepeat(JOY_RIGHT))
		{
			g_nDrawCnt++;
		}
		else
			if (GetSelectSongFinalCheckStatus() ||
				GetKeyboardRepeat(DIK_LEFT) || GetPOVRepeat(JOY_LEFT) ||
				GetKeyboardRepeat(DIK_RIGHT) || GetPOVRepeat(JOY_RIGHT))
			{
				if (!GetSettingStatus() && !GetErrorListStatus())
				{
					g_bDrawRanking = false;
					g_nDrawCnt = g_nDrawRanking = 0;
					g_fAlfaBg = g_fAlfa = 0.0f;
				}
			}

		if ((g_nDrawCnt > DRAW_TIME) && (g_nDrawCnt <= (DRAW_TIME + DRAW_FADE)))
		{
			g_fAlfaBg += 1.0f / DRAW_FADE;
			if (g_fAlfaBg > 1.0f)
				g_fAlfaBg = 1.0f;
			g_fAlfa += 1.0f / DRAW_FADE;
		}
		else if ((g_nDrawCnt > (DRAW_TIME + DRAW_WAIT)) && (g_nDrawCnt <= (DRAW_TIME + DRAW_WAIT + DRAW_FADE)))
			g_fAlfa -= 1.0f / DRAW_FADE;
		else if (g_nDrawCnt > (DRAW_TIME + DRAW_WAIT + DRAW_FADE))
		{
			g_fAlfa = 0.0f;
			if (g_nDrawRanking < RANKING_MAX - 1)
				g_nDrawRanking++;
			else
				g_nDrawRanking = 0;
			g_nDrawCnt = DRAW_TIME + 1;
		}

		if ((g_nDrawCnt == DRAW_TIME) ||
			((g_nDrawCnt > DRAW_TIME) && (g_nDrawCnt <= (DRAW_TIME + DRAW_WAIT + DRAW_FADE + 1)) &&
			(GetKeyboardRepeat(DIK_UP) || GetPOVRepeat(JOY_UP)) ||
				(GetKeyboardRepeat(DIK_DOWN) || GetPOVRepeat(JOY_DOWN))) && !GetSelectSongFinalCheckStatus())
		{
			std::string ranking_file = RANKING_FILE + GetSongName(GetSelectNum()) + std::to_string(GetLevel()) + FILE_EXTENSION;

			std::ifstream input_file(ranking_file);
			std::string input_line;
			bool bNameLine = false;
			int nLineCnt = 0;
			while (std::getline(input_file, input_line))
			{
				if (bNameLine)
				{
					g_sRankingName[nLineCnt] = input_line;
					bNameLine = false;
					nLineCnt++;
				}
				else
				{
					g_nRankingScore[nLineCnt] = atoi(input_line.c_str());
					bNameLine = true;
				}
			}
			g_bDrawRanking = true;

			g_nGetLevel = GetLevel();
			if (g_nGetLevelOld != g_nGetLevel && g_nDrawCnt > DRAW_TIME)
			{
				g_nDrawCnt = DRAW_TIME + DRAW_FADE;
				g_nDrawRanking = 0;
				g_fAlfaBg = g_fAlfa = 1.0f;
				g_nGetLevelOld = g_nGetLevel;
			}
		}
		break;
	}
	case MODE_RESULT:
	{
		if (g_nMyRank != -1)
		{
			g_nDrawCnt++;
			if ((g_nDrawCnt == DRAW_TIME) ||
				((g_nDrawCnt > DRAW_TIME) && (g_nDrawCnt <= (DRAW_TIME + DRAW_WAIT + DRAW_FADE + 1))))
			{
				std::string ranking_file = RANKING_FILE + GetSongName(GetSelectNum()) + std::to_string(GetLevel()) + FILE_EXTENSION;

				std::ifstream input_file(ranking_file);
				std::string input_line;
				bool bNameLine = false;
				int nLineCnt = 0;
				while (std::getline(input_file, input_line))
				{
					if (bNameLine)
					{
						g_sRankingName[nLineCnt] = input_line;
						bNameLine = false;
						nLineCnt++;
					}
					else
					{
						g_nRankingScore[nLineCnt] = atoi(input_line.c_str());
						bNameLine = true;
					}
				}
				g_bDrawRanking = true;
			}

			if ((g_nDrawCnt > DRAW_TIME) && (g_nDrawCnt <= (DRAW_TIME + DRAW_FADE)))
			{
				g_fAlfaBg += 1.0f / DRAW_FADE;
				if (g_fAlfaBg > 1.0f)
					g_fAlfaBg = 1.0f;
				g_fAlfa += 1.0f / DRAW_FADE;
			}
			else if ((g_nDrawCnt > (DRAW_TIME + DRAW_WAIT)) && (g_nDrawCnt <= (DRAW_TIME + DRAW_WAIT + DRAW_FADE)))
				g_fAlfa -= 1.0f / DRAW_FADE;
			else if (g_nDrawCnt > (DRAW_TIME + DRAW_WAIT + DRAW_FADE))
			{
				g_fAlfa = 0.0f;
				if (g_nDrawRanking < RANKING_MAX - 1)
					g_nDrawRanking++;
				else
					g_nDrawRanking = 0;
				g_nDrawCnt = DRAW_TIME + 1;
			}
			if (g_nDrawRanking == g_nMyRank)
			{
				if (g_nMyRankAnimCnt < DRAW_MY_RANK_ANIM)
				{
					g_fAlfaMyRank += 0.2f / DRAW_MY_RANK_ANIM;
				}
				else if (DRAW_MY_RANK_ANIM <= g_nMyRankAnimCnt)
				{
					g_fAlfaMyRank -= 0.2f / DRAW_MY_RANK_ANIM;
				}
				if (g_nMyRankAnimCnt == DRAW_MY_RANK_ANIM * 2)
				{
					g_fAlfaMyRank = 0.0f;
					g_nMyRankAnimCnt = 0;
				}
				g_nMyRankAnimCnt++;
			}
			else
			{
				g_nMyRankAnimCnt = 0;
			}
		}
		break;
	}
	}
}

void DrawRanking()
{
	if (g_bDrawRanking)
	{
		SetLight(FALSE);
		SetZBuffer(FALSE);
		SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlfaBg));
		SetTexture(TEXTURE_NUM::TEX_RANKING_BG);
		DrawPolygon(g_ranking.x, g_ranking.y, 0, 0, g_tex_bg.u, g_tex_bg.v, false);
		SetTexture(TEXTURE_NUM::TEX_RANKING_HEADER);
		DrawPolygon(g_ranking.x, g_ranking.y - 44.0f, 0, g_tex_header.v * GetLevel(), g_tex_header.u, g_tex_header.v, false);
		SetTextStatus(D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fAlfa), D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fAlfa), 0);
		SetTextStyle(DT_LEFT);
		OutputText(FONTLIST::F_RANKING_NAME, g_ranking.x - 100.0f, g_ranking.y - 28.0f, g_sRankingName[g_nDrawRanking].c_str());
		SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlfa));
		SetTexture(TEXTURE_NUM::TEX_RANKING_RANK);
		DrawPolygon(g_ranking.x - 85.0f, g_ranking.y + 28.0f, 0, g_tex_rank.v * g_nDrawRanking, g_tex_rank.u, g_tex_rank.v, false);
		SetTexture(TEXTURE_NUM::TEX_RANKING_SCORE_NUM);
		int nRankingScore = g_nRankingScore[g_nDrawRanking];
		int nDigit = 0;
		do
		{
			nRankingScore /= 10;
			nDigit++;
		} while (nRankingScore != 0);
		nRankingScore = g_nRankingScore[g_nDrawRanking];
		int nCnt;
		for (nCnt = 0; nCnt < nDigit; nCnt++)
		{
			int nNum = nRankingScore % 10;
			DrawPolygon(g_ranking.x + 75.0f - 16.0f * nCnt, g_ranking.y + 28.0f, g_tex_score_num.u * nNum, 0, g_tex_score_num.u, g_tex_score_num.v, false);
			nRankingScore /= 10;
		}
		if (g_nDrawRanking == g_nMyRank)
		{
			SetBlend(D3DBLEND_BLENDFACTOR);
			SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlfaMyRank));
			SetTexture(TEXTURE_NUM::TEX_RESULT_RANKING_FILTER);
			DrawPolygon(g_ranking.x, g_ranking.y - 3.0f, 0, 0, g_tex_result_ranking_filter.u, g_tex_result_ranking_filter.v, false);
			SetBlend(D3DBLEND_INVSRCALPHA);
		}
	}
	DebugText(0, 480, "GameMode:%d", GetGameMode());
	DebugText(0, 500, "DrawRanking:%d", g_nDrawRanking);
	DebugText(0, 520, "MyRank:%d", g_nMyRank);
}

void SetRanking()
{
	if (GetOnline())
	{
		std::vector<std::string> file_data;
		std::string ranking_file = RANKING_FILE + GetSongName(Data::g_player.music) + std::to_string(Data::g_player.music_level) + FILE_EXTENSION;
		std::ifstream input_file(ranking_file);
		std::string input_line;
		while (std::getline(input_file, input_line)) { file_data.push_back(input_line); }

		std::ofstream output_file(ranking_file);
		int nDataCnt = 0;
		bool bRankChange = false;
		bool bChange = false;
		std::string sScore1;
		std::string sScore2;
		std::string sName1;
		std::string sName2;
		for (auto data : file_data)
		{
			if (nDataCnt % 2 == 0)
			{
				if ((atoi(data.c_str()) <= (int)Data::g_player.score) && !bChange)
				{
					sScore1 = data;
					data = std::to_string((int)Data::g_player.score);
					bRankChange = true;
				}
				else if (bChange)
				{
					sScore2 = data;
					data = sScore1;
					sScore1 = sScore2;
				}
				output_file << data + "\n";
			}
			else
			{
				if (bRankChange)
				{
					sName1 = data;
					data = Data::g_player.name;
					bRankChange = false;
					bChange = true;
				}
				else if (bChange)
				{
					sName2 = data;
					data = sName1;
					sName1 = sName2;
				}
				output_file << data + "\n";
			}
			nDataCnt++;
			if (nDataCnt == 6) break;
		}
	}
	std::vector<std::string> file_data;
	std::string ranking_file = RANKING_FILE + GetSongName(GetSelectNum()) + std::to_string(GetLevel()) + FILE_EXTENSION;

	std::ifstream input_file(ranking_file);
	std::string input_line;
	while (std::getline(input_file, input_line)) { file_data.push_back(input_line); }

	std::ofstream output_file(ranking_file);
	int nDataCnt = 0;
	bool bRankChange = false;
	bool bChange = false;
	std::string sScore1;
	std::string sScore2;
	std::string sName1;
	std::string sName2;
	for (auto data : file_data)
	{
		if (nDataCnt % 2 == 0)
		{
			if ((atoi(data.c_str()) <= GetScore()) && !bChange)
			{
				sScore1 = data;
				data = std::to_string((int)GetScore());
				bRankChange = true;
				g_nMyRank = nDataCnt / 2;
			}
			else if (bChange)
			{
				sScore2 = data;
				data = sScore1;
				sScore1 = sScore2;
			}
			output_file << data + "\n";
		}
		else
		{
			if (bRankChange)
			{
				sName1 = data;
				data = GetName();
				bRankChange = false;
				bChange = true;
			}
			else if (bChange)
			{
				sName2 = data;
				data = sName1;
				sName1 = sName2;
			}
			output_file << data + "\n";
		}
		nDataCnt++;
		if (nDataCnt == 6) break;
	}
}

void CheckRanking()
{
	for (int nSelectNum = 0; nSelectNum < GetMaxFileNum(); nSelectNum++)
	{
		for (int nLevel = 0; nLevel < MAX_LEVEL; nLevel++)
		{
			std::string ranking_file(RANKING_FILE + GetSongName(nSelectNum) + std::to_string(nLevel) + FILE_EXTENSION);
			std::ifstream load_file(ranking_file);
			if (load_file.fail())
			{
				std::ofstream output_file(ranking_file);
				//ここでリセット
				output_file << "700000\n";
				output_file << "NEU\n";
				output_file << "600000\n";
				output_file << "TRI\n";
				output_file << "500000\n";
				output_file << "NO\n";
			}
		}
	}
}

std::string GetSongName(int song_num)
{
	std::ifstream song_list(SONGLIST_FILE);
	std::string song_name;
	for (int nCnt = 0; nCnt <= song_num; nCnt++)
	{
		std::getline(song_list, song_name);
	}
	int a = song_name.size();
	std::string extention = FILE_EXTENSION;
	for (auto song : extention)
	{
		song_name.erase(song_name.begin() + song_name.size() - 1);
	}
	return song_name;
}