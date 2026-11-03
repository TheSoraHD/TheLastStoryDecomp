#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "atn/Task.h"
#include "Event/Path.h"
#include "atn/Bgm.h"
#include "std/vector.h"

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

// inferred: task at 0x1AA8 of the event manager (names invented)
struct ManagerFade_unk : public atn::Task {
    int m48; // inferred: active
    int m4C;
    int m50;
    int m54;
    int m58;
    unsigned char m5C[0x6C - 0x5C];
    int m6C;
    int m70;
    float m74;
};

// inferred: task at 0x1A7C of the event manager (float at 0x100)
struct ManagerTask1A7C_unk : public atn::Task {
    unsigned char m48[0x100 - 0x48];
    float m100;
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

    // inferred: sequence with the id `id` (current one first, then the list m64; inlined); name invented
    Sequence* FindSequence_unk(int id);
    int fn_8047F2EC(int id); // inferred: the sequence `id` is playing (state 10)
    int fn_8047F420(int id); // inferred: the sequence `id` is missing or of kind 1
    int fn_8047F498(int id); // inferred: the sequence `id` is missing or of kind 3
    int fn_8047F510(int id); // inferred: the sequence `id` is missing or has m98_8
    int fn_8047F6B0(int arg);
    void fn_8047F6E4();
    void fn_8047F850(int a, int b, int c);
    void fn_8047F88C(int arg);
    void fn_8047F8A4(float value);
    void fn_8047F8C0(int arg);
    // inferred: next state of m1AA0 (inlined in fn_8047F6B0 and fn_8047F6E4); name invented
    void Step1AA0_unk() {
        if (m1AA0 == 0) {
            m1AA0 = 1;
        } else if (m1AA0 == 2) {
            m1AA0 = 3;
        }
    }

    unsigned char m48[0x64 - 0x48];
    std::vector<Sequence*> m64; // inferred: sequences of the manager
    Sequence* m70; // inferred: current sequence
    unsigned char m74[0xC8 - 0x74];
    EventCamera_unk mCameras[4]; // 0xC8
    int m1A38;
    unsigned char m1A3C[0x1A6C - 0x1A3C];
    GameScene* m1A6C; // inferred: scene of the event when there is no field
    unsigned char m1A70[0x1A7C - 0x1A70];
    ManagerTask1A7C_unk* m1A7C; // inferred: frozen by fn_8048169C
    atn::Task* m1A80;          // inferred: frozen by fn_804817D4
    int m1A84;
    int m1A88;
    int m1A8C;
    int m1A90;
    unsigned char m1A94[0x1A9C - 0x1A94];
    atn::Task* m1A9C;
    int m1AA0; // inferred: state (0, 1, 2, 3)
    int m1AA4;
    ManagerFade_unk* m1AA8;
    int m1AAC;
    unsigned char m1AB0[0x1F10 - 0x1AB0];
    std::vector<int> m1F10; // inferred: cleared by fn_80484FBC with the lights
    int m1F1C;
    atn::Bgm m1F20; // inferred: music of the event
    int m1F34;      // inferred: the music is used
    unsigned char m1F38[0x2380 - 0x1F38];
    int m2380;
    unsigned char m2384[0x23AC - 0x2384];
    atn::Task* m23AC; // inferred: frozen by fn_8048F184 (0)
    atn::Task* m23B0; // inferred: frozen by fn_8048F184 (1)
    unsigned char m23B4[0x2400 - 0x23B4];
    ManagerSubTask_unk* m2400;
};

} // namespace Event

extern "C" Event::Manager* lbl_8087F540; // inferred: the event manager (.sbss of Event/Manager.cpp)

#endif
