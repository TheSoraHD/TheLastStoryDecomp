#ifndef GAME_GAMECAMERA_H
#define GAME_GAMECAMERA_H

#include "math/Vec.h"
#include "atn/Camera_unk.h"
#include "atn/RenderSetting_unk.h"

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
    void fn_803903E0(const atn::Vector3* target, float angle, int arg); // inferred: turns toward `target`
    void fn_80392C94(GameCameraState_unk* state) const; // inferred: saves the mode
    void fn_80392CE0(const GameCameraState_unk* state); // inferred: restores it
    void fn_803933A4(); // inferred: reset of the event state
    void fn_8037F744(); // inferred: update of the camera mode (tables 0x8074E018 and 0x8074E114)
    void fn_803918EC(); // inferred: corrections of the camera (not written)
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
    unsigned char m3E8[0x46C - 0x3E8];
    int m46C;
    unsigned char m470[0x4B4 - 0x470];
    GameCameraTimer_unk m4B4; // inferred: blend from m684 to m4C0
    GameCameraParam_unk m4C0;
    atn::Vector3 m4FC;     // inferred: position of the fixed mode (relative to the character when m518 is set)
    atn::Vector3 m508;     // inferred: target of the fixed mode
    float m514;            // inferred: fovy of the fixed mode
    int m518;              // inferred: m4FC/m508 follow the character
    unsigned char m51C[0x684 - 0x51C];
    GameCameraParam_unk m684;
    unsigned char m6C0[0x7F0 - 0x6C0];
    int m7F0; // inferred: debug display of the camera (fn_8039328C)
    int m7F4;
    unsigned char m7F8[0x7FC - 0x7F8];
    int m7FC;              // inferred: sub mode (fn_80389838)
    int m800;              // inferred: previous sub mode
    int m804;
    int m808;
    atn::Vector3 m80C;
    atn::Vector3 m818;
    unsigned char m824[0x830 - 0x824];
    float m830;
    int m834;
    int m838;
    int m83C;
    atn::Vector3 m840;     // inferred: point looked at (fn_8037F690)
    atn::Vector3 m84C;
    int m858;
    float m85C;
    int m860;
    unsigned char m864[0x868 - 0x864];
    atn::Vector3 m868; // inferred: target of the tactical view camera (fn_80387910)
    unsigned char m874[0x880 - 0x874];
    int m880;
    unsigned char m884[0x8A0 - 0x884];
    float m8A0; // inferred: yaw of the tactical view camera
    atn::Vector3 m8A4; // inferred: point the tactical view starts from (TacView::fn_805AA4E4)
    unsigned char m8B0[0x8D4 - 0x8B0];
    int m8D4; // inferred: set at the end of the replay rewind (GameTask::fn_80373AE8)
    unsigned char m8D8[0x8E4 - 0x8D8];
    int m8E4; // inferred: cleared by GameTask::vf10 when the tactical view closes
    int m8E8; // inferred: cleared by GameTask::vf10 at the end of a map change
    int m8EC;
    int m8F0;
    void* m8F4;            // 0x8F4: inferred: ambient animation of the map (MapTask::m68, LH_Xsca)
    int m8F8;
    float m8FC;            // 0x8FC: inferred: scale of the ambient animation (MapId_unk::m64)
    int m900;              // inferred: set from the event scripts (fn_803A450C keeps its parity)
    int m904;
    float m908;
    float m90C;
    unsigned char m910;
    unsigned char m911[0x914 - 0x911];
    int m914;
    int m918; // inferred: maximum of m914
    unsigned char m91C[0x93C - 0x91C];
    atn::Vector3 m93C; // inferred: offset of the position of the render camera (fn_803928C0)
    atn::Vector3 m948; // inferred: offset of the target
    float m954;        // inferred: offset of the fovy
    unsigned char m958[0xA0C - 0x958];
    float mA0C[5];
    unsigned char mA20[0xA28 - 0xA20];
    atn::Vector3 mA28;     // inferred: position the render camera is blended from (fn_80392FE8)
    atn::Vector3 mA34;     // inferred: target of the blend
    atn::Vector3 mA40;     // inferred: up vector of the blend
    unsigned char mA4C[0xA70 - 0xA4C];
    float mA70;            // inferred: fovy of the blend
    unsigned char mA74[0xC14 - 0xA74];
    float mC14;            // inferred: blend weight, decreased by mC18 every frame
    float mC18;
    unsigned char mC1C[0xC20 - 0xC1C];
    gs::CameraBox* mC20;   // 0xC20: inferred: camera box (MapTask::fn_804A04AC)
    DofParam_unk mC24;     // inferred: depth of field saved while the camera box sets its own (fn_80389884)
    DofParam_unk mC54;     // inferred: depth of field of the camera box
    int mC84;              // inferred: mC54 is applied
    unsigned char mC88[0xCAC - 0xC88];
};

#endif
