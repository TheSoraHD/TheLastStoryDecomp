// inferred: camera/animation curves read from an XSF file (atn::LoadXsca builds one Xsca_unk from
// the loaded data). The file is a list of named chunks; every chunk holds the name of a curve set
// and its keys, read on demand by fn_805BDCBC. Names invented.

#include "std/string.h"
#include "std/string_members.h"
#include "std/vector.h"

void* operator new(unsigned long size, int heap, const char* file, const char* name, int line);
void* operator new[](unsigned long size, int heap, const char* file, const char* name, int line);
void operator delete[](void* p);

// inferred: chunk header (see atn/XsfParser_unk.cpp)
struct XsfChunk_unk {
    unsigned int mSize;
    unsigned short mType;  // 0x04
    unsigned short mFlags; // 0x06: bit 0: an extended header follows
    unsigned int mExtSize; // 0x08: inferred: size of the extended header
};

// inferred: header check of an XSF file (atn/XsfParser_unk.cpp)
class XsfFile_unk {
public:
    XsfFile_unk(void* data);
    ~XsfFile_unk();

    unsigned int* mData;
    void* m04;
    int mValid;           // 0x08
    int mSwapped;         // 0x0C
    XsfChunk_unk* mFirst; // 0x10
};

// inferred: position in the chunk list (atn/XsfParser_unk.cpp)
class XsfIter_unk {
public:
    void fn_805BEC68(XsfChunk_unk* p);
    int fn_805BEC70(); // inferred: the chunk has an extended header
    XsfChunk_unk* fn_805BECA0(); // inferred: next chunk of the same level
    XsfChunk_unk* fn_805BED50(); // inferred: first child chunk
    int fn_805BEE04(); // inferred: number of child chunks

    XsfChunk_unk* mChunk;
};

// inferred: three floats
struct XsfVec_unk {
    float x, y, z;
};

// inferred: curve set of 13 channels (0xF4 bytes, atn/XsfParser_unk.cpp)
struct XsfCurveSet_unk {
    XsfCurveSet_unk(const char* name); // 0x805BEEC4
    ~XsfCurveSet_unk();                // 0x805BF164
    void fn_805BF660();                // inferred: finishes the set once its keys are read

    unsigned char m00[4];
    XsfVec_unk m04; // inferred: copied from the chunk header
    unsigned char m10[0xF4 - 0x10];
};

// inferred: read the keys of a chunk into a channel (defined in atn/XsfParser_unk.cpp, where the
// split of the original file is not known; they receive the Xsca_unk unchanged)
extern "C" void fn_805BE8FC(void* self, unsigned int* chunk, XsfCurveSet_unk* curves, int channel);
extern "C" void fn_805BEA34(void* self, unsigned int* chunk, XsfCurveSet_unk* curves, int channel);

// inferred: size of the header of the chunk at `it`
static inline unsigned int __HeaderSize_unk(XsfIter_unk& it) {
    unsigned int size = 8;
    if (it.fn_805BEC70()) {
        size = ((it.mChunk->mExtSize + 3) & ~3) + 12;
    }
    return size;
}

// inferred: data of the chunk at `it`
static inline unsigned int* __Data_unk(XsfIter_unk& it) {
    return (unsigned int*)((char*)it.mChunk + __HeaderSize_unk(it));
}

static inline unsigned int __Swap32_unk(unsigned int x) {
    unsigned int r = x << 24;
    r = __rlwimi(r, x, 8, 24, 31);
    r = __rlwimi(r, x, 24, 16, 23);
    r = __rlwimi(r, x, 8, 8, 15);
    return r;
}

// inferred: byte swap of three floats
static inline XsfVec_unk __SwapVec_unk(float x, float y, float z) {
    XsfVec_unk r;
    __stwbrx(*(unsigned int*)&x, &r.x, 0);
    __stwbrx(*(unsigned int*)&y, &r.y, 0);
    __stwbrx(*(unsigned int*)&z, &r.z, 0);
    return r;
}

