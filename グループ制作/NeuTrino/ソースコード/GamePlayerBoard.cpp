#include "GamePlayerBoard.h"
#include "main.h"
#include "score.h"
#include "SelectSongWaitMember.h"
#include "Texture.h"
#include "Polygon.h"
#include "Light.h"
#include "data.h"
#include "name.h"
#include "icon.h"		//追加

#define PLAYER_INIT_X			(93.0f)
#define PLAYER_INIT_Y			(265.0f)
#define RIVAL_INIT_X			(1156.0f)
#define RIVAL_INIT_Y			(230.0f)
#define DIGIT_MAX				(7)
#define RANK_UPDATE				(30)		//ランクの更新フレーム数

static TEXTURE g_tex_player_board;
static TEXTURE g_tex_player_matching_rank;
static TEXTURE g_tex_rival_board;
static TEXTURE g_tex_rival_score_word;
static TEXTURE g_tex_rival_score_num;
static TEXTURE g_tex_player_icon;
static D3DXVECTOR2 g_player_board;
static D3DXVECTOR2 g_rival_board;
static float g_fRivalScore;
static int g_nPlayerRankV;
static int g_nRivalRankV;
static int g_nCntRankUpdate;
static bool g_bRivalStatus;

void InitGamePlayerBoard()
{
	g_tex_player_board = GetTextureStatus(TEXTURE_NUM::TEX_GAME_PLAYER_BOARD);
	g_tex_player_matching_rank = GetTextureStatus(TEXTURE_NUM::TEX_GAME_PLAYER_MATCHING_RANK);
	g_tex_rival_board = GetTextureStatus(TEXTURE_NUM::TEX_GAME_RIVAL_BOARD);
	g_tex_rival_score_word = GetTextureStatus(TEXTURE_NUM::TEX_GAME_SCORE_WORD);
	g_tex_rival_score_num = GetTextureStatus(TEXTURE_NUM::TEX_GAME_SCORE_NUM);
	g_tex_player_icon = GetTextureStatus(TEXTURE_NUM::TEX_FINAL_CHECK_PLAYER_ICON);
	g_player_board = D3DXVECTOR2(PLAYER_INIT_X, PLAYER_INIT_Y);
	g_rival_board = D3DXVECTOR2(RIVAL_INIT_X, RIVAL_INIT_Y);
	g_nPlayerRankV = 0;
	g_nRivalRankV = 0;
	g_nCntRankUpdate = 0;
	if (((GetRoomCreate() && Data::g_player.room_entry) || (GetRoomEntry() && Data::g_player.room_create)) && (GetRoomReady() && Data::g_player.ready))
	{
		g_bRivalStatus = true;
	}
	else
	{
		g_bRivalStatus = false;
	}
	InitText(FONTLIST::F_GAME_PLAYER_NAME, PLAYER_NAME_FONT_SIZE, "MS コジック");
	InitText(FONTLIST::F_GAME_RIVAL_NAME, RIVAL_NAME_FONT_SIZE, "MS コジック");
}

void UninitGamePlayerBoard()
{
	UninitText(FONTLIST::F_GAME_PLAYER_NAME);
	UninitText(FONTLIST::F_GAME_RIVAL_NAME);
	if (GetScore() > Data::g_player.score)
	{
		g_nPlayerRankV = 0;
		g_nRivalRankV = 1;
	}
	else if (GetScore() < Data::g_player.score)
	{
		g_nPlayerRankV = 1;
		g_nRivalRankV = 0;
	}
	else
	{
		g_nPlayerRankV = 0;
		g_nRivalRankV = 0;
	}
}

void UpdateGamePlayerBoard()
{

	g_fRivalScore = Data::g_player.display_score;
	g_nCntRankUpdate++;
	if (g_nCntRankUpdate == RANK_UPDATE)
	{
		if (GetScore() > Data::g_player.score)
		{
			g_nPlayerRankV = 0;
			g_nRivalRankV = 1;
		}
		else if(GetScore() < Data::g_player.score)
		{
			g_nPlayerRankV = 1;
			g_nRivalRankV = 0;
		}
		else
		{
			g_nPlayerRankV = 0;
			g_nRivalRankV = 0;
		}
		g_nCntRankUpdate = 0;
	}
}

