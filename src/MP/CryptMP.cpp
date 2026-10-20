#include "MP/CryptMP.h"

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" unsigned short OSCalcCRC16(const void* data, unsigned long len);
extern "C" unsigned long fn_806B3C20();

// inferred: engine memory manager (function-local static) and its free
struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void fn_80083AD4(MemoryManager_unk* mgr, void* p);

// inferred: global packet manager (lbl_8087F628) and packer (lbl_8087F62C)
extern "C" void* lbl_8087F628;
extern "C" atn::net::PacketPack* lbl_8087F62C;
extern "C" int fn_8050E410(atn::net::PacketPack* pack, unsigned char** p, int arg);

// inferred: getter of the packer, called out of line from about 200 places in the DOL (weak copy
// emitted after its first user): it is never inlined
__declspec(noinline) inline atn::net::PacketPack* fn_804AE3BC() {
    return lbl_8087F62C;
}

namespace std {
namespace tr1 {

// inferred: MSL TR1 <random> (no source available). The state is kept twice (2 * n words) and
// refilled one half at a time (0x804AE568).
template <class UIntType, int w, int n, int m, int r, UIntType a, int u, int s, UIntType b, int t,
          UIntType c, int l>
class mersenne_twister {
public:
    explicit mersenne_twister(unsigned long value) { seed(value); }
    mersenne_twister(const mersenne_twister& other) { *this = other; }

    void seed(unsigned long value) {
        x[0] = value;
        for (i = 1; i < n; i++) {
            x[i] = 1812433253 * (x[i - 1] ^ (x[i - 1] >> 30)) + i;
        }
    }

    UIntType min() const { return 0; }
    UIntType max() const {
        UIntType res = 0;
        for (int k = 0; k < w; k++) {
            res |= (UIntType)1 << k;
        }
        return res;
    }

    UIntType operator()() {
        if (i == n) {
            twist(0);
        } else if (i >= 2 * n) {
            twist(1);
        }
        UIntType z = x[i];
        i++;
        z ^= z >> u;
        z ^= (z << s) & b;
        z ^= (z << t) & c;
        z ^= z >> l;
        return z;
    }

    void twist(int phase);

    UIntType x[2 * n];
    int i;
};

template <class UIntType, int w, int n, int m, int r, UIntType a, int u, int s, UIntType b, int t,
          UIntType c, int l>
void mersenne_twister<UIntType, w, n, m, r, a, u, s, b, t, c, l>::twist(int phase) {
    if (phase == 0) {
        for (int j = n; j < 2 * n; j++) {
            UIntType y = (x[j - n] & 0x80000000) | (x[j - n + 1] & 0x7FFFFFFF);
            x[j] = (y >> 1) ^ x[j - n + m] ^ (y & 1 ? a : 0);
        }
    } else if (phase == 1) {
        int j;
        for (j = 0; j < n - m; j++) {
            UIntType y = (x[j + n] & 0x80000000) | (x[j + n + 1] & 0x7FFFFFFF);
            x[j] = (y >> 1) ^ x[j + n + m] ^ (y & 1 ? a : 0);
        }
        for (j = n - m; j < n - 1; j++) {
            UIntType y = (x[j + n] & 0x80000000) | (x[j + n + 1] & 0x7FFFFFFF);
            x[j] = (y >> 1) ^ x[j - n + m] ^ (y & 1 ? a : 0);
        }
        UIntType y = (x[2 * n - 1] & 0x80000000) | (x[0] & 0x7FFFFFFF);
        x[n - 1] = x[m - 1] ^ (y >> 1) ^ (y & 1 ? a : 0);
        i = 0;
    }
}

typedef mersenne_twister<unsigned long, 32, 624, 397, 31, 0x9908B0DF, 11, 7, 0x9D2C5680, 15, 0xEFC60000, 18>
    mt19937;

template <class RealType = double>
class uniform_real {
public:
    explicit uniform_real(RealType min_arg = RealType(0), RealType max_arg = RealType(1))
        : mMin(min_arg), mMax(max_arg) {}
    template <class Gen>
    RealType operator()(Gen& gen) {
        return mMin + (mMax - mMin) * ((gen() - gen.min()) / gen.max());
    }

    RealType mMin;
    RealType mMax;
};

// inferred: engine adapter giving values in [0, 1)
template <class Engine>
class __engine_holder_unk {
public:
    __engine_holder_unk(Engine e) : mEng(e) {}

