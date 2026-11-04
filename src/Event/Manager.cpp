// inferred: Event::PostSequence and Event::SequenceRoot share one TU (their RTTI names are
// packed together in .rodata). SequenceRoot is declared first: vtables are emitted in reverse
// declaration order.
#include "Event/SequenceRoot.h"
#include "Event/PostSequence.h"
#include "Event/Manager.h"
#include "Event/Sequence.h"
#include "atn/Light_unk.h"
#include "game/NewCampScene2_unk.h"
#include "game/GameConfig_unk.h"
#include "Event/Behavior.h"
#include "atn/RenderSetting_unk.h"
#include "game/GameField_unk.h"
#include "Event/Cut.h"
#include "Event/Section.h"
#include "Event/Channel.h"
#include "Event/Action.h"
#include "Event/Label.h"

extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings (frame speed at 0x3A4)
extern "C" GameConfig_unk* lbl_8087F0A8;     // inferred: game settings
extern "C" atn::Vector3 lbl_807C7030;        // inferred: zero vector
extern "C" atn::Vector2_unk lbl_807C7028;    // inferred: zero 2D vector
extern "C" void fn_8054E574();               // inferred: creates the action processors (Event/ActionActorBehavior.cpp)
extern "C" void fn_8054F380();               // inferred: deletes the action processors (Event/ActionActorBehavior.cpp)
extern "C" void fn_800D94F8(atn::Task* parent); // inferred: creates the screen capture fade
// inferred: items of the debug menu (atn/MindowsCommon.h)
extern "C" void fn_80087994(void* folder, const char* name, int* value, void* func, void* arg);
extern "C" void fn_800874C8(void* folder, const char* name, int* value, int min, int max, int step, void* func,
                            void* arg);
extern "C" void fn_8008771C(void* folder, const char* name, float* value, float min, float max, float step,
                            void* func, void* arg);

