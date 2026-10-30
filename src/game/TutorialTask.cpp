#include "game/TutorialTask.h"
#include "atn/Memory.h"
#include "atn/Csv.h"
#include "atn/D2A_Task.h"
#include "atn/VideoTHP.h"
#include "atn/SoundRef.h"
#include "atn/Keyboard.h"
#include "game/CharaTask.h"
#include "game/SetConfig.h"
#include "game/Input_unk.h"
#include "game/EnvSeManager.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" TutorialData_unk* fn_8021F09C(int id); // inferred: tutorial by id
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value); // inferred: sets a value
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text); // inferred: sets a text
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" void fn_80097A9C(void* model, int id, atn::LH_Motion* motion); // inferred: sets a motion slot
extern "C" atn::SoundRef fn_800C31F4(const char* name, int a, int b, float volume); // inferred: plays a sound
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" float fn_8006EF48(void* font, const wchar_t* text, int a, int b, float size, float spacing); // inferred: text width
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector


// inferred: settings (lbl_8087F0A8); only the members used here
struct TutorialSettings_unk {
    unsigned char m000[0x484];
    float m484;         // inferred: offset of the popups
    unsigned char m488[0x48C - 0x488];
    Input_unk mInput;   // 0x48C
};
extern "C" TutorialSettings_unk* lbl_8087F0A8;
extern "C" void* lbl_8087EF70; // inferred: pad

class HomeButtonMenu {
public:
    static HomeButtonMenu* sInstance;
    unsigned char m00[0x88];
    int m88; // inferred: the home button menu is disabled
};

static const char* const sSe = "SE_UICOM_022";
// inferred: tutorials with a movie (compared with the name of the tutorial)
static const char* const sMovies[5] = {"tt005", "tt006", "tt007", "tt007b", "tt009"};


TutorialTask* TutorialTask::fn_805B9F50(atn::Task* parent, int id, CharaTask* chara) {
    return new ((MemoryTag)4, "", "", 0) TutorialTask(parent, id, chara);
}

TutorialTask::TutorialTask(atn::Task* parent, int id, CharaTask* chara)
    : atn::Task(parent), mId(id), mData(fn_8021F09C(id)), mChara(chara), m84(0.0f), mState(0), mCamera(0, 0),
      mCamera2(0, 0), mLoading(1), mPage(0), m5B4(0), m5B8(0), m5BC(0), m5C0(0) {
    char path[0x100];
    sprintf(path, "database/tutorial/%s", mData->mName);
    mFile.vf0C(path);
    if (mChara != 0) {
        mChara->mMotion.m34C = 1;
        mChara->mMotion.mTracks[0].m20 = 1.0f;
        mChara->mMotion.mTracks[0].m0C = 0.0f;
    }
}

int TutorialTask::vf0C() {
    if (mLoading != 0) {
        if (!mFile.fn_80473F50()) {
            mLoading = 0;
            fn_805BB654();
            mFile.fn_80473F88();
        }
        return 0;
    }
    int loading = 0;
    for (unsigned int i = 0; i < 4; i++) {
        if (mPages[i].mEnabled != 0 && mPages[i].mMotion.mLoader.fn_800A0448() != 0) {
            loading = 1;
        }
    }
    if (loading == 0 && fn_800D3FA4() != 0) {
        for (unsigned int i = 0; i < 4; i++) {
            TutorialPage_unk* page = &mPages[i];
            if (page->mEnabled != 0) {
                page->mLayout->mFlags |= 4;
                page->mLayout->fn_800D246C(0);
                page->mLayout->SetText_unk("comment", (int)mData->mText);
                if (page->mVideo != 0) {
                    atn::D2A_Task* layout = page->mLayout;
                    const wchar_t* text = fn_80116FC0(0, page->mMsg0);
                    fn_801FEE08(&layout->mData.m10, fn_800DC6B4("string0009(1)(1)"), text);
                    layout = page->mLayout;
                    text = fn_80116FC0(0, page->mMsg1);
                    fn_801FEE08(&layout->mData.m10, fn_800DC6B4("string0009(1)"), text);
                }
                if (page->mVideoLayout != 0) {
                    page->mVideoLayout->mFlags |= 4;
                    page->mVideoLayout->fn_800D246C(0);
                    page->mVideoLayout->mSpeed = 0.0f;
                    page->mVideoLayout->mRepeat = 0;
                }
            }
        }
        return 1;
    }
    return 0;
}

// inferred: weak copy 0x805BA920 of an inline accessor of the motion loader (name invented)
inline int HasMotion_unk(atn::LH_Motion* motion) {
    return motion->m08 != 0;
}

