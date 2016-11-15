#ifndef USART_DEFINE_H_
#define USART_DEFINE_H_ 

#include "stm32f10x.h"

typedef struct _USART {
    USART_TypeDef *no;
    u32 bound;
    struct _USART* (*init)(u32);
    void (*deInit)(void);
    u8 (*read)(void);
    void (*write)(u8);
    void (*print)(const char*, ...);
    void (*enable)(void);
    void (*disable)(void);
    void (*callback)(void);
}USART;

#endif /* USART_DEFINE_H_ */
