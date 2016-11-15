#ifndef TRANSMIT_H_
#define TRANSMIT_H_

#include "transfer.h"

void Transmit_Init(void);
void Transmit_DeInit(void);
Transfer* Transmit_Create_Transfer(void);
void Transmit_Destroy_Transfer(Transfer*);

#endif /* TRANSMIT_H_ */
