#include "led3.h" 
#include "stdlib.h"
#include "hardware_factory.h"

#define LED3_IO_ENABLE() IOPD->enable()
#define LED3_PIN IOPD->_2
#define LED3_INSTANCE led3

static LED *LED3_INSTANCE;

static void _LED3_Light(void)
{
    *LED3_PIN.W = 0;
}

static void _LED3_Close(void)
{
    *LED3_PIN.W = 1;
}

static void _LED3_Twinkle(void)
{
    *LED3_PIN.W = !(*LED3_PIN.W);
}

static LED* _LED3_Init(void)
{
    LED3_IO_ENABLE();
    GPIO_CONFIG_OUTPUT_50M(LED3_PIN);
    _LED3_Close();
    
    return LED3_INSTANCE;
}

static void _LED3_DeInit(void)
{
    _LED3_Close();
}

LED* LED3_Create(void)
{
    if(!LED3_INSTANCE){
        
        LED3_INSTANCE = (LED*)malloc(sizeof(LED));
        LED3_INSTANCE->init = _LED3_Init;
        LED3_INSTANCE->deInit = _LED3_DeInit;
        LED3_INSTANCE->light = _LED3_Light;
        LED3_INSTANCE->close = _LED3_Close;
        LED3_INSTANCE->twinkle = _LED3_Twinkle;
    }
    return LED3_INSTANCE;
}
void LED3_Destroy(void)
{
    free(LED3_INSTANCE);
    LED3_INSTANCE = 0;
}
