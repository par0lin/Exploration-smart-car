#include "section.h"
#include "control.h"
#include "led.h"
#include "pid.h"
#include "key.h"
#include "control.h"
#include "bsp_can.h"
#include "JY901.h"
#include "uart_it.h"
#include "tim_it.h"
#include "bsp_sensor.h"
#include "delay.h"
#include "hw.h"
#include "sys.h"
#include "bsp_u8g2.h"
#include "u8g2.h"
#include "sensor8.h"
#include "servo.h"
#include "MyTask.h"

#define WaitForDark while(SensorR(2)==0||SensorR(3)==0)
#define WaitForLight while(SensorR(2)==1||SensorR(3)==1)
#define Key_Pause while(!KeyState(0))Stop()
	
Struct_Sight_Skip SKIP={0};


#define SPEED_MAX 13000
#define SPEED_jiansudai 4000
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*---------------------------NEW-SECTION---------------------------------*/

//////////////////before_the_door/////////////////////
void p1_to_p2()
{
	Servo_5_mode(0);
	ready();
	jinxingban_start();
//	while(!KeyState(0))Stop();
	SpeedUP_track(SPEED_MAX,4000,120,40);//Ramp_dis
	long_bridge(5000);
//	while(!KeyState(0))Stop();
	SpeedUP_track(SPEED_MAX,5000,90,40);
	WaitForDark;
	pingtai(0);
}

void p2_to_p3()
{
	Track_tim(8000,1,0);
	track_turn(8000,-42,0,0);
	SpeedUP_track(6000,3000,50,20);
	jieti_1(3000);
	Track_tim(4000,0,1);
	delay_distance(10);
	track_turn(4000,-135,1,5);
	SpeedUP_track(SPEED_MAX,5000,410,20);
	WaitForDark;
	pingtai(0);
}

void p2_to_p4()
{
	Track_tim(8000,2,0);
	track_turn(8000,-42,0,0);
	SpeedUP_track(6000,3000,50,20);
	jieti_1(3000);
	Track_tim(5000,0,1);
	delay_distance(10);
	track_turn(6000,42,0,5);
	Track_tim(6000,0,1);
	SpeedUP_track(SPEED_MAX,5000,390,20);
	pingtai(0);
}

//////////p3_p4///////////
void p3_to_s01()
{
	SpeedUP_track(SPEED_MAX,8000,250,25);//Ramp_dis
	track_turn(8000,150,0,4);
	Track_tim(5000,2,1);
	delay_distance(30);
	SpeedUP_track(SPEED_MAX,6000,250,25);//Ramp_dis
	zhili();
}
	
void s01_to_p4()
{
	Track_tim(3000,1,1);
	SpeedUP_track(SPEED_MAX,6000,250,25);//Ramp_dis
	Track_tim(6000,1,1);
	WaitForCross();
	delay_distance(30);
	Track_tim(6000,0,1);
	SpeedUP_track(SPEED_MAX,5000,500,25);//Ramp_dis
	pingtai(0);
}
//////////p3_p4///////////
//merge p3_p4
void p3_to_p4()
{
	if(SKIP.Zhili_1==0)
	{
		p3_to_s01();
		s01_to_p4();
	}
	else
	{
		SpeedUP_track(SPEED_MAX,5000,820,20);
		pingtai(0);
	}
}

//////////p4_cross2///////////
void p4_to_s02()
{
	SpeedUP_track(SPEED_MAX,6000,70,25);//Ramp_dis
	track_turn(6000,-135,0,0);
	SpeedUP_track(SPEED_MAX,6000,140,25);//Ramp_dis
	zhili();
}

void s02_to_cross2()
{
	SpeedUP_track(SPEED_MAX,6000,120,25);//Ramp_dis
	Track_tim(6000,2,1);
	WaitForCross();
	delay_distance(30);
	Track_tim(8000,0,1);
}
//////////p4_cross2///////////		
//merge p4_cross2
void p4_to_cross2()
{
	if(SKIP.Zhili_2==0)
	{
		p4_to_s02();
		s02_to_cross2();
	}
	else
	{
		SpeedUP_track(SPEED_MAX,5000,220,30);
	}
}


//////////////////door1 & door3/////////////////////
void cross3_to_p6()
{
	Track_tim(7000,0,1);
	SpeedUP_track(SPEED_MAX,7000,120,50);//Ramp_dis
	pingtai6();
}

void cross3_to_p8()
{
	Track_tim(7000,0,1);
	SpeedUP_track(SPEED_MAX,7000,250,40);//Ramp_dis
	
	Track_tim(6000,0,1);
	track_turn(6000,90,0,0);
	Track_tim(7000,0,1);
	SpeedUP_track(SPEED_MAX,7000,250,40);//Ramp_dis
	track_turn(7000,90,0,-5);
	SpeedUP_track(SPEED_MAX,7000,95,40);//Ramp_dis
	track_turn(7000,90,0,0);
	SpeedUP_track(SPEED_MAX,7000,130,40);//Ramp_dis
	track_turn(7000,-120,1,0);
	SpeedUP_track(SPEED_MAX,7000,170,40);
	track_turn(7000,-150,0,6);
	Track_tim(7000,1,1);
	delay_distance(20);
	Track_tim(8000,0,1);
	pingtai8(0);
}
//////////p6_p8///////////
void p6_to_s03()
{
	Track_tim(6000,0,1);
	track_turn(6000,90,0,0);
	zhili_2(20,90);
}