namespace Event {

// inferred: same body as in Event/Sequence.cpp (declared inline in Event/Sequence.h)
inline Cut* Sequence::FindCutByIndex_unk(int index) {
    for (int i = 0; i < (int)mB8.size(); i++) {
        if (index == mB8[i]->mIndex) {
            return mB8[i].get();
        }
    }
    return 0;
}

// inferred: target of a jump parameter (0x8000: start of the cut, otherwise a label); sets the cut
// or the frame of the label in the cut itself (inlined three times in fn_8047B768)
static inline void JumpTarget_unk(Sequence* seq, Cut* cut, ActionParam_unk* param, Cut*& result, int& frame) {
    int value = param->m04.i;
    if (value & 0x8000) {
        int index = value & ~0x8000;
        if (seq->FindCutByIndex_unk(index) != cut) {
            result = seq->FindCutByIndex_unk(index);
        }
    } else {
        Label* label = seq->fn_805412A8(value);
        if (label->mCut != cut) {
            result = label->mCut;
        } else {
            frame = label->mFrame;
        }
    }
}

// NONMATCHING: 99.3% - registers only (original: next cut r31, section r30, channel r29)
// inferred: cut that follows `cut`: the first jump of the jump channels (kind 8 of the sections of
// kind 1) to another cut, else the next cut. Names invented.
Cut* fn_8047B768(Sequence* seq, Cut* cut) {
    Cut* result = 0;
    Cut* next = seq->FindCutByIndex_unk(cut->fn_8053B028());
    int frame = 0;
    for (int i = 0; i < (int)cut->mSections.size(); i++) {
        Section* section = cut->GetSection_unk(i);
        if (section->m14 != 0 && section->m0C == 1) {
            for (int j = 0; j < (int)section->mChannels.size(); j++) {
                Channel* channel = section->GetChannel_unk(j);
                if (channel->m10 != 0 && channel->m0C == 8) {
                    for (int k = 0; k < (int)channel->mActions.size(); k++) {
                        Action* action = channel->GetAction_unk(k);
                        if (action->m10 >= frame) {
                            int type = action->GetParam_unk(0)->m04.i;
                            if (type == 0) {
                                JumpTarget_unk(seq, cut, action->GetParam_unk(1), result, frame);
                            } else if (type == 1) {
                                JumpTarget_unk(seq, cut, action->GetParam_unk(1), result, frame);
                            } else if (type == 2 || type == 3) {
                                JumpTarget_unk(seq, cut, action->GetParam_unk(5), result, frame);
                            }
                            if (result != 0) {
                                break;
                            }
                        }
                    }
                    if (result != 0) {
                        break;
                    }
                }
            }
            if (result != 0) {
                break;
            }
        }
    }
    if (result == 0 && next != cut) {
        result = next;
    }
    return result;
}

// inferred: one jump parameter of fn_8047BBBC (written out three times): a jump to another cut ends
// the cut at the action, a jump to a label of the cut itself skips the frames up to the label
#define JUMP_LENGTH_UNK(param)                                                                       \
    {                                                                                                \
        int value = (param)->m04.i;                                                                  \
        if (value & 0x8000) {                                                                        \
            int index = value & ~0x8000;                                                             \
            if (seq->FindCutByIndex_unk(index) != cut) {                                             \
                end = action->m10;                                                                   \
                done = 1;                                                                            \
                break;                                                                               \
            }                                                                                        \
        } else {                                                                                     \
            Label* label = seq->fn_805412A8(value);                                                  \
            if (label->mCut != cut) {                                                                \
                end = action->m10;                                                                   \
                done = 1;                                                                            \
                break;                                                                               \
            }                                                                                        \
            frame = label->mFrame;                                                                   \
            skipped += frame - action->m10;                                                          \
        }                                                                                            \
    }

// inferred: number of frames played in `cut` (its length minus the frames skipped by jumps and
// waits). Names invented.
int fn_8047BBBC(Sequence* seq, Cut* cut) {
    if (seq == 0 || cut == 0) {
        return 0;
    }
    seq->FindCutByIndex_unk(cut->fn_8053B028());
    int frame = 0;
    int end = cut->m2C;
    int skipped = 0;
    int done = 0;
    for (int i = 0; i < (int)cut->mSections.size(); i++) {
        Section* section = cut->GetSection_unk(i);
        if (section->m14 != 0 && section->m0C == 1) {
            for (int j = 0; j < (int)section->mChannels.size(); j++) {
                Channel* channel = section->GetChannel_unk(j);
                if (channel->m10 != 0 && (channel->m0C == 0x35 || channel->m0C == 8)) {
                    for (int k = 0; k < (int)channel->mActions.size(); k++) {
                        Action* action = channel->GetAction_unk(k);
                        if (action->m10 >= frame) {
                            if (action->m0C == 8) {
                                int type = action->GetParam_unk(0)->m04.i;
                                if (type == 0) {
                                    JUMP_LENGTH_UNK(action->GetParam_unk(1));
                                } else if (type == 1) {
                                    JUMP_LENGTH_UNK(action->GetParam_unk(1));
                                } else if (type == 2 || type == 3) {
                                    JUMP_LENGTH_UNK(action->GetParam_unk(5));
                                }
                            } else if (action->m0C == 0x35) {
                                if (action->m14 - action->m10 <= 0) {
                                    end = action->m10;
                                    done = 1;
                                    break;
                                }
                                skipped += action->m14 - action->m10;
                                frame = action->m14;
                            }
                        }
                    }
                    if (done) {
                        break;
                    }
                }
            }
            if (done) {
                break;
            }
        }
    }
    return end - skipped;
}
#undef JUMP_LENGTH_UNK

// NONMATCHING: 99.5% - registers only (original: section index r30, section count r12)
// inferred: first action of kind 0x2B (channels of the sections of kind 1) whose first parameter is
// 0, following the cuts from the current one. Names invented.
Action* fn_8047BFF4(Sequence* seq) {
    Cut* cut = seq != 0 ? seq->FindCutByIndex_unk(seq->m94) : 0;
    while (cut != 0) {
        int count = cut->mSections.size();
        for (int i = 0; i < count; i++) {
            Section* section = cut->GetSection_unk(i);
            if (section->m0C == 1) {
                int channels = section->mChannels.size();
                for (int j = 0; j < channels; j++) {
                    Channel* channel = section->GetChannel_unk(j);
                    if (channel->m0C == 0x2B) {
                        for (int k = 0; k < (int)channel->mActions.size(); k++) {
                            Action* action = channel->GetAction_unk(k);
                            if (action->GetParam_unk(0)->m04.i == 0) {
                                return action;
                            }
                        }
                    }
                }
            }
        }
        cut = fn_8047B768(seq, cut);
    }
    return 0;
}

} // namespace Event

