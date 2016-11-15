#include "transfer.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "net.h"

static void _Transfer_Send (Transfer* thiz, Buffer* buffer)
{
    Iterator* iter = buffer->iterator(buffer);
    
    thiz->bundle_usart->write((NET_SOT >> 24) & 0xFF);
    thiz->bundle_usart->write((NET_SOT >> 16) & 0xFF);
    thiz->bundle_usart->write((NET_SOT >> 8) & 0xFF);
    thiz->bundle_usart->write((NET_SOT) & 0xFF);

    while(iter->hasNext(iter)){
        thiz->bundle_usart->write(iter->next(iter));
    }
    
    thiz->bundle_usart->write((NET_EOT >> 24) & 0xFF);
    thiz->bundle_usart->write((NET_EOT >> 16) & 0xFF);
    thiz->bundle_usart->write((NET_EOT >> 8) & 0xFF);
    thiz->bundle_usart->write((NET_EOT) & 0xFF);
    
    Iterator_Destroy(iter);
}

static void _Transfer_Read (Transfer* thiz, Buffer* buffer)
{

}

Transfer* Transfer_Create(USART* usart)
{
    Transfer* trans = (Transfer*)malloc(sizeof(Transfer));

    trans->bundle_usart = usart;
    trans->send = _Transfer_Send;
    trans->read = _Transfer_Read;

    return trans;
}

void Transfer_Destroy(Transfer *thiz)
{
    free(thiz);
}
