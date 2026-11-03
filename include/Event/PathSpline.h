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
// class as WarLionPath_unk of game/Em403WarLion.h). Also embedded in the event actors (Actor at 0x5C).
struct PathSpline_unk {
    PathSpline_unk() : mNumSegments(0), mLength(0.0f) {}
    // inferred: defined in Event/ActionActorPosition.cpp (their literals in a shared header would renumber
    // the @N labels of the files that include it)
    inline atn::Vector3 Segment_unk(int i, float u);
    inline atn::Vector3 GetPos_unk(float t);
    inline void Build_unk(const atn::Vector3* pts, int n);

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