void TutorialTask::fn_805BA354(const Camera_unk* camera) {
    for (unsigned int i = 0; i < 4; i++) {
        TutorialPage_unk* page = &mPages[i];
        if (page->mEnabled != 0) {
            if (page->mMotion.mLoader.m08 != 0 && mChara != 0) {
                fn_80097A9C(&mChara->mMotion, i + 0xEA, &page->mMotion.mLoader);
            }
            page->mLayout->mFrame = 0.0f;
            page->mLayout->mFlags |= 4;
        }
    }
    mCamera = *camera;
    mPage = 0;
    mCamera2 = mCamera;
    if (mChara != 0) {
        mPos = mChara->mPos;
        mRot = mChara->mRot;
    } else {
        mPos = lbl_807C7030;
        mRot = lbl_807C7030;
    }
    fn_805BA934();
}

class SoundManager_unk;
extern "C" SoundManager_unk* lbl_8087EFE8;
extern "C" int fn_800D089C(SoundManager_unk* mgr, int a, int b);

void TutorialTask::fn_805BA934() {
    m84 = 0.0f;
    m5B4 = 0;
    m5C0 = 0;
    if (mPages[mPage].mType == 1) {
        m5B4 = SetConfig::sInstance->mAttackType != 0;
    }
    mState = 1;
    fn_805BC1C0();
    fn_800C31F4(sSe, 0, -1, 0.5f);
    m5BC = 1;
    HomeButtonMenu::sInstance->m88 = 1;
    if (mPages[mPage].mVideo != 0) {
        m5BC = 0;
        mPages[mPage].mVideo->vf20();
        mPages[mPage].mLayout->ApplyFrame();
        atn::D2A_DrawRect pos = fn_801F4E8C(mPages[mPage].mLayout, fn_800DC6B4("tt_mv"));
        mPages[mPage].mVideo->m10 = pos.z;
        mPages[mPage].mVideo->m04 = 1;
        mPages[mPage].mVideo->m0C = 1;
        pos.x -= 256.0f;
        pos.y -= 112.0f;
        pos.z -= 0.0001f;
        fn_801FED24(&mPages[mPage].mVideoLayout->mData.m10, fn_800DC6B4("basePoint"), 0, pos.x);
        fn_801FED24(&mPages[mPage].mVideoLayout->mData.m10, fn_800DC6B4("basePoint"), 1, pos.y);
        fn_801FED24(&mPages[mPage].mVideoLayout->mData.m10, fn_800DC6B4("basePoint"), 4, pos.z);
        if (lbl_8087EFE8 != 0 && fn_800D089C(lbl_8087EFE8, 1, 0) > 0 && lbl_8087F418 != 0) {
            lbl_8087F418->fn_803606CC(0, 8, 15);
            lbl_8087F418->fn_80360780(0, 8, 15);
        }
        HomeButtonMenu::sInstance->m88 = 0;
    }
}

// inferred: helpers of the field and of the UI (names keep the addresses)
struct TutorialField_unk;
extern "C" TutorialField_unk* lbl_8087F430;
extern "C" void fn_802F0990();
extern "C" void fn_80366DF8(int show);
extern "C" void fn_80148B38_unk(CharaTask* chara, float dist);
extern "C" int fn_803D6F44(atn::D2A_Task* gauge);
extern "C" void fn_803D2134(atn::D2A_Task* gauge, float speed);
extern "C" void fn_803D6F68(atn::D2A_Task* gauge, float value);
extern "C" void fn_8037F688(Camera_unk* camera, float fovy);
extern "C" Camera_unk* fn_8008B964();
extern "C" void fn_80392A04(Camera_unk* a, Camera_unk* b);
extern "C" void fn_8037529C(int a, int color, int b, int c, int frames);
extern "C" float fn_800F8524(const float* a, const float* b, float t); // inferred: lerp of two floats
extern "C" void C_QUATSlerp(const atn::Quaternion_unk* p, const atn::Quaternion_unk* q, atn::Quaternion_unk* r,
                            float t);
extern "C" void fn_805F9AB0(atn::Quaternion_unk* q, const atn::Vector3* axis, float rad); // inferred: C_QUATRotAxisRad

// inferred: quaternion of a yaw (weak copy 0x805BB50C; name invented)
inline atn::Quaternion_unk YawQuat_unk(float yaw) {
    atn::Quaternion_unk q;
    atn::Vector3 axis(0.0f, 1.0f, 0.0f);
    fn_805F9AB0(&q, &axis, yaw);
    return q;
}
// inferred: weak copy 0x805BB544 (name invented)
inline atn::Quaternion_unk Slerp_unk(const atn::Quaternion_unk& a, const atn::Quaternion_unk& b, float t) {
    atn::Quaternion_unk q;
    C_QUATSlerp(&a, &b, &q, t);
    return q;
}

