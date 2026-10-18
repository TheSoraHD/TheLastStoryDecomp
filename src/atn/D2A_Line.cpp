#include "atn/D2A_Line.h"

// inferred: 2D renderer instance and its line drawing method (class unknown)
extern "C" void* lbl_8087EEB0;
extern "C" void fn_8006144C(void* renderer, float x0, float y0, float x1, float y1, unsigned int color, float z);

namespace atn {

// inferred: converts a [0,1] float channel to 0..255 (inline, name invented)
inline unsigned int fn_ToU8(float f) {
    return (f >= 1.0f) ? 255 : (f <= 0.0f) ? 0 : (unsigned int)(f * 255.0f + 0.5f);
}

void D2A_Line::vf18() {
    if (!fn_801EDC78()) {
        return;
    }
    D2A_DrawRect rect;
    D2A_DrawColor color;
    D2A_DrawExtra extra;
    int work[6];
    fn_801ED928(&rect, &color, &extra, work);
    float x = rect.x;
    float y = rect.y;
    float right = x + rect.w;
    float bottom = y + rect.h;
    unsigned int r = fn_ToU8(color.r);
    unsigned int g = fn_ToU8(color.g);
    unsigned int b = fn_ToU8(color.b);
    unsigned int a = fn_ToU8(color.a);
    fn_8006144C(lbl_8087EEB0, x, y, right, bottom, (a << 24) | (r << 16) | (g << 8) | b, rect.z);
}

D2A_Line::~D2A_Line() {}

D2A_Line::D2A_Line() {
    mType = 4;
}

} // namespace atn
