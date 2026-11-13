#ifndef EVENT_ACTIONPROCESSORS_UNK_H
#define EVENT_ACTIONPROCESSORS_UNK_H

#include "Event/Behavior.h"
#include "atn/SingletonNoStatic.h"
#include "math/Vec.h"

struct MapArea;

// Processors of the actions of the events (vf08 per frame, vf0C per phase), created by
// fn_8054E574 (Event/ActionActorBehavior.cpp) and kept in Event::sProcessors_unk. Class names from
// the RTTI; their members are defined in Event/ActionEffectPlay.cpp, Event/ActionCameraMode.cpp,
// Event/ActionActorPosition.cpp and Event/ActionActorBehavior.cpp. File name invented.

namespace Event {

// Event/ActionEffectPlay.cpp

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

// Event/ActionCameraMode.cpp

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

// Event/ActionActorPosition.cpp

class ActionActorPosition : public ActionProcessor, public atn::SingletonNoStatic<ActionActorPosition> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
    // inferred: moves the actor of a position action; t is the number of frames left (static: vf08
    // only passes the action)
    static void fn_80551194(Action* arg, float t);
};

class ActionActorMotion : public ActionProcessor, public atn::SingletonNoStatic<ActionActorMotion> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorFacial : public ActionProcessor, public atn::SingletonNoStatic<ActionActorFacial> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorEyes : public ActionProcessor, public atn::SingletonNoStatic<ActionActorEyes> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
    static void fn_80553A74(Action* arg, float now); // inferred: eyes towards the camera, with jitter
    // inferred: eyes towards a point (`cur` moves towards `goal` in `frames`; angles from the parameters
    // when `mode` is not 0)
    static void fn_80553DC8(Action* arg, atn::Vector3* cur, const atn::Vector3* goal, int mode, float frames);
    // inferred: eyes outside the wandering window (`left` frames left, at frame `now`)
    static void fn_80554088(Action* arg, int length, atn::Vector3* v, int mode, float left, float now);
};

class ActionActorPose : public ActionProcessor, public atn::SingletonNoStatic<ActionActorPose> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorHeading : public ActionProcessor, public atn::SingletonNoStatic<ActionActorHeading> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorNode : public ActionProcessor, public atn::SingletonNoStatic<ActionActorNode> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorSetting : public ActionProcessor, public atn::SingletonNoStatic<ActionActorSetting> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
    int fn_80557E00(int, const char*); // inferred: the node is a hair node of the hair kind
    void fn_80557F84(Actor*, int, int, int, int); // inferred: enables the physics bones of the settings
};

class ActionActorWind : public ActionProcessor, public atn::SingletonNoStatic<ActionActorWind> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorMaterial : public ActionProcessor, public atn::SingletonNoStatic<ActionActorMaterial> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

// Event/ActionActorBehavior.cpp
class ActionActorBehavior : public ActionProcessor, public atn::SingletonNoStatic<ActionActorBehavior> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

} // namespace Event

#endif
