#ifndef __BSP_U8G2_H
#define __BSP_U8G2_H

#include "main.h"
#include "u8g2.h"
#include "stdio.h"


extern u8g2_t u8g2;

uint8_t STM32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2_init(void);
void drawLogo(u8g2_t *u8g2);
void u8g2_Show(u8g2_t *u8g2, char *str);

#endif
