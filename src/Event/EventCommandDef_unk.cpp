// inferred file name (splits-auto-A): definitions of the event commands (effect.play, actor.motion,
// dialog.text...) and of their attributes; the readers look them up by command id. Only the two
// lookups and the __sinit that hashes the names are code.
// Generated from the DOL data by a script (see docs/batch-auto-20kb-game.md); .rodata, .data,
// .sdata2 and their relocations are identical to the original.
// NONMATCHING: __sinit 99.96% - only the last store (hash of dialog.text) is scheduled before the
// `addi r11` of the epilogue instead of after it (also with other -ipa settings).
#include "Event/Node.h"

namespace Event {

// inferred: definition of an event command (0x28 bytes, table 0x80775208)
struct CommandDef_unk {
    int mId;                       // 0x00
    const char* mLabel;            // 0x04: inferred: name shown by the tools
    const char* mName;             // 0x08: xml name
    unsigned int mHash;            // 0x0C: fn_800DC880(mName, 789456)
    int m10;                       // 0x10: inferred: category (1 effect/audio, 2 camera...)
    int mParamCount;               // 0x14
    ParamDef_unk* mParams;         // 0x18
    int mVariantParam;             // 0x1C: inferred: attribute that selects the variant, -1 none
    int mVariantCount;             // 0x20
    ParamDef_unk** mVariants;      // 0x24: inferred: attributes of each variant
};

static ParamDef_unk sCommonParams_unk[5] = {
    {"name", fn_800DC880("name"), 0x1C, 0},
    {"frame_start", fn_800DC880("frame_start"), 0, 0},
    {"frame_end", fn_800DC880("frame_end"), 0, 0},
    {"frame_in", fn_800DC880("frame_in"), 0, 0},
    {"frame_out", fn_800DC880("frame_out"), 0, 0},
};

static const ParamEnum_unk sEffectPlayParamsContinueTypeNames_unk[2] = {
    {"再生", "play"},
    {"停止", "stop"},
};

static const ParamEnum_unk sEffectPlayParamsContinueSlotNames_unk[4] = {
    {"0", "0"},
    {"1", "1"},
    {"2", "2"},
    {"3", "3"},
};

static ParamDef_unk sEffectPlayParams_unk[15] = {
    {"id", fn_800DC880("id"), 0x10, 0},
    {"actor", fn_800DC880("actor"), 7, 0},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"offset_x", fn_800DC880("offset_x"), 4, 0},
    {"offset_y", fn_800DC880("offset_y"), 5, 0},
    {"offset_z", fn_800DC880("offset_z"), 6, 0},
    {"scale", fn_800DC880("scale"), 1, 1.0f},
    {"fix_rot", fn_800DC880("fix_rot"), 2, 0},
    {"rot_x", fn_800DC880("rot_x"), 3, 0},
    {"rot_y", fn_800DC880("rot_y"), 3, 0},
    {"rot_z", fn_800DC880("rot_z"), 3, 0},
    {"stop", fn_800DC880("stop"), 2, 0},
    {"continue", fn_800DC880("continue"), 2, 0},
    {"continue_type", fn_800DC880("continue_type"), 0x1F, 0, 2, sEffectPlayParamsContinueTypeNames_unk},
    {"continue_slot", fn_800DC880("continue_slot"), 0x1F, 0, 4, sEffectPlayParamsContinueSlotNames_unk},
};

static ParamDef_unk sAnimPlayParams_unk[4] = {
    {"id", fn_800DC880("id"), 0x15, 0},
    {"speed", fn_800DC880("speed"), 1, 1.0f},
    {"loop", fn_800DC880("loop"), 2, 1},
    {"continue", fn_800DC880("continue"), 2, 0},
};

static ParamDef_unk sMessageChoiceParams_unk[5] = {
    {"id", fn_800DC880("id"), 0x11, 0},
    {"label1", fn_800DC880("label1"), 8, -1},
    {"label2", fn_800DC880("label2"), 8, -1},
    {"label3", fn_800DC880("label3"), 8, -1},
    {"timeout", fn_800DC880("timeout"), 8, -1},
};

static ParamDef_unk sMessageWindowParams_unk[2] = {
    {"id", fn_800DC880("id"), 0x11, 0},
    {"continue", fn_800DC880("continue"), 2, 0},
};

static const ParamEnum_unk sMessagePrintParamsAlignNames_unk[3] = {
    {"center", "center"},
    {"left", "left"},
    {"right", "right"},
};

static const ParamEnum_unk sMessagePrintParamsLayoutNames_unk[3] = {
    {"bottom/center", "bottom/center"},
    {"middle/center", "middle/center"},
    {"bottom/right", "bottom/right"},
};

static ParamDef_unk sMessagePrintParams_unk[8] = {
    {"num", fn_800DC880("num"), 0, 0},
    {"id", fn_800DC880("id"), 0x11, 0},
    {"pos_x", fn_800DC880("pos_x"), 1, 0.0f},
    {"offset", fn_800DC880("offset"), 1, 0.0f},
    {"layout", fn_800DC880("layout"), 0x1F, 0, 3, sMessagePrintParamsLayoutNames_unk},
    {"align", fn_800DC880("align"), 0x1F, 0, 3, sMessagePrintParamsAlignNames_unk},
    {"color", fn_800DC880("color"), 0x14, -1},
    {"continue", fn_800DC880("continue"), 2, 0},
};

static ParamDef_unk sAudioSoundParams_unk[12] = {
    {"id", fn_800DC880("id"), 0xD, 0},
    {"3d", fn_800DC880("3d"), 2, 1},
    {"actor", fn_800DC880("actor"), 7, 0},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"offset_x", fn_800DC880("offset_x"), 4, 0},
    {"offset_y", fn_800DC880("offset_y"), 5, 0},
    {"offset_z", fn_800DC880("offset_z"), 6, 0},
    {"volume", fn_800DC880("volume"), 1, 1.0f},
    {"reverb", fn_800DC880("reverb"), 1, 0.0f},
    {"pitch", fn_800DC880("pitch"), 1, 1.0f},
    {"forget", fn_800DC880("forget"), 2, 1},
    {"speaker", fn_800DC880("speaker"), 2, 0},
};

static ParamDef_unk sAudioMusicParams_unk[2] = {
    {"id", fn_800DC880("id"), 0xE, 0},
    {"volume", fn_800DC880("volume"), 1, 1.0f},
};

static const ParamEnum_unk sAudioEnvParamsTypeNames_unk[5] = {
    {"event play", "event play"},
    {"event stop", "event stop"},
    {"map ambient", "map ambient"},
    {"map 3d", "map 3d"},
    {"foot step", "foot step"},
};

static const ParamEnum_unk sAudioEnvParams0SlotNames_unk[4] = {
    {"1", "1"},
    {"2", "2"},
    {"3", "3"},
    {"4", "4"},
};

static ParamDef_unk sAudioEnvParams_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 5, sAudioEnvParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x22},
    {"param2", fn_800DC880("param2"), 0x22},
    {"param3", fn_800DC880("param3"), 0x22},
    {"param4", fn_800DC880("param4"), 0x22},
    {"param5", fn_800DC880("param5"), 0x22},
};

static ParamDef_unk sAudioEnvParams0_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 5, sAudioEnvParamsTypeNames_unk},
    {"slot", fn_800DC880("slot"), 0x1F, 0, 4, sAudioEnvParams0SlotNames_unk},
    {"id", fn_800DC880("id"), 0xD, 0},
    {"volume", fn_800DC880("volume"), 1, 1.0f},
    {"reverb", fn_800DC880("reverb"), 1, 0.0f},
    {"pitch", fn_800DC880("pitch"), 1, 1.0f},
};

static ParamDef_unk sAudioEnvParams1_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 5, sAudioEnvParamsTypeNames_unk},
    {"slot", fn_800DC880("slot"), 0x1F, 0, 4, sAudioEnvParams0SlotNames_unk},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sAudioEnvParams2_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 5, sAudioEnvParamsTypeNames_unk},
    {"enable", fn_800DC880("enable"), 2, 1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sAudioEnvParams3_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 5, sAudioEnvParamsTypeNames_unk},
    {"enable", fn_800DC880("enable"), 2, 1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sAudioEnvParams4_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 5, sAudioEnvParamsTypeNames_unk},
    {"enable", fn_800DC880("enable"), 2, 1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk* sAudioEnvVariants_unk[6] = {
    sAudioEnvParams0_unk,
    sAudioEnvParams1_unk,
    sAudioEnvParams2_unk,
    sAudioEnvParams3_unk,
    sAudioEnvParams4_unk,
};

static ParamDef_unk sAudioEffectParams_unk[1] = {
    {"reverb", fn_800DC880("reverb"), 0xF, 1},
};

static ParamDef_unk sAudioVoiceParams_unk[12] = {
    {"id", fn_800DC880("id"), 0xD, 0},
    {"3d", fn_800DC880("3d"), 2, 0},
    {"actor", fn_800DC880("actor"), 7, 0},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"offset_x", fn_800DC880("offset_x"), 4, 0},
    {"offset_y", fn_800DC880("offset_y"), 5, 0},
    {"offset_z", fn_800DC880("offset_z"), 6, 0},
    {"volume", fn_800DC880("volume"), 1, 1.0f},
    {"reverb", fn_800DC880("reverb"), 1, 0.0f},
    {"pitch", fn_800DC880("pitch"), 1, 1.0f},
    {"forget", fn_800DC880("forget"), 2, 1},
    {"lips", fn_800DC880("lips"), 2, 1},
};