    Engine mEng;
};

template <class Engine>
class __engine_holder2_unk : public __engine_holder_unk<Engine> {
public:
    __engine_holder2_unk(Engine e) : __engine_holder_unk<Engine>(e) {}
};

template <class Engine>
class __engine_wrapper_unk : public __engine_holder2_unk<Engine> {
public:
    __engine_wrapper_unk(Engine e) : __engine_holder2_unk<Engine>(e) {
        mFactor = 1.0 / (1.0 + (double)(this->mEng.max() - this->mEng.min()));
    }
    double operator()() { return mFactor * (double)this->mEng(); }
    double min() const { return 0.0; }
    double max() const { return 1.0; }

    double mFactor;
};

template <class Engine, class Distribution>
class variate_generator {
public:
    variate_generator(Engine e, Distribution d) : mEng(e), mDist(d) {}
    double operator()() { return mDist(mEng); }

    __engine_wrapper_unk<Engine> mEng;
    Distribution mDist;
};

} // namespace tr1
} // namespace std

typedef std::tr1::variate_generator<std::tr1::mt19937, std::tr1::uniform_real<double> > Random_unk;
// inferred: engine random generator (global)
extern "C" Random_unk* fn_800A2664();

// inferred: range of the key characters (two doubles in .data)
static double sRandMin_unk = 0.0;
static double sRandMax_unk = 1.0;

namespace MP {

// NONMATCHING: 95.2% - stack slots of the temporary engine/distribution copies (the original
// keeps them in ascending order from the first one, here the first one is placed last) and
// register choice in the last loop. Also the original keeps both range doubles in .data
// (0x807908C0 = 0.0, 0x807908C8 = 1.0); MWCC always puts a zero double in .bss (tried struct,
// array, static uniform_real, template statics, folded initializers).
char* CryptMP::vf0C() {
    __alloca(0); // inferred: the frame is addressed through r31 (only function of the DOL like this)
    if (m10 == 0) {
        Random_unk gen(std::tr1::mt19937(0xFC06A148),
                       std::tr1::uniform_real<double>(sRandMin_unk, sRandMax_unk));
        unsigned int k;
        for (k = 0; k < vf10() * 2; k++) {
            m14[k] = (int)(93.0 * gen()) + 0x21;
        }
        m14[k] = 0;
        m10 = 1;
    }
    int j = 0;
    unsigned int k;
    for (k = 0; k < vf10() * 2; k++) {
        if ((int)k % 2 == 0) {
            m18[j++] = m14[k];
        }
    }
    m18[j] = 0;
    unsigned long key = fn_806B3C20();
    for (unsigned int n = 0; n < vf10() / 4; n++) {
        key = ((unsigned long*)m18)[n] ^ key;
        ((unsigned long*)m18)[n] = key;
    }
    return m18;
}

unsigned int CryptMP::vf10() {
    return 0x20;
}

unsigned char* CryptMP::vf1C(int flag, unsigned char* buf, int len) {
    int k;
    unsigned char* result;
    if (flag == 0) {
        result = buf + len;
        unsigned char* p = result;
        for (k = 0; k < 16; k++) {
            *p++ = (int)(93.0 * (*fn_800A2664())()) + 0x21;
        }
    } else {
        result = &buf[len - 16];
    }
    return result;
}

int CryptMP::vf14(unsigned char* dst, unsigned int dstSize, const unsigned char* src, unsigned int len) {
    unsigned short crc = OSCalcCRC16(src, len);
    if (len + 2 > dstSize) {
        return 0;
    }
    memcpy(dst + len, &crc, 2);
    return 1;
}

// NONMATCHING: 94% - the original calls fn_804AE3BC out of line (weak copy emitted after this
// function); here it is inlined and the u8 narrowing of the vf0C result moves
int CryptMP::vf18(unsigned char* data, int maxLen) {
    if (lbl_8087F628 == 0) {
        return 0;
    }
    unsigned char* p = data;
    while (true) {
        unsigned char end = fn_804AE3BC()->vf0C();
        unsigned char c = fn_8050E410(fn_804AE3BC(), &p, 0);
        if (c == end) {
            break;
        }
        if (p - data > maxLen) {
            return 0;
        }
    }
    p++;
    unsigned short crc = OSCalcCRC16(data, p - data);
    unsigned short stored;
    memcpy(&stored, p, 2);
    return crc == stored;
}



CryptMP::~CryptMP() {
    char* p = m14;
    if (p != 0) {
        fn_80083AD4(fn_800827E0(), p);
        m14 = 0;
    }
    p = m18;
    if (p != 0) {
        fn_80083AD4(fn_800827E0(), p);
        m18 = 0;
    }
}

} // namespace MP

inline atn::net::Crypt::~Crypt() {
    char* p = m08;
    if (p != 0) {
        fn_80083AD4(fn_800827E0(), p);
        m08 = 0;
    }
    p = m0C;
    if (p != 0) {
        fn_80083AD4(fn_800827E0(), p);
        m0C = 0;
    }
}
