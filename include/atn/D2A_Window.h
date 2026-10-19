#ifndef ATN_D2A_WINDOW_H
#define ATN_D2A_WINDOW_H

#include "atn/D2A_Common.h"
#include "math/Vec.h"

namespace atn {

class D2A_Window : public D2A_Common {
public:
    D2A_Window(D2A_Data* owner, unsigned short id); // 0x801FF1B8
    virtual ~D2A_Window();
    virtual void vf18(); // inferred: draw

    // inferred: draws the window texture `id`
    void fn_801FF8F0(int id, unsigned int color, float x, float y, float z, float w, float h, int arg);

    int m12C;           // inferred: texture id
    Vector2_unk m130;   // inferred: scale (1, 1)
    Vector2_unk m138;   // inferred: offset (0, 0)
};

} // namespace atn

#endif
