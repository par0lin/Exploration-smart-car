#include "stdio.h"
#include "usart.h"
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int__io_putchar(int ch)
#else

#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
HAL_UART_Transmit(&huart3,(uint8_t *)&ch,1,0xFFFF);
return ch;
}
