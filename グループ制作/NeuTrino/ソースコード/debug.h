#ifndef DEBUG_H
#define DEBUG_H

typedef enum {
	F_EDIT,
	F_SELECT_SONG_TITLE,
	F_SELECT_SONG_SUBTITLE,
	F_SELECT_NOTE_DESIGNER,
	F_GAME_SONG_TITLE,
	F_SETTING,
	F_TEST_MODE,
	F_VERSION,
	F_USER_NAME,
	F_GAME_PLAYER_NAME,
	F_GAME_RIVAL_NAME,
	F_ERROR_LIST,
	F_RANKING_NAME,		//追加
	F_PLAYER_ENTRY_CENTER,
	F_PLAYER_ENTRY_SIDE,
	F_PLAYER_ENTRY_NAME,
	F_MAX
}FONTLIST;

void InitDebugText();
void UninitDebugText();

//------------------------------------------------------------------------------
//	関数名:	SetDebugTextColor
//	引数:	文字の色
//	戻り値:	なし
//	説明:	デバッグテキスト色の変更
//------------------------------------------------------------------------------
void SetDebugTextColor(D3DCOLOR color);
void DebugText(int x, int y, const char* pFormat, ...);

//------------------------------------------------------------------------------
//	関数名:	InitText
//	引数:	フォントナンバー フォントサイズ	フォントスタイル
//	戻り値:	なし
//	説明:	フォントの初期化
//------------------------------------------------------------------------------
void InitText(int nNumFont, int nFontSize, const char* pFormat);
void UninitText(int nNumFont);
void OutputText(int nNumFont, int x, int y, const char* pFormat, ...);

//------------------------------------------------------------------------------
//	関数名:	SetText
//	引数:	フォントナンバー フォントサイズ	フォントスタイル
//	戻り値:	なし
//	説明:	フォントサイズの変更(なるべく使わないで)
//------------------------------------------------------------------------------
void SetText(int nNumFont, int nSize, const char* pFormat);

//------------------------------------------------------------------------------
//	関数名:	SetTextStatus
//	引数:	文字の色　フチの色　フチの太さ(0の時はフチなし)
//	戻り値:	なし
//	説明:	フォント色の変更
//------------------------------------------------------------------------------
void SetTextStatus(D3DCOLOR FontColor, D3DCOLOR FontOutColor, int nSize);

//------------------------------------------------------------------------------
//	関数名:	SetTextStyle
//	引数:	DT_LEFT	など
//	戻り値:	なし
//	説明:	フォントの描画スタイルの変更
//------------------------------------------------------------------------------
void SetTextStyle(int nStyle);

#endif