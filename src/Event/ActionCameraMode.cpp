#include "Event/Cut.h"
#include "atn/RenderManager.h"
#include "Event/Manager.h"
#include "Event/Behavior.h"
#include "Event/Path.h"
#include "atn/LH_Xsca.h"
#include "atn/Color4f_unk.h"
#include "game/MapManager.h"
#include "macros.h"
#include "atn/SoundManager.h"
#include <boost/bind.hpp>

// Camera actions of the events: processors with vf08 (per frame) and vf0C (phase 0: start, 3/4:
// end), as the Event::Behavior* ones. Class names from the RTTI; the order of the vtables is
// Inertia, Motion, Focus, Setting, Shake, Effect, Mode.

namespace atn {

// inferred: base named in the RTTI of the processors (no member is used here)
template <class T>
class SingletonNoStatic {};

} // namespace atn

extern "C" atn::Vector3 lbl_807C7030;      // inferred: zero vector

namespace Event {

} // namespace Event

extern "C" RenderSetting_unk* lbl_8087EFA8; namespace atn {
class RenderManager;
}
// inferred: scene of the event (fn_80541210): its kind at 0x0C
struct EventScene_unk {
    unsigned char m00[0x0C];
    int m0C;
};

namespace Event {

// inferred: camera of the section of an action
inline EventCamera_unk* GetCamera_unk(ActionOwner_unk* section) {
    return &lbl_8087F540->mCameras[section->m10];
}

class ActionCameraInertia : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraInertia> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCameraMotion : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraMotion> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCameraFocus : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraFocus> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCameraSetting : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraSetting> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCameraShake : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraShake> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

// inferred: screen effect of the camera mode action (fade/flash: kind, two times and an ARGB color)

class ActionCameraMode : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraMode> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCameraEffect : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraEffect> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
    void fn_8055BB08(MapArea* area, const void* arg); // inferred: fog of a map area (bound with boost::bind)
    void fn_8055BC5C(MapArea* area, void* arg);       // inferred: haze of a map area (bound with boost::bind)
};

int ActionCameraMode::vf08(Action* arg) {
    EventManager_unk* manager = arg->fn_805381C8();
    ActionOwner_unk* section = arg->fn_805381A0();
    float now = manager->m198;
    int length;
    int start = arg->m10;
    int mode = arg->GetParam_unk(0)->m04.i;
    if (mode == 0) {
        length = arg->m18;
        if (length <= 0) {
            return 0;
        }
        GetCamera_unk(section)->fn_801129C0((now - start) / length);
    } else if (mode == 4) {
        length = arg->m14 - start;
        if (length <= 0) {
            return 0;
        }
        GetCamera_unk(section)->fn_801129C0((now - start) / length);
    }
    return 0;
}

int ActionCameraShake::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        lbl_8087F540->fn_80481818();
        ActionOwner_unk* section = arg->fn_805381A0();
        arg->fn_805381C8();
        atn::Vector3 amp;
        amp.x = arg->GetParam_unk(0)->m04.f;
        amp.y = arg->GetParam_unk(1)->m04.f;
        amp.z = arg->GetParam_unk(2)->m04.f;
        float a = arg->GetParam_unk(3)->m04.f;
        atn::Vector3 freq;
        freq.x = arg->GetParam_unk(4)->m04.f;
        freq.y = arg->GetParam_unk(5)->m04.f;
        freq.z = arg->GetParam_unk(6)->m04.f;
        float b = arg->GetParam_unk(7)->m04.f;
        GetCamera_unk(section)->fn_8011462C(1, amp, freq, a, b);
        break;
    }
    case 3:
    case 4:
        GetCamera_unk(arg->fn_805381A0())->fn_8011462C(0, lbl_807C7030, lbl_807C7030, 0.0f, 0.0f);
        break;
    }
    return 0;
}

int ActionCameraSetting::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        lbl_8087F540->fn_80481818();
        ActionOwner_unk* section = arg->fn_805381A0();
        if (arg->GetParam_unk(0)->m04.i != 0) {
            GetCamera_unk(section)->fn_80112840();
        }
    }
    return 0;
}