void s03_to_p8()
{
	Track_tim(7000,0,1);
	SpeedUP_track(SPEED_MAX,7000,190,40);//Ramp_dis
	track_turn(7000,90,0,-5);
	SpeedUP_track(SPEED_MAX,7000,95,40);//Ramp_dis
	track_turn(7000,90,0,0);
	SpeedUP_track(SPEED_MAX,7000,130,40);//Ramp_dis
	track_turn(7000,-120,1,0);
	SpeedUP_track(SPEED_MAX,7000,170,40);
	track_turn(7000,-150,0,6);
	Track_tim(7000,1,1);
	delay_distance(20);
	Track_tim(8000,0,1);
	pingtai8(0);
}
//////////p6_p8///////////		
//merge p6_p8
void p6_to_p8()
{
	if(SKIP.Zhili_3==0)
	{
		p6_to_s03();
		s03_to_p8();
	}
	else
	{
		Track_tim(7000,0,1);
		track_turn(7000,90,0,0);
		Track_tim(7000,0,1);
		SpeedUP_track(SPEED_MAX,7000,250,30);//Ramp_dis
		track_turn(7000,90,0,-5);
		SpeedUP_track(SPEED_MAX,7000,95,30);//Ramp_dis
		track_turn(7000,90,0,-3);
		SpeedUP_track(SPEED_MAX,7000,130,30);//Ramp_dis
		track_turn(7000,-120,1,0);
		SpeedUP_track(SPEED_MAX,7000,170,30);
		track_turn(7000,-150,0,6);
		Track_tim(7000,1,1);
		delay_distance(20);
		Track_tim(8000,0,1);
		pingtai8(0);
	}
}
	
void p8_to_p7()
{
	Track_tim(8000,0,1);
	SpeedUP_track(SPEED_MAX,4000,120,50);//Ramp_dis
	Track_tim(4000,0,1);
	jieti_1(3000);
	Track_tim(4000,0,1);
	SpeedUP_track(SPEED_MAX,4000,110,30);//Ramp_dis
	jiansudai_double(SPEED_jiansudai);
	Track_tim(6000,0,1);
	delay_distance(25);
	pingtai7(0);
}

void p7_to_p5()
{
	Track_tim(6000,0,1);
	jiansudai_double(SPEED_jiansudai);
	Track_tim(6000,0,1);
	track_turn(6000,90,1,0);
	track_turn(6000,90,0,-5);
	SpeedUP_track(SPEED_MAX,4000,100,40);//Ramp_dis
	jieti_1(3500);
	Track_tim(4000,0,1);
	SpeedUP_track(SPEED_MAX,6000,110,30);//Ramp_dis
	track_turn(6000,90,0,-5);
	SpeedUP_track(SPEED_MAX,6000,130,30);//Ramp_dis
	track_turn(6000,90,0,0);
	SpeedUP_track(SPEED_MAX,6000,100,40);//Ramp_dis
	Track_tim(6000,0,1);
	jieti_1(4000);
	SpeedUP_track(SPEED_MAX,8000,80,30);//Ramp_dis
//////////////////////////////////////
	track_turn(8000,-150,0,8);
	Track_tim(8000,1,1);
	SpeedUP_track(SPEED_MAX,8000,150,25);//Ramp_dis

	Track_tim(8000,0,1);
	track_turn(8000,-80,0,0);
	SpeedUP_track(SPEED_MAX,5000,120,25);//Ramp_dis
	pingtai(0);
}

void p7_to_cross3()
{
	Track_tim(5000,0,1);
	jiansudai_double(SPEED_jiansudai);
	Track_tim(6000,0,1);
	track_turn(6000,90,1,0);
	track_turn(6000,90,0,-2);
	SpeedUP_track(SPEED_MAX,4000,100,40);//Ramp_dis
	jieti_1(3500);
	Track_tim(4000,0,1);
	SpeedUP_track(SPEED_MAX,6000,110,40);//Ramp_dis
	track_turn(6000,90,0,-5);
	SpeedUP_track(SPEED_MAX,6000,130,40);//Ramp_dis
	track_turn(6000,80,0,0);
	SpeedUP_track(SPEED_MAX,6000,100,40);//Ramp_dis
	Track_tim(6000,0,1);
	jieti_1(4000);
	SpeedUP_track(SPEED_MAX,8000,380,40);//Ramp_dis
	track_turn(8000,-90,0,-2);
	SpeedUP_track(SPEED_MAX,5000,90,40);//Ramp_dis
}

//////////p5_cross3///////////
void p5_to_s05()
{
	Track_tim(8000,0,1);
	SpeedUP_track(SPEED_MAX,8000,230,25);//Ramp_dis
	track_turn(8000,90,0,0);
	zhili_2(20,-90);
}

void s05_to_s04()
{
	track_turn(6000,-90,0,0);
	SpeedUP_track(SPEED_MAX,8000,270,25);//Ramp_dis
	track_turn(8000,-90,1,-3);
	zhili_2(20,90);
	Track_tim(6000,0,1);
}

