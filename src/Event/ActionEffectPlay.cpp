#include "Event/Behavior.h"
#include "game/GameTask.h"
#include "Event/Manager.h"
#include "Event/MovieState_unk.h"
#include "game/NewStaffRoll.h"
#include "game/GameVoice.h"
#include "game/GameBgm.h"
#include "atn/RenderSetting_unk.h"
#include "atn/Bgm.h"
#include "atn/SoundRef.h"
#include "Event/Cut.h"
#include "std/string.h"
#include <boost/bind.hpp>
#include "game/MapManager.h"
#include "game/EnvSeManager.h"
#include "game/GameUI.h"
#include "atn/D2A_Task.h"
#include "atn/Color4f_unk.h"
#include "atn/Model_unk.h"
#include "Event/Path.h"
#include "atn/SoundManager.h"
#include "game/ColliAttrManager.h"
#include "game/SongManager.h"
#include "game/GameConfig_unk.h"
#include "atn/RenderManager.h"
#include "atn/Sphere.h"
#include "Event/Dialog.h"
#include "game/GameChoiceWindow.h"
#include "game/MapInstance_unk.h"

// Effect, light, sound and message actions of the events: processors with vf08 (per frame) and vf0C
// (phase), as in Event/ActionCameraMode. Class names from the RTTI (the vtables of the last seven
// are in the data after this file's split).

namespace atn {

// inferred: base named in the RTTI of the processors (no member is used here)
template <class T>
class SingletonNoStatic {};
} // namespace atn

class NewStaffRoll;

extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings (frame speed at 0x3A4)
// inferred: color gradation on / off (same arguments)
namespace atn {

struct Camera;
} // namespace atn
typedef atn::Camera Camera_unk;
#include "Event/MessageLayoutPos_unk.h"
// inferred: prints a message of the event (window, text, kind, style, size, flags, start, length, speeds)
// inferred: effect manager lbl_8087F3C0 (only the member used here)
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8; // inferred: owner kind of the next effects (4: events)
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(Event::EventManager_unk* manager, Event::Action* arg); // inferred: owner of the next effects
extern "C" void fn_8023A680(EffectMgr_unk* mgr, void* efp, int a, int b, MtxPtr mtx, const atn::Vector3* pos,
                            const atn::Vector3* rot, int c, int d, int e, int f, float scale); // inferred: plays an efp
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, void* data, atn::Model* model, int a, const atn::Vector3* pos,
                            const atn::Vector3* rot, const atn::Color4f_unk* color, int b, int c, int d, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, Event::EventManager_unk* manager, void* owner, int stop); // inferred: stops
extern "C" unsigned int fn_800DC6B4(const char* str);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A58D0(void* pad, int port); // inferred: classic controller connected
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value); // inferred: sets a layout value
extern "C" void fn_800CB5B4(atn::SoundRef* sound, int frames, float volume); // inferred: sets the volume
// inferred: point light (index, position, radius, color)
// inferred: spot light (position, direction, two angles, color)

// inferred: cloud map shadow (scale, position, rate)
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void* fn_8001260C(Event::Actor* actor);        // inferred: collision of an actor
extern "C" void* lbl_8087EE98;                                // inferred: collision world
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test

// inferred: preset color matrices of ActionColorMatrix (identity, mono, sepia, negative, brightness and
// contrast)
static RsVector4_unk sColorMatrices_unk[12][4] = {
    {RsVector4_unk(1.0f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 1.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 1.0f, 0.0f),
     RsVector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {RsVector4_unk(0.299f, 0.299f, 0.299f, 0.0f), RsVector4_unk(0.587f, 0.587f, 0.587f, 0.0f),
     RsVector4_unk(0.114f, 0.114f, 0.114f, 0.0f), RsVector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {RsVector4_unk(0.393f, 0.349f, 0.272f, 0.0f), RsVector4_unk(0.769f, 0.686f, 0.534f, 0.0f),
     RsVector4_unk(0.189f, 0.168f, 0.131f, 0.0f), RsVector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {RsVector4_unk(-1.0f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, -1.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, -1.0f, 0.0f),
     RsVector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {RsVector4_unk(1.0f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 1.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 1.0f, 0.0f),
     RsVector4_unk(-0.5f, -0.5f, -0.5f, 1.0f)},
    {RsVector4_unk(1.0f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 1.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 1.0f, 0.0f),
     RsVector4_unk(-0.2f, -0.2f, -0.2f, 1.0f)},
    {RsVector4_unk(1.0f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 1.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 1.0f, 0.0f),
     RsVector4_unk(0.2f, 0.2f, 0.2f, 1.0f)},
    {RsVector4_unk(1.0f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 1.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 1.0f, 0.0f),
     RsVector4_unk(0.5f, 0.5f, 0.5f, 1.0f)},
    {RsVector4_unk(0.5f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.5f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 0.5f, 0.0f),
     RsVector4_unk((1.0f - 0.5f) * 0.5f, (1.0f - 0.5f) * 0.5f, (1.0f - 0.5f) * 0.5f, 0.0f)},
    {RsVector4_unk(0.8f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.8f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 0.8f, 0.0f),
     RsVector4_unk((1.0f - 0.8f) * 0.5f, (1.0f - 0.8f) * 0.5f, (1.0f - 0.8f) * 0.5f, 0.0f)},
    {RsVector4_unk(1.2f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 1.2f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 1.2f, 0.0f),
     RsVector4_unk((1.0f - 1.2f) * 0.5f, (1.0f - 1.2f) * 0.5f, (1.0f - 1.2f) * 0.5f, 0.0f)},
    {RsVector4_unk(2.0f, 0.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 2.0f, 0.0f, 0.0f), RsVector4_unk(0.0f, 0.0f, 2.0f, 0.0f),
     RsVector4_unk((1.0f - 2.0f) * 0.5f, (1.0f - 2.0f) * 0.5f, (1.0f - 2.0f) * 0.5f, 0.0f)},
};

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames);          // inferred: fades the sound in
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: 2D sound

extern "C" void* lbl_8087EE90; // inferred: sound engine
extern "C" unsigned int fn_80049B74(void* engine, const char* name); // inferred: sound id of a name
extern "C" int fn_805A38F0(const char* name, int arg2, int arg3);   // inferred: the sound can be played
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume); // 3D sound
extern "C" void fn_800CB654(atn::SoundRef* sound, float a, float b, float c); // inferred: 3D parameters
extern "C" void fn_800CB668(atn::SoundRef* sound, int mode);                    // inferred: loop

extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: starts the movie of the event (name, fade frames, the BGM is stopped...)
extern "C" int fn_800CB4A4(atn::SoundRef* sound);   // inferred: sound state flags
extern "C" int fn_803CE160(int a, int b, int c);

extern "C" int fn_80051B70(const atn::Vector3* from, const atn::Vector3* to); // inferred: visibility test
extern "C" int fn_80531E80(NewStaffRoll* roll);
extern "C" int fn_800A5584(void* pad, int port, int button); // inferred: button pressed
extern "C" int fn_800A55D4(void* pad, int port, int button); // inferred: button held
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);

struct GameVars_unk; // inferred: lbl_8087F430 (only its variable accessors are used here)
extern "C" GameVars_unk* lbl_8087F430;

namespace Event {

// inferred: effect of the event (0x20 bytes, found by id): m1C 2 = efp, otherwise a model effect
typedef EffectData EffectEntry_unk; // inferred: m18 the effect data, m1C its kind

// inferred: effect `id` of the event (as EventManager_unk::FindActor_unk)
inline EffectEntry_unk* FindEffect_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < (int)manager->mE4.size(); i++) {
        EffectEntry_unk* entry = &manager->mE4[(unsigned int)i];
        if (id == entry->m14) {
            return entry;
        }
    }
    return 0;
}

// inferred: 2D animation of the event (0x20 bytes, found by id)
typedef AnimationData AnimEntry_unk; // inferred: m18 the d2anime task, m1C 1 when loaded

// inferred: animation `id` of the event (as EventManager_unk::FindActor_unk)
inline AnimEntry_unk* FindAnim_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < (int)manager->mD4.size(); i++) {
        AnimEntry_unk* entry = &manager->mD4[(unsigned int)i];
        if (id == entry->m14) {
            return entry;
        }
    }
    return 0;
}

// inferred: movie of the event (found by id)
struct MovieRes_unk {
    unsigned char m00[0x4C];
    MovieState_unk* m4C;
};
typedef MovieData MovieEntry_unk;

