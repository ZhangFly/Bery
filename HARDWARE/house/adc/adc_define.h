#ifndef ADC_DEFINE_H_
#define ADC_DEFINE_H_

#include "stm32f10x.h"

typedef struct _ADC {
    ADC_TypeDef *no;                /* ADC编号 */
    struct _ADC* (*init)(void);     /* 初始化ADC */
    void (*deInit)(void);           /* 反初始化ADC */
    u16 (*read)(void);              /* 读取一次数据 */
}ADC;

#endif /* ADC_DEFINE_H_ */