// inferred: curve set entry: its chunk and the set once read
struct XscaEntry_unk {
    XscaEntry_unk(XsfChunk_unk* chunk, XsfCurveSet_unk* curves) : mChunk(chunk), mCurves(curves) {}
    XsfChunk_unk* mChunk;
    XsfCurveSet_unk* mCurves;
};

namespace std {

// inferred: the entries are stored like pointers (size incremented first, no null check of the
// slot, moved with memcpy), as the int and pointer specializations of std/vector.h
template <>
struct __vec_imp_unk<XscaEntry_unk> : public __vec_storage_unk<XscaEntry_unk> {
    ~__vec_imp_unk() {}
    void __push_fast_unk(unsigned int n, const XscaEntry_unk& x) {
        this->mSize = n + 1;
        this->mData[this->mSize - 1] = x;
    }
};

template <>
struct __vec_split_unk<XscaEntry_unk> : public __vec_split_base_unk<XscaEntry_unk> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<XscaEntry_unk>(alloc), mStart(0) {}
    XscaEntry_unk* begin() { return this->mData + mStart; }
    void push_back(const XscaEntry_unk& x) {
        begin()[this->mSize] = x;
        this->mSize++;
    }
    void __push_front_unk(XscaEntry_unk* first, XscaEntry_unk* last) {
        unsigned int n = ((char*)last - (char*)first) / (int)sizeof(XscaEntry_unk);
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(XscaEntry_unk));
        memset(first, 0, n * sizeof(XscaEntry_unk));
        this->mSize += n;
    }
    unsigned int mStart;
};

} // namespace std

// inferred: array owner (deletes the array; reset deletes the previous one), like
// boost::scoped_array
template <class T>
struct ScopedArray_unk {
    ScopedArray_unk(T* p = 0) : mPtr(p) {}
    ~ScopedArray_unk() { delete[] mPtr; }
    void reset(T* p) {
        ScopedArray_unk tmp(p);
        T* t = tmp.mPtr;
        tmp.mPtr = mPtr;
        mPtr = t;
    }
    T& operator[](unsigned int i) { return mPtr[i]; }
    T* mPtr;
};

struct Xsca_unk {
    Xsca_unk(void* data);
    ~Xsca_unk();
    int fn_805BD4B8();
    XsfCurveSet_unk* fn_805BDCBC(unsigned int index);
    XsfCurveSet_unk* fn_805BDE48(XscaEntry_unk* entry, const std::string& name);
    XsfCurveSet_unk* fn_805BE37C(XscaEntry_unk* entry, const std::string& name);

    void* mData;                         // 0x00
    unsigned int mCount;                 // 0x04
    int mLoaded;                         // 0x08
    int mSwapped;                        // 0x0C
    std::vector<XscaEntry_unk> mEntries; // 0x10
    ScopedArray_unk<std::string> mNames; // 0x1C
};

// inferred: name of the curve set of `chunk` (length and characters after the chunk header), in
// a file with the other byte order
static inline void __ReadNameSwapped_unk(std::string& name, XsfChunk_unk* chunk) {
    XsfIter_unk it;
    it.fn_805BEC68(chunk);
    unsigned int len = __Swap32_unk(*__Data_unk(it));
    const char* src = (const char*)__Data_unk(it) + 4;
    char* buf = new (6, "", "", 0) char[len + 1];
    buf[len] = 0;
    for (int j = 0; j < len; j++) {
        buf[j] = *src++;
    }
    name = buf;
    delete[] buf;
}

// inferred: name of the curve set of `chunk`
static inline void __ReadName_unk(std::string& name, XsfChunk_unk* chunk) {
    XsfIter_unk it;
    it.fn_805BEC68(chunk);
    unsigned int len = *__Data_unk(it);
    const char* src = (const char*)__Data_unk(it) + 4;
    char* buf = new (6, "", "", 0) char[len + 1];
    buf[len] = 0;
    for (unsigned int j = 0; j < len; j++) {
        buf[j] = *src++;
    }
    name = buf;
    delete[] buf;
}

