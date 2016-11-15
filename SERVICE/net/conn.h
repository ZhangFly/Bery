#ifndef CONN_H_
#define CONN_H_

#include "stm32f10x.h"
#include "buffer.h"

typedef enum _MCUx_TypeDef {MCU0 = 0,MCU1,MCU2,MCU3,MCU4,MCU5,MCU6,MCU7} MCUx_TypeDef;

typedef struct _Conn {
    MCUx_TypeDef target;
    void (*read)(struct _Conn*, Buffer *);
    void (*write)(struct _Conn*, Buffer *);
}Conn;

Conn* Conn_Create(void);
void Conn_Destroy(Conn* thiz);

#endif /* CONN_H_ */