static const ParamEnum_unk sAudioMultiParamsTypeNames_unk[2] = {
    {"play", "play"},
    {"stop", "stop"},
};

static ParamDef_unk sAudioMultiParams_unk[7] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 2, sAudioMultiParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x22},
    {"param2", fn_800DC880("param2"), 0x22},
    {"param3", fn_800DC880("param3"), 0x22},
    {"param4", fn_800DC880("param4"), 0x22},
    {"param5", fn_800DC880("param5"), 0x22},
    {"param6", fn_800DC880("param6"), 0x22},
};

static ParamDef_unk sAudioMultiParams0_unk[7] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 2, sAudioMultiParamsTypeNames_unk},
    {"id", fn_800DC880("id"), 0xD, 0},
    {"volume", fn_800DC880("volume"), 1, 1.0f},
    {"stopBgm", fn_800DC880("stopBgm"), 2, 0},
    {"stop2d", fn_800DC880("stop2d"), 2, 0},
    {"stop3d", fn_800DC880("stop3d"), 2, 0},
    {"stopStep", fn_800DC880("stopStep"), 2, 0},
};

static ParamDef_unk sAudioMultiParams1_unk[7] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 2, sAudioMultiParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x28},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
};

static ParamDef_unk* sAudioMultiVariants_unk[2] = {
    sAudioMultiParams0_unk,
    sAudioMultiParams1_unk,
};

static const ParamEnum_unk sMoviePlayParamsSizeNames_unk[3] = {
    {"full screen", "full screen"},
    {"center", "center"},
    {"fit width", "fit width"},
};

static ParamDef_unk sMoviePlayParams_unk[4] = {
    {"movie", fn_800DC880("movie"), 0xB, 0},
    {"size", fn_800DC880("size"), 0x1F, 0, 3, sMoviePlayParamsSizeNames_unk},
    {"loop", fn_800DC880("loop"), 2, 0},
    {"volume", fn_800DC880("volume"), 1, 1.0f},
};

static const ParamEnum_unk sCheckParamsTypeNames_unk[4] = {
    {"jump", "jump"},
    {"target", "target"},
    {"key press", "key press"},
    {"pad motion", "pad motion"},
};

static ParamDef_unk sCheckParams_unk[9] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sCheckParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x22},
    {"param2", fn_800DC880("param2"), 0x22},
    {"param3", fn_800DC880("param3"), 0x22},
    {"param4", fn_800DC880("param4"), 0x22},
    {"param5", fn_800DC880("param5"), 0x22},
    {"param6", fn_800DC880("param6"), 0x22},
    {"param7", fn_800DC880("param7"), 0x22},
    {"param8", fn_800DC880("param8"), 0x22},
};

static ParamDef_unk sCheckParams0_unk[9] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sCheckParamsTypeNames_unk},
    {"label", fn_800DC880("label"), 8, -1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
};

static ParamDef_unk sCheckParams1_unk[9] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sCheckParamsTypeNames_unk},
    {"label", fn_800DC880("label"), 8, -1},
    {"target_actor", fn_800DC880("target_actor"), 7, 0},
    {"target_node", fn_800DC880("target_node"), 0x13, 0},
    {"target_offset_x", fn_800DC880("target_offset_x"), 4, 0.0f},
    {"target_offset_y", fn_800DC880("target_offset_y"), 5, 0.0f},
    {"target_offset_z", fn_800DC880("target_offset_z"), 6, 0.0f},
    {"radius", fn_800DC880("radius"), 1, 1e+01f},
    {"param8", fn_800DC880("param8"), 0x28},
};

static const ParamEnum_unk sCheckParams2ActionNames_unk[5] = {
    {"press count", "press count"},
    {"press timer", "press timer"},
    {"release count", "release count"},
    {"release timer", "release timer"},
    {"hold count", "hold count"},
};

static ParamDef_unk sCheckParams2_unk[9] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sCheckParamsTypeNames_unk},
    {"key", fn_800DC880("key"), 0x12, 0},
    {"action", fn_800DC880("action"), 0x1F, 0, 5, sCheckParams2ActionNames_unk},
    {"count", fn_800DC880("count"), 0, 0},
    {"timer", fn_800DC880("timer"), 0, 0},
    {"label", fn_800DC880("label"), 8, -1},
    {"failure", fn_800DC880("failure"), 2, 0},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
};

static const ParamEnum_unk sCheckParams3SourceNames_unk[6] = {
    {"wiimote_x", "wiimote_x"},
    {"wiimote_y", "wiimote_y"},
    {"wiimote_z", "wiimote_z"},
    {"nunchuk_x", "nunchuk_x"},
    {"nunchuk_y", "nunchuk_y"},
    {"nunchuk_z", "nunchuk_z"},
};

static const ParamEnum_unk sCheckParams3OperatorNames_unk[1] = {
    {"sign_change", "sign_change"},
};

static ParamDef_unk sCheckParams3_unk[9] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sCheckParamsTypeNames_unk},
    {"source", fn_800DC880("source"), 0x1F, 0, 6, sCheckParams3SourceNames_unk},
    {"operator", fn_800DC880("operator"), 0x1F, 0, 1, sCheckParams3OperatorNames_unk},
    {"count", fn_800DC880("count"), 0, 0},
    {"timer", fn_800DC880("timer"), 0, 0},
    {"label", fn_800DC880("label"), 8, -1},
    {"failure", fn_800DC880("failure"), 2, 0},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
};

static ParamDef_unk* sCheckVariants_unk[4] = {
    sCheckParams0_unk,
    sCheckParams1_unk,
    sCheckParams2_unk,
    sCheckParams3_unk,
};

static const ParamEnum_unk sNotifyParamsOperatorNames_unk[5] = {
    {"set", "set"},
    {"add", "add"},
    {"substract", "substract"},
    {"multiply", "multiply"},
    {"divide", "divide"},
};

static ParamDef_unk sNotifyParams_unk[2] = {
    {"operator", fn_800DC880("operator"), 0x1F, 0, 5, sNotifyParamsOperatorNames_unk},
    {"value", fn_800DC880("value"), 0, 0},
};

static ParamDef_unk sGodRayParams_unk[2] = {
    {"color", fn_800DC880("color"), 0x14, -1},
    {"blur", fn_800DC880("blur"), 1, -3.0f},
};

static ParamDef_unk sLightSunParams_unk[6] = {
    {"area", fn_800DC880("area"), 0x18, -1},
    {"light", fn_800DC880("light"), 0, 0},
    {"dir_x", fn_800DC880("dir_x"), 1, -0.5774f},
    {"dir_y", fn_800DC880("dir_y"), 1, -0.5774f},
    {"dir_z", fn_800DC880("dir_z"), 1, -0.5774f},
    {"color", fn_800DC880("color"), 0x14, -1},
};

static ParamDef_unk sLightAmbientParams_unk[2] = {
    {"area", fn_800DC880("area"), 0x18, -1},
    {"color", fn_800DC880("color"), 0x14, -1},
};

static ParamDef_unk sLightSpotParams_unk[6] = {
    {"target_actor", fn_800DC880("target_actor"), 7, 0},
    {"target_node", fn_800DC880("target_node"), 0x13, 0},
    {"target_offset_x", fn_800DC880("target_offset_x"), 4, 0.0f},
    {"target_offset_y", fn_800DC880("target_offset_y"), 5, 0.0f},
    {"target_offset_z", fn_800DC880("target_offset_z"), 6, 0.0f},
    {"size", fn_800DC880("size"), 1, 0.0f},
};

static const ParamEnum_unk sLightPointParamsNumberNames_unk[2] = {
    {"0", "0"},
    {"1", "1"},
};

static ParamDef_unk sLightPointParams_unk[8] = {
    {"number", fn_800DC880("number"), 0x1F, 0, 2, sLightPointParamsNumberNames_unk},
    {"target_actor", fn_800DC880("target_actor"), 7, 0},
    {"target_node", fn_800DC880("target_node"), 0x13, 0},
    {"target_offset_x", fn_800DC880("target_offset_x"), 4, 0.0f},
    {"target_offset_y", fn_800DC880("target_offset_y"), 5, 0.0f},
    {"target_offset_z", fn_800DC880("target_offset_z"), 6, 0.0f},
    {"radius", fn_800DC880("radius"), 1, 0.0f},
    {"color", fn_800DC880("color"), 0x14, -1},
};

