#include "led1.h" 
#include "stdlib.h"
#include "hardware_factory.h"

#define LED1_IO_ENABLE() IOPB->enable()
#define LED1_IO_DISABLE() IOPB->disable()
#define LED1_PIN IOPB->_7
#define LED1_INSTANCE led1

static LED *LED1_INSTANCE;

static void _LED1_Light(void)
{
    *LED1_PIN.W = 0;
}

static void _LED1_Close(void)
{
    *LED1_PIN.W = 1;
}

static void _LED1_Twinkle(void)
{
    *LED1_PIN.W = !(*LED1_PIN.W);
}

static LED* _LED1_Init(void)
{
    LED1_IO_ENABLE();
    GPIO_CONFIG_OUTPUT_50M(LED1_PIN);
    _LED1_Close();
    
    return LED1_INSTANCE;
}

static void _LED1_DeInit(void)
{
    _LED1_Close();
}

LED* LED1_Create(void)
{
    if(!LED1_INSTANCE){
        
        LED1_INSTANCE = (LED*)malloc(sizeof(LED));
        LED1_INSTANCE->init = _LED1_Init;
        LED1_INSTANCE->deInit = _LED1_DeInit;
        LED1_INSTANCE->light = _LED1_Light;
        LED1_INSTANCE->close = _LED1_Close;
        LED1_INSTANCE->twinkle = _LED1_Twinkle;
    }
    return LED1_INSTANCE;
}
void LED1_Destroy(void)
{
    free(LED1_INSTANCE);
    LED1_INSTANCE = 0;
}
