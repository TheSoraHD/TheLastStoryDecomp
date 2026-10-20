#ifndef ATN_NET_PACKETPACK_H
#define ATN_NET_PACKETPACK_H

#include "atn/net/PPDATA.h"
#include "std/list.h"

struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void fn_80083AD4(MemoryManager_unk* mgr, void* p);

namespace atn {
namespace net {

class FuncMessage;
class Crypt;

// inferred: one peer of the session (element of PacketPack::mUsers, 0x920 bytes). Name invented.
struct PPUser_unk {
    // inferred: the id is stored in the body, after the queues are built (0x8050D7EC)
    PPUser_unk(unsigned int id) : mActive(1), mId(-1), m08(0), m0C(0), m10(0) { mId = id; }
    ~PPUser_unk() {
        if (m08 != 0) {
            fn_80083AD4(fn_800827E0(), m08);
            m08 = 0;
        }
        if (m0C != 0) {
            fn_80083AD4(fn_800827E0(), m0C);
            m0C = 0;
        }
    }

    unsigned char mActive; // 0x00
    unsigned int mId;      // 0x04: inferred: aid of the peer
    unsigned short* m08;   // 0x08: inferred: next sequence number per message id
    unsigned short* m0C;   // 0x0C: inferred: last received sequence number per message id
    unsigned int m10;      // 0x10: number of message ids
    PPDATAU mU;            // 0x20
    PPDATAR mR;            // 0x4A0
};

// inferred: packet packer (atn/PacketPack.cpp, vtable 0x80791718); names invented. The members
// after the virtual functions are defined in atn/PPDATAU.cpp.
class PacketPack {
public:
    virtual ~PacketPack();
    // 0x804AE3C4 (weak copy, defined inline in MP/CryptMP.cpp: atn/PPDATAU.cpp calls it without
    // emitting a copy): inferred: end marker id
    virtual int vf0C();
    virtual unsigned char vf10();    // inferred: number of message ids
    virtual unsigned char vf14(int arg);
    virtual FuncMessage* vf18(unsigned char id);

    int fn_8050D7EC(unsigned int id); // inferred: adds (or reactivates) a peer
    int fn_8050DC58(int reliable, int id); // inferred: sends the queues that are due
    void fn_8050DDDC(unsigned int id, int reliable);
    void fn_8050DE34();
    void fn_8050DE5C();
    void fn_8050DEA8(); // inferred: syncs the peers with the connected aids
    int fn_8050E094(int id, int arg, const void* src, int reliable); // inferred: queues a message
    int fn_8050E510(Crypt* crypt);
    void* fn_8050E530(void* data, unsigned int size);
    int fn_8050E598(unsigned int id, const unsigned char* header); // inferred: 1 = old sequence number
    void fn_8050E62C(int ok, const char* fmt, ...); // inferred: assert (empty in release)

    // inferred: marks the peer `id` as gone
    void __Deactivate_unk(unsigned int id) {
        std::list<PPUser_unk>::iterator end = mUsers.end();
        for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != end; ++it) {
            if (id == (*it).mId) {
                (*it).mActive = 0;
                return;
            }
        }
    }

    std::list<PPUser_unk> mUsers; // 0x04
    Crypt* m10;
};

} // namespace net
} // namespace atn

#endif
