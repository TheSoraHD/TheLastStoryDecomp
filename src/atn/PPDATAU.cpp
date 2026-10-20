// inferred: peers and per-peer message queues of atn::net::PacketPack (PPDATA/PPDATAU/PPDATAR).
#include "atn/net/PacketPack.h"
#include "MP/CryptMP.h"
#include "MP/FuncMessage.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

extern "C" void* fn_800838B8(MemoryManager_unk* mgr, unsigned long size, unsigned long align, int heap,
                             const char* file, const char* func, int line);

// inferred: global network state (class unknown, see gmk/Item.cpp)
extern "C" unsigned char fn_806B0E30();
extern "C" int fn_806A8E40();
struct NetState_unk {
    unsigned char m000[0x1F8];
    unsigned int m1F8_31_20 : 12;
    unsigned int mDisabled : 1; // inferred
    unsigned int m1F8_18_0 : 19;
    unsigned char m1FC[0xC215 - 0x1FC];
    unsigned char mC215;

    // inferred: own aid
    unsigned char GetMyAid_unk() {
        if (mC215 == 0) {
            return mDisabled ? 0 : fn_806B0E30();
        } else {
            return mDisabled ? 0 : fn_806A8E40();
        }
    }
};
extern "C" NetState_unk* lbl_8087F628;
extern "C" int fn_8050BA68(unsigned char** aids); // inferred: list of connected aids
extern "C" void fn_8050B7EC(NetState_unk* net, const void* data, unsigned int size, unsigned char aid,
                            int reliable); // inferred: sends a packet
extern "C" int fn_805089E0(atn::net::Crypt* crypt, int decode, const void* data, unsigned int* size);

// inferred: getter of the packer (out of line, see MP/CryptMP.cpp)
atn::net::PacketPack* fn_804AE3BC();

// inferred: shared 4 KB work buffer (lbl_8087F600) allocated on first use; its literals belong to
// the copy emitted by atn/NetManager (lbl_8087E1B0, lbl_8087E1AC)
extern "C" void* lbl_8087F600;
extern "C" const char lbl_8087E1B0[1];
extern "C" const char lbl_8087E1AC[1];
inline void* __GetWorkBuffer_unk() {
    if (lbl_8087F600 == 0) {
        lbl_8087F600 = fn_800838B8(fn_800827E0(), 0x1000, 4, 5, lbl_8087E1B0, lbl_8087E1AC, 0);
    }
    return lbl_8087F600;
}

// inferred: empty trace of a message (its arguments are still evaluated)
inline void __TraceMessage_unk(int id, unsigned int size) {}

// inferred: sequence number arrays of a new peer (two inline helpers: each call has its own
// literal objects in .sdata)
inline unsigned short* __AllocSeqSend_unk(unsigned int size) {
    return (unsigned short*)fn_800838B8(fn_800827E0(), size, 1, 5, "", "", 0);
}
inline unsigned short* __AllocSeqRecv_unk(unsigned int size) {
    return (unsigned short*)fn_800838B8(fn_800827E0(), size, 1, 5, "", "", 0);
}

// inferred: number of frames a queue waits before being sent (unreliable / reliable)
int lbl_8087E468 = 5;
int lbl_8087E46C = 3;

