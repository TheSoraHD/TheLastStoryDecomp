// inferred: Event::PostSequence and Event::SequenceRoot share one TU (their RTTI names are
// packed together in .rodata). SequenceRoot is declared first: vtables are emitted in reverse
// declaration order.
#include "Event/SequenceRoot.h"
#include "Event/PostSequence.h"
#include "Event/Manager.h"
#include "Event/Sequence.h"
#include "atn/Light_unk.h"
#include "atn/RenderSetting_unk.h"
#include "game/GameField_unk.h"
#include "Event/Cut.h"
#include "Event/Section.h"
#include "Event/Channel.h"
#include "Event/Action.h"
#include "Event/Label.h"

extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings (frame speed at 0x3A4)

namespace Event {

// inferred: same body as in Event/Sequence.cpp (declared inline in Event/Sequence.h)
inline Cut* Sequence::FindCutByIndex_unk(int index) {
    for (int i = 0; i < (int)mB8.size(); i++) {
        if (index == mB8[i]->mIndex) {
            return mB8[i].get();
        }
    }
    return 0;
}

// inferred: target of a jump parameter (0x8000: start of the cut, otherwise a label); sets the cut
// or the frame of the label in the cut itself (inlined three times in fn_8047B768)
static inline void JumpTarget_unk(Sequence* seq, Cut* cut, ActionParam_unk* param, Cut*& result, int& frame) {
    int value = param->m04.i;
    if (value & 0x8000) {
        int index = value & ~0x8000;
        if (seq->FindCutByIndex_unk(index) != cut) {
            result = seq->FindCutByIndex_unk(index);
        }
    } else {
        Label* label = seq->fn_805412A8(value);
        if (label->mCut != cut) {
            result = label->mCut;
        } else {
            frame = label->mFrame;
        }
    }
}

// NONMATCHING: 99.3% - registers only (original: next cut r31, section r30, channel r29)
// inferred: cut that follows `cut`: the first jump of the jump channels (kind 8 of the sections of
// kind 1) to another cut, else the next cut. Names invented.
Cut* fn_8047B768(Sequence* seq, Cut* cut) {
    Cut* result = 0;
    Cut* next = seq->FindCutByIndex_unk(cut->fn_8053B028());
    int frame = 0;
    for (int i = 0; i < (int)cut->mSections.size(); i++) {
        Section* section = cut->GetSection_unk(i);
        if (section->m14 != 0 && section->m0C == 1) {
            for (int j = 0; j < (int)section->mChannels.size(); j++) {
                Channel* channel = section->GetChannel_unk(j);
                if (channel->m10 != 0 && channel->m0C == 8) {
                    for (int k = 0; k < (int)channel->mActions.size(); k++) {
                        Action* action = channel->GetAction_unk(k);
                        if (action->m10 >= frame) {
                            int type = action->GetParam_unk(0)->m04.i;
                            if (type == 0) {
                                JumpTarget_unk(seq, cut, action->GetParam_unk(1), result, frame);
                            } else if (type == 1) {
                                JumpTarget_unk(seq, cut, action->GetParam_unk(1), result, frame);
                            } else if (type == 2 || type == 3) {
                                JumpTarget_unk(seq, cut, action->GetParam_unk(5), result, frame);
                            }
                            if (result != 0) {
                                break;
                            }
                        }
                    }
                    if (result != 0) {
                        break;
                    }
                }
            }
            if (result != 0) {
                break;
            }
        }
    }
    if (result == 0 && next != cut) {
        result = next;
    }
    return result;
}

// inferred: one jump parameter of fn_8047BBBC (written out three times): a jump to another cut ends
// the cut at the action, a jump to a label of the cut itself skips the frames up to the label
#define JUMP_LENGTH_UNK(param)                                                                       \
    {                                                                                                \
        int value = (param)->m04.i;                                                                  \
        if (value & 0x8000) {                                                                        \
            int index = value & ~0x8000;                                                             \
            if (seq->FindCutByIndex_unk(index) != cut) {                                             \
                end = action->m10;                                                                   \
                done = 1;                                                                            \
                break;                                                                               \
            }                                                                                        \
        } else {                                                                                     \
            Label* label = seq->fn_805412A8(value);                                                  \
            if (label->mCut != cut) {                                                                \
                end = action->m10;                                                                   \
                done = 1;                                                                            \
                break;                                                                               \
            }                                                                                        \
            frame = label->mFrame;                                                                   \
            skipped += frame - action->m10;                                                          \
        }                                                                                            \
    }

// inferred: number of frames played in `cut` (its length minus the frames skipped by jumps and
// waits). Names invented.
int fn_8047BBBC(Sequence* seq, Cut* cut) {
    if (seq == 0 || cut == 0) {
        return 0;
    }
    seq->FindCutByIndex_unk(cut->fn_8053B028());
    int frame = 0;
    int end = cut->m2C;
    int skipped = 0;
    int done = 0;
    for (int i = 0; i < (int)cut->mSections.size(); i++) {
        Section* section = cut->GetSection_unk(i);
        if (section->m14 != 0 && section->m0C == 1) {
            for (int j = 0; j < (int)section->mChannels.size(); j++) {
                Channel* channel = section->GetChannel_unk(j);
                if (channel->m10 != 0 && (channel->m0C == 0x35 || channel->m0C == 8)) {
                    for (int k = 0; k < (int)channel->mActions.size(); k++) {
                        Action* action = channel->GetAction_unk(k);
                        if (action->m10 >= frame) {
                            if (action->m0C == 8) {
                                int type = action->GetParam_unk(0)->m04.i;
                                if (type == 0) {
                                    JUMP_LENGTH_UNK(action->GetParam_unk(1));
                                } else if (type == 1) {
                                    JUMP_LENGTH_UNK(action->GetParam_unk(1));
                                } else if (type == 2 || type == 3) {
                                    JUMP_LENGTH_UNK(action->GetParam_unk(5));
                                }
                            } else if (action->m0C == 0x35) {
                                if (action->m14 - action->m10 <= 0) {
                                    end = action->m10;
                                    done = 1;
                                    break;
                                }
                                skipped += action->m14 - action->m10;
                                frame = action->m14;
                            }
                        }
                    }
                    if (done) {
                        break;
                    }
                }
            }
            if (done) {
                break;
            }
        }
    }
    return end - skipped;
}
#undef JUMP_LENGTH_UNK

// NONMATCHING: 99.5% - registers only (original: section index r30, section count r12)
// inferred: first action of kind 0x2B (channels of the sections of kind 1) whose first parameter is
// 0, following the cuts from the current one. Names invented.
Action* fn_8047BFF4(Sequence* seq) {
    Cut* cut = seq != 0 ? seq->FindCutByIndex_unk(seq->m94) : 0;
    while (cut != 0) {
        int count = cut->mSections.size();
        for (int i = 0; i < count; i++) {
            Section* section = cut->GetSection_unk(i);
            if (section->m0C == 1) {
                int channels = section->mChannels.size();
                for (int j = 0; j < channels; j++) {
                    Channel* channel = section->GetChannel_unk(j);
                    if (channel->m0C == 0x2B) {
                        for (int k = 0; k < (int)channel->mActions.size(); k++) {
                            Action* action = channel->GetAction_unk(k);
                            if (action->GetParam_unk(0)->m04.i == 0) {
                                return action;
                            }
                        }
                    }
                }
            }
        }
        cut = fn_8047B768(seq, cut);
    }
    return 0;
}

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

int Manager::fn_8047E964(int id, int queue) {
    // NONMATCHING: 97.6% - the original reads m23B8 again for the index (tried a ring struct with
    // its own push, unsigned casts, a pointer to the entry)
    if (queue != 0 && (unsigned int)m23B8 < 8) {
        QueuedSequence_unk* entry = &m23BC[(m23FC + m23B8) & 7];
        entry->mId = id;
        entry->m04 = 0;
        m23B8++;
    } else {
        Sequence* seq = FindSequence_unk(id);
        if (seq == 0) {
            return 0;
        }
        seq->fn_8053F6EC();
    }
    return 1;
}

int Manager::fn_8047F268(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->m194 >= 5 || seq->m194 == 0;
}

int Manager::fn_8047F2EC(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 0;
    }
    return seq->m194 == 10;
}

int Manager::fn_8047F364(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->fn_8047F400() || seq->m194 == 0;
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

void Manager::fn_8047F91C() {
    Sequence* seq = m70;
    if (seq != 0) {
        seq->fn_80550CE0();
        fn_8047EFD8(seq->m190, 0);
        seq->fn_8053E8C8(14);
        fn_8048EC7C();
    }
}

void Manager::fn_8047F984(float time) {
    mB4 = time;
    lbl_8087EFA8->m3A4 = time;
}

GameMap_unk* Manager::fn_80480438() {
    if (m1A64 != 0) {
        return *m1A60;
    }
    if (lbl_8087F430 != 0) {
        return fn_80373148(lbl_8087F430);
    }
    return 0;
}

void Manager::fn_804805B4() {
    m1A70.clear();
    m23B8 = 0;
    fn_8047F580();
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