static ParamDef_unk sLightSpot2Params_unk[11] = {
    {"target_actor", fn_800DC880("target_actor"), 7, 0},
    {"target_node", fn_800DC880("target_node"), 0x13, 0},
    {"target_offset_x", fn_800DC880("target_offset_x"), 4, 0.0f},
    {"target_offset_y", fn_800DC880("target_offset_y"), 5, 0.0f},
    {"target_offset_z", fn_800DC880("target_offset_z"), 6, 0.0f},
    {"dir_x", fn_800DC880("dir_x"), 1, 0.0f},
    {"dir_y", fn_800DC880("dir_y"), 1, 1.0f},
    {"dir_z", fn_800DC880("dir_z"), 1, 0.0f},
    {"length", fn_800DC880("length"), 1, 2e+02f},
    {"angle", fn_800DC880("angle"), 1, 1e+02f},
    {"color", fn_800DC880("color"), 0x14, -1},
};

static ParamDef_unk sLightSurroundParams_unk[1] = {
    {"color", fn_800DC880("color"), 0x14, -1},
};

static const ParamEnum_unk sBrightnessParamsBrightnessNames_unk[4] = {
    {"普通", "SCALE_1"},
    {"やや明るい", "SCALE_2"},
    {"明るい", "SCALE_4"},
    {"暗い", "DIVIDE_2"},
};

static ParamDef_unk sBrightnessParams_unk[1] = {
    {"brightness", fn_800DC880("brightness"), 0x1F, 0, 4, sBrightnessParamsBrightnessNames_unk},
};

static const ParamEnum_unk sColorMatrixParamsPresetNames_unk[13] = {
    {"custom", "custom"},
    {"default", "default"},
    {"grayscale", "grayscale"},
    {"sepia", "sepia"},
    {"negative", "negative"},
    {"brightness -0.5", "brightness -0.5"},
    {"brightness -0.2", "brightness -0.2"},
    {"brightness +0.2", "brightness +0.2"},
    {"brightness +0.5", "brightness +0.5"},
    {"constrast -0.5", "constrast -0.5"},
    {"constrast -0.2", "constrast -0.2"},
    {"constrast +1.2", "constrast +1.2"},
    {"constrast +2.0", "constrast +2.0"},
};

static ParamDef_unk sColorMatrixParams_unk[13] = {
    {"preset", fn_800DC880("preset"), 0x1F, 0, 13, sColorMatrixParamsPresetNames_unk},
    {"r_r", fn_800DC880("r_r"), 1, 1.0f},
    {"r_g", fn_800DC880("r_g"), 1, 0.0f},
    {"r_b", fn_800DC880("r_b"), 1, 0.0f},
    {"g_r", fn_800DC880("g_r"), 1, 0.0f},
    {"g_g", fn_800DC880("g_g"), 1, 1.0f},
    {"g_b", fn_800DC880("g_b"), 1, 0.0f},
    {"b_r", fn_800DC880("b_r"), 1, 0.0f},
    {"b_g", fn_800DC880("b_g"), 1, 0.0f},
    {"b_b", fn_800DC880("b_b"), 1, 1.0f},
    {"r_add", fn_800DC880("r_add"), 1, 0.0f},
    {"g_add", fn_800DC880("g_add"), 1, 0.0f},
    {"b_add", fn_800DC880("b_add"), 1, 0.0f},
};

static const ParamEnum_unk sColorGradationParamsNumberNames_unk[2] = {
    {"0", "0"},
    {"1", "1"},
};

static ParamDef_unk sColorGradationParams_unk[9] = {
    {"enable", fn_800DC880("enable"), 2, 1},
    {"number", fn_800DC880("number"), 0x1F, 0, 2, sColorGradationParamsNumberNames_unk},
    {"pos_x", fn_800DC880("pos_x"), 1, 3.2e+02f},
    {"pos_y", fn_800DC880("pos_y"), 1, 2.4e+02f},
    {"width", fn_800DC880("width"), 1, 6.4e+02f},
    {"height", fn_800DC880("height"), 1, 4.8e+02f},
    {"rotation", fn_800DC880("rotation"), 1, 0.0f},
    {"color", fn_800DC880("color"), 0x14, (int)0xFF000000},
    {"ratio", fn_800DC880("ratio"), 1, 0.8f},
};

static ParamDef_unk sLocatorHideParams_unk[12] = {
    {"loc_01", fn_800DC880("loc_01"), 0x19, -1},
    {"loc_02", fn_800DC880("loc_02"), 0x19, -1},
    {"loc_03", fn_800DC880("loc_03"), 0x19, -1},
    {"loc_04", fn_800DC880("loc_04"), 0x19, -1},
    {"loc_05", fn_800DC880("loc_05"), 0x19, -1},
    {"loc_06", fn_800DC880("loc_06"), 0x19, -1},
    {"loc_07", fn_800DC880("loc_07"), 0x19, -1},
    {"loc_08", fn_800DC880("loc_08"), 0x19, -1},
    {"loc_09", fn_800DC880("loc_09"), 0x19, -1},
    {"loc_10", fn_800DC880("loc_10"), 0x19, -1},
    {"loc_11", fn_800DC880("loc_11"), 0x19, -1},
    {"loc_12", fn_800DC880("loc_12"), 0x19, -1},
};

static ParamDef_unk sMapReflectParams_unk[1] = {
    {"reflect", fn_800DC880("reflect"), 2, 0},
};

static ParamDef_unk sSettingParams_unk[1] = {
    {"speed", fn_800DC880("speed"), 1, 1.0f},
};

static ParamDef_unk sAdjustParams_unk[1] = {
    {"apply_abbreviation", fn_800DC880("apply_abbreviation"), 2, 0},
};

static const ParamEnum_unk sCameraModeParamsModeNames_unk[6] = {
    {"target", "target"},
    {"firstPerson", "firstPerson"},
    {"zoom", "zoom"},
    {"gameplay", "gameplay"},
    {"path", "path"},
    {"movable", "movable"},
};

static const ParamEnum_unk sCameraModeParamsTransitionNames_unk[4] = {
    {"none", "none"},
    {"linear", "linear"},
    {"distance", "distance"},
    {"angular", "angular"},
};

static ParamDef_unk sCameraModeParams_unk[16] = {
    {"mode", fn_800DC880("mode"), 0x1F, 0, 6, sCameraModeParamsModeNames_unk},
    {"transition", fn_800DC880("transition"), 0x1F, 0, 4, sCameraModeParamsTransitionNames_unk},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"param1", fn_800DC880("param1"), 0x22},
    {"param2", fn_800DC880("param2"), 0x22},
    {"param3", fn_800DC880("param3"), 0x22},
    {"param4", fn_800DC880("param4"), 0x22},
    {"param5", fn_800DC880("param5"), 0x22},
    {"param6", fn_800DC880("param6"), 0x22},
    {"param7", fn_800DC880("param7"), 0x22},
    {"param8", fn_800DC880("param8"), 0x22},
    {"param9", fn_800DC880("param9"), 0x22},
    {"param10", fn_800DC880("param10"), 0x22},
    {"param11", fn_800DC880("param11"), 0x22},
    {"param12", fn_800DC880("param12"), 0x22},
    {"param13", fn_800DC880("param13"), 0x22},
};

static ParamDef_unk sCameraModeParams0_unk[16] = {
    {"mode", fn_800DC880("mode"), 0x1F, 0, 6, sCameraModeParamsModeNames_unk},
    {"transition", fn_800DC880("transition"), 0x1F, 0, 4, sCameraModeParamsTransitionNames_unk},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"position_actor", fn_800DC880("position_actor"), 7, 0},
    {"position_node", fn_800DC880("position_node"), 0x13, 0},
    {"position_offset_x", fn_800DC880("position_offset_x"), 4, 0.0f},
    {"position_offset_y", fn_800DC880("position_offset_y"), 5, 0.0f},
    {"position_offset_z", fn_800DC880("position_offset_z"), 6, 0.0f},
    {"target_actor", fn_800DC880("target_actor"), 7, 0},
    {"target_node", fn_800DC880("target_node"), 0x13, 0},
    {"target_offset_x", fn_800DC880("target_offset_x"), 4, 0.0f},
    {"target_offset_y", fn_800DC880("target_offset_y"), 5, 0.0f},
    {"target_offset_z", fn_800DC880("target_offset_z"), 6, 0.0f},
    {"rotation_z", fn_800DC880("rotation_z"), 3, 0.0f},
    {"no_reset", fn_800DC880("no_reset"), 2, 0},
    {"movement", fn_800DC880("movement"), 0x1B, 0},
};

