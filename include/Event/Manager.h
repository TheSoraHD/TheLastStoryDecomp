#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "atn/Task.h"
#include "Event/Path.h"
#include "atn/Bgm.h"
#include "std/vector.h"
#include "std/list.h"
#include "atn/Sphere.h"
#include "atn/ColorMatrixParam_unk.h"

class GameScene;
class CharaTask;
class NewStaffRoll;
struct MapLocators_unk;
struct ColorMatrixParam_unk;

namespace Event {

class Action;
class Sequence;
struct MovieState_unk;

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
    float m104;
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
    // inferred: called by Event/ActionEffectPlay.cpp
    void fn_80486BEC(int on, float value);
    void fn_804846FC(const atn::Vector3& dir, int frames);
    void fn_80484A78(int frames);
    void fn_80486DAC(int on);
    void fn_80487064(int on);
    void fn_8048EE78(int a, float r, float g, float b, float c, float d, int e, float f);
    void fn_80488050(Sequence* manager, Action* arg);
    void fn_80488A74(const char* name, int fade, float volume);
    void fn_80488D80(int fade);
    MapLocators_unk* fn_80480438();
    void fn_8048E9E0();
    void fn_80485028(int index, const atn::Vector3& pos, float radius, int color);
    void fn_80485964(const atn::Vector3& pos, const atn::Vector3& dir, float a, float b, int color);
    void fn_80484D64(atn::Vector2_unk scale, atn::Vector2_unk pos);
    void fn_80484BF4();
    void fn_80484D40(atn::Vector2_unk scale, atn::Vector2_unk pos, float rate);
    void fn_80484E88();
    ColorMatrixParam_unk* fn_80483F70();
    void fn_80484088(ColorMatrixParam_unk* param, float rate);
    void fn_804881B8(Sequence* manager, const char* name, int fade, int stopBgm, int a, int b, int c, float volume, int d, int flags);
    void fn_8048E9A4(MovieState_unk* movie, int frames);
    void fn_8048EC7C();
    void fn_80489A2C();
    int fn_8048B048(int id);                  // inferred: time curve by id
    int fn_8048B3B8(int id);                  // inferred: move curve by id
    float fn_8048BCA4(int curve, float t);    // inferred: value of a time curve
    float fn_8048BD04(int curve, float t);    // inferred: value of a move curve
    void fn_8048C884(int curve);              // inferred: releases a move curve
    // inferred: weak copies 0x80112F54 and 0x801130F4 (Event/Path.cpp)
    Sequence* fn_80112F54() { return m70; }
    int fn_801130F4() { return mC4; }
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
    void fn_8047F984(float time);
    void fn_80488D6C(); // inferred: sets the frame time (also in the game settings)
    void fn_8047F8C0(int arg);
    int fn_80481654();   // inferred: the task m1A7C is not frozen (no event is running)
    int fn_80481668();   // inferred: nonzero while the song stops the characters
    void fn_80481800(int on); // inferred: shows the check sphere
    // inferred: next state of m1AA0 (inlined in fn_8047F6B0 and fn_8047F6E4); name invented
    void Step1AA0_unk() {
        if (m1AA0 == 0) {
            m1AA0 = 1;
        } else if (m1AA0 == 2) {
            m1AA0 = 3;
        }
    }

    unsigned char m48[0x4C - 0x48];
    atn::Sphere m4C; // inferred: target of the check of type 1 (ActionCheck)
    unsigned char m5C[0x64 - 0x5C];
    std::vector<Sequence*> m64; // inferred: sequences of the manager
    Sequence* m70; // inferred: current sequence
    unsigned char m74[0xB4 - 0x74];
    float mB4; // inferred: time of a frame
    std::list<int> mB8; // inferred: element type unknown (cleared when the cut changes)
    int mC4;   // inferred: the cameras are updated at once
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
    unsigned char m1AB0[0x1D70 - 0x1AB0];
    ColorMatrixParam_unk m1D70; // inferred: color matrix of the event
    unsigned char m1DC0[0x1E94 - 0x1DC0];
    atn::Sphere m1E94; // inferred: sphere looked at by ActionCheck (type 1)
    int m1EA4; // inferred: the check sphere is shown (fn_80481800)
    int m1EA8;
    int m1EAC;
    int m1EB0;
    unsigned char m1EB4[0x1F0C - 0x1EB4];
    int m1F0C; // inferred: cloud map shown by ActionCloudMap
    std::vector<int> m1F10; // inferred: cleared by fn_80484FBC with the lights
    int m1F1C;
    atn::Bgm m1F20; // inferred: music of the event
    int m1F34;      // inferred: the music is used
    char m1F38[0x24]; // inferred: a name (tested with strlen)
    int m1F5C;
    unsigned char m1F60[0x1F6C - 0x1F60];
    float m1F6C; // inferred: movie fade time
    unsigned char m1F70[0x2380 - 0x1F70];
    int m2380;
    unsigned char m2384[0x238C - 0x2384];
    int m238C; // inferred: nonzero while messages are blocked
    int m2390; // inferred: nonzero: the dialog texts are flushed at once
    unsigned char m2394[0x23AC - 0x2394];
    atn::Task* m23AC; // inferred: frozen by fn_8048F184 (0)
    atn::Task* m23B0; // inferred: frozen by fn_8048F184 (1)
    int m23B4; // inferred: set when the check of type 1 succeeds
    unsigned char m23B8[0x2400 - 0x23B8];
    NewStaffRoll* m2400; // inferred: staff roll of the ending
    unsigned char mMoviePlaying : 1; // 0x2404
    unsigned char m2404_1 : 7;
    unsigned char m2405[0x2448 - 0x2405];
    int m2448; // inferred: nonzero to pass the checks without input
};

} // namespace Event

extern "C" Event::Manager* lbl_8087F540; // inferred: the event manager (.sbss of Event/Manager.cpp)

#endif
