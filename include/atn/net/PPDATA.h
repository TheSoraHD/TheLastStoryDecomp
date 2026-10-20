#ifndef ATN_NET_PPDATA_H
#define ATN_NET_PPDATA_H

namespace atn {
namespace net {

// inferred from __vt__Q33atn3net6PPDATA (0x80793168): queue of packed messages towards one peer.
// The data members are declared before the virtual functions: the vtable pointer is at 0x460 and
// the object takes 0x480 bytes (32-byte aligned buffer).
class PPDATA {
public:
    int m00;                                  // inferred: frames left before the queue is sent
    int m04;                                  // inferred: bytes used in mData
    char mData[0x440] __attribute__((aligned(32))); // 0x20

    PPDATA() { vf08(); }
    virtual void vf08();     // 0x8050DBCC: clears the queue
    virtual void vf0C() = 0; // inferred: reloads m00
};

// inferred: unreliable queue (vtable 0x80793140)
class PPDATAU : public PPDATA {
public:
    virtual void vf08(); // 0x8050DB74
    virtual void vf0C(); // 0x8050DBE8
};

// inferred: reliable queue (vtable 0x80793118)
class PPDATAR : public PPDATA {
public:
    virtual void vf08(); // 0x8050DBF4
    virtual void vf0C(); // 0x8050DC4C
};

} // namespace net
} // namespace atn

#endif
