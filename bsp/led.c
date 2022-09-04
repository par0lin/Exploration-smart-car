#include "led.h"

void Led_Flash(int count)
{
	static int i=0;
	i++;
	if(i>=count)
	{
		i=0;
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	}
}



int SensorR(char x)     //ºÚÉ«-->1  »ÆÉ«-->0
{
	int mode;
	switch(x)
	{
		case 1: mode = HAL_GPIO_ReadPin(R1_GPIO_Port,R1_Pin);break;
		case 2: mode = HAL_GPIO_ReadPin(R2_GPIO_Port,R2_Pin);break;
		case 3: mode = HAL_GPIO_ReadPin(R3_GPIO_Port,R3_Pin);break;
		case 4: mode = HAL_GPIO_ReadPin(R4_GPIO_Port,R4_Pin);break;
		case 5: mode = HAL_GPIO_ReadPin(R5_GPIO_Port,R5_Pin);break;
		case 6: mode = HAL_GPIO_ReadPin(R6_GPIO_Port,R6_Pin);break;
		case 7: mode = HAL_GPIO_ReadPin(R7_GPIO_Port,R7_Pin);break;
		case 8: mode = HAL_GPIO_ReadPin(R8_GPIO_Port,R8_Pin);break;
	}
	return mode;
}

