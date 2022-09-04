#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"
#include "bsp_can.h"
#include "pid.h"
#include "led.h"
#include "usart.h"
#include "JY901.h"
#include "ramp.h"
#include "hw.h"
#include "delay.h"
#include "bsp_u8g2.h"
#include "sensor8.h"
#include "uart_it.h"
#include "tim_it.h"

#define a0 0
#define b0 1
#define vxmax 8000
#define vymax 8000

extern int32_t w;
extern int SpdL,SpdR,Spd_w;
extern int speed_tim;
extern volatile int distance_r;
extern volatile uint8_t stop_flag;
extern volatile uint8_t dingjiao_flag,dingjiao_mode,dingju_flag,xunji_flag,track_flag;
extern int32_t a1,a2,a3,a4;
extern int32_t s_x,s_y;
extern double initaz;
extern double yaw;
extern double yaw0;
extern double th0;
extern float voltage;
extern PID_TypeDef 	dingjiao_pid;
extern PID_TypeDef 	track_pid;
extern PID_TypeDef motor_pid[4];  //2006电机pid结构体
extern ramp_mod vx_ramp;
extern ramp_mod vy_ramp;
extern first_order_filter_type_t vx_filter;
extern first_order_filter_type_t vy_filter;
extern volatile uint8_t Ltime,Ytime;
extern uint8_t cnt;
extern volatile int Sensor_GetCount;
extern int location_numble[][17];
extern volatile uint8_t track_mode;
extern volatile uint8_t dingjiao_state;
extern int speed_r;
extern int speed_track;
extern volatile int ahead_location;
extern volatile int last_location;
extern volatile uint8_t OPENMV[2];
extern volatile uint8_t Traffic_light;
extern uint8_t Traffic_pass_record;
extern uint8_t Traffic_pass_record_rc;
extern float Anti_lose;
extern uint8_t Weak_mode;
extern volatile uint8_t track_flag_bridge;
extern volatile uint64_t Trig_Sensor_flag;
extern volatile int track_bridge_out;
extern volatile uint8_t door_find_fail;

float my_abs(float data);
void wheel_init(void);
void WT_init(void);
void yundong_cal(int32_t left,int32_t right,int32_t w_t);
void Stop(void);
void dingjiao(float th,uint8_t flag);
void dingjiao_run(double th,int speed,int state);
void Track_s(int speed);
void WaitForCross();
void Track_2(int speed);
int Count_Sensor(void);
//void dingju(int32_t dis,int32_t th);
void Track_tim(int speed, int mode,	int state);
void Measure(char mode);
void Tracking(char mode, int speed);
void track_turn(int speed,int th,int mode,int offset);
void Turn_180(float yaw_temp);
void delay_distance(int dis);
void Ramp_dis(int spd1,int spd2,int dis);
void SpeedUP_track(uint32_t speed_max,uint32_t speed_final,uint32_t distance,uint16_t ramp_dis );
void flash_led();
void battery();
void show_battery();
void battery_check();
char* itoa(int num,char* str,int radix);
void yundong_add(int32_t left,int32_t right,int32_t w_t);
uint8_t detect_Traffic_light();
int Traffic_pass();
void skip_cross(uint8_t dis);
void Servo_5_mode(uint8_t mode);
void record_to_flash(uint8_t door_id);
void record_read();
void record_show();

#endif 
