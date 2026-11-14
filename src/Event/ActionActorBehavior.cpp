#include "game/SetConfig.h"
#include "game/GameTask.h"
#include "game/GameConfig_unk.h"
#include "Event/Dialog.h"
#include "Event/Cut.h"
#include "Event/Section.h"
#include "Event/Channel.h"
#include "Event/Action.h"
#include "Event/Behavior.h"
#include "Event/Sequence.h"
#include "Event/Manager.h"
#include "Event/ActionProcessors_unk.h"
#include "atn/Memory.h"



extern "C" void fn_8047F6E4(void* object);
extern "C" void fn_8047F8B0(void* object);
extern "C" void fn_8047F850(void* object, int start, int colorStart, int colorEnd);
extern "C" void fn_8047F88C(void* object, int frame);
extern "C" void fn_8047F994(void* object, int arg1, int arg2);
namespace Event {
// inferred: processor of each kind of action (.bss of this file)
ActionProcessor* sProcessors_unk[56];

// inferred: the processors (.sbss of this file, laid out in the order of declaration)
static ActionActorMaterial* sActorMaterial_unk; // lbl_8087F8B0
static ActionActorWind* sActorWind_unk; // lbl_8087F8B4
static ActionActorSetting* sActorSetting_unk; // lbl_8087F8B8
static ActionActorNode* sActorNode_unk; // lbl_8087F8BC
static ActionActorHeading* sActorHeading_unk; // lbl_8087F8C0
static ActionActorPose* sActorPose_unk; // lbl_8087F8C4
static ActionActorEyes* sActorEyes_unk; // lbl_8087F8C8
static ActionActorFacial* sActorFacial_unk; // lbl_8087F8CC
static ActionActorMotion* sActorMotion_unk; // lbl_8087F8D0
static ActionActorPosition* sActorPosition_unk; // lbl_8087F8D4
static ActionActorBehavior* sActorBehavior_unk; // lbl_8087F8D8
static ActionCameraInertia* sCameraInertia_unk; // lbl_8087F8DC
static ActionCameraMotion* sCameraMotion_unk; // lbl_8087F8E0
static ActionCameraFocus* sCameraFocus_unk; // lbl_8087F8E4
static ActionCameraSetting* sCameraSetting_unk; // lbl_8087F8E8
static ActionCameraShake* sCameraShake_unk; // lbl_8087F8EC
static ActionCameraEffect* sCameraEffect_unk; // lbl_8087F8F0
static ActionCameraMode* sCameraMode_unk; // lbl_8087F8F4
static ActionCutAdjust* sCutAdjust_unk; // lbl_8087F8F8
static ActionDialogText* sDialogText_unk; // lbl_8087F8FC
static ActionDialogSpeaker* sDialogSpeaker_unk; // lbl_8087F900
static ActionGameSetting* sGameSetting_unk; // lbl_8087F904
static ActionMapReflect* sMapReflect_unk; // lbl_8087F908
static ActionLocatorHide* sLocatorHide_unk; // lbl_8087F90C
static ActionCameraChange* sCameraChange_unk; // lbl_8087F910
static ActionColorGradation* sColorGradation_unk; // lbl_8087F914
static ActionColorMatrix* sColorMatrix_unk; // lbl_8087F918
static ActionBrightness* sBrightness_unk; // lbl_8087F91C
static ActionLightSurround* sLightSurround_unk; // lbl_8087F920
static ActionSpotLight* sSpotLight_unk; // lbl_8087F924
static ActionPointLight* sPointLight_unk; // lbl_8087F928
static ActionCloudMap* sCloudMap_unk; // lbl_8087F92C
static ActionAmbientLight* sAmbientLight_unk; // lbl_8087F930
static ActionSunLight* sSunLight_unk; // lbl_8087F934
static ActionGodRay* sGodRay_unk; // lbl_8087F938
static ActionMoviePlay* sMoviePlay_unk; // lbl_8087F93C
static ActionMultiAudio* sMultiAudio_unk; // lbl_8087F940
static ActionVoicePlay* sVoicePlay_unk; // lbl_8087F944
static ActionAudioEffect* sAudioEffect_unk; // lbl_8087F948
static ActionMusicPlay* sMusicPlay_unk; // lbl_8087F94C
static ActionEnvSound* sEnvSound_unk; // lbl_8087F950
static ActionSoundPlay* sSoundPlay_unk; // lbl_8087F954
static ActionMessagePrint* sMessagePrint_unk; // lbl_8087F958
static ActionMessageWindow* sMessageWindow_unk; // lbl_8087F95C
static ActionMessageChoice* sMessageChoice_unk; // lbl_8087F960
static ActionNotify* sNotify_unk; // lbl_8087F964
static ActionCheck* sCheck_unk; // lbl_8087F968
static ActionAnimationPlay* sAnimationPlay_unk; // lbl_8087F96C
static ActionEffectPlay* sEffectPlay_unk; // lbl_8087F970

// inferred: creates a processor once (one overload per class: the "" of the operator new go to the
// .sdata of each copy)
#define EVENT_PROCESSOR_CREATE(T)                                                                 \
    inline void CreateProcessor_unk(T*& processor) {                                              \
        if (processor == 0) {                                                                      \
            processor = new ((MemoryTag)1, "", "", 0) T;                                           \
        }                                                                                          \
    }
EVENT_PROCESSOR_CREATE(ActionEffectPlay)
EVENT_PROCESSOR_CREATE(ActionAnimationPlay)
EVENT_PROCESSOR_CREATE(ActionMessageChoice)
EVENT_PROCESSOR_CREATE(ActionMessageWindow)
EVENT_PROCESSOR_CREATE(ActionMessagePrint)
EVENT_PROCESSOR_CREATE(ActionSoundPlay)
EVENT_PROCESSOR_CREATE(ActionEnvSound)
EVENT_PROCESSOR_CREATE(ActionMusicPlay)
EVENT_PROCESSOR_CREATE(ActionAudioEffect)
EVENT_PROCESSOR_CREATE(ActionVoicePlay)
EVENT_PROCESSOR_CREATE(ActionMultiAudio)
EVENT_PROCESSOR_CREATE(ActionMoviePlay)
EVENT_PROCESSOR_CREATE(ActionCheck)
EVENT_PROCESSOR_CREATE(ActionNotify)
EVENT_PROCESSOR_CREATE(ActionGodRay)
EVENT_PROCESSOR_CREATE(ActionSunLight)
EVENT_PROCESSOR_CREATE(ActionAmbientLight)
EVENT_PROCESSOR_CREATE(ActionCloudMap)
EVENT_PROCESSOR_CREATE(ActionPointLight)
EVENT_PROCESSOR_CREATE(ActionSpotLight)
EVENT_PROCESSOR_CREATE(ActionLightSurround)
EVENT_PROCESSOR_CREATE(ActionBrightness)
EVENT_PROCESSOR_CREATE(ActionColorMatrix)
EVENT_PROCESSOR_CREATE(ActionColorGradation)
EVENT_PROCESSOR_CREATE(ActionLocatorHide)
EVENT_PROCESSOR_CREATE(ActionCameraChange)
EVENT_PROCESSOR_CREATE(ActionMapReflect)
EVENT_PROCESSOR_CREATE(ActionGameSetting)
EVENT_PROCESSOR_CREATE(ActionCameraMode)
EVENT_PROCESSOR_CREATE(ActionCameraEffect)
EVENT_PROCESSOR_CREATE(ActionCameraShake)
EVENT_PROCESSOR_CREATE(ActionCameraSetting)
EVENT_PROCESSOR_CREATE(ActionCameraFocus)
EVENT_PROCESSOR_CREATE(ActionCameraMotion)
EVENT_PROCESSOR_CREATE(ActionCameraInertia)
EVENT_PROCESSOR_CREATE(ActionActorBehavior)
EVENT_PROCESSOR_CREATE(ActionActorPosition)
EVENT_PROCESSOR_CREATE(ActionActorMotion)
EVENT_PROCESSOR_CREATE(ActionActorFacial)
EVENT_PROCESSOR_CREATE(ActionActorEyes)
EVENT_PROCESSOR_CREATE(ActionActorPose)
EVENT_PROCESSOR_CREATE(ActionActorHeading)
EVENT_PROCESSOR_CREATE(ActionActorNode)
EVENT_PROCESSOR_CREATE(ActionActorSetting)
EVENT_PROCESSOR_CREATE(ActionActorWind)
EVENT_PROCESSOR_CREATE(ActionActorMaterial)
EVENT_PROCESSOR_CREATE(ActionDialogSpeaker)
EVENT_PROCESSOR_CREATE(ActionDialogText)
EVENT_PROCESSOR_CREATE(ActionCutAdjust)

// inferred: deletes a processor
template <class T>
inline void DeleteProcessor_unk(T*& processor) {
    if (processor != 0) {
        delete processor;
        processor = 0;
    }
}

} // namespace Event