// NONMATCHING: 99.09% - the inlined push_back loads the capacity before the size (the original
// the other way round, same open case as the push_back of std/vector.h), the two temporaries of
// the inlined growth helper are swapped on the stack, and the loop strides take other registers.
// inferred: reads the chunk list: one entry and one name per curve set
int Xsca_unk::fn_805BD4B8() {
    XsfFile_unk file(mData);
    if (!file.mValid) {
        return 0;
    }
    if (file.mSwapped) {
        mSwapped = 1;
    }
    XsfIter_unk root;
    root.mChunk = file.mFirst;
    mCount = root.fn_805BEE04() - 1;
    mEntries.__reserve_checked_unk(mCount);
    XsfIter_unk first;
    first.mChunk = root.fn_805BED50();
    XsfIter_unk it;
    it.mChunk = first.fn_805BECA0();
    for (unsigned int i = 0; i < mCount; i++) {
        mEntries.push_back(XscaEntry_unk(it.mChunk, 0));
        it.mChunk = it.fn_805BECA0();
    }
    mNames.reset(new (6, "", "", 0) std::string[mCount]);
    for (unsigned int i = 0; i < mCount; i++) {
        if (mSwapped) {
            __ReadNameSwapped_unk(mNames[i], mEntries[i].mChunk);
        } else {
            __ReadName_unk(mNames[i], mEntries[i].mChunk);
        }
    }
    return 1;
}

// inferred: curve set `index`, read the first time
XsfCurveSet_unk* Xsca_unk::fn_805BDCBC(unsigned int index) {
    if (index >= mCount) {
        return 0;
    }
    XscaEntry_unk* entry = &mEntries[index];
    if (entry->mCurves != 0) {
        return entry->mCurves;
    }
    std::string& name = mNames[index];
    if (mSwapped) {
        return fn_805BE37C(entry, name);
    }
    return fn_805BDE48(entry, name);
}

Xsca_unk::Xsca_unk(void* data) : mData(data), mCount(0), mLoaded(0), mSwapped(0) {
    mLoaded = fn_805BD4B8();
}

Xsca_unk::~Xsca_unk() {
    for (XscaEntry_unk* p = mEntries.begin(); p != mEntries.end(); p++) {
        if (p->mCurves != 0) {
            delete p->mCurves;
        }
    }
}

// inferred: reads the curve set of `entry`
XsfCurveSet_unk* Xsca_unk::fn_805BDE48(XscaEntry_unk* entry, const std::string& name) {
    XsfIter_unk it;
    it.fn_805BEC68(entry->mChunk);
    unsigned int len = *__Data_unk(it);
    char* str = (char*)__Data_unk(it) + 4;
    str += (len + 3) & ~3;
    float* header = (float*)str;
    XsfCurveSet_unk* curves = new (6, "", "", 0) XsfCurveSet_unk(name.data());
    entry->mCurves = curves;
    float z;
    float y;
    float x;
    x = header[1];
    y = header[2];
    z = header[3];
    curves->m04.x = x;
    curves->m04.y = y;
    curves->m04.z = z;

    it.mChunk = it.fn_805BED50();
    fn_805BEA34(this, __Data_unk(it), curves, 0);

    it.mChunk = it.fn_805BECA0();
    XsfIter_unk a;
    a.mChunk = it.fn_805BED50();
    fn_805BEA34(this, __Data_unk(a), curves, 1);
    a.mChunk = a.fn_805BECA0();
    fn_805BEA34(this, __Data_unk(a), curves, 2);
    a.mChunk = a.fn_805BECA0();
    fn_805BEA34(this, __Data_unk(a), curves, 3);

    it.mChunk = it.fn_805BECA0();
    XsfIter_unk b;
    b.mChunk = it.fn_805BED50();
    fn_805BEA34(this, __Data_unk(b), curves, 4);
    b.mChunk = b.fn_805BECA0();
    fn_805BEA34(this, __Data_unk(b), curves, 5);
    b.mChunk = b.fn_805BECA0();
    fn_805BEA34(this, __Data_unk(b), curves, 6);

    it.mChunk = it.fn_805BECA0();
    XsfIter_unk c;
    c.mChunk = it.fn_805BED50();
    fn_805BEA34(this, __Data_unk(c), curves, 7);
    c.mChunk = c.fn_805BECA0();
    fn_805BEA34(this, __Data_unk(c), curves, 8);
    c.mChunk = c.fn_805BECA0();
    fn_805BEA34(this, __Data_unk(c), curves, 9);

    it.mChunk = it.fn_805BECA0();
    XsfIter_unk d;
    d.mChunk = it.fn_805BED50();
    fn_805BEA34(this, __Data_unk(d), curves, 10);
    d.mChunk = d.fn_805BECA0();
    fn_805BEA34(this, __Data_unk(d), curves, 11);
    d.mChunk = d.fn_805BECA0();
    fn_805BEA34(this, __Data_unk(d), curves, 12);

    curves->fn_805BF660();
    return curves;
}

