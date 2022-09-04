#include "ramp.h"
void ramp_init(ramp_mod *p,int16_t freq_hz)
{
	p->target_val = 0;      //目标参考值
	p->step_val = 0;				//当前参考值到目标参考值所需要的步数
	p->inc_val = 0;					//步长/斜率
	p->present_ref = 0;			//当前参考值
	p->freq_hz = freq_hz;		//PID频率
}
int32_t ramp_calc(ramp_mod *p)
{
	int32_t ref;
	ref = p->present_ref;
	
	if(p->step_val > 1)
	{
		ref += p->inc_val;
		p->step_val--;
	}
	else if(p->step_val == 1)
	{
		ref = p->target_val;
		p->step_val = 0;
	}
	p->present_ref = ref;
	
	return ref;
}
//定周期斜坡函数
//target_val：目标值
//durationms:斜坡周期 单位ms
void ramp_exec_t(ramp_mod *p,int32_t target_val,int32_t durationms)
{
	int32_t inc = 0;
	int32_t ref = 0;
	ref = p->present_ref;
	if(durationms == 0)
	{
		p->step_val = 0;
		p->inc_val = 0;
		p->present_ref = target_val;
	}
	else
	{
		p->target_val = target_val;
		//计算步长度
		p->step_val = (int32_t)durationms*p->freq_hz / 1000;
		p->inc_val = (p->target_val - ref)/p->step_val;
	}
}
//定步长斜坡函数
void ramp_exec_s(ramp_mod *p,int32_t target_val,int32_t inc)
{
	p->target_val = target_val;
	p->step_val = (target_val - p->present_ref)/inc + 1;
	p->inc_val = inc;
}
//返回值
//1：斜坡完成
//0：斜坡未完成
uint8_t ramp_completed(ramp_mod *p)
{
	uint8_t retval = 0;
	if(p->step_val == 0)
	{
		retval = 1;
	}
	return retval;
}
//停止斜坡
void ramp_stop(ramp_mod *p)
{
	p->step_val = 0;	
	p->inc_val = 0;
}

/**
  * @brief          一阶低通滤波初始化
  * @author         RM
  * @param[in]      一阶低通滤波结构体
  * @param[in]      间隔的时间，单位 s
  * @param[in]      滤波参数
  * @retval         返回空
  */
void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, const float zb)
{
    first_order_filter_type->zb = zb;
    first_order_filter_type->input = 0;
    first_order_filter_type->out = 0;
}

/**
  * @brief          一阶低通滤波计算
  * @author         RM
  * @param[in]      一阶低通滤波结构体
  * @param[in]      间隔的时间，单位 s
  * @retval         返回空
  */
void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, int16_t input)
{
    first_order_filter_type->input = input;
    first_order_filter_type->out =
			 first_order_filter_type->input * first_order_filter_type->zb + first_order_filter_type->out * (1 - first_order_filter_type->zb); 
}
