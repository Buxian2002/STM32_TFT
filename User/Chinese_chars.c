#include "stm32f10x.h"
#include "Key.h"
#include "TFT.h"
#include "Spi.h"
#include "Word_stock.h"
#include "Chinese_chars.h"

int max_page = 7;

uint32_t color_default = 0X60589E;
uint32_t color_select = 0XA098CE;
uint32_t color_off = 0X40387E;

enum Pages{
	Page_Start = 0,
	Page_SetorFunction,
	Page_Set,
	Page_Function,
	Page_Tail,
	Page_Eye,
	Page_Ear,	
};



//int page_max[7] = {2,2,4,4,};

/*
Arrow Arrow_struct;

void Arrow_Init(void)
{
	Arrow_struct.Page = 0;
	Arrow_struct.Title_level = 0;
	Arrow_struct.Primary_row = 0;
	Arrow_struct.Secondary_row = 0;
		
}

void arrow_next_page(void)
{
	Arrow_struct.Page++;
}

void arrow_last_page(Arrow *arrow_struct)
{
	Arrow_struct.Page--;
}

void arrow_next_level(Arrow *arrow_struct)
{
	Arrow_struct.Title_level = 1;
	Arrow_struct.Secondary_row = 0;
}

void arrow_last_level(Arrow *arrow_struct)
{
	Arrow_struct.Title_level = 0;
}

void arrow_up(Arrow *arrow_struct)
{
	if(arrow_struct -> Title_level){
		if(arrow_struct -> Secondary_row < page_max[arrow_struct -> Page])
			arrow_struct -> Secondary_row++;
	}
	else{
		arrow_struct -> Primary_row++;
	}
}

void arrow_down(Arrow *arrow_struct)
{
	if(arrow_struct -> Title_level){
		arrow_struct -> Secondary_row--;
	}
	else
		arrow_struct -> Primary_row--;
}
*/

void display_char(unsigned int size,unsigned int x,unsigned int y,uint32_t color,
const unsigned char *point,unsigned int mode)
{
	unsigned int column, size_length = 0;
	unsigned char tm=0,temp;
	size_length = (size/4)*(size/4)*8;
	set_window(x, y, size, size);

    
	if(mode){		
		for(column=0; column<size_length; column++)	//column loop
		{
			temp = *point;
			for(tm=0; tm<8; tm++)
			{
				if(temp&0x01){
					TFT_SEND_DATA(color>>16);
					TFT_SEND_DATA(color>>8);
					TFT_SEND_DATA(color);				
				}
				else{
					TFT_SEND_DATA(0);
					TFT_SEND_DATA(0);
					TFT_SEND_DATA(0);
				}
				temp>>=1;
			}
			point++;          
		}
	}
	else{
		for(column=0; column<size_length; column++)	//column loop
		{
			temp = *point;
			for(tm=0; tm<8; tm++)
			{
				if(temp&0x01){
					TFT_SEND_DATA(0);
					TFT_SEND_DATA(0);
					TFT_SEND_DATA(0);					
				}
				else{
					TFT_SEND_DATA(color>>16);
					TFT_SEND_DATA(color>>8);
					TFT_SEND_DATA(color);
				}
				temp>>=1;
			}
			point++;          
		}
	}
}

//display_phrase其实是一个个词组的打印，我如果需要做结构体的话，得放该词组的基准x、y、词组长度（字符个数）、存储字符的数组地址
//居中模式下的x是取词组最中间的那个像素作基准x，左对齐模式是取词组最左的像素作为基准x
/*
void display_phrase(unsigned int display_mode, unsigned int x, unsigned int y, unsigned int phrase_len, 
	unsigned int display_status, const unsigned int *point)
{
	int start_x, start_y, mode, i, size_use = 24;
	unsigned long color;
	int arrow_x_ALIGN_CENTER = x-phrase_len*20-36;
	int arrow_x_FLUSH_LEFT = x-40;
	int arrow_y = y-16;
	int clean_length = phrase_len*48+40;
	
	
	if(display_mode == FRESHEN_CENTER)
		window_clear(arrow_x_ALIGN_CENTER, arrow_y, clean_length, 32);
	else if(display_mode == FRESHEN_LEFT)
		window_clear(arrow_x_FLUSH_LEFT, arrow_y, clean_length, 32);

	
	switch(display_status){
		case DEFAULT:
			mode = 1;
			color = color_default;
			break;
		case SELECT:
			mode = 1;
			color = color_select;
			size_use = 32;
			if(display_status == ALIGN_CENTER | display_mode == FRESHEN_CENTER)
				display_char(32, arrow_x_ALIGN_CENTER, arrow_y, color, arrow_32[2], mode);
			else if(display_mode == FLUSH_LEFT | display_mode == FRESHEN_LEFT)
				display_char(32, arrow_x_FLUSH_LEFT, arrow_y, color, arrow_32[2], mode);
			break;
		case SELECTED:
			mode = 1;
			color = color_select;
			break;
		case OFF:
			mode = 1;
			color = color_off;
			break;
		case ON:
			mode = 0;
			color = color_default;
			break;
	}
	
	if(display_mode == ALIGN_CENTER | display_mode == FRESHEN_CENTER)
		start_x = x - phrase_len * size_use / 2 - (phrase_len - 1)* 4;
	else if(display_mode == FLUSH_LEFT | display_mode == FRESHEN_LEFT)
		start_x = x;
	start_y = y - size_use / 2;
	
	switch(size_use){
		case 24:
			for(i = 0; i < phrase_len; i++){
				display_char(size_use, start_x, start_y, color, chinese_24[point[i]], mode);
				start_x = start_x + size_use + 8;
			}
			break;
		case 32:
			for(i = 0; i < phrase_len; i++){
				display_char(size_use, start_x, start_y, color, chinese_32[point[i]], mode);
				start_x = start_x + size_use + 8;
			}
			break;
		}
}
*/
void display_phrase(unsigned int page_index,unsigned int phrase_index,unsigned int display_status,unsigned int refresh)
{
	const Display_StoreDef *phrase =Phrase_Struct[page_index][phrase_index];
	int arrow_x, arrow_y, start_x, start_y, total_width;//_ALIGN_CENTER = phrase->reference_x - phrase->phrase_len*20-36;
	//int arrow_x_FLUSH_LEFT = phrase->reference_x - 40;
	arrow_y = phrase->reference_y - 16;
	if(phrase->reference_x == ALIGN_CENTER)
		arrow_x = phrase->reference_x - phrase->phrase_len*20-36;
	else 
		arrow_x = phrase->reference_x - 40;
	
	total_width = 40 * phrase->phrase_len + 32;
	if(refresh == Fresh)
		window_clear(arrow_x, arrow_y, total_width, 32);
	
	
}