namespace atn {
namespace net {

// NONMATCHING: 97.7% - register allocation only: the original keeps `id` in r28 and the PPDATA
// vtable in r31 (four saved registers); here `id` takes r31 and the vtable r29. Tried: a local end,
// the id stored by the PPUser_unk constructor, unsigned int count.
int PacketPack::fn_8050D7EC(unsigned int id) {
    std::list<PPUser_unk>::iterator end = mUsers.end();
    for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != end; ++it) {
        if (id == (*it).mId) {
            fn_8050E62C((*it).mActive != 1, "in adduser");
            (*it).mU.vf08();
            (*it).mR.vf08();
            if ((*it).m08 != 0) {
                memset((*it).m08, 0, (*it).m10 * 2);
            }
            if ((*it).m0C != 0) {
                memset((*it).m0C, 0, (*it).m10 * 2);
            }
            (*it).mActive = 1;
            return 1;
        }
    }
    PPUser_unk user(id);
    mUsers.push_back(user);
    PPUser_unk& added = mUsers.back();
    unsigned int count = vf10();
    unsigned int size = count * 2;
    added.m08 = __AllocSeqSend_unk(size);
    memset(added.m08, 0, size);
    added.m0C = __AllocSeqRecv_unk(size);
    memset(added.m0C, 0, size);
    added.m10 = count;
    return 1;
}

void PPDATAU::vf08() {
    PPDATA::vf08();
    vf0C();
}

void PPDATA::vf08() {
    m04 = 0;
    m00 = 0;
    memset(mData, 0, sizeof(mData));
}

void PPDATAU::vf0C() {
    m00 = lbl_8087E46C;
}

void PPDATAR::vf08() {
    PPDATA::vf08();
    vf0C();
}

void PPDATAR::vf0C() {
    m00 = lbl_8087E468;
}

// NONMATCHING: 99.7% - only the order of the two first addi (string pool / list end). Tried the
// end declared first or last.
int PacketPack::fn_8050DC58(int reliable, int id) {
    unsigned char sent[8];
    int sentCount = 0;
    int result = 0;
    int index = 0;
    std::list<PPUser_unk>::iterator end = mUsers.end();
    for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != end; ++it, index++) {
        PPDATA* data = reliable == 0 ? (PPDATA*)&(*it).mU : (PPDATA*)&(*it).mR;
        if ((*it).mActive == 1 && data->m04 != 0 && (id == -1 || id == (*it).mId)) {
            if (result != 1) {
                result = 2;
            }
            if (--data->m00 <= 0) {
                unsigned int size = data->m04 + 1;
                data->vf0C();
                const void* buf;
                if (m10 != 0) {
                    fn_805089E0(m10, 0, data->mData, &size);
                    buf = m10->m08;
                } else {
                    buf = data->mData;
                }
                fn_8050B7EC(lbl_8087F628, buf, size, (*it).mId, reliable);
                fn_8050E62C(data->mData[data->m04] == (unsigned char)vf0C(), "assert in sendmessage\n");
                sent[sentCount] = index;
                result = 1;
                sentCount++;
                data->m04 = 0;
            }
        }
    }
    return result;
}

void PacketPack::fn_8050DDDC(unsigned int id, int reliable) {
    while (fn_8050DC58(reliable, id) != 0) {
    }
}

void PacketPack::fn_8050DE34() {
    std::list<PPUser_unk>::iterator end = mUsers.end();
    for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != end; ++it) {
        (*it).mR.m04 = 0;
        (*it).mU.m04 = 0;
    }
}

void PacketPack::fn_8050DE5C() {
    fn_8050DEA8();
    fn_8050DC58(0, -1);
    fn_8050DC58(1, -1);
}

// NONMATCHING: 99.5% - register allocation: the aid count and the end of the second loop swap
// r30/r31, and the inline lookup loads the id before the list end. Tried: the end declared first or
// after the count, local ends in the inner loops, the lookup inline with and without a local end.
void PacketPack::fn_8050DEA8() {
    unsigned char* aids;
    std::list<PPUser_unk>::iterator end;
    int count = fn_8050BA68(&aids);
    for (int i = 0; i < count; i++) {
        if (aids[i] != lbl_8087F628->GetMyAid_unk()) {
            int found = 0;
            std::list<PPUser_unk>::iterator end1 = mUsers.end();
            for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != end1; ++it) {
                if ((*it).mActive == 1 && aids[i] == (*it).mId) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                fn_8050D7EC(aids[i]);
            }
        }
    }
    end = mUsers.end();
    for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != end; ++it) {
        if ((*it).mActive == 1) {
            int found = 0;
            for (int i = 0; i < count; i++) {
                if (aids[i] != lbl_8087F628->GetMyAid_unk() && aids[i] == (*it).mId) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                __Deactivate_unk((*it).mId);
            }
        }
    }
}

