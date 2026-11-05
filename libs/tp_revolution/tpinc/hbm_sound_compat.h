#ifndef HBM_SOUND_COMPAT_H
#define HBM_SOUND_COMPAT_H
// Declarations the homebuttonLib sound sources of zeldaret/tp expect and its revolution headers
// lack (sync flags copied from libs/ss_nw4r/include/rvl/AX/AXVPB.h).
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif
enum AXVPBSyncFlags_et
{
	AX_VPB_SYNC_FLAG_SRC_TYPE				= (1 <<  0),
	AX_VPB_SYNC_FLAG_MIXER_CTRL				= (1 <<  1),
	AX_VPB_SYNC_FLAG_STATE					= (1 <<  2),
	AX_VPB_SYNC_FLAG_TYPE					= (1 <<  3),
	AX_VPB_SYNC_FLAG_MIX					= (1 <<  4),
	AX_VPB_SYNC_FLAG_ITD					= (1 <<  5),
	AX_VPB_SYNC_FLAG_ITD_TARGET				= (1 <<  6),
	AX_VPB_SYNC_FLAG_DPOP					= (1 <<  7),
	AX_VPB_SYNC_FLAG_VE						= (1 <<  8),
	AX_VPB_SYNC_FLAG_VE_DELTA				= (1 <<  9),
	AX_VPB_SYNC_FLAG_ADDR					= 1 << 10,
	AX_VPB_SYNC_FLAG_ADDR_LOOP_FLAG			= 1 << 11,
	AX_VPB_SYNC_FLAG_ADDR_LOOP_ADDR			= 1 << 12,
	AX_VPB_SYNC_FLAG_ADDR_END_ADDR			= 1 << 13,
	AX_VPB_SYNC_FLAG_ADDR_CURRENT_ADDR		= 1 << 14,
	AX_VPB_SYNC_FLAG_ADPCM					= 1 << 15,
	AX_VPB_SYNC_FLAG_SRC					= 1 << 16,
	AX_VPB_SYNC_FLAG_SRC_RATIO				= 1 << 17,
	AX_VPB_SYNC_FLAG_ADPCM_LOOP				= 1 << 18,
	AX_VPB_SYNC_FLAG_LPF					= 1 << 19,
	AX_VPB_SYNC_FLAG_LPF_COEFS				= 1 << 20,
	AX_VPB_SYNC_FLAG_BIQUAD					= 1 << 21,
	AX_VPB_SYNC_FLAG_BIQUAD_COEFS			= 1 << 22,
	AX_VPB_SYNC_FLAG_RMT_ON					= 1 << 23,
	AX_VPB_SYNC_FLAG_RMT_MIXER_CTRL			= 1 << 24,
	AX_VPB_SYNC_FLAG_RMT_MIX				= 1 << 25,
	AX_VPB_SYNC_FLAG_RMT_DPOP				= 1 << 26,
	AX_VPB_SYNC_FLAG_RMT_SRC				= 1 << 27,
	AX_VPB_SYNC_FLAG_RMT_IIR				= 1 << 28,
	AX_VPB_SYNC_FLAG_RMT_IIR_LPF_COEFS		= 1 << 29,
	AX_VPB_SYNC_FLAG_RMT_IIR_BIQUAD_COEFS	= 1 << 30,
	AX_VPB_SYNC_FLAG_FULL_PB				= 1 << 31
};
BOOL AXIsInit(void);
// Mixer control flags (names and values of libs/ss_nw4r/include/rvl/AX/AXPB.h) and voice priority.
#ifndef AX_PRIORITY_MAX
#define AX_PRIORITY_MAX 31
#endif
enum HBMAXMixerCtrl_et
{
	AX_MIXER_CTRL_L = 1 << 0, AX_MIXER_CTRL_R = 1 << 1, AX_MIXER_CTRL_DELTA = 1 << 2,
	AX_MIXER_CTRL_S = 1 << 3, AX_MIXER_CTRL_DELTA_S = 1 << 4,
	AX_MIXER_CTRL_A_L = 1 << 16, AX_MIXER_CTRL_A_R = 1 << 17, AX_MIXER_CTRL_A_DELTA = 1 << 18,
	AX_MIXER_CTRL_A_S = 1 << 19, AX_MIXER_CTRL_A_DELTA_S = 1 << 20,
	AX_MIXER_CTRL_B_L = 1 << 21, AX_MIXER_CTRL_B_R = 1 << 22, AX_MIXER_CTRL_B_DELTA = 1 << 23,
	AX_MIXER_CTRL_B_S = 1 << 24, AX_MIXER_CTRL_B_DELTA_S = 1 << 25,
	AX_MIXER_CTRL_C_L = 1 << 26, AX_MIXER_CTRL_C_R = 1 << 27, AX_MIXER_CTRL_C_DELTA = 1 << 28,
	AX_MIXER_CTRL_C_S = 1 << 29, AX_MIXER_CTRL_C_DELTA_S = 1 << 30
};
// Release build: the asserts of the TP sources compile to nothing.
#ifndef OSAssertMessage_Line
#define OSAssertMessage_Line(line_, cond_, ...) ((void)0)
#endif
#ifndef POINTER_ADD_TYPE
#define POINTER_ADD_TYPE(type_, ptr_, offset_) ((type_)((unsigned long)(ptr_) + (unsigned long)(offset_)))
#endif
#ifndef ATTR_UNUSED
#define ATTR_UNUSED
#endif
#ifdef __cplusplus
}
#endif
#endif
