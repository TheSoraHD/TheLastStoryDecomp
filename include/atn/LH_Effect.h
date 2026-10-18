#ifndef ATN_LH_EFFECT_H
#define ATN_LH_EFFECT_H

#include "atn/LoadHandle.h"

namespace atn {

// inferred from __vt__Q23atn9LH_Effect (0x8078FCF8); no destructor declared (weak copy 0x8023756C)
class LH_Effect : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x804738E0

    void* fn_804738C4(); // inferred: returns the loaded effect data
};

} // namespace atn

#endif
