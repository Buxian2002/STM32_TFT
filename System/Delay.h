#ifndef __DELAY_H
#define __DELAY_H

#include "stdint.h"

void DWT_Init(void);

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

extern volatile uint32_t sys_tick;

void SysTick_Init(void);
void SysTick_Handler(void);
uint32_t GetSysTick(void);

#endif



