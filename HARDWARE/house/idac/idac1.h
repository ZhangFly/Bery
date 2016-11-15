#ifndef IDAC1_H_
#define IDAC1_H_

#include "stm32f10x.h"
#include "idac_define.h"

IDAC* IDAC1_Create(void);       /* 工厂模式创建IDAC对象，同一编号IDAC不会反复创建 */
void IDAC1_Destroy(void);      /* 销毁IDAC对象，回收其内存 */

#endif /* IDAC1_H_ */
