#include "hardware_factory.h"
#include "adc1.h"
#include "idac1.h"
#include "gpiox.h"
#include "tim2.h"
#include "tim3.h"
#include "spi1.h"
#include "spi2.h"
#include "usart1.h"
#include "led1.h"
#include "led2.h"
#include "led3.h"
#include "led4.h"

extern u8 Sys_Mode;

void* Hardware_Factory_Get(void *typeDef){
    switch((u32)typeDef)
    {
        case (u32)GPIOA: return GPIOx_Create(GPIOA);
        case (u32)GPIOB: return GPIOx_Create(GPIOB);
        case (u32)GPIOC: return GPIOx_Create(GPIOC);
        case (u32)GPIOD: return GPIOx_Create(GPIOD);
        case (u32)ADC1: return ADC1_Create();
        case (u32)IDAC1: return IDAC1_Create();
        case (u32)TIM2: return TIM2_Create();
        case (u32)TIM3: return TIM3_Create();
        case (u32)SPI1: return SPI1_Create();
        case (u32)SPI2: return SPI2_Create();
        case (u32)USART1: return USART1_Create();
        case (u32)LEDG: return Sys_Mode ? LED3_Create() : LED1_Create();
        case (u32)LEDR: return Sys_Mode ? LED4_Create() : LED2_Create();
    }
    return 0;
}

void Hardware_Factory_Release(void *typeDef){
    switch((u32)typeDef)
    {
        case (u32)GPIOA: GPIOx_Destroy(GPIOA);break;
        case (u32)GPIOB: GPIOx_Destroy(GPIOB);break;
        case (u32)GPIOC: GPIOx_Destroy(GPIOC);break;
        case (u32)GPIOD: GPIOx_Destroy(GPIOD);break;
        case (u32)ADC1: ADC1_Destroy();break;
        case (u32)IDAC1: IDAC1_Destroy();break;
        case (u32)TIM2: TIM2_Destroy();break;
        case (u32)TIM3: TIM3_Destroy();break;
        case (u32)SPI1: SPI1_Destroy();break;
        case (u32)SPI2: SPI2_Destroy();break;
        case (u32)USART1: USART1_Destroy();break;
        case (u32)LEDG: Sys_Mode ? LED3_Destroy() : LED1_Destroy();break;
        case (u32)LEDR: Sys_Mode ? LED4_Destroy() : LED2_Destroy();break;
    }
}
