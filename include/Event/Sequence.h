#ifndef EVENT_SEQUENCE_H
#define EVENT_SEQUENCE_H

namespace Event {

// inferred: event sequence (Event/Sequence.cpp, creates the cuts at 0x80540A08). Partial: only the
// members used by the files that include this header (layout from Event::Cut 0x8053B030).
class Sequence {
public:
    unsigned char m000[0x1A8];
    int mLabelCount; // 0x1A8: inferred: highest label index given to Event::Cut::fn_8053B030
};

} // namespace Event

#endif
