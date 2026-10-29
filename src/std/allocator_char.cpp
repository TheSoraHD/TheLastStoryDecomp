// std/allocator_char.cpp: reader of the tagged text files (TextParser_unk, 0x800E0CF0-0x800E19AC) and
// the weak MSL iostream code around it (std::basic_stringbuf<char>, std::basic_ostream<char>
// sentries, the iostream __sinit). The unit name comes from the old automatic split; the real file
// is a game file (probably the same as atn/WordWrap_unk and std/char_traits_char, see
// docs/batch-auto-10kb-qA2.md). Parser and MSL stream internals are partially reconstructed.
#include "std/string.h"
#include "std/string_members.h"
#include "std/ios_init.h"

namespace std {
namespace tr1 {
namespace detail {
struct shared_ptr_deleter_common {
    void release();
};
} // namespace detail
} // namespace tr1
} // namespace std

// inferred: reader of the tagged text files ("<Name args...>", "</Name>", "<!-- -->"); token type 1
// opens a block, 2 closes it, 3 is the text after a block, 4 a comment (names invented)
struct TextParser_unk {
    TextParser_unk(void* data, void* size); // inferred: size passed as a pointer-sized word

    const char* mData;
    unsigned int mSize;
    unsigned int mPos;
    std::string mName;  // 0x0C
    std::string mLast;  // 0x18
    char mBuf[0x100];   // 0x24
    int mType;          // 0x124
    int mClosed;        // 0x128: inferred: the last block was "<Name/>"
};

static inline bool IsSpace_unk(char c) {
    return c == 9 || c == 10 || c == 13 || c == 0x20;
}

// inferred: appends one character (two for a Shift-JIS lead byte) to the buffer
static inline void AppendChar_unk(char* buf, char c) {
    unsigned long n = strlen(buf);
    buf[n] = c;
    buf[n + 1] = 0;
}

extern "C" void fn_800E1374(TextParser_unk* parser);

TextParser_unk::TextParser_unk(void* data, void* size)
    : mData((const char*)data), mSize((unsigned int)size), mPos(0) {
    mBuf[0] = 0;
    mType = 0;
    mClosed = 0;
    fn_800E1374(this);
}