// inferred: creates the processors of the actions and fills the table of each kind (called by
// Event::Manager)
extern "C" void fn_8054E574() {
    Event::CreateProcessor_unk(Event::sEffectPlay_unk);
    Event::CreateProcessor_unk(Event::sAnimationPlay_unk);
    Event::CreateProcessor_unk(Event::sMessageChoice_unk);
    Event::CreateProcessor_unk(Event::sMessageWindow_unk);
    Event::CreateProcessor_unk(Event::sMessagePrint_unk);
    Event::CreateProcessor_unk(Event::sSoundPlay_unk);
    Event::CreateProcessor_unk(Event::sEnvSound_unk);
    Event::CreateProcessor_unk(Event::sMusicPlay_unk);
    Event::CreateProcessor_unk(Event::sAudioEffect_unk);
    Event::CreateProcessor_unk(Event::sVoicePlay_unk);
    Event::CreateProcessor_unk(Event::sMultiAudio_unk);
    Event::CreateProcessor_unk(Event::sMoviePlay_unk);
    Event::CreateProcessor_unk(Event::sCheck_unk);
    Event::CreateProcessor_unk(Event::sNotify_unk);
    Event::CreateProcessor_unk(Event::sGodRay_unk);
    Event::CreateProcessor_unk(Event::sSunLight_unk);
    Event::CreateProcessor_unk(Event::sAmbientLight_unk);
    Event::CreateProcessor_unk(Event::sCloudMap_unk);
    Event::CreateProcessor_unk(Event::sPointLight_unk);
    Event::CreateProcessor_unk(Event::sSpotLight_unk);
    Event::CreateProcessor_unk(Event::sLightSurround_unk);
    Event::CreateProcessor_unk(Event::sBrightness_unk);
    Event::CreateProcessor_unk(Event::sColorMatrix_unk);
    Event::CreateProcessor_unk(Event::sColorGradation_unk);
    Event::CreateProcessor_unk(Event::sLocatorHide_unk);
    Event::CreateProcessor_unk(Event::sCameraChange_unk);
    Event::CreateProcessor_unk(Event::sMapReflect_unk);
    Event::CreateProcessor_unk(Event::sGameSetting_unk);
    Event::CreateProcessor_unk(Event::sCameraMode_unk);
    Event::CreateProcessor_unk(Event::sCameraEffect_unk);
    Event::CreateProcessor_unk(Event::sCameraShake_unk);
    Event::CreateProcessor_unk(Event::sCameraSetting_unk);
    Event::CreateProcessor_unk(Event::sCameraFocus_unk);
    Event::CreateProcessor_unk(Event::sCameraMotion_unk);
    Event::CreateProcessor_unk(Event::sCameraInertia_unk);
    Event::CreateProcessor_unk(Event::sActorBehavior_unk);
    Event::CreateProcessor_unk(Event::sActorPosition_unk);
    Event::CreateProcessor_unk(Event::sActorMotion_unk);
    Event::CreateProcessor_unk(Event::sActorFacial_unk);
    Event::CreateProcessor_unk(Event::sActorEyes_unk);
    Event::CreateProcessor_unk(Event::sActorPose_unk);
    Event::CreateProcessor_unk(Event::sActorHeading_unk);
    Event::CreateProcessor_unk(Event::sActorNode_unk);
    Event::CreateProcessor_unk(Event::sActorSetting_unk);
    Event::CreateProcessor_unk(Event::sActorWind_unk);
    Event::CreateProcessor_unk(Event::sActorMaterial_unk);
    Event::CreateProcessor_unk(Event::sDialogSpeaker_unk);
    Event::CreateProcessor_unk(Event::sDialogText_unk);
    Event::CreateProcessor_unk(Event::sCutAdjust_unk);
    for (int i = 0; i < 55; i++) {
        Event::sProcessors_unk[i] = 0;
    }
    Event::sProcessors_unk[7] = Event::sEffectPlay_unk;
    Event::sProcessors_unk[15] = Event::sAnimationPlay_unk;
    Event::sProcessors_unk[10] = Event::sMessageChoice_unk;
    Event::sProcessors_unk[11] = Event::sMessageWindow_unk;
    Event::sProcessors_unk[12] = Event::sMessagePrint_unk;
    Event::sProcessors_unk[1] = Event::sSoundPlay_unk;
    Event::sProcessors_unk[2] = Event::sEnvSound_unk;
    Event::sProcessors_unk[4] = Event::sMusicPlay_unk;
    Event::sProcessors_unk[3] = Event::sAudioEffect_unk;
    Event::sProcessors_unk[5] = Event::sVoicePlay_unk;
    Event::sProcessors_unk[43] = Event::sMultiAudio_unk;
    Event::sProcessors_unk[6] = Event::sMoviePlay_unk;
    Event::sProcessors_unk[8] = Event::sCheck_unk;
    Event::sProcessors_unk[9] = Event::sNotify_unk;
    Event::sProcessors_unk[48] = Event::sGodRay_unk;
    Event::sProcessors_unk[16] = Event::sSunLight_unk;
    Event::sProcessors_unk[18] = Event::sAmbientLight_unk;
    Event::sProcessors_unk[17] = Event::sCloudMap_unk;
    Event::sProcessors_unk[47] = Event::sPointLight_unk;
    Event::sProcessors_unk[49] = Event::sSpotLight_unk;
    Event::sProcessors_unk[50] = Event::sLightSurround_unk;
    Event::sProcessors_unk[51] = Event::sBrightness_unk;
    Event::sProcessors_unk[19] = Event::sColorMatrix_unk;
    Event::sProcessors_unk[52] = Event::sColorGradation_unk;
    Event::sProcessors_unk[20] = Event::sLocatorHide_unk;
    Event::sProcessors_unk[29] = Event::sCameraChange_unk;
    Event::sProcessors_unk[46] = Event::sMapReflect_unk;
    Event::sProcessors_unk[21] = Event::sGameSetting_unk;
    Event::sProcessors_unk[22] = Event::sCameraMode_unk;
    Event::sProcessors_unk[26] = Event::sCameraEffect_unk;
    Event::sProcessors_unk[25] = Event::sCameraShake_unk;
    Event::sProcessors_unk[27] = Event::sCameraSetting_unk;
    Event::sProcessors_unk[24] = Event::sCameraFocus_unk;
    Event::sProcessors_unk[28] = Event::sCameraMotion_unk;
    Event::sProcessors_unk[30] = Event::sCameraInertia_unk;
    Event::sProcessors_unk[31] = Event::sActorBehavior_unk;
    Event::sProcessors_unk[37] = Event::sActorPosition_unk;
    Event::sProcessors_unk[33] = Event::sActorMotion_unk;
    Event::sProcessors_unk[32] = Event::sActorFacial_unk;
    Event::sProcessors_unk[44] = Event::sActorEyes_unk;
    Event::sProcessors_unk[34] = Event::sActorPose_unk;
    Event::sProcessors_unk[35] = Event::sActorHeading_unk;
    Event::sProcessors_unk[45] = Event::sActorNode_unk;
    Event::sProcessors_unk[38] = Event::sActorSetting_unk;
    Event::sProcessors_unk[39] = Event::sActorWind_unk;
    Event::sProcessors_unk[54] = Event::sActorMaterial_unk;
    Event::sProcessors_unk[41] = Event::sDialogSpeaker_unk;
    Event::sProcessors_unk[42] = Event::sDialogText_unk;
    Event::sProcessors_unk[53] = Event::sCutAdjust_unk;
}

