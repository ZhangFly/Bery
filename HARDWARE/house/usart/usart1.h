#ifndef USART1_H_
#define USART1_H_

#include "stm32f10x.h"	
#include "usart_define.h"

USART* USART1_Create(void);
void USART1_Destroy(void);

#endif /* USART1_H_ */
