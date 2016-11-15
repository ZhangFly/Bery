#include "led2.h" 	
#include "stdlib.h"
#include "hardware_factory.h"

#define LED2_IO_ENABLE() IOPB->enable()
#define LED2_PIN IOPB->_6
#define LED2_INSTANCE led2

static LED *LED2_INSTANCE;

static void _LED2_Light(void)
{
    *LED2_PIN.W = 0;
}

static void _LED2_Close(void)
{
    *LED2_PIN.W = 1;
}

static void _LED2_Twinkle(void)
{
    *LED2_PIN.W = !(*LED2_PIN.W);
}

static LED* _LED2_Init(void)
{
    LED2_IO_ENABLE();
    GPIO_CONFIG_OUTPUT_50M(LED2_PIN);
    _LED2_Close();
    return LED2_INSTANCE;
}

static void _LED2_DeInit(void)
{
    _LED2_Close();
}

LED* LED2_Create(void)
{
    if(!LED2_INSTANCE){
        LED2_INSTANCE = (LED*)malloc(sizeof(LED));
        LED2_INSTANCE->init = _LED2_Init;
        LED2_INSTANCE->deInit = _LED2_DeInit;
        LED2_INSTANCE->light = _LED2_Light;
        LED2_INSTANCE->close = _LED2_Close;
        LED2_INSTANCE->twinkle = _LED2_Twinkle;
    }
    return LED2_INSTANCE;
}
void LED2_Destroy(void)
{
    free(LED2_INSTANCE);
    LED2_INSTANCE = 0;
}
