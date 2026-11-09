// Event/EventParser_unk.cpp (0x805763F8-0x8057E83C, file name invented): reader of the binary event files.
// The data is a list of chunks (tag, size, data); each tag has a handler in a table (.rodata 0x80760DE8)
// that fills the Event::Sequence being loaded. Handler names are built from the tags; the other names are
// invented.
#include "Event/Sequence.h"
#include "Event/Cut.h"
#include "Event/Action.h"
#include "Event/Path.h"
#include "Event/MotionData.h"
#include "std/string_members.h"

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" bool fn_800696B4();            // inferred: the platform is little endian (game/IffChunkFile_unk)
extern "C" void* fn_8006BA8C(const char* path, int* size, int flag); // inferred: loads a file

namespace Event {

// inferred: list of nodes owned by the reader (a class derived from std::vector, its destructor tests the
// address four times)
struct NodeList_unk : public std::vector<Node*> {};

// inferred: state of the reader (0x5C bytes, cleared when it is created by Sequence::fn_8057D5F4): the
// objects the next chunks refer to
struct ParserState_unk {
    ParserState_unk() {
        unsigned int* w = (unsigned int*)this;
        for (int i = 0; i < 0x17; i++) {
            w[i] = 0;
        }
    }
    // inferred: deletes the nodes of m04
    ~ParserState_unk() {
        for (int i = 0; i < (int)m04.size(); i++) {
            delete m04[i];
        }
    }
    int m00;
    NodeList_unk m04; // inferred: nodes owned by the reader
    Sequence* m10;      // inferred: the sequence being loaded
    Cut* m14;           // inferred: current cut ("CUT ")
    Label* m18;         // inferred: current label ("LABE")
    Section* m1C;       // inferred: current section ("SECT")
    Channel* m20;       // inferred: current channel ("CHAN")
    Action* m24;        // inferred: current action ("ACTI", "ACT2")
    ActionParam_unk* m28; // inferred: current parameter ("PARA", "PAST")
    MotionData* m2C;    // inferred: current data of each list ("..ID")
    AnimationData* m30;
    EffectData* m34;
    ModelData* m38;
    CommonData* m3C;    // inferred: dummy data ("DUID")
    StringData* m40;
    CameraData* m44;
    PoseData* m48;
    MovieData* m4C;
    Path* m50;
    Actor* m54;
    EffectLists_unk* m58; // inferred: current curve ("CURV")
};

static ParserState_unk* sState_unk; // inferred: the reader (lbl_8087F9B8)

inline void SetName_unk(Node* node, const char* s) {
    node->mName = s;
}
inline void SetName2_unk(String_unk& name, const char* s) {
    name.assign(s, s != 0 ? strlen(s) : 0);
}

inline void SetDataName_unk(CommonData* d, const std::string& name) {
    d->m08 = name;
}

// inferred: group chunks ("..GR"): their children are read as chunks of their own
static int ParseGroup_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return 0;
}

// inferred: positions of the message layouts (.bss of Event/Manager.cpp)
#include "Event/MessageLayoutPos_unk.h"

// inferred: converts the parameters of the old versions of some actions after reading
static void FixAction_unk(Action* action) {
    if (action->m0C == 0xC) {
        float x = action->fn_80539330(fn_800DC880("pos_x"))->m04.f;
        float offset = action->fn_80539330(fn_800DC880("offset"))->m04.f;
        if (x != 0.0f || offset != 0.0f) {
            float best = 999999.0f;
            int layout = 0;
            for (int i = 0; i < 3; i++) {
                float dx = x - lbl_807C8AB0[i].x;
                float dy = offset - lbl_807C8AB0[i].y;
                float d = dx * dx + dy * dy;
                if (d < best) {
                    best = d;
                    layout = i;
                }
            }
            ActionParam_unk* p = action->fn_80539330(fn_800DC880("layout"));
            p->m00 = 0x1F;
            p->m04.i = layout;
            p = action->fn_80539330(fn_800DC880("pos_x"));
            p->m00 = 1;
            p->m04.f = 0.0f;
            p = action->fn_80539330(fn_800DC880("offset"));
            p->m00 = 1;
            p->m04.f = 0.0f;
        }
    } else if (action->m0C == 0x21) {
        if (action->m18 != 0) {
            if (action->fn_80539330(fn_800DC880("transition"))->m04.i == 0) {
                int transition = action->m18;
                ActionParam_unk* p = action->fn_80539330(fn_800DC880("transition"));
                p->m00 = 0;
                p->m04.i = transition;
            }
            action->m18 = 0;
        }
    } else if (action->m0C == 0x1A) {
        if (action->GetParam_unk(0)->m04.i == 3 && action->m18 > 0) {
            int end = action->m10 + action->m18;
            action->m18 = 0;
            action->m14 = end;
        }
    }
}

