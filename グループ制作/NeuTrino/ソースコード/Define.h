#ifndef DEFINE_H
#define DEFINE_H

#define FVF_VERTEX_2D					(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX3D					(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX3D_TEX				(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

#define TIME_PER_HOLD					(200)		// �z�[���h�����𑫂��ꂽ�p�x(ms)

#define HOLD_MAX_A						(255)		// �z�[���h���̍ő�l
#define HOLD_MIN_A						(150)		// �z�[���h���̍ŏ��l

#define HOLD_BLEND_FACTOR_DRAW_NUM		(1)			// �z�[���h�����Z�����̕`���
#define SLIDE_BLEND_FACTOR_DRAW_NUM		(1)			// �X���C�h�����Z�����̕`���
#define HOLD_BLEND_FACTOR_MAX_A			(127)		// �z�[���h�����Z�����̍ő�l
#define HOLD_BLEND_FACTOR_MIN_A			(31)		// �z�[���h�����Z�����̍ŏ��l

#define N								(100.0f)	// 2D���W����3D���W�ɕϊ�������퐔

#define MAX_SONG						(100)			// �ő�Ȑ�
#define MAX_WORD						(128)			// ������̒���
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
