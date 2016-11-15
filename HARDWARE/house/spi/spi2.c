#include "spi2.h"
#include "stdlib.h"
#include "hardware_factory.h"

#define SPI2_NSS  GPIO_Pin_12  //PA
#define SPI2_SCK  GPIO_Pin_13  //PB
#define SPI2_MISO GPIO_Pin_14  //PB
#define SPI2_MOSI GPIO_Pin_15  //PB

#define SPI2_INSTANCE spi2

static SPI *SPI2_INSTANCE;

static void _SPI2_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin   = SPI2_SCK | SPI2_MOSI;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = SPI2_MISO;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static SPI* _SPI2_Init(u16 SPI_Mode_x)
{
    SPI_InitTypeDef  SPI_InitStructure;

	_SPI2_GPIO_Init();             

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                     
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;		            
    SPI_InitStructure.SPI_Mode              = SPI_Mode_x;                       
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;    			
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;		                	
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;	                	
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;	                
	SPI_InitStructure.SPI_CRCPolynomial     = 7;                                               
    SPI_Init(SPI2, &SPI_InitStructure);                                         
    SPI_Cmd(SPI2, ENABLE);                                                   

    SPI2_INSTANCE->mode = SPI_Mode_x;	 

    return SPI2_INSTANCE;
} 

static void _SPI2_DeInit(void)
{
}

static u8 _SPI2_Read(void)
{		
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);             
	return  SPI_I2S_ReceiveData(SPI2);                                          					    
}

static void _SPI2_Write(const u8 TxData)
{		
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);             			  
	SPI_I2S_SendData(SPI2, TxData);                                           		    
}

static u8 _SPI2_ReadWrite(const u8 TxData)
{
	u8 delay = 0xff;
	_SPI2_Write(TxData);
	while(delay--);
    return _SPI2_Read();
}

SPI* SPI2_Create(void)
{
    if(!SPI2_INSTANCE){
        SPI2_INSTANCE = (SPI*)malloc(sizeof(SPI));
        SPI2_INSTANCE->no = SPI2;
        SPI2_INSTANCE->init = _SPI2_Init;
        SPI2_INSTANCE->deInit = _SPI2_DeInit;
        SPI2_INSTANCE->read = _SPI2_Read;
        SPI2_INSTANCE->write = _SPI2_Write;
        SPI2_INSTANCE->readWrite = _SPI2_ReadWrite;
    }
    return SPI2_INSTANCE;
}

void SPI2_Destroy(void)
{
    _SPI2_DeInit();
    free(SPI2_INSTANCE);
    SPI2_INSTANCE = 0;
}
