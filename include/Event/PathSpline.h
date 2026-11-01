#ifndef EVENT_PATHSPLINE_H
#define EVENT_PATHSPLINE_H

#include "math/Vec.h"
#include "std/vector.h"
#include "Event/PathCubic.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

namespace Event {

// inferred: lengths of the segments: a class derived from std::vector<float> (its destructor has one more
// level than those of the other vectors, as WarLionPathList_unk of game/Em403WarLion.h)
struct PathLengths_unk : public std::vector<float> {};

// inferred: natural cubic spline through the points (names invented; see game/PathData_unk.cpp; same
// class as WarLionPath_unk of game/Em403WarLion.h). Also embedded in the event actors (Actor_unk at 0x5C).
struct PathSpline_unk {
    PathSpline_unk() : mNumSegments(0), mLength(0.0f) {}
    // inferred: point of segment `i` at `u` in [0, 1] (as WarLionPath_unk::Segment_unk)
    atn::Vector3 Segment_unk(int i, float u) {
        if (i < 0) {
            return mStart;
        }
        if (i >= mNumSegments - 1) {
            return mEnd;
        }
        if (mNumSegments < 2) {
            return lbl_807C7030;
        }
        atn::Vector3 r;
        r.x = mX[i].Eval_unk(u);
        r.y = mY[i].Eval_unk(u);
        r.z = mZ[i].Eval_unk(u);
        return r;
    }
    // inferred: point at `t` in [0, 1] of the whole path (as WarLionPath_unk::GetPos_unk; inlined in
    // the weak copy 0x8055202C of Event/ActionActorPosition)
    atn::Vector3 GetPos_unk(float t) {
        if (t <= 0.0f) {
            return mStart;
        }
        if (t >= 1.0f) {
            return mEnd;
        }
        t = mLength * t;
        for (int i = 0; i < mNumSegments - 1; i++) {
            float len = mLengths[i];
            if (t <= len) {
                return Segment_unk(i, t == 0.0f ? 0.0f : t / len);
            }
            t -= len;
        }
        return mStart;
    }

    int mNumSegments;                    // 0x00
    float mLength;                       // 0x04
    atn::Vector3 mStart;                 // 0x08
    atn::Vector3 mEnd;                   // 0x14
    std::vector<PathCubic_unk> mX;       // 0x20
    std::vector<PathCubic_unk> mY;       // 0x2C
    std::vector<PathCubic_unk> mZ;       // 0x38
    PathLengths_unk mLengths;            // 0x44
};

} // namespace Event

#endif
