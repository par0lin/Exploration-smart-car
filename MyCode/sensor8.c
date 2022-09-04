#include "main.h"
#include "usart.h"
#include "sensor8.h"


extern UART_HandleTypeDef huart2;
extern unsigned char TxBuffer_T;

volatile uint8_t USART_RX_STA[2] = { 0 };       //接收状态标记	  
int Num=0;



void Read_Data(int *Data)	       
{	
	uint8_t y=0;
	int Receive_data = 0;       //数据缓存区
	uint8_t Temp=0x57;
	HAL_UART_Transmit(&huart2, &Temp, 1, 1000);
	
/*---------------------------数字量数值----------------------------*/	
//	
//	if(Num != 1)
//	{
//		
//		HAL_Delay(1);
//		if(y>5)
//		{
//			y=0;
//			return ;
//		}
//		y++;
//	}
//	*Data = USART_RX_STA[0];
	if(Num != 2)
	{
		
		HAL_Delay(1);
		if(y>7)
		{
			y=0;
			return;
		}
		y++;
	}
	Receive_data = USART_RX_STA[1];
	Receive_data <<= 8;
	Receive_data |= USART_RX_STA[0];

	*Data = Receive_data;
	
/////////////////////////数字量数值///////////////////////////////	

///////////////////////////偏移量数值///////////////////////////////	
	
//	if(Num != 3)
//	{
//		
//		HAL_Delay(1);
//		if(y>10)
//		{
//			y=0;
//			uint8_t Temp=0x57;
//			HAL_UART_Transmit(&huart3, &Temp, 1, 1000);
//			return;
//		}
//		y++;
//	}
//	Receive_data = USART_RX_STA[1];
//	Receive_data <<= 8;
//	Receive_data |= USART_RX_STA[2];

//	*Data = USART_RX_STA[0];
//	*(Data+1) = Receive_data;

//	if(Num != 3)
//	{
//		
//		HAL_Delay(1);
//		if(y>10)
//		{
//			y=0;
//			return ;
//		}
//	y++;
//	}
//	Receive_data = USART_RX_STA[1];
//	Receive_data <<= 8;
//	Receive_data |= USART_RX_STA[2];

//	*Data = USART_RX_STA[0];
//	*(Data+1) = Receive_data;
//	
	
///////////////////////////偏移量数值///////////////////////////////	

	Num = 0; 
}


/*********************************************************************
 *  函数名称：Set_Data
 *  函数功能：设置功能参数
 *  形    参：add:寄存器地址,Par: 参数,
 *  输    出：无
 *  备    注：无
 ********************************************************************/
void Set_Data(int add,int Par)	       
{	
///////////////////////////设置功能参数///////////////////////////////	
//  while(HAL_UART_GetState(&huart3) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束
	uint8_t Temp_Uart=0x4C;
	HAL_UART_Transmit(&huart2, &Temp_Uart,1,1000);
	while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束
	HAL_UART_Transmit(&huart2, (uint8_t *)&add,1,1000);
	while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束
	HAL_UART_Transmit(&huart2, (uint8_t *)&Par,1,1000);
	while(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束

///////////////////////////设置功能参数///////////////////////////////		
}

void Sensor_Init(void)
{
	uint8_t Temp=0x57;
	HAL_UART_Transmit(&huart2, &Temp, 1, 1000);
	HAL_UART_Receive_IT(&huart2,&USART_RX_STA[Num++],1);			//使能串口3，一般每次都调用就能实现实时接收蓝牙传输的信息
}



void Trig_Sensor()
{
	uint8_t Temp=0x57;
	HAL_UART_Transmit_DMA(&huart2, &Temp, sizeof(Temp));
}






