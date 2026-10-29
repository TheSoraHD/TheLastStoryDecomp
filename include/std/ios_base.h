#ifndef STD_IOS_BASE_H
#define STD_IOS_BASE_H

// Partial reconstruction of MSL C++ <ios> (std::ios_base, std::basic_ios, std::fpos) as used by the
// game (no MSL C++ source is available). Names of the types and members come from the USA
// LastWorld_tools.rso (seekdir/openmode/iostate/fmtflags are nested types of ios_base, fpos<int>);
// the layout is inferred from ios_base::~ios_base (0x8068B50C), ios_base::setstate (0x800E0A64),
// basic_ostream::sentry (0x800E14A4) and the virtual-base offset 0x3C of basic_ostream
// (thunk @8@60@ 0x800E2A10). Data member names are invented.

namespace std {

typedef long long streamoff;
typedef long streamsize;

// inferred: 8-byte offset plus the conversion state (seekoff/seekpos return it through a hidden
// pointer: 0x800E24D4 stores -1, -1, 0)
template <class stateT>
class fpos {
public:
    fpos(streamoff off) : mOff(off), mState() {}
    operator streamoff() const { return mOff; }

    streamoff mOff;
    stateT mState;
};

typedef fpos<int> streampos;

class locale;

template <class C, class T>
class basic_streambuf;

class ios_base {
public:
    // inferred: values from setstate (0x800E0A64) and the sentry (0x800E14A4): good() tests 5
    enum iostate { goodbit = 0, badbit = 1, eofbit = 2, failbit = 4 };
    // inferred: values from basic_stringbuf (0x800E1E64, 0x800E2064)
    enum openmode { app = 0x01, ate = 0x02, binary = 0x04, in = 0x08, out = 0x10, trunc = 0x20 };
    // inferred: values from basic_stringbuf::seekoff (0x800E2064)
    enum seekdir { beg = 1, cur = 2, end = 4 };
    // inferred: only unitbuf is used (0x800E1630)
    enum fmtflags { unitbuf = 0x2000 };

    virtual ~ios_base(); // 0x8068B50C

    locale getloc() const; // 0x800E1D5C (weak copy, std/allocator_char)
    bool good() const { return mState == 0; }
    // inferred: tests badbit | failbit (0x800E1630)
    bool fail() const { return (mState & (badbit | failbit)) != 0; }
    unsigned short flags() const { return mFlags; }
    // inferred: inlined in the sentry (0x800E14A4); the string is the @STRING@ of clear in the RSO
    void clear(iostate state) {
        mState = state;
        if (mRdbuf == 0) {
            mState |= badbit;
        }
        if ((mState & mExceptions) != 0) {
            throw_failure("ios_base failure in clear");
        }
    }
    // weak copy 0x800E0A64 (atn/WordWrap_unk): clear(rdstate() | state)
    void setstate(iostate state);
    static void throw_failure(const char* msg); // 0x8068B39C

    // inferred: 0x8068B770 builds cin/cout/cerr/clog on __files (once, with guards)
    class Init {
    public:
        Init();  // 0x8068B770
        ~Init(); // 0x8068BC30
    };

    // inferred layout: ~ios_base frees 0x04, 0x10 and 0x18 and deletes the locale at 0x20
    void* mCallbacks_unk;        // 0x04
    unsigned long mCallbackNum;  // 0x08
    char m0C[0x14];
    locale* mLoc;                // 0x20
    void* mRdbuf;                // 0x24: setstate sets badbit without a stream buffer
    char m28[8];
    unsigned short mFlags;       // 0x30
    unsigned char mState;        // 0x32
    unsigned char mExceptions;   // 0x33
};

template <class C, class T>
class basic_ostream;

// inferred: the tied stream at 0x34; the object ends at 0x3C (offset of the virtual base slot)
template <class C, class T>
class basic_ios : public ios_base {
public:
    virtual ~basic_ios() {}
    basic_streambuf<C, T>* rdbuf() const { return (basic_streambuf<C, T>*)mRdbuf; }
    basic_ostream<C, T>* tie() const { return mTie; }

    basic_ostream<C, T>* mTie; // 0x34
    char m38[4];
};

} // namespace std

#endif
