#include "std/string.h"
#include "atn/MemoryManager.h"
#include "std/string_members.h"
#include "atn/RenderManager.h"
#include "atn/IRender.h"
#include "atn/Layer3D.h"
#include "atn/VideoTHP.h"
#include "atn/GraphicsDevice.h"

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;

// inferred: THP player of the SDK sample library (automatic names)
extern "C" int fn_805B55A8(int audio);            // THPPlayerInit
extern "C" int fn_805B5710(const char* name);     // THPPlayerOpen
extern "C" int fn_805B68A0(void* videoInfo);      // THPPlayerGetVideoInfo
extern "C" int fn_805B68E8(void* audioInfo);      // THPPlayerGetAudioInfo
extern "C" unsigned int fn_805B5A4C();            // THPPlayerCalcNeedMemory
extern "C" void fn_805B5AD4(void* buffer);        // THPPlayerSetBuffer
extern "C" void fn_805B6018();                    // THPPlayerStop
extern "C" void fn_805B602C();                    // THPPlayerClose
extern "C" void fn_805B59BC();
extern "C" void fn_805B56C0();                    // THPPlayerQuit
extern "C" int fn_805B5E7C(int arg);              // THPPlayerPrepare
extern "C" void fn_805B6004();                    // THPPlayerPlay
extern "C" void fn_805B6C44(int arg);
extern "C" unsigned int fn_805B6954();            // THPPlayerGetTotalFrame
extern "C" void fn_805B6120(int arg);
extern "C" float fn_805B6930();                   // THPPlayerGetFrameRate
extern "C" int fn_805B6C0C(void* self, float volume, int time); // inferred: receives the video unchanged
extern "C" int fn_805B6AE0(int volume, int time); // THPPlayerSetVolume
extern "C" int fn_805B6474(void* rmode, int x, int y, int w, int h); // THPPlayerDrawCurrentFrame
extern "C" void GXDrawDone();

extern "C" void OSPanic(const char* file, int line, const char* msg, ...);
extern "C" void fn_80087BB4(void* group, const char* name, float* value, float min, float max, float step, void* func,
                            void* arg); // inferred: debug slider for a 2D vector
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" int fn_800499D0(void* audio);          // inferred: AudioDevice output mode

// inferred: debug menu manager (lbl_8087EF10); fn_8008937C adds a group, fn_800897D8 removes it
struct DebugMenu_unk {
    unsigned char m00[0x10];
    unsigned char m10[1];
};
extern "C" DebugMenu_unk* lbl_8087EF10;
extern "C" void* fn_8008937C(void* parent, const char* name);
extern "C" void fn_800897D8(DebugMenu_unk* menu, void* group);

inline DebugGroup_unk::~DebugGroup_unk() {
    if (mGroup != 0 && lbl_8087EF10 != 0) {
        fn_800897D8(lbl_8087EF10, mGroup);
    }
}
inline void* DebugGroup_unk::Create(const char* name) {
    if (mGroup == 0 && lbl_8087EF10 != 0) {
        mGroup = fn_8008937C(lbl_8087EF10->m10, name);
        return mGroup;
    }
    return 0;
}

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: global of unknown class
extern "C" void* lbl_8087EE90;

namespace atn {

inline Video::Video() : m04(0), m08(0), m0C(0), m10(0.0f), mPos(0.0f, 0.0f), m1C(0) {}
inline Video::~Video() {}

inline void VideoTHP::SetName_unk(const char* name) {
    if (name != mName) {
        memcpy(mName, name, strlen(name) + 1);
    }
}

} // namespace atn

extern "C" atn::VideoTHP* lbl_8087F010;