static ParamDef_unk sCameraModeParams1_unk[16] = {
    {"mode", fn_800DC880("mode"), 0x1F, 0, 6, sCameraModeParamsModeNames_unk},
    {"transition", fn_800DC880("transition"), 0x1F, 0, 4, sCameraModeParamsTransitionNames_unk},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"position_actor", fn_800DC880("position_actor"), 7, 0},
    {"position_node", fn_800DC880("position_node"), 0x13, 0},
    {"position_offset_x", fn_800DC880("position_offset_x"), 4, 0.0f},
    {"position_offset_y", fn_800DC880("position_offset_y"), 5, 0.0f},
    {"position_offset_z", fn_800DC880("position_offset_z"), 6, 0.0f},
    {"limit_x", fn_800DC880("limit_x"), 3, 8e+01f},
    {"limit_z", fn_800DC880("limit_z"), 3, 1.2e+02f},
    {"noflash", fn_800DC880("noflash"), 2, 0},
    {"continue", fn_800DC880("continue"), 2, 0},
    {"no_reset", fn_800DC880("no_reset"), 2, 0},
    {"ap_head", fn_800DC880("ap_head"), 2, 0},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
};

static ParamDef_unk sCameraModeParams2_unk[16] = {
    {"mode", fn_800DC880("mode"), 0x1F, 0, 6, sCameraModeParamsModeNames_unk},
    {"transition", fn_800DC880("transition"), 0x1F, 0, 4, sCameraModeParamsTransitionNames_unk},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"target_actor", fn_800DC880("target_actor"), 7, 0},
    {"target_node", fn_800DC880("target_node"), 0x13, 0},
    {"target_offset_x", fn_800DC880("target_offset_x"), 4, 0.0f},
    {"target_offset_y", fn_800DC880("target_offset_y"), 5, 0.0f},
    {"target_offset_z", fn_800DC880("target_offset_z"), 6, 0.0f},
    {"rotation_z", fn_800DC880("rotation_z"), 3, 0.0f},
    {"distance", fn_800DC880("distance"), 1, 1.0f},
    {"no_reset", fn_800DC880("no_reset"), 2, 0},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
};

static ParamDef_unk sCameraModeParams3_unk[16] = {
    {"mode", fn_800DC880("mode"), 0x1F, 0, 6, sCameraModeParamsModeNames_unk},
    {"transition", fn_800DC880("transition"), 0x1F, 0, 4, sCameraModeParamsTransitionNames_unk},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"no_reset", fn_800DC880("no_reset"), 2, 0},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
};

static ParamDef_unk sCameraModeParams4_unk[16] = {
    {"mode", fn_800DC880("mode"), 0x1F, 0, 6, sCameraModeParamsModeNames_unk},
    {"transition", fn_800DC880("transition"), 0x1F, 0, 4, sCameraModeParamsTransitionNames_unk},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"pos_path", fn_800DC880("pos_path"), 0xC, -1},
    {"tar_path", fn_800DC880("tar_path"), 0xC, -1},
    {"target_actor", fn_800DC880("target_actor"), 7, 0},
    {"target_node", fn_800DC880("target_node"), 0x13, 0},
    {"target_offset_x", fn_800DC880("target_offset_x"), 4, 0.0f},
    {"target_offset_y", fn_800DC880("target_offset_y"), 5, 0.0f},
    {"target_offset_z", fn_800DC880("target_offset_z"), 6, 0.0f},
    {"rotation_z", fn_800DC880("rotation_z"), 3, 0.0f},
    {"movement", fn_800DC880("movement"), 0x1B, 0},
    {"no_reset", fn_800DC880("no_reset"), 2, 0},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
};

static ParamDef_unk sCameraModeParams5_unk[16] = {
    {"mode", fn_800DC880("mode"), 0x1F, 0, 6, sCameraModeParamsModeNames_unk},
    {"transition", fn_800DC880("transition"), 0x1F, 0, 4, sCameraModeParamsTransitionNames_unk},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"pos_path", fn_800DC880("pos_path"), 0xC, -1},
    {"tar_path", fn_800DC880("tar_path"), 0xC, -1},
    {"target_actor", fn_800DC880("target_actor"), 7, 0},
    {"target_node", fn_800DC880("target_node"), 0x13, 0},
    {"target_offset_x", fn_800DC880("target_offset_x"), 4, 0.0f},
    {"target_offset_y", fn_800DC880("target_offset_y"), 5, 0.0f},
    {"target_offset_z", fn_800DC880("target_offset_z"), 6, 0.0f},
    {"rotation_z", fn_800DC880("rotation_z"), 3, 0.0f},
    {"movement", fn_800DC880("movement"), 0x1B, 0},
    {"speed", fn_800DC880("speed"), 1, 1.0f},
    {"start", fn_800DC880("start"), 1, 0.5f},
    {"no_reset", fn_800DC880("no_reset"), 2, 0},
    {"param13", fn_800DC880("param13"), 0x28},
};

static ParamDef_unk* sCameraModeVariants_unk[6] = {
    sCameraModeParams0_unk,
    sCameraModeParams1_unk,
    sCameraModeParams2_unk,
    sCameraModeParams3_unk,
    sCameraModeParams4_unk,
    sCameraModeParams5_unk,
};

static const ParamEnum_unk sCameraEffectParamsTypeNames_unk[8] = {
    {"blur", "blur"},
    {"bloom", "bloom"},
    {"fog", "fog"},
    {"capture fade", "fade"},
    {"damage", "damage"},
    {"rain", "rain"},
    {"haze", "haze"},
    {"color fade", "color fade"},
};

static ParamDef_unk sCameraEffectParams_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x22},
    {"param2", fn_800DC880("param2"), 0x22},
    {"param3", fn_800DC880("param3"), 0x22},
    {"param4", fn_800DC880("param4"), 0x22},
    {"param5", fn_800DC880("param5"), 0x22},
    {"param6", fn_800DC880("param6"), 0x22},
    {"param7", fn_800DC880("param7"), 0x22},
    {"param8", fn_800DC880("param8"), 0x22},
    {"param9", fn_800DC880("param9"), 0x22},
    {"param10", fn_800DC880("param10"), 0x22},
    {"param11", fn_800DC880("param11"), 0x22},
    {"param12", fn_800DC880("param12"), 0x22},
    {"param13", fn_800DC880("param13"), 0x22},
    {"param14", fn_800DC880("param14"), 0x22},
};

static ParamDef_unk sCameraEffectParams0_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"blur", fn_800DC880("blur"), 2, 0},
    {"scale", fn_800DC880("scale"), 1, 1.0f},
    {"shift", fn_800DC880("shift"), 1, 0.0f},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
};

static const ParamEnum_unk sCameraEffectParams1Bias1Names_unk[3] = {
    {"0", "ZERO"},
    {"1", "ADDHALF"},
    {"2", "SUBHALF"},
};

static const ParamEnum_unk sCameraEffectParams1Scale1Names_unk[4] = {
    {"0", "SCALE_1"},
    {"1", "SCALE_2"},
    {"2", "SCALE_4"},
    {"3", "DIVIDE_2"},
};

static ParamDef_unk sCameraEffectParams1_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"bloom", fn_800DC880("bloom"), 2, 0},
    {"color", fn_800DC880("color"), 0x14, -1},
    {"bias1", fn_800DC880("bias1"), 0x1F, 0, 3, sCameraEffectParams1Bias1Names_unk},
    {"bias2", fn_800DC880("bias2"), 0x1F, 0, 3, sCameraEffectParams1Bias1Names_unk},
    {"bias3", fn_800DC880("bias3"), 0x1F, 0, 3, sCameraEffectParams1Bias1Names_unk},
    {"scale1", fn_800DC880("scale1"), 0x1F, 0, 4, sCameraEffectParams1Scale1Names_unk},
    {"scale2", fn_800DC880("scale2"), 0x1F, 0, 4, sCameraEffectParams1Scale1Names_unk},
    {"scale3", fn_800DC880("scale3"), 0x1F, 0, 4, sCameraEffectParams1Scale1Names_unk},
    {"strength", fn_800DC880("strength"), 0, 1},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
};

static const ParamEnum_unk sCameraEffectParams2FogModeNames_unk[5] = {
    {"linear", "linear"},
    {"exponential", "exponential"},
    {"exponential2", "exponential2"},
    {"reverse exp", "reverse exp"},
    {"reverse exp 2", "reverse exp 2"},
};

static ParamDef_unk sCameraEffectParams2_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"fog_enable", fn_800DC880("fog_enable"), 2, 0},
    {"fog_mode", fn_800DC880("fog_mode"), 0x1F, 0, 5, sCameraEffectParams2FogModeNames_unk},
    {"fog_start", fn_800DC880("fog_start"), 1, 6e+02f},
    {"fog_end", fn_800DC880("fog_end"), 1, 1e+03f},
    {"fog_colot", fn_800DC880("fog_colot"), 0x14, -1},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
};

static ParamDef_unk sCameraEffectParams3_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x28},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
};

static ParamDef_unk sCameraEffectParams4_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"color", fn_800DC880("color"), 0x14, -1},
    {"angle", fn_800DC880("angle"), 3, 0.0f},
    {"scale", fn_800DC880("scale"), 1, 0.0f},
    {"zoom", fn_800DC880("zoom"), 1, 0.0f},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
};

