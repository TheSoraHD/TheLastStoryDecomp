#include "Event/DialogHead.h"
#include "game/GameConfig_unk.h"
#include "game/EquipItemData.h"
#include "atn/LipSync_unk.h"
#include "atn/DrawHelper.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "macros.h"
#include <boost/function.hpp>
#include "std/string_members.h"

// Everything below is inferred from Event::DialogHead (0x802251F8..0x802280B4); names are invented.

extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: graphics device (lbl_8087EEE0, atn/GraphicsDevice.cpp)
struct GraphicsDevice_unk;
extern "C" GraphicsDevice_unk* lbl_8087EEE0;
extern "C" float fn_80076FF8(GraphicsDevice_unk* device); // inferred: screen aspect ratio
extern "C" void fn_8007642C(GraphicsDevice_unk* device, int a, int b);
extern "C" void fn_80071D68(GraphicsDevice_unk* device, const float projection[4][4]);
extern "C" void fn_800760BC(GraphicsDevice_unk* device, int left, int top, int width, int height); // inferred: viewport
extern "C" void fn_800760D0(GraphicsDevice_unk* device); // inferred: restores the viewport
extern "C" atn::RenderView_unk* lbl_8087EFB4;
extern "C" void fn_800BDB58(atn::RenderView_unk* view, atn::IRender* render, float order, int pass); // inferred: queues a render
extern "C" void fn_80094BD8(void* model, int mode);
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" std::string fn_800DC980(const char* format, ...); // inferred: formatted string
extern "C" int fn_802180A8(const char* name);                // inferred: motion id from its name
extern "C" int fn_8006AA20(const char* path);                // inferred: nonzero if the file exists
extern "C" int rand();
extern "C" char* strstr(const char* s, const char* find);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" float PSVECMag(const atn::Vector3* v);

// inferred: frames per second of the fade in (the only .sdata value of the file)
float lbl_8087DBB0 = 30.0f;

// inferred: camera owned by a model (as in game/CharaTask.cpp)
struct ModelCamera_unk {
    ModelCamera_unk() : m00(0) {}
    int m00;
    void* m04;
    Camera_unk mCamera; // 0x08
};
// inferred: inlined (its literals are @STRING@ objects in .sdata, shared with game/CharaTask.cpp)
inline void CreateCamera_unk(atn::Model* model) {
    model->m208.reset(new ((MemoryTag)6, "", "", 0) ModelCamera_unk());
}

// inferred: length of the column `i` of a matrix (as in game/CostumeEditChara.cpp)
inline float __ColumnLength_unk(const atn::Matrix3x4& m, int i) {
    atn::Vector3 v(m.m[0][i], m.m[1][i], m.m[2][i]);
    return PSVECMag(&v);
}

// inferred: largest scale axis of a matrix (as in game/CostumeEditChara.cpp)
inline float __MaxScaleT_unk(const atn::Matrix3x4& m) {
    float sz = __ColumnLength_unk(m, 2);
    float sy = __ColumnLength_unk(m, 1);
    float sx = __ColumnLength_unk(m, 0);
    atn::Vector3 s(sx, sy, sz);
    return MAX(s.x, MAX(s.y, s.z));
}

// inferred: item `i` of a character, 0 past the end
inline EquipItemData* GetItem_unk(CharaTask* chara, int i) {
    return i < (int)chara->m6A4.mCount ? chara->m6A4.mData[i] : 0;
}