// inferred: movie `id` of the event (as EventManager_unk::FindActor_unk)
inline MovieEntry_unk* FindMovie_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < (int)manager->m144.size(); i++) {
        MovieEntry_unk* entry = &manager->m144[(unsigned int)i];
        if (id == entry->m14) {
            return entry;
        }
    }
    return 0;
}

// inferred: named entry of the event (found by id): streams of the multi audio actions, locators
typedef StringData AudioEntry_unk; // inferred: m08 the stream name

// inferred: stream `id` of the event (as EventManager_unk::FindActor_unk)
inline AudioEntry_unk* FindAudio_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < (int)manager->m114.size(); i++) {
        AudioEntry_unk* entry = &manager->m114[(unsigned int)i];
        if (id == entry->m14) {
            return entry;
        }
    }
    return 0;
}

} // namespace Event


// inferred: text of the dialog balloons (atn, lbl_8087F008); the type is the one of Event/Dialog.cpp
struct DialogTextMgr_unk;
extern "C" DialogTextMgr_unk* lbl_8087F008;
extern "C" void fn_800DBC1C(DialogTextMgr_unk* mgr); // inferred: shows all the texts at once
extern "C" wchar_t* fn_800DC3C8(wchar_t* s, const wchar_t* delim, wchar_t** context); // inferred: wcstok
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" unsigned long wcslen(const wchar_t* s);

namespace Event {

class ActionColorGradation : public ActionProcessor, public atn::SingletonNoStatic<ActionColorGradation> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionColorMatrix : public ActionProcessor, public atn::SingletonNoStatic<ActionColorMatrix> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
    void fn_80561B50(MapArea* model, void* arg); // inferred: applies the color matrix to a map model
};

class ActionBrightness : public ActionProcessor, public atn::SingletonNoStatic<ActionBrightness> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionLightSurround : public ActionProcessor, public atn::SingletonNoStatic<ActionLightSurround> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionSpotLight : public ActionProcessor, public atn::SingletonNoStatic<ActionSpotLight> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionPointLight : public ActionProcessor, public atn::SingletonNoStatic<ActionPointLight> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCloudMap : public ActionProcessor, public atn::SingletonNoStatic<ActionCloudMap> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
    static atn::Vector2_unk fn_80560FE8(Action* arg); // inferred: position of the cloud map
};

class ActionAmbientLight : public ActionProcessor, public atn::SingletonNoStatic<ActionAmbientLight> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionSunLight : public ActionProcessor, public atn::SingletonNoStatic<ActionSunLight> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionGodRay : public ActionProcessor, public atn::SingletonNoStatic<ActionGodRay> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionMoviePlay : public ActionProcessor, public atn::SingletonNoStatic<ActionMoviePlay> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionMultiAudio : public ActionProcessor, public atn::SingletonNoStatic<ActionMultiAudio> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionVoicePlay : public ActionProcessor, public atn::SingletonNoStatic<ActionVoicePlay> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionAudioEffect : public ActionProcessor, public atn::SingletonNoStatic<ActionAudioEffect> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionMusicPlay : public ActionProcessor, public atn::SingletonNoStatic<ActionMusicPlay> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionEnvSound : public ActionProcessor, public atn::SingletonNoStatic<ActionEnvSound> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionSoundPlay : public ActionProcessor, public atn::SingletonNoStatic<ActionSoundPlay> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionMessagePrint : public ActionProcessor, public atn::SingletonNoStatic<ActionMessagePrint> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionMessageWindow : public ActionProcessor, public atn::SingletonNoStatic<ActionMessageWindow> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionMessageChoice : public ActionProcessor, public atn::SingletonNoStatic<ActionMessageChoice> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionNotify : public ActionProcessor, public atn::SingletonNoStatic<ActionNotify> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCheck : public ActionProcessor, public atn::SingletonNoStatic<ActionCheck> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionAnimationPlay : public ActionProcessor, public atn::SingletonNoStatic<ActionAnimationPlay> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionEffectPlay : public ActionProcessor, public atn::SingletonNoStatic<ActionEffectPlay> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCutAdjust : public ActionProcessor, public atn::SingletonNoStatic<ActionCutAdjust> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionDialogText : public ActionProcessor, public atn::SingletonNoStatic<ActionDialogText> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionDialogSpeaker : public ActionProcessor, public atn::SingletonNoStatic<ActionDialogSpeaker> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionGameSetting : public ActionProcessor, public atn::SingletonNoStatic<ActionGameSetting> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionMapReflect : public ActionProcessor, public atn::SingletonNoStatic<ActionMapReflect> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
    void fn_805628B8(MapArea* model, const void* arg); // inferred: applies parameter 0 to a map model
};

class ActionLocatorHide : public ActionProcessor, public atn::SingletonNoStatic<ActionLocatorHide> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionCameraChange : public ActionProcessor, public atn::SingletonNoStatic<ActionCameraChange> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

// NONMATCHING: first draft
// inferred: degrees to radians
inline float ToRad_unk(float deg) {
    return 0.017453292f * deg;
}

int ActionEffectPlay::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        EventManager_unk* manager = arg->fn_805381C8();
        int effect = arg->GetParam_unk(0)->m04.i;
        int actorId = arg->GetParam_unk(1)->m04.i;
        int node = arg->GetParam_unk(2)->m04.i;
        atn::Vector3 pos;
        pos.x = arg->GetParam_unk(3)->m04.f;
        pos.y = arg->GetParam_unk(4)->m04.f;
        pos.z = arg->GetParam_unk(5)->m04.f;
        float scale = arg->GetParam_unk(6)->m04.f;
        int follow = arg->GetParam_unk(7)->m04.i;
        atn::Vector3 rot;
        rot.x = ToRad_unk(arg->GetParam_unk(8)->m04.f);
        rot.y = ToRad_unk(arg->GetParam_unk(9)->m04.f);
        rot.z = ToRad_unk(arg->GetParam_unk(10)->m04.f);
        if (arg->GetParam_unk(12)->m04.i != 0) {
            ActionParam_unk* keyed = arg->GetParam_unk(13);
            ActionParam_unk* key = arg->GetParam_unk(14);
            if (keyed->m04.i != 0) {
                break;
            }
            manager->fn_805430A0(key->m04.i, effect, arg);
        }
        EffectEntry_unk* entry = FindEffect_unk(manager, effect);
        Actor* actor = manager->FindActor_unk(actorId);
        if (actor->m20 == 6) {
            pos = manager->fn_80541BD8(pos);
        }
        atn::Vector3 base = actor->fn_80010374();
        atn::Vector3 angle = lbl_807C7030;
        angle.x += rot.x;
        angle.y += rot.y;
        angle.z += rot.z;
        atn::Vector3 world;
        world.x = base.x + pos.x;
        world.y = base.y + pos.y;
        world.z = base.z + pos.z;
        atn::Model* model = actor->fn_8001296C();
        MtxPtr mtx = actor->fn_80012AC4(node);
        fn_800DC6B4("[weapon]");
        lbl_8087F3C0->mB8 = 4;
        if (entry->m1C == 2) {
            void* efp = entry->m18;
            fn_80232B7C(manager, arg);
            if (mtx != 0) {
                fn_8023A680(lbl_8087F3C0, efp, -1, 5, mtx, &pos, &angle, 0, follow != 0, -1, 1, scale);
            } else {
                fn_8023A680(lbl_8087F3C0, efp, -1, 0, 0, &world, &angle, 0, 0, -1, 1, scale);
            }
        } else {
            void* data = entry->m18;
            fn_80232B7C(manager, arg);
            atn::Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
            fn_8023A8B4(lbl_8087F3C0, data, model, 0, &pos, &angle, &color, -1, -1, 1, scale);
        }
        lbl_8087F3C0->mB8 = 0;
        break;
    }
    case 3:
    case 4: {
        EventManager_unk* manager = arg->fn_805381C8();
        void* owner = arg;
        int keyed = arg->GetParam_unk(12)->m04.i;
        int key = arg->GetParam_unk(13)->m04.i;
        if (keyed != 0) {
            int id = arg->GetParam_unk(14)->m04.i;
            if (key == 0) {
                break;
            }
            owner = manager->fn_805430BC(id, arg->GetParam_unk(0)->m04.i);
            if (owner == (void*)-1) {
                break;
            }
        }
        if ((arg->m14 - arg->m10 > 0 && keyed == 0) || (keyed != 0 && key == 1)) {
            int keep = arg->GetParam_unk(11)->m04.i;
            lbl_8087F3C0->mB8 = 4;
            fn_80239DAC(lbl_8087F3C0, manager, owner, keep == 0);
            lbl_8087F3C0->mB8 = 0;
        }
        break;
    }
    case 6: {
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(1)->m04.i;
        if (manager->FindActor_unk(id)->m20 == 6) {
            arg->fn_80538DC4(3);
        }
        break;
    }
    }
    return 0;
}

