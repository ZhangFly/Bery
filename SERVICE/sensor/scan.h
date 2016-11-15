#ifndef SCAN_H_
#define SCAN_H_	

#include "stm32f10x.h"
#include "hardware_factory.h"
#include "buffer.h"

#define SCAN_MODE_X_FIRST 0x0
#define SCAN_MODE_Y_FIRST 0x1

typedef struct _Point {
	u8  x : 8;
	u8  y : 8;
}Point;

typedef struct _Scan {
    ADC *bundle_adc;                            /* 每个Scan对象必须绑定一个ADC对象 */
    IDAC *bundle_idac;                          /* 每个Scan对象必须绑定一个IDAC对象 */
    Buffer *buffer;                             /* 可选添加，如果添加了buffer对象则自动将扫描结果存入buffer */
    Point cur;                                  /* 当前扫描点 */
    Point beg;                                  /* 扫描区域起始点 */
    Point end;                                  /* 扫描区域终止点 */     
    u8 mode;                                    /* 扫描模式 */
    u8 loop;                                    /* 是否循环扫描 */  
    u8 over;                                    /* 区域扫描是否完成 */ 
    u16 (*point)(struct _Scan *, Point pPoint); /* 扫描指定坐标点 */  
    u16 (*step)(struct _Scan *);                /* 逐步扫描 */                   
    void (*restart)(struct _Scan *);            /* 复位扫描区域 */
    void (*start)(struct _Scan *);              /* 开启扫描 */
    void (*stop)(struct _Scan *);               /* 关闭扫描 */
}Scan;

Scan* Scan_Create(void);                        /* 创建一个Scan对象 */
void Scan_Destroy(Scan*);                       /* 销毁一个Scan对象，回收其内存 */

#endif /* SCAN_H_ */
