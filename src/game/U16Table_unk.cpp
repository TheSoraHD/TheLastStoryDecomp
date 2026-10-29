// inferred file name (splits-auto): run-length table of u16 values (route links between points,
// read by ChaserNpc, CrowdSimulation, Augur, MeetingSpot and built by GameScene).
#include "atn/Memory.h"
#include "atn/Csv.h"
#include "std/vector.h"

extern "C" int atoi(const char* s);

// inferred: one run of the table: `mValue` repeated `mCount` times
struct U16Run_unk {
    unsigned short mCount;
    unsigned short mValue;
};

// inferred: the same pair with a constructor, collected in a std::vector while building
struct U16RunPair_unk {
    U16RunPair_unk(unsigned short count, unsigned short value) : mCount(count), mValue(value) {}
    unsigned short mCount;
    unsigned short mValue;
};

namespace std {

// inferred: the runs are stored like int elements (size incremented first, element assigned) but
// moved with copy construction when the vector grows
template <>
struct __vec_imp_unk<U16RunPair_unk> : public __vec_storage_unk<U16RunPair_unk> {
    ~__vec_imp_unk() {}
    void __push_fast_unk(unsigned int n, const U16RunPair_unk& x) {
        this->mSize = n + 1;
        this->mData[this->mSize - 1] = x;
    }
};

template <>
struct __vec_split_unk<U16RunPair_unk> : public __vec_split_base_unk<U16RunPair_unk> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<U16RunPair_unk>(alloc), mStart(0) {}
    U16RunPair_unk* begin() { return this->mData + mStart; }
    void push_back(const U16RunPair_unk& x) {
        begin()[this->mSize] = x;
        this->mSize++;
    }
    void __push_front_unk(U16RunPair_unk* first, U16RunPair_unk* last) {
        U16RunPair_unk* p = begin();
        while (last > first) {
            new (--p) U16RunPair_unk(*--last);
            mStart--;
            this->mSize++;
        }
    }
    unsigned int mStart;
};

} // namespace std

// inferred: run-length encoded array of u16 (8 bytes)
struct U16Table_unk {
    unsigned short mCount; // inferred: number of values
    unsigned short mRuns;  // inferred: number of runs
    U16Run_unk* mData;
};

extern "C" void fn_803CCF68(U16Table_unk* table, const unsigned short* values, unsigned int count);

// inferred: builds the table from `count` values
extern "C" void fn_803CCF64(U16Table_unk* table, const unsigned short* values, unsigned int count) {
    fn_803CCF68(table, values, count);
}

// NONMATCHING: 89.6% - inlined std::vector::push_back: the original loads the size before the capacity
// (open problem of include/std/vector.h) and the saved registers of the loop variables differ
extern "C" void fn_803CCF68(U16Table_unk* table, const unsigned short* values, unsigned int count) {
    if (count == 0) {
        return;
    }
    table->mCount = count;
    std::vector<U16RunPair_unk> runs;
    unsigned short run;
    unsigned short value;
    for (unsigned int i = 0; i < count; i++, values++) {
        if (i == 0) {
            value = *values;
            run = 1;
        } else if (value != *values) {
            runs.push_back(U16RunPair_unk(run, value));
            value = *values;
            run = 1;
        } else {
            run++;
        }
    }
    runs.push_back(U16RunPair_unk(run, value));
    table->mRuns = runs.size();
    table->mData = new ((MemoryTag)3, "", "", 0) U16Run_unk[table->mRuns];
    for (unsigned short i = 0; i < table->mRuns; i++) {
        table->mData[i].mCount = runs[i].mCount;
        table->mData[i].mValue = runs[i].mValue;
    }
}

// NONMATCHING: 88.5% - same push_back problem as fn_803CCF68
// inferred: builds the table from "value,count" pairs of a CSV
extern "C" void fn_803CD598(U16Table_unk* table, atn::Csv* csv, unsigned int count) {
    if (count == 0) {
        return;
    }
    table->mCount = count;
    std::vector<U16RunPair_unk> runs;
    for (;;) {
        const char* field = csv->fn_8005B9CC();
        if (*field == 0) {
            break;
        }
        unsigned short value = atoi(field);
        unsigned short run = atoi(csv->fn_8005B9CC());
        runs.push_back(U16RunPair_unk(run, value));
    }
    table->mRuns = runs.size();
    table->mData = new ((MemoryTag)3, "", "", 0) U16Run_unk[table->mRuns];
    for (unsigned short i = 0; i < table->mRuns; i++) {
        table->mData[i].mCount = runs[i].mCount;
        table->mData[i].mValue = runs[i].mValue;
    }
}

extern "C" void fn_803CD918(U16Table_unk* table) {
    ::operator delete[](table->mData);
    table->mData = 0;
    table->mCount = 0;
    table->mRuns = 0;
}

// NONMATCHING: 98.0% - register allocation (the original keeps mRuns in r6, the data in r5)
// inferred: value at `index`
extern "C" unsigned short fn_803CD958(U16Table_unk* table, unsigned int index) {
    unsigned short end = 0;
    for (unsigned short i = 0; i < table->mRuns; i++) {
        end += table->mData[i].mCount;
        if (index < end) {
            return table->mData[i].mValue;
        }
    }
    return 0;
}
