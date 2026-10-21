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

// NONMATCHING: ~98% - registers of the buffer pointer and the size in the inlined std::string assign.
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

// NONMATCHING: ~90% - the original keeps the constant 1 of the space test in r31 (all the other
// saved registers shift by one); same instructions otherwise.
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

} // extern "C"
