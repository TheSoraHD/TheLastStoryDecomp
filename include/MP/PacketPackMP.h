#ifndef MP_PACKETPACKMP_H
#define MP_PACKETPACKMP_H

#include "atn/net/PacketPack.h"
#include "atn/SingletonNoStatic.h"
#include "atn/net/FuncMessage.h"

namespace MP {

// inferred: multiplayer packet packer (vtable 0x80792A28; RTTI base list 0x80792A44). Its destructor
// is emitted in atn/PacketPack.cpp (0x804FC2F0). Declared before the MP::func_* classes
// (MP/FuncMessage.h) and after atn::net::FuncMessage: the RTTI and vtable of a class come after those of the classes declared later.
class PacketPackMP : public atn::net::PacketPack, public atn::SingletonNoStatic<PacketPackMP> {
public:
    // inferred: inline (weak copies 0x8050105C and 0x80501054, after the weak copies of the MP::func_*
    // classes in the original)
    virtual int vf0C() { return 0; }
    virtual unsigned char vf10() { return 0x41; }
    virtual unsigned char vf14(int code);
    virtual atn::net::FuncMessage* vf18(unsigned char id);
};

} // namespace MP

#endif