void s04_to_cross3()
{
	Track_tim(6000,0,1);
}
//////////p5_cross3///////////		
//merge p5_cross3
void p5_to_cross3()
{
	if(SKIP.Zhili_5==1&&SKIP.Zhili_4==1)
	{
		Track_tim(8000,0,1);
		SpeedUP_track(SPEED_MAX,5000,280,30);//Ramp_dis
		jiansudai_double(SPEED_jiansudai);
		Track_tim(6000,0,1);
		if(Traffic_pass_record==1)
		{
			track_turn(8000,-90,0,5);
		}
		if(Traffic_pass_record==3)
		{
			track_turn(8000,-140,0,5);
		}
	}
	
	if(SKIP.Zhili_5==0&&SKIP.Zhili_4==0)
	{
		p5_to_s05();
		s05_to_s04();
		s04_to_cross3();
	}
	if(SKIP.Zhili_5==1&&SKIP.Zhili_4==0)
	{
		Track_tim(8000,0,1);
		SpeedUP_track(SPEED_MAX,8000,230,25);//Ramp_dis
		track_turn(8000,90,0,0);
		SpeedUP_track(SPEED_MAX,6000,100,20);
		track_turn(6000,-90,0,0);
		SpeedUP_track(SPEED_MAX,8000,270,25);//Ramp_dis
		track_turn(8000,-90,1,-3);
		zhili_2(20,90);
		s04_to_cross3();
	}
	if(SKIP.Zhili_5==0&&SKIP.Zhili_4==1)
	{
		p5_to_s05();
		track_turn(6000,-90,0,0);
		SpeedUP_track(SPEED_MAX,8000,270,25);//Ramp_dis
		track_turn(8000,-90,1,-3);
		SpeedUP_track(SPEED_MAX,6000,100,20);
		Track_tim(6000,0,1);
	}
}



void cross3_to_cross12()
{
	if(Traffic_pass_record==1)
	{
//		if(SKIP.Zhili_5==1&&SKIP.Zhili_4==1)Track_tim(6000,0,1);
//		else skip_cross(20);
		skip_cross(20);
		SpeedUP_track(SPEED_MAX,5000,200,40);//Ramp_dis
		track_turn(6000,-90,0,0);
	}
	
	if(Traffic_pass_record==3)
	{
//		if(SKIP.Zhili_5==1&&SKIP.Zhili_4==1)Track_tim(6000,0,1);
//		else track_turn(6000,-50,0,0);
		track_turn(6000,-50,0,0);
		SpeedUP_track(SPEED_MAX,5000,350,40);//Ramp_dis
		track_turn(5000,135,0,0);
	}
}


//////////////////door2 & door4/////////////////////
void cross4_to_p5()
{
	SpeedUP_track(SPEED_MAX,5000,230,30);//Ramp_dis
	pingtai(0);
}

void cross4_to_p6()
{
	if(Traffic_pass_record==2)track_turn(6000,-140,0,5);
	if(Traffic_pass_record==4)track_turn(6000,-90,0,5);
	Track_tim(6000,1,1);
	delay_distance(15);
	Track_tim(6000,0,1);
	SpeedUP_track(10000,5000,60,29);//Ramp_dis
	jiansudai_double(SPEED_jiansudai);
	SpeedUP_track(SPEED_MAX,5000,160,50);//Ramp_dis
	pingtai6();
	Track_tim(4000,0,1);
}

void cross4_to_p8()
{
	if(Traffic_pass_record==2)track_turn(6000,-140,0,5);
	if(Traffic_pass_record==4)track_turn(6000,-90,0,5);
	Track_tim(6000,1,1);
	delay_distance(15);
	Track_tim(6000,0,1);
	SpeedUP_track(8000,5000,50,24);//Ramp_dis
	jiansudai_double(SPEED_jiansudai);
	SpeedUP_track(SPEED_MAX,5000,320,50);//Ramp_dis

	Track_tim(6000,0,1);
	track_turn(6000,90,0,0);
	Track_tim(7000,0,1);
	SpeedUP_track(SPEED_MAX,7000,250,40);//Ramp_dis
	track_turn(7000,90,0,-5);
	SpeedUP_track(SPEED_MAX,7000,95,40);//Ramp_dis
	track_turn(7000,90,0,0);
	SpeedUP_track(SPEED_MAX,7000,130,40);//Ramp_dis
	track_turn(7000,-120,1,0);
	SpeedUP_track(SPEED_MAX,7000,170,40);
	track_turn(7000,-150,0,6);
	Track_tim(7000,1,1);
	delay_distance(20);
	Track_tim(8000,0,1);
	pingtai8(0);
}
//////////p5_p6///////////
void p5_to_s05();

void s05_to_s04();

void s04_to_p6()
{
	track_turn(6000,90,0,0);
	SpeedUP_track(SPEED_MAX,6000,130,20);
	pingtai6();
	Track_tim(4000,0,1);
}
//////////p5_p6///////////

