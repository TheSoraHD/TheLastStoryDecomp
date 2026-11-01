#include "PowerPC_EABI_Support/MSL_C/MSL_Common/arith.h"


int abs(int x)
{
    return x > 0 ? x : -x;
}

//might be llabs
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
long labs(long x)
{
    return x > 0 ? x : -x;
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
s64 llabs(s64 __x){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
div_t div(s32 __numer, s32 __denom){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void ldiv(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void lldiv(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __msl_add(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __msl_ladd(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __lladd(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __msl_mul(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __msl_lmul(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __llmul(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __msl_div(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __msl_ldiv(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __lldiv(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __msl_mod(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __msl_lmod(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __llmod(){
}
#endif
