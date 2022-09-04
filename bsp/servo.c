#include "servo.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_u8g2.h"
#include "u8g2.h"


void servo_init()
{
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	servo(89,1);
	servo(89,2);
	servo(90,3);
	servo(87,4);
	servo(30,5);
}

void servo(uint16_t angle,int ID)
{
   float temp;
   temp =(1.0 / 9.0) * angle + 5.0;  //占空比值 = 1/9 * 角度 + 5
	if(ID==1)__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, (uint16_t )temp);
	if(ID==2)__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, (uint16_t )temp);
	if(ID==3)__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, (uint16_t )temp);
	if(ID==4)__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, (uint16_t )temp);
	if(ID==5)__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, (uint16_t )temp);
}


void bend(int f)
{
	if(f==0)servo(87,4);
	if(f==1){servo(15,4);raise(0);}
}

void raise(int f)
{
	if(f==0){servo(89,2);servo(90,3);}
	if(f==1){servo(44,3);servo(135,2);}
}

void head(int f)
{
	if(f==0)servo(89,1);
	if(f==-1)servo(45,1);
	if(f==1)servo(135,1);
}