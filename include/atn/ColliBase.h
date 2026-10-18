#ifndef ATN_COLLIBASE_H
#define ATN_COLLIBASE_H

#include "atn/LH_ColliTree.h"

namespace atn {

// Layout inferred from the inlined constructor in ColliTree::ColliTree (0x80058A1C).
class ColliBase {
public:
    virtual ~ColliBase(); // 0x80056E40
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();

    int m04;
    unsigned int mFlags; // 0x08
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    unsigned char m28[0x14];
};

class ColliTree : public ColliBase {
public:
    ColliTree(); // 0x80058A1C
    virtual ~ColliTree() {}

    void fn_80058B78(const char* path); // inferred: starts loading the tree
    int fn_80058BBC();                  // inferred: nonzero while loading

    LH_ColliTree mLoader; // 0x3C
    void* mTree;          // 0x44
    float m48[12];
};

} // namespace atn

#endif
