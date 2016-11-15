#include "sensor.h"
#include "hardware_factory.h"

#define SENSOR_IO_ENABLE() {IOPA->enable(); IOPB->enable(); IOPC->enable();}

#define SENSOR_CS0     IOPC->_5   //PC5
#define SENSOR_CS1     IOPB->_0   //PB0
#define SENSOR_CS2     IOPC->_8   //PC8
#define SENSOR_CS3     IOPA->_12  //PA12
#define SENSOR_CS4     IOPA->_10  //PA10
#define SENSOR_CS5     IOPA->_8   //PA8

#define SENSOR_ADDR0   IOPB->_12  //PB12
#define SENSOR_ADDR1   IOPB->_11  //PB11
#define SENSOR_ADDR2   IOPB->_10  //PB10
#define SENSOR_ADDR3   IOPB->_1   //PB1
#define SENSOR_ADDR4   IOPA->_11  //PA11
#define SENSOR_ADDR5   IOPA->_9   //PA9
#define SENSOR_ADDR6   IOPC->_9   //PC9
#define SENSOR_ADDR7   IOPC->_7   //PC7

u16 Sensor_Get_Code(void){
    u8 cs0,cs1,cs2,cs3,cs4,cs5,addr0,addr1,addr2,addr3,addr4,addr5,addr6,addr7;

    cs0 = *SENSOR_CS0.W;
    cs1 = *SENSOR_CS1.W;
    cs2 = *SENSOR_CS2.W;
    cs3 = *SENSOR_CS3.W;
    cs4 = *SENSOR_CS4.W;
    cs5 = *SENSOR_CS5.W;
    addr0 = *SENSOR_ADDR0.W;
    addr1 = *SENSOR_ADDR1.W;
    addr2 = *SENSOR_ADDR2.W;
    addr3 = *SENSOR_ADDR3.W;
    addr4 = *SENSOR_ADDR4.W;
    addr5 = *SENSOR_ADDR5.W;
    addr6 = *SENSOR_ADDR6.W;
    addr7 = *SENSOR_ADDR7.W;
	
    return cs0 << 13 | cs1 << 12 | cs2 << 11 | cs3 << 10 | cs4 << 9 | cs5 << 8 | addr0 << 7 | addr1 << 6 | addr2 << 5 | addr3 << 4 | addr4 << 3 | addr5 << 2 | addr6 << 1 | addr7;
}

void Sensor_Init(void)
{
    SENSOR_IO_ENABLE();
	
    GPIO_CONFIG_OUTPUT_50M(SENSOR_CS0);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_CS1);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_CS2);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_CS3);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_CS4);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_CS5);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_ADDR0);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_ADDR1);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_ADDR2);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_ADDR3);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_ADDR4);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_ADDR5);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_ADDR6);
    GPIO_CONFIG_OUTPUT_50M(SENSOR_ADDR7);
}

static void _Sensor_Code_X_ADDR(const u8 x)
{
    *SENSOR_ADDR0.W = x & 0x1 ? 1 : 0;
    *SENSOR_ADDR1.W = x & 0x2 ? 1 : 0;
    *SENSOR_ADDR2.W = x & 0x4 ? 1 : 0;
    *SENSOR_ADDR3.W = x & 0x8 ? 1 : 0; 
}

static void _Sensor_Code_Y_ADDR(const u8 y)
{
    *SENSOR_ADDR4.W = y & 0x1 ? 1 : 0;
    *SENSOR_ADDR5.W = y & 0x2 ? 1 : 0;
    *SENSOR_ADDR6.W = y & 0x4 ? 1 : 0;
    *SENSOR_ADDR7.W = y & 0x8 ? 1 : 0;
}

void Sensor_Select_X(const u8 x)
{
    if(x < 16){     
        _Sensor_Code_X_ADDR(x);
        *SENSOR_CS0.W = 0;
        *SENSOR_CS1.W = 1;
        *SENSOR_CS2.W = 1;
    }else if(x < 32){     
        _Sensor_Code_X_ADDR(x);
        *SENSOR_CS0.W = 1;
        *SENSOR_CS1.W = 0;
        *SENSOR_CS2.W = 1;
    }else if(x < 48){
        _Sensor_Code_X_ADDR(x);
        *SENSOR_CS0.W = 1;
        *SENSOR_CS1.W = 1;
        *SENSOR_CS2.W = 0;
    }
}

void Sensor_Select_Y(const u8 y)
{
    if(y < 16){
        _Sensor_Code_Y_ADDR(y);
        *SENSOR_CS3.W = 0;
        *SENSOR_CS4.W = 1;
        *SENSOR_CS5.W = 1;
    }else if(y < 32){
        _Sensor_Code_Y_ADDR(y);
        *SENSOR_CS3.W = 1;
        *SENSOR_CS4.W = 0;
        *SENSOR_CS5.W = 1;
    }else if(y < 48){
        _Sensor_Code_Y_ADDR(y);
        *SENSOR_CS3.W = 1;
        *SENSOR_CS4.W = 1;
        *SENSOR_CS5.W = 0;
    }
}

void Sensor_Select(const u8 x, const u8 y)
{
    Sensor_Select_X(x);
    Sensor_Select_Y(y);
}

Scan* Sensor_Create_Scan(void)
{
    return Scan_Create();
}

void Sensor_Destroy_Scan(Scan* scan)
{
    Scan_Destroy(scan);
}
