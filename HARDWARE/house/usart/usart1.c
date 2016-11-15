#include "usart1.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "hardware_factory.h"

#define USART1_INSTANCE usart1

static USART *USART1_INSTANCE;

__inline static void _USART1_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static USART* _USART1_Init(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    _USART1_GPIO_Init();

    USART_InitStructure.USART_BaudRate              = bound;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;
    USART_InitStructure.USART_Parity                = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                  = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE); 

    USART1_INSTANCE->bound = bound;
    
    return USART1_INSTANCE;
}

static void _USART1_DeInit(void)
{
}

static void _USART1_Print(const char *fmt, ...)
{
    char buf[32];
    u8 len     = 0;
    u8 i       = 0;
    u8 data    = 0;
    va_list args;
    va_start(args, fmt);
    len        = vsprintf(buf, fmt, args);
    va_end(args);
    
    while(len--)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        data = buf[i++];
        USART_SendData(USART1, data);
    }
   
}

static void _USART1_Write(u8 TXData)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, TXData);
}

USART* USART1_Create(void)
{
    if(!USART1_INSTANCE){
        USART1_INSTANCE = (USART*)malloc(sizeof(USART));
        USART1_INSTANCE->init = _USART1_Init;
        USART1_INSTANCE->deInit = _USART1_DeInit;
        USART1_INSTANCE->write = _USART1_Write;
        USART1_INSTANCE->print = _USART1_Print;
    }
    return USART1_INSTANCE;
}
void USART1_Destroy(void)
{
    _USART1_DeInit();
    free(USART1_INSTANCE);
    USART1_INSTANCE = 0;
}