namespace Event {

inline DialogSide_unk* DialogHead::GetSide_unk() {
    return m1620 ? m1620 : &lbl_8087F0A8->mDialog.mLeft;
}

DialogHead* DialogHead::fn_802251F8(atn::Task* parent, CharaTask* chara) {
    if (parent != 0) {
        return new ((MemoryTag)4, "", "", 0) DialogHead(parent, chara);
    }
    return 0;
}

DialogHead::DialogHead(atn::Task* parent, CharaTask* chara)
    : CharaTask(parent, chara->mType == 5 ? 5 : 1, chara->GetData_unk()), m1428(chara), m142C(0, 0), m1620(0),
      m1820(0.0f), m1824(-1), m1834(0.0f, 0.0f, 0.0f), m1A10(0), m1A14(0) {
    mFlagB23 = 1;
    if (chara->mType == 5) {
        fn_80136544(chara->m84.fn_80473F18());
    }
    m142C.mPos.Set_unk(0.0f, 0.0f, 40.0f);
    m142C.fn_8004B378();
    m1624.m00 = 1;
    m1624.m04 = &lbl_8087EEB0->m70;
    memset(m1840, 0, sizeof(m1840));
    mMotion.m1DC = (struct Lens_unk*)&lbl_8087F0A8->mDialog.mScenes[0][0];
}

// NONMATCHING: 98.0% - the copy of the camera into m1624 takes other address registers (same
// blocker as the camera copies of game/CosEditBase.cpp and game/CharaTask.cpp fn_80144F4C)
void DialogHead::fn_802254B0(float top, float bottom) {
    Camera_unk camera(0, 1);
    camera.mUp.Set_unk(0.0f, 1.0f, 0.0f);
    camera.mPos.Set_unk(0.0f, 0.5f * (bottom + top), -100.0f);
    camera.mTarget.Set_unk(0.0f, 0.5f * (bottom + top), 0.0f);
    camera.mTop = top - bottom;
    camera.mBottom = 0.0f;
    camera.mLeft = -10.0f;
    camera.mRight = 10.0f;
    camera.fn_8004B378();
    m1624.SetCamera_unk(camera);
    fn_80095F10(&mMotion, &m1624);
    for (unsigned int i = 0; i < 8; i++) {
        if (m680[i] != 0 && m680[i]->m000 < 4) {
            fn_80095F10(m680[i]->m024, &m1624);
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        if (m6A4.mData[i]->mFlags.mFlag) {
            fn_80095F10(&m6A4.mData[i]->mModel, &m1624);
        }
    }
}

// NONMATCHING: 99.9% - copy of the head position into m1828: source and target address registers
// swapped (r3/r4, known blocker of the Vector3 copy from a temporary)
int DialogHead::vf0C() {
    int loading = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 2; k++) {
                if (m1840[i][j][k] && m1840[i][j][k]->fn_8011EF14()) {
                    loading = 1;
                }
            }
        }
    }
    if (fn_8013655C() == 0 && !loading) {
        CreateCamera_unk(&mMotion);
        mMotion.fn_80097C08(0, 0xEE, 1, 0, 1, 1.0f, 0.1f);
        mMotion.mTracks[0].m0C = lbl_8087F0A8->mDialog.mUseWaitMotion ? 1.0f : 0.0f;
        mMotion.fn_80097E80(1);
        mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
        for (unsigned int i = 0; i < m6A4.mCount; i++) {
            m6A4.mData[i]->mFlags.mFlag = GetItem_unk(m1428, i)->mFlags.mFlag;
        }
        fn_80094BD8(&mMotion, 4);
        chr::CharaEquip_unk** p = m680;
        for (unsigned int i = 0; i < 8; i++, p++) {
            if (*p != 0 && (*p)->m000 < 4) {
                fn_80094BD8((*p)->m024, 4);
            }
        }
        for (unsigned int i = 0; i < m6A4.mCount; i++) {
            if (m6A4.mData[i]->mFlags.mFlag) {
                fn_80094BD8(&m6A4.mData[i]->mModel, 4);
            }
        }
        m1824 = mMotion.GetNodeNoByName("head", 0);
        m1828 = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(m1824))->GetTrans_unk();
        m1834.y = MIN(1.6f, 0.3f * -(m1828.y - 15.771023f));
        fn_80225C80(0);
        m1A10 = rand() % 90;
        m1188.m7C = 0.5f;
        return 1;
    }
    return 0;
}

void DialogHead::fn_80225C80(int right) {
    DialogConfig_unk* cfg = &lbl_8087F0A8->mDialog;
    int type = fn_80219558(m1428->m50);
    m1620 = right ? &cfg->mRight : &cfg->mLeft;
    if (strstr(mMotion.fn_8008B130(), "pc107_") != 0) {
        m1620 = right ? &cfg->m070 : &cfg->mLeft;
    } else if (strstr(mMotion.fn_8008B130(), "pc011_") != 0) {
        m1834.x = 0.3f;
        m1834.z = 0.0f;
    } else {
        if (type < 0 || type == 11) {
            m1834.x = right ? 0.9f : -0.9f;
            m1834.z = 1.1f;
        } else if (type == 3) {
            m1834.x = right ? 0.7f : -0.7f;
            m1834.z = 0.7f;
        } else if (type == 5) {
            m1834.x = right ? 0.3f : -0.3f;
            m1834.z = 0.4f;
        }
    }
}

