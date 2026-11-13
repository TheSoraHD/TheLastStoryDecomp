#include "MP/CryptMP.h"
#include "atn/MemoryManager.h"
#include "std/ios_init.h" // inferred: the __sinit (0x804AE7C4) constructs the MSL iostream statics

// inferred: inline in the class (weak copy 0x804AE3C4); defined here so that atn/PPDATAU.cpp, which
// calls it, does not emit a copy
inline int atn::net::PacketPack::vf0C() {
    return 0;
}

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" unsigned short OSCalcCRC16(const void* data, unsigned long len);
extern "C" unsigned long fn_806B3C20();

// inferred: engine memory manager (function-local static) and its free

// inferred: global packet manager (lbl_8087F628) and packer (lbl_8087F62C)
extern "C" void* lbl_8087F628;
extern "C" atn::net::PacketPack* lbl_8087F62C;
extern "C" int fn_8050E410(atn::net::PacketPack* pack, unsigned char** p, int arg);

// inferred: getter of the packer, called out of line from about 200 places in the DOL (weak copy
// emitted after its first user): it is never inlined
__declspec(noinline) inline atn::net::PacketPack* fn_804AE3BC() {
    return lbl_8087F62C;
}

#include "std/tr1_random.h"

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
        atn::MemoryManager::Instance()->Free(p);
        m14 = 0;
    }
    p = m18;
    if (p != 0) {
        atn::MemoryManager::Instance()->Free(p);
        m18 = 0;
    }
}

} // namespace MP

inline atn::net::Crypt::~Crypt() {
    char* p = m08;
    if (p != 0) {
        atn::MemoryManager::Instance()->Free(p);
        m08 = 0;
    }
    p = m0C;
    if (p != 0) {
        atn::MemoryManager::Instance()->Free(p);
        m0C = 0;
    }
}
