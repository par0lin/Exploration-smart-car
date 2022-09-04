#include "control.h"
#include "bsp_can.h"
#include "math.h"
#include "adc.h"
#include "key.h"
#include "servo.h"
#include "bsp_flash.h"
#include "MyTask.h"

#define USER_FLASH_ADDRESS  ADDR_FLASH_SECTOR_11
#define FLASH_DATA_LENGHT   4


int32_t w;
double initaz;
int speed_tim;
int SpdL,SpdR,Spd_w;
volatile uint8_t stop_flag=0;
volatile uint8_t dingjiao_flag;
volatile uint8_t dingjiao_state=0;
volatile uint8_t dingjiao_mode;
volatile uint8_t dingju_flag;
volatile uint8_t xunji_flag;
volatile uint8_t track_flag=0;
volatile uint8_t track_flag_bridge=0;
volatile uint8_t door_find_fail;

volatile int track_bridge_out=0;
volatile uint8_t track_mode=0;//0,中；1，左；2，右
volatile int Sensor_GetCount=0;
int32_t a1,a2,a3,a4;
int32_t s_x,s_y;
double yaw;
double yaw0;
double th0;
float voltage;
PID_TypeDef motor_pid[4];  //2006电机pid结构体
PID_TypeDef 	dingjiao_pid;
PID_TypeDef 	track_pid;
volatile uint8_t OPENMV[2];
volatile uint8_t Traffic_light;
int mark[17]={0};
volatile int ahead_location=0;
volatile int last_location=0;
volatile uint8_t Ltime = 0, Rtime = 0, Ytime = 0;
int speed_r=0;
int speed_track=0;
volatile int distance_r=0;
uint8_t Traffic_pass_record;
uint8_t Traffic_pass_record_rc=0;
float Anti_lose=0;		//防丢线灵敏度
uint8_t Weak_mode=0;
volatile uint64_t Trig_Sensor_flag=0;



float my_abs(float data)
{
	return data>0?data:-data;
}



char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 
    //获取要转换的整数的绝对值
    if(radix==10&&num<0)//要转换成十进制数并且是负数
    {
        unum=(unsigned)-num;//将num的绝对值赋给unum
        str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
    }
    else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位
 
    }while(unum);//直至unum为0退出循环
 
    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整
 
    char temp;//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
 
    return str;//返回转换后的字符串
 
}




void wheel_init()
{
  my_can_filter_init_recv_all(&hcan1);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
  for(int i=0; i<4; i++)
  {	
		float pid[3]={5,0.5,0};
		PID_init(&motor_pid[i], PID_DELTA, pid, 16800, 1000);
  }
	{
		float pid[3]={60,0.5,50};
		PID_init(&dingjiao_pid, PID_POSITION, pid, 6000, 0.1);
	}
	{
		float pid[3]={60,0.5,20};
		PID_init(&track_pid, PID_POSITION, pid, 8000, 0.1);
	}
	
}

/***********************陀螺仪*************************/
void WT_init()
{
	HAL_UART_Receive_IT(&huart4,&TxBuffer_T,1);
	initaz = (float)stcAngle.Angle[2]/32768*180;
}


int Traffic_pass()
{
	if(Traffic_light==1)return 1;
	else return 0;
}
	

//左速度，右速度，转动速度逆时针为正
void yundong_cal(int32_t left,int32_t right,int32_t w_t)
{
	int v1,v2;
	v1=right-w_t;
	v2=-left-w_t;
	motor_pid[0].set = v1; 
	motor_pid[1].set = v1; 
	motor_pid[2].set = v2;
	motor_pid[3].set = v2;
	
}

void yundong_add(int32_t left,int32_t right,int32_t w_t)
{
	int v1,v2;
	v1=right-w_t;
	v2=-left-w_t;
	motor_pid[0].set += v1; 
	motor_pid[1].set += v1; 
	motor_pid[2].set += v2;
	motor_pid[3].set += v2;
	
}

uint8_t detect_Traffic_light()
{
	if(OPENMV[0]==0&&OPENMV[1]==0)return 0;
	if(OPENMV[0]==0&&OPENMV[1]==1)return 1;
	if(OPENMV[0]==1&&OPENMV[1]==0)return 2;
	if(OPENMV[0]==1&&OPENMV[1]==1)return 3;
}
	

void Stop(void)
{
	track_flag=0;
	track_flag_bridge=0;
	dingjiao_state=0;
	dingjiao_mode=0;
	dingjiao_flag=0;
	speed_r=0;
	w=0;
}

