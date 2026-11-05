// inferred: Event::PostSequence and Event::SequenceRoot share one TU (their RTTI names are
// packed together in .rodata). SequenceRoot is declared first: vtables are emitted in reverse
// declaration order.
#include "Event/SequenceRoot.h"
#include "Event/PostSequence.h"
#include "Event/Manager.h"
#include "atn/DrawHelper.h"
#include "macros.h"
#include "Event/Sequence.h"
#include "atn/Light_unk.h"
#include "game/NewCampScene2_unk.h"
#include "game/GameConfig_unk.h"
#include "Event/Behavior.h"
#include "db/EventIdTable_unk.h"
#include "atn/EffectManager.h"
#include "game/SkillEffectManager.h"
#include "game/GameVoice.h"
#include "atn/SoundCore.h"
#include "atn/SoundManager.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "atn/RenderSetting_unk.h"
#include "game/GameField_unk.h"
#include "game/CharaTask.h"
#include "game/GameScene.h"
#include "game/GimmickManager.h"
#include "game/MapManager.h"
#include "game/ColliAttrManager.h"
#include "game/GameBgm.h"
#include "game/EnvSeManager.h"
#include "game/PlayerManager.h"
#include "game/Inventory.h"
#include "game/NpcManager.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"
#include "game/NpcTask.h"
#include "game/FriendManager.h"
#include "atn/FileList.h"
#include "Event/Cut.h"
#include "Event/Section.h"
#include "Event/Channel.h"
#include "Event/Action.h"
#include "Event/StringData.h"
#include "Event/Label.h"

extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings (frame speed at 0x3A4)

// inferred: zoom of the damage effect at the start and at the end of the event flash (fn_80481A20)
float lbl_8087E088 = 0.05f;
float lbl_8087E08C = -0.6f;
extern "C" GameConfig_unk* lbl_8087F0A8;     // inferred: game settings
extern "C" atn::Vector3 lbl_807C7030;        // inferred: zero vector
extern "C" atn::Vector2_unk lbl_807C7028;    // inferred: zero 2D vector
extern "C" void fn_8054E574();               // inferred: creates the action processors (Event/ActionActorBehavior.cpp)
extern "C" void fn_8054F380();               // inferred: deletes the action processors (Event/ActionActorBehavior.cpp)
extern "C" void fn_800D94F8(atn::Task* parent); // inferred: creates the screen capture fade
extern "C" void fn_800D2338(atn::Task* task);   // inferred: removes the task
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" std::string fn_800DC980(const char* format, ...); // inferred: formatted string
// inferred: area data of the maps (db; m68: light settings of the area)
struct AreaData_unk {
    unsigned char m00[0x68];
    void* m68;
};
extern "C" AreaData_unk* fn_8021771C(int area, int map, int part);
extern "C" void fn_8049D52C(char* name, int area, int map, int part); // inferred: name of the scene file
extern "C" char* fn_8006BA8C(const char* path, int* size, int arg);   // inferred: loads a file
extern "C" int fn_8006AA20(const char* path);                        // inferred: the file exists
extern "C" GameMap_unk* fn_8049D68C(atn::Task* parent, const char* path); // inferred: creates a map
extern "C" GameMap_unk* fn_8049D704(atn::Task* parent, int area, int map, int part);
extern "C" void fn_800C3094(void* light);
struct DialogTextMgr_unk;
extern "C" DialogTextMgr_unk* lbl_8087F008;                     // inferred: text renderer (Event/Dialog.cpp)
extern "C" void fn_800DAA3C(DialogTextMgr_unk* mgr, int arg);
extern "C" NewCampScene2_unk* fn_800C16B4(Camera_unk* camera); // inferred: scene of the render view
extern "C" void fn_8004895C(atn::SoundEngine_unk* sound);
extern "C" void fn_80049654(atn::SoundEngine_unk* sound);
extern "C" void fn_8004829C(atn::SoundEngine_unk* sound, const char** groups, int count);
extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" void* fn_8008B964(); // inferred: returns lbl_8087EFB4 (camera)
extern "C" Camera_unk* fn_800F7258(void* camera); // inferred: camera of the game
extern "C" atn::Vector3 fn_80383728(Camera_unk* camera); // inferred: position looked at by the camera
extern "C" atn::Vector2_unk fn_800F84D8(const atn::Vector2_unk& a, const atn::Vector2_unk& b, float t);
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" float fn_802A7964(float rad);
extern "C" void* fn_802F0990();         // inferred: returns lbl_8087EFA8
extern "C" float fn_802F0988(void* rs); // inferred: frame time (+0x3A4)
extern "C" void* lbl_8087EF70;          // inferred: pad
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
// inferred: weak copies, as in Event/Path.cpp and game/GameUI.cpp
inline void* GetPad_unk() {
    return lbl_8087EF70;
}
inline atn::SoundManager* GetSoundMgr_unk() {
    return lbl_8087EFE8;
}
inline void atn::D2A_Task::SetLayoutValue_unk(const char* name, float value) {
    fn_801FECE0(&mData.m10, fn_800DC6B4(name), value);
}
inline void atn::D2A_Task::SetLayoutValue_unk(const char* name, int index, float value) {
    fn_801FED24(&mData.m10, fn_800DC6B4(name), index, value);
}
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
// inferred: inverse of a matrix (weak copy 0x80487BD0); name invented
inline atn::Matrix3x4 Inverse_unk(const atn::Matrix3x4& m) {
    atn::Matrix3x4 r(m);
    PSMTXInverse(r.m, r.m);
    return r;
}
// inferred: sets the layout value `name` (as atn::D2A_Task::SetLayoutValue_unk of game/GameUI.cpp, with
// the layout taken first, as in Event/ActionEffectPlay.cpp)
inline void SetLayoutValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
extern "C" void* lbl_8087F048;              // inferred: bullet manager
extern "C" void fn_80370B78(GameField_unk* field, unsigned int color, int frames, int arg, float speed); // inferred: fade of the field
extern "C" void fn_801011B8(void* bullets); // inferred: removes every bullet
extern "C" atn::SoundEngine_unk* lbl_8087EE90; // inferred: sound engine
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
    : m88(1), m8C(0), m90(0), m94(1), m98(200.0f), m9C(500.0f), mA0(30.0f), mA4(10.0f), mA8(320), mAC(224),
      mB0(1.5f), mB4(1.5f) {}

// NONMATCHING: 91% - float constant registers and the stack slots of the atn::Color temporaries
// inferred: constructor of the event manager (0x8047CF7C)
Manager::Manager(atn::Task* parent)
    : atn::Task(parent), m70(0), mB4(1.0f), mC4(0), m1A38(0), m1A3C(0), m1A40(0), m1A44(0), m1A48(0),
      m1A4C(1), m1A50(0x3E), m1A54(0), m1A58(8.0f), m1A6C(0), m1A84(0.0f), m1A88(0), m1A8C(5), m1A90(0),
      m1A94(0), m1A98(0), m1A9C(0), m1DC0(),
      m1E74(0), m1E78(0), m1E88(0.0f),
      m1E8C(0.0f), m1E90(0), m1EA4(0), m1EA8(0), m1EAC(0), m1EB0(0), m1EB4(30.0f), m1F1C(0), m1F34(0),
      m1F58(0), m1F5C(0), m1F60(0), m1F64(2), m1F68(0.0f), m1F6C(0.0f), m1F70(0), m1F74(0), m1F78(0),
      m1F7C(0), m237C(0), m2380(0), m2384(0), m2388(1), m238C(0), m2390(0), m2400(0), m240C(0), m2410(0.5f), m2414(1.0f),
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

// NONMATCHING: 97.6% - registers only (original: this in r29, the index in r31)
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

