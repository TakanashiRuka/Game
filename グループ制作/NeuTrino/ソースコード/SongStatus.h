//----------------------------------------------------------------------------
//
//	ゲーム用の曲ステータス
//
//														 制作者：劉喆
//                                                       作成日：2018/01/01
//----------------------------------------------------------------------------

#ifndef SONG_STATUS_H
#define SONG_STATUS_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------

void InitSongBoard();
void UninitSongBoard();
void UpdateSongBoard();
void DrawSongBoard();

void InitSongStatusWord();
void UninitSongStatusWord();
void UpdateSongStatusWord();
void DrawSongStatusWord();

void InitGameAlbumBoard();
void UninitGameAlbumBoard();
void UpdateGameAlbumBoard();
void DrawGameAlbumBoard();

void InitGameAlbum();
void UninitGameAlbum();
void UpdateGameAlbum();
void DrawGameAlbum();

void InitGameSongTitle();
void UninitGameSongTitle();
void UpdateGameSongTitle();
void DrawGameSongTitle();

//------------------------------------------------------------------------------
//	関数名:	SetSongTitleGame
//	引数:	タイトル文字列の先頭アドレス
//	戻り値:	なし
//	説明:	曲名を設定する
//------------------------------------------------------------------------------
void SetSongTitleGame(char *cTitle);

void InitGameLevelWord();
void UninitGameLevelWord();
void UpdateGameLevelWord();
void DrawGameLevelWord();

void DrawGameLevelNum();

void InitPlayPosition();
void UninitPlayPosition();
void UpdatePlayPosition();
void DrawPlayPosition();

#endif // !SONG_STATUS_H