// inferred: deletes the processors and clears the table
extern "C" void fn_8054F380() {
    Event::DeleteProcessor_unk(Event::sEffectPlay_unk);
    Event::DeleteProcessor_unk(Event::sAnimationPlay_unk);
    Event::DeleteProcessor_unk(Event::sMessageChoice_unk);
    Event::DeleteProcessor_unk(Event::sMessageWindow_unk);
    Event::DeleteProcessor_unk(Event::sMessagePrint_unk);
    Event::DeleteProcessor_unk(Event::sSoundPlay_unk);
    Event::DeleteProcessor_unk(Event::sEnvSound_unk);
    Event::DeleteProcessor_unk(Event::sMusicPlay_unk);
    Event::DeleteProcessor_unk(Event::sAudioEffect_unk);
    Event::DeleteProcessor_unk(Event::sVoicePlay_unk);
    Event::DeleteProcessor_unk(Event::sMultiAudio_unk);
    Event::DeleteProcessor_unk(Event::sMoviePlay_unk);
    Event::DeleteProcessor_unk(Event::sCheck_unk);
    Event::DeleteProcessor_unk(Event::sNotify_unk);
    Event::DeleteProcessor_unk(Event::sGodRay_unk);
    Event::DeleteProcessor_unk(Event::sSunLight_unk);
    Event::DeleteProcessor_unk(Event::sAmbientLight_unk);
    Event::DeleteProcessor_unk(Event::sCloudMap_unk);
    Event::DeleteProcessor_unk(Event::sPointLight_unk);
    Event::DeleteProcessor_unk(Event::sSpotLight_unk);
    Event::DeleteProcessor_unk(Event::sLightSurround_unk);
    Event::DeleteProcessor_unk(Event::sBrightness_unk);
    Event::DeleteProcessor_unk(Event::sColorMatrix_unk);
    Event::DeleteProcessor_unk(Event::sColorGradation_unk);
    Event::DeleteProcessor_unk(Event::sLocatorHide_unk);
    Event::DeleteProcessor_unk(Event::sCameraChange_unk);
    Event::DeleteProcessor_unk(Event::sMapReflect_unk);
    Event::DeleteProcessor_unk(Event::sGameSetting_unk);
    Event::DeleteProcessor_unk(Event::sCameraMode_unk);
    Event::DeleteProcessor_unk(Event::sCameraEffect_unk);
    Event::DeleteProcessor_unk(Event::sCameraShake_unk);
    Event::DeleteProcessor_unk(Event::sCameraSetting_unk);
    Event::DeleteProcessor_unk(Event::sCameraFocus_unk);
    Event::DeleteProcessor_unk(Event::sCameraMotion_unk);
    Event::DeleteProcessor_unk(Event::sCameraInertia_unk);
    Event::DeleteProcessor_unk(Event::sActorBehavior_unk);
    Event::DeleteProcessor_unk(Event::sActorPosition_unk);
    Event::DeleteProcessor_unk(Event::sActorMotion_unk);
    Event::DeleteProcessor_unk(Event::sActorFacial_unk);
    Event::DeleteProcessor_unk(Event::sActorEyes_unk);
    Event::DeleteProcessor_unk(Event::sActorPose_unk);
    Event::DeleteProcessor_unk(Event::sActorHeading_unk);
    Event::DeleteProcessor_unk(Event::sActorNode_unk);
    Event::DeleteProcessor_unk(Event::sActorSetting_unk);
    Event::DeleteProcessor_unk(Event::sActorWind_unk);
    Event::DeleteProcessor_unk(Event::sActorMaterial_unk);
    Event::DeleteProcessor_unk(Event::sDialogSpeaker_unk);
    Event::DeleteProcessor_unk(Event::sDialogText_unk);
    Event::DeleteProcessor_unk(Event::sCutAdjust_unk);
    for (int i = 0; i < 55; i++) {
        Event::sProcessors_unk[i] = 0;
    }
}
extern "C" GameConfig_unk* lbl_8087F0A8; // inferred: game settings
extern "C" GameTask* lbl_8087F430;      // inferred: field task


