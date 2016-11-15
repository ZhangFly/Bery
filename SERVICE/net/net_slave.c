#include "net_slave.h"
#include "net.h"
#include "buffer.h"
#include "sample.h"
#include "hardware_factory.h"

static Status sStatus; 
static SPI *spi_4_net_slave;
static Buffer *sample_buffer;
static u32 RXData     = 0;
static Iterator *iter = 0;
static LED *led_g,*led_r;

static void _Net_Slave_In_Waiting(void)
{
    static u8 retry = 0x1f;
    static u8 time_out = 0x1f;
    /* 等待接受到通讯开始标志 */
    if (RXData != NET_SOT)
    {
        if(!time_out--){
            time_out = 0x1f;
            sStatus = MCU_ERROR;
        }
        return;
    }
    time_out = 0x1f;
    /* 查询扫描是否结束 */
    sample_buffer = Sample_Buffer_Get();
    if(sample_buffer)
    {
        Sample_Pause();
        iter = sample_buffer->iterator(sample_buffer);
        sStatus = MCU_WORKING;
        led_g->light();
        spi_4_net_slave->write(NET_RDY);
    }else
    {
        /* 等待扫描结束 */
        if(!retry--){
            retry = 0x1f;
            sStatus = MCU_ERROR;
        }else
            spi_4_net_slave->write(NET_BSY);
    }
}

static void _Net_Slave_In_Working(void)
{
    static u8 over_flow = 0;
    /* 判断通讯是否结束 */
    if (RXData == NET_EOT)
    {
        over_flow = 0;
        Iterator_Destroy(iter);
        sStatus = MCU_WAITING;
        sample_buffer->reset(sample_buffer);
        led_g->close();
        Sample_Resume();
    }else
    {
        if(iter->hasNext(iter))
        {
          spi_4_net_slave->write(iter->next(iter));
        }else if (over_flow == 2)
        {
          over_flow++;
          spi_4_net_slave->write(0x00);
        }else
        {
          over_flow++;
          spi_4_net_slave->write(0xFF);
        }
        if (over_flow >= 10)
        {
          sStatus = MCU_ERROR;
        }
    }
}

static void _Net_Slave_In_Error(void)
{
    led_r->close();
    if (RXData == NET_EOT)
    {
        sStatus = MCU_WAITING;
        spi_4_net_slave->write(NET_OK);
        if(sample_buffer)
            sample_buffer->reset(sample_buffer);
        led_r->light();
        Sample_Resume();
    }else{
        spi_4_net_slave->write(NET_EOR);
    }
}

static void _Net_Slave_Task(void)
{
    RXData <<= 8;
    RXData |= spi_4_net_slave->read();

    if ((RXData & 0xFF) == NET_EOR)
    {
        sStatus = MCU_ERROR;
    } 

    if (sStatus == MCU_WAITING)
    {
        _Net_Slave_In_Waiting();
    }
    if (sStatus == MCU_WORKING)
    {
        _Net_Slave_In_Working();
    }
    if (sStatus == MCU_ERROR)
    {
        _Net_Slave_In_Error();
    }    
}

void Net_Slave_Init(void)
{
    sStatus = MCU_WAITING;
    spi_4_net_slave = Hardware_Factory_Get(SPI1);
    spi_4_net_slave->callback = _Net_Slave_Task;
    spi_4_net_slave->init(SPI_Mode_Slave);
    led_g = ((LED*)Hardware_Factory_Get(LEDG));
    led_g->init();
    led_r = ((LED*)Hardware_Factory_Get(LEDR));
    led_r->init();
    led_r->light();
}

void Net_Slave_DeInit(void)
{ 
    Hardware_Factory_Release(SPI1);
}