static ParamDef_unk sCameraEffectParams5_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"enable", fn_800DC880("enable"), 2, 0},
    {"rate", fn_800DC880("rate"), 1, 0.05f},
    {"warp", fn_800DC880("warp"), 1, 1.0f},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
};

static ParamDef_unk sCameraEffectParams6_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"enable", fn_800DC880("enable"), 2, 0},
    {"warp", fn_800DC880("warp"), 1, 0.2f},
    {"scale_x", fn_800DC880("scale_x"), 1, 1e+01f},
    {"scale_y", fn_800DC880("scale_y"), 1, 5.0f},
    {"scroll_x", fn_800DC880("scroll_x"), 1, 0.0f},
    {"scroll_y", fn_800DC880("scroll_y"), 1, 0.01f},
    {"position_x", fn_800DC880("position_x"), 1, 0.0f},
    {"position_y", fn_800DC880("position_y"), 1, 224.0f},
    {"size_x", fn_800DC880("size_x"), 1, 6.4e+02f},
    {"size_y", fn_800DC880("size_y"), 1, 224.0f},
    {"fade_x", fn_800DC880("fade_x"), 1, 0.0f},
    {"fade_y", fn_800DC880("fade_y"), 1, 0.0f},
    {"fade_z", fn_800DC880("fade_z"), 1, 1.0f},
    {"fade_w", fn_800DC880("fade_w"), 1, 1.0f},
};

static ParamDef_unk sCameraEffectParams7_unk[15] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 8, sCameraEffectParamsTypeNames_unk},
    {"color", fn_800DC880("color"), 0x14, (int)0xFF000000},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
};

static ParamDef_unk* sCameraEffectVariants_unk[8] = {
    sCameraEffectParams0_unk,
    sCameraEffectParams1_unk,
    sCameraEffectParams2_unk,
    sCameraEffectParams3_unk,
    sCameraEffectParams4_unk,
    sCameraEffectParams5_unk,
    sCameraEffectParams6_unk,
    sCameraEffectParams7_unk,
};

static ParamDef_unk sCameraShakeParams_unk[8] = {
    {"amp_x", fn_800DC880("amp_x"), 1, 0.0f},
    {"amp_y", fn_800DC880("amp_y"), 1, 0.0f},
    {"amp_z", fn_800DC880("amp_z"), 1, 0.0f},
    {"amp_rnd", fn_800DC880("amp_rnd"), 1, 0.0f},
    {"frq_x", fn_800DC880("frq_x"), 1, 0.0f},
    {"frq_y", fn_800DC880("frq_y"), 1, 0.0f},
    {"frq_z", fn_800DC880("frq_z"), 1, 0.0f},
    {"frq_rnd", fn_800DC880("frq_rnd"), 1, 0.0f},
};

static ParamDef_unk sCameraSettingParams_unk[5] = {
    {"reset", fn_800DC880("reset"), 2, 0},
    {"fov", fn_800DC880("fov"), 3, 0.0f},
    {"aspect", fn_800DC880("aspect"), 1, 0.0f},
    {"near_clip", fn_800DC880("near_clip"), 1, 0.0f},
    {"far_clip", fn_800DC880("far_clip"), 1, 0.0f},
};

static const ParamEnum_unk sCameraFocusParamsBlurWHNames_unk[5] = {
    {"0.5/0.5", "0.5/0.5"},
    {"1.0/1.0", "1.0/1.0"},
    {"1.5/1.5", "1.5/1.5"},
    {"0.1/0.1", "0.1/0.1"},
    {"0.01/0.01", "0.01/0.01"},
};

static ParamDef_unk sCameraFocusParams_unk[7] = {
    {"enable", fn_800DC880("enable"), 2, 1},
    {"far_start", fn_800DC880("far_start"), 1, 2e+02f},
    {"far_end", fn_800DC880("far_end"), 1, 6e+02f},
    {"near_start", fn_800DC880("near_start"), 1, 5e+01f},
    {"near_end", fn_800DC880("near_end"), 1, 1.0f},
    {"blur_w/h", fn_800DC880("blur_w/h"), 0x1F, 2, 5, sCameraFocusParamsBlurWHNames_unk},
    {"clear_far", fn_800DC880("clear_far"), 2, 0},
};

static ParamDef_unk sCameraMotionParams_unk[6] = {
    {"id", fn_800DC880("id"), 0x16, 0},
    {"speed", fn_800DC880("speed"), 1, 1.0f},
    {"trans", fn_800DC880("trans"), 1, 1.0f},
    {"rotation", fn_800DC880("rotation"), 1, 1.0f},
    {"start", fn_800DC880("start"), 0, 0},
    {"global", fn_800DC880("global"), 2, 0},
};

static const ParamEnum_unk sCameraChangeParamsTypeNames_unk[3] = {
    {"pad", "pad"},
    {"nearest", "nearest"},
    {"no collide", "no collide"},
};

static const ParamEnum_unk sCameraChangeParamsCamera1Names_unk[5] = {
    {"none", "none"},
    {"main", "main"},
    {"sub 1", "sub 1"},
    {"sub 2", "sub 2"},
    {"sub 3", "sub 3"},
};

static ParamDef_unk sCameraChangeParams_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 3, sCameraChangeParamsTypeNames_unk},
    {"camera1", fn_800DC880("camera1"), 0x1F, 0, 5, sCameraChangeParamsCamera1Names_unk},
    {"camera2", fn_800DC880("camera2"), 0x1F, 0, 5, sCameraChangeParamsCamera1Names_unk},
    {"camera3", fn_800DC880("camera3"), 0x1F, 0, 5, sCameraChangeParamsCamera1Names_unk},
    {"camera4", fn_800DC880("camera4"), 0x1F, 0, 5, sCameraChangeParamsCamera1Names_unk},
    {"timer", fn_800DC880("timer"), 0, 30},
};

static ParamDef_unk sCameraInertiaParams_unk[2] = {
    {"accel", fn_800DC880("accel"), 1, 1.0f},
    {"decel", fn_800DC880("decel"), 1, 1.0f},
};

static const ParamEnum_unk sDialogSpeakerParamsMoodNames_unk[7] = {
    {"デフォルト", "default"},
    {"怒り", "angry"},
    {"哀しい", "sad"},
    {"笑い", "happy"},
    {"special1", "special1"},
    {"special2", "special2"},
    {"special3", "special3"},
};

static const ParamEnum_unk sDialogSpeakerParamsMoodIdNames_unk[4] = {
    {"1", "0"},
    {"2", "1"},
    {"3", "2"},
    {"4", "3"},
};

static const ParamEnum_unk sDialogSpeakerParamsSceneNames_unk[3] = {
    {"Normal", "normal"},
    {"Dark", "dark"},
    {"Bright", "bright"},
};

static ParamDef_unk sDialogSpeakerParams_unk[5] = {
    {"actor", fn_800DC880("actor"), 7, 0},
    {"mood", fn_800DC880("mood"), 0x1F, 0, 7, sDialogSpeakerParamsMoodNames_unk},
    {"replace", fn_800DC880("replace"), 2, 0},
    {"mood_id", fn_800DC880("mood_id"), 0x1F, 0, 4, sDialogSpeakerParamsMoodIdNames_unk},
    {"scene", fn_800DC880("scene"), 0x1F, 0, 3, sDialogSpeakerParamsSceneNames_unk},
};

static const ParamEnum_unk sDialogTextParamsAnimNames_unk[4] = {
    {"static", "static"},
    {"from left", "from left"},
    {"from right", "from right"},
    {"from top", "from top"},
};

static const ParamEnum_unk sDialogTextParamsSizeNames_unk[3] = {
    {"small", "small"},
    {"normal", "normal"},
    {"big", "big"},
};

static const ParamEnum_unk sDialogTextParamsStyleNames_unk[2] = {
    {"normal", "normal"},
    {"bold", "bold"},
};

static const ParamEnum_unk sDialogTextParamsAppendNames_unk[3] = {
    {"line break", "line break"},
    {"next", "next"},
    {"next (with space)", "next (with space)"},
};

static ParamDef_unk sDialogTextParams_unk[7] = {
    {"id", fn_800DC880("id"), 0x11, 0},
    {"anim", fn_800DC880("anim"), 0x1F, 0, 4, sDialogTextParamsAnimNames_unk},
    {"size", fn_800DC880("size"), 0x1F, 1, 3, sDialogTextParamsSizeNames_unk},
    {"style", fn_800DC880("style"), 0x1F, 0, 2, sDialogTextParamsStyleNames_unk},
    {"color", fn_800DC880("color"), 0x14, -1},
    {"append", fn_800DC880("append"), 0x1F, 0, 3, sDialogTextParamsAppendNames_unk},
    {"speed", fn_800DC880("speed"), 1, 0.5f},
};

