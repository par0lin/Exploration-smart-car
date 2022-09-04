#ifndef RAMP_H
#define RAMP_H
#include "stdint.h"
typedef struct _ramp_mod
{
	int16_t target_val;		//目标参考值
	int16_t present_ref;	//当前参考值
	int16_t step_val;			//当前参考值到目标参考值所需要的步数
	int16_t inc_val;			//步长/斜率
	int16_t freq_hz;			//PID频率
}ramp_mod;

typedef __packed struct
{
    int16_t input;        //输入数据
    int16_t out;          //滤波输出的数据
    float zb;       //滤波参数
}first_order_filter_type_t;

void ramp_init(ramp_mod *p,int16_t freq_hz);
int32_t ramp_calc(ramp_mod *p);
void ramp_exec_t(ramp_mod *p,int32_t target_val,int32_t durationms);
void ramp_exec_s(ramp_mod *p,int32_t target_val,int32_t inc);
uint8_t ramp_completed(ramp_mod *p);
void ramp_stop(ramp_mod *p);
//一阶滤波初始化
void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, const float zb);
//一阶滤波计算
void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, int16_t input);
#endif
