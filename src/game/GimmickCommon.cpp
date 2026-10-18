// inferred: file boundaries from the per-file constants (.sdata2 0x80885F50-0x80885F88) and the
// gimmick factory jump tables (0x8078C6C8); the file also holds the factories at 0x803EC16C/0x803EC374.

#include "game/GimmickCommon.h"
#include "gmk/RainDrops.h"
#include "gmk/SkeltonsMirror.h"
#include "gmk/TodomeCountUp.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" unsigned long strlen(const char* s);
extern "C" char* strstr(const char* s, const char* find);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC6B4(const char* s);   // inferred: string hash
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" atn::Matrix3x4 lbl_807C7060;              // inferred: identity matrix

// inferred: camera (lbl_8087EFB4) and its projection to the screen
struct Camera_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void fn_800BFAC8(atn::Vector3* out, Camera_unk* camera, const atn::Vector3* pos);
// inferred: game settings (debug switches)
struct Settings_unk {
    unsigned char m00[0x74];
    int mDrawGimmickState; // 0x74
    unsigned char m78[0x7C - 0x78];
    int m7C;
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" int lbl_8087FA20;
extern "C" void* lbl_8087EEB0; // inferred: debug font
extern "C" void fn_800616C0(void* font, float x, float y, float z, float w, float h, float f6, const char* text,
                            int color, int arg6, int arg7, int arg8); // inferred: debug text
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))
#define __MIN_unk(a, b) ((a) < (b) ? (a) : (b))

// inferred: rotation matrix z * y * x (inlined twice in 0x803ECBD4). Written out instead of
// using __MtxRotate: the angle is read through the vector after the 0.0f constant is loaded,
// and the rotation matrix is above the product on the stack.
inline void __MakeRotation_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
}

// inferred: sets the translation column (the arguments are read z first)
inline void __SetTrans_unk(atn::Matrix3x4& m, float x, float y, float z) {
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
}

inline void __Vec3Set_unk(atn::Vector3& v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}

// inferred: largest component
inline float __MaxElem_unk(const atn::Vector3& v) {
    return __MAX_unk(v.x, __MAX_unk(v.y, v.z));
}

// inferred: float color (converted from/to GXColor)
struct ColorF_unk {
    ColorF_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};

void GimmickMaterialFade_unk::fn_803EBC44(atn::CsvTab* csv, Model_unk* model) {
    mMaterial = model->fn_80092954(csv->fn_8005B9CC());
    if (mMaterial != 0) {
        mStart = atoi(csv->fn_8005B9CC());
        mLength = atoi(csv->fn_8005B9CC());
        mFrom = ColorF_unk(mMaterial->mColor).a;
        mTo = fn_800DC288(csv->fn_8005B9CC());
        mMode = mMaterial->m4D;
    }
}

void GimmickMaterialFade_unk::fn_803EBD6C(Model_unk* model, const char* name, int start, int length, float to) {
    mMaterial = model->fn_80092954(name);
    if (mMaterial != 0) {
        mStart = start;
        mLength = length;
        mFrom = ColorF_unk(mMaterial->mColor).a;
        mTo = to;
        mMode = mMaterial->m4D;
    }
}

void GimmickMaterialFade_unk::fn_803EBE74(Model_unk* model) {
    if (mMaterial != 0) {
        float t;
        float frame = model->mFrame;
        if (mLength > 0) {
            t = (frame - mStart) / mLength;
        } else {
            t = (frame < mStart) ? 0.0f : 1.0f;
        }
        t = __MIN_unk(__MAX_unk(t, 0.0f), 1.0f);
        if (mReverse != 0) {
            t = 1.0f - t;
        }
        float a = mFrom + t * (mTo - mFrom);
        ColorF_unk color(mMaterial->mColor);
        color.a = a;
        mMaterial->mColor = color.ToGX();
        if (t > 0.0f) {
            fn_8007FAF0(mMaterial, 3);
            model->mFlags |= 0x10;
        } else {
            fn_8007FAF0(mMaterial, mMode);
            model->mFlags &= ~0x10;
        }
    }
}

