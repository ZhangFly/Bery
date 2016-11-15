#ifndef NET_H_
#define NET_H_

#include "stm32f10x.h"
#include "conn.h"

#define NET_SOT  0xAAAA00AA   
#define NET_EOT  0xFFFF00FF
#define NET_NUL  0x00
#define NET_RDY  0xFE
#define NET_EOR  0xFD
#define NET_OK   0xFC
#define NET_BSY  0xFB

#include "conn.h"

typedef enum _Status{
    MCU_WAITING,
    MCU_WORKING,
    MCU_ERROR
}Status;

void Net_Init(u16 SPI_Mode_x);
Conn* Net_Create_Conn(MCUx_TypeDef MCUx);
void Net_Destroy_Conn(Conn *conn);

#endif /* NET_H_ */
