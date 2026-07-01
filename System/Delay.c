#include "stm32f10x.h"
#include "Delay.h"
#include "stdio.h"

#define DWT_CTRL    (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT  (*(volatile uint32_t *)0xE0001004)
#define DEMCR       (*(volatile uint32_t *)0xE000EDFC)

#define DEMCR_TRCENA_Msk  (1UL << 24)

void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//设置定时器重装值
	SysTick->VAL = 0x00;					//清空当前计数值
	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
	SysTick->CTRL = 0x00000004;				//关闭定时器
}

void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
} 

void DWT_Init(void)
{
    DEMCR |= DEMCR_TRCENA_Msk;   // 使能 DWT
    DWT_CYCCNT = 0;
    DWT_CTRL |= 1;               // 启动 CYCCNT
}

void delay_us(uint32_t us)
{
    uint32_t start = DWT_CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    while ((DWT_CYCCNT - start) < ticks);
}

void delay_ms(uint32_t ms)
{
	uint32_t start = sys_tick;
    while ((sys_tick - start) < ms);
}
 

volatile uint32_t sys_tick = 0;//是不是我这里没有初始化造成的问题？

void SysTick_Init(void)
{
	uint32_t ret = SysTick_Config(SystemCoreClock / 1000);
	printf("SysTick_Config ret=%u\r\n", ret);
    printf("CTRL=%08x\r\n", *(volatile uint32_t *)0xE000E010);
}
	
uint32_t GetSysTick(void)
{
    return sys_tick;
}

void SysTick_Handler(void)
{
	sys_tick++;
	//printf("tick = %u\n",sys_tick);

}

