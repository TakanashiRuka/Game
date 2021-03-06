//----------------------------------------------------------------------------
//
//	曲選択の最終確認
//
//														 制作者：劉��
//                                                       作成日：2018/01/24
//----------------------------------------------------------------------------

#ifndef SELECT_SONG_FINAL_CHECK_H
#define SELECT_SONG_FINAL_CHECK_H

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------
void InitSelectSongFinalCheck();
void UninitSelectSongFinalCheck();
void UpdateSelectSongFinalCheck();
void DrawSelectSongFinalCheck();

//------------------------------------------------------------------------------
//	関数名:	GetSelectSongFinalCheckStatus
//	引数:	なし
//	戻り値:	最終確認の状態
//	説明:	最終確認の状態を取得する
//------------------------------------------------------------------------------
bool GetSelectSongFinalCheckStatus();

#endif // !SELECT_SONG_FINAL_CHECK_H

