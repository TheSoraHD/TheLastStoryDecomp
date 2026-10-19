// GameSpy SDK hashtable.c (see libs/gamespy/license.txt). Original author: David Wright, 10/8/98.
// Fixed array of buckets, one DArray per bucket.
#include "darray.h"
#include "hashtable.h"
#include "nonport.h"

struct HashImplementation {
    DArray* buckets;
    int nbuckets;
    TableElementFreeFn freefn;
    TableHashFn hashfn;
    TableCompareFn compfn;
};

HashTable TableNew(int elemSize, int nBuckets, TableHashFn hashFn, TableCompareFn compFn,
                   TableElementFreeFn freeFn) {
    return TableNew2(elemSize, nBuckets, 4, hashFn, compFn, freeFn);
}

HashTable TableNew2(int elemSize, int nBuckets, int nChains, TableHashFn hashFn,
                    TableCompareFn compFn, TableElementFreeFn freeFn) {
    HashTable table;
    int i;

    table = (HashTable)gsimalloc(sizeof(struct HashImplementation));
    table->buckets = (DArray*)gsimalloc(nBuckets * sizeof(DArray));
    for (i = 0; i < nBuckets; i++)
        table->buckets[i] = ArrayNew(elemSize, nChains, freeFn);
    table->nbuckets = nBuckets;
    table->freefn = freeFn;
    table->compfn = compFn;
    table->hashfn = hashFn;
    return table;
}

void TableFree(HashTable table) {
    int i;

    if (NULL == table)
        return;

    for (i = 0; i < table->nbuckets; i++)
        ArrayFree(table->buckets[i]);
    gsifree(table->buckets);
    gsifree(table);
}

int TableCount(HashTable table) {
    int i, count = 0;

    if (NULL == table)
        return count;

    for (i = 0; i < table->nbuckets; i++)
        count += ArrayLength(table->buckets[i]);
    return count;
}

void TableEnter(HashTable table, const void* newElem) {
    int hash, itempos;

    if (NULL == table)
        return;

    hash = table->hashfn(newElem, table->nbuckets);
    itempos = ArraySearch(table->buckets[hash], newElem, table->compfn, 0, 0);
    if (itempos == NOT_FOUND)
        ArrayAppend(table->buckets[hash], newElem);
    else
        ArrayReplaceAt(table->buckets[hash], newElem, itempos);
}

int TableRemove(HashTable table, const void* delElem) {
    int hash, itempos;

    if (NULL == table)
        return 0;

    hash = table->hashfn(delElem, table->nbuckets);
    itempos = ArraySearch(table->buckets[hash], delElem, table->compfn, 0, 0);
    if (itempos == NOT_FOUND)
        return 0;
    else
        ArrayDeleteAt(table->buckets[hash], itempos);
    return 1;
}

void* TableLookup(HashTable table, const void* elemKey) {
    int hash, itempos;

    if (NULL == table)
        return NULL;

    hash = table->hashfn(elemKey, table->nbuckets);
    itempos = ArraySearch(table->buckets[hash], elemKey, table->compfn, 0, 0);
    if (itempos == NOT_FOUND)
        return NULL;
    else
        return ArrayNth(table->buckets[hash], itempos);
}

void TableMapSafe(HashTable table, TableMapFn fn, void* clientData) {
    int i;

    for (i = 0; i < table->nbuckets; i++)
        ArrayMapBackwards(table->buckets[i], fn, clientData);
}

void* TableMapSafe2(HashTable table, TableMapFn2 fn, void* clientData) {
    int i;
    void* pcurr;

    for (i = 0; i < table->nbuckets; i++) {
        pcurr = ArrayMapBackwards2(table->buckets[i], fn, clientData);
        if (pcurr)
            return pcurr;
    }

    return NULL;
}
