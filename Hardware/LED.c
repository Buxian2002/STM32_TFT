#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Key.h"

/**
  * 函    数：LED初始化
  * 参    数：无
  * 返 回 值：无
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*GPIO初始化*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);						//将PA1和PA2引脚初始化为推挽输出
	
	/*设置GPIO初始化后的默认电平*/
	GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7);				//设置PA1和PA2引脚为高电平
	
}

/**
  * 函    数：LED1开启
  * 参    数：无
  * 返 回 值：无
  */
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);		//设置PA1引脚为低电平
}

/**
  * 函    数：LED1关闭
  * 参    数：无
  * 返 回 值：无
  */
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_6);		//设置PA1引脚为高电平
}

/**
  * 函    数：LED1状态翻转
  * 参    数：无
  * 返 回 值：无
  */
void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_6) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);					//则设置PA1引脚为高电平
	}
	else													//否则，即当前引脚输出高电平
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);					//则设置PA1引脚为低电平
	}
}

/**
  * 函    数：LED2开启
  * 参    数：无
  * 返 回 值：无
  */
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);		//设置PA2引脚为低电平
}

/**
  * 函    数：LED2关闭
  * 参    数：无
  * 返 回 值：无
  */
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_7);		//设置PA2引脚为高电平
}

/**
  * 函    数：LED2状态翻转
  * 参    数：无
  * 返 回 值：无
  */
void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{                                                  
		GPIO_SetBits(GPIOC, GPIO_Pin_7);               		//则设置PA2引脚为高电平
	}                                                  
	else                                               		//否则，即当前引脚输出高电平
	{                                                  
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);             		//则设置PA2引脚为低电平
	}
}

int status_now1 = LED_off;

void LED_status(int key_input)
{
	switch(status_now1){
		case LED_off:
			if(key_input == button_up_long){
				LED1_ON();
				LED2_OFF();
				status_now1 = LED_1;
			}
			break;
		case LED_1:
			if(key_input == button_down_short){
				LED1_OFF();
				LED2_ON();
				status_now1 = LED_2;
			}
			if(key_input == button_down_long){
				LED1_OFF();
				status_now1 = LED_off;
			}
			break;
		case LED_2:
			if(key_input == button_up_short){
				LED1_ON();
				LED2_OFF();
				status_now1 = LED_1;
			}
			if(key_input == button_down_long){
				LED2_OFF();
				status_now1 = LED_off;
			}
			break;
	}
}