static const ParamEnum_unk sActorBehaviorParamsTypeNames_unk[3] = {
    {"moveto", "moveto"},
    {"follow", "follow"},
    {"gesture", "gesture"},
};

static ParamDef_unk sActorBehaviorParams_unk[11] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 3, sActorBehaviorParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x22},
    {"param2", fn_800DC880("param2"), 0x22},
    {"param3", fn_800DC880("param3"), 0x22},
    {"param4", fn_800DC880("param4"), 0x22},
    {"param5", fn_800DC880("param5"), 0x22},
    {"param6", fn_800DC880("param6"), 0x22},
    {"param7", fn_800DC880("param7"), 0x22},
    {"param8", fn_800DC880("param8"), 0x22},
    {"param9", fn_800DC880("param9"), 0x22},
    {"param10", fn_800DC880("param10"), 0x22},
};

static const ParamEnum_unk sActorBehaviorParams0PaceNames_unk[2] = {
    {"walk", "walk"},
    {"run", "run"},
};

static ParamDef_unk sActorBehaviorParams0_unk[11] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 3, sActorBehaviorParamsTypeNames_unk},
    {"target", fn_800DC880("target"), 7, 0},
    {"offset_x", fn_800DC880("offset_x"), 4, 0},
    {"offset_y", fn_800DC880("offset_y"), 5, 0},
    {"offset_z", fn_800DC880("offset_z"), 6, 0},
    {"distance", fn_800DC880("distance"), 1, 1.0f},
    {"pace", fn_800DC880("pace"), 0x1F, 0, 2, sActorBehaviorParams0PaceNames_unk},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
};

static ParamDef_unk sActorBehaviorParams1_unk[11] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 3, sActorBehaviorParamsTypeNames_unk},
    {"target", fn_800DC880("target"), 7, 0},
    {"offset_x", fn_800DC880("offset_x"), 4, 0},
    {"offset_y", fn_800DC880("offset_y"), 5, 0},
    {"offset_z", fn_800DC880("offset_z"), 6, 0},
    {"distance", fn_800DC880("distance"), 1, 1.0f},
    {"pace", fn_800DC880("pace"), 0x1F, 0, 2, sActorBehaviorParams0PaceNames_unk},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
};

static ParamDef_unk sActorBehaviorParams2_unk[11] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 3, sActorBehaviorParamsTypeNames_unk},
    {"key1", fn_800DC880("key1"), 0x12, 0},
    {"motion1", fn_800DC880("motion1"), 9, 0},
    {"key2", fn_800DC880("key2"), 0x12, 0},
    {"motion2", fn_800DC880("motion2"), 9, 0},
    {"key3", fn_800DC880("key3"), 0x12, 0},
    {"motion 3", fn_800DC880("motion 3"), 9, 0},
    {"key 4", fn_800DC880("key 4"), 0x12, 0},
    {"motion 4", fn_800DC880("motion 4"), 9, 0},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
};

static ParamDef_unk* sActorBehaviorVariants_unk[4] = {
    sActorBehaviorParams0_unk,
    sActorBehaviorParams1_unk,
    sActorBehaviorParams2_unk,
};

static const ParamEnum_unk sActorPositionParamsTypeNames_unk[4] = {
    {"point", "point"},
    {"path", "path"},
    {"point-position", "point-position"},
    {"point-orientation", "point-orientation"},
};

static ParamDef_unk sActorPositionParams_unk[18] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sActorPositionParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x22},
    {"param2", fn_800DC880("param2"), 0x22},
    {"param3", fn_800DC880("param3"), 0x22},
    {"param4", fn_800DC880("param4"), 0x22},
    {"param5", fn_800DC880("param5"), 0x22},
    {"param6", fn_800DC880("param6"), 0x22},
    {"param7", fn_800DC880("param7"), 0x22},
    {"param8", fn_800DC880("param8"), 0x22},
    {"param9", fn_800DC880("param9"), 0x22},
    {"param10", fn_800DC880("param10"), 0x22},
    {"param11", fn_800DC880("param11"), 0x22},
    {"param12", fn_800DC880("param12"), 0x22},
    {"param13", fn_800DC880("param13"), 0x22},
    {"param14", fn_800DC880("param14"), 0x22},
    {"param15", fn_800DC880("param15"), 0x22},
    {"param16", fn_800DC880("param16"), 0x22},
    {"param17", fn_800DC880("param17"), 0x22},
};

static ParamDef_unk sActorPositionParams0_unk[18] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sActorPositionParamsTypeNames_unk},
    {"position", fn_800DC880("position"), 7, 0},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"offset_x", fn_800DC880("offset_x"), 4, 0.0f},
    {"offset_y", fn_800DC880("offset_y"), 5, 0.0f},
    {"offset_z", fn_800DC880("offset_z"), 6, 0.0f},
    {"local_pos", fn_800DC880("local_pos"), 2, 0},
    {"ang_x", fn_800DC880("ang_x"), 3, 0.0f},
    {"orientation", fn_800DC880("orientation"), 3, 0.0f},
    {"ang_z", fn_800DC880("ang_z"), 3, 0.0f},
    {"local_ang", fn_800DC880("local_ang"), 2, 0},
    {"facing", fn_800DC880("facing"), 2, 0},
    {"sync_use", fn_800DC880("sync_use"), 2, 0},
    {"sync_mot", fn_800DC880("sync_mot"), 9, 0},
    {"temp_x", fn_800DC880("temp_x"), 3, 0.0f},
    {"temp_y", fn_800DC880("temp_y"), 3, 0.0f},
    {"temp_z", fn_800DC880("temp_z"), 3, 0.0f},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
};

static const ParamEnum_unk sActorPositionParams1FacingNames_unk[3] = {
    {"none", "none"},
    {"horizontal", "horizontal"},
    {"3d", "3d"},
};

static ParamDef_unk sActorPositionParams1_unk[18] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sActorPositionParamsTypeNames_unk},
    {"path", fn_800DC880("path"), 0xC, -1},
    {"facing", fn_800DC880("facing"), 0x1F, 0, 3, sActorPositionParams1FacingNames_unk},
    {"sync_use", fn_800DC880("sync_use"), 2, 0},
    {"sync_mot", fn_800DC880("sync_mot"), 9, 0},
    {"param5", fn_800DC880("param5"), 0x28},
    {"param6", fn_800DC880("param6"), 0x28},
    {"param7", fn_800DC880("param7"), 0x28},
    {"param8", fn_800DC880("param8"), 0x28},
    {"param9", fn_800DC880("param9"), 0x28},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
    {"param15", fn_800DC880("param15"), 0x28},
    {"param16", fn_800DC880("param16"), 0x28},
    {"param17", fn_800DC880("param17"), 0x28},
};

static ParamDef_unk sActorPositionParams2_unk[18] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sActorPositionParamsTypeNames_unk},
    {"position", fn_800DC880("position"), 7, 0},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"offset_x", fn_800DC880("offset_x"), 4, 0.0f},
    {"offset_y", fn_800DC880("offset_y"), 5, 0.0f},
    {"offset_z", fn_800DC880("offset_z"), 6, 0.0f},
    {"local_pos", fn_800DC880("local_pos"), 2, 0},
    {"facing", fn_800DC880("facing"), 2, 0},
    {"sync_use", fn_800DC880("sync_use"), 2, 0},
    {"sync_mot", fn_800DC880("sync_mot"), 9, 0},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
    {"param15", fn_800DC880("param15"), 0x28},
    {"param16", fn_800DC880("param16"), 0x28},
    {"param17", fn_800DC880("param17"), 0x28},
};

static ParamDef_unk sActorPositionParams3_unk[18] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 4, sActorPositionParamsTypeNames_unk},
    {"position", fn_800DC880("position"), 7, 0},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"ang_x", fn_800DC880("ang_x"), 3, 0.0f},
    {"orientation", fn_800DC880("orientation"), 3, 0.0f},
    {"ang_z", fn_800DC880("ang_z"), 3, 0.0f},
    {"local_ang", fn_800DC880("local_ang"), 2, 0},
    {"temp_x", fn_800DC880("temp_x"), 3, 0.0f},
    {"temp_y", fn_800DC880("temp_y"), 3, 0.0f},
    {"temp_z", fn_800DC880("temp_z"), 3, 0.0f},
    {"param10", fn_800DC880("param10"), 0x28},
    {"param11", fn_800DC880("param11"), 0x28},
    {"param12", fn_800DC880("param12"), 0x28},
    {"param13", fn_800DC880("param13"), 0x28},
    {"param14", fn_800DC880("param14"), 0x28},
    {"param15", fn_800DC880("param15"), 0x28},
    {"param16", fn_800DC880("param16"), 0x28},
    {"param17", fn_800DC880("param17"), 0x28},
};

static ParamDef_unk* sActorPositionVariants_unk[4] = {
    sActorPositionParams0_unk,
    sActorPositionParams1_unk,
    sActorPositionParams2_unk,
    sActorPositionParams3_unk,
};

