#ifndef EVENT_CUT_H
#define EVENT_CUT_H

#include "Event/Section.h"
#include "Event/Label.h"
#include "std/string.h"

namespace Event {

class Sequence;

// inferred: actions found in a cut by the event loader (Sequence::fn_80576258): kinds 0x10, 0x2F and 0x11
// (light) and 0x12 (ambient); while a cut has none of them Event::Manager::fn_8048F9B0 adjusts the
// lights itself. Copied by a weak copy constructor (0x8048FC0C). Names invented.
struct CutLightFlags_unk {
    CutLightFlags_unk() : m00(0), m01(0), m02(0), m03(0) {}
    unsigned char m00;
    unsigned char m01;
    unsigned char m02;
    unsigned char m03;
};

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
    // inferred: index checks of the lists (inlined in Event/Sequence.cpp; names invented)
    bool IsLabelIndex_unk(int i) const { return i >= 0 && i < (int)mLabels.size(); }
    bool IsSectionIndex_unk(int i) const { return i >= 0 && i < (int)mSections.size(); }
    // inferred: checked accessor (inlined in Event/EventLoader_unk.cpp; name invented)
    Section* GetSection_unk(unsigned int i) {
        if (IsSectionIndex_unk(i)) {
            return mSections[i].get();
        }
        return 0;
    }

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
    // inferred: weak copy 0x8048FC30 (Event/Manager.cpp); name invented
    const CutLightFlags_unk& GetLightFlags_unk() { return mLightFlags; }

    CutLightFlags_unk mLightFlags; // 0x48: inferred: set by the event loader (Event/EventLoader_unk.cpp)
};

} // namespace Event

#endif
