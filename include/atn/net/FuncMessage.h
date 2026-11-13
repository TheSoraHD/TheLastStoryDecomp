#ifndef ATN_NET_FUNCMESSAGE_H
#define ATN_NET_FUNCMESSAGE_H

// Base of the multiplayer messages (the MP::func_* classes are in MP/FuncMessage.h).

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);

namespace atn {
namespace net {

// inferred: abstract base without a destructor in its vtable
class FuncMessage {
public:
    virtual int vf08() = 0;          // inferred: message id
    virtual unsigned int vf0C() = 0; // inferred: size of the payload
    // inferred: vf10, vf14 and vf1C are shared by the vtables of all the MP::func_* classes, so they
    // are inline members of the base (weak copies 0x805006C0, 0x8050062C, 0x80500624)
    virtual void vf10(void* dst, const void* src) {
        if (src != 0) {
            memcpy(dst, src, vf0C());
        }
    }
    virtual void vf14(void* dst, int arg, const char** pos, unsigned int mode) {
        if (mode == 1 && *pos != 0) {
            memcpy(dst, *pos, vf0C());
        }
        *pos += vf0C();
    }
    virtual unsigned int vf18() = 0; // inferred unsigned: compared with cmplwi (0x8050E094)
    // inferred: compares a queued message with new data (atn::net::PacketPack 0x8050E094)
    virtual unsigned int vf1C(const char* data, const void* arg) { return 1; }
};

} // namespace net
} // namespace atn

#endif
