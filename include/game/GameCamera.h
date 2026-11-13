#ifndef GAME_GAMECAMERA_H
#define GAME_GAMECAMERA_H

#include "math/Vec.h"
#include "atn/Camera_unk.h"
#include "atn/RenderSetting_unk.h"
#include "game/ResHandle.h"

namespace gs {
struct CameraBox;
}
class CharaTask;

// inferred: camera mode saved and restored around an event (GameCamera::fn_80392C94; names invented)
struct GameCameraState_unk {
    int mMode;
    int mPrevMode;
    int m08;
    atn::Vector3 m0C;
    atn::Vector3 m18;
    float m24;
};

// inferred: blend timer of the camera parameters (0xC bytes; weak copies 0x803829C0, 0x803829D4); names
// invented
struct GameCameraTimer_unk {
    GameCameraTimer_unk(); // inferred: clears mActive (inline, defined in game/GameCamera_unk.cpp)
    bool fn_803829C0() const { return mActive != 0; } // inferred: running
    void fn_803829D4() {                                // inferred: steps towards 0
        mTime = mTime - mStep;
        mTime = 0.0f > mTime ? 0.0f : mTime;
    }
    int mActive;
    float mTime; // inferred: set from the replay rewind (GameTask::fn_80373AE8)
    float mStep;
};

// inferred: parameters of a camera mode (0x3C bytes, at 0x4C0 and 0x684; copy 0x8037E964, blend 0x8037F830);
// names invented
struct GameCameraParam_unk {
    GameCameraParam_unk(); // inferred: clears m34 (inline, defined in game/GameCamera_unk.cpp)
    // inferred: weak copy 0x8037E964
    GameCameraParam_unk(const GameCameraParam_unk& o)
        : m00(o.m00), m04(o.m04), m08(o.m08), m0C(o.m0C), m10(o.m10), m14(o.m14), m18(o.m18), m1C(o.m1C),
          m20(o.m20), m24(o.m24), m28(o.m28), m2C(o.m2C), m30(o.m30), m34(o.m34), m38(o.m38) {}
    // inferred: weak copy 0x8037F830: blend from a to b
    void fn_8037F830(const GameCameraParam_unk& a, const GameCameraParam_unk& b, float t) {
        m00 = t * (b.m00 - a.m00) + a.m00;
        m04 = t * (b.m04 - a.m04) + a.m04;
        m08 = t * (b.m08 - a.m08) + a.m08;
        m0C = t * (b.m0C - a.m0C) + a.m0C;
        m10 = t * (b.m10 - a.m10) + a.m10;
        m14 = t * (b.m14 - a.m14) + a.m14;
        m18 = t * (b.m18 - a.m18) + a.m18;
        m1C = t * (b.m1C - a.m1C) + a.m1C;
        m20 = t * (b.m20 - a.m20) + a.m20;
        m24 = t * (b.m24 - a.m24) + a.m24;
        m28 = t * (b.m28 - a.m28) + a.m28;
        m2C = t * (b.m2C - a.m2C) + a.m2C;
        m30 = t * (b.m30 - a.m30) + a.m30;
        m34 = t * (b.m34 - a.m34) + a.m34;
        m38 = a.m38;
    }
    float m00; // inferred: distance
    float m04; // inferred: fovy
    float m08; // inferred: pitch
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C; // inferred: weight of the distance
    float m20;
    float m24; // inferred: weight of the fovy
    float m28;
    float m2C;
    float m30;
    float m34;
    int m38;
};

// inferred: parameters a camera mode imposes (GameCamera::m46C): mode (4 keeps the current one), weight and
// speed of the blend, and the parameters; names invented
struct GameCameraModeParam_unk {
    GameCameraModeParam_unk(); // inferred: clears mMode (inline, defined in game/GameCamera_unk.cpp)
    void fn_8037E89C(int mode, const GameCameraParam_unk& param, float step); // inferred: starts a mode
    int mMode;
    float mWeight;
    float mStep;
    GameCameraParam_unk mParam;
};

// inferred: shake of the camera (GameCamera::mShake: frame, length, width and height; names invented)
struct CameraShake_unk {
    CameraShake_unk(); // inferred: clears the shake (inline, defined in game/GameCamera_unk.cpp)
    atn::Vector3 GetOffset_unk() const; // 0x803933F4
    int mFrame;
    int mLength;
    float mWidth;
    float mHeight;
};