// inferred: sets the layout value called `name` (as atn::D2A_Task::SetLayoutValue_unk of game/GameUI.cpp,
// with the layout taken first)
inline void SetLayoutValue_unk(atn::D2A_Layout_unk* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

int ActionAnimationPlay::vf08(Action* arg) {
    arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    int id = arg->GetParam_unk(0)->m04.i;
    AnimEntry_unk* entry = FindAnim_unk(manager, id);
    if (entry->m1C == 1) {
        atn::D2A_Task* task = entry->m18;
        if (lbl_8087EF70 != 0) {
            int classic = fn_800A58D0(lbl_8087EF70, 0);
            atn::D2A_Layout_unk* layout = &task->mData.m10;
            SetLayoutValue_unk(layout, "contorolClassic", classic != 0 ? 1.0f : 0.0f);
        }
    }
    return 0;
}

// NONMATCHING: 97% - registers of the animation search (id in r5 in the original)
int ActionAnimationPlay::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(0)->m04.i;
        float speed = arg->GetParam_unk(1)->m04.f;
        int repeat = arg->GetParam_unk(2)->m04.i;
        AnimEntry_unk* entry = FindAnim_unk(manager, id);
        if (entry->m1C == 1) {
            atn::D2A_Task* task = entry->m18;
            if ((task->mFlags & 4) == 4) {
                task->mFlags &= ~4;
            }
            if (speed >= 0.0f) {
                task->Rewind_unk();
            } else {
                task->SetEnd_unk();
            }
            task->mSpeed = speed;
            task->mRepeat = repeat;
        }
        break;
    }
    case 3:
    case 4: {
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        AnimEntry_unk* entry = FindAnim_unk(manager, arg->GetParam_unk(0)->m04.i);
        if (entry->m1C == 1) {
            atn::D2A_Task* task = entry->m18;
            if ((task->mFlags & 4) != 4) {
                task->mFlags |= 4;
            }
        }
        break;
    }
    }
    return 0;
}

// NONMATCHING: 98.2% - registers of the parameter count and of the label read before the call
int ActionCheck::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0:
        arg->m38 = 0;
        break;
    case 5: {
        EventManager_unk* manager = arg->fn_805381C8();
        if (arg->GetParam_unk(0)->m04.i == 0) {
            manager->fn_8054136C(arg->GetParam_unk(1)->m04.i);
            return 1;
        }
    }
    case 3:
    case 4: {
        int type = arg->GetParam_unk(0)->m04.i;
        if (type == 2 || type == 3) {
            int flags = arg->m38;
            if (arg->GetParam_unk(6)->m04.i != 0 && ((flags >> 1) & 1) == 0) {
                int label = arg->GetParam_unk(5)->m04.i;
                arg->fn_805381C8()->fn_8054136C(label);
                return 1;
            }
        }
        if (type == 1) {
            lbl_8087F540->fn_804817D4();
            lbl_8087F540->m1EB0 = 1;
        }
        break;
    }
    case 6:
        if (arg->GetParam_unk(0)->m04.i == 1) {
            EventManager_unk* manager = arg->fn_805381C8();
            int id = arg->GetParam_unk(2)->m04.i;
            if (manager->FindActor_unk(id)->m20 == 6) {
                arg->fn_80538DC4(4);
            }
        }
        break;
    }
    return 0;
}

// inferred: stick axes of the checks of type 1 (any) and 3 (by parameter 1)
static int sCheckAxes_unk[2] = {0, 1};
static int sStickAxes_unk[6] = {4, 5, 6, 7, 8, 9};

// inferred: state of the check of type 2 and 3 kept in Action::m38
union CheckState_unk {
    int mWord;
    struct {
        unsigned int mCount : 16; // inferred: inputs counted
        unsigned int mWait : 14;  // inferred: frames without input (types 2 and 3) or held (type 2, mode 4)
        unsigned int mDone : 1;
        unsigned int mDir : 1;    // inferred: last stick direction (type 3)
    } s;
};

// NONMATCHING: 93.9% - first draft: the screen size of the sphere (d1/d2), the eye and direction copies,
// the copy of the sphere into m4C and a signed compare of the stick direction still differ
int ActionCheck::vf08(Action* arg) {
    arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    switch (arg->GetParam_unk(0)->m04.i) {
    case 0:
        manager->fn_8054136C(arg->GetParam_unk(1)->m04.i);
        return 1;
    case 1: {
        manager->m98 &= ~0x40000;
        atn::Sphere sphere;
        sphere.mCenter = arg->fn_805384CC(2);
        sphere.mRadius = arg->GetParam_unk(7)->m04.f;
        atn::Vector3 center = atn::RenderManager::instance_->GetScreenPosition(sphere.mCenter);
        atn::Vector3 top = atn::RenderManager::instance_->GetScreenPosition(sphere.mCenter + atn::Vector3(0.0f, sphere.mRadius, 0.0f));
        atn::Vector3 bottom = atn::RenderManager::instance_->GetScreenPosition(sphere.mCenter + atn::Vector3(0.0f, -sphere.mRadius, 0.0f));
        atn::Vector3 d1(center.x - top.x, center.y - top.y, 0.0f);
        float size = PSVECMag(&d1);
        atn::Vector3 d2 = center - bottom;
        if (size > PSVECMag(&d2)) {
            size = PSVECMag(&d2);
        }
        sphere.mRadius += 0.8f * (50.0f * sphere.mRadius / size - sphere.mRadius);
        lbl_8087F540->m1EA8 = 0;
        lbl_8087F540->fn_80481800(1);
        lbl_8087F540->m1E94 = sphere;
        atn::RenderManager::instance_->m54964 = 1;
        atn::Vector3 eye;
        eye.Assign_unk(lbl_8087F540->mCameras[lbl_8087F540->m1A38].mPos);
        atn::Vector3 dir = lbl_8087F540->mCameras[lbl_8087F540->m1A38].mTarget - eye;
        PSVECNormalize(&dir, &dir);
        dir = dir * 800.0f + eye; // inferred: unused (only the stores are left)
        if (fn_80051B70(&eye, &sphere.mCenter) == 0 && lbl_8087F540->m2448 == 0) {
            break;
        }
        lbl_8087F540->m1EA8 = 1;
        int still = 1;
        for (unsigned int i = 0; i < 2; i++) {
            if ((float)__fabs(fn_800A56A8(lbl_8087EF70, 0, sCheckAxes_unk[i], 0)) < 0.3f) {
                continue;
            }
            still = 0;
            break;
        }
        unsigned int frames = arg->m38;
        if (still != 0) {
            frames++;
            if (frames > 3) {
                manager->fn_8054136C(arg->GetParam_unk(1)->m04.i);
                lbl_8087F540->fn_804817D4();
                lbl_8087F540->fn_80481800(0);
                lbl_8087F540->m4C = sphere;
                lbl_8087F540->m23B4 = 1;
                return 1;
            }
        } else {
            frames = 0;
        }
        arg->m38 = frames;
        break;
    }
    case 2: {
        manager->m98 &= ~0x40000;
        int button = arg->GetParam_unk(1)->m04.i;
        int mode = arg->GetParam_unk(2)->m04.i;
        unsigned int count = arg->GetParam_unk(3)->m04.i;
        unsigned int limit = arg->GetParam_unk(4)->m04.i;
        if (count == 0) {
            count = 1;
        }
        CheckState_unk state;
        state.mWord = arg->m38;
        if (mode == 0) {
            if (fn_800A555C(lbl_8087EF70, 0, button) != 0 || lbl_8087F540->m2448 != 0) {
                state.s.mCount++;
            }
        } else if (mode == 1) {
            if (fn_800A555C(lbl_8087EF70, 0, button) != 0 || lbl_8087F540->m2448 != 0) {
                state.s.mCount++;
            } else {
                state.s.mWait++;
            }
            if (state.s.mWait >= limit) {
                state.s.mCount = state.s.mWait = 0;
            }
        } else if (mode == 2) {
            if (fn_800A5584(lbl_8087EF70, 0, button) != 0 || lbl_8087F540->m2448 != 0) {
                state.s.mCount++;
            }
        } else if (mode == 3) {
            if (fn_800A5584(lbl_8087EF70, 0, button) != 0 || lbl_8087F540->m2448 != 0) {
                state.s.mCount++;
            } else {
                state.s.mWait++;
            }
            if (state.s.mWait >= limit) {
                state.s.mCount = state.s.mWait = 0;
            }
        } else if (mode == 4) {
            if (fn_800A55D4(lbl_8087EF70, 0, button) != 0 || lbl_8087F540->m2448 != 0) {
                state.s.mWait++;
            } else {
                state.s.mWait = 0;
            }
            if (state.s.mWait >= limit) {
                state.s.mCount++;
            }
        }
        if (state.s.mCount >= count) {
            if (manager->m190 != 0x151E ||
                (lbl_8087F540->m2400 != 0 && fn_80531E80(lbl_8087F540->m2400) != 0)) {
                state.s.mDone = 1;
            }
        }
        arg->m38 = state.mWord;
        int keep = arg->GetParam_unk(6)->m04.i;
        if (state.s.mDone == 1 && keep == 0) {
            manager->fn_8054136C(arg->GetParam_unk(5)->m04.i);
            return 1;
        }
        break;
    }
    case 3: {
        manager->m98 &= ~0x40000;
        int axis = arg->GetParam_unk(1)->m04.i;
        int mode = arg->GetParam_unk(2)->m04.i;
        unsigned int count = arg->GetParam_unk(3)->m04.i;
        unsigned int limit = arg->GetParam_unk(4)->m04.i;
        if (count == 0) {
            count = 1;
        }
        CheckState_unk state;
        state.mWord = arg->m38;
        if (mode == 0) {
            int moved = 0;
            float value = fn_800A56A8(lbl_8087EF70, 0, sStickAxes_unk[axis], 0);
            if ((float)__fabs(value) > 0.1f) {
                bool dir = !(value < 0.0f);
                if (dir != state.s.mDir) {
                    moved = 1;
                    state.s.mDir = dir == true;
                }
            }
            if (moved != 0 || lbl_8087F540->m2448 != 0) {
                state.s.mCount++;
            } else {
                state.s.mWait++;
            }
            if (state.s.mWait >= limit) {
                state.s.mCount = state.s.mWait = 0;
            }
        }
        if (state.s.mCount >= count) {
            state.s.mDone = 1;
        }
        arg->m38 = state.mWord;
        int keep = arg->GetParam_unk(6)->m04.i;
        if (state.s.mDone == 1 && keep == 0) {
            manager->fn_8054136C(arg->GetParam_unk(5)->m04.i);
            return 1;
        }
        break;
    }
    }
    return 0;
}

