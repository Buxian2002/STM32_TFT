#include "stm32f10x.h"
#include "Spi.h"

void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	DMA_DeInit(DMA1_Channel3);
	DMA_channel3_Init();
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
	
	//GPIO_SetBits(SPI_SCK);
	//GPIO_SetBits(SPI_SDA);
	GPIO_SetBits(SPI_RST);
	GPIO_SetBits(SPI_DC);
	GPIO_SetBits(SPI_CS);
	GPIO_ResetBits(SPI_BL);
	
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	
}

void DMA_channel3_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(SPI1->DR);  // 外设地址：SPI数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)0;            // 暂不设置，每次传输前更新
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;  // 内存到外设
    DMA_InitStructure.DMA_BufferSize            = 0;                     // 暂不设置，每次传输前设置
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable; // 外设地址不变
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;      // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte; // 8位
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;     // 8位
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;             // 普通模式
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                  = DMA_M2M_Disable;             // 内存到外设，非内存到内存
    
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);	
}


void SPI1_WriteByte(uint8_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); // 等待发送缓冲区空
    SPI_I2S_SendData(SPI1, data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);//事实证明SET这个是可取的
}


void DMA_StartLineTransfer(const uint8_t *data, uint16_t len)
{
    // 等待上一次传输完成
    DMA_ClearFlag(DMA1_FLAG_TC3);

    DMA_Cmd(DMA1_Channel3, DISABLE);
    DMA1_Channel3->CMAR = (uint32_t)data;
    DMA1_Channel3->CNDTR = len;
    DMA_Cmd(DMA1_Channel3, ENABLE);

    // 阻塞等待本次传输完成（简单可靠）
    while(DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC3);
}

