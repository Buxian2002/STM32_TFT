#ifndef __CHINSES_CHAR_24_H
#define __CHINSES_CHAR_24_H
#include <stdint.h>

enum refresh{
	Fresh = 0,
	NFresh，
};

enum display_status{
	DEFAULT = 0,
	SELECT,
	SELECTED,
	OFF,
	ON,
};


typedef struct
{
	unsigned int Current_Page;    
	unsigned int Last_Page;
	unsigned int Current_Option;
	unsigned int Last_Option;
}Status_Record_Def;


void display_char(unsigned int size,unsigned int x,unsigned int y,uint32_t color,
const unsigned char *point,unsigned int mode);
//void display_phrase(unsigned int display_mode,unsigned int x,unsigned int y,unsigned int phrase_len,
	//unsigned int display_status,const unsigned int *point);
//void display_phrase(unsigned int display_mode,unsigned int x,unsigned int y,unsigned int phrase_len,
	//unsigned int display_status,const unsigned int *point);
void display_phrase(unsigned int page_index,unsigned int phrase_index,unsigned int display_status,unsigned int refresh);

void page_Start(void);//开机界面
void page_SetorFunction(void);//主页面
void page_Setting(void);//设置
void page_Function(void);//功能
void page_Tail_Manual(void);//可动尾
void page_Tail_Auto(void);//可动尾
void page_Eye_Manual(void);//可动眼
void page_Eye_Auto(void);//可动眼
void page_Ear_Manual(void);//可动耳
void page_Ear_Auto(void);//可动耳
void page_Buzzer_Manual(void);//蜂鸣器
void page_Buzzer_Auto(void);//蜂鸣器

void Start_Change1_2(void);
void Start_Change2_1(void);


void PAGE_Display(int page_index);
void Freshen_Area(Status_Record_Def *Record_Struct);

/*
typedef struct
{
	unsigned int Page;    
	unsigned int Title_level;
	unsigned int Primary_row;
	unsigned int Secondary_row;
}Arrow;
extern Arrow Arrow_struct;
*/





#endif


