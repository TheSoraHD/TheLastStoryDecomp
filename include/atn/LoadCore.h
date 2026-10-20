#ifndef ATN_LOADCORE_H
#define ATN_LOADCORE_H

namespace atn {

// inferred from __vt__Q23atn8LoadCore and its constructor (0x80472FAC): a resource being loaded,
// identified by its path; base of atn::Load* (LoadMotion, ...).
class LoadCore {
public:
    LoadCore(const char* path); // 0x80472FAC
    virtual ~LoadCore();        // 0x8047304C
    virtual void vf0C();
    virtual void vf10(char* out); // 0x8046F5B4: inferred: writes a description

    void fn_80473104(unsigned char state); // inferred: sets mState
    void fn_804730D4();                    // inferred: adds a reference
    void fn_804730E4();                    // inferred: drops a reference
    void fn_80473130();
    // inferred: the state as an int (0x80473F50 compares it signed)
    int GetState_unk() const { return mState; }

    volatile unsigned char mState; // 0x04: inferred volatile (threaded loader): 0x80473104 reloads it after the store
    volatile unsigned char m05;
    char mPath[0x42];     // 0x06
    unsigned int m48;     // 0x48: inferred: hash of mPath (fn_800DC6B4)
    int mRefCount;        // 0x4C
    void* m50;
    void* m54;
    int m58;
};

} // namespace atn

#endif
