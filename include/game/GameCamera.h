#ifndef GAME_GAMECAMERA_H
#define GAME_GAMECAMERA_H

#include "math/Vec.h"

namespace gs {
struct CameraBox;
}
class CharaTask;

// Camera of the field (GameTask + 0x6C, constructed by the GameTask constructor; code in
// game/GameCamera_unk.cpp). Real name from the RSO (__ct__10GameCameraFv, Update__10GameCameraFPC9CharaTask,
// StartRollBack__10GameCameraFiif...); the constructor 0x8037D8B8 builds two atn::Camera at 0x00 and 0x1F4.
// Size inferred from the next member of GameTask (FinishCamera_unk at 0xD18). Member names invented.
class GameCamera {
public:
    GameCamera(); // 0x8037D8B8
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
    void fn_80389838(int mode); // inferred: sets the sub mode (previous one kept in m800)

    unsigned char m000[0x14];
    atn::Vector3 m014;     // inferred: position of the camera at 0x00
    unsigned char m020[0x50 - 0x20];
    float m050;
    unsigned char m054[0x7FC - 0x54];
    int m7FC;              // inferred: sub mode (fn_80389838)
    int m800;              // inferred: previous sub mode
    int m804;
    int m808;
    unsigned char m80C[0x838 - 0x80C];
    int m838;
    int m83C;
    atn::Vector3 m840;     // inferred: point looked at (fn_8037F690)
    atn::Vector3 m84C;
    int m858;
    float m85C;
    int m860;
    unsigned char m864[0x880 - 0x864];
    int m880;
    unsigned char m884[0x8F4 - 0x884];
    void* m8F4;            // 0x8F4: inferred: ambient animation of the map (MapTask::m68, LH_Xsca)
    int m8F8;
    float m8FC;            // 0x8FC: inferred: scale of the ambient animation (MapId_unk::m64)
    int m900;              // inferred: set from the event scripts (fn_803A450C keeps its parity)
    int m904;
    float m908;
    float m90C;
    unsigned char m910[0xA0C - 0x910];
    float mA0C[5];
    unsigned char mA20[0xC20 - 0xA20];
    gs::CameraBox* mC20;   // 0xC20: inferred: camera box (MapTask::fn_804A04AC)
    unsigned char mC24[0xCAC - 0xC24];
};

#endif