extern "C" void C_QUATSlerp(const atn::Quaternion_unk* p, const atn::Quaternion_unk* q, atn::Quaternion_unk* r, float t);
extern "C" void fn_8072D210(float angle, float* s, float* c); // inferred: sine and cosine (256 units a turn)
extern "C" float fn_800F8524(const float* a, const float* b, float t); // inferred: lerp of two floats

// inferred: euler angles to a quaternion (inlined here; same code as
// atn::Quaternion_unk::fn_801498F0 of game/CharaTask.cpp)
static inline atn::Quaternion_unk EulerToQuat_unk(const atn::Vector3& rot) {
    atn::Quaternion_unk q;
    float rx = rot.x;
    float ry = rot.y;
    float rz = rot.z;
    float cx, cy, cz;
    float sx, sy, sz;
    fn_8072D210(40.743664f * (rx / 2.0f), &sx, &cx);
    fn_8072D210(40.743664f * (ry / 2.0f), &sy, &cy);
    fn_8072D210(40.743664f * (rz / 2.0f), &sz, &cz);
    float cycz = cy * cz;
    float sysz = sy * sz;
    q.w = cx * cycz + sx * sysz;
    q.x = sx * cycz - cx * sysz;
    q.y = cz * (cx * sy) + sz * (sx * cy);
    q.z = sz * (cx * cy) - cz * (sx * sy);
    return q;
}

// inferred: name invented (same as in game/TutorialTask.cpp)
static inline atn::Quaternion_unk Slerp_unk(const atn::Quaternion_unk& a, const atn::Quaternion_unk& b, float t) {
    atn::Quaternion_unk q;
    C_QUATSlerp(&a, &b, &q, t);
    return q;
}

// NONMATCHING: 97.1% - stack layout: the original copies the slerp result (0xC8) into the dead slot of
// qb (0xF0); here the copy gets a slot of its own (frame 0x270, not 0x260). Tried: no copy, RVO'd
// helper, copy construction, by-value helper, nested temporaries.
// inferred: direction between `a` and `b` (slerp of their rotations; names invented)
atn::Vector3 fn_8047C1D8(const atn::Vector3& a, const atn::Vector3& b, float t) {
    atn::Quaternion_unk qa = EulerToQuat_unk(a.ToRotation_unk());
    atn::Quaternion_unk qb = EulerToQuat_unk(b.ToRotation_unk());
    atn::Quaternion_unk r;
    r = Slerp_unk(qb, qa, t);
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXQuat(m, &r);
    PSMTXMultVec(m, &dir, &dir);
    return dir;
}

