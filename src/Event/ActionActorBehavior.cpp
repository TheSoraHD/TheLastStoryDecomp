#include "game/SetConfig.h"
#include "game/GameTask.h"
#include "Event/Cut.h"
#include "Event/Section.h"
#include "Event/Channel.h"
#include "Event/Action.h"
#include "Event/Behavior.h"
#include "Event/Sequence.h"
#include "Event/Manager.h"
#include "Event/ActionProcessors_unk.h"
#include "atn/Memory.h"



extern "C" volatile float lbl_80887E20;
extern "C" volatile float lbl_80887E24;
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
extern "C" void fn_80489A50(void* manager, int arg1, int arg2);
extern "C" void fn_8048F9B0(void* manager, void* event);
extern "C" void fn_80484FBC(void* manager);
extern "C" void fn_804870F4(void* manager);
extern "C" void fn_80486F50(void* manager);
extern "C" void fn_8048F184(void* manager, int arg);
extern "C" void fn_8048FC7C(void* manager);
extern "C" void* lbl_8087F0A8;
extern "C" void* lbl_8087F430;


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

// inferred: method of Event::Sequence (layout still read through offsets here)
void Event::Sequence::fn_805501DC(int enabled) {
    void* self = this;
    int& state = *reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 0x1C0);
    if (state == 0 && enabled != 0) {
        lbl_8087F540->fn_8047F8C0(1);
    } else if (state != 0 && enabled == 0) {
        lbl_8087F540->fn_8047F8C0(0);
    }
    state = enabled;
}

