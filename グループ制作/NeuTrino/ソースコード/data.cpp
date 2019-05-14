#include"data.h"
#include"main.h"
#include"network.h"
#include"score.h"
#include"SelectSongPanel.h"
#include"SelectSongFinalCheck.h"
#include"SelectSongWaitMember.h"
#include"game.h"
#include"debug.h"
#include"input.h"
#include "game.h"
#include "score.h"
#include "GamePreface.h"
#include "name.h"
#include "icon.h"		//追加

Player Data::g_player;
static unsigned int g_nSendCnt;
static const unsigned int gc_nSendFrame = 2;	//何フレーム毎に送信するか

void UpdateSendData()
{
	g_nSendCnt++;
	if (g_nSendCnt % gc_nSendFrame == 0)
	{
		Player player = {};
		player.name = GetName();		//変更点
		player.game_mode = GetGameMode();
		player.music = GetSelectNum();
		player.music_level = GetLevel();
		player.score = GetScore();
		switch (player.game_mode)
		{
		case MODE_SELECT_SONG:
			player.music_level_num = GetLevelNum();
			//player.score = 0.0f;
			player.character_icon = GetIcon();		//追加
			player.room_create = GetRoomCreate();
			player.room_entry = GetRoomEntry();
			player.ready = GetRoomReady();
			break;
		case MODE_GAME:
			player.game_preface = GetGamePreface();
			player.display_score = GetDisplayScore();
			player.character_icon = GetIcon();		//追加
			player.room_create = GetRoomCreate();
			player.room_entry = GetRoomEntry();
			player.ready = GetRoomReady();
			player.init_game = GetInitGame();
			player.multiplay_cancel = GetMultiPlayCancel();
			break;
		}
		g_nSendCnt = 0;
		SendUDP(&player);
	}
}

void DrawRecvDataDebug()
{
	DebugText(1000, 120, "RecvName			: %s", Data::g_player.name.c_str());
	DebugText(1000, 140, "RecvCharacterIcon	: %d", Data::g_player.character_icon);
	DebugText(1000, 160, "RecvMusic			: %d", Data::g_player.music);
	DebugText(1000, 180, "RecvMusicLevel	: %d", Data::g_player.music_level);
	DebugText(1000, 200, "RecvMusicLevelNum	: %d", Data::g_player.music_level_num);
	DebugText(1000, 220, "RecvGameMode		: %d", Data::g_player.game_mode);
	DebugText(1000, 240, "RecvScore			: %f", Data::g_player.score);
	DebugText(1000, 260, "RecvOnline		: %d", Data::g_player.online);
	DebugText(1000, 300, "RecvRoomCreate	: %d", Data::g_player.room_create);
	DebugText(1000, 320, "RecvRoomEntry		: %d", Data::g_player.room_entry);
	DebugText(1000, 340, "RecvReady			: %d", Data::g_player.ready);
	DebugText(1000, 360, "RecvMultiPlayCancel	: %d", Data::g_player.multiplay_cancel);
}

void ResetData()
{
	Data::g_player.score = 0.0f;
	Data::g_player.room_create = false;
	Data::g_player.room_entry = false;
	Data::g_player.online = false;
	Data::g_player.ready = false;
	Data::g_player.multiplay_cancel = false;
}