// inferred: FixAction_unk on every action of the sequence
static void FixActions_unk(Sequence* seq) {
    for (int i = 0; i < (int)seq->mB8.size(); i++) {
        Cut* cut = seq->IsCutIndex_unk(i) ? seq->mB8[i].get() : 0;
        for (int j = 0; j < (int)cut->mSections.size(); j++) {
            Section* section = cut->GetSection_unk(j);
            for (int k = 0; k < (int)section->mChannels.size(); k++) {
                Channel* channel = section->GetChannel_unk(k);
                for (int l = 0; l < (int)channel->mActions.size(); l++) {
                    FixAction_unk(channel->GetAction_unk(l));
                }
            }
        }
    }
}

// inferred: header of the file ("EVEN")
static int ParseCAT_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    unsigned int magic;
    memcpy(&magic, data, 4);
    if (magic != 'EVEN') {
        char name[5];
        memcpy(name, data, 4);
        name[4] = 0;
    }
    return 4;
}

// inferred: settings of the sequence ("SEQU" and "SEQ2" records, 0x28 bytes; the flags moved in "SEQ2")
// inferred: flags of the sequence as stored in the file (copied to Sequence::m58)
struct SequenceStateRec_unk {
    unsigned short m00;
    unsigned char m02;
    unsigned char m03;
};
struct SequenceRecord_unk {
    unsigned int mFlags; // inferred: kind (bits 0-3), m54 (bits 4-7), then the flags of m98
    SequenceStateRec_unk mState;
    short m08;
    short m0A;
    float mPos[3];
    float mRot[3];
    float mYaw;
};

inline void SetFlag3_unk(Sequence* seq, int on) { seq->m98_3 = on != 0; }
inline void SetFlag4_unk(Sequence* seq, int on) { seq->m98_4 = on != 0; }
inline void SetFlag7_unk(Sequence* seq, int on) { seq->m98_7 = on != 0; }
inline void SetFlag8_unk(Sequence* seq, int on) { seq->m98_8 = on != 0; }
inline void SetFlag9_unk(Sequence* seq, int on) { seq->m98_9 = on != 0; }

// NONMATCHING: the copy of the state (m58) loads its bytes in reverse order and stores a temporary late
// (also tried a raw buffer and memberwise copies); the rest matches
static int ParseSEQ2_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Sequence* seq = sState_unk->m10;
    sState_unk->m14 = 0;
    sState_unk->m18 = 0;
    sState_unk->m1C = 0;
    sState_unk->m20 = 0;
    sState_unk->m24 = 0;
    sState_unk->m28 = 0;
    if (tag == 'SEQU') {
        SequenceRecord_unk rec;
        memcpy(&rec, data, sizeof(rec));
        seq->m50 = rec.mFlags & 0xF;
        seq->m54 = (rec.mFlags >> 4) & 0xF;
        SetFlag3_unk(seq, rec.mFlags & 0x20);
        SetFlag7_unk(seq, rec.mFlags & 0x40);
        SetFlag4_unk(seq, rec.mFlags & 0x80);
        SetFlag8_unk(seq, rec.mFlags & 0x100);
        SetFlag9_unk(seq, rec.mFlags & 0x200);
        SequenceStateRec_unk st = rec.mState;
        seq->m58.m00 = st.m00;
        seq->m58.m02 = st.m02;
        seq->m58.m03 = st.m03;
        seq->m94 = rec.m08;
        seq->m9C = atn::Vector3(rec.mPos[0], rec.mPos[1], rec.mPos[2]);
        seq->mA8 = atn::Vector3(rec.mRot[0], rec.mRot[1], rec.mRot[2]);
        seq->mB4 = rec.mYaw;
        return sizeof(rec);
    } else {
        SequenceRecord_unk rec;
        memcpy(&rec, data, sizeof(rec));
        seq->m50 = rec.mFlags & 0xF;
        seq->m54 = (rec.mFlags >> 4) & 0xF;
        SetFlag3_unk(seq, rec.mFlags & 0x100);
        SetFlag7_unk(seq, rec.mFlags & 0x200);
        SetFlag4_unk(seq, rec.mFlags & 0x400);
        SetFlag8_unk(seq, rec.mFlags & 0x800);
        SetFlag9_unk(seq, rec.mFlags & 0x1000);
        SequenceStateRec_unk st = rec.mState;
        seq->m58.m00 = st.m00;
        seq->m58.m02 = st.m02;
        seq->m58.m03 = st.m03;
        seq->m94 = rec.m08;
        seq->m9C = atn::Vector3(rec.mPos[0], rec.mPos[1], rec.mPos[2]);
        seq->mA8 = atn::Vector3(rec.mRot[0], rec.mRot[1], rec.mRot[2]);
        seq->mB4 = rec.mYaw;
        return sizeof(rec);
    }
}

