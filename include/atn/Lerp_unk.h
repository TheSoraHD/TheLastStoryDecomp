#ifndef ATN_LERP_UNK_H
#define ATN_LERP_UNK_H

#include "math/Vec.h"
#include "atn/Color.h"
#include "atn/Vector4_unk.h"

// inferred: by-value operators of the colour and of the 4D vector, and (b - a) * t + a (always inlined:
// Event/Manager, MapTask::fn_804A0F80, Em209SmallGolem, PostFxColorMatrix, Rain); names invented.
// In their own header: adding them to math/Vec.h or atn/Color.h renumbers the @N labels of every unit.
namespace atn {

inline Color operator-(const Color& a, const Color& b) {
    return Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}
inline Color operator+(const Color& a, const Color& b) {
    return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}
inline Color operator*(const Color& a, float s) {
    return Color(a.r * s, a.g * s, a.b * s, a.a * s);
}
inline Vector4_unk operator-(const Vector4_unk& a, const Vector4_unk& b) {
    return Vector4_unk(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
inline Vector4_unk operator+(const Vector4_unk& a, const Vector4_unk& b) {
    return Vector4_unk(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
inline Vector4_unk operator*(const Vector4_unk& a, float s) {
    return Vector4_unk(a.x * s, a.y * s, a.z * s, a.w * s);
}
// the Vector3 overload of math/Vec.h (a + (b - a) * t) is another weak function
template <class T>
inline T Lerp_unk(const T& a, const T& b, float t) {
    return (b - a) * t + a;
}

} // namespace atn

#endif