int ActionNotify::vf0C(Action* arg, int phase) {
    if (phase == 5 || phase == 0) {
        if (lbl_8087F430 == 0) {
            return 2;
        }
        int id = arg->fn_805381C8()->m1AC;
        if (id == -1) {
            return 2;
        }
        int value = ((GameTask*)lbl_8087F430)->fn_80370A78(id);
        int result = value;
        int op = arg->GetParam_unk(0)->m04.i;
        int operand = arg->GetParam_unk(1)->m04.i;
        if (op == 0) {
            result = operand;
        } else if (op == 1) {
            result = value + operand;
        } else if (op == 2) {
            result = value - operand;
        } else if (op == 3) {
            result = value * operand;
        } else if (op == 4) {
            result = value / operand;
        }
        ((GameTask*)lbl_8087F430)->fn_80370AE4(id, result);
    }
    return 0;
}

int ActionMessageChoice::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        if (lbl_8087F540->m238C > 0) {
            break;
        }
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        GameMessage* msg = manager->m1D4.fn_803B3B38(arg->GetParam_unk(0)->m04.i);
        manager->m1EC->fn_803B58D8(msg);
        break;
    }
    case 3:
    case 4: {
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        manager->fn_8054136C(arg->GetParam_unk(4)->m04.i);
        manager->m1EC->fn_803B57EC();
        break;
    }
    }
    return 0;
}

int ActionMessageChoice::vf08(Action* arg) {
    GameMessageWindow* window;
    arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    window = manager->m1EC;
    if (window->mChoices->fn_803B4C24()) {
        int labels[3];
        labels[0] = arg->GetParam_unk(1)->m04.i;
        labels[1] = arg->GetParam_unk(2)->m04.i;
        labels[2] = arg->GetParam_unk(3)->m04.i;
        unsigned int answer = window->mChoices->mChoice;
        if (answer <= 2) {
            manager->fn_8054136C(labels[answer]);
        }
        window->fn_803B57EC();
    }
    return 0;
}

int ActionMessageWindow::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        if (lbl_8087F540->m238C > 0) {
            break;
        }
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        GameMessage* msg = manager->m1D4.fn_803B3B38(arg->GetParam_unk(0)->m04.i);
        GameMessageWindow* window = manager->m1EC;
        if (window->IsShown_unk()) {
            window->fn_803B5830(msg, 0);
        } else {
            window->fn_803B5774(msg, 0);
        }
        break;
    }
    case 3:
    case 4: {
        arg->fn_805381A0();
        GameMessageWindow* window = arg->fn_805381C8()->m1EC;
        if (arg->GetParam_unk(1)->m04.i == 0 && window->IsShown_unk()) {
            window->fn_803B57B0(0);
        }
        break;
    }
    }
    return 0;
}

int ActionMessagePrint::vf08(Action* arg) {
    return 0;
}

int ActionMessagePrint::vf0C(Action* arg, int phase) {
    if (phase == 0 && lbl_8087F540->m238C <= 0) {
        int window;
        int text;
        unsigned int kind;
        int style;
        int size;
        int wait;
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        window = arg->GetParam_unk(0)->m04.i;
        text = arg->GetParam_unk(1)->m04.i;
        kind = arg->GetParam_unk(4)->m04.i;
        style = arg->GetParam_unk(5)->m04.i;
        size = arg->GetParam_unk(6)->m04.i;
        wait = arg->GetParam_unk(7)->m04.i;
        float speed0 = 0.5f;
        float speed1 = 0.5f;
        if (kind <= 2) {
            speed0 = lbl_807C8AB0[kind].x;
            speed1 = lbl_807C8AB0[kind].y;
        }
        GameMessage* msg = manager->m1D4.fn_803B3B38(text);
        if (kind == 0 && style == 0 && manager->m50 == 3 && lbl_8087F490 != 0 && lbl_8087F490->m2640 != 0) {
            lbl_8087F490->fn_803E3BE8(msg, arg->m14 - arg->m10, 0.0f);
        } else {
            int flags = 1;
            if (wait != 0) {
                flags |= 0x01000000;
            }
            int start = arg->m10;
            manager->fn_80541548(window, msg, kind, style, size, flags, start, arg->m14 - start, speed0, speed1);
        }
    }
    return 0;
}

// inferred: sound slot `n` of the event
inline atn::SoundRef* GetSound_unk(EventManager_unk* manager, unsigned int n) {
    return n <= 3 ? &manager->m174[n] : 0;
}

// inferred: the sound object of `sound` with one more reference (kept in Action::m38)
inline atn::SoundObject* Keep_unk(atn::SoundRef& sound) {
    atn::SoundObject* p = sound.mPtr;
    if (p != 0) {
        p->mRefCount++;
    } else {
        p = 0;
    }
    return p;
}