void PAGE_Display(int page_index)
{
	int i;
	OLED_clear();
	display_phrase(ALIGN_CENTER,x[page_index],y_array[page_index][0],char_num[page_index][0], 
	SELECT, char_index[page_index][0]);
	for(i=1; i< char_num[page_index][i]; i++)
	{
		display_phrase(ALIGN_CENTER,x[page_index],y_array[page_index][i],char_num[page_index][i], 
		DEFAULT, char_index[page_index][i]);
	}
}

void Freshen_Area(Status_Record_Def *Record_Struct)
{
	display_phrase(FRESHEN_CENTER, x[Record_Struct->Current_Page],
	y_array[Record_Struct->Current_Page][Record_Struct->Current_Option],
	char_num[Record_Struct->Current_Page][Record_Struct->Current_Option],
	SELECT, char_index[Record_Struct->Current_Page][Record_Struct->Current_Option]);
	
	display_phrase(FRESHEN_CENTER, x[Record_Struct->Current_Page],
	y_array[Record_Struct->Current_Page][Record_Struct->Last_Option],
	char_num[Record_Struct->Current_Page][Record_Struct->Last_Option],
	SELECT, char_index[Record_Struct->Current_Page][Record_Struct->Last_Option]);
}
	
void page_Start(void)//开机界面
{
	PAGE_Display(Page_Start);
	//Display_Image_start();
}

/*
void page_Start(void)//开机界面
{
	OLED_clear();
	Display_Image_start();
	display_phrase(ALIGN_CENTER,250,96,2,SELECT,two_characters[0],0);
	display_phrase(ALIGN_CENTER,250,144,2,DEFAULT,two_characters[1],0);
}

void Start_Change1_2(void)
{
	display_phrase(FRESHEN_CENTER,250,96,2,DEFAULT,two_characters[0],0);
	display_phrase(FRESHEN_CENTER,250,144,2,SELECT,two_characters[1],0);
}

void Start_Change2_1(void)
{
	display_phrase(FRESHEN_CENTER,250,96,2,SELECT,two_characters[0],0);
	display_phrase(FRESHEN_CENTER,250,144,2,DEFAULT,two_characters[1],0);
}

void page_SetorFunction(void)//主页面？
{
	OLED_clear();
	display_phrase(ALIGN_CENTER,159,95,2,SELECT,two_characters[2],0);
	display_phrase(ALIGN_CENTER,159,143,2,DEFAULT,two_characters[3],0);
}

void SoF_Change1_2(void)
{
	display_phrase(FRESHEN_CENTER,159,95,2,DEFAULT,two_characters[2],0);
	display_phrase(FRESHEN_CENTER,159,143,2,SELECT,two_characters[3],0);
}

void SoF_Change2_1(void)
{
	display_phrase(FRESHEN_CENTER,159,95,2,SELECT,two_characters[2],0);
	display_phrase(FRESHEN_CENTER,159,143,2,DEFAULT,two_characters[3],0);
}

void page_Setting(void)//设置
{
	OLED_clear();
	display_phrase(ALIGN_CENTER,159,59,3,SELECT,three_characters[0],0);
	display_phrase(ALIGN_CENTER,159,99,3,DEFAULT,three_characters[1],0);
	display_phrase(ALIGN_CENTER,159,139,3,DEFAULT,three_characters[2],0);
	display_phrase(ALIGN_CENTER,159,179,2,DEFAULT,two_characters[4],0);
}
*/
void Setting_Change1_2(void)
{}
void Setting_Change2_1(void)
{}
void Setting_Change2_3(void)
{}
void Setting_Change3_2(void)
{}
void Setting_Change3_4(void)
{}
void Setting_Change4_3(void)
{}
/*
void page_Function(void)//功能
{
	OLED_clear();
	display_phrase(ALIGN_CENTER,159,59,3,SELECT,three_characters[0],0);
	display_phrase(ALIGN_CENTER,159,99,3,DEFAULT,three_characters[1],0);
	display_phrase(ALIGN_CENTER,159,139,3,DEFAULT,three_characters[2],0);
	display_phrase(ALIGN_CENTER,159,179,3,DEFAULT,three_characters[3],0);
}
*/

void page_Tail_Manual(void)//可动尾（手动
{
	
}
void page_Tail_Auto(void)//可动尾（自动
{}
void page_Eye_Manual(void)//可动眼（手动
{}
void page_Eye_Auto(void)//可动眼（自动
{}
void page_Ear_Manual(void)//可动耳（手动
{}
void page_Ear_Auto(void)//可动耳（自动
{}
void page_Buzzer_Manual(void)//蜂鸣器
{}
void page_Buzzer_Auto(void)//蜂鸣器
{

}
		



	