#include "stm32f10x.h"                  // Device header
#include "Spi.h"
#include "TFT.h"
#include "Key.h"
#include "Chinese_chars.h"
#include "Status_machine.h"


enum STATUS{
	PAGE_START_1,
	PAGE_START_2,
	PAGE_SETorFUNCTION_1,
	PAGE_SETorFUNCTION_2,
	PAGE_SETING1,
	PAGE_SETING1_1,
	PAGE_SETING1_2,
	PAGE_SETING1_3,
	PAGE_SETING2,
	PAGE_SETING2_1,
	PAGE_SETING2_2,
	PAGE_SETING2_3,
	PAGE_SETING3,
	PAGE_SETING3_1,
	PAGE_SETING3_2,
	PAGE_SETING3_3,
	PAGE_SETING4,
	PAGE_SETING4_1,
	PAGE_SETING4_2,
	PAGE_SETING4_3,
	PAGE_FUNCTION_TAIL,
	PAGE_FUNCTION_EYE,
	PAGE_FUNCTION_EAR,
	PAGE_FUNCTION_BUZZER,
	PAGE_FUNCTION_TAIL1,
	PAGE_FUNCTION_TAIL2,
	PAGE_FUNCTION_TAIL3,
	PAGE_FUNCTION_TAIL4,
	PAGE_FUNCTION_TAIL5,
	PAGE_FUNCTION_TAIL6,
	PAGE_FUNCTION_AutoTAIL,
	
};

Status_Record_Def Record_Struct = {0, 0, 0, 0};
/*
typedef struct
{
	unsigned int Current_Page;    
	unsigned int Last_Page;
	unsigned int Current_Option;
	unsigned int Last_Option;
}Status_Record_Def;
*/
int status_now = PAGE_START_1;
void display_status(int key_input)
{
	switch(status_now){
		case PAGE_START_1:
			if(key_input == button_down_short)
			{
				Record_Struct.Last_Option = Record_Struct.Current_Option;
				Record_Struct.Current_Option++;
				Freshen_Area(&Record_Struct);
				
				status_now = PAGE_START_2;
			}
			if(key_input == button_ok_short)
			{
				
				status_now = PAGE_SETorFUNCTION_1;
			}
			break;
		case PAGE_START_2:
			if(key_input == button_up_short)
			{
		
			}
			break;
		case PAGE_SETorFUNCTION_1:
			if(key_input == button_down_short)
			{
			
			}
			if(key_input == button_ok_short)
			{
				status_now = PAGE_SETING1;
			}
			if(key_input == button_back_short)
			{
				
				status_now = PAGE_START_1;
			}
			break;
		case PAGE_SETorFUNCTION_2:
			if(key_input == button_up_short)
			{
				
			}
			if(key_input == button_ok_short)
			{
				status_now = PAGE_FUNCTION_TAIL;
			}
			if(key_input == button_back_short)
			{
				
				status_now = PAGE_START_1;
			}
			break;
		/*case PAGE_START_1:
			if(key_input == button_back_short)
			{
				page1();
				status_now = PAGE_1_1;
			}
			break;
		case PAGE_2_2://5
			break;
		case PAGE_3_1://6
			break;
		case PAGE_3_2://7
			break;
		case PAGE_4_1://8
			break;
		case PAGE_4_2://9
			break;*/

	}
}






