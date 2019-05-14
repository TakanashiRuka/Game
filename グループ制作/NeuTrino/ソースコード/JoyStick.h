#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Windows.h>
#define  DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

#define JOY_POS_MIN			  (30)
#define JOY_UP                (JOY_POS_MIN + 3)
#define JOY_DOWN              (JOY_POS_MIN + 1)
#define JOY_LEFT              (JOY_POS_MIN)
#define JOY_RIGHT             (JOY_POS_MIN + 2)

#define JOY_MISS              (-JOY_POS_MIN)

#define JOY_NUM_UP                (0)
#define JOY_NUM_DOWN              (18000)
#define JOY_NUM_LEFT              (27000)
#define JOY_NUM_RIGHT             (9000)
#define JOY_NUM_UP_RIGHT          (4500)
#define JOY_NUM_DOWN_RIGHT        (13500)
#define JOY_NUM_DOWN_LEFT         (22500)
#define JOY_NUM_UP_LEFT           (31500)


#define JOY_RECT              (0)
#define JOY_CROSS             (1)
#define JOY_SPHERE            (2)
#define JOY_TRI               (3)
#define JOY_L1                (4)
#define JOY_R1                (5)
#define JOY_L2                (6)
#define JOY_R2                (7)
#define JOY_SHERE             (8)
#define JOY_OPTION            (9)
#define JOY_L3         		  (10)
#define JOY_R3                (11)
#define JOY_PS                (12)
#define JOY_PAD               (13)

typedef enum {
	AS_LEFT,
	AS_RIGHT,
	AS_MAX
}ANALOG_STICK;

HRESULT InitJoyStick(HINSTANCE hInstance, HWND hWnd);
void UninitJoyStick();
void UpdateJoyStick();

bool GetJoyStickPress(int nKey);
bool GetJoyStickTrigger(int nKey);
bool GetJoyStickRepeat(int nKey);
bool GetJoyStickRelease(int nKey);

BOOL CALLBACK EnumJoySticksCallback(const DIDEVICEINSTANCE *pdidinstance, VOID *pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);

void DrawDebugKey();

bool GetPOVPress(int nKey);
bool GetPOVTrigger(int nKey);
bool GetPOVRelease(int nKey);
bool GetPOVRepeat(int nKey);

float GetStickLength(int nNum);
float GetStickAngle(int nNum);
LPDIRECTINPUTDEVICE8 GetJSDev();

#endif
