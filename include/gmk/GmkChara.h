#ifndef GMK_GMKCHARA_H
#define GMK_GMKCHARA_H

#include "game/CharaTask.h"
#include "std/string.h"

namespace gmk {

// inferred: character shown by a gimmick (gmk::CharaObj, gmk::CharaPathMove). The name is in the
// DOL (__vt__Q23gmk8GmkChara, 0x1428 bytes, character type 5); all its members are inline (weak
// copies at the end of gmk/CharaObj.cpp).
class GmkChara : public CharaTask {
public:
    // inferred: the path is read inside the constructor (after the CharaTask constructor)
    GmkChara(atn::Task* parent, const std::string& path) : CharaTask(parent, 5, 0) {
        fn_80136544(path.data());
        m1188.Initialize(&mMotion);
    }
    virtual ~GmkChara() {}
    virtual int vf0C() { return fn_8013655C() == 0; }
    virtual void vf10() { fn_80145334(); }
    virtual void vf14() { fn_80149A30(); }

    int m1424; // inferred: unused here (sizeof(GmkChara) == 0x1428)
};

} // namespace gmk

#endif