// inferred: name of the sequence
// NONMATCHING (as the other String_unk names CUNA, LANA, SCNA, CHNA and ATNA): the original keeps the
// sequence in r30 and the length in r28 (registers of the inlined String_unk assignment, also the blocker
// of atn::Material_unk::fn_80080B34); tried a reference, a local, assign() with the length in a local
// (if/else, ternary) and a Node setter
static int ParseSENA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m10->SetName_unk(data);
    return size;
}

// inferred: message file of the sequence
static int ParseSEMS_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m10->SetMessage_unk(data);
    return size;
}

// inferred: adds a number to the list m5C of the sequence
static int ParseARNU_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Sequence* seq = sState_unk->m10;
    int value;
    memcpy(&value, data, 4);
    seq->m5C.push_back((int)value);
    return 4;
}

// inferred: generic version (the actors have an explicit specialization in Event/Sequence.cpp)
template <class T>
void DataList_unk<T>::Add_unk(const char* name, int arg) {
    this->push_back(T(mOwner, name, arg));
}

// inferred: adds a motion data (its id is offset by 0xEF)
static int ParseMOID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Sequence* seq = sState_unk->m10;
    int id;
    memcpy(&id, data, 4);
    id += 0xEF;
    seq->mC4.Add_unk("", id);
    sState_unk->m2C = &seq->mC4[seq->mC4.size() - 1];
    return 4;
}

static int ParseMONA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m2C->SetName_unk(data);
    return size;
}

// DRAFT: stub that returns the size
// NONMATCHING: stub - adds an animation data
static int ParseANID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParseANNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m30->SetName_unk(data);
    return size;
}

// DRAFT: stub that returns the size
// NONMATCHING: stub - adds an effect data
static int ParseEFID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParseEFNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m34->SetName_unk(data);
    return size;
}

// DRAFT: stub that returns the size
// NONMATCHING: stub - adds a model data
static int ParseMDID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParseMDNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m38->SetName_unk(data);
    return size;
}

// inferred: file of the model
static int ParseMDMA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m38->SetFile_unk(data);
    return size;
}

// DRAFT: stub that returns the size
// NONMATCHING: stub - adds a dummy data
static int ParseDUID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParseDUNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m3C->SetName_unk(data);
    return size;
}

// DRAFT: stub that returns the size
// NONMATCHING: stub - adds a camera data
static int ParseCAID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParseCANA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m44->SetName_unk(data);
    return size;
}

// DRAFT: stub that returns the size
// NONMATCHING: stub - adds a pose data
static int ParsePOID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParsePONA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m48->SetName_unk(data);
    return size;
}

// DRAFT: stub that returns the size
// NONMATCHING: stub - adds a movie data
static int ParseMVID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParseMVNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m4C->SetName_unk(data);
    return size;
}

// DRAFT: stub that returns the size
// NONMATCHING: stub - adds a path
static int ParsePATH_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParsePANA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m50->SetName2_unk(data);
    return size;
}

