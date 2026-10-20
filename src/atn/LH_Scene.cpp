#include "atn/LH_Scene.h"
#include "atn/Array_unk.h"
#include "atn/LoadManager.h"
#include "atn/Loader_unk.h"
#include "math/Vec.h"

// inferred: animation track of the scene file (12 bytes); evaluates the value at `frame`
struct SceneTrack_unk {
    float fn_8047202C(const short& frame, float defaultValue);
    unsigned char m00[0xC];
};

// inferred: camera of the scene file
struct SceneCameraData_unk {
    unsigned char m00[0x14];
    int mType;               // 0x14: 1 = position and target
    int mFovType;            // 0x18
    unsigned char m1C[4];
    SceneTrack_unk mPos[3];    // 0x20
    SceneTrack_unk mTarget[3]; // 0x44
    SceneTrack_unk mDir[3];    // 0x68
    SceneTrack_unk m8C;
    SceneTrack_unk m98;        // field of view
    SceneTrack_unk mA4;
    SceneTrack_unk mB0;
    SceneTrack_unk mBC;
    SceneTrack_unk mC8;
};

// inferred: header of the scene file (at 0x10 of the loaded data)
struct SceneHeader_unk {
    unsigned char m00[0x10];
    int mNumCameras;                  // 0x10
    SceneCameraData_unk** mCameras;   // 0x14
    unsigned char m18[8];
    int mNumLights;                   // 0x20
    void** mLights;                   // 0x24
};

// inferred: engine camera (0x1F4 bytes; constructor 0x8004B290 with default arguments,
// destructor 0x8004B338, update 0x8004B378)
struct Camera_unk {
    Camera_unk(int a = 0, int b = 0);
    ~Camera_unk();
    void fn_8004B378();

    int mType;               // 0x00
    int mFovType;            // 0x04
    float mPos[3];           // 0x08
    float mTarget[3];        // 0x14
    unsigned char m20[0xC];
    atn::Vector3 mDir;       // 0x2C
    float m38;
    float m3C;
    float m40;
    float m44;
    float m48;
    unsigned char m4C[4];
    float m50;
    float mFov;              // 0x54
    unsigned char m58[0xC8 - 0x58];
    float mC8;
    float mCC;
    unsigned char mD0[0x1F4 - 0xD0];
};

// inferred: engine light (0x44 bytes; constructor 0x80079044, no destructor)
struct LightPair_unk {
    int a;
    int b;
};
struct LightData_unk;
struct Light_unk {
    Light_unk();
    Light_unk(LightData_unk* data); // inferred: builds a light from the file data (atn/Light_unk.cpp)
    int m00;
    int m04;
    atn::Vector3 m08;
    atn::Vector3 m14;
    int m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    LightPair_unk m34;
    LightPair_unk m3C;
};

// inferred: value of a track at frame 0 (each call has its own frame variable on the stack)
static inline float __EvalTrack_unk(SceneTrack_unk& track) {
    short frame = 0;
    return track.fn_8047202C(frame, 0.0f);
}

namespace atn {

// inferred from __vt__Q23atn9LoadScene (0x8078FF38)
class LoadScene : public LoadCore {
public:
    LoadScene(const char* path) : LoadCore(path) {}
    virtual ~LoadScene();
    virtual void vf0C();
    void fn_804778DC(SceneHeader_unk* header); // inferred: builds the cameras and lights

    int m5C;
    Buffer_unk<Light_unk> mLights;   // 0x60
    Buffer_unk<Camera_unk> mCameras; // 0x68
};

LoadScene::~LoadScene() {}

void LoadScene::vf0C() {
    if (mState == 2) {
        fn_80473104(3);
        Loader_unk loader;
        fn_804714A4(&loader);
        fn_804714B0(&loader, m54, m50, LoaderCallback_unk(), 0);
        fn_804778DC((SceneHeader_unk*)((char*)m54 + 0x10));
    }
}

void LoadScene::fn_804778DC(SceneHeader_unk* header) {
    mCameras.Resize_unk(header->mNumCameras, 0);
    for (int i = 0; i < header->mNumCameras; i++) {
        SceneCameraData_unk* data = header->mCameras[i];
        Camera_unk* camera = &mCameras[i];
        float x = __EvalTrack_unk(data->mPos[0]);
        float y = __EvalTrack_unk(data->mPos[1]);
        float z = __EvalTrack_unk(data->mPos[2]);
        camera->mPos[0] = x;
        camera->mPos[1] = y;
        camera->mPos[2] = z;
        float fov = __EvalTrack_unk(data->m98);
        camera->mFov = fov;
        camera->mC8 = __EvalTrack_unk(data->mA4);
        camera->mCC = __EvalTrack_unk(data->mB0);
        if (data->mType == 1) {
            float tx = __EvalTrack_unk(data->mTarget[0]);
            float ty = __EvalTrack_unk(data->mTarget[1]);
            float tz = __EvalTrack_unk(data->mTarget[2]);
            camera->mTarget[0] = tx;
            camera->mTarget[1] = ty;
            camera->mTarget[2] = tz;
            camera->m38 = __EvalTrack_unk(data->m8C);
            camera->mType = 2;
        } else {
            float dx = __EvalTrack_unk(data->mDir[0]);
            float dy = __EvalTrack_unk(data->mDir[1]);
            float dz = __EvalTrack_unk(data->mDir[2]);
            camera->mDir = Vector3(dx, dy, dz);
            camera->mType = 1;
        }
        if (data->mFovType == 1) {
            camera->m50 = __EvalTrack_unk(data->mBC);
            camera->mFovType = 0;
        } else {
            float v = __EvalTrack_unk(data->mC8);
            camera->m3C = v;
            camera->m40 = -v;
            camera->m44 = -v * fov;
            camera->m48 = v * fov;
            camera->mFovType = 1;
        }
        camera->fn_8004B378();
    }
    mLights.Resize_unk(header->mNumLights, 0);
    for (int i = 0; i < header->mNumLights; i++) {
        Light_unk light((LightData_unk*)header->mLights[i]);
        mLights[i] = light;
    }
}

void LH_Scene::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadScene>(path, 1);
}

} // namespace atn
