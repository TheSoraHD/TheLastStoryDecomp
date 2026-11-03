// inferred: Event::PostSequence and Event::SequenceRoot share one TU (their RTTI names are
// packed together in .rodata). SequenceRoot is declared first: vtables are emitted in reverse
// declaration order.
#include "Event/SequenceRoot.h"
#include "Event/PostSequence.h"
#include "Event/Manager.h"
#include "Event/Sequence.h"
#include "atn/Light_unk.h"

namespace Event {

inline Sequence* Manager::FindSequence_unk(int id) {
    if (m70 != 0 && id == m70->m190) {
        return m70;
    }
    for (int i = 0; i < (int)m64.size(); i++) {
        if (id == m64[i]->m190) {
            return m64[i];
        }
    }
    return 0;
}

int Manager::fn_8047F2EC(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 0;
    }
    return seq->m194 == 10;
}

// inferred: the sequence is ending (states 13 and 14)
int Sequence::fn_8047F400() {
    return m194 == 13 || m194 == 14;
}

int Manager::fn_8047F420(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->m50 == 1;
}

int Manager::fn_8047F498(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->m50 == 3;
}

int Manager::fn_8047F510(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->m98_8;
}

int Manager::fn_8047F6B0(int arg) {
    m1AA4 = arg;
    Step1AA0_unk();
    return m1AA0;
}

void Manager::fn_8047F6E4() {
    if (m1AAC == 0) {
        return;
    }
    Step1AA0_unk();
    if (m1AA0 == 3) {
        m1AAC = 0;
    }
}

void Manager::fn_8047F850(int a, int b, int c) {
    m1AA8->m54 = a;
    m1AA8->m6C = b;
    m1AA8->m70 = c;
    m1AA8->m4C = 0;
    m1AA8->m48 = 1;
    m1AA8->m58 = 0;
}

void Manager::fn_8047F88C(int arg) {
    if (arg > 0) {
        m1AAC = 1;
        m1AA4 = arg;
    }
}

void Manager::fn_8047F8A4(float value) {
    m1AA8->m74 = value;
}

void Manager::fn_8047F8B0() {
    m1AA8->m48 = 0;
}

void Manager::fn_8047F8C0(int arg) {
    if (m1AA8 != 0) {
        m1AA8->fn_800D246C(arg);
    }
    if (m1A9C != 0) {
        m1A9C->fn_800D246C(arg);
    }
}

int Manager::fn_80481654() {
    return m1A7C->IsFrozen_unk() == 0;
}

int Manager::fn_80481668() {
    int result = 0;
    if (!m1A7C->IsFlag_unk(4)) {
        if (m1A7C->m104 >= 0.0f) {
            result = 1;
        }
    }
    return result;
}

void Manager::fn_8048169C(int arg) {
    if (arg == 0) {
        if (m1A90 == 0) {
            m1A90 = 1;
            m1A8C = 5;
        }
    } else {
        m1A7C->m100 = 0.0f;
        m1A7C->FreezeTask();
        m1A90 = 0;
        m1A8C = -1;
    }
}

void Manager::fn_804817D4() {
    if (!m1A80->IsFlag_unk(4)) {
        m1A80->FreezeTask();
        m1A88 = 1;
    }
}

void Manager::fn_80481800(int on) {
    m1EA4 = on;
    if (on != 0) {
        m1EB0 = 0;
    }
}

// inferred: removes the lights of the event
void Manager::fn_80484FBC() {
    int n = lbl_8087EEF8->mCount;
    for (int i = 0; i < n; i++) {
        lbl_8087EEF8->fn_8007A154(0);
    }
    m1F10.clear();
}

void Manager::fn_8048F184(int index) {
    if ((unsigned int)index > 1) {
        return;
    }
    if (index == 0) {
        if (!m23AC->IsFlag_unk(4)) {
            m23AC->FreezeTask();
        }
    } else {
        if (!m23B0->IsFlag_unk(4)) {
            m23B0->FreezeTask();
        }
    }
}

PostSequence::~PostSequence() {
    sInstance = 0;
}

void PostSequence::vf14() {}

int PostSequence::vf0C() { return 1; }

SequenceRoot::~SequenceRoot() {
    sInstance = 0;
}

void SequenceRoot::vf14() {}

void SequenceRoot::vf10() {}

int SequenceRoot::vf0C() { return 1; }

} // namespace Event
