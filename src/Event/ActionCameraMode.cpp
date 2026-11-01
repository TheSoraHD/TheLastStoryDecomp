#include "Event/Behavior.h"
#include "Event/Path.h"
#include "atn/LH_Xsca.h"
#include "atn/Color4f_unk.h"

// Camera actions of the events: processors with vf08 (per frame) and vf0C (phase 0: start, 3/4:
// end), as the Event::Behavior* ones. Class names from the RTTI; the order of the vtables is
// Inertia, Motion, Focus, Setting, Shake, Effect, Mode.

namespace atn {

// inferred: base named in the RTTI of the processors (no member is used here)
template <class T>
class SingletonNoStatic {};

} // namespace atn

extern "C" void fn_80481818(void* manager); // inferred: called when a camera action starts
extern "C" atn::Vector3 lbl_807C7030;      // inferred: zero vector

namespace Event {

// inferred: the event manager (lbl_8087F540) seen through its cameras (four of 0x65C bytes at
// 0xC8, one per section: Section::m10)
struct CameraOwner_unk {
    unsigned char m00[0xB4];
    float mB4; // inferred: frames of the current step (Setting interpolates over them)
    unsigned char mB8[0xC8 - 0xB8];
    EventCamera_unk mCameras[4];
};

} // namespace Event

extern "C" Event::CameraOwner_unk* lbl_8087F540;

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
struct CameraFx_unk {
    unsigned char m00[0x88];
    int m88; // inferred: kind + 1 (0: off)
    float m8C;
    float m90;
    atn::Color4f_unk m94;
};

class ActionCameraMode : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraMode> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
    void fn_8055BB08(CameraFx_unk* fx, Action* arg); // inferred: bound with boost::bind (0x80794D78)
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
        fn_80481818(lbl_8087F540);
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
        fn_80481818(lbl_8087F540);
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
void ActionCameraMode::fn_8055BB08(CameraFx_unk* fx, Action* arg) {
    if (arg->GetParam_unk(1)->m04.i != 0) {
        fx->m88 = arg->GetParam_unk(2)->m04.i + 1;
        fx->m8C = arg->GetParam_unk(3)->m04.f;
        fx->m90 = arg->GetParam_unk(4)->m04.f;
        unsigned int c = arg->GetParam_unk(5)->m04.i;
        fx->m94 = atn::Color4f_unk((1.0f / 255.0f) * ((c >> 16) & 0xFF), (1.0f / 255.0f) * ((c >> 8) & 0xFF),
                                   (1.0f / 255.0f) * (c & 0xFF), (1.0f / 255.0f) * (c >> 24));
    } else {
        fx->m88 = 0;
    }
}

// inferred: speeds of the focus change selected by parameter 5 (pairs of the same value)
static float sFocusSpeed_unk[5][2] = {
    {0.5f, 0.5f}, {1.0f, 1.0f}, {1.5f, 1.5f}, {0.1f, 0.1f}, {0.01f, 0.01f},
};

// NONMATCHING: 96.9% - registers of the speed row (the original indexes with lfsx and keeps the
// parameter 6 pointer in r7)
int ActionCameraFocus::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        fn_80481818(lbl_8087F540);
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
inline CameraAnim_unk* FindCameraAnim_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < manager->m128; i++) {
        CameraAnim_unk* anim = &manager->m124[(unsigned int)i];
        if (id == anim->m14) {
            return anim;
        }
    }
    return 0;
}
inline Xsca_unk* GetCameraAnim_unk(EventManager_unk* manager, int id) {
    CameraAnim_unk* anim = FindCameraAnim_unk(manager, id);
    return anim->m18 != 0 ? (Xsca_unk*)anim->m18->fn_804786F8() : 0;
}

// NONMATCHING: 94.9% - registers of the animation search and order of the int->float conversions of
// the start time (tried casts, a local for the current frame, both groupings)
int ActionCameraMotion::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        fn_80481818(lbl_8087F540);
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
        fn_80481818(lbl_8087F540);
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
