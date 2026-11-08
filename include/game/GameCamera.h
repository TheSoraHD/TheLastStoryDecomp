#ifndef GAME_GAMECAMERA_H
#define GAME_GAMECAMERA_H

#include "math/Vec.h"
#include "atn/Camera_unk.h"

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
    void fn_8038A1D8();
    void fn_80390374(); // inferred: goes back to the mode m804
    void fn_803903E0(const atn::Vector3* target, float angle, int arg); // inferred: turns toward `target`
    void fn_80392C94(GameCameraState_unk* state); // inferred: saves the mode
    void fn_80392CE0(const GameCameraState_unk* state); // inferred: restores it
    void fn_803933A4(); // inferred: reset of the event state

    atn::Camera m1F4;      // 0x1F4: inferred: camera given to the render manager
    unsigned char m3E8[0x46C - 0x3E8];
    int m46C;
    unsigned char m470[0x4B8 - 0x470];
    float m4B8; // inferred: set from the replay rewind (GameTask::fn_80373AE8)
    unsigned char m4BC[0x4FC - 0x4BC];
    atn::Vector3 m4FC;     // inferred: position of the fixed mode (relative to the character when m518 is set)
    atn::Vector3 m508;     // inferred: target of the fixed mode
    float m514;            // inferred: fovy of the fixed mode
    int m518;              // inferred: m4FC/m508 follow the character
    unsigned char m51C[0x7F4 - 0x51C];
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
    unsigned char m864[0x880 - 0x864];
    int m880;
    unsigned char m884[0x8D4 - 0x884];
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
    unsigned char m918[0xA0C - 0x918];
    float mA0C[5];
    unsigned char mA20[0xC20 - 0xA20];
    gs::CameraBox* mC20;   // 0xC20: inferred: camera box (MapTask::fn_804A04AC)
    unsigned char mC24[0xC84 - 0xC24];
    int mC84;
    unsigned char mC88[0xCAC - 0xC88];
};

#endif