static ParamDef_unk sActorMotionParams_unk[12] = {
    {"transition", fn_800DC880("transition"), 0, 0},
    {"motion", fn_800DC880("motion"), 9, 0},
    {"speed", fn_800DC880("speed"), 1, 1.0f},
    {"start", fn_800DC880("start"), 0, 0},
    {"loop", fn_800DC880("loop"), 2, 1},
    {"part_mot", fn_800DC880("part_mot"), 2, 0},
    {"up_mot", fn_800DC880("up_mot"), 9, 0},
    {"up_speed", fn_800DC880("up_speed"), 1, 1.0f},
    {"up_start", fn_800DC880("up_start"), 0, 0},
    {"up_loop", fn_800DC880("up_loop"), 2, 1},
    {"ref_matrix", fn_800DC880("ref_matrix"), 2, 1},
    {"eye_motion", fn_800DC880("eye_motion"), 2, 1},
};

static ParamDef_unk sActorFacialParams_unk[8] = {
    {"motion", fn_800DC880("motion"), 9, 0},
    {"speed", fn_800DC880("speed"), 1, 1.0f},
    {"start", fn_800DC880("start"), 0, 0},
    {"loop", fn_800DC880("loop"), 2, 1},
    {"eyes", fn_800DC880("eyes"), 2, 0},
    {"neck", fn_800DC880("neck"), 2, 0},
    {"hair", fn_800DC880("hair"), 2, 0},
    {"blend_max", fn_800DC880("blend_max"), 1, 1.0f},
};

static ParamDef_unk sActorEyesParams_unk[16] = {
    {"actor", fn_800DC880("actor"), 7, 0},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"offset_x", fn_800DC880("offset_x"), 4, 0.0f},
    {"offset_y", fn_800DC880("offset_y"), 5, 0.0f},
    {"offset_z", fn_800DC880("offset_z"), 6, 0.0f},
    {"continue", fn_800DC880("continue"), 2, 0},
    {"ang_x", fn_800DC880("ang_x"), 3, 0.0f},
    {"orientation", fn_800DC880("orientation"), 3, 0.0f},
    {"ang_z", fn_800DC880("ang_z"), 3, 0.0f},
    {"ang_onry", fn_800DC880("ang_onry"), 2, 0},
    {"blr_x", fn_800DC880("blr_x"), 3, 0.0f},
    {"blr_y", fn_800DC880("blr_y"), 3, 0.0f},
    {"blr_inter", fn_800DC880("blr_inter"), 1, 0.0f},
    {"cont_cam_eye", fn_800DC880("cont_cam_eye"), 2, 0.0f},
    {"eye_speed_max", fn_800DC880("eye_speed_max"), 1, 0.0f},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
};

static ParamDef_unk sActorPoseParams_unk[1] = {
    {"pose", fn_800DC880("pose"), 0xA, 0},
};

static ParamDef_unk sActorHeadingParams_unk[10] = {
    {"actor", fn_800DC880("actor"), 7, 0},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"pos_x", fn_800DC880("pos_x"), 4, 0.0f},
    {"pos_y", fn_800DC880("pos_y"), 5, 0.0f},
    {"pos_z", fn_800DC880("pos_z"), 6, 0.0f},
    {"ang_x", fn_800DC880("ang_x"), 3, 0.0f},
    {"ang_y", fn_800DC880("ang_y"), 3, 0.0f},
    {"ang_z", fn_800DC880("ang_z"), 3, 0.0f},
    {"continue", fn_800DC880("continue"), 2, 0},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
};

static ParamDef_unk sActorNodeParams_unk[9] = {
    {"node", fn_800DC880("node"), 0x13, 0},
    {"pos_x", fn_800DC880("pos_x"), 1, 0.0f},
    {"pos_y", fn_800DC880("pos_y"), 1, 0.0f},
    {"pos_z", fn_800DC880("pos_z"), 1, 0.0f},
    {"ang_x", fn_800DC880("ang_x"), 3, 0.0f},
    {"ang_y", fn_800DC880("ang_y"), 3, 0.0f},
    {"ang_z", fn_800DC880("ang_z"), 3, 0.0f},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
    {"       continue", fn_800DC880("       continue"), 2, 1},
};

static const ParamEnum_unk sActorSettingParamsTypeNames_unk[11] = {
    {"display", "display"},
    {"collision", "collision"},
    {"equip", "equip"},
    {"eyes blink", "eyes blink"},
    {"trigger", "trigger"},
    {"model effect", "model effect"},
    {"node visibility", "node visibility"},
    {"motion command", "motion command"},
    {"shield visibility", "shield visibility"},
    {"stone effect", "stone effect"},
    {"pbone", "pbone"},
};

static ParamDef_unk sActorSettingParams_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"param1", fn_800DC880("param1"), 0x22},
    {"param2", fn_800DC880("param2"), 0x22},
    {"param3", fn_800DC880("param3"), 0x22},
    {"param4", fn_800DC880("param4"), 0x22},
    {"param5", fn_800DC880("param5"), 0x22},
};

static ParamDef_unk sActorSettingParams0_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"display", fn_800DC880("display"), 2, 1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams1_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"collision", fn_800DC880("collision"), 2, 1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams2_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"equipment", fn_800DC880("equipment"), 0x17, -1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams3_unk[7] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"enable", fn_800DC880("enable"), 2, 1},
    {"frequency", fn_800DC880("frequency"), 1, 1e+01f},
    {"randomize", fn_800DC880("randomize"), 1, 0.0f},
    {"brend_R", fn_800DC880("brend_R"), 1, 1.0f},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams4_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"trigger", fn_800DC880("trigger"), 0, 1},
    {"immediate", fn_800DC880("immediate"), 2, 0},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams5_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"effect", fn_800DC880("effect"), 2, 0},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams6_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"node", fn_800DC880("node"), 0x13, 0},
    {"visible", fn_800DC880("visible"), 2, 0},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams7_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"motion", fn_800DC880("motion"), 2, 0},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams8_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"shield", fn_800DC880("shield"), 2, 1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams9_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"enable", fn_800DC880("enable"), 2, 1},
    {"param2", fn_800DC880("param2"), 0x28},
    {"param3", fn_800DC880("param3"), 0x28},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorSettingParams10_unk[6] = {
    {"type", fn_800DC880("type"), 0x1F, 0, 11, sActorSettingParamsTypeNames_unk},
    {"hair", fn_800DC880("hair"), 2, 0},
    {"earring", fn_800DC880("earring"), 2, 0},
    {"waistcloth", fn_800DC880("waistcloth"), 2, 0},
    {"param4", fn_800DC880("param4"), 0x28},
    {"param5", fn_800DC880("param5"), 0x28},
};

static ParamDef_unk sActorMaterialParams_unk[9] = {
    {"material", fn_800DC880("material"), 0x1E, 0},
    {"visible", fn_800DC880("visible"), 2, 1},
    {"def_a", fn_800DC880("def_a"), 1, 0.0f},
    {"def_r", fn_800DC880("def_r"), 1, 0.0f},
    {"def_g", fn_800DC880("def_g"), 1, 0.0f},
    {"def_b", fn_800DC880("def_b"), 1, 0.0f},
    {"reset_start", fn_800DC880("reset_start"), 2, 0},
    {"reset_original", fn_800DC880("reset_original"), 2, 0},
    {"trans_curve", fn_800DC880("trans_curve"), 0x1B, 0},
};

static ParamDef_unk* sActorSettingVariants_unk[12] = {
    sActorSettingParams0_unk,
    sActorSettingParams1_unk,
    sActorSettingParams2_unk,
    sActorSettingParams3_unk,
    sActorSettingParams4_unk,
    sActorSettingParams5_unk,
    sActorSettingParams6_unk,
    sActorSettingParams7_unk,
    sActorSettingParams8_unk,
    sActorSettingParams9_unk,
    sActorSettingParams10_unk,
};

static ParamDef_unk sActorWindParams_unk[5] = {
    {"pos_x", fn_800DC880("pos_x"), 1, 0.0f},
    {"pos_y", fn_800DC880("pos_y"), 1, 0.0f},
    {"pos_z", fn_800DC880("pos_z"), 1, 1.0f},
    {"pow_min", fn_800DC880("pow_min"), 1, 0.0f},
    {"pow_max", fn_800DC880("pow_max"), 1, 0.0f},
};

