#ifndef _DATA_H_
#define _DATA_H_

#include<string>

#define PLAYER_NAME ("PLAYER1")

struct Player
{
	std::string name;		//名前
	int character_icon;		//キャラクターのアイコン
	int music;				//曲の番号
	int music_level;		//曲のレベル
	int music_level_num;	//曲のレベル数字
	int game_mode;			//ゲームのモード
	float score;			//スコア
	float display_score;
	bool online;			//ネットワークがオンかオフか
	bool room_create;		//ルームの作成
	bool room_entry;		//ルームに参加
	bool ready;				//ゲームの準備ができているか
	bool init_game;			//ルームに参加ができているか
	bool game_preface;
	bool multiplay_cancel;
};

class Data
{
public:
	static Player g_player;
};

//------------------------------------------------------------------------------
//	関数名:	UpdateSendData
//	引数:	なし
//	戻り値:	なし
//	説明:	自身のアップデートされたデータを送信する
//------------------------------------------------------------------------------
void UpdateSendData();

//------------------------------------------------------------------------------
//	関数名:	DrawRecvDataDebug
//	引数:	なし
//	戻り値:	なし
//	説明:	受信した相手のデータをデバッグ文字で描画する
//------------------------------------------------------------------------------
void DrawRecvDataDebug();

void ResetData();

#endif // !_DATA_H_