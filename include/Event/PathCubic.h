#ifndef EVENT_PATHCUBIC_H
#define EVENT_PATHCUBIC_H

namespace Event {

// inferred: cubic of one segment and axis: a0 + a1 u + a2 u^2 + a3 u^3 (also the segments of
// game/Em403WarLion.cpp, whose weak vector code uses the @STRING@ name of this class)
struct PathCubic_unk {
    // inferred: empty constructor: copied float by float
    PathCubic_unk() {}
    float Eval_unk(float u) const { return m0 + u * (m1 + u * (m2 + m3 * u)); }
    float m0;
    float m1;
    float m2;
    float m3;
};

} // namespace Event

#endif