// NONMATCHING: 99.0% - the kept sound object is in r27 in the original (r30 here; tried an inline
// helper with early returns)
int ActionSoundPlay::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        atn::SoundObject* handle = 0;
        if (lbl_8087F540->mC4 == 0) {
            arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            AudioEntry_unk* entry = FindAudio_unk(manager, arg->GetParam_unk(0)->m04.i);
            int is3D = arg->GetParam_unk(1)->m04.i;
            atn::Vector3 pos = arg->fn_805384CC(2);
            float volume = arg->GetParam_unk(7)->m04.f;
            float range = arg->GetParam_unk(8)->m04.f;
            float pitch = arg->GetParam_unk(9)->m04.f;
            int noKeep = arg->GetParam_unk(10)->m04.i;
            int loop = arg->GetParam_unk(11)->m04.i;
            if (entry != 0) {
                if (entry->m08 == "SONG_PLAY") {
                    if (SongManager::sInstance != 0) {
                        SongManager::sInstance->mF4 = arg->m18;
                        SongManager::sInstance->fn_805A2FD4(1);
                        SongManager::sInstance->mF4 = 0;
                    }
                    handle = 0;
                } else if (entry->m08 == "SONG_STOP") {
                    if (SongManager::sInstance != 0) {
                        SongManager::sInstance->mF4 = arg->m1C;
                        SongManager::sInstance->fn_805A2FD4(0);
                        SongManager::sInstance->mF4 = 0;
                    }
                    handle = 0;
                } else {
                    unsigned int id = lbl_8087EE90 != 0 ? fn_80049B74(lbl_8087EE90, entry->m08.c_str()) : 0;
                    if (id != 0 && (id == fn_80049B74(lbl_8087EE90, "SE_EV0605_001") ||
                                    id == fn_80049B74(lbl_8087EE90, "SE_EV2527_001"))) {
                        lbl_8087EFE8->m34D0 = 5;
                    } else {
                        lbl_8087EFE8->m34D0 = 3;
                    }
                    atn::SoundRef sound;
                    if (fn_805A38F0(entry->m08.c_str(), 3, 0) != 0) {
                        if (is3D != 0) {
                            sound = fn_800C344C(entry->m08.c_str(), &pos, 0, -1, volume);
                            fn_800CB654(&sound, range, 1.0f, 1.0f);
                        } else {
                            sound = fn_800C31F4(entry->m08.c_str(), 0, -1, volume);
                        }
                    }
                    lbl_8087EFE8->m34D0 = 0;
                    if (arg->m18 > 0) {
                        fn_800CB518(&sound, arg->m18);
                    }
                    if (loop != 0) {
                        fn_800CB668(&sound, 2);
                    }
                    fn_800CB688(&sound, 0, pitch);
                    if (noKeep != 0) {
                        handle = 0;
                    } else {
                        handle = Keep_unk(sound);
                    }
                }
            }
        }
        arg->m38 = (int)handle;
        break;
    }
    case 2: {
        bool stop = arg->GetParam_unk(10)->m04.i == 0 && arg->m38 != 0;
        if (stop) {
            atn::SoundRef sound;
            sound = (atn::SoundObject*)arg->m38;
            sound.mPtr->mRefCount--;
            fn_800CB5C8(&sound, arg->m1C, 0);
        }
        break;
    }
    case 6: {
        EventManager_unk* manager = arg->fn_805381C8();
        if (manager->FindActor_unk(arg->GetParam_unk(2)->m04.i)->m20 == 6) {
            arg->fn_80538DC4(4);
        }
        break;
    }
    }
    return 0;
}

int ActionSoundPlay::vf08(Action* arg) {
    if (arg->m38 != 0) {
        atn::SoundRef sound;
        sound = (atn::SoundObject*)arg->m38;
        sound.mPtr->mRefCount--;
        int frame = arg->fn_805381C8()->m198;
        float volume = arg->GetParam_unk(7)->m04.f;
        fn_800CB5B4(&sound, 0, volume * arg->fn_8053A2E8(arg->fn_805393A4(frame)));
    }
    return 0;
}

// NONMATCHING: 98.1% - the original keeps arg in r30 and the entry in r31 (tried the id in a local and
// a switch on the type)
int ActionEnvSound::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        EventManager_unk* manager = arg->fn_805381C8();
        int type = arg->GetParam_unk(0)->m04.i;
        if (type == 0) {
            if (lbl_8087F540->mC4 == 0) {
                int slot = arg->GetParam_unk(1)->m04.i;
                AudioEntry_unk* entry = FindAudio_unk(manager, arg->GetParam_unk(2)->m04.i);
                float volume = arg->GetParam_unk(3)->m04.f;
                float unused = arg->GetParam_unk(4)->m04.f;
                float pitch = arg->GetParam_unk(5)->m04.f;
                atn::SoundRef* sound = GetSound_unk(manager, slot);
                if (entry != 0 && sound != 0) {
                    fn_800CB5C8(sound, 0, 0);
                    lbl_8087EFE8->m34D0 = 3;
                    lbl_8087EFE8->m34C8 = 0;
                    *sound = fn_800C31F4(entry->m08.c_str(), 0, -1, volume);
                    lbl_8087EFE8->m34C8 = 1;
                    lbl_8087EFE8->m34D0 = 0;
                    if (arg->m18 > 0) {
                        fn_800CB518(sound, arg->m18);
                    }
                    fn_800CB688(sound, 0, pitch);
                }
            }
        } else if (type == 1) {
            atn::SoundRef* sound = GetSound_unk(manager, arg->GetParam_unk(1)->m04.i);
            if (sound != 0) {
                fn_800CB5C8(sound, arg->m1C, 0);
            }
        } else if (type == 2) {
            int on = arg->GetParam_unk(1)->m04.i;
            if (lbl_8087F418 != 0) {
                lbl_8087F418->fn_80360780(on, 4, arg->m18 + arg->m1C);
            }
        } else if (type == 3) {
            int on = arg->GetParam_unk(1)->m04.i;
            if (lbl_8087F418 != 0) {
                lbl_8087F418->fn_803606CC(on, 4, arg->m18 + arg->m1C);
            }
        } else if (type == 4) {
            int enabled = arg->GetParam_unk(1)->m04.i;
            if (ColliAttrManager::sInstance != 0) {
                ColliAttrManager::sInstance->mEnabled = enabled;
            }
        }
    }
    return 0;
}

// NONMATCHING: 89.8% - load order of GameBgm::sInstance and the action length (tried locals for the
// instance, the length and the parameter)
int ActionMusicPlay::vf0C(Action* arg, int phase) {
    if (phase == 5 || phase == 0) {
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(0)->m04.i;
        float volume = arg->GetParam_unk(1)->m04.f;
        int fade = arg->m18 + arg->m1C;
        if (GameBgm::sInstance != 0) {
            if (lbl_8087F540->mC4 != 0) {
                if (manager->m190 == 0x5E6) {
                    fade /= 2;
                } else {
                    fade = fade * 2 / 3;
                }
            }
            GameBgm::sInstance->fn_8037D4C0(id, fade, 1, 0, volume);
        }
    }
    return 0;
}

int ActionAudioEffect::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        arg->fn_805381A0();
        arg->fn_805381C8();
        int reverb = arg->GetParam_unk(0)->m04.i;
        if (lbl_8087F418 != 0) {
            lbl_8087F418->fn_803601E4(reverb, 1.0f);
        }
    }
    return 0;
}

// inferred: the sound engine knows `name` (it returns index + 1, as in atn/Se_unk fn_800C3094)
inline bool HasSound_unk(const char* name) {
    return fn_80049B74(lbl_8087EE90, name) - 1 != -1;
}

// inferred: plays the voice of an actor: with lip sync on its character (or on the balloon head when
// the event shows dialog heads, flag 0x00800000), or without when it has none
// NONMATCHING: 97.4% - arg in r27 (r28 in the original) and the long-string test of size() is shared
// with c_str() here (the original tests it again); tried size()/length()/empty() forms and Keep_unk
int ActionVoicePlay::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        AudioEntry_unk* entry = FindAudio_unk(manager, arg->GetParam_unk(0)->m04.i);
        if (entry == 0) {
            break;
        }
        if (entry->m08.size() == 0) {
            break;
        }
        if (lbl_8087EE90 != 0 && !HasSound_unk(entry->m08.c_str())) {
            break;
        }
        atn::Vector3 pos = arg->fn_805384CC(2);
        float volume = arg->GetParam_unk(7)->m04.f;
        float range = arg->GetParam_unk(8)->m04.f;
        float pitch = arg->GetParam_unk(9)->m04.f;
        int noKeep = arg->GetParam_unk(10)->m04.i;
        int lipSync = arg->GetParam_unk(11)->m04.i;
        if (lbl_8087F498->fn_803EB2A4(entry->m08.c_str()) == 0) {
            lbl_8087F540->fn_80489A2C();
        }
        if (lbl_8087F0A8->mVoiceLimitLevel >= 3) {
            volume = 0.0f;
        }
        atn::SoundRef sound;
        Actor* actor = manager->FindActor_unk(arg->GetParam_unk(2)->m04.i);
        CharaTask* chara = actor != 0 ? actor->fn_800128FC() : 0;
        if (((manager->m98 >> 23) & 1) == 1) {
            chara = manager->m280->fn_8022960C(actor);
        }
        if (entry != 0) {
            if (lipSync != 0 && chara != 0) {
                sound = lbl_8087F498->fn_803EA09C(chara, entry->m08.c_str(), 0, 1, 0, volume);
            } else {
                sound = lbl_8087F498->fn_803E9608(entry->m08.c_str(), 0, 1, volume);
            }
        }
        lbl_8087F540->fn_80489A50(arg, 0);
        if (arg->m18 > 0) {
            fn_800CB518(&sound, arg->m18);
        }
        fn_800CB688(&sound, 0, pitch);
        if (lbl_8087F540->mC4 != 0) {
            fn_800CB5B4(&sound, 10, 0.0f);
        }
        atn::SoundObject* handle = 0;
        if (noKeep == 0 && sound.mPtr != 0) {
            sound.mPtr->mRefCount++;
            handle = sound.mPtr;
        }
        arg->m38 = (int)handle;
        break;
    }
    case 2: {
        bool stop = arg->GetParam_unk(10)->m04.i == 0 && arg->m38 != 0;
        if (stop) {
            atn::SoundRef sound;
            sound = (atn::SoundObject*)arg->m38;
            sound.mPtr->mRefCount--;
            fn_800CB5C8(&sound, arg->m1C, 0);
        }
        break;
    }
    case 6: {
        EventManager_unk* manager = arg->fn_805381C8();
        if (manager->FindActor_unk(arg->GetParam_unk(2)->m04.i)->m20 == 6) {
            arg->fn_80538DC4(4);
        }
        break;
    }
    }
    return 0;
}

