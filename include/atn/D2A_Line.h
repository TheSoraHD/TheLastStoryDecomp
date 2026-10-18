#ifndef ATN_D2A_LINE_H
#define ATN_D2A_LINE_H

#include "atn/D2A_Common.h"

namespace atn {

class D2A_Line : public D2A_Common {
public:
    D2A_Line(); // 0x801F0AC0
    virtual ~D2A_Line();
    virtual void vf18(); // inferred: draw
};

} // namespace atn

#endif
