// GameSpy SDK platform header, Revolution flavour (see libs/gamespy/license.txt).
#ifndef __GSPLATFORM_H__
#define __GSPLATFORM_H__

#include <types.h>
#include <revolution/OS.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GS_ASSERT(x)
#define GSI_UNUSED(x) (void)(x)

typedef unsigned char gsi_u8;
typedef signed char gsi_i8;
typedef unsigned short gsi_u16;
typedef signed short gsi_i16;
typedef unsigned int gsi_u32;
typedef signed int gsi_i32;
typedef unsigned int gsi_uint;
typedef signed int gsi_int;
typedef long long gsi_i64;
typedef unsigned long long gsi_u64;
typedef int gsi_bool;
typedef unsigned long gsi_time;
#define gsi_true 1
#define gsi_false 0

#define GSI_DOMAIN_NAME "gs.nintendowifi.net"

#ifdef __cplusplus
}
#endif

#endif

#ifndef __GSDEBUGFORMAT__
#define __GSDEBUGFORMAT__
// Release build: the debug output is compiled out, but the arguments are still evaluated
// (gt2AddressToString calls survive in the target).
#define gsDebugFormat(cat, type, level, ...) ((void)(__VA_ARGS__))
#endif
