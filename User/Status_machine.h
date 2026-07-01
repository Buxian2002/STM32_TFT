#ifndef __STATUS_MACHINE_H
#define __STATUS_MACHINE_H

#include "stdint.h"
void display_status(int key_input);

extern const unsigned char chinese_16[][32];
extern const unsigned char chinese_24[][72];
extern const unsigned char chinese_32[][128];
extern const unsigned int two_characters[][2];
extern const unsigned int three_characters[][3];
extern const unsigned int four_characters[][4];
extern const unsigned char arrow_32[][128];



#endif