// NONMATCHING: 98.7% - the interpolation factor is in f2 here, f3 in the original (tried ternaries,
// inverted branches, a copy of the factor)
int ActionCameraSetting::vf08(Action* arg) {
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    float now = manager->m198;
    float left = (float)(arg->m10 + arg->m18) - now;
    float t;
    if (left > 0.0f) {
        t = lbl_8087F540->mB4 / left;
        if (t > 1.0f) {
            t = 1.0f;
        }
    } else {
        t = 1.0f;
    }
    EventCamera_unk* camera = GetCamera_unk(section);
    float fovy = arg->GetParam_unk(1)->m04.f;
    if (fovy != 0.0f) {
        float f = camera->mFovy + t * (atn::DegToRad_unk(fovy) - camera->mFovy);
        camera->m1FC = f;
        camera->mFovy = f;
    }
    float aspect = arg->GetParam_unk(2)->m04.f;
    if (aspect != 0.0f) {
        camera->mAspect = camera->mAspect + t * (aspect - camera->mAspect);
    }
    // inferred: two more parameters are tested but not used
    float p3 = arg->GetParam_unk(3)->m04.f;
    if (p3 != 0.0f) {
        float unused = p3;
    }
    float p4 = arg->GetParam_unk(4)->m04.f;
    if (p4 != 0.0f) {
        float unused = p4;
    }
    return 0;
}

