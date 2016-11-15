#ifndef SPI_DEFINE_H_
#define SPI_DEFINE_H_

#include "stm32f10x.h"

typedef struct _SPI{
    SPI_TypeDef *no;
    u16 mode;
    struct _SPI* (*init)(u16);
    void (*deInit)(void);
    u8 (*read)(void);
    void (*write)(const u8 TXData);
    u8 (*readWrite)(const u8 TXData);
    void (*pause)(void);
    void (*resume)(void);
    void (*callback)(void);
}SPI;

#endif /* SPI_DEFINE_H_ */
