#ifndef __SECTION_H
#define __SECTION_H
#include "main.h"
extern int color_flag;

typedef
	struct
	{
		uint8_t Zhili_1;
		uint8_t Zhili_2;
		uint8_t Zhili_3;
		uint8_t Zhili_4;
		uint8_t Zhili_5;
		
	}Struct_Sight_Skip;

extern Struct_Sight_Skip SKIP;
/*---------------------------NEW-SECTION---------------------------------*/

////////////////before_the_door/////////////////////
void p1_to_p2();
void p2_to_p3();
void p2_to_p4();
	
void p3_to_s01();
void s01_to_p4();
void p3_to_p4();
	
void p4_to_s02();
void s02_to_cross2();
void p4_to_cross2();
	
//////////////////door1 & door3/////////////////////

void cross3_to_p6();
void cross3_to_p8();
	
void p6_to_s03();
void s03_to_p8();
void p6_to_p8();

void p8_to_p7();
void p7_to_p5();
void p7_to_cross3();

void p5_to_s05();
void s05_to_s04();
void s04_to_cross3();
void p5_to_cross3();

void cross3_to_cross12();

//////////////////door2 & door4/////////////////////
void cross4_to_p5();
void cross4_to_p6();
void cross4_to_p8();

void p5_to_s05();
void s05_to_s04();
void s04_to_p6();
void p5_to_p6();

void p6_to_s03();
void s03_to_p8();
void p6_to_p8();

void p8_to_p7();
void p7_to_cross4();	
void cross4_to_cross12();
///////////////////////////BACK////////////////////////////////////////////
void cross12_back();
/////////////////////find//door////////////////////////////
void find_door();



/*
void p1_to_p2();

void p2_to_s01();
void s01_to_p3();

void p2_to_p3();

void p3_to_p4();
void p4_to_s02();
void s02_to_p5();

void p4_to_P5();

void p5_to_p7();
void p7_to_p8();

void p5_s05();
void s05_s04();
void s04_s03();
void s03_p8();
void p8_p7();
void p7_p1();
*/


void jiansudai(int speed);
void jiansudai_double(int speed);
void jieti_1(int speed);
void jieti_2();
void qiaoban(int speed);
void pingtai6();
void ready();
void jinxingban_start();
void long_bridge(int speed);
void pingtai(int th);
void pingtai7(int th);
void zhili();
void zhili_2(int dis ,int turn);
void pingtai8(int th);
void backhome();
	
#endif
