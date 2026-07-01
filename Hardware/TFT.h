#ifndef __TFT_H
#define __TFT_H

#include "stdint.h"
typedef struct
{
	const unsigned char* pic_point;
	unsigned int start_col;    
	unsigned int start_row;
	unsigned int length_width;
	unsigned int length_high;
	unsigned int pixel_num;
	
}Pic_SetDef;

typedef struct{
	int window_x;
	int window_y;
	int width;
	int high;
}Window_Def;

void TFT_SEND_CMD(unsigned char o_command);
void TFT_SEND_DATA(unsigned char o_data);
void window_set(Window_Def *Window_Struct);
void set_window(unsigned int x, unsigned int y, unsigned int width, unsigned int high);
void OLED_clear(void);
//void window_clear(Window_Def *Window_Struct);
void window_clear(int x, int y, int width, int high);
void Display_Image_start(void);
void DMA_Trans(const unsigned char *point, unsigned int pixel_num);
void Display_RGB888_Image(Pic_SetDef* Pic_SetStruct);
void OLED_init(void);


#endif