// NONMATCHING: 65.17% - manager gating, action windows, cut changes and timeouts are reconstructed;
// exact MWCC register allocation and nested action-state dispatch still differ.
void Event::Sequence::fn_80550254() {
    void* self = this;
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned int& flags = *reinterpret_cast<unsigned int*>(object + 0x98);
    flags |= 0x00040000;
    if (SetConfig::sInstance != 0 && SetConfig::sInstance->mEventCameraControl == 0) {
        flags &= ~0x00040000;
    }

    void* manager = lbl_8087F540;
    void* current = *reinterpret_cast<void**>(object + 0x1C0);
    if (current != 0) {
        int type = *reinterpret_cast<int*>(static_cast<unsigned char*>(current) + 0x0C);
        void* handler;
        if (type == 0x1F) {
            unsigned char* first = *reinterpret_cast<int*>(static_cast<unsigned char*>(current) + 0x30) > 0
                ? *reinterpret_cast<unsigned char**>(static_cast<unsigned char*>(current) + 0x2C) : 0;
            handler = *reinterpret_cast<void**>(first + 4);
        } else {
            handler = Event::sProcessors_unk[type];
        }
        if (handler == 0 || reinterpret_cast<int (*)(void*, void*)>(
                (*reinterpret_cast<void***>(handler))[2])(handler, current) != 3) {
            if (current != 0) {
                ((Manager*)manager)->fn_8047F8C0(0);
            }
            *reinterpret_cast<void**>(object + 0x1C0) = 0;
        } else {
            return;
        }
    }

    static_cast<Event::Sequence*>(self)->fn_8054FBE0(1);
    float& time = *reinterpret_cast<float*>(object + 0x198);
    time += *reinterpret_cast<float*>(static_cast<unsigned char*>(manager) + 0xB4);
    float& previousTime = *reinterpret_cast<float*>(object + 0x19C);
    if (previousTime > time) {
        previousTime = lbl_80887E20;
    }
    static_cast<Event::Sequence*>(self)->fn_8054FFE0();
    int mode = *reinterpret_cast<int*>(object + 0x50);
    if (mode == 1 || mode == 3) {
        ((Manager*)manager)->fn_80481A20();
        fn_80489A50(manager, 0, 0);
        ((Manager*)manager)->fn_80488DB8();
    }

    unsigned char* event = static_cast<unsigned char*>((void*)fn_80541210());
    if (event != 0) {
        int count = *reinterpret_cast<int*>(event + 0x40);
        unsigned char** records = *reinterpret_cast<unsigned char***>(event + 0x3C);
        for (int i = 0; i < count; ++i) {
            unsigned char* record = records[i];
            if (record == 0 || *reinterpret_cast<int*>(record + 0x14) == 0) {
                break;
            }
            if (*reinterpret_cast<int*>(record + 0x0C) == 3) {
                int key = *reinterpret_cast<int*>(record + 0x10);
                int objectCount = *reinterpret_cast<int*>(object + 0x168);
                unsigned char* objects = *reinterpret_cast<unsigned char**>(object + 0x164);
                int selected = 0;
                for (; selected < objectCount; ++selected) {
                    if (*reinterpret_cast<int*>(objects + selected * 0x1C0 + 0x14) == key) {
                        break;
                    }
                }
                *reinterpret_cast<int*>(object + 0x1BC) = selected < objectCount ? selected * 0x1C0 : 0;
            } else {
                *reinterpret_cast<int*>(object + 0x1BC) = 0;
            }

            int groupCount = *reinterpret_cast<int*>(record + 0x1C);
            unsigned char** groups = *reinterpret_cast<unsigned char***>(record + 0x18);
            for (int j = 0; j < groupCount; ++j) {
                unsigned char* group = groups[j];
                if (group == 0 || *reinterpret_cast<int*>(group + 0x10) == 0) {
                    break;
                }
                int actionCount = *reinterpret_cast<int*>(group + 0x18);
                unsigned char** actions = *reinterpret_cast<unsigned char***>(group + 0x14);
                for (int k = 0; k < actionCount; ++k) {
                    unsigned char* action = actions[k];
                    int type = *reinterpret_cast<int*>(action + 0x0C);
                    void* handler;
                    if (type == 0x1F) {
                        unsigned char* entry = *reinterpret_cast<int*>(action + 0x30) > 0
                            ? *reinterpret_cast<unsigned char**>(action + 0x2C) : 0;
                        handler = *reinterpret_cast<void**>(entry + 4);
                    } else {
                        handler = Event::sProcessors_unk[type];
                    }
                    if (handler == 0) {
                        continue;
                    }
                    float start = static_cast<float>(*reinterpret_cast<int*>(action + 0x10));
                    float end = static_cast<float>(*reinterpret_cast<int*>(action + 0x18) +
                                                   *reinterpret_cast<int*>(action + 0x10));
                    float lo = *reinterpret_cast<float*>(object + 0x19C);
                    float hi = *reinterpret_cast<float*>(object + 0x198);
                    bool starts = start >= lo - 1.0f && start < hi;
                    bool ends = end >= lo - 1.0f && end < hi;
                    int active = *reinterpret_cast<int*>(object + 0x1BC);
                    bool selected = active != 0 && start >= lo - 1.0f && end >= hi;
                    if (selected) {
                        void** table = *reinterpret_cast<void***>(handler);
                        reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 0);
                        reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 1);
                        reinterpret_cast<int (*)(void*, void*)>(table[2])(handler, action);
                        reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 2);
                        reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 3);
                    } else {
                        void** table = *reinterpret_cast<void***>(handler);
                        if (starts) reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 0);
                        if (ends) reinterpret_cast<void (*)(void*, void*, int)>(table[3])(handler, action, 1);
                        if (lo >= start && hi <= end) {
                            *reinterpret_cast<int*>(object + 0x1C0) =
                                reinterpret_cast<int (*)(void*, void*)>(table[2])(handler, action);
                        }
                        if (*reinterpret_cast<int*>(object + 0x1C0) == 3) {
                            ((Manager*)manager)->fn_8047F8C0(1);
                        }
                    }
                }
            }
        }
    }

    static_cast<Event::Sequence*>(self)->fn_8054FBE0(0);
    mode = *reinterpret_cast<int*>(object + 0x50);
    if (mode == 1 || mode == 3) {
        fn_8048F9B0(manager, event);
    }
    event = static_cast<unsigned char*>((void*)fn_80541210());
    if (event != 0 && time > static_cast<float>(*reinterpret_cast<int*>(event + 0x2C))) {
        time = static_cast<float>(*reinterpret_cast<int*>(event + 0x2C));
        int cutId = reinterpret_cast<Event::Cut*>(event)->fn_8053B028();
        if (cutId != -1) {
            int count = *reinterpret_cast<int*>(object + 0xBC);
            unsigned char** entries = *reinterpret_cast<unsigned char***>(object + 0xB8);
            int i = 0;
            for (; i < count; ++i) {
                if (*reinterpret_cast<int*>(entries[i] + 0x0C) == cutId) break;
            }
            fn_80541520(fn_80541198(0), 0);
            if (mode == 1 || mode == 3) {
                fn_80484FBC(manager); fn_804870F4(manager); fn_80486F50(manager);
                fn_8048F184(manager, 0); fn_8048F184(manager, 1); fn_8048FC7C(manager);
            }
            if (reinterpret_cast<Event::Cut*>(event)->fn_8053B028() != *reinterpret_cast<int*>(event + 0x0C)) {
                fn_80550254();
            }
        } else if (lbl_8087F0A8 != 0 && *reinterpret_cast<int*>(static_cast<unsigned char*>(lbl_8087F0A8) + 0x194) != 0 &&
                   *reinterpret_cast<int*>(object + 0x190) == 0x19D) {
            ((GameTask*)lbl_8087F430)->fn_80370B78(-1, lbl_80887E20, 0, 0);
            fn_8053E8C8(11);
        } else {
            fn_8053E8C8(13);
        }
    }
    *reinterpret_cast<float*>(object + 0x19C) = time;
}

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

