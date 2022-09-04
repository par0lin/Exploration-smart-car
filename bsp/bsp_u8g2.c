#include "bsp_u8g2.h"


u8g2_t u8g2; // 包含一次显示的所有数据的结构



void delay_US(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
}

uint8_t STM32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    //printf("%s:msg = %d,arg_int = %d\r\n",__FUNCTION__,msg,arg_int);
    switch(msg)
    {
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
        HAL_Delay(1);
        break;
    case U8X8_MSG_DELAY_I2C:		    // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        //delay 5us
        delay_US(5);						// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us

    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
        if(arg_int == 1)
        {
            HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET);
        }
        else if(arg_int == 0)
        {
            HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET);
        }
        break;							// arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:		// arg_int=0: Output low at I2C data pin
        //  printf("U8X8_MSG_GPIO_I2C_DATA:%d\r\n",arg_int);
        if(arg_int == 1)
        {
            HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET);
        }
        else if(arg_int == 0)
        {
            HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET);
        }
        break;							// arg_int=1: Input dir with pullup high for I2C data pin

    default:
        u8x8_SetGPIOResult(u8x8, 1);		// default return value
        break;
    }
    return 1;
}

void u8g2_init()
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, STM32_gpio_and_delay);  // 初始化u8g2结构体
	u8g2_InitDisplay(&u8g2); // 发送初始化序列到显示器，显示器在此之后处于睡眠模式
	u8g2_SetPowerSave(&u8g2, 0); // 打开显示，默认是节能模式，显示屏是关闭的。
}

void drawLogo(u8g2_t *u8g2)
{
	u8g2_SetFontMode(u8g2,1);

	u8g2_SetFontDirection(u8g2,0);         
	u8g2_SetFont(u8g2,u8g2_font_inb24_mf);
	u8g2_DrawStr(u8g2,0, 40, "CKYF");     	
    
}

void u8g2_Show(u8g2_t *u8g2, char *str)
{
	u8g2_ClearBuffer(u8g2);
	u8g2_SetFontDirection(u8g2,0);         
	u8g2_SetFont(u8g2,u8g2_font_inb24_mf);
	u8g2_DrawStr(u8g2,0, 30, str);      //写ASCII码 
	u8g2_SendBuffer(u8g2);
}



