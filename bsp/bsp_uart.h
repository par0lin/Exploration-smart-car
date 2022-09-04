#ifndef  _COMMON_H_
#define  _COMMON_H_
#define uint8  unsigned char
#define uint32    unsigned int   
#define uint16    unsigned short int  
#include "usart.h"
#include "string.h"
unsigned char ASC_To_Valu(unsigned char  asc);
void u32tostr(unsigned long dat,char *str);
void UART_PutChar(UART_HandleTypeDef* USARTx, uint8_t Data);
void UART_PutStr (UART_HandleTypeDef* USARTx, char *str);
void UART_PutChar_DMA(UART_HandleTypeDef* USARTx, uint8_t Data);
void UART_PutStr_DMA (UART_HandleTypeDef* USARTx, uint8_t *str);
void UART_Put_Num(UART_HandleTypeDef* USARTx, uint16_t  dat);
void UART_Put_Inf(UART_HandleTypeDef* USARTx, char *inf,uint16_t dat);
#endif
