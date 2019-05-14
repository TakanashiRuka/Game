#ifndef GAME_H
#define GAME_H

// プロトタイプ宣言
void InitGame();
void UninitGame();
void UpdateGame();
void DrawGame();

int GetPlayMusicOffset();
void SetSongURL(char *cURL);
bool GetInitGame();

void SetGamePlayMusicMemo(bool bStatus);

#endif