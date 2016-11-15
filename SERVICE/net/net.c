#include "net.h"
#include "stdlib.h"
#include "sample.h"
#include "net_master.h"
#include "net_slave.h"
#include "hardware_factory.h"

#define NET_IO_ENABLE() {IOPA->enable(); IOPC->enable();}

void Net_Init(u16 SPI_Mode_x)
{
    if (SPI_Mode_x == SPI_Mode_Master)
    {
      NET_IO_ENABLE();
      Net_Master_Init();
    } 
    if (SPI_Mode_x == SPI_Mode_Slave)
    {
      Net_Slave_Init();
    }
}

Conn* Net_Create_Conn(MCUx_TypeDef MCUx)
{
    Conn* conn = Conn_Create();
    conn->target = MCUx;
    return conn;
}

void Net_Destroy_Conn(Conn *conn)
{
    Conn_Destroy(conn);
}
