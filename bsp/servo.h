#ifndef __servo_H
#define __servo_H
#include "gpio.h"

void servo_init(); 
void servo(uint16_t angle,int ID);
void bend(int f);
void head(int f);
void raise(int f);


#endif
