#include "JY901.h"


static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 

char ACCCALSW[5] = {0XFF,0XAA,0X01,0X01,0X00};//������ٶ�У׼ģʽ
char SAVACALSW[5]= {0XFF,0XAA,0X00,0X00,0X00};//���浱ǰ����

struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;
struct SQ       stcQ;



void UART4_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;  
  while(__HAL_UART_GET_FLAG(&huart4,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
}



//�ô���2��JYģ�鷢��ָ��
void sendcmd(char cmd[])
{
	char i;
	for(i=0;i<5;i++)
	{
		while((UART4->SR&0X40)==0);  
			UART4->DR = cmd[i]; 
	}
}


void CopeSerial1Data(unsigned char ucData)
{	
	UART4_Put_Char(ucData);//ת������1�յ������ݸ�����2��JYģ�飩
}

//CopeSerialDataΪ����2�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
			case 0x59:	memcpy(&stcQ,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;//��ջ�����
	}
}

void UART4_Put_String(unsigned char *Str)
{
	while(*Str)
	{
		if(*Str=='\r')UART4_Put_Char(0x0d);
			else if(*Str=='\n')UART4_Put_Char(0x0a);
				else UART4_Put_Char(*Str);
		Str++;
	}
}

//void USART2_IRQHandler(void)
//{
//  if(HAL_UART_GetState(&huart2) != RESET)
//  {   
////    USART_SendData(USART2, TxBuffer[TxCounter++]); 
//		int len = sizeof(TxBuffer)/sizeof(TxBuffer[0]);
//		HAL_UART_Transmit(&huart2,(uint8_t*)TxBuffer,len,1000);	//���ͽ��յ�������
//    USART_ClearITPendingBit(USART2, USART_IT_TXE);
//    if(TxCounter == count) USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//  }
//	else if(HAL_UART_GetState(&huart2) != RESET)
//  {
//		CopeSerial2Data((unsigned char)USART2->DR);//��������
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//  }
//	
//	USART_ClearITPendingBit(USART2,USART_IT_ORE);
//}