// NONMATCHING: 78.6% - scheduling: the original loads the next parameter count before each store
// (tried locals and a const accessor); the color part matches
void ActionCameraEffect::fn_8055BB08(MapArea* area, const void* data) {
    Action* arg = (Action*)data;
    if (arg->GetParam_unk(1)->m04.i != 0) {
        area->mFog.mMode = arg->GetParam_unk(2)->m04.i + 1;
        area->mFog.mStart = arg->GetParam_unk(3)->m04.f;
        area->mFog.mEnd = arg->GetParam_unk(4)->m04.f;
        unsigned int c = arg->GetParam_unk(5)->m04.i;
        area->mFog.mColor = atn::Color4f_unk((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                                   (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24));
    } else {
        area->mFog.mMode = 0;
    }
}

void ActionCameraEffect::fn_8055BC5C(MapArea* area, void* arg) {
    area->mHaze.mValue = *(HazeParam_unk*)arg;
}

// inferred: phase 0 sets the effect of type param 0 (1 bloom, 2 fog, 3 wait, 4 damage, 5 rain, 6 haze,
// 7 screen fade), phase 2 fades the screen back and phase 3 restores the settings
// NONMATCHING: 99.4% - stack slots of the temporaries (colors, rain, damage copies: other order; the
// haze uses one vector temporary for all its pairs). The haze vectors and the bloom color are filled
// member by member: with the constructors the function goes over the inlining budget (docs/inlining.md
// R3) and GetParam_unk is called out of line (32%). The bias/scale loops and the kernel array give the
// unrolled checks and the word copy of the original
int ActionCameraEffect::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0:
        lbl_8087F540->fn_80481818();
        arg->fn_805381A0();
        switch (arg->GetParam_unk(0)->m04.i) {
        case 1: {
            int enable = arg->GetParam_unk(1)->m04.i;
            if (enable != 0) {
                BloomParam_unk bloom = lbl_8087EFA8->m054;
                bloom.mEnabled = enable;
                unsigned int c = arg->GetParam_unk(2)->m04.i;
                RsColor_unk color;
                color.r = (1.0f / 255.0f) * ((c >> 16) & 0xFF);
                color.g = (1.0f / 255.0f) * ((c >> 8) & 0xFF);
                color.b = (1.0f / 255.0f) * (c & 0xFF);
                color.a = (1.0f / 255.0f) * (c >> 24);
                bloom.mColor1 = color;
                for (int i = 0; i < 3; i++) {
                    bloom.mBias[i] = arg->GetParamAt_unk(3 + i)->m04.i;
                }
                for (int i = 0; i < 3; i++) {
                    bloom.mScale[i] = arg->GetParamAt_unk(6 + i)->m04.i;
                }
                bloom.mStrength = arg->GetParam_unk(9)->m04.i;
                if (bloom.mStrength < 0) {
                    bloom.mStrength = 0;
                }
                if (bloom.mStrength > 0x80) {
                    bloom.mStrength = 0x80;
                }
                lbl_8087EFA8->m054 = bloom;
            } else {
                lbl_8087EFA8->m054.mEnabled = enable;
            }
            break;
        }
        case 2: {
            atn::Scene* view = atn::RenderManager::instance_->fn_800C16B4();
            if (arg->GetParam_unk(1)->m04.i != 0) {
                view->mFog.mMode = arg->GetParam_unk(2)->m04.i + 1;
                view->mFog.mStart = arg->GetParam_unk(3)->m04.f;
                view->mFog.mEnd = arg->GetParam_unk(4)->m04.f;
                unsigned int c = arg->GetParam_unk(5)->m04.i;
                view->mFog.mColor = atn::Color4f_unk((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                                             (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24));
            } else {
                view->mFog.mMode = 0;
            }
            MapManager::sInstance->fn_80491528(boost::bind(&ActionCameraEffect::fn_8055BB08, this, _1, _2), arg);
            break;
        }
        case 3:
            arg->m38 = 0;
            break;
        case 4: {
            DamageEffectParam_unk damage;
            damage.mEnabled = 1;
            unsigned int c = arg->GetParam_unk(1)->m04.i;
            damage.mColor = RsColor_unk((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                                        (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24));
            damage.mAngle = arg->GetParam_unk(2)->m04.f;
            damage.mScale = arg->GetParam_unk(3)->m04.f;
            damage.mZoom = MIN(MAX(arg->GetParam_unk(4)->m04.f, -1.0f), 0.05f);
            DamageEffectParam_unk result = damage;
            lbl_8087EFA8->m374 = result;
            break;
        }
        case 5: {
            RainParam_unk rain;
            rain.mEnabled = arg->GetParam_unk(1)->m04.i;
            rain.mRate = arg->GetParam_unk(2)->m04.f;
            rain.mWarp = arg->GetParam_unk(3)->m04.f;
            lbl_8087EFA8->m2AC = rain;
            break;
        }
        case 6: {
            HazeParam_unk haze;
            haze.mEnabled = arg->GetParam_unk(1)->m04.i;
            haze.mWarp = arg->GetParam_unk(2)->m04.f;
            atn::Vector2_unk v;
            v.x = arg->GetParam_unk(3)->m04.f;
            v.y = arg->GetParam_unk(4)->m04.f;
            if (v.x <= 0.0f) {
                v.x = 0.01f;
            }
            if (v.y <= 0.0f) {
                v.y = 0.01f;
            }
            haze.mScale = v;
            v.x = arg->GetParam_unk(5)->m04.f;
            v.y = arg->GetParam_unk(6)->m04.f;
            haze.mScroll = v;
            v.x = arg->GetParam_unk(7)->m04.f;
            v.y = arg->GetParam_unk(8)->m04.f;
            haze.mPos = v;
            v.x = arg->GetParam_unk(9)->m04.f;
            v.y = arg->GetParam_unk(10)->m04.f;
            haze.mSize = v;
            RsVector4_unk fade;
            fade.x = arg->GetParam_unk(11)->m04.f;
            fade.y = arg->GetParam_unk(12)->m04.f;
            fade.z = arg->GetParam_unk(13)->m04.f;
            fade.w = arg->GetParam_unk(14)->m04.f;
            haze.mFade = fade;
            atn::RenderManager::instance_->fn_800C16B4()->mHaze.mValue = haze;
            MapManager::sInstance->fn_80491528(boost::bind(&ActionCameraEffect::fn_8055BC5C, this, _1, _2), &haze);
            break;
        }
        case 7: {
            arg->m38 = 0;
            int color = arg->GetParam_unk(1)->m04.i;
            lbl_8087F540->fn_8047F8A4(-1.0f);
            lbl_8087F540->fn_8047F850(arg->m18, color & 0x00FFFFFF, color | 0xFF000000);
            if (arg->fn_805381C8()->m190 == 0x151E && arg->m18 > 0 && lbl_8087EFE8 != 0) {
                lbl_8087EFE8->fn_800CF7DC(1, 4, arg->m18 - 30);
            }
            break;
        }
        }
        break;
    case 2:
        if (arg->GetParam_unk(0)->m04.i == 7) {
            int color = arg->GetParam_unk(1)->m04.i;
            lbl_8087F540->fn_8047F8A4(-1.0f);
            lbl_8087F540->fn_8047F850(arg->m1C, color | 0xFF000000, color & 0x00FFFFFF);
        }
        break;
    case 3:
        switch (arg->GetParam_unk(0)->m04.i) {
        case 0:
            lbl_8087F540->m1DC0 = lbl_8087EFA8->m240;
            break;
        case 4:
            DamageEffectParam_unk damage;
            DamageEffectParam_unk result = damage;
            lbl_8087EFA8->m374 = result;
            break;
        case 7: {
            EventManager_unk* manager = arg->fn_805381C8();
            if (manager == 0 ||
                (manager->m190 != 0xC87 &&
                 (manager->m190 != 0xFB5 || manager->fn_80541210() == 0 || manager->fn_80541210()->mIndex != 0xE))) {
                lbl_8087F540->fn_8047F8A4(-100.0f);
            }
            if (arg->fn_805381C8()->m190 == 0x151E && arg->m18 > 0 && lbl_8087EFE8 != 0) {
                lbl_8087EFE8->fn_800CF45C(0, 0);
            }
            break;
        }
        }
        break;
    }
    return 0;
}

// inferred: 0 to 1 over the fade in (m18 frames from the start), 1 to 0 over the fade out (m1C frames
// before the end), 1 in between
inline float FadeRate_unk(Action* arg, float now, int start, int end) {
    float rate;
    if (start <= now && now <= start + arg->m18) {
        float t = now - start;
        if (t <= 0.0f) {
            rate = 0.0f;
        } else {
            rate = t / arg->m18;
        }
    } else if (end - arg->m1C <= now && now <= end) {
        float t = now - (end - arg->m1C);
        if (t <= 0.0f) {
            rate = 1.0f;
        } else {
            rate = 1.0f - t / arg->m1C;
        }
    } else {
        rate = 1.0f;
    }
    return rate;
}

// inferred: damage effect of the parameters of `arg` (zoom faded in and out)
inline DamageEffectParam_unk GetDamage_unk(Action* arg, float now, int start, int end) {
    DamageEffectParam_unk damage;
    damage.mEnabled = 1;
    unsigned int c = arg->GetParam_unk(1)->m04.i;
    damage.mColor = RsColor_unk((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                                (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24));
    damage.mAngle = arg->GetParam_unk(2)->m04.f;
    damage.mScale = arg->GetParam_unk(3)->m04.f;
    float rate = FadeRate_unk(arg, now, start, end);
    float wide = arg->GetParam_unk(4)->m04.f;
    float zoom = rate * (1.0f + wide);
    damage.mZoom = MIN(MAX(zoom - 1.0f, -1.0f), 0.05f);
    DamageEffectParam_unk result = damage;
    return result;
}

// NONMATCHING: 98.6% - only the saved registers (arg r28, start r31, end r30, fade in r29, fade out r27
// in the original); tried ~15 orders of the locals. The fade helper takes start/end but reads m18/m1C
// and the damage copy goes through a local (the original copies it twice)
// inferred: per frame: 0 motion blur, 3 waits for the fade of the event, 4 damage effect, 7 fade of the
// screen to a color
int ActionCameraEffect::vf08(Action* arg) {
    EventManager_unk* manager = arg->fn_805381C8();
    int fadeIn = arg->m18;
    float now = manager->m198;
    int end = arg->m14;
    int start = arg->m10;
    int fadeOut = arg->m1C;
    switch (arg->GetParam_unk(0)->m04.i) {
    case 0: {
        MotionBlurParam_unk& from = lbl_8087F540->m1DC0;
        MotionBlurParam_unk blur;
        blur.mEnabled = arg->GetParam_unk(1)->m04.i;
        float left = (float)(start + fadeIn) - now;
        float t;
        if (left > 0.0f) {
            t = 1.0f - left / fadeIn;
        } else {
            t = 1.0f;
        }
        blur.mScale = arg->GetParam_unk(2)->m04.f;
        float shift = arg->GetParam_unk(3)->m04.f;
        blur.mScale = from.mScale + t * (blur.mScale - from.mScale);
        blur.mShift = from.mShift + t * (shift - from.mShift);
        lbl_8087EFA8->m240 = blur;
        break;
    }
    case 3:
        if (arg->m38 == 0) {
            if (fadeIn == 0) {
                fadeIn = end - start;
            }
            if (lbl_8087F540->fn_8047F6B0(fadeIn) == 3) {
                arg->m38 = 1;
            }
        }
        break;
    case 4:
        lbl_8087EFA8->m374 = GetDamage_unk(arg, now, start, end);
        break;
    case 7:
        if (fadeIn >= now - start) {
            break;
        }
        if (fadeOut >= end - now) {
            break;
        }
        if (arg->m38 == 0) {
            int color = arg->GetParam_unk(1)->m04.i | 0xFF000000;
            lbl_8087F540->fn_8047F8A4(-1.0f);
            lbl_8087F540->fn_8047F850((end - fadeOut) - (start + fadeIn), color, color);
            arg->m38 = 1;
        }
        break;
    }
    return 0;
}

// inferred: speeds of the focus change selected by parameter 5 (pairs of the same value)
static float sFocusSpeed_unk[5][2] = {
    {0.5f, 0.5f}, {1.0f, 1.0f}, {1.5f, 1.5f}, {0.1f, 0.1f}, {0.01f, 0.01f},
};

// NONMATCHING: 96.9% - registers of the speed row (the original indexes with lfsx and keeps the
// parameter 6 pointer in r7)
int ActionCameraFocus::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        lbl_8087F540->fn_80481818();
        ActionOwner_unk* section = arg->fn_805381A0();
        if (arg->GetParam_unk(0)->m04.i != 0) {
            float a = arg->GetParam_unk(1)->m04.f;
            float b = arg->GetParam_unk(2)->m04.f;
            float c = arg->GetParam_unk(3)->m04.f;
            float d = arg->GetParam_unk(4)->m04.f;
            int speed = arg->GetParam_unk(5)->m04.i;
            ActionParam_unk* mode = arg->GetParam_unk(6);
            float* sp = sFocusSpeed_unk[speed];
            float e = sp[0];
            GetCamera_unk(section)->fn_8011447C(a, b, c, d, sp[1], e, arg->m18, mode->m04.i);
        } else {
            GetCamera_unk(section)->fn_80114468();
        }
    }
    return 0;
}

