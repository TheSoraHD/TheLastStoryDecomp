// std/allocator_char.cpp: reader of the tagged text files (TextParser_unk, 0x800E0CF0-0x800E19AC) and
// the weak MSL iostream code around it (std::basic_stringbuf<char>, std::basic_ostream<char>
// sentries, the iostream __sinit). The unit name comes from the old automatic split; the real file
// is a game file (probably the same as atn/WordWrap_unk and std/char_traits_char, see
// docs/batch-auto-10kb-qA2.md). Only the parser is decompiled: there is no MSL C++ iostream source.
#include "std/string.h"

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

// inferred: get area and high-water pointers in the MSL string buffer.
struct StringBuf_unk {
    void* mVtable;
    void* m04;
    char* mGptr;
    char* mEgptr;
    void* m10;
    char* mHighWater;
    char m18[0x10];
    char* mBufferEnd;
};
int fn_800E1D88(StringBuf_unk* stream) {
    if (stream->mGptr == 0) {
        return -1;
    }
    if (stream->mBufferEnd < stream->mHighWater) {
        stream->mBufferEnd = stream->mHighWater;
    }
    if (stream->mEgptr < stream->mBufferEnd) {
        stream->mEgptr = stream->mBufferEnd;
    }
    if (stream->mGptr < stream->mEgptr) {
        return (unsigned char)*stream->mGptr;
    }
    return -1;
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
    *out = *source;
    if (source->mControl != 0) {
        ((SharedControl_unk*)source->mControl)->mReferences++;
    }
}

} // extern "C"
