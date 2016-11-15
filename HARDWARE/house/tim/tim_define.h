#ifndef TIM_DEFINE_H_
#define TIM_DEFINE_H_

#include "stm32f10x.h"

typedef struct _TIM{
    TIM_TypeDef *no;
    u16 arr;
    u16 psc;
    struct _TIM* (*init)(u16 arr, u16 psc);
    void (*deInit)(void);
    void (*enable)(void);
    void (*disable)(void);
    void (*task)(void);
}TIM;

#endif /* TIM_DEFINE_H_ */