Manager* Manager::fn_8047E4BC(atn::Task* parent) {
    if (lbl_8087F540 == 0) {
        lbl_8087F540 = new ((MemoryTag)4, "", "", 0) Manager(parent);
    }
    return lbl_8087F540;
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

// inferred: removes every sequence
void Manager::fn_8047E528() {
    for (Sequence** it = m64.begin(); it != m64.end(); ++it) {
        fn_800D2338(*it);
    }
    m64.clear();
    for (int i = 0; i < 16; i++) {
        m74[i] = 0;
    }
    m70 = 0;
}


// inferred: adds a sequence that is not there yet. Nothing calls it, so the linker drops it, but the
// original has it here: its weak copy of Sequence::GetId_unk (0x8047E5D4) is emitted before
// fn_8047E5DC and its "vector length error" is in the string pool before "event2/%s.event". Body
// invented from those two traces.
void Manager::AddSequence_unk(Sequence* seq) {
    if (FindSequence_unk(seq->GetId_unk()) == 0) {
        m64.push_back(seq);
    }
}

// NONMATCHING: 99.2% - registers of the growth path of push_back (size/capacity loaded into r5/r4)
// inferred: loads the sequence `id` ("event2/<name>.event") unless it is already there
int Manager::fn_8047E5DC(int id) {
    if (FindSequence_unk(id) == 0) {
        EventIdEntry_unk* entry = EventIdTable_unk::fn_8020F130()->fn_8020F71C(id);
        if (entry == 0) {
            return 0;
        }
        char path[0x100];
        sprintf(path, "event2/%s.event", entry->mName.c_str());
        Sequence* seq = Sequence::fn_8053E86C(SequenceRoot::sInstance);
        m64.push_back(seq);
        seq->m190 = id;
        seq->fn_80576030(path, 0);
    }
    return 1;
}

int Manager::fn_8047E964(int id, int queue) {
    if (queue != 0 && m23B8.size() < 8) {
        QueuedSequence_unk& entry = m23B8.Next_unk();
        entry.mId = id;
        entry.m04 = 0;
        m23B8.mCount++;
    } else {
        Sequence* seq = FindSequence_unk(id);
        if (seq == 0) {
            return 0;
        }
        seq->fn_8053F6EC();
    }
    return 1;
}

// NONMATCHING: 98.8% - the original reloads the ring count in every test of the erase loop
// inferred: starts the sequence `id` (the cut list and the queue are reset for the in-game kinds 1
// and 3); names invented
int Manager::fn_8047EA34(int id, int arg, int arg2) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 0;
    }
    if (seq->m50 == 1 || seq->m50 == 3) {
        if (m70 != 0) {
            return 0;
        }
        mB8.clear();
        int first = 1;
        mB8.push_back(first);
        m70 = seq;
        m1A38 = 0;
        m1A3C = 0;
        m1EA4 = 0;
        m1EB0 = 0;
        m1EA8 = 0;
        if (seq->m50 == 1) {
            if (lbl_8087F048 != 0) {
                fn_801011B8(lbl_8087F048);
            }
            if (lbl_8087F9E8 != 0) {
                lbl_8087F9E8->fn_8059C32C(0);
            }
            lbl_8087F3C0->fn_8023A60C(0);
            lbl_8087F3C0->fn_8023A60C(3);
            lbl_8087F3C0->fn_8023A60C(5);
        }
        Sequence* cur = FindSequence_unk(id);
        if (cur != 0 && cur->m98_14 != 1 && (cur->m50 == 1 || cur->m50 == 3)) {
            if (lbl_8087F0A8->mPreloadEventMA != 0) {
                m1F38.mStr[0] = 0;
                m1F5C = 0;
                fn_8048871C(cur, 0, 0, 0);
            }
            lbl_8087F540->fn_80488E00(cur);
            Manager* mgr = lbl_8087F540;
            if (lbl_8087F0A8->mPreloadEventVoice != 0 && lbl_8087EE90 != 0 && lbl_8087EE90->m04 != 0 &&
                (mgr->m70 == 0 || mgr->m70->m190 != 0x151E)) {
                if (lbl_8087F498 != 0) {
                    lbl_8087F498->fn_803EB4A8(0, 0);
                }
                mgr->m2378 = -1;
                if (mgr->m1F74 != 0) {
                    mgr->fn_80489A50(mgr->m1F78, 1);
                }
                mgr->m237C = 1;
            }
            cur->m98_14 = 1;
        }
        for (int i = 0; i < (int)seq->mB8.size(); i++) {
            Cut* cut = seq->IsCutIndex_unk(i) ? seq->mB8[i].get() : 0;
            if (seq->m98_8 == 1) {
                cut->mSkip = 0;
            } else if ((int)seq->m144.size() > 0) {
                if (cut->mSkip != 0) {
                    cut->mSkip = 2;
                }
            } else if ((unsigned int)(cut->mSkip - 1) <= 1) {
                cut->mSkip = 3;
            }
            if ((id == 0x2391 || id == 0x23F3) && cut->mSkip != 0) {
                cut->mSkip = 0;
            }
        }
        if (m1A90 != 0) {
            m1A90 = 0;
        }
    } else {
        for (int i = 0; i < 16; i++) {
            if (m74[i] == 0) {
                m74[i] = seq;
                break;
            }
        }
    }
    seq->fn_80541244(seq->fn_80541198(seq->FindCutByIndex_unk(seq->m94)));
    seq->m1AC = arg2;
    seq->fn_8054F9DC();
    for (Ring_unk<QueuedSequence_unk, 8>::iterator it = m23B8.begin(); it != m23B8.end(); ++it) {
        if (id == it->mId) {
            m23B8.erase(it);
            break;
        }
    }
    return 1;
}

// inferred: stops the music of the event (inlined twice; name invented)
inline void Manager::StopMusic_unk(Sequence* seq) {
    if (m1E78 != 0 || m1F1C != 0) {
        if (m1F1C & 0x200000) {
            fn_80488534(seq, 60);
        } else {
            fn_80488534(seq, 15);
        }
    }
    m1F20.fn_8004B1EC(60);
    m1F20.mSound.Reset();
}

