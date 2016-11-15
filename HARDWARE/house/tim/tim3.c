#include "tim3.h"
#include "stdlib.h"

#define TIM2_INSTANCE tim3

static TIM *TIM3_INSTANCE;

static TIM* _TIM3_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);      
	TIM_TimeBaseStructure.TIM_Period                        = arr;                 
	TIM_TimeBaseStructure.TIM_Prescaler                     = psc;                    
	TIM_TimeBaseStructure.TIM_ClockDivision                 = 0;                    
	TIM_TimeBaseStructure.TIM_CounterMode                   = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 
	TIM_ITConfig(TIM3, TIM_IT_Update ,ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);       
    NVIC_InitStructure.NVIC_IRQChannel                      = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 1;       
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 3;              
	NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;               
	NVIC_Init(&NVIC_InitStructure);                               
	
    TIM3_INSTANCE->arr = arr;
    TIM3_INSTANCE->psc = psc;    
        
    return TIM3_INSTANCE;
}

static void _TIM3_DeInit(void)
{
    TIM_DeInit(TIM3);
}

static void _TIM3_Enable(void)
{
    TIM_Cmd(TIM3, ENABLE);
}

static void _TIM3_Disable(void)
{
    TIM_Cmd(TIM3, DISABLE);
}

void TIM3_IRQHandler(void)                                          
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)             
	{
        if(TIM3_INSTANCE->task)
            TIM3_INSTANCE->task();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);                 
	}
}

TIM* TIM3_Create(void)
{
    if(!TIM3_INSTANCE)
    {
        TIM3_INSTANCE = (TIM*) malloc(sizeof(TIM));
        TIM3_INSTANCE->no = TIM3;
        TIM3_INSTANCE->init = _TIM3_Init;
        TIM3_INSTANCE->deInit = _TIM3_DeInit;
        TIM3_INSTANCE->enable = _TIM3_Enable;
        TIM3_INSTANCE->disable = _TIM3_Disable;
    }
    return TIM3_INSTANCE;
}

void TIM3_Destroy(void)
{
    _TIM3_DeInit();
    free(TIM3_INSTANCE);
    TIM3_INSTANCE = 0;
}
