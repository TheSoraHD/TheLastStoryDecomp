#include "game/CharaStatus.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

CharaStatusBase::CharaStatusBase() {
    memset(m1A8, 0, sizeof(m1A8));
    memset(this, 0, sizeof(*this));
}

CharaStatus::~CharaStatus() {}
