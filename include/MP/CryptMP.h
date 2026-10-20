#ifndef MP_CRYPTMP_H
#define MP_CRYPTMP_H

// Key generator/checker of the multiplayer packets (MP/CryptMP.cpp). Member names are invented.

#include "atn/net/PacketPack.h"

namespace atn {
namespace net {

// inferred: abstract base (vtable 0x80790908: destructor and five pure virtuals); the
// destructor frees two buffers (weak copy at 0x804AE4C4)
class Crypt {
public:
    virtual ~Crypt();
    virtual char* vf0C() = 0;
    virtual unsigned int vf10() = 0;
    virtual int vf14(unsigned char* dst, unsigned int dstSize, const unsigned char* src, unsigned int len) = 0;
    virtual int vf18(unsigned char* data, int maxLen) = 0;
    virtual unsigned char* vf1C(int flag, unsigned char* buf, int len) = 0;

    int m04;
    char* m08;
    char* m0C;
};

} // namespace net
} // namespace atn

namespace MP {

class CryptMP : public atn::net::Crypt {
public:
    virtual ~CryptMP();
    virtual char* vf0C();
    virtual unsigned int vf10();
    virtual int vf14(unsigned char* dst, unsigned int dstSize, const unsigned char* src, unsigned int len);
    virtual int vf18(unsigned char* data, int maxLen);
    virtual unsigned char* vf1C(int flag, unsigned char* buf, int len);

    unsigned char m10; // inferred: m14 is ready
    char* m14;         // inferred: generated key (2 * vf10() characters)
    char* m18;         // inferred: key scrambled with fn_806B3C20
};

} // namespace MP

#endif
