#ifndef __SPI_H
#define __SPI_H

#include "stdint.h"

#define SPI_SCK  GPIOA, GPIO_Pin_5    
#define SPI_SDA  GPIOA, GPIO_Pin_7      
#define SPI_RST  GPIOA, GPIO_Pin_6      
#define SPI_DC   GPIOA, GPIO_Pin_3    
#define SPI_CS   GPIOA, GPIO_Pin_4
#define SPI_BL   GPIOA, GPIO_Pin_2


void SPI1_Init(void);
void SPI1_WriteByte(uint8_t data);

void DMA_channel3_Init(void);
void DMA_StartLineTransfer(const uint8_t *data, uint16_t len);

#endif