namespace Event {

// inferred: the sequence is paused (names invented; flags 0x100000/0x200000 of m98)
void Sequence::fn_8054F9DC() {
    int state = m194;
    if (state == 4) {
        m98 |= 0x300000;
    } else if (state == 5) {
        fn_8053F6EC();
        m98 |= 0x100000;
    } else if (state == 6) {
        m98 |= 0x100000;
    } else if (state == 8 || state == 14) {
        if (fn_8054011C()) {
            m198 = 0.0f;
            m19C = 0.0f;
            fn_8053E8C8(9);
        } else {
            fn_8053E8C8(6);
            m98 |= 0x100000;
        }
    } else if (state == 12) {
        fn_8053E8C8(10);
    }
    MeltTask();
    lbl_8087F540->fn_8047F984(1.0f);
}

// inferred: the sequence is resumed
void Sequence::fn_8054FAD4() {
    if (m194 == 10) {
        fn_8053E8C8(11);
    }
    lbl_8087F540->fn_8047F984(0.0f);
}

// inferred: the sequence is skipped (`mark`: flag 0x10000 of m98)
void Sequence::fn_8054FB10(int mark) {
    int state = m194;
    if (state == 10 || state == 12) {
        fn_80550A78();
        m198 = 0.0f;
        m19C = 0.0f;
        m1A4 = -1;
        fn_8053E8C8(7);
    } else if (state == 14) {
        m198 = 0.0f;
        m19C = 0.0f;
        m1A4 = -1;
        fn_8053E8C8(14);
    }
    lbl_8087F540->fn_8047F984(1.0f);
    if (mark != 0) {
        m98 |= 0x10000;
    }
}

void Sequence::fn_8054FBC8() {
    if (m98_15 == 1) {
        fn_8053EF70();
    }
}

} // namespace Event

