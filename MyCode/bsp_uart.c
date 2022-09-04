#include "common.h"
/**************************************************************************
函数功能：将字符ASSIC码转换成十进制数 比如'1' 转成十进制1
入口参数：asc 码值
返回  值：assic码对应十进制数 
**************************************************************************/
unsigned char ASC_To_Valu(unsigned char  asc)
{	
	uint8 valu;
	switch(asc)
	{
		case 0x30:valu=0;break;
		case 0x31:valu=1;break;
		case 0x32:valu=2;break;
		case 0x33:valu=3;break;
		case 0x34:valu=4;break;
		case 0x35:valu=5;break;
		case 0x36:valu=6;break;
		case 0x37:valu=7;break;
		case 0x38:valu=8;break;
		case 0x39:valu=9;break;
	}
	return valu;
}
/**************************************************************************
函数功能：将一个长整型数字转换成字符串 比如12345 转换成字符串"12345"
入口参数：dat 对应长整型变量  *str字符串用于存储转换后的字符串
返回  值：无
**************************************************************************/

void u32tostr(unsigned long dat,char *str) 
{
 char  temp[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 unsigned char  i=0,j=0;
 i=0;
 while(dat)
 {
  temp[i]=dat%10+0x30;
  i++;
  dat/=10;
 }
 j=i;
 for(i=0;i<j;i++)
 {
  str[i]=temp[j-i-1];
 } 
 str[i]='\0';
}
/**************************************************************************
函数功能：串口发送一个字符
入口参数：USART_TypeDef* TUSARTx 可选择初始化对应串口  data 要发送的字符
返回  值：无
**************************************************************************/

void UART_PutChar(UART_HandleTypeDef* USARTx, uint8_t Data)  
{  
	HAL_UART_Transmit(USARTx, &Data,1,0xff);  
}  

/**************************************************************************
函数功能：串口DMA发送一个字符
入口参数：USART_TypeDef* TUSARTx 可选择初始化对应串口  data 要发送的字符
返回  值：无
**************************************************************************/

void UART_PutChar_DMA(UART_HandleTypeDef* USARTx, uint8_t Data)  
{  
	HAL_UART_Transmit_DMA(USARTx, &Data,1);  
}  

/**************************************************************************
函数功能：串口发送一个字符串
入口参数：USART_TypeDef* TUSARTx 可选择初始化对应串口  *str 要发送的字符串
返回  值：无
**************************************************************************/

void UART_PutStr (UART_HandleTypeDef* USARTx, char *str) 
{
	unsigned int i = 0;
	while(*str != '\0' && i < 500)
	{
		UART_PutChar(USARTx, *str++); 
		i++;
	}
}

/**************************************************************************
函数功能：串口DMA发送一个字符串
入口参数：USART_TypeDef* TUSARTx 可选择初始化对应串口  *str 要发送的字符串
返回  值：无
**************************************************************************/

void UART_PutStr_DMA (UART_HandleTypeDef* USARTx, uint8_t *str)
{
	HAL_UART_Transmit_DMA(USARTx, str, strlen((char*) str));  
}

/**************************************************************************
函数功能：将一个无符号整型数转换成对应字符串，再通过串口发送出去
入口参数: unit16 data
返回  值：无
**************************************************************************/
void UART_Put_Num(UART_HandleTypeDef* USARTx, uint16_t  dat)
{
 char temp[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 u32tostr(dat,temp);
 UART_PutStr(USARTx,temp); 
}

/**************************************************************************
函数功能：将一个无符号整型数转换成对应字符串，再通过串口发送出去
入口参数: 字符串*inf  无符号整型 dat
返回  值：无
**************************************************************************/
void UART_Put_Inf(UART_HandleTypeDef* USARTx, char *inf,uint16_t dat)
{
 UART_PutStr (USARTx,inf); 
 UART_Put_Num(USARTx, dat);
 UART_PutStr (USARTx,"\n");  
}