// NONMATCHING: 97.02% - only the scheduling of the three
// byte swaps: the original keeps the addresses of the results in r6/r7/r5 and interleaves the
// swaps with the copies. Tried the swaps through an inline (by value/reference), a struct, the
// statement order.
// inferred: same as fn_805BDE48 for a file with the other byte order
XsfCurveSet_unk* Xsca_unk::fn_805BE37C(XscaEntry_unk* entry, const std::string& name) {
    XsfIter_unk it;
    it.fn_805BEC68(entry->mChunk);
    unsigned int len = __Swap32_unk(*__Data_unk(it));
    char* str = (char*)__Data_unk(it) + 4;
    str += (len + 3) & ~3;
    float* header = (float*)str;
    XsfCurveSet_unk* curves = new (6, "", "", 0) XsfCurveSet_unk(name.data());
    entry->mCurves = curves;
    float z;
    float y;
    float x;
    float rz;
    float ry;
    float rx;
    x = header[1];
    y = header[2];
    z = header[3];
    __stwbrx(*(unsigned int*)&x, &rx, 0);
    __stwbrx(*(unsigned int*)&y, &ry, 0);
    __stwbrx(*(unsigned int*)&z, &rz, 0);
    curves->m04.x = rx;
    curves->m04.y = ry;
    curves->m04.z = rz;

    it.mChunk = it.fn_805BED50();
    fn_805BE8FC(this, __Data_unk(it), curves, 0);

    it.mChunk = it.fn_805BECA0();
    XsfIter_unk a;
    a.mChunk = it.fn_805BED50();
    fn_805BE8FC(this, __Data_unk(a), curves, 1);
    a.mChunk = a.fn_805BECA0();
    fn_805BE8FC(this, __Data_unk(a), curves, 2);
    a.mChunk = a.fn_805BECA0();
    fn_805BE8FC(this, __Data_unk(a), curves, 3);

    it.mChunk = it.fn_805BECA0();
    XsfIter_unk b;
    b.mChunk = it.fn_805BED50();
    fn_805BE8FC(this, __Data_unk(b), curves, 4);
    b.mChunk = b.fn_805BECA0();
    fn_805BE8FC(this, __Data_unk(b), curves, 5);
    b.mChunk = b.fn_805BECA0();
    fn_805BE8FC(this, __Data_unk(b), curves, 6);

    it.mChunk = it.fn_805BECA0();
    XsfIter_unk c;
    c.mChunk = it.fn_805BED50();
    fn_805BE8FC(this, __Data_unk(c), curves, 7);
    c.mChunk = c.fn_805BECA0();
    fn_805BE8FC(this, __Data_unk(c), curves, 8);
    c.mChunk = c.fn_805BECA0();
    fn_805BE8FC(this, __Data_unk(c), curves, 9);

    it.mChunk = it.fn_805BECA0();
    XsfIter_unk d;
    d.mChunk = it.fn_805BED50();
    fn_805BE8FC(this, __Data_unk(d), curves, 10);
    d.mChunk = d.fn_805BECA0();
    fn_805BE8FC(this, __Data_unk(d), curves, 11);
    d.mChunk = d.fn_805BECA0();
    fn_805BE8FC(this, __Data_unk(d), curves, 12);

    curves->fn_805BF660();
    return curves;
}

// inferred: basic_string::reserve was instantiated by the original file (its message is in the
// string pool between "" and the messages of __replace_unk); the weak copy is dropped by the linker
template void std::string::reserve(unsigned long);
