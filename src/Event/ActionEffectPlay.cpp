#include "Event/Behavior.h"
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

// Effect, light, sound and message actions of the events: processors with vf08 (per frame) and vf0C
// (phase), as in Event/ActionCameraMode. Class names from the RTTI (the vtables of the last seven
// are in the data after this file's split).

namespace atn {

// inferred: base named in the RTTI of the processors (no member is used here)
template <class T>
class SingletonNoStatic {};

} // namespace atn

// inferred: the event manager (lbl_8087F540) seen through the members used here
struct EventEffects_unk {
    unsigned char m00[0xC4];
    int mC4; // inferred: nonzero when the music fades are shortened
    unsigned char mC8[0x1A38 - 0xC8];
    int m1A38; // inferred: camera change in progress
    unsigned char m1A3C[0x1EB0 - 0x1A3C];
    int m1EB0; // inferred: set by ActionCheck (type 1)
    unsigned char m1EB4[0x1F20 - 0x1EB4];
    atn::Bgm m1F20; // inferred: BGM of the multi audio actions
    int m1F34;      // inferred: m1F20 is in use
    unsigned char m1F38[0x1F6C - 0x1F38];
    float m1F6C; // inferred: movie fade time
    unsigned char m1F70[0x238C - 0x1F70];
    int m238C; // inferred: nonzero while messages are blocked
    unsigned char m2390[0x2404 - 0x2390];
    unsigned char mMoviePlaying : 1; // 0x2404
    unsigned char m2404_1 : 7;
};
extern "C" EventEffects_unk* lbl_8087F540;
extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings (frame speed at 0x3A4)
extern "C" void fn_80486BEC(EventEffects_unk* mgr, int on, float value); // inferred: god rays
extern "C" void fn_804846FC(EventEffects_unk* mgr, const atn::Vector3& dir, int frames); // inferred: sun light
extern "C" void fn_80484A78(EventEffects_unk* mgr, int frames);          // inferred: ambient light
extern "C" void fn_80486DAC(EventEffects_unk* mgr, int on);              // inferred: surrounding light
extern "C" void fn_80487064(EventEffects_unk* mgr, int on);              // inferred: brightness
// inferred: color gradation on / off (same arguments)
extern "C" void fn_8048EE78(EventEffects_unk* mgr, int a, float r, float g, float b, float c, float d, int e,
                            float f);
extern "C" void fn_8048F184(EventEffects_unk* mgr, int a, float r, float g, float b, float c, float d, int e,
                            float f);
extern "C" void fn_80488050(EventEffects_unk* mgr, Event::EventManager_unk* manager, Event::Action* arg);
extern "C" void fn_80488A74(EventEffects_unk* mgr, const char* name, int fade, float volume); // inferred: plays a stream
extern "C" void fn_80488D80(EventEffects_unk* mgr, int fade); // inferred: stops the stream
// inferred: map locator shown or hidden by ActionLocatorHide
struct Locator_unk {
    unsigned char m00[0x88];
    unsigned int mHidden : 1; // 0x88
    unsigned int m88_1 : 31;
};
struct MapLocators_unk;
extern "C" MapLocators_unk* fn_80480438(EventEffects_unk* mgr);                      // inferred: map locators
extern "C" Locator_unk* fn_804A0614(MapLocators_unk* map, const std::string& name); // inferred: by name
// inferred: light settings of the camera (as in gmk/CharaLight.cpp)
struct ReflectTarget_unk {
    unsigned char m00[0x120];
    int m120; // inferred: map reflection
};
namespace atn {
struct Camera;
}
typedef atn::Camera Camera_unk;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" ReflectTarget_unk* fn_800C16B4(Camera_unk* camera);
extern "C" void fn_8048E9E0(EventEffects_unk* mgr); // inferred: starts the movie
extern "C" void fn_804817D4(EventEffects_unk* mgr);
extern "C" float lbl_807C8AB0[3][2]; // inferred: text speeds of the message actions
// inferred: prints a message of the event (window, text, kind, style, size, flags, start, length, speeds)
extern "C" void fn_80541548(Event::EventManager_unk* manager, int window, void* msg, unsigned int kind, int style,
                            int size, int flags, float start, float length, float speed0, float speed1);
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
extern "C" void fn_805430A0(Event::EventManager_unk* manager, int key, int id, Event::Action* arg);
extern "C" void* fn_805430BC(Event::EventManager_unk* manager, int key, int id);
extern "C" atn::Model* fn_8001296C(Event::Actor_unk* actor); // inferred: model of an actor
extern "C" MtxPtr fn_80012AC4(Event::Actor_unk* actor, int node); // inferred: node matrix of an actor
extern "C" unsigned int fn_800DC6B4(const char* str);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::Vector3 fn_80541BD8(Event::EventManager_unk* manager, const atn::Vector3& pos); // inferred: event to world
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A58D0(void* pad, int port); // inferred: classic controller connected
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value); // inferred: sets a layout value
extern "C" void fn_800CB5B4(atn::SoundRef* sound, int frames, float volume); // inferred: sets the volume
// inferred: point light (index, position, radius, color)
extern "C" void fn_80485028(EventEffects_unk* mgr, int index, const atn::Vector3& pos, float radius, int color);
// inferred: spot light (position, direction, two angles, color)
extern "C" void fn_80485964(EventEffects_unk* mgr, const atn::Vector3& pos, const atn::Vector3& dir, float a,
                            float b, int color);