// DRAFT: stub
// NONMATCHING: stub - points of the path
static int ParsePOIN_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

// DRAFT: stub
// NONMATCHING: stub - adds an actor
static int ParseACID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParseACNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    if (sState_unk->m54 != 0) {
        sState_unk->m54->SetName_unk(data);
    }
    return size;
}

// DRAFT: stub
// NONMATCHING: stub - adds a stream data
static int ParseSTID_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

static int ParseSTNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    if (sState_unk->m40 != 0) {
        sState_unk->m40->SetName_unk(data);
    }
    return size;
}

// inferred: curve of the sequence (list at 0x184)
// NONMATCHING: order of the stores of the two vector constructors (the original clears the data and capacity
// words of both before the sizes)
static int ParseCURV_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    int id;
    memcpy(&id, data, sizeof(id));
    Sequence* seq = sState_unk->m10;
    EffectLists_unk lists;
    seq->fn_8054204C(id, lists);
    sState_unk->m58 = seq->fn_80543060(id);
    return sizeof(id);
}

// DRAFT: stub
// NONMATCHING: stub - points of the curve
static int ParseCVPT_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    return size;
}

// inferred: record of a cut (0x14 bytes)
struct CutRecord_unk {
    int mIndex;
    unsigned int mFlags; // inferred: skip (bits 0-3), cinematic (bit 4)
    int mNextCut;
    int m0C;
    int m10;
};

static int ParseCUT_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Cut* cut = sState_unk->m10->fn_80540A08();
    sState_unk->m14 = cut;
    sState_unk->m18 = 0;
    sState_unk->m1C = 0;
    sState_unk->m20 = 0;
    sState_unk->m24 = 0;
    sState_unk->m28 = 0;
    CutRecord_unk rec;
    memcpy(&rec, data, sizeof(rec));
    cut->mIndex = rec.mIndex;
    cut->mSkip = rec.mFlags & 0xF;
    cut->mCinematic = rec.mFlags & 0x10;
    cut->mNextCut = rec.mNextCut;
    cut->m2C = rec.m10;
    return sizeof(rec);
}

// inferred: fades of the cut
static int ParseCUFA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Cut* cut = sState_unk->m14;
    short rec[4];
    memcpy(rec, data, sizeof(rec));
    cut->m18 = rec[0];
    cut->m1C = rec[1];
    cut->m20 = rec[2];
    cut->m24 = rec[3];
    return sizeof(rec);
}

static int ParseCUNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m14->SetName_unk(data);
    return size;
}

// inferred: record of a label (8 bytes)
struct LabelRecord_unk {
    int mIndex;
    int mFrame;
};

static int ParseLABE_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    LabelRecord_unk rec;
    memcpy(&rec, data, sizeof(rec));
    Label* label = sState_unk->m14->fn_8053B030(rec.mIndex);
    sState_unk->m18 = label;
    sState_unk->m20 = 0;
    sState_unk->m24 = 0;
    sState_unk->m28 = 0;
    label->mFrame = rec.mFrame;
    return sizeof(rec);
}

static int ParseLANA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m18->SetName_unk(data);
    return size;
}

// inferred: record of a section (8 bytes)
struct SectionRecord_unk {
    int m00;
    short mIndex;
    short m06;
};

static int ParseSECT_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    SectionRecord_unk rec;
    memcpy(&rec, data, sizeof(rec));
    Section* section = sState_unk->m14->fn_8053B828(rec.mIndex);
    sState_unk->m1C = section;
    sState_unk->m20 = 0;
    sState_unk->m24 = 0;
    sState_unk->m28 = 0;
    section->m10 = rec.m00;
    section->m0C = rec.mIndex;
    return sizeof(rec);
}

static int ParseSCNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m1C->SetName_unk(data);
    return size;
}

// inferred: record of a channel (4 bytes)
struct ChannelRecord_unk {
    short mCommand;
    short m02;
};

static int ParseCHAN_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Channel* channel = sState_unk->m1C->fn_8053C788();
    sState_unk->m20 = channel;
    sState_unk->m24 = 0;
    sState_unk->m28 = 0;
    ChannelRecord_unk rec;
    memcpy(&rec, data, sizeof(rec));
    channel->m0C = rec.mCommand;
    return sizeof(rec);
}

