#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "stm32f10x.h" 
#include "buffer.h"

void Sample_Start(void);
void Sample_Stop(void);
void Sample_Pause(void);
void Sample_Resume(void);
Buffer* Sample_Buffer_Get(void);

#endif /* SAMPLE_H_ */