namespace Event {

// inferred: cut `i` of the sequence (index checked)
inline Cut* CutAt_unk(Sequence* seq, int i) {
    return seq->IsCutIndex_unk(i) ? seq->mB8[i].get() : 0;
}

// inferred: leaves the current cut for m1B4 (inlined in both jumps of fn_8054FBE0)
inline void LeaveCut_unk(Sequence* seq, int run) {
    if (run) {
        seq->fn_80550A78();
    }
    if (CutAt_unk(seq, seq->m1A0)->mSkip != 3 || CutAt_unk(seq, seq->m1B4)->mSkip != 3) {
        Manager* mgr = lbl_8087F540;
        mgr->mB8.clear();
        lbl_8087F540->fn_8047F994(1, 1);
    }
    seq->m1A0 = seq->m1B4;
}

// inferred: applies a pending jump (m1B0: 1 to a cut, 2 to a frame of the cut; m1B4 cut, m1B8 frame)
void Sequence::fn_8054FBE0(int run) {
    if (m1B0 == 0) {
        return;
    }
    if (m1B0 == 1) {
        if (m1B4 != m1A0) {
            LeaveCut_unk(this, run);
            Manager* manager = lbl_8087F540;
            if (strlen(manager->m1F38.c_str()) != 0) {
                if (lbl_8087F540->m1F5C != 0) {
                    manager->m1F5C = 0;
                } else if ((lbl_8087F540->m1F1C & 0x80000000) == 0x80000000) {
                    manager->fn_80488D6C();
                }
            }
        }
        m19C = m1B8;
        m198 = m1B8;
        for (int i = 0; i < (int)m164.size(); i++) {
            Actor* actor = &m164[i];
            if (actor != 0) {
                actor->mB8[0] = 0;
                actor->mB8[1] = 0;
                actor->mB8[2] = 0;
            }
        }
        if (m190 == 0x5E7 || m190 == 0x841 || (m190 == 0xE14 && fn_80541210() != 0 && fn_80541210()->mIndex == 0) ||
            (m190 == 0x151E && fn_80541210() != 0 && fn_80541210()->mIndex == 3)) {
            mPadCamera_unk = 0;
        }
    } else {
        if (m1B4 != m1A0) {
            LeaveCut_unk(this, run);
            m19C = 0.0f;
        }
        m198 = m1B8;
    }
    m1B0 = 0;
}

} // namespace Event

