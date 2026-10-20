// inferred: reader of XSF files (chunks of {size, type, flags}, "XSF@" magic, byte swapped when the
// magic is "@FSX") and the curves read from them (one sorted map of keys per channel). Names
// invented; the std::map code of the curves (0x805BEFE4, 0x805BF214, 0x805BF4B4, 0x805BF590,
// 0x805BF794, 0x805BF8F4, 0x805BFA54) is not reconstructed.

extern "C" int fn_8006969C(); // inferred: nonzero when the data must be byte swapped

// inferred: chunk header
struct XsfChunk_unk {
    unsigned int mSize;
    unsigned short mType;  // 0x04
    unsigned short mFlags; // 0x06
};

// inferred: curve set: 13 channels (sorted maps of keys, 16 bytes) at 0x24
struct XsfCurveSet_unk {
    unsigned char m00[0x24];
    struct Channel_unk {
        int mCount;
        unsigned char m04[0xC];
    } mChannels[13]; // 0x24

    int fn_805BF1E8(int channel);
    float fn_805BF204(float time, int channel);
    float fn_805BF214(float time, Channel_unk* channel);
    void fn_805BF410(float time, float* out, int x, int y, int z);
    int fn_805BF4B4(int channel, float time, float v0, float v1, float v2, float v3, float v4);
    int fn_805BF590(int channel, float time, float value);
};

// inferred: byte swap written with the rlwimi intrinsic (slwi + three rlwimi in the original)
static inline unsigned int __Swap32_unk(unsigned int x) {
    unsigned int r = x << 24;
    r = __rlwimi(r, x, 8, 24, 31);
    r = __rlwimi(r, x, 24, 16, 23);
    r = __rlwimi(r, x, 8, 8, 15);
    return r;
}

static inline void __SwapInPlace32_unk(unsigned int* p) {
    __stwbrx(*p, p, 0);
}

static inline void __SwapInPlace16_unk(unsigned short* p) {
    __sthbrx(*p, p, 0);
}

static inline float __SwapF_unk(float f) {
    float r;
    __stwbrx(*(unsigned int*)&f, &r, 0);
    return r;
}

// inferred: keys of a chunk with the other byte order
// NONMATCHING: 50.4% - the original swaps each float through a stack temporary whose address is
// recomputed in the loop; here the temporaries' addresses are kept in saved registers.
extern "C" void fn_805BE8FC(void* self, unsigned int* chunk, XsfCurveSet_unk* curves, int channel) {
    unsigned int count = __Swap32_unk(chunk[0]);
    unsigned int size = __Swap32_unk(chunk[1]);
    float* p = (float*)(chunk + 2);
    for (unsigned int i = 0; i < count; i++) {
        float time = __SwapF_unk(p[0]);
        float value = __SwapF_unk(p[1]);
        if (size > 8) {
            curves->fn_805BF4B4(channel, time, value, __SwapF_unk(p[2]), __SwapF_unk(p[3]), __SwapF_unk(p[4]),
                                __SwapF_unk(p[5]));
        } else {
            curves->fn_805BF590(channel, time, value);
        }
        p = (float*)((char*)p + (size & ~3));
    }
}

// inferred: keys of a chunk
// NONMATCHING: 99.5% - count/step registers swapped (r31/r29)
extern "C" void fn_805BEA34(void* self, unsigned int* chunk, XsfCurveSet_unk* curves, int channel) {
    unsigned int size = chunk[1];
    unsigned int count = chunk[0];
    float* p = (float*)(chunk + 2);
    for (unsigned int i = 0; i < count; i++) {
        float time = p[0];
        float value = p[1];
        if (size > 8) {
            curves->fn_805BF4B4(channel, time, value, p[2], p[3], p[4], p[5]);
        } else {
            curves->fn_805BF590(channel, time, value);
        }
        p = (float*)((char*)p + (size & ~3));
    }
}

static const char sMagic[] = "XSF@";
static const char sMagicSwapped[] = "@FSX";

// inferred: header check of an XSF file
class XsfFile_unk {
public:
    XsfFile_unk(void* data);
    ~XsfFile_unk();
    void fn_805BEB54();

    unsigned int* mData; // 0x00
    void* m04;
    int mValid;          // 0x08
    int mSwapped;        // 0x0C
    XsfChunk_unk* mFirst; // 0x10
};

XsfFile_unk::XsfFile_unk(void* data) {
    mData = (unsigned int*)data;
    m04 = data;
    mValid = 0;
    mSwapped = 0;
    mFirst = 0;
    fn_805BEB54();
}