int ActionMultiAudio::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        if (arg->GetParam_unk(0)->m04.i != 0) {
            break;
        }
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(1)->m04.i;
        float volume = arg->GetParam_unk(2)->m04.f;
        AudioEntry_unk* entry = FindAudio_unk(manager, id);
        if (entry != 0) {
            lbl_8087F540->fn_80488050(manager, arg);
            int fade = 60;
            if (arg->m18 > 0) {
                fade = arg->m18;
            }
            if (lbl_8087F540->mC4 == 0) {
                lbl_8087F540->fn_80488A74(entry->m08.c_str(), fade, volume);
            }
        }
        break;
    }
    case 3:
        if (arg->GetParam_unk(0)->m04.i != 0) {
            lbl_8087F540->fn_80488D80(arg->m1C);
        }
        break;
    }
    return 0;
}

int ActionMultiAudio::vf08(Action* arg) {
    if (arg->GetParam_unk(0)->m04.i == 0) {
        int busy = 0;
        if (lbl_8087F540->m1F34 != 0 && lbl_8087F540->m1F20.fn_8004B0E4() != 0) {
            busy = 1;
        }
        if (busy) {
            return 3;
        }
    }
    return 0;
}

// NONMATCHING: 98.3% - registers of the id and the entry, the BGM sound is tested through lwzu, and the
// 1.0f of phase 2 is loaded before the virtual (tried the entry declared first)
int ActionMoviePlay::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(0)->m04.i;
        int loop = arg->GetParam_unk(2)->m04.i;
        float volume = arg->GetParam_unk(3)->m04.f;
        MovieEntry_unk* entry = FindMovie_unk(manager, id);
        // inferred: the player seen through its own view (atn::VideoTHP declares vf3C without arguments)
        MovieState_unk* movie = entry != 0 ? (MovieState_unk*)entry->m18->m4C : 0;
        if (movie != 0) {
            if (loop != 0) {
                movie->m0C = 1;
            } else {
                movie->m0C = 0;
            }
            movie->vf3C(arg->m18, volume);
            int stopBgm = 1;
            GameBgm* bgm = GameBgm::sInstance;
            if (bgm != 0) {
                int next = bgm->m84 != 0 ? bgm->mNext.mId : bgm->mReq.mId;
                if (next > 0 && bgm->mCur != 0 && bgm->mCur->mSound.mPtr != 0 &&
                    fn_800CB4A4(&bgm->mCur->mSound) == 0 && fn_803CE160(0, 0, 0) == 0) {
                    stopBgm = 0;
                }
            }
            if (manager->m190 == 0x151F) {
                stopBgm = 0;
            }
            lbl_8087F540->fn_804881B8(manager, "", 10, stopBgm, 1, 1, 1, 1.0f, 1, 0x400000);
            if (manager->m190 == 0x5E6 && GameBgm::sInstance != 0) {
                if (GameBgm::sInstance->mCur != 0) {
                    GameBgm::sInstance->mCur->fn_8004B1EC(0);
                }
                if (GameBgm::sInstance->mPrev != 0) {
                    GameBgm::sInstance->mPrev->fn_8004B1EC(0);
                }
            }
        }
        lbl_8087F540->fn_8048E9A4(movie, 10);
        break;
    }
    case 2:
        if (arg->m1C > 0) {
            EventManager_unk* manager = arg->fn_805381C8();
            MovieEntry_unk* entry = FindMovie_unk(manager, arg->GetParam_unk(0)->m04.i);
            if (entry != 0 && entry->m18->m4C != 0) {
                ((MovieState_unk*)entry->m18->m4C)->vf3C(arg->m1C, 1.0f);
            }
        }
        break;
    case 3:
    case 4:
    case 5: {
        EventManager_unk* manager = arg->fn_805381C8();
        MovieEntry_unk* entry = FindMovie_unk(manager, arg->GetParam_unk(0)->m04.i);
        if (entry != 0) {
            MovieState_unk* movie = (MovieState_unk*)entry->m18->m4C;
            if (movie != 0 && movie->m08 == 4) {
                movie->m08 = 1;
            }
        }
        lbl_8087F540->fn_8048EC7C();
        if (lbl_8087F0A8->m194 != 0 && manager->m190 == 0x19D) {
            ((GameTask*)lbl_8087F430)->fn_80370B78(-1, 0, 0, 1.0f);
            manager->fn_8053E8C8(11);
        } else {
            manager->fn_8053E8C8(13);
        }
        break;
    }
    }
    return 0;
}

int ActionMoviePlay::vf08(Action* arg) {
    EventManager_unk* manager = arg->fn_805381C8();
    int id = arg->GetParam_unk(0)->m04.i;
    MovieEntry_unk* entry = FindMovie_unk(manager, id);
    if (!lbl_8087F540->mMoviePlaying) {
        bool fading = lbl_8087F540->m1F6C > 0.0f;
        if (!fading) {
            lbl_8087F540->fn_8048E9E0();
        } else {
            return 3;
        }
    } else if (entry != 0) {
        MovieState_unk* state = (MovieState_unk*)entry->m18->m4C;
        if (state != 0 && state->m08 == 2) {
            vf0C(arg, 3);
        }
    }
    return 0;
}

int ActionGodRay::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        int on = arg->GetParam_unk(0)->m04.i;
        float value = arg->GetParam_unk(1)->m04.f;
        lbl_8087F540->fn_80486BEC(on, value);
    }
    return 0;
}

int ActionSunLight::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        arg->fn_805381C8();
        atn::Vector3 dir = arg->fn_80538B18(2);
        lbl_8087F540->fn_804846FC(dir, arg->GetParam_unk(5)->m04.i);
        break;
    }
    case 6:
        arg->fn_80538FD4(2);
        break;
    }
    return 0;
}

int ActionAmbientLight::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        lbl_8087F540->fn_80484A78(arg->GetParam_unk(1)->m04.i);
    }
    return 0;
}

// inferred: position of the cloud map: actor of parameter 0 (node of parameter 1) plus the offset of
// parameters 2-4, seen from above
// NONMATCHING: 99.8% - r3/r4 swapped in the copy of the node position (tried a Vector3 built from the
// matrix, GetTrans_unk on MtxPtr and on a Matrix3x4 pointer)
atn::Vector2_unk ActionCloudMap::fn_80560FE8(Action* arg) {
    int id;
    EventManager_unk* manager = arg->fn_805381C8();
    id = arg->GetParam_unk(0)->m04.i;
    unsigned int node = arg->GetParam_unk(1)->m04.i;
    atn::Vector3 offset = arg->fn_805381F0(2);
    float unused = arg->GetParam_unk(5)->m04.f;
    Actor* actor = manager->FindActor_unk(id);
    if (actor == 0) {
        return atn::Vector2_unk(0.0f, 0.0f);
    }
    atn::Vector3 pos = actor->fn_80010374();
    if (actor->m20 == 6) {
        offset = manager->fn_80541BD8(offset);
    }
    atn::Model* model = actor->fn_8001296C();
    if (model != 0) {
        atn::Matrix3x4* mtx = (atn::Matrix3x4*)model->GetNodeMtx_unk(model->fn_800928B0(node, 0));
        if (mtx != 0) {
            pos = mtx->GetTrans_unk();
        }
    }
    pos += offset;
    atn::Vector2_unk ret(pos.x, pos.z);
    return ret;
}

int ActionCloudMap::vf08(Action* arg) {
    float scale = arg->GetParam_unk(5)->m04.f;
    atn::Vector2_unk pos = fn_80560FE8(arg);
    atn::Vector2_unk size(scale, scale);
    lbl_8087F540->fn_80484D64(size, pos);
    return 0;
}

// inferred: 1 / frames (1 without frames)
inline float Rate_unk(int frames) {
    return frames > 0 ? 1.0f / frames : 1.0f;
}