namespace Event {

// inferred: fades of the current cut (m18/m1C: fade in, kind and frames; m20/m24: fade out)
void Sequence::fn_8054FFE0() {
    Cut* cut = fn_80541210();
    if (cut != 0 && (m50 == 1 || m50 == 3)) {
        int in = cut->m1C;
        if (m1A4 != m1A0) {
            switch (cut->m18) {
            case 0:
                lbl_8087F540->fn_8047F8B0();
                break;
            case 1:
                lbl_8087F540->fn_8047F850(in, 0xFF000000, 0);
                break;
            case 2:
                lbl_8087F540->fn_8047F850(in, -1, 0x00FFFFFF);
                break;
            case 3:
                lbl_8087F540->fn_8047F88C(in);
                break;
            }
        }
        int out = cut->m24;
        if (m198 >= (float)(cut->m2C - out) && m19C < (float)(cut->m2C - out)) {
            switch (cut->m20) {
            case 1:
                lbl_8087F540->fn_8047F850(out, 0, 0xFF000000);
                break;
            case 2:
                lbl_8087F540->fn_8047F850(out, 0x00FFFFFF, -1);
                break;
            }
        }
        if (out != 0 && m198 >= (float)(cut->m2C - out / 2) && m19C < (float)(cut->m2C - out / 2)) {
            if (cut->m20 == 3) {
                lbl_8087F540->fn_8047F88C(out);
            }
        }
        lbl_8087F540->fn_8047F6E4();
    }
    m1A4 = m1A0;
}

} // namespace Event