// NONMATCHING: written from m2c; the original calls most of its inline helpers out of line (the
// weak copies 0x805BB50C-0x805BB594 follow it)
void TutorialTask::vf10() {
    TutorialPage_unk* page = &mPages[mPage];
    switch (mState) {
    case 1:
        if (HasMotion_unk(&page->mMotion.mLoader) && mChara != 0) {
            fn_802F0990();
            fn_80366DF8(1);
            m84 = m84 + 1.0f;
            float rate = m84 / 15.0f > 1.0f ? 1.0f : m84 / 15.0f;
            atn::Vector3 pos(page->mPos);
            atn::Vector3 target(page->mTarget);
            atn::Vector3 base = atn::Lerp_unk(mPos, mPos2, 1.5f * rate > 1.0f ? 1.0f : 1.5f * rate);
            atn::Quaternion_unk q0 = YawQuat_unk(mRot.y);
            atn::Quaternion_unk q1 = YawQuat_unk(mRot2.y);
            atn::Quaternion_unk q = Slerp_unk(q0, q1, 1.5f * rate > 1.0f ? 1.0f : 1.5f * rate);
            atn::Matrix3x4 m0;
            m0.fn_80147A08(q);
            atn::Matrix3x4 m1;
            m1.fn_80147A08(q1);
            pos.fn_80011410(m1);
            target.fn_80011410(m1);
            m84 = m84;
            mCamera2.SetPosition(pos);
            mCamera2.SetTarget_unk(target);
            mCamera2.fn_8004B378();
            *fn_8008B964() = mCamera2;
        } else if (m84 < 15.0f) {
            m84 = 15.0f;
        }
        if (m84 >= 15.0f) {
            if (m5C0 == 0) {
                m5C0 = 0;
            }
        }
        break;
    case 3:
        if (m5B8 != 0) {
        } else {
            fn_8037529C(-1, 0xFFFFFF, 0, 0, 15);
        }
        fn_805BBD7C();
        return;
    case 2:
        m84 = m84 + 1.0f;
        if (m84 >= 15.0f) {
            fn_80392A04(&mCamera, &mCamera2);
            mPos = mPos2;
            mRot = mRot2;
            fn_805BA934();
        }
        break;
    }
}

void TutorialTask::vf14() {
    if (mState == 1 && mPage < 4 && mPages[mPage].mVideo != 0) {
        atn::D2A_Task* layout = mPages[mPage].mLayout;
        atn::D2A_DrawRect pos = fn_801F4E8C(layout, fn_800DC6B4("tt_mv"));
        if (mPages[mPage].mLayout->mFrame >= pos.w) {
            mPages[mPage].mVideo->vf28();
        }
    }
}

extern "C" std::string fn_8006AC08(const std::string& path); // inferred: extension of a path
extern "C" std::string fn_8006B2D8(const std::string& path); // inferred: name without extension
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b); // inferred: joins two paths
extern "C" std::string fn_8006B46C(const std::string& path); // inferred
extern "C" void fn_8006AD24(std::string& path, const std::string& ext); // inferred: adds an extension
extern "C" int fn_8006BA30(); // inferred: language
extern "C" const char* fn_8006BA38(); // inferred: language code
extern "C" int fn_8006AA20(const char* path); // inferred: the file exists

