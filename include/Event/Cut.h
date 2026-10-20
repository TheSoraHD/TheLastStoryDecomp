#ifndef EVENT_CUT_H
#define EVENT_CUT_H

#include "Event/Section.h"
#include "Event/Label.h"
#include "std/string.h"

namespace Event {

class Sequence;

// inferred: cut of an event sequence (0x4C bytes, vtable 0x80793D98). The members after the
// index follow the attributes of the cut ("skip", "cinematic", "in_effect", ... in Cut.cpp).
class Cut : public Node {
public:
    Cut(Sequence* sequence, int index); // 0x8053AFB8
    virtual ~Cut();                     // 0x8053C208
    int fn_8053B028();                  // inferred: index of the next cut
    Label* fn_8053B030(int index);      // inferred: appends a new label and returns it
    Section* fn_8053B828(int index);    // inferred: appends a new section and returns it
    const char* fn_8053C124(int skip);  // inferred: name of a "skip" value
    void fn_Stripped_unk(std::string& path); // inferred: stripped by the linker (see Cut.cpp)

    Sequence* mSequence; // 0x08
    int mIndex;          // 0x0C
    int mSkip;           // 0x10
    int mCinematic;      // 0x14: inferred: 1 after construction
    int m18;
    int m1C;
    int m20;
    int m24;
    int mNextCut;        // 0x28: inferred: -1 after construction
    int m2C;
    std::vector<boost::shared_ptr<Label> > mLabels;     // 0x30
    std::vector<boost::shared_ptr<Section> > mSections; // 0x3C
    unsigned char m48; // inferred: flags set by the event loader (see Event/EventLoader_unk.cpp)
    unsigned char m49;
    unsigned char m4A;
    unsigned char m4B;
};

} // namespace Event

#endif