void Ramp_dis(int spd1,int spd2,int dis)
{
	int delta_spd=(spd2-spd1)/dis;
	for(int i=1;i<dis;i++)
	{
		speed_r=spd1+delta_spd*i;
		delay_distance(1);
	}
	speed_r=spd2;
}
//0可以写在while外面，到达角度会自动退出，1会一直执行
void dingjiao(float th,uint8_t flag)
{
	dingjiao_state=0;
	PID_clear(&dingjiao_pid);
//	dingjiao_pid.Ki=0.2+0.6*((my_abs(err)-90)/90);
	Stop();
	osDelay(50);
	dingjiao_mode=1;dingjiao_flag=0;
	yaw0=th;
	if(flag==0)
	{
		while(dingjiao_flag==0);
		int speed_temp=(moto_chassis[0].speed_rpm+moto_chassis[1].speed_rpm+moto_chassis[2].speed_rpm+moto_chassis[3].speed_rpm)/4.0;
		osDelay(my_abs(speed_temp)*0.08);
		Stop();
	}
}


void set_th()
{
	th0=yaw;
}

	

//陀螺仪走直线（角度，速度，开关）
void dingjiao_run(double th,int speed,int state)
{
	PID_clear(&dingjiao_pid);
	if(state==0)
	{
		dingjiao_mode=0;
		speed_r=speed;
		w=0;
	}else
	{		
		track_flag=0;
		track_flag_bridge=0;
		w=0;
		yaw0=th;
		double err=yaw-yaw0;
		if(err > 180)
		{
			err = err-360;
		}
		if(err < -180)
		{
			err = err+360;
		}
		dingjiao_pid.Ki=0.2+0.6*((my_abs(err)-90)/90);
		dingjiao_flag=0;
		speed_r=speed;
		dingjiao_mode=1;
	}
}


int Count_Sensor()
{
	int cnt=0;
	for(int i=0; i<16; i++)
	{
		if(Sensor[i] == 0)
			cnt++;
	}
	return cnt;
}

//16路传感器
void Track_s(int speed)
{
	speed_r=speed;
	static int w_cnt=0,w_cnt_last=0;
	Sensor_GetCount = Count_Sensor();

	for(int i=7;i<=15;i++)
	{
		if(Sensor[i]==1&&Sensor[i+1]==1)			 {w_cnt=-(i+0.5-7.5)*speed_r/5;break;}
		if(Sensor[15-i]==1&&Sensor[15-(i+1)]==1){w_cnt=(i+0.5-7.5)*speed_r/5;break;}
		if(i==15)w_cnt = w_cnt_last;
	}
	w_cnt_last=w_cnt;
	w=w_cnt;
}

void WaitForCross()
{
	int i,count=0;
	while(count<=4)
	{
		for(i=3,count=0;i<=12;i++)if(Sensor[i]==1)count++;
	}
}
	

void Tracking(char mode, int speed)
{	
	speed_r=speed;

	if(mode == 0)
	for(int i=7;i<=15;i++)
	{
		if(Sensor[i]==1&&Sensor[i+1]==1)			 {ahead_location=-(i+0.5-7.5)*10;break;}
		if(Sensor[15-i]==1&&Sensor[15-(i+1)]==1){ahead_location=(i+0.5-7.5)*10;break;}
		if(i==15)ahead_location=last_location;
	}
	
	else if(mode == 1)
	for(int i=0;i<=15;i++)
	{
		if(Sensor[i]==1&&Sensor[i+1]==1){ahead_location=-(i+0.5-7.5)*10;break;}
		if(i==15)ahead_location=last_location;
	}
	
	else if(mode == 2)
	for(int i=15;i>=0;i--)
	{
		if(Sensor[i]==1&&Sensor[i+1]==1){ahead_location=-(i+0.5-7.5)*10;break;}
		if(i==0)ahead_location=last_location;
	}
	last_location=ahead_location;
}


