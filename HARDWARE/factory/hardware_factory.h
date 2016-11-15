#ifndef HARDWARE_FACTORY_H_
#define HARDWARE_FACTORY_H_

#include "spi_define.h"
#include "adc_define.h"
#include "idac_define.h"
#include "gpiox_define.h"
#include "tim_define.h"
#include "usart_define.h"
#include "led_define.h"

#define IOPA ((GPIO_Group*)Hardware_Factory_Get(GPIOA))
#define IOPB ((GPIO_Group*)Hardware_Factory_Get(GPIOB))
#define IOPC ((GPIO_Group*)Hardware_Factory_Get(GPIOC))
#define IOPD ((GPIO_Group*)Hardware_Factory_Get(GPIOD))

void* Hardware_Factory_Get(void *);
void Hardware_Factory_Release(void *);

#endif /* HARDWARE_FACTORY_H_ */