// NONMATCHING: 93.1% - the manager for the m1F0C store goes in r5 and is scheduled before the copies of
// the arguments
int ActionCloudMap::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        float scale = arg->GetParam_unk(5)->m04.f;
        atn::Vector2_unk pos = fn_80560FE8(arg);
        atn::Vector2_unk size(scale, scale);
        float rate = Rate_unk(arg->m18);
        size.x *= rate;
        size.y *= rate;
        lbl_8087F540->fn_80484BF4();
        lbl_8087F540->m1F0C = 1;
        lbl_8087F540->fn_80484D40(size, pos, rate);
        break;
    }
    case 2:
        if (arg->m1C > 0) {
            float scale = arg->GetParam_unk(5)->m04.f;
            atn::Vector2_unk pos = fn_80560FE8(arg);
            atn::Vector2_unk size(scale, scale);
            float rate = Rate_unk(arg->m1C);
            lbl_8087F540->m1F0C = 0;
            lbl_8087F540->fn_80484D40(size, pos, rate);
        }
        break;
    case 3:
        lbl_8087F540->fn_80484E88();
        break;
    }
    return 0;
}

int ActionPointLight::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        int index = arg->GetParam_unk(0)->m04.i;
        atn::Vector3 pos = arg->fn_805384CC(1);
        float radius = arg->GetParam_unk(6)->m04.f;
        lbl_8087F540->fn_80485028(index, pos, radius, arg->GetParam_unk(7)->m04.i);
        break;
    }
    case 6: {
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(1)->m04.i;
        if (manager->FindActor_unk(id)->m20 == 6) {
            arg->fn_80538DC4(3);
        }
        break;
    }
    }
    return 0;
}

int ActionSpotLight::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        atn::Vector3 pos = arg->fn_805384CC(0);
        atn::Vector3 dir = arg->fn_80538B18(5);
        float a = arg->GetParam_unk(8)->m04.f;
        float b = arg->GetParam_unk(9)->m04.f;
        lbl_8087F540->fn_80485964(pos, dir, a, b, arg->GetParam_unk(10)->m04.i);
        break;
    }
    case 6: {
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(0)->m04.i;
        if (manager->FindActor_unk(id)->m20 == 6) {
            arg->fn_80538DC4(2);
        }
        break;
    }
    }
    return 0;
}

int ActionLightSurround::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        lbl_8087F540->fn_80486DAC(arg->GetParam_unk(0)->m04.i);
    }
    return 0;
}

int ActionBrightness::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        lbl_8087F540->fn_80487064(arg->GetParam_unk(0)->m04.i);
    }
    return 0;
}

// NONMATCHING: 98.6% - register allocation of the three modes (the original keeps arg in r27 and the
// wait in r30; tried the hit vector at the top of mode 2)
int ActionCameraChange::vf08(Action* arg) {
    int wait = arg->GetParam_unk(5)->m04.i;
    int mode = arg->GetParam_unk(0)->m04.i;
    if (mode == 0) {
        if (fn_800A555C(lbl_8087EF70, 0, 6) != 0) {
            int cameras[4];
            int current = lbl_8087F540->m1A38;
            int last = 3;
            int next = 0;
            for (int i = 0; i < 4; i++) {
                int camera = arg->GetParamAt_unk(i + 1)->m04.i;
                cameras[i] = camera;
                if (current == camera - 1) {
                    next = i;
                }
                if (camera == 0) {
                    last = i;
                    break;
                }
            }
            next++;
            if (next > last) {
                next = 0;
            }
            int camera = cameras[next] - 1;
            lbl_8087F540->m1A38 = camera < 0 ? 0 : camera;
        }
    } else if (mode == 1) {
        int timer = arg->m38;
        if (timer == 0) {
            float best = 3.4028235e+38f;
            int selected = 0;
            for (int i = 0; i < 4; i++) {
                int camera = arg->GetParamAt_unk(i + 1)->m04.i;
                if (camera == 0) {
                    break;
                }
                EventCamera_unk* cam = &lbl_8087F540->mCameras[camera - 1];
                atn::Vector3 dir = cam->mTarget - cam->mPos;
                float dist = PSVECMag(&dir);
                if (dist < best) {
                    best = dist;
                    selected = camera - 1;
                }
            }
            if (selected != lbl_8087F540->m1A38) {
                lbl_8087F540->m1A38 = selected < 0 ? 0 : selected;
                timer = wait;
            }
        } else {
            timer--;
        }
        arg->m38 = timer;
    } else if (mode == 2) {
        int timer = arg->m38;
        if (timer == 0) {
            atn::Vector3 hit;
            int selected = 0;
            for (int i = 0; i < 4; i++) {
                int camera = arg->GetParamAt_unk(i + 1)->m04.i;
                if (camera == 0) {
                    break;
                }
                EventCamera_unk* cam = &lbl_8087F540->mCameras[camera - 1];
                void* ignore = 0;
                if (cam->m240.mActor != 0) {
                    ignore = fn_8001260C(cam->m240.mActor);
                }
                int a = fn_8004ECC0(lbl_8087EE98, &hit, &cam->mPos, &cam->mTarget, 0, ignore, 0);
                int b = fn_8004ECC0(lbl_8087EE98, &hit, &cam->mTarget, &cam->mPos, 0, ignore, 0);
                if (a == 0 && b == 0) {
                    selected = camera - 1;
                    break;
                }
            }
            if (selected != lbl_8087F540->m1A38) {
                lbl_8087F540->m1A38 = selected < 0 ? 0 : selected;
                timer = wait;
            }
        } else {
            timer--;
        }
        arg->m38 = timer;
    }
    return 0;
}

int ActionCameraChange::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0:
        arg->m38 = 0;
        break;
    case 3:
    case 4:
        lbl_8087F540->m1A38 = 0;
        break;
    }
    return 0;
}

void ActionColorMatrix::fn_80561B50(MapArea* model, void* arg) {
    ColorMatrixParam_unk* dst = model->mScene.mColorMatrix.mSet != 0 ? &model->mScene.mColorMatrix.mValue : &lbl_8087EFA8->m324;
    *dst = *(ColorMatrixParam_unk*)arg;
}

int ActionColorMatrix::vf08(Action* arg) {
    EventManager_unk* manager = arg->fn_805381C8();
    if (manager == 0) {
        return 0;
    }
    float frame = manager->m198;
    int start = arg->m10;
    int end = arg->m14;
    float rate;
    if ((float)start <= frame && frame <= (float)(start + arg->m18)) {
        float t = frame - (float)start;
        if (t <= 0.0f) {
            rate = 0.0f;
        } else {
            rate = t / (float)arg->m18;
        }
    } else if ((float)(end - arg->m1C) <= frame && frame <= (float)end) {
        float t = frame - (float)(end - arg->m1C);
        if (t <= 0.0f) {
            rate = 1.0f;
        } else {
            rate = 1.0f - t / (float)arg->m1C;
        }
    } else {
        rate = 1.0f;
    }
    ColorMatrixParam_unk dst;
    dst.mEnabled = 1;
    ColorMatrixParam_unk src;
    src.mEnabled = 1;
    Event::Manager* mgr = lbl_8087F540;
    for (int i = 0; i < 4; i++) {
        src.mRows[i] = mgr->m1D70.mRows[i];
    }
    int type = arg->GetParam_unk(0)->m04.i;
    if (type == 0) {
        for (int i = 0; i < 4; i++) {
            dst.mRows[i] = RsVector4_unk(arg->GetParamAt_unk(i * 3 + 1)->m04.f, arg->GetParamAt_unk(i * 3 + 2)->m04.f,
                                         arg->GetParamAt_unk(i * 3 + 3)->m04.f, 1.0f);
        }
    } else if (type == 1) {
        lbl_8087F540->fn_80484088(&src, rate);
        return 0;
    } else {
        for (int i = 0; i < 4; i++) {
            dst.mRows[i] = sColorMatrices_unk[type - 1][i];
        }
    }
    float inv = 1.0f - rate;
    for (int i = 0; i < 4; i++) {
        dst.mRows[i].x = dst.mRows[i].x * rate + src.mRows[i].x * inv;
        dst.mRows[i].y = dst.mRows[i].y * rate + src.mRows[i].y * inv;
        dst.mRows[i].z = dst.mRows[i].z * rate + src.mRows[i].z * inv;
        dst.mRows[i].w = dst.mRows[i].w * rate + src.mRows[i].w * inv;
    }
    atn::Scene* target = atn::RenderManager::instance_->fn_800C16B4();
    ColorMatrixParam_unk* out = target->mColorMatrix.mSet != 0 ? &target->mColorMatrix.mValue : &lbl_8087EFA8->m324;
    *out = dst;
    MapManager::instance_->fn_80491528(boost::bind(&ActionColorMatrix::fn_80561B50, this, _1, _2), &dst);
    return 0;
}