// inferred: camera animation `id` of the event (no null test of the entry, as in the original)
inline CameraData* FindCameraAnim_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < (int)manager->m124.size(); i++) {
        CameraData* anim = &manager->m124[(unsigned int)i];
        if (id == anim->m14) {
            return anim;
        }
    }
    return 0;
}
inline Xsca_unk* GetCameraAnim_unk(EventManager_unk* manager, int id) {
    CameraData* anim = FindCameraAnim_unk(manager, id);
    return anim->m18 != 0 ? (Xsca_unk*)anim->m18->fn_804786F8() : 0;
}

// NONMATCHING: 94.9% - registers of the animation search and order of the int->float conversions of
// the start time (tried casts, a local for the current frame, both groupings)
int ActionCameraMotion::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        lbl_8087F540->fn_80481818();
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(0)->m04.i;
        float a = arg->GetParam_unk(1)->m04.f;
        float b = arg->GetParam_unk(2)->m04.f;
        float c = arg->GetParam_unk(3)->m04.f;
        int frame = arg->GetParam_unk(4)->m04.i;
        Xsca_unk* anim = GetCameraAnim_unk(manager, id);
        EventCamera_unk* camera = GetCamera_unk(section);
        camera->m400 = anim;
        camera->m404 = a;
        camera->m40C = b;
        camera->m410 = c;
        camera->m408 = frame;
        camera->m41C = 0;
        camera = GetCamera_unk(section);
        camera->m414 = arg->m18 + (arg->m10 - manager->m198);
        camera->m418 = 0.0f;
        break;
    }
    case 2: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        EventCamera_unk* camera = GetCamera_unk(section);
        float now = manager->m198;
        camera->m414 = -(arg->m18 + ((arg->m14 - arg->m1C) - now));
        camera->m418 = 0.0f;
        break;
    }
    case 3:
    case 4: {
        EventCamera_unk* camera = GetCamera_unk(arg->fn_805381A0());
        camera->m400 = 0;
        camera->m404 = 1.0f;
        camera->m40C = 1.0f;
        camera->m410 = 1.0f;
        camera->m408 = 0.0f;
        camera->m41C = 0;
        break;
    }
    }
    return 0;
}

int ActionCameraInertia::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        lbl_8087F540->fn_80481818();
        ActionOwner_unk* section = arg->fn_805381A0();
        float a = arg->GetParam_unk(0)->m04.f;
        float b = arg->GetParam_unk(1)->m04.f;
        GetCamera_unk(section)->fn_80112974(1, a, b);
        break;
    }
    case 3:
    case 4:
        GetCamera_unk(arg->fn_805381A0())->fn_80112974(0, 1.0f, 1.0f);
        break;
    }
    return 0;
}

int ActionCameraInertia::vf08(Action* arg) {
    return 0;
}

int ActionCameraMotion::vf08(Action* arg) {
    return 0;
}

int ActionCameraFocus::vf08(Action* arg) {
    return 0;
}

int ActionCameraShake::vf08(Action* arg) {
    return 0;
}

} // namespace Event
