#include "iterator.h"
#include "stdlib.h"

static u8 _Iterator_Has_Next(Iterator* thiz);
static u8 _Iterator_Next(Iterator* thiz);

Iterator* Iterator_Create(u8 *pHead, u16 len) {
    Iterator *pIterator = (Iterator*) malloc(sizeof (Iterator));

    if (pIterator == 0)
        return 0;

    pIterator->pHead = pHead;
    pIterator->cur = 0;
    pIterator->len = len;
    pIterator->hasNext = _Iterator_Has_Next;
    pIterator->next = _Iterator_Next;
    return pIterator;
}

void Iterator_Destroy(Iterator* pIterator) {
    if (pIterator == 0)
        return;
    free(pIterator);
}

static u8 _Iterator_Has_Next(Iterator* thiz) {
    return thiz->cur < thiz->len;
}

static u8 _Iterator_Next(Iterator* thiz) {
    return thiz->pHead[thiz->cur++];
}
