#include "key.h"

int KeyState(int button)
{
	if(button == 0)
	{	
		if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0)
		{
			
			while(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0);
			return 1;
		}
	}
	else if(button == 1)
	{	
		if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0)
		{
			
			while(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0);
			return 1;
		}
	}
	else if(button == 2)
	{
		if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0)
		{
			
			while(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0);
			return 1;
		}
	}
	else if(button == 3)
	{
		if(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==1)
		{
			
			while(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==1);
			return 1;
		}
	}
	return 0;
}




