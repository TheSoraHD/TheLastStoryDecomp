#include "std/string.h"
#include "atn/MemoryManager.h"
#include "std/string_members.h"

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
extern "C" void fn_80087BB4(void* group, const char* name, float* value, int arg3, int arg4, float min,
                            float max, float step); // inferred: debug slider for a 2D vector
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" int fn_800499D0(void* audio);          // inferred: AudioDevice output mode
extern "C" void fn_800BDB58(void* camera, void* video, float alpha, int arg);

// inferred: debug menu manager (lbl_8087EF10); fn_8008937C adds a group, fn_800897D8 removes it
struct DebugMenu_unk {
    unsigned char m00[0x10];
    unsigned char m10[1];
};
extern "C" DebugMenu_unk* lbl_8087EF10;
extern "C" void* fn_8008937C(void* parent, const char* name);
extern "C" void fn_800897D8(DebugMenu_unk* menu, void* group);

// inferred: owning handle to a debug menu group (like game/ResHandle.h, without constructor)
struct DebugGroup_unk {
    ~DebugGroup_unk() {
        if (mGroup != 0 && lbl_8087EF10 != 0) {
            fn_800897D8(lbl_8087EF10, mGroup);
        }
    }
    void* Create(const char* name) {
        if (mGroup == 0 && lbl_8087EF10 != 0) {
            mGroup = fn_8008937C(lbl_8087EF10->m10, name);
            return mGroup;
        }
        return 0;
    }
    void* mGroup;
};

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: globals of unknown classes
struct RenderState_unk {
    unsigned char m00[0x10];
    unsigned short m10; // inferred: frame buffer height
    unsigned char m12[0x3C - 0x12];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
    int m44;     // inferred: progressive / 16:9 flag
};
struct Drawer_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
};
struct Camera_unk {
    unsigned char m00[0x48];
    Drawer_unk* m48;
};
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void* lbl_8087EE90;

namespace atn {

class VideoTHP;

struct Vector2f_unk {
    float x, y;
};

// inferred: renderer interface (RTTI 0x80777518, emitted with its key function in another file)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C(); // 0x80046218
    virtual void vf10();
    virtual int vf14() = 0;
    virtual void vf18() = 0;
    virtual void vf1C(); // 0x800461FC
};

// inferred: video player interface (RTTI emitted here)
class Video : public IRender {
public:
    Video() : m04(0), m08(0), m0C(0), m10(0.0f), m14(0.0f), m18(0.0f), m1C(0) {}
    virtual ~Video() {}
    virtual void vf20() = 0;
    virtual void vf24() = 0;
    virtual void vf28() = 0;
    virtual float vf2C() = 0;
    virtual unsigned int vf30() = 0;
    virtual unsigned int vf34() = 0;
    virtual void vf38(float volume, int time) = 0;
    virtual void vf3C(float volume, int time) = 0;

    int m04;   // inferred: layout mode
    int m08;   // inferred: state
    int m0C;   // inferred: loop
    float m10; // inferred: alpha
    float m14; // inferred: x
    float m18; // inferred: y
    int m1C;   // inferred: finished
};

class VideoTHP : public Video {
public:
    VideoTHP(const char* name);
    virtual ~VideoTHP();
    virtual void vf10();
    virtual int vf14();
    virtual void vf18();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual float vf2C();
    virtual unsigned int vf30();
    virtual unsigned int vf34();
    virtual void vf38(float volume, int time);
    virtual void vf3C(float volume, int time);

    void SetName_unk(const char* name) {
        if (name != mName) {
            memcpy(mName, name, strlen(name) + 1);
        }
    }

    unsigned int m20[3];     // THPVideoInfo
    unsigned int m2C[4];     // THPAudioInfo
    void* m3C;               // inferred: THP work buffer
    char mName[0x40];        // 0x40
    unsigned int m80;        // inferred: current frame
    Vector2f_unk m84;        // inferred: scale
    Vector2f_unk m8C;        // inferred: offset
    DebugGroup_unk m94;      // inferred: debug menu group
};

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
    fn_80087BB4(group, "Scale", &m84.x, 0, 0, 0.1f, 100.0f, 0.01f);
    fn_80087BB4(group, "Offset", &m8C.x, 0, 0, -640.0f, 640.0f, 0.5f);
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
    fn_800BDB58(lbl_8087EFB4, this, m10, 0x12);
}

// NONMATCHING: 99.28% - only volatile register choices differ (r8/r9 for the converted height,
// r6/r8 for the converted draw height, and one lis/addi pair); tried variable orders and temporaries
void VideoTHP::vf10() {
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
    RenderState_unk* rs = lbl_8087EEE0;
    int h = rs->mHeight;
    float aspect = (float)h / (float)(int)rs->m10;
    int w = rs->mWidth;
    int x;
    int y;
    int dw;
    int dh;
    if (m04 == 1) {
        dw = m20[0];
        dh = m20[1];
        x = (int)(m14 - (float)(m20[0] >> 1));
        y = (int)(m18 - (float)(m20[1] >> 1));
    } else if (m04 == 2 || m04 == 3) {
        if (rs->m44 != 0) {
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
        if (rs->m44 != 0) {
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
    lbl_8087EFB4->m48->vf1C();
}

void VideoTHP::vf18() {}

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

int VideoTHP::vf14() {
    return 1;
}

} // namespace atn
