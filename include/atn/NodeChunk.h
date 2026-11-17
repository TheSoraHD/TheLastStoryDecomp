#ifndef ATN_NODECHUNK_H
#define ATN_NODECHUNK_H

namespace atn {

// inferred: node of a model (only the members used; names invented)
struct NodeChunk {
    unsigned char m00[0x10];
    const char* mName; // 0x10: inferred: node name (compared with "spine", game/ReplayBuffer.cpp)
    unsigned int mHash; // 0x14: inferred: hash of the node name (fn_800DC6B4; compared unsigned in gmk::SwingObject::fn_803FEF78)
};

} // namespace atn

#endif
