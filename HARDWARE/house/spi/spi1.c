#include "spi1.h"
#include "stdlib.h"
#include "hardware_factory.h"

#define SPI1_NSS  GPIO_Pin_4   //PA
#define SPI1_SCK  GPIO_Pin_5   //PA
#define SPI1_MISO GPIO_Pin_6   //PA
#define SPI1_MOSI GPIO_Pin_7   //PA

#define SPI1_INSTANCE spi1

static SPI *SPI1_INSTANCE;

static void _SPI1_GPIO_Init(u16 SPI_Mode_x)
{
    GPIO_InitTypeDef GPIO_InitStructure;   
    GPIO_InitStructure.GPIO_Speed        = GPIO_Speed_50MHz;
  
    if(SPI_Mode_x == SPI_Mode_Master)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = SPI1_SCK | SPI1_MOSI;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin  = SPI1_MISO;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if(SPI_Mode_x == SPI_Mode_Slave)
    {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			           
        GPIO_InitStructure.GPIO_Pin  = SPI1_SCK | SPI1_MISO;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
			
        GPIO_InitStructure.GPIO_Pin  = SPI1_MOSI;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);    
		
        GPIO_InitStructure.GPIO_Pin  = SPI1_NSS;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure); 
    }
}

static void _SPI1_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel                   = SPI1_IRQn; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;            
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;   
    NVIC_Init(&NVIC_InitStructure);
}

static void _SPI1_Pause(void)
{
    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
}

static void _SPI1_Resume(void)
{
    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
}

static SPI* _SPI1_Init(u16 SPI_Mode_x)
{
    SPI_InitTypeDef  SPI_InitStructure;

	_SPI1_GPIO_Init(SPI_Mode_x);
	if(SPI_Mode_x == SPI_Mode_Slave) 
        _SPI1_NVIC_Init();                                        

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    if(SPI_Mode_x == SPI_Mode_Slave)
        SPI_InitStructure.SPI_NSS           = SPI_NSS_Hard;                    
	else
		SPI_InitStructure.SPI_NSS           = SPI_NSS_Soft;                     
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;		            
    SPI_InitStructure.SPI_Mode              = SPI_Mode_x;                       
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;    			
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;		                	
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;	                	
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;	                
	SPI_InitStructure.SPI_CRCPolynomial     = 7;                                               
    SPI_Init(SPI1, &SPI_InitStructure);                                         
    SPI_Cmd(SPI1, ENABLE);                                                   
	
    SPI1_INSTANCE->mode = SPI_Mode_x;

    if(SPI_Mode_x == SPI_Mode_Slave)
        _SPI1_Resume();	  

    return SPI1_INSTANCE;
} 

static void _SPI1_DeInit(void)
{
    _SPI1_Pause();
}

static u8 _SPI1_Read(void)
{		
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);             	
	return  SPI_I2S_ReceiveData(SPI1);                                          					    
}

static void _SPI1_Write(const u8 TxData)
{		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);          		  
	SPI_I2S_SendData(SPI1, TxData);                                             			    
}

static u8 _SPI1_ReadWrite(const u8 TxData)
{
	u16 delay = 0x2ff;
	_SPI1_Write(TxData);
	while(delay--);
    return _SPI1_Read();
}

void SPI1_IRQHandler(void)
{
    if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)
    {   
        if(SPI1_INSTANCE->callback)
            SPI1_INSTANCE->callback();
        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
    }
}

SPI* SPI1_Create(void)
{
    if(!SPI1_INSTANCE){
        SPI1_INSTANCE = (SPI*)malloc(sizeof(SPI));
        SPI1_INSTANCE->no = SPI1;
        SPI1_INSTANCE->init = _SPI1_Init;
        SPI1_INSTANCE->deInit = _SPI1_DeInit;
        SPI1_INSTANCE->read = _SPI1_Read;
        SPI1_INSTANCE->write = _SPI1_Write;
        SPI1_INSTANCE->readWrite = _SPI1_ReadWrite;
        SPI1_INSTANCE->pause = _SPI1_Pause;
        SPI1_INSTANCE->resume = _SPI1_Resume;
    }
    return SPI1_INSTANCE;
}

void SPI1_Destroy(void)
{
    _SPI1_DeInit();
    free(SPI1_INSTANCE);
    SPI1_INSTANCE = 0;
}


