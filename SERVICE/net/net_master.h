#ifndef NET_MASTER_H_
#define NET_MASTER_H_ 

#include "buffer.h"

void Net_Master_Init(void);
void Net_Master_DeInit(void);
void Net_Master_Read(Buffer*);

#endif /* NET_MASTER_H_ */
