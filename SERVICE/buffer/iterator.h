#ifndef ITERATOR_H_
#define ITERATOR_H_

#include "stm32f10x.h"

typedef struct _Iterator {
    u8* pHead;                          /* 迭代器迭代数组的头指针，才创建迭代器时自动添加 */
    u16 cur;                            /* 当前迭代坐标 */
    u16 len;                            /* 迭代总长度 */
    u8(*hasNext)(struct _Iterator*);    /* 当前迭代元素是否存在下个元素，用于判断迭代是否完成 */
    u8(*next)(struct _Iterator*);       /* 返回当前元素并将迭代坐标后移一位 */
} Iterator;

Iterator* Iterator_Create(u8 *, u16);   /* 创建迭代器 */
void Iterator_Destroy(Iterator*);       /* 销毁迭代器，回收其内存 */

#endif /* TERATOR_H_ */

