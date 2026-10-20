#ifndef ATN_NET_PACKETPACK_H
#define ATN_NET_PACKETPACK_H

namespace atn {
namespace net {

class FuncMessage;

// inferred: packet packer (atn/PacketPack.cpp, vtable 0x80791718); only the virtual functions are
// declared, names invented
class PacketPack {
public:
    virtual ~PacketPack();
    virtual int vf0C() { return 0; } // 0x804AE3C4 (weak copy)
    virtual void vf10();
    virtual void vf14();
    virtual FuncMessage* vf18(unsigned char id);
};

} // namespace net
} // namespace atn

#endif