//merge p5_p6
void p5_to_p6()
{
	if(SKIP.Zhili_5==1&&SKIP.Zhili_4==1)
	{
		Track_tim(8000,0,1);
		SpeedUP_track(SPEED_MAX,5000,280,30);//Ramp_dis
		jiansudai_double(SPEED_jiansudai);
		Track_tim(6000,0,1);
		SpeedUP_track(SPEED_MAX,6000,160,30);//Ramp_dis
		pingtai6();
		Track_tim(4000,0,1);
	}
	
	if(SKIP.Zhili_5==0&&SKIP.Zhili_4==0)
	{
		p5_to_s05();
		s05_to_s04();
		s04_to_p6();
	}
	if(SKIP.Zhili_5==1&&SKIP.Zhili_4==0)
	{
		Track_tim(8000,0,1);
		SpeedUP_track(SPEED_MAX,8000,230,25);//Ramp_dis
		track_turn(8000,90,0,0);
		SpeedUP_track(SPEED_MAX,6000,100,20);
		track_turn(6000,-90,0,0);
		SpeedUP_track(SPEED_MAX,8000,270,25);//Ramp_dis
		track_turn(8000,-90,1,-3);
		zhili_2(20,90);
		s04_to_p6();
	}
	if(SKIP.Zhili_5==0&&SKIP.Zhili_4==1)
	{
		p5_to_s05();
		track_turn(6000,-90,0,0);
		SpeedUP_track(SPEED_MAX,8000,270,25);//Ramp_dis
		track_turn(8000,-90,1,-3);
		SpeedUP_track(SPEED_MAX,6000,100,20);
		track_turn(6000,90,0,0);
		SpeedUP_track(SPEED_MAX,8000,130,20);
		pingtai6();
		Track_tim(4000,0,1);
	}
}

//////////p6_p8///////////
void p6_to_s03();

void s03_to_p8();
//////////p6_p8///////////
//merge p6_p8
void p6_to_p8();

void p8_to_p7();

void p7_to_cross4()
{
	Track_tim(6000,0,1);
	jiansudai_double(SPEED_jiansudai);
	Track_tim(6000,0,1);
	track_turn(6000,90,1,0);
	track_turn(6000,90,0,-2);
	SpeedUP_track(SPEED_MAX,4000,100,40);//Ramp_dis
	jieti_1(3500);
	Track_tim(4000,0,1);
	SpeedUP_track(SPEED_MAX,6000,110,40);//Ramp_dis
	track_turn(6000,90,0,-5);
	SpeedUP_track(SPEED_MAX,6000,130,40);//Ramp_dis
	track_turn(6000,80,0,0);
	SpeedUP_track(SPEED_MAX,6000,100,40);//Ramp_dis
	Track_tim(6000,0,1);
	jieti_1(4000);
	SpeedUP_track(SPEED_MAX,8000,80,40);//Ramp_dis
///////////////////////////////////
	track_turn(8000,-90,0,8);
	delay_distance(80);
}

void cross4_to_cross12()
{
	if(Traffic_pass_record==4)
	{
		skip_cross(20);
		SpeedUP_track(SPEED_MAX,8000,210,40);//Ramp_dis
		track_turn(8000,90,0,7);
	}
	
	if(Traffic_pass_record==2)
	{
		track_turn(8000,50,0,7);
		SpeedUP_track(SPEED_MAX,8000,350,40);//Ramp_dis
		track_turn(8000,-140,0,0);
	}
}

	
///////////////////////////BACK////////////////////////////////////////////

void cross12_back()
{
	if(Traffic_pass_record==1||Traffic_pass_record==2)//BACK//door1 & door2//
	{
		SpeedUP_track(SPEED_MAX,6000,110,40);
		track_turn(6000,45,0,5);
		SpeedUP_track(8000,4000,45,20);
		jiansudai(SPEED_jiansudai);
		SpeedUP_track(7000,4000,40,20);
		track_turn(5000,-45,0,8);
		Track_tim(5000,0,1);
		backhome();
	}
	
	if(Traffic_pass_record==3||Traffic_pass_record==4)//BACK//door3 & door4//
	{
		SpeedUP_track(SPEED_MAX,6000,110,40);
		track_turn(6000,-132,0,5);
		SpeedUP_track(8000,4000,45,20);
		jiansudai(SPEED_jiansudai);
		SpeedUP_track(7000,4000,40,20);
		track_turn(5000,-45,0,8);
		Track_tim(5000,0,1);
		backhome();
	}
}