// NONMATCHING: 95.5% - same instructions, register allocation differs: the original gives the
// outer loop (iterator r22, end r14, message r20) the low registers and the scan of the queue the
// high ones; here it is the other way round.
int PacketPack::fn_8050E094(int id, int arg, const void* src, int reliable) {
    FuncMessage* msg = vf18(vf14(arg));
    if (msg == 0) {
        return 0;
    }
    for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != mUsers.end(); ++it) {
        if ((*it).mActive != 1) {
            continue;
        }
        if (id != -1 && id != (*it).mId) {
            continue;
        }
        PPDATA* data = reliable == 0 ? (PPDATA*)&(*it).mU : (PPDATA*)&(*it).mR;
        unsigned short seq = (*it).m08[(unsigned char)msg->vf08()]++;
        int pos = -1;
        int offset = data->m04;
        if (offset != 0 && msg->vf18() == 1) {
            int used = data->m04;
            int msgId = (unsigned char)msg->vf08();
            const char* start = data->mData;
            const char* cur = start;
            int found = 0;
            for (;;) {
                const char* prev = cur;
                if (prev - start >= used) {
                    break;
                }
                PacketPack* pack = fn_804AE3BC();
                int curId = *(unsigned char*)cur;
                FuncMessage* m = pack->vf18(curId);
                if (m == 0) {
                    curId = pack->vf0C();
                } else {
                    void* buf = __GetWorkBuffer_unk();
                    cur += 4;
                    m->vf14(buf, 0x1000, &cur, 0);
                }
                if ((unsigned char)curId == (unsigned char)fn_804AE3BC()->vf0C()) {
                    break;
                }
                if ((unsigned char)curId == msgId && msg->vf1C(prev, src) == 1) {
                    found = 1;
                    cur = prev;
                    break;
                }
            }
            if (!found) {
                pos = -1;
            } else {
                pos = cur - data->mData;
            }
            if (pos != -1) {
                offset = pos;
            }
        }
        __TraceMessage_unk(msg->vf08(), msg->vf0C());
        int size = msg->vf0C();
        if (offset + size + 5 >= 0x3DF) {
            fn_8050DDDC((*it).mId, reliable);
            offset = 0;
        }
        char* dst = data->mData + offset;
        memset(dst, msg->vf08(), 1);
        memset(dst + 2, seq, 2);
        msg->vf10(dst + 4, src);
        if (pos == -1) {
            data->m04 += size + 4;
            dst[size + 4] = vf0C();
        }
    }
    return 1;
}

int PacketPack::fn_8050E510(Crypt* crypt) {
    if (m10 != 0) {
        return 1;
    }
    m10 = crypt;
    return 1;
}

void* PacketPack::fn_8050E530(void* data, unsigned int size) {
    if (m10 == 0) {
        return 0;
    }
    if (fn_805089E0(m10, 1, data, &size) == 0) {
        return 0;
    }
    return m10->m0C;
}

int PacketPack::fn_8050E598(unsigned int id, const unsigned char* header) {
    std::list<PPUser_unk>::iterator end = mUsers.end();
    for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != end; ++it) {
        if ((*it).mActive == 1 && id == (*it).mId) {
            unsigned char msg = header[0];
            unsigned short seq = *(unsigned short*)(header + 2);
            unsigned short last = (*it).m0C[msg];
            if (last <= seq) {
                (*it).m0C[msg] = seq;
                break;
            }
            if (last - seq >= 0x8000) {
                (*it).m0C[msg] = seq;
                break;
            }
            return 1;
        }
    }
    return 0;
}

} // namespace net
} // namespace atn

// inferred: reads one message of a received packet (called by MP::CryptMP)
extern "C" int fn_8050E410(atn::net::PacketPack* pack, unsigned char** p, unsigned int mode) {
    unsigned char id = **p;
    atn::net::FuncMessage* msg = pack->vf18(id);
    if (msg == 0) {
        return pack->vf0C();
    }
    if (mode == 1) {
        __TraceMessage_unk(msg->vf08(), msg->vf0C());
    }
    void* buf = __GetWorkBuffer_unk();
    *p += 4;
    msg->vf14(buf, 0x1000, (const char**)p, mode);
    return id;
}

// inferred: sets (unless -1) and returns the frame count of the unreliable queues
extern "C" int fn_8050E618(int count) {
    if (count != -1) {
        lbl_8087E46C = count;
    }
    return lbl_8087E46C;
}

namespace atn {
namespace net {

void PacketPack::fn_8050E62C(int ok, const char* fmt, ...) {}

} // namespace net
} // namespace atn