namespace Event {

// inferred: the action that holds the sequence (its vf08 returned 3); the manager is told when it starts
// and when it ends
void Sequence::fn_805501DC(Action* action) {
    if (m1C0 == 0 && action != 0) {
        lbl_8087F540->fn_8047F8C0(1);
    } else if (m1C0 != 0 && action == 0) {
        lbl_8087F540->fn_8047F8C0(0);
    }
    m1C0 = action;
}

// inferred: processor of an action (kind 0x1F: a behaviour chosen by parameter 0)
inline ActionProcessor* GetProcessor_unk(Action* action) {
    if (action->m0C == 0x1F) {
        return fn_8010DB54(action->GetParam_unk(0)->m04.i);
    }
    return sProcessors_unk[action->m0C];
}

// inferred: the frame `t` was reached in this step (from m19C, with a small tolerance, up to m198)
inline int IsStepFrame_unk(const Sequence* seq, float t) {
    return t >= seq->m19C - 0.00001f && t < seq->m198;
}

// inferred: time of a frame of the event manager
inline float FrameTime_unk() {
    return lbl_8087F540->mB4;
}

// inferred: the holding action still holds the sequence (else it is released)
inline bool IsHeld_unk(Sequence* seq) {
    if (seq->m1C0 != 0) {
        ActionProcessor* processor = GetProcessor_unk(seq->m1C0);
        if (processor == 0) {
            return false;
        }
        if (processor->vf08(seq->m1C0) == 3) {
            return true;
        }
        seq->fn_805501DC(0);
    }
    return false;
}

// NONMATCHING: 99.4% - saved registers: the original gives r24 to the processor and r21-r23 to the action,
// channel and section (here one higher each, the processor in r21)
// inferred: one frame of the sequence: waits for the holding action, advances the frame and runs the phases
// of the actions of the current cut whose marks were reached (0 start, 1 end of the fade in, 2 start of the
// fade out, 3 end; vf08 while it spans the step); at the end of the cut it goes to the next one
void Sequence::fn_80550254() {
    mPadCamera_unk = 1;
    if (SetConfig::sInstance != 0 && SetConfig::sInstance->mEventCameraControl == 0) {
        mPadCamera_unk = 0;
    }
    if (IsHeld_unk(this)) {
        return;
    }
    fn_8054FBE0(1);
    m198 += FrameTime_unk();
    if (m19C > m198) {
        m19C = 0.0f;
    }
    fn_8054FFE0();
    if (m50 == 1 || m50 == 3) {
        lbl_8087F540->fn_80481A20();
        lbl_8087F540->fn_80489A50(0, 0);
        lbl_8087F540->fn_80488DB8();
    }
    Cut* cut = fn_80541210();
    if (cut != 0) {
        for (int i = 0; i < (int)cut->mSections.size(); i++) {
            Section* section = cut->GetSection_unk(i);
            if (section->m14 == 0) {
                continue;
            }
            if (section->m0C == 3) {
                m1BC = FindActor_unk(section->m10);
            } else {
                m1BC = 0;
            }
            for (int j = 0; j < (int)section->mChannels.size(); j++) {
                Channel* channel = section->GetChannel_unk(j);
                if (channel->m10 == 0) {
                    continue;
                }
                for (int k = 0; k < (int)channel->mActions.size(); k++) {
                    Action* action = channel->GetAction_unk(k);
                    action->m3C = k;
                    ActionProcessor* processor = GetProcessor_unk(action);
                    if (processor == 0) {
                        continue;
                    }
                    float start = action->m10;
                    float fadeIn = action->m10 + action->m18;
                    float fadeOut = action->m14 - action->m1C;
                    float end = action->m14;
                    int atStart = IsStepFrame_unk(this, start);
                    int atFadeIn = IsStepFrame_unk(this, fadeIn);
                    int atFadeOut = IsStepFrame_unk(this, fadeOut);
                    int atEnd = IsStepFrame_unk(this, end);
                    int result = 0;
                    if (atStart && atEnd) {
                        processor->vf0C(action, 0);
                        processor->vf0C(action, 1);
                        result = processor->vf08(action);
                        processor->vf0C(action, 2);
                        processor->vf0C(action, 3);
                    } else {
                        if (atStart) {
                            processor->vf0C(action, 0);
                        }
                        if (atFadeIn) {
                            processor->vf0C(action, 1);
                        }
                        if (m19C >= start && m198 <= end) {
                            result = processor->vf08(action);
                        }
                        if (atFadeOut) {
                            processor->vf0C(action, 2);
                        }
                        if (atEnd) {
                            processor->vf0C(action, 3);
                        }
                    }
                    if (result == 3) {
                        fn_805501DC(action);
                    }
                }
            }
        }
        fn_8054FBE0(0);
        if (m50 == 1 || m50 == 3) {
            lbl_8087F540->fn_8048F9B0(cut);
        }
        Cut* current = fn_80541210();
        if (current != 0 && m198 > (float)current->m2C) {
            m198 = (float)current->m2C;
            if (m98_8 != 1 || (m280 != 0 && m280->fn_802291BC() == 0)) {
                if (current->fn_8053B028() != -1) {
                    fn_80541520(fn_80541198(FindCutByIndex_unk(current->fn_8053B028())), 0);
                    if (m50 == 1 || m50 == 3) {
                        lbl_8087F540->fn_80484FBC();
                        lbl_8087F540->fn_804870F4();
                        lbl_8087F540->fn_80486F50();
                        lbl_8087F540->fn_8048F184(0);
                        lbl_8087F540->fn_8048F184(1);
                        lbl_8087F540->fn_8048FC7C();
                    }
                    if (m280 != 0) {
                        m280->fn_802297AC();
                    }
                    int index = current->mIndex;
                    if (current->fn_8053B028() != index) {
                        fn_80550254();
                    }
                } else if (lbl_8087F0A8->m194 != 0 && m190 == 0x19D) {
                    lbl_8087F430->fn_80370B78(-1, 0, 0, 0.0f);
                    fn_8053E8C8(11);
                } else {
                    fn_8053E8C8(13);
                }
            }
        }
    }
    m19C = m198;
}

} // namespace Event

