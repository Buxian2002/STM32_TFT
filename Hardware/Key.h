#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "stdint.h"

enum KeyNum {
    UPWARD = 0,     //PB6
    DOWNWARD,    	//PB7
    LEFT,  			//PB8
    RIGHT,   		//PB9
    OK,    			//PC11
    BACK,  			//PC12
};

enum key_input{
	button_up_short = 1,
	button_up_long,
	button_down_short,
	button_down_long,
	button_left_short,
	button_left_long,
	button_right_short,
	button_right_long,
	button_ok_short,
	button_ok_long,
	button_back_short,
	button_back_long,
};

typedef enum {
    KEY_IDLE,
    KEY_DEBOUNCE,
    KEY_PRESSED,
    KEY_RELEASE_DEBOUNCE
} KeyState_t;

#pragma pack(1)
typedef struct {
    GPIO_TypeDef *port;      // GPIO 端口
    uint16_t pin;            // GPIO 引脚
    KeyState_t state;        // 当前状态
    uint32_t tick_start;     // 状态进入时的 SysTick
    volatile uint8_t  pending;        // 中断置位，表示有事件待处理
    uint8_t  short_press;    // 短按事件（一次有效）
    uint8_t  long_press;     // 长按事件（一次有效）
} Key_t;
#pragma pack()


#define NUM_KEYS 6

/*
extern volatile uint8_t keyup_flag;
extern volatile uint8_t keydown_flag;
extern volatile uint8_t keyleft_flag;
extern volatile uint8_t keyright_flag;
extern volatile uint8_t keyOK_flag;
extern volatile uint8_t keyback_flag;
*/

extern Key_t keys[NUM_KEYS];

void Key_Init(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void Key_Process(Key_t *key);



#endif