namespace atn {

// inferred: by-value operators of the color and a + (b - a) * t (names invented)
inline Color operator-(const Color& a, const Color& b) {
    return Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}
inline Color operator+(const Color& a, const Color& b) {
    return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}
inline Color operator*(const Color& a, float s) {
    return Color(a.r * s, a.g * s, a.b * s, a.a * s);
}
inline Vector4_unk operator-(const Vector4_unk& a, const Vector4_unk& b) {
    return Vector4_unk(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
inline Vector4_unk operator+(const Vector4_unk& a, const Vector4_unk& b) {
    return Vector4_unk(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
inline Vector4_unk operator*(const Vector4_unk& a, float s) {
    return Vector4_unk(a.x * s, a.y * s, a.z * s, a.w * s);
}
template <class T>
inline T Lerp_unk(const T& a, const T& b, float t) {
    return (b - a) * t + a;
}

} // namespace atn

inline ColorMatrixParam_unk* NewCampScene2_unk::fn_8047C7DC() {
    return mUseColorMatrix != 0 ? &mColorMatrix : &lbl_8087EFA8->m324;
}

namespace Event {

void SceneSetting_unk::fn_8047C88C(NewCampScene2_unk* scene) {
    *scene->fn_8047C770() = mFog;
    scene->GetAmbient_unk() = mAmbient;
    scene->GetLS_unk() = mLS;
    *scene->fn_8047C778() = m3C;
    *scene->fn_8047C780() = mGodRay;
    *scene->fn_8047C7D4() = mShadow;
    *scene->fn_8047C7DC() = mColorMatrix;
    *scene->fn_8047C848() = mHaze;
    scene->fn_8047CB6C(m148);
    *fn_800C2448(scene, 0) = mLight;
}

void SceneSetting_unk::fn_8047CB74(NewCampScene2_unk* scene, float t) {
    *scene->fn_8047C770() = mFog;
    scene->GetAmbient_unk() = atn::Lerp_unk(mAmbient, scene->GetAmbient_unk(), t);
    scene->GetLS_unk() = atn::Lerp_unk(mLS, scene->GetLS_unk(), t);
    *scene->fn_8047C778() = atn::Lerp_unk(m3C, *scene->fn_8047C778(), t);
    scene->fn_8047C780()->mColor = atn::Lerp_unk(mGodRay.mColor, scene->fn_8047C780()->mColor, t);
    scene->fn_8047C780()->mPos = atn::Lerp_unk(mGodRay.mPos, scene->fn_8047C780()->mPos, t);
    scene->fn_8047C780()->mRadialScale = fn_800F8524(&mGodRay.mRadialScale, &scene->fn_8047C780()->mRadialScale, t);
    *scene->fn_8047C7D4() = mShadow;
    for (unsigned int i = 0; i < 4; i++) {
        scene->fn_8047C7DC()->mRows[i] = atn::Lerp_unk(mColorMatrix.mRows[i], scene->fn_8047C7DC()->mRows[i], t);
    }
    *scene->fn_8047C848() = mHaze;
    scene->fn_8047CB6C(m148);
    fn_800C2448(scene, 0)->SetDir_unk(fn_8047C1D8(*mLight.GetDir_unk(), *fn_800C2448(scene, 0)->GetDir_unk(), t));
    fn_800C2448(scene, 0)->SetColor(atn::Lerp_unk(*mLight.GetColor_unk(), *fn_800C2448(scene, 0)->GetColor_unk(), t));
}

inline EventConfig_unk::EventConfig_unk()
    : m08(1), m0C(0), m10(3), m14(3), m18(1), m1C(1.0f), m20(1.25f), m24(1.25f), m28(0.0f), m2C(2.0f),
      m30(1.0f), m34(0.5f), m38(1.0f), m3C(0.5f), m40(atn::Color(1.0f, 1.0f, 1.0f, 1.0f)),
      m50(atn::Color(1.0f, 1.0f, 1.0f, 1.0f)), m60(atn::Color(1.0f, 1.0f, 1.0f, 1.0f)), m70(0), m74(0),
      m78(0), m7C(0), m80(0), m84(0), m88(1), m8C(0), m90(0), m94(1), m98(200.0f), m9C(500.0f), mA0(30.0f),
      mA4(10.0f), mA8(320), mAC(224), mB0(1.5f), mB4(1.5f), mB8(0), mBC(0), mC0(0), mC4(1.0f), mC8(0.0f),
      mCC(0), mD0(0), mD4(0.02f), mD8(0.02f), mDC(0), mE0(0), mE4(1.0f, 1.0f, 1.0f, 1.0f),
      mF4(0.0f, 0.0f, 0.0f), m100(0), m104(0), m108(1.0f), m10C(0.05f) {}

// NONMATCHING: 91% - float constant registers and the stack slots of the atn::Color temporaries
// inferred: constructor of the event manager (0x8047CF7C)
Manager::Manager(atn::Task* parent)
    : atn::Task(parent), m70(0), mB4(1.0f), mC4(0), m1A38(0), m1A3C(0), m1A40(0), m1A44(0), m1A48(0),
      m1A4C(1), m1A50(0x3E), m1A54(0), m1A58(8.0f), m1A6C(0), m1A84(0.0f), m1A88(0), m1A8C(5), m1A90(0),
      m1A94(0), m1A98(0), m1A9C(0), m1DC0(0), m1DC4(0), m1DC8(0), m1DCC(1.0f), m1DD0(0.0f), m1DD4(0),
      m1DD8(0), m1DDC(0.02f), m1DE0(0.02f), m1E74(0), m1E78(0), m1E88(0.0f),
      m1E8C(0.0f), m1E90(0), m1EA4(0), m1EA8(0), m1EAC(0), m1EB0(0), m1EB4(30.0f), m1F1C(0), m1F34(0),
      m1F58(0), m1F5C(0), m1F60(0), m1F64(2), m1F68(0.0f), m1F6C(0.0f), m1F70(0), m1F74(0), m1F78(0),
      m1F7C(0), m237C(0), m2380(0), m2384(0), m2388(1), m238C(0), m2390(0), m23B8(0), m23FC(0), m2400(0), m240C(0), m2410(0.5f), m2414(1.0f),
      m2418(1.0f), m2448(0) {
    fn_8054E574();
    fn_8010D98C();
    for (int i = 0; i < 16; i++) {
        m74[i] = 0;
    }
    m1AB0.m00 = 0;
    m1AB0.m01 = 0;
    m1DE4 = -1;
    m1DE8 = 0;
    m1DF8 = 0;
    m1DEC = 0;
    m1DF0 = 0;
    m1DF4 = 0;
    m1A7C = fn_801F3FF8(this, "/data/d2anime/ui_c_black_mask.d2a", 0);
    m1A7C->fn_800D246C(1);
    m1A80 = fn_801F3FF8(this, "/event2/d2anime/ui_event_start_flash.d2a", 0);
    m1A80->fn_800D246C(1);
    m1AA8 = atn::FadeColor::Create(this, 0, 0, 0);
    m1AA8->fn_800D246C(1);
    m1AA0 = 0;
    m1AA4 = 0;
    m1AAC = 0;
    SequenceRoot::Create_unk(this);
    PostSequence::Create_unk(this);
    fn_800D94F8(this);
    void* folder = m1A5C.Load("Event");
    fn_80087994(folder, "DebugCamera", &m1A40, 0, 0);
    fn_80087994(folder, "DebugInfoDisplay", &m1A44, 0, 0);
    fn_80087994(folder, "DrawViewEventSphere", &m1E90, 0, 0);
    fn_80087994(folder, "SimulateWave", &m2384, 0, 0);
    fn_80087994(folder, "FixEventDelay", &m2388, 0, 0);
    fn_800874C8(folder, "MAStartOffset", &m1F60, 0, 9999, 1, 0, 0);
    fn_800874C8(folder, "MAStartDelay", &m1F64, 0, 9999, 1, 0, 0);
    fn_800874C8(folder, "Draw2DMode", &m238C, 0, 5, 1, 0, 0);
    void* light = fn_8008937C(folder, "AutoLightFix");
    fn_800874C8(light, "Type", &m240C, 0, 3, 1, 0, 0);
    fn_8008771C(light, "Ratio", &m2410, 0.0f, 1.0f, 0.1f, 0, 0);
    fn_8008771C(light, "KeyIntensity", &m2414, 0.0f, 1.0f, 0.1f, 0, 0);
    fn_8008771C(light, "AmbientIntensity", &m2418, 0.0f, 1.0f, 0.1f, 0, 0);
    fn_8008771C(folder, "AcceptAnimeScale", &m5C, 0.0f, 10.0f, 0.1f, 0, 0);
    fn_8008771C(folder, "AcceptAnimeSpeed", &m60, 0.0f, 10.0f, 0.1f, 0, 0);
    fn_8008771C(folder, "FastForwardSpeed", &m1A58, 1.0f, 100.0f, 0.1f, 0, 0);
    m1EB8.fn_800C1814("/data/texture/gm044_000a.texture");
    m1EB8.m04 = atn::Vector2_unk(0.0f, 0.0f);
    m1EB8.mSize = atn::Vector2_unk(0.0f, 0.0f);
    m1D70.mRows[0] = RsVector4_unk(1.0f, 0.0f, 0.0f, 0.0f);
    m1D70.mRows[1] = RsVector4_unk(0.0f, 1.0f, 0.0f, 0.0f);
    m1D70.mRows[2] = RsVector4_unk(0.0f, 0.0f, 1.0f, 0.0f);
    m1D70.mRows[3] = RsVector4_unk(0.0f, 0.0f, 0.0f, 0.0f);
    m1EB8.m00 = 0;
    m1F0C = 1;
    m1D70.mEnabled = 0;
    mMoviePlaying = 0;
    m2404_1 = 0;
    m2404_2 = 0;
    m2404_3 = 0;
    m2404_4 = 0;
    m2404_5 = 0;
    m23AC = fn_801F3FF8(this, "/data/d2anime/ev_gradation.d2a", 0);
    m23AC->fn_800D246C(1);
    m23B0 = fn_801F3FF8(this, "/data/d2anime/ev_gradation.d2a", 0);
    m23B0->fn_800D246C(1);
    m48 = fn_801F3FF8(this, "/data/d2anime/ui_aspect_event.d2a", 0);
    m48->fn_800D246C(1);
    m4C.mCenter = atn::Vector3(lbl_807C7030);
    m5C = 5.0f;
    m60 = 2.0f;
    m241C = fn_801F3FF8(this, "/data/d2anime/ui_seek.d2a", 0);
    m241C->fn_800D246C(1);
    m2424 = lbl_807C7028;
    m242C = lbl_807C7028;
    m2434 = 0.0f;
    m2438 = fn_801F3FF8(this, "/data/d2anime/ui_seek_arow.d2a", 0);
    m2438->fn_800D246C(1);
    m243C = atn::Vector3(0.0f, 0.0f, 1.0f);
    m23B4 = 0;
    if (lbl_8087F0A8->mPreloadEventMA == 0) {
        m1F60 = 0;
    }
}

// NONMATCHING: 96.4% - the vectors at 0x64, 0x1A60, 0x1A70 and 0x1F10 have one more null test of
// their address in the original (one more base class; adding it to every vector<T*> loses 30
// functions elsewhere), and registers (original: this in r29, the index in r31)
Manager::~Manager() {
    lbl_8087F540 = 0;
    for (int i = 0; i < (int)m23A0.size(); i++) {
        fn_8048D3A4(i);
    }
    for (int i = 0; i < (int)m2394.size(); i++) {
        fn_8048DE54(i);
    }
    m23A0.clear();
    m2394.clear();
    fn_8054F380();
}

inline Sequence* Manager::FindSequence_unk(int id) {
    if (m70 != 0 && id == m70->m190) {
        return m70;
    }
    for (int i = 0; i < (int)m64.size(); i++) {
        if (id == m64[i]->m190) {
            return m64[i];
        }
    }
    return 0;
}

int Manager::fn_8047E964(int id, int queue) {
    // NONMATCHING: 97.6% - the original reads m23B8 again for the index (tried a ring struct with
    // its own push, unsigned casts, a pointer to the entry)
    if (queue != 0 && (unsigned int)m23B8 < 8) {
        QueuedSequence_unk* entry = &m23BC[(m23FC + m23B8) & 7];
        entry->mId = id;
        entry->m04 = 0;
        m23B8++;
    } else {
        Sequence* seq = FindSequence_unk(id);
        if (seq == 0) {
            return 0;
        }
        seq->fn_8053F6EC();
    }
    return 1;
}

int Manager::fn_8047F268(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->m194 >= 5 || seq->m194 == 0;
}

int Manager::fn_8047F2EC(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 0;
    }
    return seq->m194 == 10;
}

int Manager::fn_8047F364(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->fn_8047F400() || seq->m194 == 0;
}

// inferred: the sequence is ending (states 13 and 14)
int Sequence::fn_8047F400() {
    return m194 == 13 || m194 == 14;
}

int Manager::fn_8047F420(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->m50 == 1;
}

int Manager::fn_8047F498(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->m50 == 3;
}

int Manager::fn_8047F510(int id) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 1;
    }
    return seq->m98_8;
}

int Manager::fn_8047F6B0(int arg) {
    m1AA4 = arg;
    Step1AA0_unk();
    return m1AA0;
}

void Manager::fn_8047F6E4() {
    if (m1AAC == 0) {
        return;
    }
    Step1AA0_unk();
    if (m1AA0 == 3) {
        m1AAC = 0;
    }
}

void Manager::fn_8047F850(int a, int b, int c) {
    m1AA8->mDuration = a;
    m1AA8->mColor0 = b;
    m1AA8->mColor1 = c;
    m1AA8->mFrame = 0;
    m1AA8->mActive = 1;
    m1AA8->mDelay = 0;
}

void Manager::fn_8047F88C(int arg) {
    if (arg > 0) {
        m1AAC = 1;
        m1AA4 = arg;
    }
}

void Manager::fn_8047F8A4(float value) {
    m1AA8->mZ = value;
}

void Manager::fn_8047F8B0() {
    m1AA8->mActive = 0;
}

void Manager::fn_8047F8C0(int arg) {
    if (m1AA8 != 0) {
        m1AA8->fn_800D246C(arg);
    }
    if (m1A9C != 0) {
        m1A9C->fn_800D246C(arg);
    }
}

void Manager::fn_8047F91C() {
    Sequence* seq = m70;
    if (seq != 0) {
        seq->fn_80550CE0();
        fn_8047EFD8(seq->m190, 0);
        seq->fn_8053E8C8(14);
        fn_8048EC7C();
    }
}

void Manager::fn_8047F984(float time) {
    mB4 = time;
    lbl_8087EFA8->m3A4 = time;
}

GameMap_unk* Manager::fn_80480438() {
    if (m1A60.size() != 0) {
        return m1A60[0];
    }
    if (lbl_8087F430 != 0) {
        return fn_80373148(lbl_8087F430);
    }
    return 0;
}

void Manager::fn_804805B4() {
    m1A70.clear();
    m23B8 = 0;
    fn_8047F580();
}

int Manager::fn_80481654() {
    return m1A7C->IsFrozen_unk() == 0;
}

int Manager::fn_80481668() {
    int result = 0;
    if (!m1A7C->IsFlag_unk(4)) {
        if (m1A7C->mSpeed >= 0.0f) {
            result = 1;
        }
    }
    return result;
}

void Manager::fn_8048169C(int arg) {
    if (arg == 0) {
        if (m1A90 == 0) {
            m1A90 = 1;
            m1A8C = 5;
        }
    } else {
        m1A7C->mFrame = 0.0f;
        m1A7C->FreezeTask();
        m1A90 = 0;
        m1A8C = -1;
    }
}

void Manager::fn_804817D4() {
    if (!m1A80->IsFlag_unk(4)) {
        m1A80->FreezeTask();
        m1A88 = 1;
    }
}

void Manager::fn_80481800(int on) {
    m1EA4 = on;
    if (on != 0) {
        m1EB0 = 0;
    }
}

// inferred: removes the lights of the event
void Manager::fn_80484FBC() {
    int n = lbl_8087EEF8->mCount;
    for (int i = 0; i < n; i++) {
        lbl_8087EEF8->fn_8007A154(0);
    }
    m1F10.clear();
}

void Manager::fn_8048F184(int index) {
    if ((unsigned int)index > 1) {
        return;
    }
    if (index == 0) {
        if (!m23AC->IsFlag_unk(4)) {
            m23AC->FreezeTask();
        }
    } else {
        if (!m23B0->IsFlag_unk(4)) {
            m23B0->FreezeTask();
        }
    }
}

PostSequence::~PostSequence() {
    sInstance = 0;
}

void PostSequence::vf14() {}

int PostSequence::vf0C() { return 1; }

SequenceRoot::~SequenceRoot() {
    sInstance = 0;
}

void SequenceRoot::vf14() {}

void SequenceRoot::vf10() {}

int SequenceRoot::vf0C() { return 1; }

} // namespace Event