static int ParseCHNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m20->SetName_unk(data);
    return size;
}

// inferred: records of an action ("ACTI" 8 bytes, "ACT2" 12 bytes)
struct ActionRecord_unk {
    short m00;
    short m02;
    short m04;
    short m06;
};
struct ActionRecord2_unk {
    short m00;
    short m02;
    short m04;
    short m06;
    int m08;
};

static int ParseACTI_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Action* action = sState_unk->m20->fn_8053A510(-1);
    sState_unk->m24 = action;
    sState_unk->m28 = 0;
    if (tag == 'ACT2') {
        ActionRecord2_unk rec;
        memcpy(&rec, data, sizeof(rec));
        action->m10 = rec.m00;
        action->m18 = rec.m02;
        action->m1C = rec.m04;
        action->m14 = rec.m06;
        return sizeof(rec);
    } else {
        ActionRecord_unk rec;
        memcpy(&rec, data, sizeof(rec));
        action->m10 = rec.m00;
        action->m18 = rec.m02;
        action->m1C = rec.m04;
        action->m14 = rec.m06;
        return sizeof(rec);
    }
}

static int ParseATNA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    sState_unk->m24->SetName_unk(data);
    return size;
}

// inferred: record of a parameter (8 bytes)
struct ParamRecord_unk {
    unsigned int mHash;
    int mValue;
};

static int ParsePARA_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    int value;
    Action* action = sState_unk->m24;
    ParamRecord_unk rec;
    memcpy(&rec, data, sizeof(rec));
    ActionParam_unk* param = action->fn_80539330(rec.mHash);
    if (param != 0) {
        param->m04.i = rec.mValue;
        const CommandDef_unk* def = fn_800081C0(action->mChannel->m0C);
        int variant = def->mVariantParam;
        if (variant >= 0) {
            value = action->GetParamAt_unk(variant)->m04.i;
            int i = action->fn_805391E4(rec.mHash);
            param->m00 = def->mVariants[value][i].mType;
        }
    }
    sState_unk->m28 = param;
    return sizeof(rec);
}

// inferred: string parameter (a motion name)
static int ParsePAST_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Action* action = sState_unk->m24;
    unsigned int hash;
    memcpy(&hash, data, sizeof(hash));
    int motion = fn_802180A8(data + 4);
    ActionParam_unk* param = action->fn_80539330(hash);
    if (param != 0) {
        param->m04.i = motion;
    }
    sState_unk->m28 = param;
    return size;
}

// inferred: key of the curve of the action
static int ParseFORC_unk(unsigned int tag, int size, const char* data, ParserState_unk* state) {
    Action* action = sState_unk->m24;
    ActionKey_unk rec;
    memcpy(&rec, data, sizeof(rec));
    ActionKey_unk key;
    key.mTime = rec.mTime;
    key.mValue = rec.mValue;
    action->fn_805393F8(key);
    return sizeof(rec);
}

// inferred: header of a chunk
struct ChunkHeader_unk {
    unsigned int mTag;
    int mSize;
};

typedef int (*ParseFunc_unk)(unsigned int tag, int size, const char* data, ParserState_unk* state);
struct ParseEntry_unk {
    unsigned int mTag;
    ParseFunc_unk mFunc;
};

