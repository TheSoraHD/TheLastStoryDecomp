#ifndef EVENT_PUPPETEER_H
#define EVENT_PUPPETEER_H

#include "atn/Task.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "Event/Sequence.h"

namespace Event {

// Event::Puppeteer<T> (names from the vtables, Event/Puppeteer_CharaTask.cpp): task that moves a character
// or a model for an actor of the event (0xC0 bytes). Its virtuals are explicit specializations defined at
// the start of the file; member names are invented.
template <class T>
class Puppeteer : public atn::Task {
public:
    Puppeteer(Sequence* sequence, T* target)
        : atn::Task(sequence), m48(target), m50(sequence), m54(0), mB8(0), mB9(0) {
        m4C_0 = 1;
    }
    virtual ~Puppeteer() {}
    // inferred: weak copy 0x8000DDA0 for CharaTask (inlined for atn::Model); names invented
    static Puppeteer* Create_unk(Sequence* sequence, T* target) {
        return new ((MemoryTag)4, "", "", 0) Puppeteer(sequence, target);
    }
    T* GetTarget_unk() { return m48; } // inferred: weak copy 0x8000DE3C for CharaTask
    void SetFlag0_unk(int on) { m4C_0 = on != 0; } // inferred: inlined in Event::Actor::fn_8001256C
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    T* m48;                  // inferred: the driven character or model
    unsigned int m4C_0 : 1;  // inferred: set by the constructor
    unsigned int m4C_1 : 31;
    Sequence* m50;           // inferred: the sequence of the event
    atn::Matrix3x4* m54;     // inferred: matrix the target is placed on (0: its own position)
    atn::Matrix3x4 m58;
    atn::Matrix3x4 m88;
    unsigned char mB8;
    unsigned char mB9;
    unsigned char mBA[0xC0 - 0xBA];
};

} // namespace Event

#endif
