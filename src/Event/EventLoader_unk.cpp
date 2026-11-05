// inferred file name (splits-auto): loading of the event files of an Event::Sequence
// (".event" files under "event2/", their ".pk" package); methods of Event::Sequence defined in
// another file than Event/Sequence.cpp.
#include "atn/LH_Binary.h"
#include "atn/FileList.h"
#include "db/EventIdTable_unk.h"
#include "std/string_members.h"
#include "Event/Sequence.h"
#include "Event/Cut.h"
#include "Event/Action.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" char* strcpy(char* dst, const char* src);

extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" std::string fn_8006B2D8(const std::string& path); // inferred: name without extension

#include "Event/Manager.h"

namespace Event {

// inferred: size of the event data
int Sequence::fn_80575F0C() {
    if (m98_0 == 1) {
        return m1CC;
    }
    if (m1C4.fn_80473F50() != 0) {
        return 0;
    }
    return m1C4.fn_8047059C();
}

// inferred: event data
void* Sequence::fn_80575F6C() {
    if (m98_0 == 1) {
        return m1D0;
    }
    if (m1C4.fn_80473F50() != 0) {
        return 0;
    }
    return m1C4.fn_80470580();
}

// inferred: releases the event data
void Sequence::fn_80575FCC() {
    if (m98_0 == 1) {
        atn::FileFreeBuffer((char*)m1D0, false);
        m1D0 = 0;
        m1CC = 0;
    } else {
        m1C4.fn_80473F50();
        m1C4.fn_80473F88();
    }
}

// inferred: loads the event `path`
void Sequence::fn_80576030(const char* path, void* arg) {
    if (strstr(path, ".event") != 0) {
        fn_8057D5F4(path, arg);
        if (m190 == -1) {
            char name[0x100] = {0};
            const char* p = path;
            if (*p == '/') {
                p++;
            }
            p += strlen("event2/");
            strcpy(name, p);
            char* ext = strstr(name, ".event");
            if (ext != 0) {
                *ext = 0;
            }
            m190 = EventIdTable_unk::fn_8020F130()->fn_8020F75C(name);
        }
    } else {
        m194 = 0;
        return;
    }
    if (m194 == 4) {
        fn_80576258();
    }
    if (lbl_8087F540 != 0) {
        lbl_8087F540->fn_80480468(this, (unsigned int)arg);
    }
    m2A0 = (fn_8006B2D8(fn_8006B174(std::string(path))) + ".pk").c_str();
}

// inferred: a reader of ".xml" event files was stripped by the linker; only its literal is left in
// @stringBase0 (before the basic_string messages of the instantiated members). Reconstructed
// minimally so that the pool keeps its layout.
static int fn_Stripped_unk(const char* path) {
    return strstr(path, ".xml") != 0;
}

// inferred: flags the cuts that contain actions of kinds 0x10, 0x2F, 0x11 and 0x12
// NONMATCHING: 99.0% - Get_unk with an unsigned index gives the counter-first increments and `cut`
// declared before the loop leaves the counters in r31..r28 as in the original; only the volatile
// pointers differ (original: cut r9, group r10, track r11, cut offset r12; here r12/r9/r10/r11).
// Tried: pointers declared at the top in both orders, `i` declared first (back to cut in r31),
// operator[], a reference to the cut vector, int/unsigned outer counter.
void Sequence::fn_80576258() {
    Cut* cut;
    for (unsigned int i = 0; i < mB8.size(); i++) {
        cut = mB8[i].get();
        for (int j = 0; j < (int)cut->mSections.size(); j++) {
            Section* section = cut->GetSection_unk(j);
            if (section->m14 != 0) {
                for (int k = 0; k < (int)section->mChannels.size(); k++) {
                    Channel* channel = section->GetChannel_unk(k);
                    if (channel->m10 != 0) {
                        for (int l = 0; l < (int)channel->mActions.size(); l++) {
                            Action* action = channel->GetAction_unk(l);
                            int kind = action->m0C;
                            if (kind == 0x10) {
                                cut->m48 = 1;
                            } else if (kind == 0x2F) {
                                cut->m49 = 1;
                            } else if (kind == 0x11) {
                                cut->m4A = 1;
                            } else if (kind == 0x12) {
                                cut->m4B = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace Event
