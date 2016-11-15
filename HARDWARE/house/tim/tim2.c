#include "tim2.h"
#include "stdlib.h"

#define TIM2_INSTANCE tim2

static TIM *TIM2_INSTANCE;

static TIM* _TIM2_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    
	TIM_TimeBaseStructure.TIM_Period                        = arr;                 
	TIM_TimeBaseStructure.TIM_Prescaler                     = psc;                    
	TIM_TimeBaseStructure.TIM_ClockDivision                 = 0;                    
	TIM_TimeBaseStructure.TIM_CounterMode                   = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                 
	TIM_ITConfig(TIM2, TIM_IT_Update ,ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);       
    NVIC_InitStructure.NVIC_IRQChannel                      = TIM2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 1;       
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 3;              
	NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;               
	NVIC_Init(&NVIC_InitStructure);  

    TIM2_INSTANCE->arr = arr;
    TIM2_INSTANCE->psc = psc;   

    return TIM2_INSTANCE;
}

static void _TIM2_DeInit(void)
{
    TIM_DeInit(TIM2);
}

static void _TIM2_Enable(void)
{
    TIM_Cmd(TIM2, ENABLE);
}

static void _TIM2_Disable(void)
{
    TIM_Cmd(TIM2, DISABLE);
}

void TIM2_IRQHandler(void)                                         
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)            
	{
        if(TIM2_INSTANCE->task)
            TIM2_INSTANCE->task();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                
	}
}

TIM* TIM2_Create(void)
{
    if(!TIM2_INSTANCE)
    {
        TIM2_INSTANCE = (TIM*) malloc(sizeof(TIM));
        TIM2_INSTANCE->no = TIM2;
        TIM2_INSTANCE->init = _TIM2_Init;
        TIM2_INSTANCE->deInit = _TIM2_DeInit;
        TIM2_INSTANCE->enable = _TIM2_Enable;
        TIM2_INSTANCE->disable = _TIM2_Disable;
    }
    return TIM2_INSTANCE;
}

void TIM2_Destroy(void)
{
    _TIM2_DeInit();
    free(TIM2_INSTANCE);
    TIM2_INSTANCE = 0;
}