struct GameVars_unk; // inferred: lbl_8087F430 (only its variable accessors are used here)
extern "C" GameVars_unk* lbl_8087F430;
extern "C" int fn_80370A78(GameVars_unk* game, int id);             // inferred: value of a variable
extern "C" void fn_80370AE4(GameVars_unk* game, int id, int value); // inferred: sets a variable

namespace Event {

// inferred: effect of the event (0x20 bytes, found by id): m1C 2 = efp, otherwise a model effect
struct EffectEntry_unk {
    unsigned char m00[0x14];
    int m14;    // inferred: id
    void* m18;  // inferred: the effect data
    int m1C;    // inferred: kind
};

// inferred: effect `id` of the event (as EventManager_unk::FindActor_unk)
inline EffectEntry_unk* FindEffect_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < manager->mE8; i++) {
        EffectEntry_unk* entry = &manager->mE4[(unsigned int)i];
        if (id == entry->m14) {
            return entry;
        }
    }
    return 0;
}

// inferred: 2D animation of the event (0x20 bytes, found by id)
struct AnimEntry_unk {
    unsigned char m00[0x14];
    int m14;              // inferred: id
    atn::D2A_Task* m18;
    int m1C;              // inferred: 1 when loaded
};

// inferred: animation `id` of the event (as EventManager_unk::FindActor_unk)
inline AnimEntry_unk* FindAnim_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < manager->mD8; i++) {
        AnimEntry_unk* entry = &manager->mD4[(unsigned int)i];
        if (id == entry->m14) {
            return entry;
        }
    }
    return 0;
}

// inferred: movie of the event (found by id)
struct MovieState_unk {
    unsigned char m00[0x08];
    int m08; // inferred: 2 when finished
};
struct MovieRes_unk {
    unsigned char m00[0x4C];
    MovieState_unk* m4C;
};
struct MovieEntry_unk {
    unsigned char m00[0x14];
    int m14; // inferred: id
    MovieRes_unk* m18;
};

// inferred: movie `id` of the event (as EventManager_unk::FindActor_unk)
inline MovieEntry_unk* FindMovie_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < manager->m148; i++) {
        MovieEntry_unk* entry = &manager->m144[(unsigned int)i];
        if (id == entry->m14) {
            return entry;
        }
    }
    return 0;
}

// inferred: named entry of the event (found by id): streams of the multi audio actions, locators
struct AudioEntry_unk {
    unsigned char m00[0x08];
    std::string mName; // 0x08: inferred: stream name
    int m14;           // inferred: id
};

// inferred: stream `id` of the event (as EventManager_unk::FindActor_unk)
inline AudioEntry_unk* FindAudio_unk(EventManager_unk* manager, int id) {
    for (int i = 0; i < manager->m118; i++) {
        AudioEntry_unk* entry = &manager->m114[(unsigned int)i];
        if (id == entry->m14) {
            return entry;
        }
    }
    return 0;
}

// inferred: message window of the events (state 2 or 4: open)
struct MsgChoice_unk {
    unsigned char m00[0x7C];
    unsigned int m7C; // inferred: chosen answer
};
struct MsgWindow_unk {
    bool IsOpen_unk() { return m50 == 2 || m50 == 4; }
    unsigned char m00[0x50];
    int m50;
    MsgChoice_unk* m54; // inferred: answers of a choice
};

} // namespace Event