// Camera of the field (GameTask + 0x6C, constructed by the GameTask constructor; code in
// game/GameCamera_unk.cpp). Real name from the RSO (__ct__10GameCameraFv, Update__10GameCameraFPC9CharaTask,
// StartRollBack__10GameCameraFiif...); the constructor 0x8037D8B8 builds two atn::Camera at 0x00 and 0x1F4.
// Size inferred from the next member of GameTask (FinishCamera_unk at 0xD18). Member names invented.
class GameCamera : public atn::Camera {
public:
    GameCamera(); // 0x8037D8B8
    ~GameCamera(); // 0x8037DC68
    void fn_8039328C(); // inferred: end of the frame of the camera (called by GameTask::vf14)
    void fn_8037DD00(); // inferred: resets the camera (also called by the constructor)
    // inferred: reads the camera csv of the map (data and size of MapTask::m60) for a camera and a sub camera
    void fn_8037E12C(void* data, int size, int camera, int sub);
    void fn_8037E9E0(); // inferred: registers the debug menu of the camera
    void fn_80389884(int mode); // inferred: sets the camera mode (MapTask::fn_804A04AC: 0, 10 or 11)
    // inferred: follows the leader (fn_8037EF48 with the head of the player list)
    void fn_8037EF30(int a, int b, float f);
    void fn_8037EF48(CharaTask* chara, int a, int b, float f);
    void fn_8037F690(const atn::Vector3* pos); // inferred: looks at `pos` from the current position
    void fn_8037F6D8(const atn::Vector3* pos); // inferred: same, with m858 set
    void fn_8037F724(const atn::Vector3* pos); // inferred: moves the target only
    void fn_8037F9AC(const CharaTask* chara); // inferred: update of the default mode (table 0x8074E114)
    void fn_803897EC(const CharaTask* chara); // inferred: default mode with m8F0 set before frame 67 of the motion
    void fn_80389838(int mode); // inferred: sets the sub mode (previous one kept in m800)
    void fn_80382A1C(const CharaTask* chara);
    void fn_8038A1B8(const CharaTask* chara); // inferred: forced update of the current mode
    void fn_8038A1D8(const CharaTask* chara); // inferred: the character is not used (mode table entry)
    void fn_80390374(); // inferred: goes back to the mode m804
    void fn_8038F52C(CharaTask*); // inferred: chooses the mode from the action of the character
    void fn_80390A88(); // inferred: free camera of the tactical view
    void fn_803903E0(const atn::Vector3* target, float angle, int arg); // inferred: turns toward `target`
    void fn_80392C94(GameCameraState_unk* state) const; // inferred: saves the mode
    void fn_80392CE0(const GameCameraState_unk* state); // inferred: restores it
    void fn_803933A4(); // inferred: reset of the event state
    void fn_8037F744(); // inferred: update of the camera mode (tables 0x8074E018 and 0x8074E114)
    void fn_8037ECF0(const char* name, void* menu, GameCameraParam_unk* param); // inferred: debug menu of a mode
    // inferred: moves the camera from the saved one (m80C) to pos/target over t; with collide, the eye is moved
    // out of the walls
    void fn_8038AEF0(const atn::Vector3* pos, const atn::Vector3* target, const CharaTask* chara, int collide,
                     float fovy, float t);
    void fn_8038E540(const CharaTask* chara, GameCameraParam_unk* param, atn::Vector3* pos, atn::Vector3* target,
                     const atn::Vector3* charaPos); // inferred: collision of the eye
    // inferred: pushes the eye out of the walls around the character (0x8038E23C; uses m7FC, m89C; the target is
    // passed but not used)
    void fn_8038E23C(const CharaTask*, GameCameraParam_unk*, atn::Vector3*, atn::Vector3*);
    void fn_8038EF94(const GameCameraParam_unk*, int); // inferred: sets the parameters of a mode
    void fn_803918EC(); // inferred: ambient sway of the camera (xsca of the map)
    void fn_803920C8();
    void fn_80392510();
    void fn_80392FE8();
    void fn_803928C0(); // inferred: adds the offsets m93C/m948/m954 to the render camera
    // inferred: end of the update: copies the camera to m1F4, applies the selected corrections and gives
    // m1F4 to the render manager
    void fn_80392964(int a, int b, int c, int d);
    // inferred: functions of the mode tables of fn_8037F744 (start and update of each mode; not written)
    void fn_803830A8(const CharaTask* chara);
    void fn_8038378C(const CharaTask* chara);
    void fn_80387DE4(const CharaTask*); // inferred: bird view camera (setting mAlwaysBirdview)
    void fn_80384188(const CharaTask* chara);
    void fn_80385B48(const CharaTask* chara);
    void fn_80387548(const CharaTask* chara);
    void fn_80387910(const CharaTask* chara); // inferred: tactical view camera
    void fn_8038E9B0(const CharaTask* chara, GameCameraParam_unk* param, atn::Vector3* pos, atn::Vector3* target,
                     atn::Vector3* target2);
    void fn_803884B8(const CharaTask* chara);
    void fn_80388A88(const CharaTask* chara);
    void fn_80388C44(const CharaTask* chara);
    void fn_80389060(const CharaTask* chara);
    void fn_80389210(const CharaTask* chara);
    void fn_803893A0(const CharaTask* chara);
    void fn_8038952C(const CharaTask* chara);
    void fn_80389678(const CharaTask* chara);
    void fn_80389A00(const CharaTask* chara);
    void fn_8038A200(const CharaTask* chara);
    void fn_8038A5F0(const CharaTask* chara);
    void fn_8038B148(const CharaTask* chara);
    void fn_8038B650(const CharaTask* chara);
    void fn_8038BD78(const CharaTask* chara);
    void fn_8038C070(const CharaTask* chara);
    void fn_8038C5B0(const CharaTask* chara);
    void fn_8038C928(const CharaTask* chara);
    void fn_8038CD94(const CharaTask* chara);
    void fn_8038D1C0(const CharaTask* chara);
    void fn_8038D4D4(const CharaTask* chara);
    void fn_8038D854(const CharaTask* chara);
    void fn_8038DBE4(const CharaTask* chara);
    void fn_8038DDE0(const CharaTask* chara);
    void fn_8038DF5C(const CharaTask* chara);