XsfFile_unk::~XsfFile_unk() {}

// NONMATCHING: 94.6% - register allocation of the magic addresses and of the swap loop
void XsfFile_unk::fn_805BEB54() {
    unsigned int magic = *(unsigned int*)sMagic;
    unsigned int swapped = *(unsigned int*)sMagicSwapped;
    unsigned int value = *mData;
    if (fn_8006969C() != 0) {
        magic = __Swap32_unk(magic);
        swapped = __Swap32_unk(swapped);
    }
    if (value == magic) {
        mValid = 1;
        mSwapped = 0;
    } else if (value == swapped) {
        mValid = 1;
        mSwapped = 1;
        for (XsfChunk_unk* p = (XsfChunk_unk*)(mData + 1); p->mSize != 0;
             p = (XsfChunk_unk*)((char*)p + p->mSize)) {
            __SwapInPlace32_unk(&p->mSize);
            __SwapInPlace16_unk(&p->mType);
            __SwapInPlace16_unk(&p->mFlags);
        }
    } else {
        mValid = 0;
    }
    mFirst = (XsfChunk_unk*)(mData + 1);
}

// inferred: position in the chunk list
class XsfIter_unk {
public:
    static bool IsValid(XsfChunk_unk* p) { return p != 0 && p->mSize != 0; }
    static unsigned short GetType(XsfChunk_unk* p) { return p != 0 ? p->mType : 0; }
    static unsigned int GetSize(XsfChunk_unk* p) { return p != 0 ? p->mSize : 0; }

    void fn_805BEC68(XsfChunk_unk* p);
    int fn_805BEC70();
    XsfChunk_unk* fn_805BECA0();
    XsfChunk_unk* fn_805BED50();
    int fn_805BEE04();

    XsfChunk_unk* mChunk;
};

void XsfIter_unk::fn_805BEC68(XsfChunk_unk* p) {
    mChunk = p;
}

int XsfIter_unk::fn_805BEC70() {
    if ((mChunk != 0 ? mChunk->mFlags : 0) & 1) {
        return 1;
    }
    return 0;
}

// inferred: next chunk of the same level
XsfChunk_unk* XsfIter_unk::fn_805BECA0() {
    XsfChunk_unk* p = mChunk;
    if (IsValid(p)) {
        unsigned short type = GetType(p);
        do {
            p = (XsfChunk_unk*)((char*)p + p->mSize);
            if (!IsValid(p)) {
                break;
            }
            if (type == GetType(p)) {
                return p;
            }
            if (type > GetType(p)) {
                break;
            }
        } while (p->mSize != 0);
    }
    return 0;
}

// inferred: first child chunk
XsfChunk_unk* XsfIter_unk::fn_805BED50() {
    XsfChunk_unk* p = mChunk;
    if (IsValid(p)) {
        unsigned int type = GetType(p) + 1;
        do {
            p = (XsfChunk_unk*)((char*)p + p->mSize);
            if (!IsValid(p)) {
                break;
            }
            if (type == GetType(p)) {
                return p;
            }
            if (type > GetType(p)) {
                break;
            }
        } while (p->mSize != 0);
    }
    return 0;
}

// inferred: number of child chunks
// NONMATCHING: 87.3% - the original keeps `first == 0` as a value and a running pointer; tried a
// running pointer instead of the offset (73%).
int XsfIter_unk::fn_805BEE04() {
    XsfChunk_unk* first = mChunk;
    unsigned int type = GetType(first) + 1;
    int count = 0;
    int offset = 0;
    for (;;) {
        XsfChunk_unk* p = (XsfChunk_unk*)((char*)first + offset + GetSize(first));
        if (!IsValid(p)) {
            return count;
        }
        if (type > GetType(p)) {
            return count;
        }
        if (type == GetType(p)) {
            count++;
        }
        offset += GetSize(p);
    }
}

int XsfCurveSet_unk::fn_805BF1E8(int channel) {
    return mChannels[channel].mCount != 0;
}

// NONMATCHING: only because fn_805BF214 (std::map lookup) is not reconstructed
float XsfCurveSet_unk::fn_805BF204(float time, int channel) {
    return fn_805BF214(time, &mChannels[channel]);
}

// NONMATCHING: only because fn_805BF214 (std::map lookup) is not reconstructed
void XsfCurveSet_unk::fn_805BF410(float time, float* out, int x, int y, int z) {
    out[0] = fn_805BF214(time, &mChannels[x]);
    out[1] = fn_805BF214(time, &mChannels[y]);
    out[2] = fn_805BF214(time, &mChannels[z]);
}