extern "C" void* fn_803B3B38(void* table, int id);                              // inferred: message `id`
extern "C" void fn_803B5830(Event::MsgWindow_unk* window, void* msg, int arg); // inferred: next message
extern "C" void fn_803B5774(Event::MsgWindow_unk* window, void* msg, int arg); // inferred: opens with a message
extern "C" void fn_803B57B0(Event::MsgWindow_unk* window, int arg);            // inferred: closes
extern "C" void fn_803B58D8(Event::MsgWindow_unk* window, void* msg);          // inferred: opens a choice
extern "C" void fn_803B57EC(Event::MsgWindow_unk* window);                     // inferred: closes the choice
extern "C" int fn_803B4C24(Event::MsgChoice_unk* choice);                       // inferred: answered
extern "C" void fn_8054136C(Event::EventManager_unk* manager, int label);      // inferred: jumps to a label
extern "C" Event::Cut* fn_80541210(Event::EventManager_unk* manager);           // inferred: current cut
extern "C" int fn_80541198(Event::EventManager_unk* manager, Event::Cut* cut);  // inferred: index of a cut
extern "C" void fn_80541520(Event::EventManager_unk* manager, int index, int arg); // inferred: goes to a cut

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
    void fn_805628B8(MapModel_unk* model, void* arg); // inferred: applies parameter 0 to a map model
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
        rot.x = 0.017453292f * arg->GetParam_unk(8)->m04.f;
        rot.y = 0.017453292f * arg->GetParam_unk(9)->m04.f;
        rot.z = 0.017453292f * arg->GetParam_unk(10)->m04.f;
        if (arg->GetParam_unk(12)->m04.i != 0) {
            ActionParam_unk* keyed = arg->GetParam_unk(13);
            ActionParam_unk* key = arg->GetParam_unk(14);
            if (keyed->m04.i != 0) {
                break;
            }
            fn_805430A0(manager, key->m04.i, effect, arg);
        }
        EffectEntry_unk* entry = FindEffect_unk(manager, effect);
        Actor_unk* actor = manager->FindActor_unk(actorId);
        if (actor->m20 == 6) {
            pos = fn_80541BD8(manager, pos);
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
        atn::Model* model = fn_8001296C(actor);
        MtxPtr mtx = fn_80012AC4(actor, node);
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
            owner = fn_805430BC(manager, id, arg->GetParam_unk(0)->m04.i);
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

int ActionAnimationPlay::vf08(Action* arg) {
    arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    AnimEntry_unk* entry = FindAnim_unk(manager, arg->GetParam_unk(0)->m04.i);
    if (entry->m1C == 1 && lbl_8087EF70 != 0) {
        atn::D2A_Task* task = entry->m18;
        float classic = fn_800A58D0(lbl_8087EF70, 0) != 0 ? 1.0f : 0.0f;
        fn_801FECE0((char*)task + 0x58, fn_800DC6B4("contorolClassic"), classic);
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
            fn_8054136C(manager, arg->GetParam_unk(1)->m04.i);
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
                fn_8054136C(arg->fn_805381C8(), label);
                return 1;
            }
        }
        if (type == 1) {
            fn_804817D4(lbl_8087F540);
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

int ActionNotify::vf0C(Action* arg, int phase) {
    if (phase == 5 || phase == 0) {
        if (lbl_8087F430 == 0) {
            return 2;
        }
        int id = arg->fn_805381C8()->m1AC;
        if (id == -1) {
            return 2;
        }
        int value = fn_80370A78(lbl_8087F430, id);
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
        fn_80370AE4(lbl_8087F430, id, result);
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
        void* msg = fn_803B3B38(manager->m1D4, arg->GetParam_unk(0)->m04.i);
        fn_803B58D8(manager->m1EC, msg);
        break;
    }
    case 3:
    case 4: {
        arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        fn_8054136C(manager, arg->GetParam_unk(4)->m04.i);
        fn_803B57EC(manager->m1EC);
        break;
    }
    }
    return 0;
}

int ActionMessageChoice::vf08(Action* arg) {
    MsgWindow_unk* window;
    arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    window = manager->m1EC;
    if (fn_803B4C24(window->m54)) {
        int labels[3];
        labels[0] = arg->GetParam_unk(1)->m04.i;
        labels[1] = arg->GetParam_unk(2)->m04.i;
        labels[2] = arg->GetParam_unk(3)->m04.i;
        unsigned int answer = window->m54->m7C;
        if (answer <= 2) {
            fn_8054136C(manager, labels[answer]);
        }
        fn_803B57EC(window);
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
        void* msg = fn_803B3B38(manager->m1D4, arg->GetParam_unk(0)->m04.i);
        MsgWindow_unk* window = manager->m1EC;
        if (window->IsOpen_unk()) {
            fn_803B5830(window, msg, 0);
        } else {
            fn_803B5774(window, msg, 0);
        }
        break;
    }
    case 3:
    case 4: {
        arg->fn_805381A0();
        MsgWindow_unk* window = arg->fn_805381C8()->m1EC;
        if (arg->GetParam_unk(1)->m04.i == 0 && window->IsOpen_unk()) {
            fn_803B57B0(window, 0);
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
            speed0 = lbl_807C8AB0[kind][0];
            speed1 = lbl_807C8AB0[kind][1];
        }
        void* msg = fn_803B3B38(manager->m1D4, text);
        if (kind == 0 && style == 0 && manager->m50 == 3 && lbl_8087F490 != 0 && lbl_8087F490->m2640 != 0) {
            lbl_8087F490->fn_803E3BE8(msg, arg->m14 - arg->m10, 0.0f);
        } else {
            int flags = 1;
            if (wait != 0) {
                flags |= 0x01000000;
            }
            int start = arg->m10;
            fn_80541548(manager, window, msg, kind, style, size, flags, start, arg->m14 - start, speed0, speed1);
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
            fn_80488050(lbl_8087F540, manager, arg);
            int fade = 60;
            if (arg->m18 > 0) {
                fade = arg->m18;
            }
            if (lbl_8087F540->mC4 == 0) {
                fn_80488A74(lbl_8087F540, entry->mName.c_str(), fade, volume);
            }
        }
        break;
    }
    case 3:
        if (arg->GetParam_unk(0)->m04.i != 0) {
            fn_80488D80(lbl_8087F540, arg->m1C);
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

int ActionMoviePlay::vf08(Action* arg) {
    EventManager_unk* manager = arg->fn_805381C8();
    int id = arg->GetParam_unk(0)->m04.i;
    MovieEntry_unk* entry = FindMovie_unk(manager, id);
    if (!lbl_8087F540->mMoviePlaying) {
        bool fading = lbl_8087F540->m1F6C > 0.0f;
        if (!fading) {
            fn_8048E9E0(lbl_8087F540);
        } else {
            return 3;
        }
    } else if (entry != 0) {
        MovieState_unk* state = entry->m18->m4C;
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
        fn_80486BEC(lbl_8087F540, on, value);
    }
    return 0;
}

int ActionSunLight::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        arg->fn_805381C8();
        atn::Vector3 dir = arg->fn_80538B18(2);
        fn_804846FC(lbl_8087F540, dir, arg->GetParam_unk(5)->m04.i);
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
        fn_80484A78(lbl_8087F540, arg->GetParam_unk(1)->m04.i);
    }
    return 0;
}

int ActionPointLight::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        int index = arg->GetParam_unk(0)->m04.i;
        atn::Vector3 pos = arg->fn_805384CC(1);
        float radius = arg->GetParam_unk(6)->m04.f;
        fn_80485028(lbl_8087F540, index, pos, radius, arg->GetParam_unk(7)->m04.i);
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
        fn_80485964(lbl_8087F540, pos, dir, a, b, arg->GetParam_unk(10)->m04.i);
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
        fn_80486DAC(lbl_8087F540, arg->GetParam_unk(0)->m04.i);
    }
    return 0;
}