// NONMATCHING: 99.1% - the original keeps a dead `lwz m524; cmpwi` before fn_80076FF8 (not
// reproduced) and swaps r3/r4 in the copy of the camera target
void DialogHead::vf10() {
    mMotion.mTracks[0].m0C = lbl_8087F0A8->mDialog.mUseWaitMotion ? 1.0f : 0.0f;
    DialogSide_unk* side = GetSide_unk();
    atn::Matrix3x4 trans;
    PSMTXTrans(trans.m, side->mPos.x, side->mPos.y, side->mPos.z);
    __MtxCopy(mMotion.mMtx, trans.m);
    mMotion.m54 = __MaxScaleT_unk(trans);
    for (int i = 0; i < mMotion.m16C->m44; i++) {
        mMotion.m220[i].m00 = 0;
    }
    m1154.fn_8007A7BC(&mMotion);
    if (--m1A10 <= 0) {
        m1188.StartBlink(1.0f, 1.0f, 0.0f, 3.0f, 4.0f);
        m1A10 = rand() % 90 + 30;
    }
    mMotion.fn_80097E80(1);
    mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
    fn_80148B38(1.0f);
    m142C.mAspect = fn_80076FF8(lbl_8087EEE0);
    m142C.mFovy = GetSide_unk()->mCamAngleOfView;
    m142C.mTarget = m1828 + m1834 + GetSide_unk()->mCamTarget;
    m142C.mPos = m1828 + m1834 + GetSide_unk()->mCamPos;
    m142C.fn_8004B378();
    m1820 = MAX(m1820, 0.01f);
    float h = 50.0f * m1820;
    float y = GetSide_unk()->mPos.y;
    fn_802254B0(y + h, y);
    m1820 += 1.0f / lbl_8087DBB0;
    int mode;
    if (m1820 >= 0.9f) {
        mode = 1;
    } else {
        mode = 4;
    }
    fn_80094BD8(&mMotion, mode);
    chr::CharaEquip_unk** p = m680;
    for (unsigned int i = 0; i < 8; i++, p++) {
        if (*p != 0 && (*p)->m000 < 4) {
            fn_80094BD8((*p)->m024, mode);
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        if (m6A4.mData[i]->mFlags.mFlag) {
            fn_80094BD8(&m6A4.mData[i]->mModel, mode);
        }
    }
}

void DialogHead::vf14() {
    fn_800BDB58(lbl_8087EFB4, this, 10.0f, 2);
    fn_800BDB58(lbl_8087EFB4, this, 10.0f, 0x10);
}

// NONMATCHING: 98.2% - instruction order of the camera copies (copy constructor of `saved`) and the
// registers of the equipment loop
void DialogHead::vf18(atn::RenderView_unk* view) {
    Camera_unk saved(view->mCamera);
    view->SetCamera_unk(m142C);
    GetModel_unk()->vf18(view);
    chr::CharaEquip_unk** p = m680;
    for (unsigned int i = 0; i < 8; i++, p++) {
        if (*p != 0 && (m6A0 & (1U << i)) == (1U << i) && (*p)->m000 < 4) {
            (*p)->GetModel_unk()->vf18(view);
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        if (m6A4.mData[i]->mFlags.mFlag) {
            m6A4.mData[i]->mModel.vf18(view);
        }
    }
    view->SetCamera_unk(saved);
}

void DialogHead::fn_80226B28(int dark, int right) {
    mMotion.m1DC = (struct Lens_unk*)&lbl_8087F0A8->mDialog.mScenes[dark][right];
}

// NONMATCHING: 99.4% - registers of the four inlined GetSide_unk calls of the viewport and of the
// camera copies
void DialogHead::vf10(atn::RenderView_unk* view) {
    m142C.fn_8004B378();
    fn_8007642C(lbl_8087EEE0, 0, 0);
    Camera_unk saved(view->mCamera);
    view->SetCamera_unk(m142C);
    fn_80071D68(lbl_8087EEE0, m142C.mProj.m);
    fn_800760BC(lbl_8087EEE0, (int)GetSide_unk()->mVPos.x, (int)GetSide_unk()->mVPos.y,
                (int)GetSide_unk()->mVSize.x, (int)GetSide_unk()->mVSize.y);
    GetModel_unk()->vf10(view);
    chr::CharaEquip_unk** p = m680;
    for (unsigned int i = 0; i < 8; i++, p++) {
        if (*p != 0 && (m6A0 & (1U << i)) == (1U << i) && (*p)->m000 < 4) {
            (*p)->GetModel_unk()->vf10(view);
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        if (m6A4.mData[i]->mFlags.mFlag) {
            m6A4.mData[i]->mModel.vf10(view);
        }
    }
    fn_800760D0(lbl_8087EEE0);
    view->SetCamera_unk(saved);
    fn_80071D68(lbl_8087EEE0, saved.mProj.m);
}

// NONMATCHING: 99.2% - stack layout: the original places the string and its char temporaries below the
// ptmf temporaries (tried a nested block, kept; an inline function gets the stack right but not the
// registers) and the registers of the inlined SimpleVector copy
void DialogHead::fn_80227434(int kind, int arg, int right) {
    float weight = lbl_8087F0A8->mDialog.mUseWaitMotion ? 0.0f : 1.0f;
    if (m1840[kind][arg][right]) {
        m1188.SetMotionParts(m1840[kind][arg][right]->mKeys, true, true, 1.0f, weight);
        CharaFaceControl* face = &m1188;
        face->m44[face->m5C] = m1840[kind][arg][right]->mEdits;
    } else {
        int motion;
        {
            std::string name("CH_FACE_");
            switch (kind) {
            case 0:
                name += "DFN";
                break;
            case 1:
                name += "ANG";
                break;
            case 2:
                name += "SAD";
                break;
            case 3:
                name += "SML";
                break;
            }
            name += fn_800DC980("%02d", arg);
            motion = fn_802180A8(name.c_str());
        }
        if (motion < 0 || mMotion.fn_80097D40(motion) == 0) {
            motion = 0xB4;
        }
        if (m1428->m50 == 1 && motion == 0xA3) {
            motion = 0xA6;
        }
        m1188.SetMotion(motion, true, true, true, 1.0f, weight);
    }
    fn_80225C80(right);
}

// NONMATCHING: 99.7% - registers of the inlined std::string(const char*) of the model name (r24-r26
// rotated)
int DialogHead::fn_80227A94(char* path, int kind, int arg, int right) {
    std::string name("");
    if (m1428 != 0 && m1428->GetModel_unk()->m16C != 0) {
        name = std::string(fn_8006B174(m1428->GetModel_unk()->fn_8008B130()), 0, 5);
    }
    const char* k = "";
    switch (kind) {
    case 0:
        k = "dfn";
        break;
    case 1:
        k = "ang";
        break;
    case 2:
        k = "sad";
        break;
    case 3:
        k = "sml";
        break;
    }
    const char* s = right ? "r" : "l";
    sprintf(path, "/database/chara/face/ce_%s_%s%02d_%s.face", name.c_str(), k, arg, s);
    return fn_8006AA20(path);
}

// inferred: instantiated right after its first use, as in atn/FileUtil_unk: its "basic_string:
// out_of_range" follows the strings of fn_80227A94 in the pool (the linker drops the copy)
template std::string::basic_string(const std::string& str, std::string::size_type pos, std::string::size_type n);

void DialogHead::fn_80227D2C(int kind, int arg) {
    if (!m1840[kind][arg][0] && !m1840[kind][arg][1]) {
        char path[0x100];
        if (fn_80227A94(path, kind, arg, 0)) {
            m1840[kind][arg][0].reset(new ((MemoryTag)4, "", "", 0) FaceData_unk);
            m1840[kind][arg][0]->fn_8011EEA0(path);
        }
        if (fn_80227A94(path, kind, arg, 1)) {
            m1840[kind][arg][1].reset(new ((MemoryTag)4, "", "", 0) FaceData_unk);
            m1840[kind][arg][1]->fn_8011EEA0(path);
        }
    }
}

void DialogHead::fn_80227F78(LipSyncData_unk* data) {
    if (lbl_8087F0A8->mDialog.mUseLipSync) {
        m1154.m00 = data;
        m1154.m04 = 0.0f;
    }
}

} // namespace Event
