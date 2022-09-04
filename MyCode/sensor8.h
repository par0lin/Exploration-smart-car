#ifndef __SENSOR8_H
#define __SENSOR8_H
#include "main.h"

extern volatile uint8_t USART_RX_STA[];

void Sensor_Init(void);
int Read_Data1(void);
void Read_Data(int *Data);		
void Trig_Sensor(void);

#endif 
