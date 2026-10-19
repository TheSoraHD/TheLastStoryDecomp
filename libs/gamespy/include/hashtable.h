// GameSpy SDK hash table (see libs/gamespy/license.txt).
#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HashImplementation* HashTable;

typedef int (*TableHashFn)(const void* elem, int numBuckets);
typedef int (*TableCompareFn)(const void* elem1, const void* elem2);
typedef void (*TableMapFn)(void* elem, void* clientData);
typedef int (*TableMapFn2)(void* elem, void* clientData);
typedef void (*TableElementFreeFn)(void* elem);

HashTable TableNew(int elemSize, int nBuckets, TableHashFn hashFn, TableCompareFn compFn,
                   TableElementFreeFn freeFn);
HashTable TableNew2(int elemSize, int nBuckets, int nChains, TableHashFn hashFn,
                    TableCompareFn compFn, TableElementFreeFn freeFn);
void TableFree(HashTable table);
int TableCount(HashTable table);
void TableEnter(HashTable table, const void* newElem);
int TableRemove(HashTable table, const void* delElem);
void* TableLookup(HashTable table, const void* elemKey);
void TableMapSafe(HashTable table, TableMapFn fn, void* clientData);
void* TableMapSafe2(HashTable table, TableMapFn2 fn, void* clientData);

#ifdef __cplusplus
}
#endif

#endif
