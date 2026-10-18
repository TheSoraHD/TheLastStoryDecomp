#ifndef ATN_LH_MODEL_H
#define ATN_LH_MODEL_H

#include "atn/LoadHandle.h"

namespace atn {

// inferred from __vt__Q23atn8LH_Model (0x8078FE00); no destructor declared (weak copy 0x80089C8C)
class LH_Model : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x80475E14: adds ".model" when the path has no
                                         // ".model"/".shader" extension

    void* fn_80475DF8(); // inferred: returns the loaded model data (LoadModel::m5C)
};

} // namespace atn

#endif