extern "C" {

struct StreamSentry_unk {
    unsigned char mReady;
    unsigned char mNested;
    char mPad[2];
    void* mStream;
};

extern void fn_800E0A64(void*, int);
extern void fn_8068B39C(const char*);
void* fn_800E14A4(StreamSentry_unk*, void*);
void* fn_800E1630(StreamSentry_unk*, int);
void* fn_800E1A48(void*);

static int SyncStream_unk(void* stream) {
    void** vtable = *(void***)stream;
    return ((int (*)(void*))vtable[7])(stream);
}

// inferred: deleting destructor for the char ostream subobject at offset 8.
// NONMATCHING: 99.97% - MWCC passes delete flag -1 instead of 0 to ios_base::~ios_base.
void* fn_800E0B18(void* self, int doDelete) {
    if (self != 0) {
        void** vtable = *(void***)self;
        *(int*)((char*)vtable + 0x3C) = (int)((char*)self + 8 - (char*)vtable);
        if (doDelete != 0) {
            char* base = (char*)self + 8;
            if (base != 0) {
                ((std::ios_base*)base)->std::ios_base::~ios_base();
            }
        }
        if (doDelete > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// inferred: deleting destructor for basic_stringbuf<char>; string is at +0x2C.
// NONMATCHING: 99.69% - one embedded string pointer uses a different register.
void* fn_800E0B90(void* self, int doDelete) {
    if (self != 0) {
        if ((char*)self + 0x2C != 0) {
            unsigned int flags = *(unsigned int*)((char*)self + 0x2C);
            if ((flags >> 31) != 0) {
                ::operator delete(*(void**)((char*)self + 0x34));
            }
        }
        if (self != 0) {
            void* owner = (char*)self + 0x1C;
            if (owner != 0) {
                if (owner != 0) {
                    std::tr1::detail::shared_ptr_deleter_common* control =
                        *(std::tr1::detail::shared_ptr_deleter_common**)((char*)owner + 4);
                    if (control != 0) {
                        control->release();
                    }
                }
            }
        }
        if (doDelete > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// inferred: base streambuf destructor releases its shared locale owner at +0x1C.
void* dtor_800E0C20(void* self, int doDelete) {
    if (self != 0) {
        void* owner = (char*)self + 0x1C;
        if (owner != 0) {
            if (owner != 0) {
                std::tr1::detail::shared_ptr_deleter_common* control =
                    *(std::tr1::detail::shared_ptr_deleter_common**)((char*)owner + 4);
                if (control != 0) {
                    control->release();
                }
            }
        }
        if (doDelete > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// inferred: deleting destructor for the shared owner subobject.
void* dtor_800E0C8C(void* self, int doDelete) {
    if (self != 0) {
        if (self != 0) {
            std::tr1::detail::shared_ptr_deleter_common* control =
                *(std::tr1::detail::shared_ptr_deleter_common**)((char*)self + 4);
            if (control != 0) {
                control->release();
            }
        }
        if (doDelete > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// inferred: vtable thunk adjusting the virtual base before deleting the ostream.
// NONMATCHING: 0% - MWCC inlines the deleting destructor instead of emitting a tail thunk.
void* fn_800E2A10(void* self, int doDelete) {
    int offset = *(int*)((char*)self + 0x3C);
    return fn_800E0B18((char*)self + offset - 8, doDelete);
}

// inferred: basic_ostream<char>::sentry constructor; stream state is at +0x32.
// NONMATCHING: 57.25% - guessed sentry state flow; the original uses nested stream wrappers.
void* fn_800E14A4(StreamSentry_unk* self, void* stream) {
    self->mReady = 0;
    self->mNested = 0;
    self->mStream = stream;
    if (*(unsigned char*)((char*)*(void**)stream + 0x32) == 0) {
        void* tied = *(void**)((char*)*(void**)stream + 0x34);
        if (tied != 0) {
            StreamSentry_unk nested;
            fn_800E14A4(&nested, tied);
            nested.mNested = 1;
            void* tiedIos = *(void**)tied;
            void* buffer = *(void**)((char*)tiedIos + 0x24);
            if (buffer != 0 && SyncStream_unk(buffer) == -1) {
                fn_800E0A64(tied, 1);
            }
            unsigned char state = *(unsigned char*)((char*)tiedIos + 0x32);
            if ((state & 5) == 0 &&
                (*(unsigned short*)((char*)tiedIos + 0x30) & 0x2000) != 0 && nested.mNested == 0) {
                fn_800E1A48(tied);
            }
        }
        if (*(unsigned char*)((char*)*(void**)stream + 0x32) == 0) {
            self->mReady = 1;
            return self;
        }
    }
    {
        unsigned char* state = (unsigned char*)((char*)*(void**)stream + 0x32);
        *state |= 4;
        if (*(void**)((char*)*(void**)stream + 0x24) == 0) {
            *state |= 1;
        }
        if ((*state & *(unsigned char*)((char*)*(void**)stream + 0x33)) != 0) {
            extern const char lbl_80779AB8[];
            fn_8068B39C(lbl_80779AB8);
        }
    }
    return self;
}

// inferred: sentry cleanup checks tied streams and propagates synchronization failure.
// NONMATCHING: 29.76% - cleanup state flow is only partially reconstructed.
void* fn_800E1630(StreamSentry_unk* self, int doDelete) {
    if (self != 0) {
        void* stream = self->mStream;
        void* ios = *(void**)stream;
        unsigned char state = *(unsigned char*)((char*)ios + 0x32);
        if ((state & 5) == 0 &&
            (*(unsigned short*)((char*)ios + 0x30) & 0x2000) != 0 && self->mNested == 0) {
            void* tied = *(void**)((char*)ios + 0x34);
            if (tied != 0) {
                StreamSentry_unk nested;
                fn_800E14A4(&nested, tied);
                nested.mNested = 1;
                void* tiedIos = *(void**)tied;
                void* buffer = *(void**)((char*)tiedIos + 0x24);
                if (buffer != 0 && SyncStream_unk(buffer) == -1) {
                    fn_800E0A64(tied, 1);
                }
                if ((*(unsigned char*)((char*)tiedIos + 0x32) & 5) == 0 &&
                    (*(unsigned short*)((char*)tiedIos + 0x30) & 0x2000) != 0 && nested.mNested == 0) {
                    fn_800E1A48(tied);
                }
            }
        }
        if (doDelete > 0) {
            ::operator delete(self);
        }
    }
    return self;
}

// inferred: basic_ostream<char>::flush; returns the stream reference.
// NONMATCHING: 16.52% - flush/sentry exception handling is only partially reconstructed.
void* fn_800E1A48(void* stream) {
    void* ios = *(void**)stream;
    StreamSentry_unk sentry;
    fn_800E14A4(&sentry, stream);
    void* buffer = *(void**)((char*)ios + 0x24);
    if (sentry.mReady != 0 && buffer != 0 && SyncStream_unk(buffer) == -1) {
        fn_800E0A64(stream, 1);
    }
    fn_800E1630(&sentry, -1);
    return stream;
}

// inferred: true while there is text left
bool fn_800E0D54(TextParser_unk* parser) {
    return parser->mPos < parser->mSize;
}

// inferred: next token
void fn_800E0D70(TextParser_unk* parser) {
    parser->mClosed = 0;
    switch (parser->mType) {
    case 1:
        while (parser->mPos < parser->mSize) {
            if (parser->mData[parser->mPos] == 0x3E) {
                parser->mPos++;
                break;
            }
            if (parser->mData[parser->mPos] == 0x2F && parser->mData[parser->mPos + 1] == 0x3E) {
                parser->mClosed = 1;
                parser->mPos += 2;
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
        break;
    case 4:
        while (parser->mPos < parser->mSize) {
            if (parser->mData[parser->mPos] == 0x3E) {
                parser->mPos++;
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
        break;
    case 2:
    case 3:
        while (parser->mPos < parser->mSize) {
            if (parser->mData[parser->mPos] == 0x3C) {
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
        break;
    }
    if (parser->mClosed != 0) {
        parser->mType = 2;
        return;
    }
    if (parser->mData[parser->mPos] == 0x3C) {
        if (parser->mData[parser->mPos + 1] == 0x2F) {
            parser->mType = 2;
            parser->mPos += 2;
            return;
        }
        if (parser->mData[parser->mPos + 1] == 0x2D && parser->mData[parser->mPos + 2] == 0x2D) {
            parser->mType = 4;
            parser->mPos += 3;
            return;
        }
        parser->mType = 1;
        parser->mPos++;
        return;
    }
    if (parser->mType == 1) {
        parser->mType = 3;
        return;
    }
    parser->mType = 0;
}

// NONMATCHING: 99.69% - buffer pointer and size use different saved registers in the inlined string assignment.
// inferred: name of the current block
std::string* fn_800E0F84(TextParser_unk* parser) {
    if (parser->mClosed != 0) {
        return &parser->mLast;
    }
    parser->mBuf[0] = 0;
    for (unsigned int i = parser->mPos; i < parser->mSize; i++) {
        if (IsSpace_unk(parser->mData[i]) || parser->mData[i] == 0x3E) {
            break;
        }
        AppendChar_unk(parser->mBuf, parser->mData[i]);
        if ((unsigned char)parser->mData[i] >= 0x80) {
            AppendChar_unk(parser->mBuf, parser->mData[++i]);
        }
    }
    char* buf = parser->mBuf;
    parser->mName = buf;
    parser->mLast = parser->mName;
    return &parser->mName;
}

// NONMATCHING: 93.10% - locals use different saved registers from the original throughout the scan.
// inferred: argument `index` of the current block (quoted strings)
std::string* fn_800E114C(TextParser_unk* parser, int index) {
    parser->mBuf[0] = 0;
    if (parser->mType != 1) {
        parser->mName.clear();
        return &parser->mName;
    }
    unsigned int i = parser->mPos;
    while (i < parser->mSize) {
        if (IsSpace_unk(parser->mData[i]) || parser->mData[i] == 0x3E) {
            break;
        }
        if ((unsigned char)parser->mData[i] >= 0x80) {
            i++;
        }
        i++;
    }
    int count = 0;
    bool quoted = false;
    for (; i < parser->mSize; i++) {
        if (parser->mData[i] == 0x3E) {
            break;
        }
        if (!quoted && IsSpace_unk(parser->mData[i])) {
            continue;
        }
        if (parser->mData[i] == 0x22) {
            quoted = !quoted;
            if (!quoted) {
                count++;
            }
        } else if (quoted && index == count) {
            AppendChar_unk(parser->mBuf, parser->mData[i]);
            if ((unsigned char)parser->mData[i] >= 0x80) {
                AppendChar_unk(parser->mBuf, parser->mData[++i]);
            }
        }
        if (index < count) {
            break;
        }
    }
    parser->mName = parser->mBuf;
    return &parser->mName;
}

// inferred: skips the spaces and the declarations ("<?...?>", "<!...!>")
void fn_800E1374(TextParser_unk* parser) {
    for (;;) {
        while (parser->mPos < parser->mSize) {
            if (!IsSpace_unk(parser->mData[parser->mPos])) {
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
        if (parser->mPos >= parser->mSize) {
            return;
        }
        if (parser->mData[parser->mPos] != 0x3C) {
            return;
        }
        if (parser->mData[parser->mPos + 1] != 0x3F && parser->mData[parser->mPos + 1] != 0x21) {
            return;
        }
        parser->mPos += 3;
        while (parser->mPos < parser->mSize) {
            if ((parser->mData[parser->mPos - 1] == 0x3F || parser->mData[parser->mPos - 1] == 0x21) &&
                parser->mData[parser->mPos] == 0x3E) {
                parser->mPos++;
                break;
            }
            if ((unsigned char)parser->mData[parser->mPos] >= 0x80) {
                parser->mPos++;
            }
            parser->mPos++;
        }
    }
}

// inferred: simple MSL iostream vtable slots; the observed return-register values determine the
// C++ return values. Keep the automatic symbols until their class declarations are recovered.
void fn_800E24CC(void*) {}
void fn_800E24D0(void*) {}
int fn_800E1D4C(void*) { return 0; }
int fn_800E1D54(void*) { return -1; }
int fn_800E2504(void*) { return 0; }
int fn_800E2608(void*) { return -1; }
int fn_800E2668(void*) { return -1; }

// inferred: failed stream positioning returns streamoff(-1) with a zero conversion state.
struct StreamPos_unk {
    int mOffsetHigh;
    int mOffsetLow;
    int mState;
};
void fn_800E24D4(StreamPos_unk* pos, void*) {
    pos->mOffsetLow = -1;
    pos->mOffsetHigh = -1;
    pos->mState = 0;
}
void fn_800E24EC(StreamPos_unk* pos, void*) {
    pos->mOffsetLow = -1;
    pos->mOffsetHigh = -1;
    pos->mState = 0;
}

// inferred: get area and high-water pointers in the MSL string buffer.
struct StringBuf_unk;
struct StringBuf_unk {
    virtual int vf00();
    virtual int vf04();
    virtual int vf08();
    virtual int vf0C();
    virtual int vf10();
    virtual int vf14();
    virtual int vf18();
    virtual int vf1C();
    virtual int vf20();
    virtual int vf24();
    virtual int vf28();
    virtual int vf2C();
    virtual int vf30(int);
    virtual int vf34();

    int underflow();
    int pbackfail(int);
    int overflow(int);

    char* mEback;
    char* mGptr;
    char* mEgptr;
    char* mPbase;
    char* mPptr;
    char* mEpptr;
    char m1c[8];
    unsigned char mMode;
    char m25[3];
    char* mBufferEnd;
    std::string mString;
};

// inferred: read a sequence, using uflow when the get area is exhausted.
int fn_800E250C(StringBuf_unk* stream, char* dst, int n) {
    int available;
    int remaining = n;
    int* selected = &remaining;
    available = (int)(stream->mEgptr - stream->mGptr);
    if (available < remaining) {
        selected = &available;
    }
    int count = *selected;
    if (count > 0) {
        memcpy(dst, stream->mGptr, count);
        stream->mGptr += count;
        dst += count;
        remaining -= count;
    }
    int read = count;
    while (remaining > 0) {
        int c;
        if (stream->mGptr < stream->mEgptr) {
            char* current = stream->mGptr;
            stream->mGptr = current + 1;
            c = (unsigned char)*current;
        } else {
            c = stream->vf24();
        }
        if (c == -1) {
            break;
        }
        *dst = (char)c;
        read++;
        dst++;
        remaining--;
    }
    return read;
}

// inferred: consume one character after asking underflow to make the get area valid.
int fn_800E2610(StringBuf_unk* stream) {
    if (stream->vf20() == -1) {
        return -1;
    }
    char* current = stream->mGptr;
    stream->mGptr = current + 1;
    return (unsigned char)*current;
}

// inferred: write a sequence, using overflow when the put area is exhausted.
int fn_800E2670(StringBuf_unk* stream, const char* src, int n) {
    int available;
    int remaining = n;
    available = (int)(stream->mEpptr - stream->mPptr);
    int& selected = available < remaining ? available : remaining;
    int count = selected;
    if (count > 0) {
        memcpy(stream->mPptr, src, count);
        stream->mPptr += count;
        src += count;
        remaining -= count;
    }
    int written = count;
    while (remaining > 0) {
        int c;
        char ch = *src++;
        char* end = stream->mEpptr;
        char* current = stream->mPptr;
        if (current < end) {
            *current = ch;
            stream->mPptr = current + 1;
            c = (unsigned char)*current;
        } else {
            c = stream->vf30((unsigned char)ch);
        }
        if (c == -1) {
            break;
        }
        written++;
        remaining--;
    }
    return written;
}

int StringBuf_unk::overflow(int c) {
    if ((mMode & 0x10) == 0) {
        return -1;
    }
    if (c == -1) {
        return 0;
    }
    if (mPptr < mEpptr) {
        *mPptr = (char)c;
        mPptr++;
        if (mBufferEnd < mPptr) {
            mBufferEnd = mPptr;
        }
        if ((mMode & 0x08) != 0 && mEgptr < mBufferEnd) {
            mEgptr = mBufferEnd;
        }
        return c;
    }

    unsigned int putOffset = (unsigned int)(mPptr - mPbase);
    unsigned int highOffset = (unsigned int)(mBufferEnd - mPbase);
    unsigned int getOffset = (unsigned int)(mGptr - mEback);
    unsigned int newPutOffset = putOffset + 1;
    char ch = (char)c;
    mString.__replace_unk(mString.size(), 0, &ch, &ch + 1, std::random_access_iterator_tag());
    unsigned int capacity = (unsigned int)mString.capacity();
    unsigned int size = (unsigned int)mString.size();
    if (capacity - 1 > size) {
        mString.replace(size, 0, capacity - 1 - size, (char)0);
    } else {
        mString.replace(capacity - 1, size - (capacity - 1), 0, (char)0);
    }
    char* data = (char*)mString.data();
    size = (unsigned int)mString.size();
    if ((mMode & 0x08) != 0) {
        mEback = data;
        mGptr = data + getOffset;
        mEgptr = data + highOffset;
    }
    mPbase = data;
    mPptr = data + newPutOffset;
    mEpptr = data + size;
    mBufferEnd = data + size;
    return c;
}

int StringBuf_unk::underflow() {
    if (mGptr == 0) {
        return -1;
    }
    if (mBufferEnd < mPptr) {
        mBufferEnd = mPptr;
    }
    char* end = mBufferEnd;
    if (mEgptr < end) {
        mEgptr = end;
    }
    if (mGptr < mEgptr) {
        return (unsigned char)*mGptr;
    }
    return -1;
}

// inferred: putback one character, accepting a different character only in putback mode.
int StringBuf_unk::pbackfail(int c) {
    if (mEback >= mGptr) {
        return -1;
    }
    if (c == -1) {
        mGptr--;
        return 0;
    }
    if ((mMode & 0x10) == 0 && (signed char)mGptr[-1] != (signed char)c) {
        return -1;
    }
    mGptr--;
    *mGptr = (char)c;
    return c;
}

// inferred: seekoff/seekpos use an 8-byte streamoff plus a 4-byte conversion state.
void fn_800E2064(StreamPos_unk* result, StringBuf_unk* stream, int offsetHigh, unsigned int offsetLow,
                 int way, unsigned int which) {
    unsigned int mode = which & 0x18;
    if (mode == 0 || (mode == 0x18 && way == 2)) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    bool input = (mode & 0x10) != 0;
    bool output = (mode & 0x08) != 0;
    if ((output && (stream->mMode & 0x08) == 0) || (input && (stream->mMode & 0x10) == 0)) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    if ((input && stream->mGptr == 0) || (output && stream->mPptr == 0)) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    if (stream->mPptr != 0 && stream->mBufferEnd < stream->mPptr) {
        stream->mBufferEnd = stream->mPptr;
    }
    if (stream->mEgptr != 0 && stream->mEgptr < stream->mBufferEnd) {
        stream->mEgptr = stream->mBufferEnd;
    }
    int extent;
    int base;
    if (input) {
        extent = (int)(stream->mBufferEnd - stream->mPbase);
        if (way == 1) {
            base = 0;
        } else if (way == 2) {
            base = (int)(stream->mPptr - stream->mPbase);
        } else if (way == 4) {
            base = extent;
        } else {
            result->mOffsetHigh = -1;
            result->mOffsetLow = -1;
            result->mState = 0;
            return;
        }
    } else {
        extent = (int)(stream->mEgptr - stream->mEback);
        if (way == 1) {
            base = 0;
        } else if (way == 2) {
            base = (int)(stream->mGptr - stream->mEback);
        } else if (way == 4) {
            base = extent;
        } else {
            result->mOffsetHigh = -1;
            result->mOffsetLow = -1;
            result->mState = 0;
            return;
        }
    }
    long long position = ((long long)offsetHigh << 32) | offsetLow;
    position += base;
    if (position < 0 || position > extent) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    if ((stream->mMode & 1) != 0 && input && position != extent) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    if (input) {
        stream->mGptr = stream->mEback + (int)position;
    }
    if (output) {
        stream->mPptr = stream->mPbase + (int)position;
    }
    result->mOffsetHigh = 0;
    result->mOffsetLow = (int)position;
}

void fn_800E22E8(StreamPos_unk* result, StringBuf_unk* stream, const StreamPos_unk* pos,
                 unsigned int which) {
    unsigned int mode = which & 0x18;
    if (mode == 0) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    bool input = (mode & 0x10) != 0;
    bool output = (mode & 0x08) != 0;
    if ((output && (stream->mMode & 0x08) == 0) || (input && (stream->mMode & 0x10) == 0)) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    if ((input && stream->mGptr == 0) || (output && stream->mPptr == 0)) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    if (stream->mPptr != 0 && stream->mBufferEnd < stream->mPptr) {
        stream->mBufferEnd = stream->mPptr;
    }
    if (stream->mEgptr != 0 && stream->mEgptr < stream->mBufferEnd) {
        stream->mEgptr = stream->mBufferEnd;
    }
    int extent = input ? (int)(stream->mBufferEnd - stream->mPbase)
                       : (int)(stream->mEgptr - stream->mEback);
    long long position = ((long long)pos->mOffsetHigh << 32) | (unsigned int)pos->mOffsetLow;
    if (position < 0 || position > extent) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    if ((stream->mMode & 1) != 0 && input && position != extent) {
        result->mOffsetHigh = -1;
        result->mOffsetLow = -1;
        result->mState = 0;
        return;
    }
    if (input) {
        stream->mGptr = stream->mEback + (int)position;
    }
    if (output) {
        stream->mPptr = stream->mPbase + (int)position;
    }
    result->mOffsetHigh = pos->mOffsetHigh;
    result->mOffsetLow = pos->mOffsetLow;
    result->mState = pos->mState;
}

// inferred: the pair at +0x20 owns a reference-counted deleter/control block.
struct SharedPair_unk {
    unsigned int mObject;
    unsigned int mControl;
};
struct SharedOwner_unk {
    char mPad[0x20];
    SharedPair_unk* mPair;
};
struct SharedControl_unk {
    unsigned int mUnknown;
    unsigned int mReferences;
};
void fn_800E1D5C(SharedPair_unk* out, SharedOwner_unk* owner) {
    SharedPair_unk* source = owner->mPair;
    out->mObject = source->mObject;
    out->mControl = source->mControl;
    if (out->mControl != 0) {
        ((SharedControl_unk*)out->mControl)->mReferences++;
    }
}

} // extern "C"

template <>
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(
    const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str) {
    if (!str.is_long()) {
        mRaw = str.mRaw;
    } else {
        __zero();
        reserve(str.mLong.mSize);
        __replace_unk(0, 0, str.mLong.mData, str.mLong.mData + str.mLong.mSize,
                      std::random_access_iterator_tag());
    }
}