/////////////////////find//door////////////////////////////
void find_door()
{
	door_find_fail=0;
	int wait_time=1000;
	if(Traffic_pass_record_rc!=0)Traffic_pass_record=Traffic_pass_record_rc;
	if(Traffic_pass_record==1)
	{
		skip_cross(20);
		SpeedUP_track(SPEED_MAX,8000,260,30);//Ramp_dis
		track_turn(8000,90,0,5);
		SpeedUP_track(SPEED_MAX,5000,130,40);//Ramp_dis
		WaitForCross();
		Stop();
		int i;
		for(i=0;i<wait_time&&!Traffic_pass();i++)
		{
			osDelay(1);
		}
		if(i==wait_time) goto Door1_close;//Door1 is closed
		record_to_flash(1);
		Track_tim(8000,0,1);
		SpeedUP_track(10000,8000,60,28);//Ramp_dis
		track_turn(8000,-90,0,0);
	}
	
	if(Traffic_pass_record==2)
	{
		skip_cross(20);
		SpeedUP_track(SPEED_MAX,6000,260,35);//Ramp_dis
		track_turn(6000,135,0,0);
		SpeedUP_track(SPEED_MAX,5000,110,30);//Ramp_dis
		WaitForCross();
		Stop();
		int i;
		for(i=0;i<wait_time&&!Traffic_pass();i++)
		{
			osDelay(1);
		}
		if(i==wait_time) goto Door2_close;//Door2 is closed
		record_to_flash(2);
		Track_tim(6000,0,1);
		delay_distance(8);
		track_turn(6000,-102,0,0);
		SpeedUP_track(SPEED_MAX,8000,160,30);//Ramp_dis
		track_turn(8000,-40,0,5);
	}
	
	if(Traffic_pass_record==3)
	{
		track_turn(6000,45,0,0);
		SpeedUP_track(SPEED_MAX,6000,110,40);//Ramp_dis
		WaitForCross();
		Stop();
		int i;
		for(i=0;i<wait_time&&!Traffic_pass();i++)
		{
			osDelay(1);
		}
		if(i==wait_time) goto Door3_close;//Door3 is closed
		record_to_flash(3);
		Track_tim(8000,0,1);
		SpeedUP_track(SPEED_MAX,8000,220,30);//Ramp_dis
		track_turn(8000,-40,0,5);
	}
	
	if(Traffic_pass_record==4)
	{
		track_turn(8000,90,0,0);
		SpeedUP_track(SPEED_MAX,5000,130,40);//Ramp_dis
		WaitForCross();
		Stop();
		int i;
		for(i=0;i<wait_time&&!Traffic_pass();i++)
		{
			osDelay(1);
		}
		if(i==wait_time) goto Door4_close;//Door4 is closed
		record_to_flash(4);
		Track_tim(8000,0,1);
		SpeedUP_track(10000,6000,60,28);
	}

	
	if(Traffic_pass_record==0)
	{
		track_turn(6000,40,0,0);
		find_start:SpeedUP_track(SPEED_MAX,5000,110,40);//Ramp_dis
		WaitForCross();
		Stop();
		int i;
		for(i=0;i<wait_time&&!Traffic_pass();i++)
		{
			osDelay(1);
		}
		if(i==wait_time)//Door3 is closed
		{
			Door3_close:Turn_180(yaw);
			door_find_fail++;
			Track_tim(6000,0,1);
			SpeedUP_track(SPEED_MAX-2000,6000,90,40);//Ramp_dis
			track_turn(6000,-130,0,6);
			SpeedUP_track(SPEED_MAX-2000,5000,130,40);//Ramp_dis
			WaitForCross();
			Stop();
			int i;
			for(i=0;i<wait_time&&!Traffic_pass();i++)
			{
				osDelay(1);
			}
			if(i==wait_time)//Door4 is closed
			{
				Door4_close:Turn_180(yaw);
				door_find_fail++;
				Track_tim(6000,0,1);
				SpeedUP_track(SPEED_MAX-2000,6000,110,40);//Ramp_dis
				track_turn(6000,90,0,8);
				SpeedUP_track(SPEED_MAX-2000,6000,270,40);//Ramp_dis
				track_turn(6000,90,0,6);
				SpeedUP_track(SPEED_MAX-2000,5000,130,40);//Ramp_dis
				WaitForCross();
				Stop();
				int i;
				for(i=0;i<wait_time&&!Traffic_pass();i++)
				{
					osDelay(1);
				}
				if(i==wait_time)//Door1 is closed
				{
					Door1_close:Turn_180(yaw);
					door_find_fail++;
					Track_tim(6000,0,1);
					SpeedUP_track(SPEED_MAX-2000,6000,110,40);//Ramp_dis
					track_turn(6000,-135,0,0);
					SpeedUP_track(SPEED_MAX-2000,5000,110,40);//Ramp_dis
					WaitForCross();
					Stop();
					int i;
					for(i=0;i<wait_time&&!Traffic_pass();i++)
					{
						osDelay(1);
					}
					if(i==wait_time)//Door2 is closed
					{
						Door2_close:Turn_180(yaw);
						door_find_fail++;
						SpeedUP_track(SPEED_MAX-2000,6000,90,30);//Ramp_dis
						track_turn(6000,-135,0,0);
						SpeedUP_track(SPEED_MAX-2000,6000,270,30);//Ramp_dis
						track_turn(6000,-135,0,0);
						goto find_start;
					}
					else //Door2 is open
					{
						record_to_flash(2);
						Track_tim(6000,0,1);
						delay_distance(8);
						track_turn(6000,-102,0,0);
						SpeedUP_track(SPEED_MAX-2000,6000,160,30);//Ramp_dis
						track_turn(6000,-40,0,5);
						return;
					}
				}
				else //Door1 is open
				{
					record_to_flash(1);
					Track_tim(6000,0,1);
					SpeedUP_track(10000,6000,60,28);
					track_turn(6000,-90,0,0);
					return;
				}
			}
			else //Door4 is open
			{
				record_to_flash(4);
				Track_tim(6000,0,1);
				SpeedUP_track(10000,6000,60,28);
				return;
			}
		}
		else //Door3 is open
		{
			record_to_flash(3);
			Track_tim(6000,0,1);
			SpeedUP_track(SPEED_MAX-2000,6000,220,30);
			track_turn(6000,-45,0,5);
			return;
		}
	}
}