static const ParseEntry_unk sParsers_unk[] = {
    {'CAT ', ParseCAT_unk},
    {'SEQ2', ParseSEQ2_unk},
    {'SENA', ParseSENA_unk},
    {'SEMS', ParseSEMS_unk},
    {'ARGR', ParseGroup_unk},
    {'ARNU', ParseARNU_unk},
    {'MOGR', ParseGroup_unk},
    {'MOID', ParseMOID_unk},
    {'MONA', ParseMONA_unk},
    {'ANGR', ParseGroup_unk},
    {'ANID', ParseANID_unk},
    {'ANNA', ParseANNA_unk},
    {'EFGR', ParseGroup_unk},
    {'EFID', ParseEFID_unk},
    {'EFNA', ParseEFNA_unk},
    {'MDGR', ParseGroup_unk},
    {'MDID', ParseMDID_unk},
    {'MDNA', ParseMDNA_unk},
    {'MDMA', ParseMDMA_unk},
    {'DUGR', ParseGroup_unk},
    {'DUID', ParseDUID_unk},
    {'DUNA', ParseDUNA_unk},
    {'CAGR', ParseGroup_unk},
    {'CAID', ParseCAID_unk},
    {'CANA', ParseCANA_unk},
    {'POGR', ParseGroup_unk},
    {'POID', ParsePOID_unk},
    {'PONA', ParsePONA_unk},
    {'MVGR', ParseGroup_unk},
    {'MVID', ParseMVID_unk},
    {'MVNA', ParseMVNA_unk},
    {'PAGR', ParseGroup_unk},
    {'PATH', ParsePATH_unk},
    {'PANA', ParsePANA_unk},
    {'POIN', ParsePOIN_unk},
    {'ACGR', ParseGroup_unk},
    {'ACID', ParseACID_unk},
    {'ACNA', ParseACNA_unk},
    {'STGR', ParseGroup_unk},
    {'STID', ParseSTID_unk},
    {'STNA', ParseSTNA_unk},
    {'CUT ', ParseCUT_unk},
    {'CUFA', ParseCUFA_unk},
    {'CUNA', ParseCUNA_unk},
    {'LABE', ParseLABE_unk},
    {'LANA', ParseLANA_unk},
    {'SECT', ParseSECT_unk},
    {'SCNA', ParseSCNA_unk},
    {'CHAN', ParseCHAN_unk},
    {'CHNA', ParseCHNA_unk},
    {'ACTI', ParseACTI_unk},
    {'ACT2', ParseACTI_unk},
    {'ATNA', ParseATNA_unk},
    {'PARA', ParsePARA_unk},
    {'PAST', ParsePAST_unk},
    {'FORC', ParseFORC_unk},
    {'CVGR', ParseGroup_unk},
    {'CURV', ParseCURV_unk},
    {'CVPT', ParseCVPT_unk},
};

// inferred: reads the chunks of the event data of `seq` (inlined in Sequence::fn_8057D5F4 and fn_8057D8C0)
static inline int ReadChunks_unk(Sequence* seq) {
    if (seq->fn_80575F0C() == 0) {
        return 0;
    }
    if (sState_unk != 0) {
        return 0;
    }
    sState_unk = new ((MemoryTag)4, "", "", 0) ParserState_unk;
    sState_unk->m10 = seq;
    ParserState_unk* state = sState_unk;
    int size = seq->fn_80575F0C();
    const char* data = (const char*)seq->fn_80575F6C();
    for (const char* p = data; p - data < size;) {
        ChunkHeader_unk header = {0, 0};
        memcpy(&header, p, sizeof(header));
        fn_800696B4();
        p += sizeof(header);
        int read = -1;
        for (int i = 0; i < (int)(sizeof(sParsers_unk) / sizeof(sParsers_unk[0])); i++) {
            if (sParsers_unk[i].mFunc != 0 && header.mTag == sParsers_unk[i].mTag) {
                read = sParsers_unk[i].mFunc(header.mTag, header.mSize, p, state);
                break;
            }
        }
        if (read == -1) {
            read = header.mSize;
        }
        p += read;
    }
    delete sState_unk;
    sState_unk = 0;
    FixActions_unk(seq);
    return 1;
}

// NONMATCHING (with fn_8057D8C0): the destructor of the reader keeps a second null test in the original, the
// registers of the loop differ and the "" literal follows "vector length error" in the pool (the push_back
// of the "..ID" readers, still stubs)
// inferred: loads the event file `path` (owned data when `owned` is set)
void Sequence::fn_8057D5F4(const char* path, void* owned) {
    m98_0 = (unsigned int)owned;
    if (m98_0 == 1) {
        m1D0 = fn_8006BA8C(path, &m1CC, 0);
        if (ReadChunks_unk(this)) {
            m194 = 4;
        } else {
            m194 = 0;
        }
        fn_80575FCC();
        fn_80541F18();
    } else {
        m1C4.vf0C(path);
        m194 = 3;
    }
}

// inferred: reads the chunks of the event data
int Sequence::fn_8057D8C0() {
    return ReadChunks_unk(this);
}

} // namespace Event
