#ifndef ATN_D2A_FRAME_H
#define ATN_D2A_FRAME_H

#include "atn/D2A_Common.h"

namespace atn {

class D2A_Frame : public D2A_Common {
public:
    D2A_Frame(); // 0x801EFB4C
    virtual ~D2A_Frame();
    virtual void vf18(); // inferred: draw
};

} // namespace atn

#endif
