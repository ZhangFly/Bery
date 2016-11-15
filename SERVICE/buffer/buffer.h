#ifndef BUFFER_H_
#define BUFFER_H_

#include "stm32f10x.h"
#include "iterator.h"

typedef struct _Buffer {
    u8* pHead;                              /* 缓存数组头指针，在创建时自动生成 */
    u16 len;                                /* 当前缓存长度 */
    u16 size;                               /* 缓存大小 */
    u8 (*empty)(struct _Buffer*);         /* 缓存是否为空 */
    void (*reset)(struct _Buffer*);         /* 清空缓存 */
    void (*put)(struct _Buffer*, u8);       /* 将数据压入缓存 */
    u8 (*get)(struct _Buffer*, u16);        /* 获取指定坐标的数据 */
    Iterator* (*iterator)(struct _Buffer*); /* 获取缓存的迭代器，在迭代器使用完后续及时销毁以回收内存 */
} Buffer;

Buffer* Buffer_Create(u32);                 /* 创建指定长度的缓存 */
void Buffer_Destroy(Buffer*);               /* 销毁缓存，回收其内存 */

#endif /* BUFFER_H_ */