// NONMATCHING: written from m2c (movie path built from the name of the page)
void TutorialTask::fn_805BB654() {
    atn::Csv csv((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
    while (csv.fn_8005BBF8() != 0) {
        unsigned int index = atoi(csv.fn_8005B9CC()) - 1;
        if (index <= 3) {
            TutorialPage_unk* page = &mPages[index];
            page->mEnabled = 1;
            (page->mLayout = fn_801F3FF8(this, csv.fn_8005B9CC(), 0))->fn_800D246C(1);
            page->mLayout->mRepeat = 0;
            page->mType = atoi(csv.fn_8005B9CC());
            page->m38 = fn_800DC288(csv.fn_8005B9CC());
            const char* motion = csv.fn_8005B9CC();
            if (*motion != 0) {
                page->mMotion.mLoader.fn_800A03E4(motion);
            }
            page->mFrame = fn_800DC288(csv.fn_8005B9CC());
            page->mPos.x = fn_800DC288(csv.fn_8005B9CC());
            page->mPos.y = fn_800DC288(csv.fn_8005B9CC());
            page->mPos.z = fn_800DC288(csv.fn_8005B9CC());
            page->mTarget.x = fn_800DC288(csv.fn_8005B9CC());
            page->mTarget.y = fn_800DC288(csv.fn_8005B9CC());
            page->mTarget.z = fn_800DC288(csv.fn_8005B9CC());
            page->mFovy = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
            const char* movie = csv.fn_8005B9CC();
            if (strcmp(movie, "") != 0) {
                std::string name(movie);
                std::string path = "movie/" + name;
                if (fn_8006BA30() != 0 && fn_8006BA30() != 1) {
                    std::string base = fn_8006B2D8(name);
                    for (unsigned int i = 0; i < 5; i++) {
                        if (base == sMovies[i]) {
                            path = fn_8006AFF8("movie/" + base + "_" + fn_8006BA38() + ".", fn_8006AC08(name));
                            break;
                        }
                    }
                }
                page->mVideo = new ((MemoryTag)6, "", "", 0) atn::VideoTHP(path.c_str());
                page->mMsg0 = atoi(csv.fn_8005B9CC());
                page->mMsg1 = atoi(csv.fn_8005B9CC());
                std::string ext("d2b");
                fn_8006AD24(name, ext);
                std::string layout = "mv_" + name;
                (page->mVideoLayout = fn_801F3FF8(this, layout.c_str(), 0))->fn_800D246C(1);
                page->mVideoLayout->mRepeat = 0;
            }
        }
    }
}

void TutorialTask::fn_805BBD7C() {
    HomeButtonMenu::sInstance->m88 = 1;
    mPages[mPage].mLayout->mFlags |= 4;
    if (mPages[mPage].mVideo != 0) {
        mPages[mPage].mVideo->vf24();
        if (mPages[mPage].mVideoLayout != 0) {
            mPages[mPage].mVideoLayout->mFlags |= 4;
        }
        if (lbl_8087F418 != 0) {
            lbl_8087F418->fn_803606CC(1, 8, 15);
            lbl_8087F418->fn_80360780(1, 8, 15);
        }
    }
    for (mPage++; mPage < 4; mPage++) {
        if (mPages[mPage].mEnabled != 0) {
            break;
        }
    }
    if (mPage < 4) {
        if (mPages[mPage].mMotion.mLoader.m08 != 0) {
            mState = 2;
            m84 = 0.0f;
            return;
        }
        mCamera = mCamera2;
        mPos = mPos2;
        mRot = mRot2;
        fn_805BA934();
        return;
    }
    mState = 4;
    for (unsigned int i = 0; i < 4; i++) {
        TutorialPage_unk* page = &mPages[i];
        if (page->mEnabled != 0) {
            if (mChara != 0) {
                mChara->mMotion.fn_80097A20(i + 0xEA);
            }
            page->mLayout->mFlags |= 4;
        }
    }
}

// NONMATCHING: not written yet (free position of the camera around the character)
void TutorialTask::fn_805BC1C0() {
    mPos2 = mPos;
    mRot2 = mRot;
}

TutorialPopup* TutorialPopup::fn_805BC6C8(atn::Task* parent, int id, CharaTask* chara) {
    return new ((MemoryTag)4, "", "", 0) TutorialPopup(parent, id, chara);
}

TutorialPopup::TutorialPopup(atn::Task* parent, int id, CharaTask* chara)
    : atn::Task(parent), mId(id), mData(fn_8021F09C(id)), mChara(chara), mTime(0.0f), mState(0), mLoading(1),
      mSpeed(0.65f) {
    mLine = 0;
    mShadow = 0;
    char path[0x100];
    if (fn_8006AC08(std::string(mData->mName)) == "d2b") {
        sprintf(path, "database/tutorial/tutorial_popup_temp.csv");
    } else {
        sprintf(path, "database/tutorial/%s", mData->mName);
    }
    mFile.vf0C(path);
    if (id != 0x8E) {
        (mShadow = fn_801F3FF8(this, "tips_shd.d2a", 0))->fn_800D246C(1);
    }
}

int TutorialPopup::vf0C() {
    if (mLoading != 0) {
        if (!mFile.fn_80473F50()) {
            mLoading = 0;
            fn_805BCF04();
            mFile.fn_80473F88();
        }
    }
    if (fn_800D3FA4() == 0) {
        return 0;
    }
    float ofs = -16.0f;
    if (lbl_8087F0A8 != 0) {
        ofs = lbl_8087F0A8->m484;
    }
    if (mId == 0xC6) {
        ofs = 0.0f;
    }
    for (unsigned int i = 0; i < 4; i++) {
        TutorialPopupLine_unk* line = &mLines[i];
        if (line->mEnabled != 0) {
            line->mLayout->mFlags |= 4;
            line->mLayout->fn_800D246C(0);
            line->mLayout->SetText_unk("comment", (int)mData->mText);
            fn_801FED24(&line->mLayout->mData.m10, fn_800DC6B4("basePoint"), 1, ofs);
        }
    }
    if (mShadow != 0) {
        mShadow->fn_800D246C(0);
        mShadow->mSpeed = 0.0f;
        mShadow->mRepeat = 0;
        mShadow->mFrame = 0.0f;
        fn_801FED24(&mShadow->mData.m10, fn_800DC6B4("basePoint"), 1, ofs);
        void* font = lbl_8087EEC8;
        float width = fn_8006EF48(font, fn_80116FC0(0, 0x895472), 1, 1, 15.0f, 0.0f);
        fn_801FED24(&mShadow->mData.m10, fn_800DC6B4("pos0011(1)"), 0, 445.0f - width);
        fn_801FED24(&mShadow->mData.m10, fn_800DC6B4("pos0002(1)"), 0, 404.0f - width);
    }
    for (unsigned int i = 0; i < 4; i++) {
        if (mLines[i].mEnabled != 0) {
            mLines[i].mLayout->mFrame = 0.0f;
            mLines[i].mLayout->mFlags |= 4;
        }
    }
    mLine = 0;
    if (mLines[0].mType == 2 && SetConfig::sInstance->mAttackType != 0) {
        mLine = 1;
    }
    mLines[mLine].mLayout->mFlags &= ~4;
    mTime = 0.0f;
    mState = 1;
    return 1;
}

// NONMATCHING: written from m2c
void TutorialPopup::vf10() {
    if (mState == 4) {
        return;
    }
    TutorialPopupLine_unk* line = &mLines[mLine];
    switch (mState) {
    case 1:
        mTime = 0.0f;
        mState = 2;
        break;
    case 2:
        mTime = mTime + mSpeed;
        if (atn::Keyboard::Order() != 0 && atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0xF1B1)) {
            mTime = line->mWait;
        }
        if (lbl_8087EF70 != 0 && lbl_8087F0A8->mInput.fn_801231D0(0x2B) != 0) {
            mTime = line->mWait;
        }
        if (mTime >= line->mWait) {
            mState = 3;
            line->mLayout->mSpeed = -1.0f;
        }
        break;
    case 3:
        if (line->mLayout->mFrame <= 1.0f) {
            fn_805BD298();
        }
        break;
    }
    if (mShadow != 0) {
        mShadow->mFrame = line->mLayout->mFrame;
    }
}

// NONMATCHING: written from m2c
void TutorialPopup::fn_805BCF04() {
    atn::Csv csv((const char*)mFile.fn_80470580(), mFile.fn_8047059C());
    while (csv.fn_8005BBF8() != 0) {
        unsigned int index = atoi(csv.fn_8005B9CC()) - 1;
        if (index <= 3) {
            TutorialPopupLine_unk* line = &mLines[index];
            line->mEnabled = 1;
            std::string name = fn_8006B46C(std::string(mData->mName));
            int temp = 0;
            if (fn_8006AC08(std::string(mData->mName)) == "d2b") {
                if (fn_8006AA20(name.c_str()) != 0) {
                    temp = 1;
                }
            }
            if (temp) {
                csv.fn_8005B9CC();
                line->mLayout = fn_801F3FF8(this, name.c_str(), 0);
            } else {
                line->mLayout = fn_801F3FF8(this, csv.fn_8005B9CC(), 0);
            }
            line->mLayout->fn_800D246C(1);
            line->mLayout->mRepeat = 0;
            line->mWait = fn_800DC288(csv.fn_8005B9CC());
            line->mType = atoi(csv.fn_8005B9CC());
        }
    }
}

void TutorialPopup::fn_805BD298() {
    mLines[mLine].mLayout->mFlags |= 4;
    for (mLine++; mLine < 4; mLine++) {
        if (mLines[mLine].mEnabled != 0) {
            break;
        }
    }
    if (mLine < 4) {
        TutorialPopupLine_unk* line = &mLines[mLine];
        if (line->mType != 2) {
            line->mLayout->mFlags &= ~4;
            mTime = 0.0f;
            mState = 1;
            return;
        }
    }
    mState = 4;
    TutorialPopupLine_unk* line = mLines;
    for (int i = 0; i < 4; i++, line++) {
        if (line->mEnabled != 0) {
            line->mLayout->mFlags |= 4;
        }
    }
}
