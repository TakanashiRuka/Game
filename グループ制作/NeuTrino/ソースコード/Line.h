//----------------------------------------------------------------------------
//
//	小節ライン
//
//														 制作者：劉��
//                                                       作成日：2017/12/12
//----------------------------------------------------------------------------

#ifndef LINE_H_
#define LINE_H_

//---------------------------インクルード-------------------------------------

//---------------------------マクロ定義---------------------------------------

//---------------------------列挙型定義---------------------------------------

//---------------------------構造体定義---------------------------------------

//---------------------------グローバル変数-----------------------------------

//---------------------------プロトタイプ宣言---------------------------------
void InitLine();
void UninitLine();
void UpdateLine(int nTime);
void DrawLine();

//------------------------------------------------------------------------------
//	関数名:	SetLine
//	引数:	なし
//	戻り値:	なし
//	説明:	小節の情報を設定する
//------------------------------------------------------------------------------
void SetLine();

#endif // !LINE_H

