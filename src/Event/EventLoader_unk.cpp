// inferred file name (splits-auto): loading of the event files of an Event::Sequence
// (".event" files under "event2/", their ".pk" package); used by Event::Manager and
// Event::Sequence. The functions keep their C names; `seq` is the sequence (layout local).
#include "atn/LH_Binary.h"
#include "atn/FileList.h"
#include "db/EventIdTable_unk.h"
#include "std/string_members.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" char* strcpy(char* dst, const char* src);

extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" std::string fn_8006B2D8(const std::string& path); // inferred: name without extension

// inferred: shared pointer element of the event vectors (8 bytes)
template <class T>
struct EventPtr_unk {
    T* mPtr;
    void* mCount;
};

// inferred: std::vector<boost::shared_ptr<T> > with a checked accessor
template <class T>
struct EventVec_unk {
    int Size_unk() const { return mSize; }
    bool IsValid_unk(int i) const { return i >= 0 && i < Size_unk(); }
    T* Get_unk(unsigned int i) {
        if (IsValid_unk(i)) {
            return mData[i].mPtr;
        }
        return 0;
    }

    EventPtr_unk<T>* mData;
    int mSize;
    int mCap;
};

// inferred: an action of the event (only its kind is read)
struct EventAction_unk {
    unsigned char m00[0x0C];
    int mKind; // 0x0C
};

struct EventTrack_unk {
    unsigned char m00[0x10];
    int m10; // inferred: nonzero if it has actions
    EventVec_unk<EventAction_unk> mActions; // 0x14
};

struct EventGroup_unk {
    unsigned char m00[0x14];
    int m14; // inferred: nonzero if it has tracks
    EventVec_unk<EventTrack_unk> mTracks; // 0x18
};

struct EventCut_unk {
    unsigned char m00[0x3C];
    EventVec_unk<EventGroup_unk> mGroups; // 0x3C
    unsigned char m48; // inferred: has actions of kind 0x10
    unsigned char m49; // kind 0x2F
    unsigned char m4A; // kind 0x11
    unsigned char m4B; // kind 0x12
};

// inferred: Event::Sequence, only the members used here
struct EventSequence_unk {
    unsigned char m000[0x98];
    unsigned int mLoaded : 1; // 0x98: inferred: data owned in m1CC/m1D0 instead of the handle
    unsigned char m09C[0xB8 - 0x9C];
    EventVec_unk<EventCut_unk> mCuts; // 0xB8
    unsigned char m0C4[0x190 - 0xC4];
    int mEventId; // 0x190
    int mState;   // 0x194
    unsigned char m198[0x1C4 - 0x198];
    atn::LH_Binary mFile; // 0x1C4
    int mSize;            // 0x1CC
    void* mData;          // 0x1D0
    unsigned char m1D4[0x2A0 - 0x1D4];
    atn::FixedString_unk<0x100> mPackName; // 0x2A0
};

extern "C" void fn_8057D5F4(EventSequence_unk* seq, const char* path, void* arg); // inferred: parses the file
extern "C" void* lbl_8087F540; // inferred: event sound/stage manager
extern "C" void fn_80480468(void* mgr, EventSequence_unk* seq, void* arg);
extern "C" void fn_80576258(EventSequence_unk* seq);

// inferred: size of the event data
extern "C" int fn_80575F0C(EventSequence_unk* seq) {
    if (seq->mLoaded == 1) {
        return seq->mSize;
    }
    if (seq->mFile.fn_80473F50() != 0) {
        return 0;
    }
    return seq->mFile.fn_8047059C();
}

// inferred: event data
extern "C" void* fn_80575F6C(EventSequence_unk* seq) {
    if (seq->mLoaded == 1) {
        return seq->mData;
    }
    if (seq->mFile.fn_80473F50() != 0) {
        return 0;
    }
    return seq->mFile.fn_80470580();
}

// inferred: releases the event data
extern "C" void fn_80575FCC(EventSequence_unk* seq) {
    if (seq->mLoaded == 1) {
        atn::FileFreeBuffer((char*)seq->mData, false);
        seq->mData = 0;
        seq->mSize = 0;
    } else {
        seq->mFile.fn_80473F50();
        seq->mFile.fn_80473F88();
    }
}

// inferred: loads the event `path`
extern "C" void fn_80576030(EventSequence_unk* seq, const char* path, void* arg) {
    if (strstr(path, ".event") != 0) {
        fn_8057D5F4(seq, path, arg);
        if (seq->mEventId == -1) {
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
            seq->mEventId = EventIdTable_unk::fn_8020F130()->fn_8020F75C(name);
        }
    } else {
        seq->mState = 0;
        return;
    }
    if (seq->mState == 4) {
        fn_80576258(seq);
    }
    if (lbl_8087F540 != 0) {
        fn_80480468(lbl_8087F540, seq, arg);
    }
    seq->mPackName = (fn_8006B2D8(fn_8006B174(std::string(path))) + ".pk").c_str();
}

// inferred: a reader of ".xml" event files was stripped by the linker; only its literal is left in
// @stringBase0 (before the basic_string messages of the instantiated members). Reconstructed
// minimally so that the pool keeps its layout.
extern "C" int fn_Stripped_unk(const char* path) {
    return strstr(path, ".xml") != 0;
}

// inferred: flags the cuts that contain actions of kinds 0x10, 0x2F, 0x11 and 0x12
// NONMATCHING: 99.0% - Get_unk with an unsigned index gives the counter-first increments and `cut`
// declared before the loop leaves the counters in r31..r28 as in the original; only the volatile
// pointers differ (original: cut r9, group r10, track r11, cut offset r12; here r12/r9/r10/r11).
// Tried: pointers declared at the top in both orders, `i` declared first (back to cut in r31),
// operator[], a reference to the cut vector, int/unsigned outer counter.
extern "C" void fn_80576258(EventSequence_unk* seq) {
    EventCut_unk* cut;
    for (unsigned int i = 0; i < seq->mCuts.mSize; i++) {
        cut = seq->mCuts.mData[i].mPtr;
        for (int j = 0; j < cut->mGroups.Size_unk(); j++) {
            EventGroup_unk* group = cut->mGroups.Get_unk(j);
            if (group->m14 != 0) {
                for (int k = 0; k < group->mTracks.Size_unk(); k++) {
                    EventTrack_unk* track = group->mTracks.Get_unk(k);
                    if (track->m10 != 0) {
                        for (int l = 0; l < track->mActions.Size_unk(); l++) {
                            EventAction_unk* action = track->mActions.Get_unk(l);
                            int kind = action->mKind;
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
