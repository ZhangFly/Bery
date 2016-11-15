#include "buffer.h"
#include "stdlib.h"

static u8 _Buffer_Empty(Buffer*);
static void _Buffer_Reset(Buffer*);
static void _Buffer_Put(Buffer*, u8);
static u8 _Buffer_Get(Buffer*, u16);
static Iterator* _Buffer_Iterator(Buffer*);

Buffer* Buffer_Create(u32 len) {
    Buffer *pBuffer = (Buffer*) malloc(sizeof (Buffer));
    u8 *pData = (u8*) malloc(sizeof (u8) * len);

    if (pBuffer == 0)
        return 0;

    if (pData == 0) {
        free(pBuffer);
        return 0;
    }

    pBuffer->pHead = pData;
    pBuffer->size = len;
    pBuffer->len = 0;
    pBuffer->empty = _Buffer_Empty;
    pBuffer->put = _Buffer_Put;
    pBuffer->get = _Buffer_Get;
    pBuffer->reset = _Buffer_Reset;
    pBuffer->iterator = _Buffer_Iterator;
    return pBuffer;
}

void Buffer_Destroy(Buffer* pBuffer) {
    free(pBuffer->pHead);
    free(pBuffer);
}

static u8 _Buffer_Empty(Buffer* thiz)
{
    return thiz->len ? 0 : 1;
}

static void _Buffer_Reset(Buffer* thiz) {
    thiz->len = 0;
}

static void _Buffer_Put(Buffer* thiz, u8 data) {
    if (thiz->len >= thiz->size)
        return;
    thiz->pHead[thiz->len++] = data;
}

static u8 _Buffer_Get(Buffer* thiz, u16 index) {
    return thiz->pHead[index];
}

static Iterator* _Buffer_Iterator(Buffer* thiz) {
    return Iterator_Create(thiz->pHead, thiz->len);
}