namespace Event {

// inferred: runs phase 4 of the actions of the current cut that span the current frame m19C
void Sequence::fn_80550A78() {
    Cut* cut = fn_80541210();
    if (cut == 0) {
        return;
    }
    for (int i = 0; i < (int)cut->mSections.size(); i++) {
        Section* section = cut->GetSection_unk(i);
        if (section->m14 == 0) {
            return;
        }
        if (section->m0C == 3) {
            m1BC = FindActor_unk(section->m10);
        } else {
            m1BC = 0;
        }
        for (int j = 0; j < (int)section->mChannels.size(); j++) {
            Channel* channel = section->GetChannel_unk(j);
            if (channel->m10 == 0) {
                return;
            }
            for (int k = 0; k < (int)channel->mActions.size(); k++) {
                Action* action = channel->GetAction_unk(k);
                ActionProcessor* processor;
                if (action->m0C == 0x1F) {
                    processor = fn_8010DB54(action->GetParam_unk(0)->m04.i);
                } else {
                    processor = sProcessors_unk[action->m0C];
                }
                if (processor != 0) {
                    float start = action->m10;
                    float end = action->m14;
                    if (start <= m19C && end >= m19C) {
                        processor->vf0C(action, 4);
                    }
                }
            }
        }
    }
}

} // namespace Event

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

// inferred: plays the rest of the event at once: phase 5 of the actions met up to the end of each
// cut, following the cuts and the jumps; then the state 13
void Sequence::fn_80550CE0() {
    int done = 0;
    while (!done) {
        Cut* cut = fn_80541210();
        m198 = cut->m2C;
        for (int i = 0; i < (int)cut->mSections.size(); i++) {
            Section* section = cut->GetSection_unk(i);
            if (section->m14 == 0) {
                return;
            }
            if (section->m0C == 3) {
                m1BC = FindActor_unk(section->m10);
            } else {
                m1BC = 0;
            }
            for (int j = 0; j < (int)section->mChannels.size(); j++) {
                Channel* channel = section->GetChannel_unk(j);
                if (channel->m10 == 0) {
                    return;
                }
                for (int k = 0; k < (int)channel->mActions.size(); k++) {
                    Action* action = channel->GetAction_unk(k);
                    ActionProcessor* processor;
                    if (action->m0C == 0x1F) {
                        processor = fn_8010DB54(action->GetParam_unk(0)->m04.i);
                    } else {
                        processor = sProcessors_unk[action->m0C];
                    }
                    if (processor != 0) {
                        float start = action->m10;
                        float end = action->m14;
                        if ((start <= m19C && end >= m198) || (start > m19C && start <= m198) ||
                            (end > m19C && end <= m198)) {
                            processor->vf0C(action, 5);
                        }
                    }
                }
            }
        }
        if (cut->fn_8053B028() != -1) {
            fn_80541520(fn_80541198(FindCutByIndex_unk(cut->fn_8053B028())), 0);
        }
        if (m1B0 != 0) {
            if (m1B0 == 1) {
                if (m1B4 != m1A0) {
                    m1A0 = m1B4;
                }
                m19C = m1B8;
                m198 = m1B8;
                for (int i = 0; i < (int)m164.size(); i++) {
                    Actor* actor = &m164[i];
                    if (actor != 0) {
                        actor->mB8[0] = 0;
                        actor->mB8[1] = 0;
                        actor->mB8[2] = 0;
                    }
                }
            } else {
                if (m1B4 != m1A0) {
                    m1A0 = m1B4;
                    m19C = 0.0f;
                }
                m198 = m1B8;
            }
            m1B0 = 0;
        } else {
            done = 1;
        }
    }
    fn_8053E8C8(13);
}

} // namespace Event

namespace Event {

// inferred: the behaviour action does nothing by itself (its work is done by Sequence, above)
int ActionActorBehavior::vf0C(Action* arg, int phase) {
    return 0;
}

int ActionActorBehavior::vf08(Action* arg) {
    return 0;
}

} // namespace Event

