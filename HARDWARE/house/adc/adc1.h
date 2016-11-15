#ifndef ADC1_H_
#define ADC1_H_	

#include "adc_define.h"

ADC* ADC1_Create(void);    /* 工厂模式创建ADC对象，同一编号ADC不会反复创建 */
void ADC1_Destroy(void);   /* 销毁一个ADC对象，回收其内存 */

#endif /* ADC1_H_ */
