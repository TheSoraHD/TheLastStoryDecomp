#ifndef GAME_NEWCAMPTASK_H
#define GAME_NEWCAMPTASK_H

#include "atn/Task.h"
#include "atn/FixedArray.h"
#include "math/Vec.h"
#include "atn/Camera_unk.h"
#include "Event/Sequence.h"

namespace atn {
class FadeColor;
struct ModelBase;
} // namespace atn

// inferred: (id, value) pair of the camp (0x8 bytes, fn_80530384); names invented
struct NewCampPair_unk {
    int mId;
    int m04;
};

// inferred from __vt__11NewCampTask (0x80793808): camp menu task, parent of the NewCampParts* (0x1380
// bytes, fn_8052A888). Members keep their offsets (names unknown).
class NewCampTask : public atn::Task {
public:
    void fn_8052C9C4(); // inferred: opens/closes the map select of the field (toggled by m133C)
    void fn_8052CA6C();
    void fn_8052DEEC(int state); // inferred: changes the parts of the camp (m464)
    void fn_80530174();
    void UpdateCamera_unk();          // inferred: resets the camera and goes to the top parts
    void fn_8053032C(atn::Vector3 pos, atn::Vector3 rot); // inferred: starts a camera path
    NewCampPair_unk& fn_80530384(int id);
    void fn_80530CD0(int duration); // inferred: fades out
    void fn_80530D30(int duration); // inferred: fades in
    void fn_80530D88(int set, atn::ModelBase* model);

    unsigned char m48[0x5C - 0x48];
    int m5C;
    unsigned char m60[0x64 - 0x60];
    atn::FadeColor* m64; // inferred: fade of the camp (FadeColor::Create in the constructor)
    int m68;             // inferred: faded out
    atn::Camera mCamera; // 0x6C
    unsigned char m260[0x458 - 0x260];
    float m458;
    float m45C;
    unsigned char m460[0x464 - 0x460];
    int m464; // inferred: state (parts shown)
    unsigned char m468[0x11F0 - 0x468];
    atn::Task* m11F0; // inferred: current parts
    unsigned char m11F4[0x11F8 - 0x11F4];
    int m11F8;
    atn::Vector3 m11FC; // inferred: camera position (40, 0, 210)
    atn::Vector3 m1208; // inferred: camera target (40, 0, -20)
    float m1214;        // inferred: field of view (55 degrees)
    unsigned char m1218[0x121C - 0x1218];
    int m121C;
    int m1220;
    atn::FixedArray_unk<NewCampPair_unk, 8> m1224;
    unsigned char m1268[0x12AC - 0x1268];
    int m12AC;
    Event::EffectLists_unk m12B0; // inferred: camera path
    float m12D0;
    atn::Vector3 m12D4;
    atn::Vector3 m12E0;
    atn::Vector3 m12EC;
    atn::Vector3 m12F8;
    unsigned char m1304[0x133C - 0x1304];
    int m133C; // inferred: the map select is open
    unsigned char m1340[0x1368 - 0x1340];
    float m1368;
    int m136C;          // inferred: the camera offset m1370 is used (4:3 screen)
    atn::Vector3 m1370; // inferred: "CameraOfs(Aspect4:3)"
    unsigned char m137C[0x1380 - 0x137C];
};

#endif