    atn::Camera m1F4;      // 0x1F4: inferred: camera given to the render manager
    unsigned char m3E8[0x41C - 0x3E8];
    float m41C;
    unsigned char m420[0x424 - 0x420];
    GameCameraTimer_unk m424; // inferred: blend of the event camera (fn_8038CD94)
    GameCameraParam_unk m430; // inferred: parameters the blend m424 starts from (fn_8037E12C)
    GameCameraModeParam_unk m46C;
    GameCameraTimer_unk m4B4; // inferred: blend from m684 to m4C0
    GameCameraParam_unk m4C0;
    atn::Vector3 m4FC;     // inferred: position of the fixed mode (relative to the character when m518 is set)
    atn::Vector3 m508;     // inferred: target of the fixed mode
    float m514;            // inferred: fovy of the fixed mode
    int m518;              // inferred: m4FC/m508 follow the character
    GameCameraParam_unk m51C; // inferred: parameters of the "hips" side mode (copied by fn_8038F52C)
    GameCameraParam_unk m558; // inferred: head mode (fn_8038A200; m10: height of the eye)
    GameCameraParam_unk m594;
    GameCameraParam_unk m5D0; // inferred: hips mode (fn_8038B148; m0C-m14: offset of the eye, m18: height of the target)
    GameCameraParam_unk m60C; // inferred: aim mode (fn_8038B650; m0C-m14: offset of the eye, m18: lowered eye)
    GameCameraParam_unk m648; // inferred: side mode (m0C: side offset, m10: height, m14: distance, m1C: follow speed)
    GameCameraParam_unk m684;
    GameCameraParam_unk m6C0;
    GameCameraParam_unk m6FC;
    GameCameraParam_unk m738; // inferred: box camera mode (fn_80388A88; m0C-m14: position, m18: height of the target)
    GameCameraParam_unk m774; // inferred: event camera (fn_8038CD94; m08: pitch, m0C-m14: offset from the head,
                              // m28: yaw in degrees)
    // inferred: "WallUp" parameters of the debug menu (fn_8037E9E0): m04 fovy of the low side mode (fn_8038C5B0),
    // m0C/m10/m14 side offset, height and distance of the side camera (fn_803884B8)
    GameCameraParam_unk m7B0;
    ResHandle m7EC; // inferred: debug menu group "GameCamera" (fn_8037E9E0)
    int m7F0; // inferred: debug display of the camera (fn_8039328C)
    int m7F4;
    float m7F8; // inferred: yaw of the leader camera (fn_8037EF48)
    int m7FC;              // inferred: sub mode (fn_80389838)
    int m800;              // inferred: previous sub mode
    int m804;
    int m808;
    atn::Vector3 m80C;
    atn::Vector3 m818;
    atn::Vector3 m824; // inferred: m818 - m80C
    float m830;
    CharaTask* m834; // inferred: character looked at (GameEventScript::fn_803A7124)
    int m838;
    int m83C;
    atn::Vector3 m840;     // inferred: point looked at (fn_8037F690)
    atn::Vector3 m84C;
    int m858;
    float m85C;
    int m860;
    unsigned char m864[0x868 - 0x864];
    atn::Vector3 m868; // inferred: target of the tactical view camera (fn_80387910)
    atn::Vector3 m874;   // inferred: offset of the eye of the hips mode
    int m880;
    atn::Vector3 m884;   // inferred: eye of the hips mode when the character does not move (mFlag14)
    atn::Vector3 m890;   // inferred: target of the character (CharaTask::mF6C) saved by fn_8038C5B0
    int m89C;            // inferred: frames the eye has been hitting a wall (0-8, fn_8038E540)
    float m8A0; // inferred: yaw of the tactical view camera
    atn::Vector3 m8A4; // inferred: point the tactical view starts from (TacView::fn_805AA4E4)
    atn::Vector3 m8B0; // inferred: copy of m8A4 kept by the bird view camera (fn_80387DE4)
    atn::Vector3 m8BC; // inferred: point given to fn_803903E0
    float m8C8;        // inferred: its height
    int m8CC;          // inferred: fn_803903E0 pulls the start point out of the walls once
    int m8D0;
    int m8D4; // inferred: set at the end of the replay rewind (GameTask::fn_80373AE8)
    float m8D8; // inferred: debug menu "BirdViewLockOnSpeed"
    float m8DC; // inferred: debug menu "BirdViewLockOnLength"
    int m8E0; // inferred: free camera of the tactical view (fn_80390A88)
    int m8E4; // inferred: cleared by GameTask::vf10 when the tactical view closes
    int m8E8; // inferred: cleared by GameTask::vf10 at the end of a map change
    int m8EC;
    int m8F0;
    void* m8F4;            // 0x8F4: inferred: ambient animation of the map (MapTask::m68, LH_Xsca)
    float m8F8;            // 0x8F8: inferred: frame of the ambient animation
    float m8FC;            // 0x8FC: inferred: scale of the ambient animation (MapId_unk::m64)
    CameraShake_unk mShake; // 0x900: inferred: set from the event scripts (fn_803A450C keeps the parity of the frame)
    unsigned char m910;
    unsigned char m911;
    unsigned char m912[0x914 - 0x912];
    int m914;
    int m918; // inferred: maximum of m914
    atn::Vector3 m91C; // inferred: target the camera blends to (fn_80392510)
    atn::Vector3 m928; // inferred: blended target
    float m934;        // inferred: fovy the camera blends to (none when <= 0)
    float m938;        // inferred: blended fovy
    atn::Vector3 m93C; // inferred: offset of the position of the render camera (fn_803928C0)
    atn::Vector3 m948; // inferred: offset of the target
    float m954;        // inferred: offset of the fovy
    float m958;
    float m95C;
    unsigned char m960[0x96C - 0x960];
    float m96C; // inferred: cleared when the ledge mode starts (fn_8038A5F0)
    float m970;
    float m974;
    unsigned char m978[0x984 - 0x978];
    int m984;
    float m988;
    atn::Vector3 m98C[8]; // inferred: cleared by the default mode (fn_80389A00)
    float m9EC; // inferred: debug menu "InterpSpeed"
    int m9F0;   // inferred: debug menu "InterpFrame"
    int m9F4;   // inferred: debug menu "CollisionMode"
    atn::Vector3 m9F8;   // inferred: hips of the character when the hips mode ends
    float mA04; // inferred: debug menu "PadDisableAngle"
    int mA08;   // inferred: debug menu "PadDisableFrame"
    float mA0C; // inferred: turn speeds (cleared by fn_8037EF48 with `reset`)
    float mA10;
    float mA14; // inferred: turn speed of the bird view (classic controller, fn_80387DE4)
    float mA18;
    float mA1C;
    atn::Camera mA20;      // inferred: camera the render camera is blended from (fn_80392FE8, fn_80392D2C)
    float mC14;            // inferred: blend weight, decreased by mC18 every frame
    float mC18;
    float mC1C;            // inferred: zoom of the hips mode (0-10)
    gs::CameraBox* mC20;   // 0xC20: inferred: camera box (MapTask::fn_804A04AC)
    DofParam_unk mC24;     // inferred: depth of field saved while the camera box sets its own (fn_80389884)
    DofParam_unk mC54;     // inferred: depth of field of the camera box
    int mC84;              // inferred: mC54 is applied
    atn::Vector2_unk mC88[3]; // inferred: cleared by the constructor
    atn::Vector2_unk mCA0;
    float mCA8; // inferred: turn speed of the leader camera (SetConfig::mTurnSpeed when negative, fn_8037EF48)
};

#endif
