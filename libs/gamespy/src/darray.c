// GameSpy SDK darray.c, adapted to the version linked in this game
// (see libs/gamespy/license.txt). Original author: David Wright, 10/8/98.
#include "darray.h"
#include "nonport.h"

#define DEF_GROWBY 8

extern void qsort(void* base, size_t num, size_t size, ArrayCompareFn compare);

struct DArrayImplementation {
    int count;
    int capacity;
    int elemsize;
    int growby;
    ArrayElementFreeFn elemfreefn;
    void* list;
};

static void* mylsearch(const void* key, void* base, int count, int size, ArrayCompareFn comparator);
static void* mybsearch(const void* elem, void* base, int num, int elemsize, ArrayCompareFn comparator,
                       int* found);

static void FreeElement(DArray array, int n) {
    if (array->elemfreefn != NULL)
        array->elemfreefn(ArrayNth(array, n));
}

static void ArrayGrow(DArray array) {
    array->capacity += array->growby;
    array->list = gsirealloc(array->list, (size_t)array->capacity * array->elemsize);
}

static void SetElement(DArray array, const void* elem, int pos) {
    memcpy(ArrayNth(array, pos), elem, (size_t)array->elemsize);
}

DArray ArrayNew(int elemSize, int numElemsToAllocate, ArrayElementFreeFn elemFreeFn) {
    DArray array;

    array = (DArray)gsimalloc(sizeof(struct DArrayImplementation));
    if (numElemsToAllocate == 0)
        numElemsToAllocate = DEF_GROWBY;
    array->count = 0;
    array->capacity = numElemsToAllocate;
    array->elemsize = elemSize;
    array->growby = numElemsToAllocate;
    array->elemfreefn = elemFreeFn;
    if (array->capacity != 0) {
        array->list = gsimalloc((size_t)array->capacity * array->elemsize);
        memset(array->list, 0, (size_t)array->capacity * array->elemsize);
    } else {
        array->list = NULL;
    }

    return array;
}

void ArrayFree(DArray array) {
    int i;

    for (i = 0; i < array->count; i++) {
        FreeElement(array, i);
    }
    if (array->list)
        gsifree(array->list);
    gsifree(array);
}

int ArrayLength(const DArray array) {
    return array->count;
}

void* ArrayNth(DArray array, int n) {
    if (!((n >= 0) && (n < array->count)))
        return NULL;

    return (char*)array->list + array->elemsize * n;
}

static void ArrayInsertAt(DArray array, const void* newElem, int n) {
    if (array->count == array->capacity)
        ArrayGrow(array);
    array->count++;
    if (n < array->count - 1)
        memmove(ArrayNth(array, n + 1), ArrayNth(array, n),
                (size_t)(array->count - 1 - n) * array->elemsize);
    SetElement(array, newElem, n);
}

void ArrayAppend(DArray array, const void* newElem) {
    if (array)
        ArrayInsertAt(array, newElem, array->count);
}

void ArrayInsertSorted(DArray array, const void* newElem, ArrayCompareFn comparator) {
    int n;
    void* res;
    int found;

    res = mybsearch(newElem, array->list, array->count, array->elemsize, comparator, &found);
    n = (((char*)res - (char*)array->list) / array->elemsize);
    ArrayInsertAt(array, newElem, n);
}

void ArrayRemoveAt(DArray array, int n) {
    if (n < array->count - 1)
        memmove(ArrayNth(array, n), ArrayNth(array, n + 1),
                (size_t)(array->count - 1 - n) * array->elemsize);
    array->count--;
}

void ArrayDeleteAt(DArray array, int n) {
    FreeElement(array, n);
    ArrayRemoveAt(array, n);
}

void ArrayReplaceAt(DArray array, const void* newElem, int n) {
    FreeElement(array, n);
    SetElement(array, newElem, n);
}

void ArraySort(DArray array, ArrayCompareFn comparator) {
    qsort(array->list, (size_t)array->count, (size_t)array->elemsize, comparator);
}

int ArraySearch(DArray array, const void* key, ArrayCompareFn comparator, int fromIndex,
                int isSorted) {
    void* res;
    int found = 1;
    if (!array || array->count == 0)
        return NOT_FOUND;

    if (isSorted)
        res = mybsearch(key, ArrayNth(array, fromIndex), array->count - fromIndex, array->elemsize,
                        comparator, &found);
    else
        res = mylsearch(key, ArrayNth(array, fromIndex), array->count - fromIndex, array->elemsize,
                        comparator);
    if (res != NULL && found)
        return (((char*)res - (char*)array->list) / array->elemsize);
    else
        return NOT_FOUND;
}

void ArrayMapBackwards(DArray array, ArrayMapFn fn, void* clientData) {
    int i;

    for (i = (array->count - 1); i >= 0; i--)
        fn(ArrayNth(array, i), clientData);
}

void* ArrayMapBackwards2(DArray array, ArrayMapFn2 fn, void* clientData) {
    int i;
    void* pcurr;

    for (i = (array->count - 1); i >= 0; i--) {
        pcurr = ArrayNth(array, i);
        if (!fn(pcurr, clientData))
            return pcurr;
    }

    return NULL;
}

void ArrayClear(DArray array) {
    int i;

    for (i = (ArrayLength(array) - 1); i >= 0; i--)
        ArrayDeleteAt(array, i);
}

static void* mylsearch(const void* key, void* base, int count, int size, ArrayCompareFn comparator) {
    int i;
    for (i = 0; i < count; i++) {
        if (comparator(key, (char*)base + size * i) == 0)
            return (char*)base + size * i;
    }
    return NULL;
}

static void* mybsearch(const void* elem, void* base, int num, int elemsize, ArrayCompareFn comparator,
                       int* found) {
    int L, H, I, C;

    L = 0;
    H = num - 1;
    *found = 0;
    while (L <= H) {
        I = (L + H) >> 1;
        C = comparator(((char*)base) + I * elemsize, elem);
        if (C == 0)
            *found = 1;
        if (C < 0)
            L = I + 1;
        else
            H = I - 1;
    }
    return ((char*)base) + L * elemsize;
}
