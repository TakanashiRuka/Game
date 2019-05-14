#ifndef DEFINE_H
#define DEFINE_H

#define FVF_VERTEX_2D					(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX3D					(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX3D_TEX				(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

#define TIME_PER_HOLD					(200)		// ホールド音符を足された頻度(ms)

#define HOLD_MAX_A						(255)		// ホールドαの最大値
#define HOLD_MIN_A						(150)		// ホールドαの最小値

#define HOLD_BLEND_FACTOR_DRAW_NUM		(1)			// ホールドα加算合成の描画回数
#define SLIDE_BLEND_FACTOR_DRAW_NUM		(1)			// スライドα加算合成の描画回数
#define HOLD_BLEND_FACTOR_MAX_A			(127)		// ホールドα加算合成の最大値
#define HOLD_BLEND_FACTOR_MIN_A			(31)		// ホールドα加算合成の最小値

#define N								(100.0f)	// 2D座標から3D座標に変換させる常数

#define MAX_SONG						(100)			// 最大曲数
#define MAX_WORD						(128)			// 文字列の長さ
#define MAX_LEVEL						(3)

#define INIT_SPEED						(3.0f)
#define INIT_OFFSET						(0)
#define INIT_SE_VOLUME					(100)

typedef enum {
	RDP_INIT,
	RDP_END,
	RDP_MAX
}RESULT_DATA_PARTS;

typedef enum {
   AFF_FADEIN,
   AFF_MIDDLE_START,
   AFF_MIDDLE_END,
   AFF_FADEOUT,
   AFF_MAX
}ANIMATION_FADEIN_FADEOUT;

typedef enum {
	CFF_FADEIN,
	CFF_MIDDLE,
	CFF_FADEOUT,
    CFF_MAX
}COUNTER_FADEIN_FADEOUT;

#endif // !DEFINE_H
