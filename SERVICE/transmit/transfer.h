#ifndef TRANSFER_H_
#define TRANSFER_H_

#include "hardware_factory.h"
#include "buffer.h"

typedef struct _Transfer {
    USART *bundle_usart;
    void (*send)(struct _Transfer*, Buffer*);
    void (*read)(struct _Transfer*, Buffer*);
}Transfer;

Transfer* Transfer_Create(USART*);
void Transfer_Destroy(Transfer*);

#endif /* TRANSFER_H_ */
