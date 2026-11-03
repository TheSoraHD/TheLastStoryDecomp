#ifndef EVENT_SEQUENCE_H
#define EVENT_SEQUENCE_H

#include "atn/Task.h"
#include "atn/LH_Binary.h"
#include "atn/Array_unk.h"
#include "atn/SoundRef.h"
#include "atn/static_string.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/vector.h"
#include <boost/shared_ptr.hpp>
#include "Event/Node.h"
#include "Event/CommonData.h"
#include "Event/MotionData.h"
#include "Event/Path.h"
#include "Event/StringData.h"
#include "Event/DummyData.h"
#include "Event/Actor.h"
#include "game/IntVector_unk.h"
#include "game/ResRef_unk.h"
#include "game/GameMessageList.h"

class GameMessageWindow;
namespace atn {
class D2A_Task;
}

// inferred: assertions compiled out (they only count for the inlining budget)
#define SEQ_ASSERT_UNK(x) ((void)0)
#define SEQ_ASSERT10_UNK                                                                                          \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0);                                                                                              \
    SEQ_ASSERT_UNK(0)
#define SEQ_ASSERT100_UNK                                                                                         \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK;                                                                                               \
    SEQ_ASSERT10_UNK


namespace Event {

class Cut;
class Label;
class Dialog;
class Sequence;

// inferred: list of event data of one kind with the sequence that owns them (0x10 bytes: the vector
// and the owner; the destructor tests the address four times, one more level than std::vector).
// Name invented.
template <class T>
class DataList_unk : public std::vector<T> {
public:
    DataList_unk() : mOwner(0) {}
    // inferred: the assertions put the destructor of Event::Sequence over the inlining budget (it calls
    // the member destructors out of line)
    ~DataList_unk() {
        SEQ_ASSERT100_UNK;
        SEQ_ASSERT100_UNK;
        SEQ_ASSERT100_UNK;
    }
    void SetOwner_unk(Sequence* owner);
    // inferred: entry with the id `id` (template member emitted at the end of Event/ActionActorPosition.cpp,
    // 0x8055A400; defined there)
    T* Find_unk(int id);
    // inferred: all the data of the list is loaded (inlined in Event::Sequence::fn_8054011C)
    bool IsLoaded_unk() {
        for (int i = 0; i < (int)this->size(); i++) {
            if ((*this)[i].vf10() == 0) {
                return false;
            }
        }
        return true;
    }
    // inferred: appends a new entry made from the owner; only the actors have one, an explicit
    // specialization defined at the end of Event/Sequence.cpp (0x80543108, before the template instances)
    void Add_unk(const char* name, int arg);

    Sequence* mOwner;
};

// inferred: defined outside the class: the instances come out in reverse order of first use, as in
// the original (0x805436B0-0x805436F8)
template <class T>
void DataList_unk<T>::SetOwner_unk(Sequence* owner) {
    mOwner = owner;
}


// inferred: data of 0x48 bytes listed at 0x104 of the sequence (class unknown)
class Data48_unk : public CommonData {
public:
    unsigned char m18[0x48 - 0x18];
};

// inferred: message slot of the sequence (0x24 bytes, four at 0x1F0; names invented)
struct ScreenEffect_unk {
    void Reset_unk() {
        m00 = 0;
        m10 = 0.0f;
        m08 = 0.0f;
        m04 = 0.0f;
        m0C = 0.5f;
        m14 = 0;
        m18 = 0;
        m1C = -1;
        m20 = 0;
    }

    GameMessage* m00; // inferred: the message shown (its text is drawn by fn_80541588)
    float m04;           // inferred: start frame
    float m08;           // inferred: length in frames
    float m0C;
    float m10;
    int m14;
    int m18;
    int m1C;
    unsigned int m20; // inferred: flags (bit 6 and 7: no fade in / out)
};

// inferred: two lists of an entry of the list at 0x184 (names invented)
struct EffectLists_unk {
    std::vector<int> m00;
    int m0C;
    IntVector_unk m10; // inferred: one destructor level more than m00 (four address tests)
    int m1C;
};

// inferred: entry of the list at 0x184 of the sequence (0x24 bytes)
struct EffectEntry24_unk {
    int m00;
    EffectLists_unk m04;
};

// inferred: one byte flags at 0x58 of the sequence (names invented)
struct SequenceState_unk {
    SequenceState_unk() : m00(0), m02(0), m03(0) {}
    short m00;
    unsigned char m02;
    unsigned char m03;
};

// inferred: entry of the "ContinueEffectList" array (8 bytes)
struct ContinueEffect_unk {
    int m00;
    void* m04; // inferred: owner of the effect (an action)
};

// inferred: event sequence (Event/Sequence.cpp, 0x2C8 bytes): a task that owns the cuts and the data of
// an event; the callers of other files see it as EventManager_unk (Event/Behavior.h) and DataOwner_unk
// (Event/MotionData.h). Member names are invented.
class Sequence : public atn::Task, public Node {
public:
    Sequence(atn::Task* parent);
    virtual ~Sequence();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static Sequence* fn_8053E86C(atn::Task* parent); // inferred: creates a sequence