/*
void Tracking(char mode, int speed)
{	
	if(mode == 0)
	for(int i=7;i<=15;i++)
	{
		if(Sensor[i]==1)
		{
			if(Sensor[i+1]==1){ahead_location=-(i+0.5-7.5)*10;break;}
			else{ahead_location=-(i-7.5)*10;break;}
		}

		if(Sensor[15-i]==1)
		{
			if(Sensor[15-(i+1)]==1){ahead_location=(i+0.5-7.5)*10;break;}
			else{ahead_location=(i-7.5)*10;break;}
		}
			 
		if(i==15)
		{
			if(Anti_lose!=0.0f)ahead_location=last_location/my_abs(last_location)*Anti_lose;
			else ahead_location=last_location;
		}
	}
	
	else if(mode == 1)
	for(int i=0;i<=15;i++)
	{
		if(Sensor[i]==1)
		{
			if(Sensor[i+1]==1){ahead_location=-(i+0.5-7.5)*10;break;}
			else{ahead_location=-(i-7.5)*10;break;}
		}
		if(i==15)
		{
			if(Anti_lose!=0.0f)ahead_location=last_location/my_abs(last_location)*Anti_lose;
			else ahead_location=last_location;
		}
	}
	
	else if(mode == 2)
	for(int i=15;i>=0;i--)
	{
		if(Sensor[i]==1)
		{
			if(Sensor[i-1]==1){ahead_location=-(i-0.5-7.5)*10;break;}
			else{ahead_location=-(i-7.5)*10;break;}
		}
		if(i==0)
		{
			if(Anti_lose!=0.0f)ahead_location=last_location/my_abs(last_location)*Anti_lose;
			else ahead_location=last_location;
		}
	}
	last_location=ahead_location;

}
*/

//mode	0，中  1，左  2，右
//state 1开始    0停止
void Track_tim(int speed, int mode,	int state)
{
	dingjiao_state=0;
	dingjiao_mode=0;
	dingjiao_flag=0;
	track_mode=mode;
	track_flag=state;
	speed_r=speed;
}

//长桥循迹
void Track_2(int speed)
{
	speed_r=speed;
	if((SensorR(2)==1&&SensorR(3)==1)||(SensorR(2)==0&&SensorR(3)==0))
	{
		track_pid.out=0;
	}
	else if(SensorR(2)==0)track_pid.out=-speed*1*0.5;
	else if(SensorR(3)==0)track_pid.out=speed*1*0.5;
}


//循迹转弯，只执行一次,mode：0打开自动平滑，1关闭自动平滑
void track_turn(int speed,int th,int mode,int offset)
{
	PID_clear(&dingjiao_pid);
	speed_r=speed;
	int i,count=0;
	Track_tim(speed,track_mode,1);
	while(count<=4)
	{
		for(i=3,count=0;i<=12;i++)if(Sensor[i]==1)count++;
	}
	
//	int temp_dis=distance_r;
//	while(count>3)
//	{
//		for(i=3,count=0;i<=12;i++)if(Sensor[i]==1)count++;
//	}
//	int temp_dis1=20-(distance_r-temp_dis)/2;
	int temp_dis1=23;
	servo(0,5);
	Track_tim(0,track_mode,0);
	w=0;
	speed_r=speed;
	if((th>120||th<-120)||mode==1)
	{
		//Track_tim(0,track_mode,0);
		//dingju(130,yaw);
		float temp1=(speed-1000)/10000.0;
		speed_r=speed;
		delay_distance(offset+temp_dis1*(1-temp1));
		Ramp_dis(speed,1000,temp_dis1*temp1);
		Stop();
		osDelay(100);
//		while(1)Stop();
//		Track_tim(0,track_mode,0);
		dingjiao(th+yaw,0);
	}
	else
	{
		delay_distance(5+offset);
		dingjiao_state=0;
		yaw0=yaw+th;
		double err=yaw-yaw0;
		if(err > 180)
		{
			err = err-360;
		}
		if(err < -180)
		{
			err = err+360;
		}
//		dingjiao_pid.Ki=0.2+0.6*((my_abs(err)-90)/90);
		dingjiao_flag=0;
		dingjiao_mode=1;
		Ramp_dis(speed,100,my_abs(err)/57.29578*15*(1-speed/16800));
		while(dingjiao_flag==0);
		int speed_temp=(moto_chassis[0].speed_rpm+moto_chassis[1].speed_rpm+moto_chassis[2].speed_rpm+moto_chassis[3].speed_rpm)/4.0;
		osDelay(my_abs(speed_temp)*0.08);
	}
	servo(30,5);
	Ramp_dis(500,speed,5);
	Track_tim(speed,track_mode,1);
}

void Turn_180(float yaw_temp)
{
	Stop();
	w=4000;
	osDelay(200);
	w=0;
	dingjiao(yaw_temp+180,0);
}


//定距延时
void delay_distance(int dis)
{
	int dis0=distance_r;
	if(dis>=0)while(distance_r<dis0+dis);
	else while(distance_r>dis0+dis);
}

//跳过路口
void skip_cross(uint8_t dis)
{
	WaitForCross();
	delay_distance(dis);
}