namespace atn {

VideoTHP::VideoTHP(const char* name) {
    SetName_unk(name);
    m84.x = 1.0f;
    m84.y = 1.16f;
    m8C.x = 0.0f;
    m8C.y = -32.0f;
    m80 = 0;
    m94.mGroup = 0;
    void* group = m94.Create(fn_8006B174(std::string(name)).c_str());
    fn_80087BB4(group, "Scale", &m84.x, 0.1f, 100.0f, 0.01f, 0, 0);
    fn_80087BB4(group, "Offset", &m8C.x, -640.0f, 640.0f, 0.5f, 0, 0);
}

VideoTHP::~VideoTHP() {
    vf24();
}

void VideoTHP::vf20() {
    fn_805B55A8(1);
    fn_805B5710(mName);
    fn_805B68A0(m20);
    fn_805B68E8(m2C);
    unsigned int size = fn_805B5A4C();
    m3C = atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6, "", "", 0, 0);
    if (m3C == 0) {
        OSPanic("atn_video_thp.cpp", 0x35, "Can't allocate the memory\n");
    }
    fn_805B5AD4(m3C);
    m08 = 2;
    lbl_8087F010 = this;
}

void VideoTHP::vf24() {
    if (m08 != 0) {
        if (m08 == 4 || m08 == 1) {
            fn_805B6018();
            fn_805B602C();
        }
        fn_805B59BC();
        fn_805B56C0();
        atn::MemoryManager::Instance()->Free(m3C);
        m08 = 0;
        lbl_8087F010 = 0;
    }
}

void VideoTHP::vf28() {
    atn::RenderManager::instance_->AddRender(this, 0x12, m10);
}

// NONMATCHING: 99.28% - only volatile register choices differ (r8/r9 for the converted height,
// r6/r8 for the converted draw height, and one lis/addi pair); tried variable orders and temporaries
void VideoTHP::vf10(RenderManager*) {
    switch (m08) {
    case 3:
        fn_805B5E7C(1);
        fn_805B6004();
        if (lbl_8087EE90 != 0) {
            fn_805B6C44(fn_800499D0(lbl_8087EE90) != 3);
        }
        m08 = 4;
        m80 = 0;
    case 4:
        if (m80 >= fn_805B6954() - 1) {
            m1C = 1;
        }
        if (m0C == 0 && m80 >= fn_805B6954() - 1) {
            m08 = 1;
        } else {
            fn_805B6120(0);
            break;
        }
    case 1:
        fn_805B6018();
        fn_805B602C();
        m08 = 2;
    case 2:
        return;
    }
    atn::GraphicsDevice_unk* rs = lbl_8087EEE0;
    int h = rs->mHeight;
    float aspect = (float)h / (float)(int)rs->mRMode.viHeight;
    int w = rs->mWidth;
    int x;
    int y;
    int dw;
    int dh;
    if (m04 == 1) {
        dw = m20[0];
        dh = m20[1];
        x = (int)(mPos.x - (float)(m20[0] >> 1));
        y = (int)(mPos.y - (float)(m20[1] >> 1));
    } else if (m04 == 2 || m04 == 3) {
        if (rs->mWide != 0) {
            dh = h;
            dw = w;
            x = 0;
            y = 0;
        } else {
            if (m04 == 2) {
                dh = h - 64;
                dw = (int)((float)dh * ((float)(w * 48 / 36) / aspect) / (float)h);
            } else {
                dh = h;
                dw = w * 48 / 36;
            }
            x = (w - dw) / 2;
            y = (h - dh) / 2;
        }
    } else {
        if (rs->mWide != 0) {
            dh = 416;
            dw = w;
            x = 0;
            y = (int)(0.5f * ((float)h - (float)dh));
        } else {
            dh = h - 64;
            y = 32;
            dw = (int)((float)(w * dh / (h * 36 / 48)) / aspect);
            x = (w - dw) / 2;
        }
    }
    m80 = fn_805B6474(rs, x, y, dw, dh);
    GXDrawDone();
    atn::RenderManager::instance_->mLayers[18]->vf1C(); // inferred: layer "2D Object"
}

void VideoTHP::vf18(RenderManager*) {}

void VideoTHP::vf3C(float volume, int time) {
    fn_805B6AE0((int)(127.0f * volume), time * 1000 / 30);
}

void VideoTHP::vf38(float volume, int time) {
    fn_805B6C0C(this, volume, time);
}

unsigned int VideoTHP::vf30() {
    return m80;
}

unsigned int VideoTHP::vf34() {
    return fn_805B6954();
}

float VideoTHP::vf2C() {
    return fn_805B6930();
}

int VideoTHP::vf14(const Matrix3x4*, const Vector3*, const Frustum_unk*, AABB_unk*) {
    return 1;
}

} // namespace atn