static CommandDef_unk sCommands_unk[49] = {
    {7, "Effect play", "effect.play", fn_800DC880("effect.play", 789456), 1, 0xF, sEffectPlayParams_unk, -1, 0, 0},
    {0xF, "Animation play", "anim.play", fn_800DC880("anim.play", 789456), 1, 4, sAnimPlayParams_unk, -1, 0, 0},
    {0xA, "Message choice", "message.choice", fn_800DC880("message.choice", 789456), 1, 5, sMessageChoiceParams_unk, -1, 0, 0},
    {0xB, "Message window", "message.window", fn_800DC880("message.window", 789456), 1, 2, sMessageWindowParams_unk, -1, 0, 0},
    {0xC, "Message print", "message.print", fn_800DC880("message.print", 789456), 1, 8, sMessagePrintParams_unk, -1, 0, 0},
    {1, "Sound play", "audio.sound", fn_800DC880("audio.sound", 789456), 1, 0xC, sAudioSoundParams_unk, -1, 0, 0},
    {2, "Environment SE", "audio.env", fn_800DC880("audio.env", 789456), 1, 6, sAudioEnvParams_unk, 0, 5, sAudioEnvVariants_unk},
    {4, "Music play", "audio.music", fn_800DC880("audio.music", 789456), 1, 2, sAudioMusicParams_unk, -1, 0, 0},
    {3, "Audio effect", "audio.effect", fn_800DC880("audio.effect", 789456), 1, 1, sAudioEffectParams_unk, -1, 0, 0},
    {5, "Voice play", "audio.voice", fn_800DC880("audio.voice", 789456), 1, 0xC, sAudioVoiceParams_unk, -1, 0, 0},
    {0x2B, "Multi-audio play", "audio.multi", fn_800DC880("audio.multi", 789456), 1, 7, sAudioMultiParams_unk, 0, 2, sAudioMultiVariants_unk},
    {6, "Movie play", "movie.play", fn_800DC880("movie.play", 789456), 1, 4, sMoviePlayParams_unk, -1, 0, 0},
    {8, "Check", "check", fn_800DC880("check", 789456), 1, 9, sCheckParams_unk, 0, 4, sCheckVariants_unk},
    {9, "Notify", "notify", fn_800DC880("notify", 789456), 1, 2, sNotifyParams_unk, -1, 0, 0},
    {0x30, "GodRay", "god.ray", fn_800DC880("god.ray", 789456), 1, 2, sGodRayParams_unk, -1, 0, 0},
    {0x10, "Sun light", "light.sun", fn_800DC880("light.sun", 789456), 1, 6, sLightSunParams_unk, -1, 0, 0},
    {0x12, "Ambient light", "light.ambient", fn_800DC880("light.ambient", 789456), 1, 2, sLightAmbientParams_unk, -1, 0, 0},
    {0x11, "Cloud map", "light.spot", fn_800DC880("light.spot", 789456), 1, 6, sLightSpotParams_unk, -1, 0, 0},
    {0x2F, "Point light", "light.point", fn_800DC880("light.point", 789456), 1, 8, sLightPointParams_unk, -1, 0, 0},
    {0x31, "Spot light", "light.spot2", fn_800DC880("light.spot2", 789456), 1, 0xB, sLightSpot2Params_unk, -1, 0, 0},
    {0x32, "Light surround", "light.surround", fn_800DC880("light.surround", 789456), 1, 1, sLightSurroundParams_unk, -1, 0, 0},
    {0x33, "Brightness", "brightness", fn_800DC880("brightness", 789456), 1, 1, sBrightnessParams_unk, -1, 0, 0},
    {0x13, "Color matrix", "color.matrix", fn_800DC880("color.matrix", 789456), 1, 0xD, sColorMatrixParams_unk, -1, 0, 0},
    {0x34, "Gradation", "color.gradation", fn_800DC880("color.gradation", 789456), 1, 9, sColorGradationParams_unk, -1, 0, 0},
    {0x1D, "Camera change", "camera.change", fn_800DC880("camera.change", 789456), 1, 6, sCameraChangeParams_unk, -1, 0, 0},
    {0x14, "Hide locator", "locator.hide", fn_800DC880("locator.hide", 789456), 1, 0xC, sLocatorHideParams_unk, -1, 0, 0},
    {0x2E, "Map reflect", "map.reflect", fn_800DC880("map.reflect", 789456), 1, 1, sMapReflectParams_unk, -1, 0, 0},
    {0x15, "Setting", "setting", fn_800DC880("setting", 789456), 1, 1, sSettingParams_unk, -1, 0, 0},
    {0x35, "Cut Adjust", "adjust", fn_800DC880("adjust", 789456), 1, 1, sAdjustParams_unk, -1, 0, 0},
    {0x16, "Camera mode", "camera.mode", fn_800DC880("camera.mode", 789456), 2, 0x10, sCameraModeParams_unk, 0, 6, sCameraModeVariants_unk},
    {0x1A, "Camera effect", "camera.effect", fn_800DC880("camera.effect", 789456), 2, 0xF, sCameraEffectParams_unk, 0, 8, sCameraEffectVariants_unk},
    {0x19, "Camera shake", "camera.shake", fn_800DC880("camera.shake", 789456), 2, 8, sCameraShakeParams_unk, -1, 0, 0},
    {0x1B, "Camera setting", "camera.setting", fn_800DC880("camera.setting", 789456), 2, 5, sCameraSettingParams_unk, -1, 0, 0},
    {0x18, "Camera focus", "camera.focus", fn_800DC880("camera.focus", 789456), 2, 7, sCameraFocusParams_unk, -1, 0, 0},
    {0x1C, "Camera motion", "camera.motion", fn_800DC880("camera.motion", 789456), 2, 6, sCameraMotionParams_unk, -1, 0, 0},
    {0x1E, "Camera inertia", "camera.inertia", fn_800DC880("camera.inertia", 789456), 2, 2, sCameraInertiaParams_unk, -1, 0, 0},
    {0x1F, "Actor behavior", "actor.behavior", fn_800DC880("actor.behavior", 789456), 3, 0xB, sActorBehaviorParams_unk, 0, 3, sActorBehaviorVariants_unk},
    {0x25, "Actor position", "actor.position", fn_800DC880("actor.position", 789456), 3, 0x12, sActorPositionParams_unk, 0, 4, sActorPositionVariants_unk},
    {0x21, "Actor motion", "actor.motion", fn_800DC880("actor.motion", 789456), 3, 0xC, sActorMotionParams_unk, -1, 0, 0},
    {0x20, "Actor facial", "actor.facial", fn_800DC880("actor.facial", 789456), 3, 8, sActorFacialParams_unk, -1, 0, 0},
    {0x2C, "Actor eyes", "actor.eyes", fn_800DC880("actor.eyes", 789456), 3, 0x10, sActorEyesParams_unk, -1, 0, 0},
    {0x22, "Actor pose", "actor.pose", fn_800DC880("actor.pose", 789456), 3, 1, sActorPoseParams_unk, -1, 0, 0},
    {0x23, "Actor heading", "actor.heading", fn_800DC880("actor.heading", 789456), 3, 0xA, sActorHeadingParams_unk, -1, 0, 0},
    {0x2D, "Actor node", "actor.node", fn_800DC880("actor.node", 789456), 3, 9, sActorNodeParams_unk, -1, 0, 0},
    {0x26, "Actor setting", "actor.setting", fn_800DC880("actor.setting", 789456), 3, 6, sActorSettingParams_unk, 0, 0xB, sActorSettingVariants_unk},
    {0x27, "Actor wind", "actor.wind", fn_800DC880("actor.wind", 789456), 3, 5, sActorWindParams_unk, -1, 0, 0},
    {0x36, "Actor Material", "actor.material", fn_800DC880("actor.material", 789456), 3, 9, sActorMaterialParams_unk, -1, 0, 0},
    {0x29, "Dialog speaker", "dialog.speaker", fn_800DC880("dialog.speaker", 789456), 4, 5, sDialogSpeakerParams_unk, -1, 0, 0},
    {0x2A, "Dialog text", "dialog.text", fn_800DC880("dialog.text", 789456), 4, 7, sDialogTextParams_unk, -1, 0, 0},
};

static int sCommandCount_unk = sizeof(sCommands_unk) / sizeof(sCommands_unk[0]);

} // namespace Event

// inferred: definition of the command `id`
// NONMATCHING: 94% - the original sets the index (li r5, 0) after the address of the table, here
// before it; fn_80008210 with the same loop matches. Tried: operands swapped, `table + i`, a
// local count, a pointer walk, while, break + single return.
extern "C" Event::CommandDef_unk* fn_800081C0(int id) {
    for (int i = 0; i < Event::sCommandCount_unk; i++) {
        if (id == Event::sCommands_unk[i].mId) {
            return &Event::sCommands_unk[i];
        }
    }
    return 0;
}

// inferred: attributes of the command `id` (of its variant `variant` when it is not negative);
// `count` receives the number of attributes
extern "C" Event::ParamDef_unk* fn_80008210(int id, int* count, int variant) {
    for (int i = 0; i < Event::sCommandCount_unk; i++) {
        if (id == Event::sCommands_unk[i].mId) {
            if (count != 0) {
                *count = Event::sCommands_unk[i].mParamCount;
            }
            if (variant >= 0) {
                return Event::sCommands_unk[i].mVariants[variant];
            }
            return Event::sCommands_unk[i].mParams;
        }
    }
    return 0;
}
