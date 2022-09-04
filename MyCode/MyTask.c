#include "MyTask.h"
#include "Vofa_send.h"
#include "bsp_can.h"
#include "math.h"
#include "adc.h"
#include "key.h"
#include "servo.h"
#include "main.h"
#include "control.h"
#include "tim.h"
#include "pid.h"
#include "section.h"
#include "bsp_flash.h"
#include "remote_control.h"
#include "cmsis_os.h"
#include "task.h"
#include "FreeRTOS.h"


void _ContestTask(void const * argument)
{
  /* USER CODE BEGIN _ContestTask */
	osDelay(300);
	HAL_TIM_Base_Start_IT(&htim7);
  	HAL_TIM_Base_Start_IT(&htim13);
	
	record_read();
	record_show();
//#define TEST_MODE
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef TEST_MODE
	bend(1);
	while(!KeyState(0));
	osDelay(5);
//////TEST_START/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
	Track_tim(5000,0,1);
	pingtai6();
	

//	vTaskSuspend(M2006TaskHandle);
	

	
	
	Stop();

	

	
	
	


	
//////TEST_STOP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
  
///////CONTEST//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TEST_MODE
SKIP.Zhili_1=1;
SKIP.Zhili_2=1;
SKIP.Zhili_3=0;
SKIP.Zhili_4=1;
SKIP.Zhili_5=1;

for(int i=1;i<=2;i++)
{
	p1_to_p2();
	p2_to_p4();
//	p3_to_p4();
	// p2_to_p3();
	p4_to_cross2();
	find_door();
	if(Traffic_pass_record==1||Traffic_pass_record==3)
	{
		if(door_find_fail>0)cross3_to_p8();
		else
		{
			cross3_to_p6();
			p6_to_p8();
		}
		p8_to_p7();
		p7_to_cross3();
//		p7_to_p5();
//		p5_to_cross3();
		cross3_to_cross12();
	}
	if(Traffic_pass_record==4)
	{
		if(door_find_fail>0)cross4_to_p8();
		else
		{
			cross4_to_p6();
			p6_to_p8();
		}
		p8_to_p7();
		p7_to_cross4();	
		cross4_to_cross12();
	}
	if(Traffic_pass_record==2)
	{
		if(door_find_fail>0)cross3_to_p8();
		else
		{
			cross3_to_p6();
			p6_to_p8();
		}
		p8_to_p7();
		p7_to_cross4();	
		cross4_to_cross12();
	}
	cross12_back();

}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for(;;)
  {
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    osDelay(100);
  }
}



void _LEDFlash(void const * argument)
{
  /* USER CODE BEGIN _LEDFlash */
	osDelay(400);
	vTaskSuspend(LEDFlashHandle);
  /* Infinite loop */
  for(;;)
  {
		flash_led();
		vTaskSuspend(LEDFlashHandle);
    osDelay(1);
  }
  /* USER CODE END _LEDFlash */
}


/*****************************电机转速控制pid******************************/
void _M2006Task()
{
	if(track_flag==1||track_flag_bridge==1)w=-track_pid.out;
		
	if(SensorR(1)==0&&stop_flag==0){yundong_cal(0,0,0);}
	else yundong_cal(speed_r,speed_r,w);
	
	distance_r =32.9*(moto_chassis[0].round_cnt+moto_chassis[1].round_cnt-moto_chassis[2].round_cnt-moto_chassis[3].round_cnt)/144.0;
	
	for(int i=0; i<4; i++)
	{
		PID_calc(&motor_pid[i],moto_chassis[i].speed_rpm,motor_pid[i].set);
	}
	
	if(Weak_mode==0)set_moto_current(&hcan1,  	motor_pid[0].out,   //将PID的计算结果通过CAN发送到电机
														motor_pid[1].out,
														motor_pid[2].out,
														motor_pid[3].out);
/****************************************************************************/
else set_moto_current(&hcan1,0,0,0,0);
}





/*****************************传感器状态更新******************************/
void _StateUpdate()
{
	OPENMV[0]=SensorR(4);	OPENMV[1]=SensorR(5);
	Traffic_light=detect_Traffic_light();		
	if(rc_ctrl.rc.ch[3]<-600)Weak_mode=1;
	else Weak_mode=0;
}





/*****************************Yaw定角pid******************************/
void _DingjiaoPIDTask()
{
	if(dingjiao_mode)
	{
		double err=yaw-yaw0;
		if(err > 180)
		{
			err = err-360;
		}
		if(err < -180)
		{
			err = err+360;
		}
		int speed_temp=(moto_chassis[0].speed_rpm+moto_chassis[1].speed_rpm+moto_chassis[2].speed_rpm+moto_chassis[3].speed_rpm)/4.0;
		if(my_abs(err)<=3){
			dingjiao_flag=1;
		}
		dingjiao_pid.set=0;
		//dingjiao_pid.ki=2*(my_abs(err)/180.0);
		PID_calc(&dingjiao_pid,err,0);
		w = dingjiao_pid.out;
	}
	else yaw0=yaw;
}





/*****************************循迹PID******************************/
void _TrackPIDTask()
{
	if(track_flag==1)
	{
		float Speed_real=(moto_chassis[0].speed_rpm+moto_chassis[1].speed_rpm-moto_chassis[2].speed_rpm-moto_chassis[3].speed_rpm)/4.0;
		track_pid.Kp=18.0f*(12000.0f/Speed_real);
		track_pid.Kd=track_pid.Kp/4.0f;
		Tracking(track_mode,speed_r);
		track_pid.set=0;
		PID_calc(&track_pid,ahead_location ,0);
	}

	if(track_flag_bridge==1)
	{
		Track_2(speed_r);
	}
}



void _TestTask()
{
	remote_set_door();
}