// NONMATCHING: 99.1% - the four row pointers of the copy into the manager get the registers in reverse
// order (tried a pointer to m1D70 and an assignment after the declaration)
int ActionColorMatrix::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        ColorMatrixParam_unk param = *lbl_8087F540->fn_80483F70();
        param.mEnabled = 1;
        Event::Manager* mgr = lbl_8087F540;
        mgr->m1D70 = param;
    }
    return 0;
}

// NONMATCHING: 99.1% - the original keeps parameter 0 in r0 and the parameter count in r5 (tried
// the arguments inside each call and a parameter pointer)
int ActionColorGradation::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        int on = arg->GetParam_unk(0)->m04.i;
        int a = arg->GetParam_unk(1)->m04.i;
        float r = arg->GetParam_unk(2)->m04.f;
        float g = arg->GetParam_unk(3)->m04.f;
        float b = arg->GetParam_unk(4)->m04.f;
        float c = arg->GetParam_unk(5)->m04.f;
        float d = arg->GetParam_unk(6)->m04.f;
        int e = arg->GetParam_unk(7)->m04.i;
        float f = arg->GetParam_unk(8)->m04.f;
        if (on != 0) {
            lbl_8087F540->fn_8048EE78(a, r, g, b, c, d, e, f);
        } else {
            lbl_8087F540->fn_8048F184(a);
        }
    }
    return 0;
}

int ActionLocatorHide::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        EventManager_unk* manager = arg->fn_805381C8();
        MapTask* map = lbl_8087F540->fn_80480438();
        for (int i = 0; i < 12; i++) {
            AudioEntry_unk* entry = FindAudio_unk(manager, arg->GetParamAt_unk(i)->m04.i);
            if (entry != 0) {
                MapInstance_unk* locator = map->fn_804A0614(entry->m08);
                if (locator != 0) {
                    locator->mHidden = 1;
                }
            }
        }
        break;
    }
    case 3:
    case 4: {
        EventManager_unk* manager = arg->fn_805381C8();
        MapTask* map = lbl_8087F540->fn_80480438();
        for (int i = 0; i < 12; i++) {
            AudioEntry_unk* entry = FindAudio_unk(manager, arg->GetParamAt_unk(i)->m04.i);
            if (entry != 0) {
                MapInstance_unk* locator = map->fn_804A0614(entry->m08);
                if (locator != 0) {
                    locator->mHidden = 0;
                }
            }
        }
        break;
    }
    }
    return 0;
}

void ActionMapReflect::fn_805628B8(MapArea* model, const void* arg) {
    model->mScene.mReflectChara = ((Action*)arg)->GetParam_unk(0)->m04.i;
}

int ActionMapReflect::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        atn::Scene* target = atn::RenderManager::instance_->fn_800C16B4();
        target->mReflectChara = arg->GetParam_unk(0)->m04.i;
        MapManager::instance_->fn_80491528(boost::bind(&ActionMapReflect::fn_805628B8, this, _1, _2), arg);
    }
    return 0;
}

int ActionGameSetting::vf08(Action* arg) {
    return 0;
}

int ActionGameSetting::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        float speed = arg->GetParam_unk(0)->m04.f;
        if (lbl_8087EFA8 != 0) {
            lbl_8087EFA8->m3A4 = speed;
        }
        break;
    }
    case 3:
    case 4:
        if (lbl_8087EFA8 != 0) {
            lbl_8087EFA8->m3A4 = 1.0f;
        }
        break;
    }
    return 0;
}

// inferred: cut `index` of the event (as Event::EventManager_unk::FindActor_unk)
inline Cut* FindCut_unk(EventManager_unk* manager, int index) {
    for (int i = 0; i < (int)manager->mB8.size(); i++) {
        Cut* cut = manager->mB8[(unsigned int)i].get();
        if (index == cut->mIndex) {
            return cut;
        }
    }
    return 0;
}

// inferred: shows the balloon of an actor (slot of the section); the big balloon when the third
// parameter is set
// NONMATCHING: 96.3% - arg and owner swapped (r31/r30 in the original) and the volatile registers of
// the parameters; tried ~13 forms (slot local, early return, switch, frames local, bool flag)
int ActionDialogSpeaker::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        ActionOwner_unk* owner = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Dialog* dialog = manager->m280;
        if (dialog != 0) {
            int id = arg->GetParam_unk(0)->m04.i;
            int kind = arg->GetParam_unk(1)->m04.i;
            int big = arg->GetParam_unk(2)->m04.i;
            int extra = arg->GetParam_unk(3)->m04.i;
            Actor* actor = manager->FindActor_unk(id);
            if (actor != 0) {
                int option = arg->GetParam_unk(4)->m04.i;
                if (big != 0) {
                    dialog->fn_80228AF8(owner->m10, actor, kind, extra, arg->m14 - arg->m10, option);
                } else {
                    dialog->fn_80228AF4(owner->m10, actor, kind, extra, arg->m14 - arg->m10, option);
                }
            }
        }
    }
    return 0;
}

// inferred: message `id` of the event in the balloon of the section, one text per line ("\\n" in the
// message)
static wchar_t sNewLine_unk[] = L"\n";
int ActionDialogText::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        ActionOwner_unk* owner = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Dialog* dialog = manager->m280;
        if (dialog != 0) {
            int id = arg->GetParam_unk(0)->m04.i;
            int align = arg->GetParam_unk(1)->m04.i;
            int size = arg->GetParam_unk(2)->m04.i;
            int param3 = arg->GetParam_unk(3)->m04.i;
            unsigned int color = arg->GetParam_unk(4)->m04.i;
            int mode = arg->GetParam_unk(5)->m04.i;
            float height = arg->GetParam_unk(6)->m04.f;
            float scale = 1.2f * height;
            GameMessage* msg = manager->m1D4.fn_803B3B38(id);
            if (msg != 0) {
                wchar_t text[256];
                wchar_t line[256];
                wcscpy(text, msg->mText);
                int len = wcslen(text);
                wchar_t* p = text;
                for (int i = 0; i < len - 1; i++, p++) {
                    if (p[0] == L'\\' && p[1] == L'n') {
                        p[1] = L'\n';
                        p[0] = L'\n';
                    }
                }
                wchar_t* context;
                int n = 0;
                wchar_t* token = fn_800DC3C8(text, sNewLine_unk, &context);
                while (token != 0) {
                    dialog->fn_80228CA0(owner->m10, token, align, size, param3, (float)(arg->m14 - arg->m10), scale,
                                        color, mode, n);
                    mode = 0;
                    wcscpy(line, token);
                    token = fn_800DC3C8(0, sNewLine_unk, &context);
                    n++;
                }
                if (lbl_8087F540->m2390 != 0) {
                    fn_800DBC1C(lbl_8087F008);
                }
            }
        }
    }
    return 0;
}

int ActionCutAdjust::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int start = arg->m10;
        int length = arg->m14 - start;
        if (length == 0) {
            int index = manager->fn_80541210()->fn_8053B028();
            manager->fn_80541520(manager->fn_80541198(FindCut_unk(manager, index)), 0);
            return 1;
        }
        manager->m198 = start + length;
    }
    return 0;
}

int ActionCutAdjust::vf08(Action* arg) {
    return 0;
}

int ActionDialogText::vf08(Action* arg) {
    return 0;
}

int ActionDialogSpeaker::vf08(Action* arg) {
    return 0;
}

int ActionMapReflect::vf08(Action* arg) {
    return 0;
}

int ActionLocatorHide::vf08(Action* arg) {
    return 0;
}

int ActionColorGradation::vf08(Action* arg) {
    return 0;
}

int ActionBrightness::vf08(Action* arg) {
    return 0;
}

int ActionLightSurround::vf08(Action* arg) {
    return 0;
}

int ActionSpotLight::vf08(Action* arg) {
    return 0;
}

int ActionPointLight::vf08(Action* arg) {
    return 0;
}

int ActionAmbientLight::vf08(Action* arg) {
    return 0;
}

int ActionSunLight::vf08(Action* arg) {
    return 0;
}

int ActionGodRay::vf08(Action* arg) {
    return 0;
}

int ActionVoicePlay::vf08(Action* arg) {
    return 0;
}

int ActionAudioEffect::vf08(Action* arg) {
    return 0;
}

int ActionMusicPlay::vf08(Action* arg) {
    return 0;
}

int ActionEnvSound::vf08(Action* arg) {
    return 0;
}

int ActionMessageWindow::vf08(Action* arg) {
    return 0;
}

int ActionNotify::vf08(Action* arg) {
    return 0;
}

int ActionEffectPlay::vf08(Action* arg) {
    return 0;
}
} // namespace Event
