#include "stm32f10x.h" 
#include "net_master.h"
#include "net.h"
#include "hardware_factory.h"

static Status mStatus;
static SPI *spi_4_net_master;
static Buffer *net_buffer;
static LED *led_r,*led_g;

static void _Net_Master_In_Waiting(void)
{
    u8 RXData = 0;
    
    spi_4_net_master->readWrite((NET_SOT >> 24) & 0xFF);
    spi_4_net_master->readWrite((NET_SOT >> 16) & 0xFF);
    spi_4_net_master->readWrite((NET_SOT >> 8) & 0xFF);
    spi_4_net_master->readWrite((NET_SOT) & 0xFF);
    RXData = spi_4_net_master->readWrite(NET_NUL);
    
    if (RXData == NET_EOR)
    {
        mStatus = MCU_ERROR;
        return;
    }
    if (RXData == NET_RDY)
    {
        mStatus = MCU_WORKING;
    }else if (RXData != NET_BSY)
    {
        mStatus = MCU_ERROR;
    }
}

static void _Net_Master_In_Working(void)
{
    u32 RXData  = 0;
    u8 delay;
    led_r->light();
    
    while(1)
    {
        RXData                     <<= 8;
        spi_4_net_master->write(NET_NUL);
        delay = 0x4f;
        while(delay--);
        RXData                      |= spi_4_net_master->read();
        net_buffer->put(net_buffer, RXData & 0xFF);
        if (RXData == NET_EOT)
        {
            mStatus = MCU_WAITING;
            spi_4_net_master->readWrite((NET_EOT >> 24) & 0xFF);
            spi_4_net_master->readWrite((NET_EOT >> 16) & 0xFF);
            spi_4_net_master->readWrite((NET_EOT >> 8) & 0xFF);
            spi_4_net_master->readWrite((NET_EOT) & 0xFF);
            net_buffer->len -= 4;
            led_r->close();
            break;
        }
        if (RXData == 0xFDFDFDFD)
        {
            mStatus = MCU_ERROR;
            break;
        }
    }
}

static void _Net_Master_In_Error(void)
{
    led_g->close();
    net_buffer->reset(net_buffer); 
    spi_4_net_master->readWrite(NET_EOR);
    spi_4_net_master->readWrite((NET_EOT >> 24) & 0xFF);
    spi_4_net_master->readWrite((NET_EOT >> 16) & 0xFF);
    spi_4_net_master->readWrite((NET_EOT >> 8) & 0xFF);
    spi_4_net_master->readWrite((NET_EOT) & 0xFF);
    if(spi_4_net_master->readWrite(NET_NUL) == NET_OK){
        mStatus = MCU_WAITING;
        led_g->light();
    }
}

void Net_Master_Read(Buffer *buffer)
{
    net_buffer = buffer;    
    
    if(mStatus == MCU_WAITING)
    {
        _Net_Master_In_Waiting();
    }
    if(mStatus == MCU_WORKING)
    {
        _Net_Master_In_Working();
    }
    if(mStatus == MCU_ERROR)
    {
        _Net_Master_In_Error();
    }
}

void Net_Master_Init(void)
{
    mStatus = MCU_WAITING;
    spi_4_net_master = Hardware_Factory_Get(SPI1);
    spi_4_net_master->init(SPI_Mode_Master);
    led_g = Hardware_Factory_Get(LEDG);
    led_g->init();
    led_g->light();
    led_r = Hardware_Factory_Get(LEDR);
    led_r->init();
}

void Net_Master_DeInit(void)
{ 
    Hardware_Factory_Release(SPI1);
}