void DrawGamePlayerBoard()
{
	SetLight(FALSE);
	SetZBuffer(FALSE);
	SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetTexture(TEXTURE_NUM::TEX_GAME_PLAYER_BOARD);
	DrawPolygon(g_player_board.x, g_player_board.y, 0, 0, g_tex_player_board.u, g_tex_player_board.v, false);
	SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_PLAYER_ICON);
	DrawPolygon(g_player_board.x - 30.0f, g_player_board.y + 12.0f, 0, g_tex_player_icon.v * GetIcon(), g_tex_player_icon.u, g_tex_player_icon.v, false);	//変更
	SetTextStatus(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);
	SetTextStyle(DT_LEFT);
	OutputText(FONTLIST::F_GAME_PLAYER_NAME, g_player_board.x - 80.0f, g_player_board.y - 68.0f, GetName().c_str());
	if (g_bRivalStatus && GetGameMode() != MODE_DEMO && GetGameMode() != MODE_TUTORIAL)
	{
		SetTexture(TEXTURE_NUM::TEX_GAME_PLAYER_MATCHING_RANK);
		DrawPolygon(g_player_board.x + 85.0f, g_player_board.y + 48.0f, 0, g_tex_player_matching_rank.v * g_nPlayerRankV, g_tex_player_matching_rank.u, g_tex_player_matching_rank.v, false);

		SetTexture(TEXTURE_NUM::TEX_GAME_RIVAL_BOARD);
		DrawPolygon(g_rival_board.x, g_rival_board.y, 0, 0, g_tex_rival_board.u, g_tex_rival_board.v, false);
		SetTexture(TEXTURE_NUM::TEX_FINAL_CHECK_PLAYER_ICON);
		SetScale(0.5f, 0.5f, 0, 0);
		DrawPolygon(g_rival_board.x - 36.0f, g_rival_board.y + 34.0f, 0, g_tex_player_icon.v * Data::g_player.character_icon, g_tex_player_icon.u, g_tex_player_icon.v, false);
		SetTextStatus(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);
		SetTextStyle(DT_LEFT);
		OutputText(FONTLIST::F_GAME_RIVAL_NAME, g_rival_board.x - 88.0f, g_rival_board.y - 35.0f, Data::g_player.name.c_str());
		SetTexture(TEXTURE_NUM::TEX_GAME_SCORE_WORD);
		SetScale(0.8f, 0.8f, 0, 0);
		DrawPolygon(g_rival_board.x, g_rival_board.y - 12.0f, 0, 0, g_tex_rival_score_word.u, g_tex_rival_score_word.v, false);

		SetTexture(TEXTURE_NUM::TEX_GAME_SCORE_NUM);
		int nRivalScore = (int)g_fRivalScore;
		int nDigit = 0;
		do
		{
			nRivalScore /= 10;
			nDigit++;
		} while (nRivalScore != 0);
		nRivalScore = (int)g_fRivalScore;
		int nCnt;
		for (nCnt = 0; nCnt < nDigit; nCnt++)
		{
			int nNum = nRivalScore % 10;
			SetScale(0.5f, 0.5f, 0, 0);
			DrawPolygon(g_rival_board.x + 54.0f - 12.0f * nCnt, g_rival_board.y + 16.0f, g_tex_rival_score_num.u * nNum, 0, g_tex_rival_score_num.u, g_tex_rival_score_num.v, false);
			nRivalScore /= 10;
		}
		SetTexture(TEXTURE_NUM::TEX_GAME_PLAYER_MATCHING_RANK);
		SetScale(0.5f, 0.5f, 0, 0);
		DrawPolygon(g_rival_board.x + 108.0f, g_rival_board.y + 40.0f, 0, g_tex_player_matching_rank.v * g_nRivalRankV, g_tex_player_matching_rank.u, g_tex_player_matching_rank.v, false);
	}
}