//0-正常模式，1-低头模式，2-抬头模式
void Servo_5_mode(uint8_t mode)
{
	if(mode==0)servo(40,5);
	if(mode==1)servo(85,5);
	if(mode==2)servo(0,5);
}
	
void SpeedUP_track(uint32_t speed_max,uint32_t speed_final,uint32_t distance,uint16_t ramp_dis )
{
	Track_tim(speed_r,track_mode,1);
	Ramp_dis(speed_r,speed_max,ramp_dis);
	delay_distance(distance-2*ramp_dis);
	Ramp_dis(speed_max,speed_final,ramp_dis);
	Track_tim(speed_final,track_mode,1);
}


void flash_led()
{
	HAL_GPIO_WritePin(R8_GPIO_Port,R8_Pin,GPIO_PIN_RESET);
	osDelay(200);
	HAL_GPIO_WritePin(R8_GPIO_Port,R8_Pin,GPIO_PIN_SET);
	osDelay(200);
	HAL_GPIO_WritePin(R8_GPIO_Port,R8_Pin,GPIO_PIN_RESET);
	osDelay(200);
	HAL_GPIO_WritePin(R8_GPIO_Port,R8_Pin,GPIO_PIN_SET);
}

void battery()
{
	float v=0;
	for(int i=0;i<100;)
	{
		HAL_ADC_Start(&hadc1);//开启ADC
		HAL_ADC_PollForConversion(&hadc1,100);//等待ADC转换完成，超时为100ms
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC))	//判断ADC是否转换成功 
		{
			v+=HAL_ADC_GetValue(&hadc1)*8.5;//读取值
			i++;
		}
	}
	
	voltage=(v/100)*0.8074;
}

void show_battery()
{
	battery();
	char v[6];
	itoa((int)voltage,v,10);
	u8g2_Show(&u8g2,v);
}

void battery_check()
{
	while(voltage<11000)
	{
		show_battery();
		HAL_Delay(500);
		u8g2_Show(&u8g2,"LOW");
		HAL_Delay(500);
		if(KeyState(0))break;
	}
		while(voltage>14000&&voltage<22200)
	{
		show_battery();
		HAL_Delay(500);
		u8g2_Show(&u8g2,"LOW");
		HAL_Delay(500);
		if(KeyState(0))break;
	}
}


// void record_to_flash(uint8_t door_id)
// {
// 	Stop();
// 	osDelay(400);
// 	Weak_mode=1;
// 	osDelay(200);
// 	set_moto_current(&hcan1,0,0,0,0);
// 	HAL_TIM_Base_Stop_IT(&htim13);
// 	Anti_lose=0;
// 	osDelay(50);
// 	uint8_t write_data[4]="T0\r\n";
// 	write_data[1]=door_id;
// 	flash_erase_address(USER_FLASH_ADDRESS, 1);
// 	flash_write_single_address(USER_FLASH_ADDRESS, (uint32_t *)write_data, (FLASH_DATA_LENGHT + 3) / 4);
// 	record_read();
// 	record_show();
// 	osDelay(200);
// 	HAL_TIM_Base_Start_IT(&htim13);
// 	Weak_mode=0;
// 	Stop();
// }
	

void record_to_flash(uint8_t door_id)
{
	Stop();
	osDelay(50);
	Weak_mode=1;
	osDelay(50);
	set_moto_current(&hcan1,0,0,0,0);
	HAL_TIM_Base_Stop_IT(&htim13);
	Anti_lose=0;
	osDelay(50);
	uint8_t write_data[4]="T0\r\n";
	write_data[1]=door_id;
	flash_erase_address(USER_FLASH_ADDRESS, 1);
	flash_write_single_address(USER_FLASH_ADDRESS, (uint32_t *)write_data, (FLASH_DATA_LENGHT + 3) / 4);
	record_read();
	record_show();
	osDelay(50);
	HAL_TIM_Base_Start_IT(&htim13);
	Weak_mode=0;
	Stop();
}

void record_read()
{
	uint8_t Read_Datab[4];	
	flash_read(USER_FLASH_ADDRESS, (uint32_t *)Read_Datab, (FLASH_DATA_LENGHT + 3) / 4);
	if(Read_Datab[0]=='T')Traffic_pass_record=Read_Datab[1];
	else Traffic_pass_record=0;
}	

void record_show()
{
	if(KeyState(0))record_to_flash(0);
	char r,str[10]="Door:0";
	str[5]+=Traffic_pass_record;
	u8g2_Show(&u8g2,str);
}
	