int ActionBrightness::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        fn_80487064(lbl_8087F540, arg->GetParam_unk(0)->m04.i);
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
            fn_8048EE78(lbl_8087F540, a, r, g, b, c, d, e, f);
        } else {
            fn_8048F184(lbl_8087F540, a, r, g, b, c, d, e, f);
        }
    }
    return 0;
}

int ActionLocatorHide::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        EventManager_unk* manager = arg->fn_805381C8();
        MapLocators_unk* map = fn_80480438(lbl_8087F540);
        for (int i = 0; i < 12; i++) {
            AudioEntry_unk* entry = FindAudio_unk(manager, arg->GetParamAt_unk(i)->m04.i);
            if (entry != 0) {
                Locator_unk* locator = fn_804A0614(map, entry->mName);
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
        MapLocators_unk* map = fn_80480438(lbl_8087F540);
        for (int i = 0; i < 12; i++) {
            AudioEntry_unk* entry = FindAudio_unk(manager, arg->GetParamAt_unk(i)->m04.i);
            if (entry != 0) {
                Locator_unk* locator = fn_804A0614(map, entry->mName);
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

void ActionMapReflect::fn_805628B8(MapModel_unk* model, void* arg) {
    model->m16C = ((Action*)arg)->GetParam_unk(0)->m04.i;
}

int ActionMapReflect::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        ReflectTarget_unk* target = fn_800C16B4(lbl_8087EFB4);
        target->m120 = arg->GetParam_unk(0)->m04.i;
        MapManager::sInstance->fn_80491528(boost::bind(&ActionMapReflect::fn_805628B8, this, _1, _2), arg);
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
    for (int i = 0; i < manager->mBC; i++) {
        Cut* cut = manager->mB8[(unsigned int)i].mCut;
        if (index == cut->mIndex) {
            return cut;
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
            int index = fn_80541210(manager)->fn_8053B028();
            fn_80541520(manager, fn_80541198(manager, FindCut_unk(manager, index)), 0);
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
