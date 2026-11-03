#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "atn/Task.h"
#include "Event/Path.h"
#include "atn/Bgm.h"

class GameScene;
class CharaTask;

namespace Event {

class Action;
class Sequence;

// inferred: task of the event manager at 0x2400 (stopped during some events; float at 0x48)
struct ManagerSubTask_unk : public atn::Task {
    float m48;
    float m4C;
    float m50;
    float m54;
};

// inferred: event manager (Event/Manager.cpp, vtable __vt__Q25Event7Manager; instance lbl_8087F540).
// Partial: only what Event/Sequence.cpp uses; member and method names are invented.
class Manager : public atn::Task {
public:
    // inferred: creates the character `id` of kind `kind` for the event (Event::Actor)
    CharaTask* fn_8047FF70(int kind, int id, float arg, const atn::Vector3& pos);
    void fn_8047E528();
    void fn_8047F580();
    void fn_8047F8B0();
    void fn_8047F994(int a, int b);
    void fn_8048169C(int arg);
    void fn_804817D4();
    void fn_80482ED8();
    void fn_80483714(int a, int b);
    void fn_80483E94(int arg);
    void fn_80484FBC();
    void fn_80489A50(Action* action, int arg);
    void fn_8048E904();
    void fn_8048F184(int arg);
    void fn_8048FE88();
    void fn_8048FEF0();
    int fn_8048FF34(); // inferred: nonzero while loading
    // inferred: the music of the event is being prepared (inlined in Event::Sequence; name invented)
    bool IsBgmBusy_unk() { return m1F34 != 0 && m1F20.fn_8004B0E4() != 0; }

    unsigned char m48[0x70 - 0x48];
    Sequence* m70; // inferred: current sequence
    unsigned char m74[0xC8 - 0x74];
    EventCamera_unk mCameras[4]; // 0xC8
    int m1A38;
    unsigned char m1A3C[0x1A6C - 0x1A3C];
    GameScene* m1A6C; // inferred: scene of the event when there is no field
    unsigned char m1A70[0x1AA0 - 0x1A70];
    int m1AA0;
    unsigned char m1AA4[0x1F20 - 0x1AA4];
    atn::Bgm m1F20; // inferred: music of the event
    int m1F34;      // inferred: the music is used
    unsigned char m1F38[0x2380 - 0x1F38];
    int m2380;
    unsigned char m2384[0x2400 - 0x2384];
    ManagerSubTask_unk* m2400;
};

} // namespace Event

extern "C" Event::Manager* lbl_8087F540; // inferred: the event manager (.sbss of Event/Manager.cpp)

#endif
