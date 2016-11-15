#include "idac1.h"
#include "stdlib.h"
#include "hardware_factory.h"

#define DAC_CMD_NOP                 0x00
#define DAC_CMD_WRITE_DATA_REG      0x01
#define DAC_CMD_READ                0x02
#define DAC_CMD_WRITE_CTRL_REG      0x55
#define DAC_CMD_WRITE_RESET_REG     0x56
#define DAC_CMD_WRITE_CONFIG_REG    0x57
#define DAC_CMD_WRITE_GAIN_REG      0x58
#define DAC_CMD_WRITE_ZERO_REG      0x59
#define DAC_CMD_WATCHDOG_RESET      0x95
#define DAC_ADR_STATUS_REG          0x00
#define DAC_ADR_DATA_REG            0x01
#define DAC_ADR_CTRL_REG            0x02
#define DAC_ADR_CONFIG_REG          0x0B
#define DAC_ADR_GAIN_REG            0x13
#define DAC_ADR_ZERO_REG            0x17

#define IDAC1_LATCH_ENABLE() IOPC->enable()
#define IDAC1_LATCH          IOPC->_10
#define IDAC1_INSTANCE       idac1

#define DELAY_TIME           0xff

static IDAC *IDAC1_INSTANCE;

static void delay(u16 count){
    while(count--);
}

__inline void _IDAC1_GPIO_Init(){
    IDAC1_LATCH_ENABLE();
    *IDAC1_LATCH.C1 = 0;
    *IDAC1_LATCH.C0 = 0;
    *IDAC1_LATCH.M1 = 1;
    *IDAC1_LATCH.M0 = 1;
    *IDAC1_LATCH.W  = 0;
}

static void _IDAC1_Reset(){
    IDAC1_INSTANCE->bundle_spi->write(DAC_CMD_WRITE_RESET_REG);
    IDAC1_INSTANCE->bundle_spi->write(DAC_CMD_NOP);
    IDAC1_INSTANCE->bundle_spi->write(0x01);
    delay(DELAY_TIME);
    *IDAC1_LATCH.W  = 1;
    delay(DELAY_TIME);
    *IDAC1_LATCH.W  = 0;
}

static IDAC* _IDAC1_Start(){
    IDAC1_INSTANCE->bundle_spi->write(DAC_CMD_WRITE_CTRL_REG);
    IDAC1_INSTANCE->bundle_spi->write(0x10);
    IDAC1_INSTANCE->bundle_spi->write(0x06);
    delay(DELAY_TIME);
    *IDAC1_LATCH.W  = 1;
    delay(DELAY_TIME);
    *IDAC1_LATCH.W  = 0;
    
    return IDAC1_INSTANCE;
}

static void _IDAC1_Stop(){
    IDAC1_INSTANCE->bundle_spi->write(DAC_CMD_WRITE_CTRL_REG);
    IDAC1_INSTANCE->bundle_spi->write(0x00);
    IDAC1_INSTANCE->bundle_spi->write(0x06);
    delay(DELAY_TIME);
    *IDAC1_LATCH.W  = 1;
    delay(DELAY_TIME);
    *IDAC1_LATCH.W  = 0;
}

static IDAC* _IDAC1_SetOutput(float mA){
    u16 TXData = ((u16)(mA / 20 * 0xFFF)) << 4;
    IDAC1_INSTANCE->bundle_spi->write(DAC_CMD_WRITE_DATA_REG);
    IDAC1_INSTANCE->bundle_spi->write(TXData >> 8);
    IDAC1_INSTANCE->bundle_spi->write(TXData);
    delay(DELAY_TIME);
    *IDAC1_LATCH.W  = 1;
    delay(DELAY_TIME);
    *IDAC1_LATCH.W  = 0;
    return IDAC1_INSTANCE;
}

static IDAC* _IDAC1_Init(SPI* spi){
    _IDAC1_GPIO_Init();
    IDAC1_INSTANCE->bundle_spi = spi;
    return IDAC1_INSTANCE;
}

static void _IDAC1_DeInit(){
    _IDAC1_Stop();
    _IDAC1_Reset();
}

IDAC* IDAC1_Create(void){   
    if(!IDAC1_INSTANCE){
        IDAC1_INSTANCE = (IDAC*) malloc(sizeof(IDAC));
        IDAC1_INSTANCE->no = IDAC1;
        IDAC1_INSTANCE->init = _IDAC1_Init;
        IDAC1_INSTANCE->deInit = _IDAC1_DeInit;
        IDAC1_INSTANCE->start = _IDAC1_Start;
        IDAC1_INSTANCE->stop = _IDAC1_Stop;
        IDAC1_INSTANCE->reset = _IDAC1_Reset;
        IDAC1_INSTANCE->setOutput = _IDAC1_SetOutput; 
    }
    return IDAC1_INSTANCE;
}

void IDAC1_Destroy(){
    _IDAC1_DeInit();
    free(IDAC1_INSTANCE);
    IDAC1_INSTANCE = 0;
}