// NONMATCHING: 99.7% - registers of the m74 loop (index and pointer swapped)
// inferred: stops the sequence `id`
int Manager::fn_8047EFD8(int id, int arg) {
    Sequence* seq = FindSequence_unk(id);
    if (seq == 0) {
        return 0;
    }
    if (seq->m50 == 1 || seq->m50 == 3) {
        m70 = 0;
        mB8.clear();
        if (m70 == 0) {
            if (arg == 0) {
                StopMusic_unk(seq);
            }
            m2378 = -1;
            if (lbl_8087F498 != 0) {
                lbl_8087F498->fn_803EB4A8(0, 1);
            }
            m237C = 0;
            m1F74 = 0;
            m2378 = -1;
        }
    } else {
        for (int i = 0; i < 16; i++) {
            if (m74[i] == seq) {
                m74[i] = 0;
                break;
            }
        }
    }
    if (arg != 0) {
        Sequence* other = FindSequence_unk(seq->m190);
        if (other != 0 && (other->m50 == 1 || other->m50 == 3)) {
            m2380 = seq->m190;
        }
    }
    seq->fn_8054FB10(arg);
    seq->vf10();
    seq->fn_80540664(arg);
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

// inferred: stops the sequence kept in m2380 (fn_8047EFD8)
// NONMATCHING: 98.4% - the original loads m2380 again for the search
__declspec(noinline) int Manager::fn_8047F580() {
    if (m2380 > 0) {
        Sequence* seq = FindSequence_unk(m2380);
        if (seq != 0) {
            StopMusic_unk(seq);
            seq->fn_8054FBC8();
            seq->fn_805406A0();
            if (seq == m70) {
                m70 = 0;
            }
        }
        m2380 = 0;
        return 1;
    }
    return 0;
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

// inferred: steps the screen capture fade (states of m1AA0: 1 capture, 3 start, 4 wait for the end)
void Manager::fn_8047F730() {
    atn::FadeCapture* fade = lbl_8087F430 != 0 ? lbl_8087F430->m5624 : m1A9C;
    if (m1AA0 == 1) {
        fade->mWait = 0;
        fade->mDelay = 0;
        fade->mColor0 = 0xFFFFFFFF;
        fade->mColor1 = 0xFFFFFF;
        fade->m78 = 0x12;
        fade->mDuration = m1AA4;
        fade->mStep = 1;
        fade->Capture();
        m1AA0 = 2;
    } else if (m1AA0 == 3) {
        fade->MeltTask();
        fade->mFrame = 0;
        fade->mZ = -100.0f;
        fade->mActive = 1;
        m1AA0 = 4;
    } else if (m1AA0 == 4) {
        float frame = mB4;
        fade->mStep = (int)(0.2f + frame);
        if (fade->IsEnd()) {
            fade->mStep = 1;
            fade->mActive = 0;
            m1AA0 = 0;
        }
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


// inferred: adds a cut change (cut `cut`; `now` applies it at once). Event 0x19D (the ending) with
// m194 of the game settings set fades the field out instead. Names invented.
void Manager::fn_8047F994(int cut, int now) {
    if (cut == 0 && lbl_8087F0A8->m194 != 0 && m70 != 0 && m70->m190 == 0x19D) {
        fn_80370B78(lbl_8087F430, 0xFFFFFFFF, 0, 0, 0.0f);
        m70->fn_8053E8C8(11);
    } else {
        mB8.push_back(cut);
        if (now != 0) {
            fn_8047FCC0();
        }
    }
}

// inferred: stops a sound (callback of fn_8047FAD0)
extern "C" void fn_8047FABC(atn::SoundCore& sound, int frames) {
    sound.mHandle.Stop(frames);
}

// NONMATCHING: 97.7% - float registers and the stack slot of the motion blur temporary
// inferred: end of an event: motion blur reset, normal speed, music and sounds stopped
__declspec(noinline) void Manager::fn_8047FAD0() {
    MotionBlurParam_unk blur;
    blur.mEnabled = 1;
    lbl_8087EFA8->m240 = blur;
    fn_8047F984(m1A58);
    mC4 = 1;
    if (strlen(m1F38.c_str()) != 0) {
        m1F20.fn_8004B1EC(30);
        m1F20.mSound.Reset();
        m1F34 = 0;
    }
    lbl_8087EFE8->fn_800D0DB0(boost::bind(&fn_8047FABC, _1, 10), 2);
    if (lbl_8087F498 != 0) {
        lbl_8087F498->fn_803EB4A8(1, 0);
    }
}

// NONMATCHING: 97.7% - registers (command and sequence in r27/r29 swapped)
// inferred: applies the pending cut changes of mB8 (0: next cut, 1: end of the fast forward, 2: skip
// the sequence); names invented
void Manager::fn_8047FCC0() {
    while (mB8.size() != 0) {
        int cmd = *mB8.begin();
        mB8.erase(mB8.begin());
        if (cmd == 2) {
            fn_8047F91C();
        } else if (cmd == 0) {
            Sequence* seq = m70;
            if (seq != 0 && seq->m50 == 1 && seq->m194 == 10) {
                switch (seq->fn_80541210()->mSkip) {
                case 1: {
                    Sequence* cur = m70;
                    if (cur != 0) {
                        Cut* cut = cur->fn_80541210();
                        if (cut != 0) {
                            cur->fn_80541534(cur->fn_80541198(cut), cut->m2C);
                        }
                    }
                    break;
                }
                case 2:
                    fn_8047F91C();
                    break;
                case 3:
                    fn_8047FAD0();
                    break;
                }
                lbl_8087EFE8->fn_800CF680(3, 10, seq->fn_80541210()->mSkip == 3);
            }
        } else if (cmd == 1) {
            if (mC4 != 0) {
                lbl_8087EFA8->m240 = m1AB0.mB8;
                fn_8047F984(1.0f);
                mC4 = 0;
                Sequence* seq = m70;
                if (strlen(m1F38.c_str()) != 0) {
                    if (seq == 0 || seq->m190 != 0x151B || seq->fn_80541210() == 0 ||
                        (seq->fn_80541210()->mIndex != 7 && seq->fn_80541210()->mIndex != 8)) {
                        fn_80488B5C(60);
                    }
                }
            }
        }
    }
}

int Manager::fn_804803A4() {
    if (m1DE4 >= 0) {
        return 0;
    }
    for (int i = 0; i < (int)m1A60.size(); i++) {
        if (!m1A60[i]->IsFlag_unk(2)) {
            return 0;
        }
    }
    for (int i = 0; i < (int)m1A70.size(); i++) {
        if (!m1A70[i]->IsFlag_unk(2)) {
            return 0;
        }
    }
    return 1;
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

// inferred: the sequence changes the map: the characters (and, for another map, the maps and the
// scene) of the event are removed
void Manager::fn_80480468(Sequence* seq, unsigned int flags) {
    int same = 0;
    int sameMap = 0;
    int area = m1DEC;
    if (area == seq->GetState_unk().m02) {
        if (m1DF0 == seq->GetState_unk().m00) {
            sameMap = 1;
        }
    }
    if (sameMap) {
        if (m1DF4 == seq->GetState_unk().m03) {
            same = 1;
        }
    }
    if (same && area != 1) {
        flags |= 0x10000000;
    }
    if (flags != 0) {
        for (int i = 0; i < (int)m1A70.size(); i++) {
            fn_800D2338(m1A70[i]);
        }
        m1A70.clear();
        if (!(flags & 0x10000000)) {
            if (m1A6C != 0) {
                fn_800D2338(m1A6C);
                m1A6C = 0;
            }
            for (int i = 0; i < (int)m1A60.size(); i++) {
                fn_800D2338(m1A60[i]);
            }
            m1A60.clear();
        }
    }
    m1DE4 = 2;
    m1DE8 = seq;
    m1DF8 = flags;
}

void Manager::fn_804805B4() {
    m1A70.clear();
    m23B8.clear();
    fn_8047F580();
}

// inferred: path of the map file of the area `area`, map `map` and part `part`
std::string fn_804805CC(int area, int map, int part) {
    return fn_800DC980("map\\tw%02d\\tw%02d_%02d_%02d.map", area, area, map, part);
}

// NONMATCHING: 96.2% - registers (map loop keeps base+offset, push_back of m1A60, string temps)
// inferred: loads what the sequence needs (flags: 0x10 scene, 0x100 maps, 0x200 keep the maps, 0x2000
// party, 0x1000 npcs/enemies/friends, 0x4000 only the characters the sequence uses, 0x10000000 same
// map). Names invented.
void Manager::fn_804805F0(Sequence* seq, unsigned int flags) {
    int area = seq->GetState_unk().m02;
    int map = seq->GetState_unk().m00;
    int part = seq->GetState_unk().m03;
    AreaData_unk* areaData = fn_8021771C(area, map, part);
    if ((flags & 0x10) && !(flags & 0x10000000) && areaData != 0) {
        char path[0x100];
        char name[0x100];
        fn_8049D52C(name, area, map, part);
        sprintf(path, "/game_scene/%s.gs", name);
        int size;
        char* data = fn_8006BA8C(path, &size, 0);
        m1A6C = GameScene::fn_803BEBAC(this, area, map, part);
        m1A6C->FreezeTask();
        m1A6C->fn_803C3894(data, size, 0);
        atn::FileFreeBuffer(data, false);
        if (area == 1 && lbl_8087F0A8->mLoadTownSubScript != 0) {
            sprintf(path, "/game_scene/tw_subquest.gs");
            if (fn_8006AA20(path) != 0) {
                char* sub = fn_8006BA8C(path, &size, 0);
                m1A6C->fn_803C3894(sub, size, 1);
                atn::FileFreeBuffer(sub, false);
            }
            sprintf(path, "/game_scene/tw_hintnpc.gs");
            if (fn_8006AA20(path) != 0) {
                char* sub = fn_8006BA8C(path, &size, 0);
                m1A6C->fn_803C3894(sub, size, 2);
                atn::FileFreeBuffer(sub, false);
            }
        }
        if (GimmickManager::instance_ == 0) {
            GimmickManager::SetTask(this);
        }
        m1A6C->fn_803CA530();
    }
    if ((flags & 0x100) && !(flags & 0x10000000) && areaData != 0 && !(flags & 0x200)) {
        atn::FixedArray_unk<MapPart_unk*, 64>& parts = MapManager::GetInstance_unk()->mParts;
        for (int i = 0; i < (int)parts.size(); i++) {
            MapManager::GetInstance_unk()->fn_80491440(parts[i]);
        }
        if (area == 1) {
            std::string file = fn_800DC980("database\\map\\tw%02d_%02d.twn", map, part);
            m1A60.push_back(fn_8049D68C(this, file.c_str()));
            m1A60[0]->m48 = area;
            m1A60[0]->m4C = map;
            m1A60[0]->m50 = part;
            for (int i = 0; i < (int)seq->m5C.size(); i++) {
                file = fn_804805CC(map, seq->m5C[i * 2 + 1], seq->m5C[i * 2]);
                m1A60.push_back(fn_8049D68C(this, file.c_str()));
            }
        } else {
            m1A60.push_back(fn_8049D704(this, area, map, part));
        }
        if (lbl_8087EE90 != 0) {
            fn_8004895C(lbl_8087EE90);
            fn_80049654(lbl_8087EE90);
            const char* groups[2] = {"GROUP_SYSTEM", "GROUP_PC001"};
            fn_8004829C(lbl_8087EE90, groups, 2);
        }
        if (areaData != 0) {
            fn_800C3094(areaData->m68);
        }
        if (ColliAttrManager::sInstance == 0) {
            ColliAttrManager::fn_801839FC(this);
        }
        ColliAttrManager::sInstance->fn_80183D00(area, map, part);
        if (GameBgm::GetInstance_unk() == 0) {
            GameBgm::fn_8037C8C4(this);
        }
        if (lbl_8087F498 == 0) {
            GameVoice::fn_803E836C(this);
        }
    }
    if (flags & 0x4000) {
        if (lbl_8087F8A0 == 0) {
            PlayerManager::fn_80549D8C(this);
        }
        if (Inventory::Order() == 0) {
            Inventory::fn_80442F10();
        }
        lbl_8087F8A0->fn_80549F14(area);
        fn_8047FF70(0, 1, 0.0f, lbl_807C7030);
    }
    if (flags & 0x1000) {
        if (NpcManager::sInstance == 0) {
            NpcManager::fn_805477C8(this);
        }
        if (EnemyManager::sInstance == 0) {
            EnemyManager::fn_80334B38(this);
        }
        if (FriendManager::sInstance == 0) {
            FriendManager::fn_80365464(this);
        }
        if (Inventory::Order() == 0) {
            Inventory::fn_80442F10();
        }
        GameScene* scene = m1A6C;
        if (scene != 0) {
            if (flags & 0x2000) {
                for (int i = 0; i < (int)scene->m80.size(); i++) {
                    if (seq->fn_80541CFC(1, scene->m80[i].mId) != 0) {
                        gs::NpcSetPoint& p = scene->m80[i];
                        fn_8047FF70(1, p.m20, p.m14, p.mPos)->m58 = scene->m80[i].mId;
                    }
                }
                for (int i = 0; i < (int)scene->m88.size(); i++) {
                    if (seq->fn_80541CFC(2, scene->m88[i].mId) != 0) {
                        gs::NpcSetPoint& p = scene->m88[i];
                        fn_8047FF70(2, p.m20, p.m14, p.mPos)->m58 = scene->m88[i].mId;
                    }
                }
                for (int i = 0; i < (int)scene->m90.size(); i++) {
                    if (seq->fn_80541CFC(3, scene->m90[i].mId) != 0) {
                        gs::NpcSetPoint& p = scene->m90[i];
                        fn_8047FF70(3, p.m20, p.m14, p.mPos)->m58 = scene->m90[i].mId;
                    }
                }
            } else {
                for (int i = 0; i < (int)scene->m80.size(); i++) {
                    gs::NpcSetPoint& p = scene->m80[i];
                    fn_8047FF70(1, p.m20, p.m14, p.mPos)->m58 = scene->m80[i].mId;
                }
                for (int i = 0; i < (int)scene->m88.size(); i++) {
                    gs::NpcSetPoint& p = scene->m88[i];
                    fn_8047FF70(2, p.m20, p.m14, p.mPos)->m58 = scene->m88[i].mId;
                }
                for (int i = 0; i < (int)scene->m90.size(); i++) {
                    gs::NpcSetPoint& p = scene->m90[i];
                    fn_8047FF70(3, p.m20, p.m14, p.mPos)->m58 = scene->m90[i].mId;
                }
            }
        }
    }
}

void Manager::fn_804814F8() {
    Sequence* seq = m70;
    Cut* cut = seq != 0 ? seq->fn_80541210() : 0;
    if (cut != 0 && seq->m194 == 10) {
        if (cut->mCinematic != 0 && m1A7C->IsFlag_unk(4)) {
            if (m1A7C->IsFlag_unk(4) != 0) {
                m1A7C->mFrame = 1.0f;
                m1A7C->mSpeed = 1.0f;
                m1A7C->MeltTask();
            }
        } else if (cut->mCinematic == 0 && !m1A7C->IsFlag_unk(4)) {
            if (m1A90 == 0) {
                m1A90 = 1;
                m1A8C = 5;
            }
        }
    }
    if (!m1A7C->IsFlag_unk(4) && m1A7C->mSpeed < 0.0f && m1A7C->mFrame <= 1.0f) {
        m1A7C->FreezeTask();
    }
    if (m1A90 != 0) {
        if (m1A8C == -1) {
            m1A90 = 0;
            m1A7C->mSpeed = -1.0f;
        } else {
            m1A8C--;
        }
    }
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

void Manager::fn_804816F4() {
    if (m1A80->IsFlag_unk(4)) {
        m1A80->mFrame = 0.0f;
        m1A80->mSpeed = 1.0f;
        m1A80->MeltTask();
        DamageEffectParam_unk damage = lbl_8087EFA8->m374;
        damage.mEnabled = 1;
        damage.mAngle = 0.0f;
        damage.mColor.Set_unk(0.7f, 0.9f, 0.95f, 1.0f);
        damage.mScale = -2.0f;
        damage.mZoom = 0.05f;
        DamageEffectParam_unk result = damage;
        lbl_8087EFA8->m374 = result;
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

void Manager::fn_80481818() {
    if (m1A88 == 1) {
        DamageEffectParam_unk damage = lbl_8087EFA8->m374;
        if (damage.mEnabled == 0) {
            m1A88 = 0;
        } else {
            m1A88 = 2;
            m1A84 = m70->m198;
            if (m23B4 != 0) {
                fn_800C31F4("SE_UICOM_011", 0, -1, 1.0f);
                m48->FreezeTask();
                float scale = m5C;
                atn::Vector2_unk center(320.0f, 224.0f);
                atn::Vector2_unk size(scale, scale);
                SetLayoutValue_unk(&m48->mData.m10, "basePos", 0, center.x);
                SetLayoutValue_unk(&m48->mData.m10, "basePos", 1, center.y);
                SetLayoutValue_unk(&m48->mData.m10, "basePos", 2, size.x);
                SetLayoutValue_unk(&m48->mData.m10, "basePos", 3, size.y);
                m48->mSpeed = m60;
                m48->mFrame = 0.0f;
                m48->mRepeat = 0;
                m48->MeltTask();
                m23B4 = 0;
            }
        }
    }
}
void Manager::fn_80481A20() {
    if (m1A88 == 2) {
        float t = m70->m198 - m1A84;
        DamageEffectParam_unk damage = lbl_8087EFA8->m374;
        if (damage.mEnabled == 0) {
            m1A88 = 0;
        } else {
            float r = MIN(MAX(t / 20.0f, 0.0f), 1.0f);
            damage.mZoom = lbl_8087E088 + r * (lbl_8087E08C - lbl_8087E088);
            if (t >= 20.0f) {
                m1A88 = 0;
                damage.mEnabled = 0;
            }
            DamageEffectParam_unk result = damage;
            RenderSetting_unk* rs = lbl_8087EFA8;
            rs->m374 = result;
        }
    }
}
// inferred: inverse of fn_8047C88C; only inlined (fn_80481B9C), so the linker strips it
void SceneSetting_unk::Get_unk(NewCampScene2_unk* scene) {
    mFog = *scene->fn_8047C770();
    mAmbient = scene->GetAmbient_unk();
    mLS = scene->GetLS_unk();
    m3C = *scene->fn_8047C778();
    mGodRay = *scene->fn_8047C780();
    mShadow = *scene->fn_8047C7D4();
    mColorMatrix = *scene->fn_8047C7DC();
    mHaze = *scene->fn_8047C848();
    m148 = scene->m120;
    mLight = *fn_800C2448(scene, 0);
}

// NONMATCHING: 58.4% - std::vector::push_back: the original takes another path for this element
// (in-place copy when size < cap, else the length check fn_8049003C, which returns 0, and the
// reallocation fn_80490098; copies through fn_80482400), not the one of include/std/vector.h
// inferred: adds the light settings of a map area to the event settings (callback, see 0x804834E4)
void fn_80481B9C(MapArea* area, void* arg) {
    EventConfig_unk* config = (EventConfig_unk*)arg;
    SceneSettingEntry_unk entry;
    entry.mSetting.Get_unk(&area->GetScene_unk());
    entry.mId = area->m24;
    config->m110.push_back(entry);
}

// inferred: argument of fn_804827D8 and fn_80482E4C (built by fn_80483E80 and fn_80487BB4): the
// event settings, the next entry (fn_80482E4C), blend or set, and the blend rate. Names invented.
struct SceneApply_unk {
    // inferred: weak copy 0x80483E80 (called by fn_80487490)
    SceneApply_unk(EventConfig_unk* config) : mConfig(config), mIndex(0), mBlend(false) {}
    // inferred: weak copy 0x80487BB4 (called by Manager::vf10)
    SceneApply_unk(EventConfig_unk* config, float rate) : mConfig(config), mIndex(0), mBlend(true), mRate(rate) {}
    EventConfig_unk* mConfig;
    int mIndex;
    bool mBlend; // 0x08
    float mRate; // 0x0C
};

// inferred: applies (or blends) the light settings of the event to a map area (callback, 0x80487500)
void fn_804827D8(MapArea* area, void* arg) {
    SceneApply_unk* apply = (SceneApply_unk*)arg;
    EventConfig_unk* config = apply->mConfig;
    for (unsigned int i = 0; i < config->m110.size(); i++) {
        if (area->m24 == config->m110[i].mId) {
            if (apply->mBlend) {
                config->m110[i].mSetting.fn_8047CB74(&area->GetScene_unk(), apply->mRate);
            } else {
                config->m110[i].mSetting.fn_8047C88C(&area->GetScene_unk());
            }
            return;
        }
    }
}
// inferred: sets the light of gmk::CharaLight on a map area (callback, as gmk fn_804225E0)
void fn_80482B24(MapArea* part, void* light) {
    part->m074 = light;
    part->m070 = light != 0 || part->m070 != 0;
    part->m0C8 = -1;
}
// inferred: argument of fn_80482B54: color matrices of the steps, the next step, the matrix it blends
// with and the rate. Names invented.
struct ColorMatrixBlend_unk {
    // inferred: built by fn_80484088
    ColorMatrixBlend_unk(float rate) : mIndex(0), mRate(rate) {}
    std::vector<ColorMatrixParam_unk> mList;
    int mIndex;                   // 0x0C
    ColorMatrixParam_unk mTarget; // 0x10
    float mRate;                  // 0x60
};

// NONMATCHING: 97.1% - float registers of the blend (1 - t in f10, not f6)
// inferred: blends the next color matrix with the target and gives it to a map area (callback)
void fn_80482B54(MapArea* area, void* arg) {
    ColorMatrixBlend_unk* blend = (ColorMatrixBlend_unk*)arg;
    if (blend->mIndex < (int)blend->mList.size()) {
        ColorMatrixParam_unk cm = blend->mList[blend->mIndex];
        ColorMatrixParam_unk target = blend->mTarget;
        float t = blend->mRate;
        for (int i = 0; i < 4; i++) {
            cm.mRows[i].x = t * cm.mRows[i].x + (1.0f - t) * target.mRows[i].x;
            cm.mRows[i].y = t * cm.mRows[i].y + (1.0f - t) * target.mRows[i].y;
            cm.mRows[i].z = t * cm.mRows[i].z + (1.0f - t) * target.mRows[i].z;
            cm.mRows[i].w = t * cm.mRows[i].w + (1.0f - t) * target.mRows[i].w;
        }
        area->mUseColorMatrix = 1;
        area->mColorMatrix = cm;
        blend->mIndex++;
    }
}


// inferred: copies the direction and the color of a light to the first light of a map area (callback)
void fn_80482D90(MapArea* area, void* arg) {
    atn::Light* src = (atn::Light*)arg;
    atn::Light* dst = fn_800C2448(&area->GetScene_unk(), 0);
    dst->SetDir_unk(*src->GetDir_unk());
    dst->SetColor(*src->GetColor_unk());
}

// inferred: copies the color and the blur of a god ray to a map area (callback)
void fn_80482DF4(MapArea* area, void* arg) {
    GodRayParam_unk* src = (GodRayParam_unk*)arg;
    area->fn_8047C780()->mColor = src->mColor;
    area->fn_8047C780()->mBlurHeight = src->mBlurHeight;
    area->fn_8047C780()->mBlurWidth = src->mBlurWidth;
}

// inferred: sets the "LS" color of a map area (callback)
void fn_80482E28(MapArea* area, void* arg) {
    area->GetLS_unk() = *(atn::Color*)arg;
}

// inferred: sets the "LS" color of the next light setting to a map area (callback)
void fn_80482E4C(MapArea* area, void* arg) {
    SceneApply_unk* it = (SceneApply_unk*)arg;
    if (it->mIndex < (int)it->mConfig->m110.size()) {
        area->GetLS_unk() = atn::Color(it->mConfig->m110[it->mIndex].mSetting.mLS);
        it->mIndex++;
    }
}

// inferred: sets the ambient color of a map area (callback)
void fn_80482EB4(MapArea* area, void* arg) {
    area->GetAmbient_unk() = *(atn::Color*)arg;
}
// inferred: map of the field, else the first map part (inlined in fn_80482ED8, fn_80483714,
// fn_80487064 and fn_804870F4); name invented
inline GameMap_unk* GetCurrentMap_unk() {
    GameMap_unk* map = 0;
    if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
        map = fn_80373148(lbl_8087F430);
    }
    if (map == 0 && MapManager::sInstance != 0 && MapManager::sInstance->mParts.size() != 0) {
        map = (GameMap_unk*)MapManager::sInstance->mParts[0]; // inferred: the same map task
    }
    return map;
}

// NONMATCHING: 97.8% - scheduling of the default damage constants and registers of m110.clear()
// inferred: saves the render settings, the light settings of the scene and of the map areas, the
// map and the BGM before an event (once: m1AB0.m00)
void Manager::fn_80482ED8() {
    if (m1AB0.m00 != 0) {
        return;
    }
    RenderSetting_unk* rs = lbl_8087EFA8;
    Camera_unk* view = lbl_8087EFB4;
    if (rs != 0 && view != 0) {
        m1AB0.mBloom = rs->m054;
        m1AB0.m88 = rs->m0D4;
        m1AB0.m8C = rs->m0D8;
        m1AB0.m90 = rs->m0DC;
        m1AB0.m94 = rs->m0E0;
        m1AB0.m98 = rs->m0E4;
        m1AB0.m9C = rs->m0E8;
        m1AB0.mA0 = rs->m0EC;
        m1AB0.mA4 = rs->m0F0;
        m1AB0.mA8 = rs->m0F4;
        m1AB0.mAC = rs->m0F8;
        m1AB0.mB0 = rs->m0FC;
        m1AB0.mB4 = rs->m100;
        m1AB0.mB8 = rs->m240;
        m1AB0.mDC = rs->m374;
        rs->m374 = DamageEffectParam_unk();
        m1AB0.m100 = rs->m2AC;
        m1BD0.Get_unk(fn_800C16B4(view));
        m1AB0.m110.clear();
        MapManager::sInstance->fn_80491528(fn_80481B9C, &m1AB0);
        m1AB0.m11C = fn_800C16B4(lbl_8087EFB4)->m074;
        GameMap_unk* map = GetCurrentMap_unk();
        if (map != 0) {
            m1D6C = map->m104;
        }
        m1D64 = GameBgm::sInstance != 0
                    ? (GameBgm::sInstance->m84 != 0 ? GameBgm::sInstance->mNext.mId : GameBgm::sInstance->mReq.mId)
                    : -1;
        m1D68 = GameBgm::sInstance != 0 ? GameBgm::sInstance->mReq.mVolume : 0.0f;
        m1AB0.m00 = 1;
    }
}
// NONMATCHING: 98.5% - load order of the damage copy (m1AB0.mDC)
// inferred: restores what fn_80482ED8 saved (b != 0: keep it saved); then fn_80483E94(a)
void Manager::fn_80483714(int a, int b) {
    if (m1AB0.m00 == 0) {
        return;
    }
    RenderSetting_unk* rs = lbl_8087EFA8;
    if (rs == 0) {
        return;
    }
    Camera_unk* view = lbl_8087EFB4;
    if (view == 0) {
        return;
    }
    if (rs != 0) {
        rs->m240 = m1AB0.mB8;
    }
    rs->m054 = m1AB0.mBloom;
    rs->m0D4 = m1AB0.m88;
    rs->m0D8 = m1AB0.m8C;
    rs->m0DC = m1AB0.m90;
    rs->m0E0 = m1AB0.m94;
    rs->m0E4 = m1AB0.m98;
    rs->m0E8 = m1AB0.m9C;
    rs->m0EC = m1AB0.mA0;
    rs->m0F0 = m1AB0.mA4;
    rs->m0F4 = m1AB0.mA8;
    rs->m0F8 = m1AB0.mAC;
    rs->m0FC = m1AB0.mB0;
    rs->m100 = m1AB0.mB4;
    DamageEffectParam_unk damage = m1AB0.mDC;
    rs->m374 = damage;
    rs->m2AC = m1AB0.m100;
    m1AB0.m01 = 0;
    m1BD0.fn_8047C88C(fn_800C16B4(view));
    SceneApply_unk apply(&m1AB0);
    MapManager::sInstance->fn_80491528(fn_804827D8, &apply);
    GameMap_unk* map = GetCurrentMap_unk();
    if (map != 0) {
        map->m104 = m1D6C;
    }
    if (mC4 != 0) {
        fn_8047F984(1.0f);
        mC4 = 0;
    }
    fn_800C16B4(lbl_8087EFB4)->SetLight_unk(m1AB0.m11C);
    fn_800DAA3C(lbl_8087F008, 0);
    if (b == 0) {
        fn_80483E94(a);
        m1AB0.m00 = 0;
    }
}
// inferred: end of the saved state: restores the BGM (arg != 0), the sounds and the collision
// attributes
__declspec(noinline) void Manager::fn_80483E94(int arg) {
    if (m1AB0.m00 != 0) {
        if (arg != 0 && m1D64 != -1 && GameBgm::sInstance != 0) {
            GameBgm::sInstance->fn_8037D4C0(m1D64, 60, 1, 0, m1D68);
        }
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->fn_800CF680(3, 60, 0);
        }
        if (lbl_8087F418 != 0) {
            lbl_8087F418->fn_80360780(1, 4, 60);
            lbl_8087F418->fn_803606CC(1, 4, 60);
        }
        if (ColliAttrManager::sInstance != 0 && ColliAttrManager::sInstance->mEnabled == 0) {
            ColliAttrManager::sInstance->mEnabled = 1;
        }
        m1AB0.m00 = 0;
    }
}
// inferred: color matrix of the map area at the target of the current camera, copied into the render
// settings (uses the manager instance, not this)
ColorMatrixParam_unk* Manager::fn_80483F70() {
    Manager* manager = lbl_8087F540;
    ColorMatrixParam_unk* dst = &lbl_8087EFA8->m324;
    atn::Sphere sphere(manager->mCameras[manager->m1A38].mTarget, 30.0f);
    if (MapManager::sInstance != 0) {
        MapArea* area = MapManager::sInstance->fn_804918A4(sphere);
        if (area != 0) {
            *dst = *area->fn_8047C7DC();
        }
    }
    return dst;
}
// NONMATCHING: 95.1% - growth path of std::vector::push_back (see docs/msl_cpp.md)
// inferred: blends the color matrices of the light settings of the map areas with `target`
void Manager::fn_80484088(ColorMatrixParam_unk* target, float rate) {
    ColorMatrixBlend_unk blend(rate);
    blend.mTarget = *target;
    blend.mList.clear();
    for (int i = 0; i < (int)m1AB0.m110.size(); i++) {
        blend.mList.push_back(m1AB0.m110[i].mSetting.mColorMatrix);
    }
    MapManager::sInstance->fn_80491528(fn_80482B54, &blend);
}
// inferred: sets the direction and the color (0xAARRGGBB) of the first light of the render scene and
// of the map areas
void Manager::fn_804846FC(const atn::Vector3& dir, int color) {
    atn::Light* light = fn_800C2448(fn_800C16B4(lbl_8087EFB4), 0);
    unsigned int c = color;
    light->SetDir_unk(dir);
    light->SetColor(atn::Color((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                               (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24)));
    MapManager::sInstance->fn_80491528(fn_80482D90, light);
}
// inferred: sets the ambient color (0xAARRGGBB) of the map areas
void Manager::fn_80484A78(int color) {
    unsigned int c = color;
    atn::Color ambient((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                       (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24));
    MapManager::sInstance->fn_80491528(fn_80482EB4, &ambient);
}

// inferred: uses the light of the manager (m1EB8) for the render scene and the map areas
void Manager::fn_80484BF4() {
    fn_800C16B4(lbl_8087EFB4)->SetLight_unk(&m1EB8);
    fn_800C16B4(lbl_8087EFB4)->SetLightIndex_unk(-1);
    MapManager::sInstance->fn_80491528(fn_80482B24, &m1EB8);
}
// inferred: position, size and speed of the light of the manager
void Manager::fn_80484D40(atn::Vector2_unk scale, atn::Vector2_unk pos, float rate) {
    m1F08 = rate;
    m1EB8.m04.x = pos.x;
    m1EB8.m04.y = pos.y;
    m1EB8.mSize = scale;
}

// inferred: scales a 2D vector (weak copy 0x80484E6C); name invented
inline atn::Vector2_unk Scale_unk(const atn::Vector2_unk& v, float s) {
    return atn::Vector2_unk(v.x * s, v.y * s);
}

// inferred: moves the light and grows it towards `scale` (cloud map shown) or shrinks it
void Manager::fn_80484D64(atn::Vector2_unk scale, atn::Vector2_unk pos) {
    m1EB8.m04.x = pos.x;
    m1EB8.m04.y = pos.y;
    if (m1F0C != 0) {
        if (m1EB8.mSize.x < scale.x && m1EB8.mSize.y < scale.y) {
            atn::Vector2_unk d = Scale_unk(scale, m1F08);
            m1EB8.mSize.x += d.x;
            m1EB8.mSize.y += d.y;
        } else {
            m1EB8.mSize = scale;
        }
    } else if (m1EB8.mSize.x > 0.0f && m1EB8.mSize.y > 0.0f) {
        atn::Vector2_unk d = Scale_unk(scale, m1F08);
        m1EB8.mSize.x -= d.x;
        m1EB8.mSize.y -= d.y;
    } else {
        atn::Vector2_unk d = Scale_unk(scale, 0.0f);
        m1EB8.mSize.x -= d.x;
        m1EB8.mSize.y -= d.y;
    }
}

// inferred: removes the light of the manager from the render scene and from the map areas
void Manager::fn_80484E88() {
    fn_800C16B4(lbl_8087EFB4)->SetLight_unk(0);
    if (MapManager::sInstance != 0) {
        MapManager::sInstance->fn_80491528(fn_80482B24, 0);
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
// inferred: sets the color (0xAARRGGBB) and the blur of the god ray of the render scene and of the
// map areas
void Manager::fn_80486BEC(int color, float blur) {
    GodRayParam_unk* godRay = fn_800C16B4(lbl_8087EFB4)->fn_8047C780();
    unsigned int c = color;
    godRay->mColor = atn::Color((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                                (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24));
    godRay->mBlurWidth = blur;
    godRay->mBlurHeight = blur;
    MapManager::sInstance->fn_80491528(fn_80482DF4, godRay);
}

// inferred: sets the "LS" color (0xAARRGGBB) of the render scene and of the map areas
void Manager::fn_80486DAC(int color) {
    atn::Color* ls = &fn_800C16B4(lbl_8087EFB4)->GetLS_unk();
    unsigned int c = color;
    *ls = atn::Color((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                     (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24));
    MapManager::sInstance->fn_80491528(fn_80482E28, ls);
}

// inferred: gives each map area the "LS" color of its light setting, in order
void Manager::fn_80486F50() {
    SceneApply_unk apply(&m1AB0);
    MapManager::sInstance->fn_80491528(fn_80482E4C, &apply);
}
// inferred: sets GameMap_unk::m104 (0-3) of the current map
void Manager::fn_80487064(int mode) {
    if (mode >= 0 && mode <= 3) {
        GameMap_unk* map = GetCurrentMap_unk();
        if (map != 0) {
            map->m104 = mode;
        }
    }
}


// inferred: restores GameMap_unk::m104 of the current map (saved by fn_80482ED8)
void Manager::fn_804870F4() {
    GameMap_unk* map = GetCurrentMap_unk();
    if (map != 0) {
        map->m104 = m1D6C;
    }
}
// inferred: true when the tasks of the manager are loaded; then they are started, hidden
int Manager::vf0C() {
    if (lbl_8087F430 == 0 && m1A9C == 0) {
        m1A9C = atn::FadeCapture::Create(this, 10, -1, 0xFFFFFF, 0);
        m1A9C->fn_800D246C(1);
        m1A9C->m78 = 0x12;
    }
    if (!m1A7C->IsFlag_unk(2)) {
        return 0;
    }
    if (!m1A80->IsFlag_unk(2)) {
        return 0;
    }
    if (!m1AA8->IsFlag_unk(2)) {
        return 0;
    }
    if (m1A9C != 0 && !m1A9C->IsFlag_unk(2)) {
        return 0;
    }
    if (m1EB8.fn_800C1990() != 0) {
        return 0;
    }
    if (!m23AC->IsFlag_unk(2)) {
        return 0;
    }
    if (!m23B0->IsFlag_unk(2)) {
        return 0;
    }
    if (!m241C->IsFlag_unk(2)) {
        return 0;
    }
    if (!m2438->IsFlag_unk(2)) {
        return 0;
    }
    if (!m48->IsFlag_unk(2)) {
        return 0;
    }
    m1A7C->fn_800D246C(0);
    m1A7C->FreezeTask();
    m1A7C->mRepeat = 0;
    m1A80->fn_800D246C(0);
    m1A80->FreezeTask();
    m1A80->mRepeat = 0;
    m1AA8->fn_800D246C(0);
    m1AA8->mActive = 0;
    m23AC->fn_800D246C(0);
    m23AC->FreezeTask();
    m23AC->mRepeat = 0;
    m23B0->fn_800D246C(0);
    m23B0->FreezeTask();
    m23B0->mRepeat = 0;
    m48->fn_800D246C(0);
    m48->FreezeTask();
    m48->mRepeat = 0;
    if (m1A9C != 0) {
        m1A9C->fn_800D246C(0);
        m1A9C->FreezeTask();
        m1A9C->mPauseAtEnd = 1;
    }
    m241C->fn_800D246C(0);
    m241C->mRepeat = 0;
    m241C->FreezeTask();
    m2438->fn_800D246C(0);
    m2438->mRepeat = 1;
    m2438->FreezeTask();
    return 1;
}
// NONMATCHING: 99.4% - the original calls no constructor for the rotation matrix (Matrix3x4() is called
// here, all the inlines being out of line: over the inlining budget)
// inferred: update of the manager: light settings blend (m1AB0.m01), pending map change, the check
// sphere (m1EA4), the queue of sequences, the movie fade and a BGM of event 0xCE7
void Manager::vf10() {
    if (m1AB0.m01 != 0) {
        m1AB0.m04 -= 0.1f;
        if (m1AB0.m04 <= 0.0f) {
            m1BD0.fn_8047C88C(fn_800C16B4((Camera_unk*)fn_8008B964()));
            SceneApply_unk apply(&m1AB0);
            MapManager::GetInstance_unk()->fn_80491528(fn_804827D8, &apply);
            m1AB0.m01 = 0;
        } else {
            m1BD0.fn_8047CB74(fn_800C16B4((Camera_unk*)fn_8008B964()), m1AB0.m04);
            SceneApply_unk apply(&m1AB0, m1AB0.m04);
            MapManager::GetInstance_unk()->fn_80491528(fn_804827D8, &apply);
        }
    }
    if (m1DE8 != 0 && m1DE4 >= 0) {
        if (m1DE4 == 0) {
            m1DEC = m1DE8->GetState_unk().m02;
            m1DF0 = m1DE8->GetState_unk().m00;
            m1DF4 = m1DE8->GetState_unk().m03;
            fn_804805F0(m1DE8, m1DF8);
        }
        m1DE4--;
    }
    if (m1EA4 != 0) {
        m241C->MeltTask();
        m2438->MeltTask();
        m1EAC++;
        atn::Vector2_unk stick(lbl_807C7028);
        if (GetPad_unk() != 0) {
            stick.x = -fn_800A56A8(GetPad_unk(), 0, 0, 0);
            stick.y = fn_800A56A8(GetPad_unk(), 0, 1, 0);
        }
        if (m1EA8 != 0) {
            m241C->SetLayoutValue_unk("colorR", 195.0f);
            m241C->SetLayoutValue_unk("colorG", 65.0f);
            m241C->SetLayoutValue_unk("colorB", 80.0f);
            m241C->SetSpeed_unk(1.0f);
            m242C = lbl_807C7028;
        } else {
            m241C->SetLayoutValue_unk("colorR", 104.0f);
            m241C->SetLayoutValue_unk("colorG", 121.0f);
            m241C->SetLayoutValue_unk("colorB", 166.0f);
            m241C->SetSpeed_unk(-1.0f);
            m242C = Scale_unk(stick, -6.0f);
        }
        m2424 = fn_800F84D8(m2424, m242C, 0.1f);
        m241C->SetLayoutValue_unk("centerPos", 0, m2424.x);
        m241C->SetLayoutValue_unk("centerPos", 1, m2424.y);
        m2438->SetLayoutValue_unk("centerPos", 0, 320.0f + m2424.x);
        m2438->SetLayoutValue_unk("centerPos", 1, 224.0f + m2424.y);
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        if (m1EAC >= 150) {
            if (m1EA8 != 0) {
                m2434 = MAX(0.0f, m2434 - 0.1f);
            } else {
                m2434 = MIN(1.0f, 0.1f + m2434);
                Camera_unk* camera = fn_800F7258(fn_8008B964());
                atn::Vector3 target = fn_80383728(camera);
                dir = m1E94.mCenter - *camera->GetPosition_unk();
                if (dir.IsZero_unk()) {
                    dir.Set_unk(0.0f, 0.0f, 1.0f);
                }
                dir.Normalize_unk();
                atn::Matrix3x4 rot;
                rot.fn_8013A13C(target.ToRotation_unk().y);
                dir = dir - target;
                dir.fn_80011410(Inverse_unk(rot));
                dir.z = 0.0f;
                if (dir.IsZero_unk()) {
                    dir.Set_unk(0.0f, 1.0f, 0.0f);
                }
                dir.Normalize_unk();
                dir.Set_unk(-dir.x, 0.0f, dir.y);
            }
        } else {
            m2434 = MAX(0.0f, m2434 - 0.1f);
        }
        m2438->SetLayoutValue_unk("arrowA", m2434);
        m243C = atn::Lerp_unk(m243C, dir, 0.15f);
        float angle = 0.0f;
        if (!m243C.IsZero_unk()) {
            angle = fn_802A7964(m243C.ToRotation_unk().y);
        }
        m241C->SetLayoutValue_unk("arrowRot", angle);
        m2438->SetLayoutValue_unk("arrowRot", angle);
        if (m1EB0 > 0) {
            m1EB0++;
            if (m1EB0 >= 3) {
                m1EA4 = 0;
            }
        }
    } else {
        m241C->Rewind_unk();
        m241C->FreezeTask();
        m1EAC = 0;
        m2424 = lbl_807C7028;
        m242C = lbl_807C7028;
        m2438->FreezeTask();
        m2434 = 0.0f;
        m243C = atn::Vector3(0.0f, 0.0f, 1.0f);
    }
    if (!m23B8.empty()) {
        QueuedSequence_unk& entry = m23B8.front();
        if (GetSoundMgr_unk() == 0 || GetSoundMgr_unk()->fn_800D089C(8, 0) <= 0) {
            entry.m04++;
        } else {
            entry.m04 = 0;
        }
        if (entry.m04 > 10) {
            fn_8047E964(entry.mId, 0);
            m23B8.pop_front();
        }
    }
    if (m1F6C > 0.0f) {
        m1F6C -= fn_802F0988(fn_802F0990());
        if (m1F6C <= 0.0f) {
            m1F6C = 0.0f;
        }
    }
    if (fn_80112F54() != 0 && fn_80112F54()->fn_8047F400() == 0 && fn_80112F54()->GetId_unk() == 0xCE7 &&
        GameBgm::GetInstance_unk() != 0 && GameBgm::GetInstance_unk()->fn_8037D34C() == 0) {
        GameBgm::GetInstance_unk()->fn_8037D3E8(1, 4, 0);
    }
}
// inferred: states 11 and 12 of a sequence (a bool, not folded with the test of 10); name invented
inline bool IsCutState_unk(Sequence* seq) {
    return seq->m194 == 11 || seq->m194 == 12;
}

// NONMATCHING: 99.4% - registers of the inlined Camera_unk copy (frustum loop) and of seq/the camera offset
// inferred: after the sequences: copies the camera m1A94, updates the cameras of a running cut and
// the pending changes of the manager
void PostSequence::vf10() {
    if (lbl_8087F540->m1A94 != 0) {
        (Camera_unk&)lbl_8087F540->mCameras[lbl_8087F540->m1A98] = *lbl_8087F540->m1A94;
        lbl_8087F540->mCameras[lbl_8087F540->m1A98].fn_801162A4(1);
    }
    Sequence* seq = lbl_8087F540->m70;
    if (seq != 0 && (seq->m194 == 10 || IsCutState_unk(seq))) {
        for (int i = 0; i < 4; i++) {
            lbl_8087F540->mCameras[i].fn_801162A4(i == lbl_8087F540->m1A38);
        }
        if (seq->m98_2 == 1) {
            seq->fn_8054FAD4();
            seq->m98_2 = 0;
        }
    }
    lbl_8087F540->m1A3C = lbl_8087F540->m1A38;
    lbl_8087F540->fn_8047FCC0();
    lbl_8087F540->fn_804814F8();
    lbl_8087F540->fn_8047F730();
}

// inferred: draw of the manager (debug information, check sphere)
void Manager::vf14() {
    if (m70 != 0) {
        if (m1A44 != 0) {
            m1DFC.fn_80221F54(m1E74);
        }
        if (m1A48 != m1A44) {
            if (m1A48 != 0 && m1A44 == 0) {
                lbl_8087F0A8->mDrawSystemInfo = 0;
            }
            m1A48 = m1A44;
        }
        if (m1E90 != 0 && m1EA4 != 0) {
            lbl_8087EEB0->fn_80063D3C(m1E94.mCenter, m1E94.mRadius, 0.0f, 0xFFFFFF00);
        }
    }
}
// inferred: as in Event/ActionActorPosition.cpp (template member defined out of the class)
template <class T>
T* DataList_unk<T>::Find_unk(int id) {
    for (int i = 0; i < (int)this->size(); i++) {
        T* p = &this->mData[i];
        if (id == p->m14) {
            return p;
        }
    }
    return 0;
}

// inferred: plays the stream named by the entry param 1 of the sequence (multi audio action)
void Manager::fn_80488050(Sequence* seq, Action* action) {
    if (seq == 0) {
        seq = action->fn_805381C8();
    }
    int id = action->GetParam_unk(1)->m04.i;
    float volume = action->GetParam_unk(2)->m04.f;
    int stopBgm = action->GetParam_unk(3)->m04.i;
    int a = action->GetParam_unk(4)->m04.i;
    int b = action->GetParam_unk(5)->m04.i;
    int c = action->GetParam_unk(6)->m04.i;
    StringData* data = seq->m114.Find_unk(id);
    if (data != 0) {
        int fade = action->m18;
        fn_804881B8(seq, data->m08.c_str(), fade, stopBgm, a, b, c, volume, 0, 0x200000);
    }
}
// inferred: actor of a character (kinds 1 and 7); name invented
inline bool IsCharaActor_unk(const Actor& actor) {
    return actor.m20 == 1 || actor.m20 == 7;
}

// NONMATCHING: 97.5% - the first actor loop reloads the size each iteration in the original; stack slot
// of the iterator tag of the string assignment
// inferred: starts the music `name` of the sequence; the flags (or those already set) say what is
// done once: stop the BGM, the environment sounds, the characters of the sequence or their sounds
void Manager::fn_804881B8(Sequence* seq, const char* name, int fade, int stopBgm, int a, int b, int c,
                          float volume, int d, int flags) {
    if ((m1F1C & 0x80000000) && m2380 > 0) {
        Sequence* prev = FindSequence_unk(m2380);
        if (prev != 0) {
            if (m1F1C & 0x200000) {
                fn_80488534(prev, 60);
            } else {
                fn_80488534(prev, 15);
            }
        }
    }
    flags |= m1F1C;
    if (fade <= 0) {
        fade = 60;
    }
    if (!(flags & 0x80000000)) {
        m1E78 = 1;
        m1E7C = name;
        m1E8C = 0.0f;
        m1E88 = 0.0f;
        flags |= 0x80000000;
    }
    if (!(flags & 1) && stopBgm != 0) {
        if (GameBgm::sInstance != 0) {
            GameBgm::sInstance->fn_8037D3E8(1, 4, fade);
        }
        flags |= 1;
    }
    if (!(flags & 2) && a != 0) {
        if (lbl_8087F418 != 0) {
            lbl_8087F418->fn_80360780(0, 4, fade);
        }
        flags |= 2;
    }
    if (!(flags & 4) && b != 0) {
        if (lbl_8087F418 != 0) {
            lbl_8087F418->fn_803606CC(0, 4, fade);
        }
        flags |= 4;
    }
    if (!(flags & 8) && c != 0) {
        bool ready = true;
        for (int i = 0; i < (int)seq->m164.size() && ready; i++) {
            if (seq->m164[i].m20 == 0) {
                ready = false;
            }
        }
        if (ready) {
            for (int i = 0; i < (int)seq->m164.size(); i++) {
                if (IsCharaActor_unk(seq->m164[i])) {
                    seq->m164[i].fn_80012774(0);
                }
            }
            if (ColliAttrManager::sInstance != 0) {
                ColliAttrManager::sInstance->mEnabled = 0;
            }
            flags |= 8;
        }
    }
    if (!(flags & 0x10) && d != 0) {
        for (PlayerTask* p = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0; p != 0; p = p->mNext) {
            p->m1220.m11 = 0;
        }
        for (EnemyTask* e = EnemyManager::sInstance != 0 ? EnemyManager::sInstance->mHead : 0; e != 0; e = e->mNext) {
            e->m1220.m11 = 0;
        }
        for (NpcTask* n = NpcManager::sInstance != 0 ? NpcManager::sInstance->mHead : 0; n != 0; n = n->mNext) {
            n->m1220.m11 = 0;
        }
        if (ColliAttrManager::sInstance != 0) {
            ColliAttrManager::sInstance->mEnabled = 0;
        }
        flags |= 0x10;
    }
    m1F1C = flags;
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
