#include "led4.h" 	
#include "stdlib.h"
#include "hardware_factory.h"

#define LED4_IO_ENABLE() IOPA->enable()
#define LED4_PIN IOPA->_8
#define LED4_INSTANCE led4

static LED *LED4_INSTANCE;

static void _LED4_Light(void)
{
    *LED4_PIN.W = 0;
}

static void _LED4_Close(void)
{
    *LED4_PIN.W = 1;
}

static void _LED4_Twinkle(void)
{
    *LED4_PIN.W = !(*LED4_PIN.W);
}

static LED* _LED4_Init(void)
{
    LED4_IO_ENABLE();
    GPIO_CONFIG_OUTPUT_50M(LED4_PIN);
    _LED4_Close();
    return LED4_INSTANCE;
}

static void _LED4_DeInit(void)
{
    _LED4_Close();
}

LED* LED4_Create(void)
{
    if(!LED4_INSTANCE){
        LED4_INSTANCE = (LED*)malloc(sizeof(LED));
        LED4_INSTANCE->init = _LED4_Init;
        LED4_INSTANCE->deInit = _LED4_DeInit;
        LED4_INSTANCE->light = _LED4_Light;
        LED4_INSTANCE->close = _LED4_Close;
        LED4_INSTANCE->twinkle = _LED4_Twinkle;
    }
    return LED4_INSTANCE;
}
void LED4_Destroy(void)
{
    free(LED4_INSTANCE);
    LED4_INSTANCE = 0;
}
