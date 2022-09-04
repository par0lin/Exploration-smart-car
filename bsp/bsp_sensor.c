#include "bsp_sensor.h"


int Sensor_Get(int num)
{
	int state=-1;
	switch(num)
	{
		case 1:state = HAL_GPIO_ReadPin(SENSOR1_GPIO_Port,SENSOR1_Pin);break;
		case 2:state = HAL_GPIO_ReadPin(SENSOR2_GPIO_Port,SENSOR2_Pin);break;
		default: break;
	}
	
	return state;
}