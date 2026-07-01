#ifndef __LED_H
#define __LED_H

enum status{
	LED_off,
	LED_1,
	LED_2,
};


extern int status_now1;

void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Turn(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED2_Turn(void);
void LED_status(int key_input);

#endif