    void fn_8053E8C8(int state); // inferred: sets the state
    void fn_8053E8D0();
    void fn_8053ED4C();
    void fn_8053EF70();
    void fn_8053F1B4();
    void fn_8053F6EC();
    void fn_8053F818();
    int fn_8054011C();
    void fn_80540664(int arg);
    void fn_805406A0();
    void fn_805406BC(int keep = 0);
    Cut* fn_80540A08();
    int fn_80541198(Cut* cut);
    Cut* fn_805411DC();
    Cut* fn_80541210();
    void fn_80541244(int index);
    Label* fn_805412A8(int index);
    void fn_8054136C(int label);
    void fn_80541520(int index, int frame);
    void fn_80541534(int index, int frame);
    void fn_80541548(int slot, GameMessage* msg, unsigned int kind, int style, int size, unsigned int flags,
                     float start, float length, float speed0, float speed1);
    void fn_80541588(const wchar_t* text, float x, float y, int show, int align, unsigned int color,
                     unsigned int flags, float alpha);
    atn::Vector3 fn_80541BD8(const atn::Vector3& pos);
    atn::Vector3 fn_80541C94(const atn::Vector3& v);
    float fn_80541CF0(float yaw);
    int fn_80541CFC(int kind, int num);
    void fn_80541F18();
    void fn_8054204C();
    void fn_80542684();
    void fn_805428A8();
    EffectLists_unk* fn_80543060(int id);
    void fn_805430A0(unsigned int index, int key, void* owner);
    void* fn_805430BC(unsigned int index, int key);

    // inferred: inline helpers (names invented)
    unsigned int GetFlag7_unk() const { return m98_7; }
    unsigned int GetFlag15_unk() const { return (m98 >> 16) & 1; }
    bool IsCutIndex_unk(int i) const { return i >= 0 && i < (int)mB8.size(); }
    inline Cut* FindCutByIndex_unk(int index); // inferred: defined in Event/Sequence.cpp
    // inferred: lookup of a path by id (inlined in ActionActorPosition::vf0C; defined there)
    inline Path* FindPath_unk(int id);
    inline DataList_unk<Actor_unk>* GetActors_unk(); // inferred: weak copy 0x80551D20 (Event/ActionActorPosition.cpp)
    Actor_unk* FindActor_unk(int id) {
        for (int i = 0; i < (int)m164.mSize; i++) {
            Actor_unk* actor = &m164.mData[(unsigned int)i];
            if (id == actor->m14) {
                return actor;
            }
        }
        return 0;
    }

    // inferred: defined in other files (Event/EventLoader_unk.cpp, Event/EventParser_unk.cpp...)
    int fn_80575F0C();
    void* fn_80575F6C();
    void fn_80576030(const char* path, void* arg);
    void fn_8057D5F4(const char* path, void* arg);
    void fn_805501DC(int arg);
    void fn_80550254();
    void fn_80575FCC();
    void fn_80576258();
    void fn_8057D8C0();

    // inferred: flag setters (called out of line by the constructor)
    inline void SetFlag3_unk(int on); // inferred: defined in Event/Sequence.cpp
    void SetFlag4_unk(int on) { m98_4 = on != 0; }
    void SetFlag7_unk(int on) { m98_7 = on != 0; }
    void SetFlag8_unk(int on) { m98_8 = on != 0; }
    void SetFlag9_unk(int on) { m98_9 = on != 0; }

    int m50; // inferred: kind of event (1 and 3: in game)
    int m54;
    SequenceState_unk m58;
    IntVector_unk m5C;
    std::string m68;
    char m74[0x20];
    int m94;
    union {
        struct {
            unsigned int m98_0 : 1;
            unsigned int m98_1 : 1;
            unsigned int m98_2 : 1;
            unsigned int m98_3 : 1;
            unsigned int m98_4 : 1;
            unsigned int mNoKill_unk : 1; // inferred: keeps the d2anime tasks alive when set
            unsigned int m98_6 : 1;
            unsigned int m98_7 : 1;
            unsigned int m98_8 : 1;
            unsigned int m98_9 : 1;
            unsigned int m98_10 : 1;
            unsigned int m98_11 : 1;
            unsigned int m98_12 : 1;
            unsigned int mPadCamera_unk : 1; // inferred: the event camera is moved with the pad
            unsigned int m98_14 : 1;
            unsigned int m98_15 : 1;
            unsigned int m98_16 : 1;
            unsigned int m98_17 : 15;
        };
        unsigned int m98;
    };
    atn::Vector3 m9C;
    atn::Vector3 mA8;
    float mB4; // inferred: yaw of the event (fn_80541C94)
    std::vector<boost::shared_ptr<Cut> > mB8; // 0xB8: inferred: cuts
    DataList_unk<MotionData> mC4;
    DataList_unk<AnimationData> mD4;
    DataList_unk<EffectData> mE4;
    DataList_unk<ModelData> mF4;
    DataList_unk<Data48_unk> m104;
    DataList_unk<StringData> m114; // inferred: named entries (streams of the multi audio actions)
    DataList_unk<CameraData> m124;
    DataList_unk<PoseData> m134;
    DataList_unk<MovieData> m144;
    DataList_unk<Path> m154;
    DataList_unk<Actor_unk> m164;
    atn::SoundRef m174[4];
    std::vector<EffectEntry24_unk> m184;
    int m190; // inferred: id of the event
    int m194; // inferred: state
    float m198; // inferred: current frame
    float m19C;
    int m1A0;
    int m1A4;
    int mLabelCount; // 0x1A8: inferred: highest label index given to Event::Cut::fn_8053B030
    int m1AC;
    int m1B0;
    int m1B4;
    int m1B8;
    int m1BC;
    int m1C0;
    atn::LH_Binary m1C4;
    int m1CC;  // inferred: size of the event data when it is owned (m98_0)
    void* m1D0; // inferred: event data when it is owned
    GameMessageList m1D4;
    GameMessageWindow* m1EC;
    ScreenEffect_unk m1F0[4];
    Dialog* m280;
    int m284;
    int m288;
    int m28C;
    int m290;
    atn::SimpleArray<ContinueEffect_unk> m294;
    ResRef_unk m29C;
    atn::static_string<32, char, std::char_traits<char> > m2A0;
    atn::D2A_Task* m2C0; // inferred: d2anime of the end of the event
    int m2C4;
};

} // namespace Event

#endif