void GimmickMotionSwitch_unk::fn_803EC0A4(atn::CsvTab* csv) {
    mHash = fn_800DC6B4(csv->fn_8005B9CC());
    mFrame = atoi(csv->fn_8005B9CC());
}

void GimmickMotionSwitch_unk::fn_803EC0F8(Model_unk* model) {
    if (mHash != 0) {
        float cur = model->mFrame;
        model->fn_8009385C(mHash, cur <= mFrame, 1, 0);
    }
}

// inferred: gimmick constructors called by the factories below (one per gimmick class)
extern "C" GimmickCommon* fn_803F1BB4(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_803F471C(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_803F69E4(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_803F744C(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_803FE390(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80400728(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80406754(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80408324(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_804093B4(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8040DC30(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8040EC30(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8040FAC0(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80412AFC(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80418198(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80418EAC(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8041C09C(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8041CD54(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8041DDE4(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8041FF08(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8042335C(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80423F64(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80426108(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8042B56C(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8042C21C(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8042CE30(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8042D628(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8042E36C(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_804310B0(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80432CF0(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80435CE4(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_80436E34(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8043A770(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_8043D594(atn::Task* parent, int id, int arg);
extern "C" GimmickCommon* fn_803F74D0(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_803F9420(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_803F9F34(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_803FBFA8(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_803FC8D8(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_803FE570(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80402978(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80405DD4(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_804067D8(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_8040D1D0(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_8040ED00(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80410A68(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80411440(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_8041537C(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_804166B0(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80416F74(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80418F30(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_8041B970(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_8041EB80(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80420ADC(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80421454(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80421AB8(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80422610(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80427EB4(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80428B0C(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80429668(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_8042F7EC(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_80435DEC(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_8043A850(atn::Task* parent, GimmickDesc_unk* desc);
extern "C" GimmickCommon* fn_8043B778(atn::Task* parent, GimmickDesc_unk* desc);

// inferred: creates the gimmick of type id / 1000 (ids below 1000 are types)
extern "C" GimmickCommon* fn_803EC16C(atn::Task* parent, int id, int arg) {
    GimmickCommon* gimmick = 0;
    int type = id / 1000;
    if (type == 0) {
        type = id;
        id *= 1000;
    }
    switch (type) {
    case 1: gimmick = fn_803F1BB4(parent, id, arg); break;
    case 2: gimmick = fn_803F471C(parent, id, arg); break;
    case 4: gimmick = fn_803F69E4(parent, id, arg); break;
    case 5: gimmick = fn_803F744C(parent, id, arg); break;
    case 10: gimmick = fn_803FE390(parent, id, arg); break;
    case 11: gimmick = fn_80400728(parent, id, arg); break;
    case 16: gimmick = fn_80406754(parent, id, arg); break;
    case 17: gimmick = fn_80408324(parent, id, arg); break;
    case 18: gimmick = fn_804093B4(parent, id, arg); break;
    case 19: gimmick = fn_803FE390(parent, id, arg); break;
    case 21: gimmick = fn_8040DC30(parent, id, arg); break;
    case 22: gimmick = fn_8040EC30(parent, id, arg); break;
    case 24: gimmick = fn_8040FAC0(parent, id, arg); break;
    case 28: gimmick = fn_80412AFC(parent, id, arg); break;
    case 33: gimmick = fn_80418198(parent, id, arg); break;
    case 34: gimmick = fn_80418EAC(parent, id, arg); break;
    case 36: gimmick = fn_8041C09C(parent, id, arg); break;
    case 37: gimmick = fn_8041CD54(parent, id, arg); break;
    case 38: gimmick = fn_8041DDE4(parent, id, arg); break;
    case 40: gimmick = fn_8041FF08(parent, id, arg); break;
    case 45: gimmick = fn_8042335C(parent, id, arg); break;
    case 46: gimmick = fn_80423F64(parent, id, arg); break;
    case 48: gimmick = fn_80426108(parent, id, arg); break;
    case 52: gimmick = gmk::RainDrops::fn_8042AFE4(parent, id, arg); break;
    case 53: gimmick = fn_8042B56C(parent, id, arg); break;
    case 54: gimmick = fn_8042C21C(parent, id, arg); break;
    case 55: gimmick = fn_8042CE30(parent, id, arg); break;
    case 56: gimmick = fn_8042D628(parent, id, arg); break;
    case 57: gimmick = fn_8042E36C(parent, id, arg); break;
    case 60: gimmick = fn_804310B0(parent, id, arg); break;
    case 61: gimmick = fn_80432CF0(parent, id, arg); break;
    case 63: gimmick = fn_80435CE4(parent, id, arg); break;
    case 64: gimmick = fn_80436E34(parent, id, arg); break;
    case 65: gimmick = fn_8043A770(parent, id, arg); break;
    case 67: gimmick = fn_8043D594(parent, id, arg); break;
    }
    return gimmick;
}

// inferred: same, from a placement description
extern "C" GimmickCommon* fn_803EC374(atn::Task* parent, GimmickDesc_unk* desc) {
    GimmickCommon* gimmick = 0;
    int type = desc->mId / 1000;
    if (type == 0) {
        type = desc->mId;
    }
    switch (type) {
    case 5: gimmick = fn_803F74D0(parent, desc); break;
    case 6: gimmick = fn_803F9420(parent, desc); break;
    case 7: gimmick = fn_803F9F34(parent, desc); break;
    case 8: gimmick = fn_803FBFA8(parent, desc); break;
    case 9: gimmick = fn_803FC8D8(parent, desc); break;
    case 10: gimmick = fn_803FE570(parent, desc); break;
    case 13: gimmick = fn_80402978(parent, desc); break;
    case 14: gimmick = fn_80405DD4(parent, desc); break;
    case 15: gimmick = fn_80405DD4(parent, desc); break;
    case 16: gimmick = fn_804067D8(parent, desc); break;
    case 20: gimmick = fn_8040D1D0(parent, desc); break;
    case 22: gimmick = fn_8040ED00(parent, desc); break;
    case 26: gimmick = fn_80410A68(parent, desc); break;
    case 27: gimmick = fn_80411440(parent, desc); break;
    case 29: gimmick = gmk::SkeltonsMirror::fn_80414AC4(parent, desc); break;
    case 30: gimmick = fn_8041537C(parent, desc); break;
    case 31: gimmick = fn_804166B0(parent, desc); break;
    case 32: gimmick = fn_80416F74(parent, desc); break;
    case 34: gimmick = fn_80418F30(parent, desc); break;
    case 35: gimmick = fn_8041B970(parent, desc); break;
    case 39: gimmick = fn_8041EB80(parent, desc); break;
    case 41: gimmick = fn_80420ADC(parent, desc); break;
    case 42: gimmick = fn_80421454(parent, desc); break;
    case 43: gimmick = fn_80421AB8(parent, desc); break;
    case 44: gimmick = fn_80422610(parent, desc); break;
    case 49: gimmick = fn_80427EB4(parent, desc); break;
    case 50: gimmick = fn_80428B0C(parent, desc); break;
    case 51: gimmick = fn_80429668(parent, desc); break;
    case 58: gimmick = gmk::TodomeCountUp::fn_8042F0B4(parent, desc); break;
    case 59: gimmick = fn_8042F7EC(parent, desc); break;
    case 63: gimmick = fn_80435DEC(parent, desc); break;
    case 65: gimmick = fn_8043A850(parent, desc); break;
    case 66: gimmick = fn_8043B778(parent, desc); break;
    }
    return gimmick;
}

GimmickCommon::GimmickCommon(atn::Task* parent, int id, int arg)
    : atn::Task(parent), mId(id), m4C(arg), m50(id / 1000), m54(0), m58(0), m5C(0), m68(0),
      m6C(0.0f, 0.0f, 0.0f), m78(0.0f, 0.0f, 0.0f), m84(0.0f, 0.0f, 0.0f), m90(1.0f, 1.0f, 1.0f),
      m9C(1), mA0(0), mA4(0), mA8(0), mAC(0), mE4(0), mE8(-1), mEC(-1) {
    if (mId < 1000) {
        m50 = mId;
        mId *= 1000;
    }
    if (lbl_8087F4A0 != 0) {
        lbl_8087F4A0->fn_803EE3F0(this);
    }
}

GimmickCommon::~GimmickCommon() {
    if (lbl_8087F4A0 != 0) {
        lbl_8087F4A0->fn_803EE41C(this);
    }
}

// inferred: the string pool has an extra NUL after each sprintf format of this unit (offsets
// 0x11, 0x92 and 0x9F of 0x80751DD0), so the formats are written with an explicit "\0"
void GimmickCommon::fn_803EC758(char* name) {
    sprintf(name, "gm%03d_%03d_%04d\0", mId / 1000, mId % 1000, m4C);
}

void GimmickCommon::fn_803EC7A0(Model_unk* model, atn::CsvTab* csv) {
    const char* field = csv->fn_8005B9CC();
    while (field != 0 && strcmp("", field) != 0) {
        if (strcmp(field, "REFLECT") == 0) {
            mA4 = 1;
        } else if (strcmp(field, "REFRACT") == 0) {
            model->mFlags |= 0x1800;
        } else if (strcmp(field, "SHADOW_CASTER") == 0) {
            model->mFlags |= 0x4;
        } else if (strcmp(field, "SHADOW_CASTER_OFF") == 0) {
            model->mFlags &= ~0x4;
        } else if (strcmp(field, "SHADOW_RECEIVER") == 0) {
            model->mFlags |= 0x8;
        } else if (strcmp(field, "SHADOW_RECEIVER_OFF") == 0) {
            model->mFlags &= ~0x8;
        } else if (strcmp(field, "CLOUDMAP") == 0) {
            model->mFlags |= 0x100000;
        }
        field = csv->fn_8005B9CC();
    }
    if (mA4 != 0) {
        model->mFlags |= 0x1400;
    }
}

int GimmickCommon::fn_803EC91C() {
    int busy = 0;
    switch (m68) {
    case 0: {
        char path[0x100];
        sprintf(path, "/database/gimmick/gm%03d_%03d.gmk\0", mId / 1000, mId % 1000);
        m68 = 1;
        mBinary.vf0C(path);
        busy = 1;
        break;
    }
    case 1:
        if (mBinary.fn_80473F50() != 0) {
            busy = 1;
        }
        if (busy == 0) {
            if (mBinary.fn_80473EFC() == 3) {
                vf20();
                mB0.fn_803EF780(mBinary.fn_80470580(), mBinary.fn_8047059C());
                switch (m50) {
                case 1:
                case 2:
                case 4:
                case 5:
                case 16:
                case 17:
                case 21:
                case 24:
                case 27:
                case 28:
                case 34:
                case 36:
                case 38:
                case 45:
                case 46:
                case 48:
                case 51:
                case 63:
                    mE4 = fn_803F1310(this);
                    break;
                }
                if (mE4 != 0) {
                    mE4->fn_800D246C(1);
                }
                m68 = 2;
            } else {
                m68 = 3;
            }
        }
    case 2:
        if (vf1C() != 0) {
            busy = 1;
        }
        if (mB0.fn_803F06B0() != 0) {
            busy = 1;
        }
        if (mE4 != 0 && (mE4->mFlags & 2) != 2) {
            busy = 1;
        }
        if (busy == 0) {
            vf24();
            m68 = 3;
            if (lbl_8087F4A0 != 0 && lbl_8087F0A8->m7C != 0 && lbl_8087FA20 == 0) {
                vf6C(lbl_8087F4A0->fn_803EEE44(m50));
            }
            if (mE4 != 0) {
                mE4->fn_800D246C(0);
            }
        }
        break;
    }
    return busy;
}

int GimmickCommon::vf1C() {
    return 0;
}

void GimmickCommon::vf2C() {
    mB0.fn_803F10AC(1);
    if (mE4 != 0) {
        mE4->fn_803F1A38(1);
    }
}

void GimmickCommon::vf20() {}

void GimmickCommon::vf24() {}

void GimmickCommon::fn_803ECBD4(Model_unk* model, int callback) {
    model->fn_80092A4C();
    atn::Matrix3x4 mtx = lbl_807C7060;
    atn::Matrix3x4 rot;
    __MakeRotation_unk(rot, m78);
    Mtx scale;
    PSMTXScale(scale, m90.x, m90.y, m90.z);
    PSMTXConcat(scale, mtx.m, mtx.m);
    PSMTXConcat(rot.m, mtx.m, mtx.m);
    atn::Matrix3x4 offset;
    __MakeRotation_unk(offset, m84);
    PSMTXConcat(offset.m, mtx.m, mtx.m);
    __SetTrans_unk(mtx, m6C.x, m6C.y, m6C.z);
    __MtxCopy(model->mMtx, mtx.m);
    atn::Vector3 ax;
    atn::Vector3 ay;
    atn::Vector3 az;
    __Vec3Set_unk(az, mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);
    float sz = PSVECMag(&az);
    __Vec3Set_unk(ay, mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);
    float sy = PSVECMag(&ay);
    __Vec3Set_unk(ax, mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);
    float sx = PSVECMag(&ax);
    atn::Vector3 s(sx, sy, sz);
    model->m54 = __MAX_unk(s.x, __MAX_unk(s.y, s.z));
    if (callback != 0) {
        fn_800902C0(model, 0, boost::function<void()>());
    }
}

void GimmickCommon::fn_803ED0D4(Model_unk* model, int callback) {
    model->fn_80092A4C();
    atn::Matrix3x4 mtx = lbl_807C7060;
    atn::Matrix3x4 rot;
    __MakeRotation_unk(rot, m78);
    Mtx scale;
    PSMTXScale(scale, m90.x, m90.y, m90.z);
    PSMTXConcat(scale, mtx.m, mtx.m);
    PSMTXConcat(rot.m, mtx.m, mtx.m);
    atn::Matrix3x4 offset;
    __MakeRotation_unk(offset, m84);
    PSMTXConcat(offset.m, mtx.m, mtx.m);
    __SetTrans_unk(mtx, m6C.x, m6C.y, m6C.z);
    __MtxCopy(model->mMtx, mtx.m);
    atn::Vector3 ax;
    atn::Vector3 ay;
    atn::Vector3 az;
    __Vec3Set_unk(az, mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);
    float sz = PSVECMag(&az);
    __Vec3Set_unk(ay, mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);
    float sy = PSVECMag(&ay);
    __Vec3Set_unk(ax, mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);
    float sx = PSVECMag(&ax);
    atn::Vector3 s(sx, sy, sz);
    model->m54 = __MAX_unk(s.x, __MAX_unk(s.y, s.z));
    if (callback != 0) {
        fn_8000D430(model, boost::function<void()>());
    }
}

void GimmickCommon::fn_803ED5D0(Model_unk* model) {
    mB0.fn_803F0764(this);
    model->fn_80097E80(1);
}

void GimmickCommon::fn_803ED610(Model_unk* model) {
    if (m9C != 0) {
        model->fn_8008CD60();
        if (lbl_8087F0A8->mDrawGimmickState != 0) {
            atn::Vector3 d = lbl_8087EFB4->mPos - m6C;
            if (fn_805F9920(&d) < 90000.0f) {
                atn::Vector3 screen;
                fn_800BFAC8(&screen, lbl_8087EFB4, &m6C);
                if (0.0f < screen.z && screen.z < 1.0f) {
                    char name[0x20];
                    char text[0x20];
                    fn_803EC758(name);
                    sprintf(text, "%s state:%d\0", name, m54);
                    fn_800616C0(lbl_8087EEB0, screen.x, screen.y - 12.0f, 0.0f, 12.0f, 12.0f, 0.0f, text, -1, 1, 1,
                                0);
                }
            }
        }
    }
}

void GimmickCommon::fn_803ED774(Model_unk* model) {
    if (m9C != 0) {
        model->fn_8008CD60();
        if (lbl_8087F0A8->mDrawGimmickState != 0) {
            atn::Vector3 screen;
            atn::Vector3 d = lbl_8087EFB4->mPos - m6C;
            if (fn_805F9920(&d) < 90000.0f) {
                fn_800BFAC8(&screen, lbl_8087EFB4, &m6C);
                if (0.0f < screen.z && screen.z < 1.0f) {
                    char text[0x20];
                    char name[0x20];
                    fn_803EC758(name);
                    sprintf(text, "%s state:%d\0", name, m54);
                    fn_800616C0(lbl_8087EEB0, screen.x, screen.y - 12.0f, 0.0f, 12.0f, 12.0f, 0.0f, text, -1, 1, 1,
                                0);
                }
            }
        }
    }
}

void GimmickCommon::fn_803ED8D8(GimmickCommon* gimmick) {
    GimmickCommon* p = this;
    while (p->m5C != 0) {
        p = p->m5C;
    }
    p->m5C = gimmick;
}

void GimmickCommon::fn_803ED8F4(GimmickCommon* gimmick) {
    GimmickCommon* p = this;
    while (p->m5C != 0 && p->m5C != gimmick) {
        p = p->m5C;
    }
    if (p->m5C != 0) {
        p->m5C = gimmick->m5C;
        gimmick->m5C = 0;
    }
}

void GimmickCommon::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80088724(group, "rotOffset", &m84, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    fn_800874C8(group, "locData", &m58, 0, 99, 1, 0, 0);
    mB0.fn_803F11F8(group);
}

void GimmickCommon::vf70(int state) {
    m54 = state;
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

void GimmickCommon::fn_803EDB0C(GimmickUvOwner_unk* owner, GimmickUvTable_unk* table) {
    mA8 = owner;
    mAC = table;
}

static inline int __IsReady_unk(GimmickUvOwner_unk* owner) {
    int ready = 0;
    if (owner->m28 != 0 || (owner->fn_800D59B8() == 0 && owner->mLoader.fn_80473EFC() == 3)) {
        ready = 1;
    }
    return ready;
}

void GimmickCommon::fn_803EDB18(Model_unk* model) {
    if (mA8 != 0) {
        if (__IsReady_unk(mA8) == 0 || mAC == 0 || model->fn_8008B130() == 0) {
            return;
        }
        {
            for (int i = 0; i < mAC->mCount; i++) {
                GimmickUvEntry_unk* e = &mAC->mEntries[i];
                if (e->mName != 0 && strlen(e->mName) != 0) {
                    atn::Csv csv(e->mName, strlen(e->mName));
                    if (strstr(model->fn_8008B130(), csv.fn_8005B9CC()) != 0) {
                        atn::Vector2_unk uv(-1.0f * e->m04 / e->m0C, 1.0f - (e->m08 + e->m0C) / e->m0C);
                        model->fn_800954DC(mA8, uv, e->m0C);
                        return;
                    }
                }
            }
        }
    }
}

void GimmickCommon::fn_803EDCF4() {
    atn::Vector3 screen;
    atn::Vector3 d = lbl_8087EFB4->mPos - m6C;
    if (fn_805F9920(&d) < 90000.0f) {
        fn_800BFAC8(&screen, lbl_8087EFB4, &m6C);
        if (0.0f < screen.z && screen.z < 1.0f) {
            char text[0x20];
            char name[0x20];
            fn_803EC758(name);
            sprintf(text, "%s state:%d\0", name, m54);
            fn_800616C0(lbl_8087EEB0, screen.x, screen.y - 12.0f, 0.0f, 12.0f, 12.0f, 0.0f, text, -1, 1, 1, 0);
        }
    }
}

int GimmickCommon::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    if (vf60() != 0 && flag == 0) {
        atn::Vector3 d = m6C - *pos;
        if (fn_805F9920(&d) > 400.0f) {
            return 0;
        }
        PSVECNormalize(&d, &d);
        return PSVECDotProduct(&d, dir) > 0.5f;
    }
    return 0;
}
