#ifndef __MYTASK_H
#define __MYTASK_H
#include "cmsis_os.h"

extern osThreadId ContestTaskHandle;
extern osThreadId LEDFlashHandle;

void _StateUpdate();
void _TestTask();
void _DingjiaoPIDTask();
void _TrackPIDTask();
void _M2006Task();

void _LEDFlash();
void _ContestTask();

#endif
