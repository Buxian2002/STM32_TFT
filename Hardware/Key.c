#include "Delay.h"
#include "Key.h"

Key_t keys[NUM_KEYS] = {
    {GPIOB, GPIO_Pin_6, KEY_IDLE, 0, 0, 0, 0},   // 0: PB6
    {GPIOB, GPIO_Pin_7, KEY_IDLE, 0, 0, 0, 0},   // 1: PB7
    {GPIOB, GPIO_Pin_8, KEY_IDLE, 0, 0, 0, 0},   // 2: PB8
    {GPIOB, GPIO_Pin_9, KEY_IDLE, 0, 0, 0, 0},   // 3: PB9
    {GPIOC, GPIO_Pin_11, KEY_IDLE, 0, 0, 0, 0},  // 4: PC11
    {GPIOC, GPIO_Pin_12, KEY_IDLE, 0, 0, 0, 0}   // 5: PC12
};


KeyState_t key_state;
/*
uint8_t short_press_event = 0;
uint8_t long_press_event = 0;

#define UP_KEY GPIOB, GPIO_Pin_6
#define DOWN_KEY GPIOB, GPIO_Pin_7
#define LEFT_KEY GPIOB, GPIO_Pin_8
#define RIGHT_KEY GPIOB, GPIO_Pin_9
#define OK_KEY GPIOC, GPIO_Pin_11
#define BACK_KEY GPIOC, GPIO_Pin_12
*/

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	/*GPIO初始化*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line12;
	EXTI_Init(&EXTI_InitStructure);
	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	SystemCoreClockUpdate();

}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line6);
		keys[UPWARD].pending = 1;
	}
	else if(EXTI_GetITStatus(EXTI_Line7) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line7);
		keys[DOWNWARD].pending = 1;
	}
	else if(EXTI_GetITStatus(EXTI_Line8) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line8);
		keys[LEFT].pending = 1;
	}
	else if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line9);
		keys[RIGHT].pending = 1;
	}
}


void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line11) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line11);
		keys[OK].pending = 1;
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line12);
		keys[BACK].pending = 1;
	}
}


void Key_Process(Key_t *key)
{

    switch (key->state)
    {
        case KEY_IDLE:
            if (key->pending)
            {
                key->pending = 0;
                key->state = KEY_DEBOUNCE;
                key->tick_start = sys_tick;
            }
            break;

        case KEY_DEBOUNCE:
            if (sys_tick - key->tick_start >= 20)   // 20ms 消抖
            {
                if (GPIO_ReadInputDataBit(key->port, key->pin) == 0) // 确认按下
                {
                    key->state = KEY_PRESSED;
                    key->tick_start = sys_tick;   // 重新计时用于长按
                }
                else
                {
                    key->state = KEY_IDLE; // 抖动，放弃
                }
            }
            break;

        case KEY_PRESSED:
            // 长按检测
            if (sys_tick - key->tick_start >= 1000)
            {
                key->long_press = 1;
                // 长按后可以保持状态，等待释放
            }
            // 检测释放（注意：这里不阻塞，只是判断电平）
            if (GPIO_ReadInputDataBit(key->port, key->pin) == 1)
            {
                key->state = KEY_RELEASE_DEBOUNCE;
                key->tick_start = sys_tick;
            }
            break;

        case KEY_RELEASE_DEBOUNCE:
            if (sys_tick - key->tick_start >= 20)
            {
                if (GPIO_ReadInputDataBit(key->port, key->pin) == 1)
                {
                    // 确认释放
                    if (!key->long_press)
                        key->short_press = 1;   // 短按
                    key->state = KEY_IDLE;
                }
                else
                {
                    key->state = KEY_PRESSED; // 抖动，回到按下
                }
            }
            break;
    }
}


