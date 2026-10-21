// inferred: reader of chunk files (see include/game/ChunkFile_unk.h). Real file name unknown: the
// link order puts it between game/HomingBullet and the item manager. Names invented.
#include "game/ChunkFile_unk.h"

extern "C" bool fn_8006969C(); // inferred: the platform is big endian
extern "C" bool fn_800696B4(); // inferred: the platform is little endian

// inferred: byte swap written with the rlwimi intrinsic (slwi + three rlwimi in the original)
static inline unsigned int Swap32_unk(unsigned int x) {
    unsigned int r = x << 24;
    r = __rlwimi(r, x, 8, 24, 31);
    r = __rlwimi(r, x, 24, 16, 23);
    r = __rlwimi(r, x, 8, 8, 15);
    return r;
}

static inline void SwapInPlace_unk(unsigned int* p) {
    __stwbrx(*p, p, 0);
}

// inferred: file header (8 words; m14 is the self-relative offset of the chunk table)
struct ChunkFileHeader_unk {
    unsigned int m00; // magic
    unsigned int m04;
    unsigned int m08;
    unsigned int m0C; // inferred: flags
    unsigned int m10;
    unsigned int m14;
    unsigned int m18;
    unsigned int m1C;
};

// NONMATCHING: 86.6% - register allocation of the swapped branch; the original computes the header
// addresses with addi where this gives lwzu
ChunkFile_unk::ChunkFile_unk(void* data, unsigned int magic)
    : mValid(0), mSwapped(0), mMagic(magic), mData(data), m10(0), m14(0), m18(0), m1C(0) {
    int order;
    unsigned int head = *(unsigned int*)data;
    if (head == magic) {
        order = 0;
    } else {
        order = 2;
        if (head == Swap32_unk(magic)) {
            order = 1;
        }
    }
    if ((fn_800696B4() && order == 0) || (fn_8006969C() && order == 1)) {
        ChunkFileHeader_unk* h = (ChunkFileHeader_unk*)mData;
        m10 = (ChunkTable*)((char*)&h->m14 + h->m14);
        char* x = (char*)m10 + m10->m04;
        unsigned int* t = (unsigned int*)(x + 4);
        m14 = t;
        unsigned int* e = t + 2;
        unsigned int* p = (unsigned int*)((char*)t + *(unsigned int*)(x + 8) + 4);
        m18 = p;
        m1C = (char*)p + *p;
        unsigned int n = t[0];
        for (unsigned int i = 0; i < n; i++, e++) {
            unsigned int* q = (unsigned int*)((char*)e + *e);
            if (*q != 0) {
                *q = (unsigned int)((char*)q + *q);
            }
        }
        mValid = 1;
    } else {
        mSwapped = 1;
        ChunkFileHeader_unk* h = (ChunkFileHeader_unk*)mData;
        m10 = (ChunkTable*)((char*)&h->m14 + Swap32_unk(h->m14));
        char* x = (char*)m10 + Swap32_unk(m10->m04);
        unsigned int* t = (unsigned int*)(x + 4);
        m14 = t;
        unsigned int* e = t + 2;
        unsigned int* p = (unsigned int*)((char*)t + Swap32_unk(*(unsigned int*)(x + 8)) + 4);
        m18 = p;
        m1C = (char*)p + Swap32_unk(*p);
        unsigned int n = Swap32_unk(t[0]);
        for (unsigned int i = 0; i < n; i++, e++) {
            unsigned int v = *e;
            __stwbrx(v, e, 0);
            unsigned int* q = (unsigned int*)((char*)e + Swap32_unk(v));
            if (*q != 0) {
                *q = (unsigned int)q + Swap32_unk(*q);
            }
        }
        SwapInPlace_unk(&((ChunkFileHeader_unk*)mData)->m00);
        SwapInPlace_unk(&((ChunkFileHeader_unk*)mData)->m04);
        SwapInPlace_unk(&((ChunkFileHeader_unk*)mData)->m08);
        SwapInPlace_unk(&((ChunkFileHeader_unk*)mData)->m0C);
        SwapInPlace_unk(&((ChunkFileHeader_unk*)mData)->m10);
        SwapInPlace_unk(&((ChunkFileHeader_unk*)mData)->m14);
        SwapInPlace_unk(&((ChunkFileHeader_unk*)mData)->m18);
        SwapInPlace_unk(&((ChunkFileHeader_unk*)mData)->m1C);
        SwapInPlace_unk(&m10->mCount);
        SwapInPlace_unk(&m10->m04);
        Chunk* c = m10->mChunks;
        for (unsigned int i = 0; i < m10->mCount; i++, c++) {
            SwapInPlace_unk((unsigned int*)&c->mType);
            SwapInPlace_unk(&c->mSize);
        }
        SwapInPlace_unk((unsigned int*)m18);
        mValid = 1;
    }
}

ChunkFile_unk::~ChunkFile_unk() {}

int ChunkFile_unk::IsSwapped() const {
    return mSwapped;
}

unsigned int ChunkFile_unk::GetChunkCount() const {
    if (mValid) {
        return m10->mCount;
    }
    return 0;
}

ChunkFile_unk::Chunk* ChunkFile_unk::FindChunk(int type, unsigned int index) {
    unsigned int n = 0;
    Chunk* c = m10->mChunks;
    for (unsigned int i = 0; i < m10->mCount; i++, c++) {
        if (c->mType == type) {
            if (n == index) {
                return c;
            }
            n++;
        }
    }
    return 0;
}
