//#define _MICROLIB
#include "stm32f10x.h"
#include "Delay.h"
#include "Sys_clk.h"
#include "Spi.h"
#include "TFT.h"
#include "Key.h"
#include "LED.h"
#include "Chinese_chars.h"
#include "Status_machine.h"
#include "stdio.h"

//volatile uint32_t sys_tick = 0; 

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    // 开时钟（USART1 在 APB2 上）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    
    // PA9 = TX，复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // PA10 = RX，浮空输入（printf 只用 TX 的话这步可以不配，但配了没坏处）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // USART 参数
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}


int main(void)
{
	int i;
	SYS_Init();
	SPI1_Init();
	Key_Init();	
	OLED_init();
	USART1_Init();
	printf("USART1\n");
	printf("sys_tick after init = %u\r\n", sys_tick);
	LED_Init();
	SysTick_Init();
	printf("SysTick\n");
	printf("sys_tick after init = %u\r\n", sys_tick);
	printf("LET'S HAVE A LOOK!");
	

	printf("after manual call, sys_tick=%u\r\n", sys_tick);
	
	while (1)
	{
		for(i = 0; i < 6; i++)
		{
			Key_Process(&keys[i]);
			
			if(keys[i].short_press){
				keys[i].short_press = 0;
				switch(i){
					case UPWARD:
						display_status(button_up_short);
						break;
					case DOWNWARD:
						display_status(button_down_short);
						break;
					case LEFT:
						display_status(button_left_short);
						break;
					case RIGHT:
						display_status(button_right_short);
						break;
					case OK:
						display_status(button_ok_short);
						break;
					case BACK:
						display_status(button_back_short);
						break;
				}	
			}
			else if(keys[i].long_press){
				keys[i].long_press = 0;
				switch(i){
					case UPWARD:
						display_status(button_up_long);
						break;
					case DOWNWARD:
						display_status(button_down_long);
						break;
					case LEFT:
						display_status(button_left_long);
						break;
					case RIGHT:
						display_status(button_right_long);
						break;
					case OK:
						display_status(button_ok_long);
						break;
					case BACK:
						display_status(button_back_long);
						break;
				}	
			}
		}
	}
}
