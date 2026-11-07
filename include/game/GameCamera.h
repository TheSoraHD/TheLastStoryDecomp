#ifndef GAME_GAMECAMERA_H
#define GAME_GAMECAMERA_H

namespace gs {
struct CameraBox;
}

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

    unsigned char m000[0x8F4];
    void* m8F4;            // 0x8F4: inferred: ambient animation of the map (MapTask::m68, LH_Xsca)
    int m8F8;
    float m8FC;            // 0x8FC: inferred: scale of the ambient animation (MapId_unk::m64)
    int m900;              // inferred: set from the event scripts (fn_803A450C keeps its parity)
    int m904;
    float m908;
    float m90C;
    unsigned char m910[0xC20 - 0x910];
    gs::CameraBox* mC20;   // 0xC20: inferred: camera box (MapTask::fn_804A04AC)
    unsigned char mC24[0xCAC - 0xC24];
};

#endif