///////////////////////////////////Contest//////////////////////////////////////////


/*---------------------------------------------------------*/
void jiansudai(int speed)
{
//	WaitForDark;
////	Track_tim(speed,0,1);
//	
//	dingjiao_run(yaw,speed,1);
//	Servo_5_mode(2);
//	delay_distance(70);
////	Servo_5_mode(2);
//	WaitForLight;

//	Servo_5_mode(1);
//	osDelay(100);
//	Stop();
//	Track_tim(speed,0,1);
//	delay_distance(10);
//	Servo_5_mode(0);
//	delay_distance(15);
		
	WaitForDark;
	Track_tim(speed,0,1);
	delay_distance(90);
	WaitForLight;
}
void jiansudai_double(int speed)
{
//	WaitForDark;
//	Stop();
//	Servo_5_mode(2);
////	Track_tim(speed,0,1);
//	
//	dingjiao_run(yaw,speed,1);
//	
//	delay_distance(150);
////	Servo_5_mode(2);
//	WaitForLight;

//	Servo_5_mode(1);
//	osDelay(100);
//	Stop();
//	Track_tim(speed,0,1);
//	delay_distance(10);
//	Servo_5_mode(0);
//	delay_distance(15);
	
	WaitForDark;
	Track_tim(speed,0,1);
	delay_distance(160);
	WaitForLight;
}

void jieti_1(int speed)
{
	WaitForDark;
	delay_distance(10);
	Servo_5_mode(1);
	Track_tim(speed,0,1);
	delay_distance(95);
	WaitForLight;
	Servo_5_mode(0);
}

void jieti_2()
{
	WaitForDark;
	float temp_yaw=yaw;
	Track_tim(3000,0,1);
	delay_distance(45);
	servo(0,5);
	dingjiao_run(temp_yaw,3000,1);
	osDelay(2000);
	//while(1)Stop();
	servo(30,5);
	WaitForLight;
	dingjiao_run(temp_yaw,3000,0);
}

void qiaoban(int dis)
{
	while(SensorR(2)==0&&SensorR(3)==0);
	delay_distance(12);
//	while(1)Stop();
//	delay_distance(2);
	track_flag=0;
//	Servo_5_mode(2);
	
	track_flag_bridge=1;
	speed_r=4000;
	delay_distance(10-4);
	delay_distance(50+dis);
	Ramp_dis(4000,500,10);
	Ramp_dis(500,1000,2);
//	while(1)Stop();
//	WaitForLight;
	Servo_5_mode(0);
	track_flag_bridge=0;
	Track_tim(4000,0,1);
	delay_distance(10);
	Track_tim(4000,0,1);
}

void ready()
{
	while(SensorR(1)==1)u8g2_Show(&u8g2,"WAIT");
	u8g2_Show(&u8g2,"READY");
//////	raise(1);
//////	HAL_GPIO_TogglePin(R8_GPIO_Port,R8_Pin);
//////	osDelay(400);
////////	head(-1);
//////	raise(0);
//////	HAL_GPIO_TogglePin(R8_GPIO_Port,R8_Pin);
//////	osDelay(400);
////////	head(1);
//////	raise(1);
//////	HAL_GPIO_TogglePin(R8_GPIO_Port,R8_Pin);
//////	osDelay(400);
////////	head(0);
//////	HAL_GPIO_TogglePin(R8_GPIO_Port,R8_Pin);
}

void jinxingban_start()
{	
	stop_flag=1;
	while(SensorR(1)==0)u8g2_Show(&u8g2,"BLOCK");
	
	
	raise(1);
	HAL_GPIO_TogglePin(R8_GPIO_Port,R8_Pin);
	osDelay(200);
//	head(-1);
	raise(0);
	HAL_GPIO_TogglePin(R8_GPIO_Port,R8_Pin);
	osDelay(200);
//	head(1);
	raise(1);
	HAL_GPIO_TogglePin(R8_GPIO_Port,R8_Pin);
	osDelay(200);
//	head(0);
	HAL_GPIO_TogglePin(R8_GPIO_Port,R8_Pin);
	
	
	u8g2_Show(&u8g2,"RUN");
	raise(0);
	dingjiao_run(yaw,0,1);
	for(int i=0;i<=10;i++)
	{
		if(SensorR(1)==0){Stop();i=0;u8g2_Show(&u8g2,"BLOCK");}
		else
		{
			speed_r=i*400;
			osDelay(80);
		}
	}
	dingjiao_run(yaw,4000,0);
	Track_tim(4000,0,1);
//	WaitForLight
//	{
//		if(SensorR(1)==0){Stop();u8g2_Show(&u8g2,"BLOCK");}
//		else Track_tim(4000,0,1);
//	}
	delay_distance(30);
//	Track_tim(4000,0,1);
	bend(1);
	stop_flag=0;
}


void long_bridge(int speed)
{
	WaitForDark;
	track_flag=0;
	track_flag_bridge=1;
	speed_r=4000;
	Servo_5_mode(1);
	delay_distance(75);
	Servo_5_mode(0);
	speed_r=speed;
	delay_distance(90);
	Servo_5_mode(1);
	speed_r=3000;
	Ramp_dis(3000,2000,45);
	Servo_5_mode(0);
	track_flag_bridge=0;
}


