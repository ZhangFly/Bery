#ifndef IDAC_DEFINE_H_
#define IDAC_DEFINE_H_

#include "stm32f10x.h"
#include "hardware_factory.h"

typedef u32 IDAC_TypeDef;

#define IDAC1 ((u32*)0x0102)            /* IDAC1注册标识符 */

typedef struct _IDAC {
    IDAC_TypeDef *no;                   /* IDAC编号 */
    SPI *bundle_spi;                    /* 绑定SPI */
    float ma;                           /* 当前输出电流 */
    struct _IDAC* (*init)(SPI*);        /* 初始化IDAC */
    void (*deInit)(void);               /* 反初始化IDAC */
    void (*reset)(void);                /* 重置IDAC */
    struct _IDAC* (*start)(void);       /* 开启IDAC */
    void (*stop)(void);                 /* 关闭IDAC */
    struct _IDAC* (*setOutput)(float);  /* 设置IDAC输出电流 */
}IDAC;

#endif /* IDAC_DEFINE_H_ */
