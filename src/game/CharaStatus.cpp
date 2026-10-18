#include "game/CharaStatus.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/ResHandle.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
// inferred: debug menu entries; the last two arguments are a callback and its argument
typedef void (*MenuCallback_unk)(void* arg);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step,
                            MenuCallback_unk callback, void* arg);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, MenuCallback_unk callback,
                            void* arg, float min, float max, float step);
extern "C" void fn_8008771C(void* group, const char* name, float* value, MenuCallback_unk callback, void* arg,
                            float min, float max, float step);

// inferred: field objects that own the route tables
struct RouteField_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
};
struct RouteField2_unk {
    unsigned char m00[0x1A6C];
    RouteTable_unk* m1A6C;
};
extern "C" RouteField_unk* lbl_8087F430;
extern "C" RouteField2_unk* lbl_8087F540;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: route tables of the current field (name invented)
inline RouteTable_unk* GetRouteTable_unk() {
    if (lbl_8087F430 != 0) {
        return lbl_8087F430->m10D8;
    }
    if (lbl_8087F540 != 0) {
        return lbl_8087F540->m1A6C;
    }
    return 0;
}

RouteWalker_unk::RouteWalker_unk() {
    mLink = 0;
    m2C = 1;
    mA0 = 0;
    mA4 = 0;
    m70 = 0.0f;
    m74 = 0.0f;
    m78 = 0.0f;
    m7C = 0.0f;
    m80 = 0.0f;
    m84 = 1.0f;
    m88 = 0.0f;
    m8C = 0.0f;
    fn_8012539C();
    m2C = 1;
}

void RouteWalker_unk::fn_8012539C() {
    if (m2C == 0) {
        RouteTable_unk* table = GetRouteTable_unk();
        if (table != 0 && mLink > 0) {
            RouteLink_unk* link = &table->mLinks[mLink - 1];
            link->m3C = 0;
        }
    }
    mMode = -1;
    m48 = 0;
    m90 = 0;
    m64 = lbl_807C7030;
    m98 = 0;
    m94 = 1;
    mLink = 0;
    mPointB = 0;
    mPointA = 0;
    m00 = 0;
    m10 = lbl_807C7030;
    m1C = 0;
    m20 = 0;
    m24 = 0;
    m28 = 0.0f;
    m2C = 0;
}

int RouteWalker_unk::fn_80125474() {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0) {
        return 0;
    }
    if ((mPointA > 0 && mPointA <= table->mNumPoints && table->mPoints[mPointA - 1].m2C == 0) ||
        (mPointB > 0 && mPointB <= table->mNumPoints && table->mPoints[mPointB - 1].m2C == 0) ||
        (mLink > 0 && mLink <= table->mNumLinks && table->mLinks[mLink - 1].m38 == 0)) {
        return 0;
    }
    if (mPointB <= 0) {
        return 0;
    }
    if (mMode == 0 && mPointA <= 0) {
        return 0;
    }
    if (mMode == 1 && mLink <= 0) {
        return 0;
    }
    if ((mMode == 4 || mMode == 6 || mMode == 7) && m24 == 0) {
        return 0;
    }
    return 1;
}

CharaLook_unk::CharaLook_unk()
    : mChara(0), mType(0), mTarget(0), mOffset(0.0f, 0.0f, 0.0f), m18(0.0f), m1C(0.0f), m20(0.0f),
      m24(0.62831855f), m28(0.7853982f), m2C(0.0f), mNeck(0), m34(0), m35(0), m36(1), mSpeed(0.3f), mEyeType(0),
      mEyeTarget(0), mEyeOffset(0.0f, 0.0f, 0.0f), mEyeSpeed(0.3f) {
    m3C = 0.0f;
    m40 = 0.0f;
    m44 = 0.0f;
    m48 = 0.0f;
    m60 = 0.0f;
    m64 = 0.0f;
    m68 = 0.0f;
    m6C = 0.0f;
    m70 = 0.0f;
    m74 = 0.0f;
}