void pingtai(int th)
{
	WaitForDark;
	WaitForDark;
	stop_flag=1;
	Anti_lose=0.01f;
	Track_tim(5000,0,1);
	delay_distance(20);
	Servo_5_mode(1);
	WaitForLight;
	Track_tim(0,0,0);
	float Temp_yaw=yaw;
	dingjiao_run(Temp_yaw,4000,1);
	delay_distance(40);
	Servo_5_mode(0);
	while(SensorR(1)==1);
	delay_distance(12);
	dingjiao_run(Temp_yaw,4000,0);
	Stop();
//	while(!KeyState(0))Stop();

	vTaskResume(LEDFlashHandle);
	{
		dingjiao_run(Temp_yaw,-2000,1);
		delay_distance(-8);
		dingjiao_run(Temp_yaw,-2000,0);
	}
	
//	dingjiao(th+180,0);
	Turn_180(Temp_yaw);
//	Track_tim(10,0,1);
//	
	dingjiao_run(yaw,4000,1);
	
	delay_distance(20);
	dingjiao_run(yaw,4000,0);
//	while(!KeyState(0))Stop();
	
	Track_tim(5000,0,1);
	delay_distance(30);
//	while(!KeyState(0))Stop();
	stop_flag=0;
//	while(!KeyState(0))Stop();
}

void pingtai7(int th)
{
	WaitForDark;
	WaitForDark;
	stop_flag=1;
	Anti_lose=0.01f;
	Track_tim(4000,0,1);
	delay_distance(40);
	float Temp_yaw11=yaw;
	delay_distance(10);
	float Temp_yaw22=yaw;
	delay_distance(10);
	float Temp_yaw33=yaw;
	float Temp_yaw_2=(Temp_yaw11+Temp_yaw22+Temp_yaw33)/3.0;
	Track_tim(3000,0,1);
	Servo_5_mode(1);
	WaitForLight;
	Track_tim(3000,0,0);
	stop_flag=1;
	dingjiao_run(Temp_yaw_2,3000,1);
	delay_distance(30);
	Servo_5_mode(0);
	while(SensorR(1)==1);
	delay_distance(12);
	dingjiao_run(Temp_yaw_2,3000,0);
	Stop();

//	dingju(-100,th);
	vTaskResume(LEDFlashHandle);
	{
		dingjiao_run(Temp_yaw_2,-2000,1);
		delay_distance(-4);
		dingjiao_run(Temp_yaw_2,-2000,0);
	}
//	dingjiao(th+180,0);

	float Kp=dingjiao_pid.Kp;
	float Ki=dingjiao_pid.Ki;
	float Kd=dingjiao_pid.Kd;
	PID_set(&dingjiao_pid,30,1,20);
	Turn_180(Temp_yaw_2);
	PID_set(&dingjiao_pid,Kp,Ki,Kd);
	
//	Track_tim(10,0,1);
//	
	dingjiao_run(yaw,3000,1);
	Servo_5_mode(1);
	delay_distance(15);
//	while(!KeyState(0))Stop();
	Track_tim(3000,0,1);
	delay_distance(20);
	Ramp_dis(3000,6000,15);
	Track_tim(6000,0,1);
	delay_distance(25);
//	while(!KeyState(0))Stop();
	Servo_5_mode(0);
	Anti_lose=0.0f;
	WaitForLight;
	Servo_5_mode(2);
	delay_distance(10);
	Servo_5_mode(0);
//	while(!KeyState(0))Stop();
	stop_flag=0;
}


