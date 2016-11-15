#ifndef GPIOX_H_
#define GPIOX_H_

#include "stm32f10x.h" 
#include "gpiox_define.h"

GPIO_Group* GPIOx_Create(GPIO_TypeDef*);
void GPIOx_Destroy(GPIO_TypeDef*);

#endif /* GPIOX_H _ */
