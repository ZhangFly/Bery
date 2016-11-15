#include "transmit.h"
#include "hardware_factory.h"

static USART* usart_4_tansmit;

void Transmit_Init(void)
{
    usart_4_tansmit = Hardware_Factory_Get(USART1);
    usart_4_tansmit->init(115200);
}

void Transmit_DeInit(void)
{
    usart_4_tansmit->deInit();
}

Transfer* Transmit_Create_Transfer(void)
{
    return Transfer_Create(usart_4_tansmit);
}

void Transmit_Destroy_Transfer(Transfer* trans)
{
    Transfer_Destroy(trans);
}