void pingtai8(int th)
{
	WaitForDark;
	WaitForDark;
	stop_flag=1;
	Anti_lose=0.01f;
	Track_tim(5000,0,1);
	delay_distance(60);
	float Temp_yaw1=yaw;
	delay_distance(10);
	float Temp_yaw2=yaw;
	delay_distance(10);
	float Temp_yaw3=yaw;
	float Temp_yaw=(Temp_yaw1+Temp_yaw2+Temp_yaw3)/3.0;
//	while(!KeyState(0))Stop();
	Track_tim(4000,0,1);
	Servo_5_mode(1);
	WaitForLight;
	Track_tim(4000,0,0);
	
	dingjiao_run(Temp_yaw,3000,1);
	delay_distance(20);
	Servo_5_mode(0);
//	while(!KeyState(0))Stop();
	stop_flag=1;
//	while(1)Stop();
	dingjiao_run(Temp_yaw,3000,1);
	delay_distance(25);
//	while(!KeyState(0))Stop();
	WaitForLight;
	dingjiao_run(Temp_yaw,0,0);
	if(th!=0)
	{
		Stop();
		osDelay(100);
		dingjiao(Temp_yaw+th,0);
	}
	dingjiao_run(Temp_yaw,3000,1);
	delay_distance(13);
//	while(!KeyState(0))Stop();
	dingjiao_run(Temp_yaw,3000,0);
//	while(1)Stop();
	
	
	
	
	
	Track_tim(4000,0,1);
	delay_distance(40);
	float Temp_yaw11=yaw;
	delay_distance(10);
	float Temp_yaw22=yaw;
	delay_distance(10);
	float Temp_yaw33=yaw;
	float Temp_yaw_2=(Temp_yaw11+Temp_yaw22+Temp_yaw33)/3.0;
	Track_tim(3000,0,1);
	Servo_5_mode(1);
	WaitForLight;
	Track_tim(3000,0,0);
	stop_flag=1;
	dingjiao_run(Temp_yaw_2,3000,1);
	delay_distance(30);
	Servo_5_mode(0);
	while(SensorR(1)==1);
	delay_distance(12);
	dingjiao_run(Temp_yaw_2,3000,0);
	Stop();

//	dingju(-100,th);
	vTaskResume(LEDFlashHandle);
	{
		dingjiao_run(Temp_yaw_2,-2000,1);
		delay_distance(-4);
		dingjiao_run(Temp_yaw_2,-2000,0);
	}
//	dingjiao(th+180,0);

	float Kp=dingjiao_pid.Kp;
	float Ki=dingjiao_pid.Ki;
	float Kd=dingjiao_pid.Kd;

	PID_set(&dingjiao_pid,30,1,20);
	Turn_180(Temp_yaw_2);
	PID_set(&dingjiao_pid,Kp,Ki,Kd);
	
	dingjiao_run(yaw,3000,1);
	Servo_5_mode(1);
	delay_distance(17);
	
	WaitForDark;
	Track_tim(3000,0,1);
	delay_distance(55);
	float Temp_yaw111=yaw;
	delay_distance(10);
	float Temp_yaw222=yaw;
	delay_distance(10);
	float Temp_yaw333=yaw;
	float Temp_yaw_3=(Temp_yaw111+Temp_yaw222+Temp_yaw333)/3.0f;
	Stop();
//	while(!KeyState(0))Stop();
//	WaitForLight;

//	while(!KeyState(0))Stop();
//	osDelay(10);





//	osDelay(80);
	dingjiao_run(Temp_yaw_3,3000,1);
	delay_distance(48);
//	while(!KeyState(0))Stop();
	WaitForLight;

	Servo_5_mode(1);
	delay_distance(15);
	dingjiao_run(Temp_yaw_3,3000,0);
//	while(!KeyState(0))Stop();
	Track_tim(3000,0,1);
	delay_distance(20);
	Ramp_dis(3000,8000,15);
	Track_tim(8000,0,1);
	delay_distance(25);
//	while(!KeyState(0))Stop();
	Servo_5_mode(0);
	WaitForLight;
	Anti_lose=0.0f;
	delay_distance(15);
//	while(!KeyState(0))Stop();
	stop_flag=0;
}


void pingtai6()
{
	skip_cross(55);
//	while(!KeyState(0))Stop();
	Track_tim(6000,2,1);
	delay_distance(50);
//	while(!KeyState(0))Stop();
	Track_tim(6000,0,1);
	delay_distance(30);
//	while(!KeyState(0))Stop();
	qiaoban(12);
	Track_tim(5000,0,1);
	delay_distance(40);
	{
	skip_cross(20);
	Stop();
	dingjiao(yaw-90,0);
	Stop();
	}
//	track_turn(4000,-90,1,-2);
	Track_tim(6000,0,1);
	pingtai(0);
	delay_distance(20);
	track_turn(5000,-100,0,0);
	delay_distance(50);
//	while(!KeyState(0))Stop();
	qiaoban(12);
	Track_tim(6000,0,1);
	skip_cross(40);
	Track_tim(6000,1,1);
	delay_distance(80);
	Track_tim(6000,0,1);
}









//Ö±Á¢¾°µã
void zhili()
{
	while(SensorR(1)==1);
	stop_flag=1;
	track_flag=0;
	w=0;
	speed_r=2000;
	delay_distance(10);
	Stop();
	speed_r=-1000;
	delay_distance(-8);
	Turn_180(yaw);

	stop_flag=0;
	Track_tim(4000,0,1);
	delay_distance(10);
}



void zhili_2(int dis ,int turn)
{
	stop_flag=1;
	track_turn(3000,turn,1,0);
	int Temp_dis=distance_r;
	Track_tim(3000,0,1);
	while(SensorR(1)==1);
	Temp_dis=distance_r-Temp_dis;
	delay_distance(15);
	track_flag=0;
	Stop();
	w=0;
	speed_r=-3000;
	delay_distance(-Temp_dis-15);
	Stop();
//	while(1);
	dingjiao(yaw-turn,0);
//	dingjiao_run(yaw+turn,0,1);
//	osDelay(800);
//	dingjiao_run(yaw+turn,0,0);
	stop_flag=0;
	Track_tim(4000,0,1);
}





void backhome()
{
	WaitForDark;
	WaitForDark;
	bend(0);
	stop_flag=1;
	Anti_lose=0.01f;
	Track_tim(5000,0,1);
	delay_distance(20);
	Servo_5_mode(1);
	WaitForLight;
	Track_tim(0,0,0);
	float Temp_yaw=yaw;
	dingjiao_run(Temp_yaw,4000,1);
	delay_distance(40);
	Servo_5_mode(0);
	stop_flag=1;
	dingjiao_run(yaw,3000,1);
	delay_distance(10);
	Stop();
	flash_led();
//	while(!KeyState(0))Stop();
	Turn_180(yaw);
	stop_flag=0;
	Stop();
}
