// GameSpy SDK dynamic array (see libs/gamespy/license.txt).
#ifndef _DARRAY_H
#define _DARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DArrayImplementation* DArray;

typedef int (*ArrayCompareFn)(const void* elem1, const void* elem2);
typedef void (*ArrayMapFn)(void* elem, void* clientData);
typedef int (*ArrayMapFn2)(void* elem, void* clientData);
typedef void (*ArrayElementFreeFn)(void* elem);

DArray ArrayNew(int elemSize, int numElemsToAllocate, ArrayElementFreeFn elemFreeFn);
void ArrayFree(DArray array);
int ArrayLength(const DArray array);
void* ArrayNth(DArray array, int n);
void ArrayAppend(DArray array, const void* newElem);
void ArrayInsertSorted(DArray array, const void* newElem, ArrayCompareFn comparator);
void ArrayRemoveAt(DArray array, int n);
void ArrayDeleteAt(DArray array, int n);
void ArrayReplaceAt(DArray array, const void* newElem, int n);
void ArraySort(DArray array, ArrayCompareFn comparator);
#define NOT_FOUND -1
int ArraySearch(DArray array, const void* key, ArrayCompareFn comparator, int fromIndex,
                int isSorted);
void ArrayMapBackwards(DArray array, ArrayMapFn fn, void* clientData);
void* ArrayMapBackwards2(DArray array, ArrayMapFn2 fn, void* clientData);
void ArrayClear(DArray array);

#ifdef __cplusplus
}
#endif

#endif