CharaLook_unk::~CharaLook_unk() {}

void CharaLook_unk::fn_801298EC(chr::Chara_unk* chara) {
    mChara = chara;
    mNeck = ((Model_unk*)&chara->mMotion)->fn_80092814("neck", 0);
}

void CharaLook_unk::fn_80129930(const atn::Vector3& pos, float speed) {
    mOffset = pos;
    mType = 2;
    mSpeed = speed;
}

void CharaLook_unk::fn_80129954(const atn::Vector3& pos, float speed) {
    mOffset = pos;
    mType = 3;
    mSpeed = speed;
}

void CharaLook_unk::fn_80129978(Model_unk* model, const char* node, const atn::Vector3& offset, float speed) {
    if (node != 0) {
        mTarget = (float*)model->GetNodeMtx_unk(model->fn_80092814(node, 0));
        mOffset = offset;
        mType = 1;
        mSpeed = speed;
    } else {
        mTarget = (float*)model->mMtx;
        mOffset = offset;
        mType = 1;
        mSpeed = speed;
    }
}

void CharaLook_unk::fn_80129A48(float speed) {
    mTarget = 0;
    mOffset.x = 0.0f;
    mOffset.y = 0.0f;
    mOffset.z = 0.0f;
    mType = 0;
    mSpeed = speed;
}

void CharaLook_unk::fn_8012ABD0(const atn::Vector3& pos, float speed) {
    mEyeOffset = pos;
    mEyeType = 3;
    mSpeed = speed;
}

void CharaLook_unk::fn_8012ABF4(const atn::Vector3& pos, float speed) {
    mEyeOffset = pos;
    mEyeType = 2;
    mEyeSpeed = speed;
}

void CharaLook_unk::fn_8012AC18(float* target, const atn::Vector3& pos, float speed) {
    mEyeTarget = target;
    mEyeOffset = pos;
    mEyeType = 1;
    mEyeSpeed = speed;
}

void CharaLook_unk::fn_8012AC40(float speed) {
    mEyeTarget = 0;
    mEyeOffset.x = 0.0f;
    mEyeOffset.y = 0.0f;
    mEyeOffset.z = 0.0f;
    mEyeType = 0;
    mEyeSpeed = speed;
}

// inferred: debug menu callback, applies the new values at once
void fn_8012AC64(CharaLook_unk* look) {
    float speed = look->mSpeed;
    float eyeSpeed = look->mEyeSpeed;
    look->mSpeed = 1.0f;
    look->mEyeSpeed = 1.0f;
    look->fn_80129A6C();
    fn_8000D430((Model_unk*)&look->mChara->mMotion, boost::function<void()>());
    look->mSpeed = speed;
    look->mEyeSpeed = eyeSpeed;
}

void CharaLook_unk::fn_8012AD20(void* menu) {
    void* group = fn_8008937C(menu, "Sight");
    fn_800874C8(group, "type", &mType, 0, 3, 1, (MenuCallback_unk)fn_8012AC64, this);
    fn_80087E9C(group, "offset", &mOffset, (MenuCallback_unk)fn_8012AC64, this, -100000.0f, 100000.0f, 1.0f);
    fn_8008771C(group, "speed", &mSpeed, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_800874C8(group, "eyeType", &mEyeType, 0, 3, 1, (MenuCallback_unk)fn_8012AC64, this);
    fn_80087E9C(group, "eyeOffset", &mEyeOffset, (MenuCallback_unk)fn_8012AC64, this, -100000.0f, 100000.0f, 1.0f);
    fn_8008771C(group, "eyeSpeed", &mEyeSpeed, 0, 0, 0.0f, 1.0f, 0.1f);
}

CharaStatusBase::CharaStatusBase() {
    memset(m1A8, 0, sizeof(m1A8));
    memset(this, 0, sizeof(*this));
}

CharaStatus::~CharaStatus() {}
