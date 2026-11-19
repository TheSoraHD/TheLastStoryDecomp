#ifndef ATN_D2A_MANAGER_H
#define ATN_D2A_MANAGER_H

#include "atn/Task.h"
#include "atn/Texture_unk.h"
#include "atn/D2A_MessageTable_unk.h"

namespace atn {

class D2A_Task;
class ID2A_TaskInstance;

// inferred from __vt__Q23atn11D2A_Manager (0x80782B10); allocated with 0x1B0 bytes; singleton lbl_8087F138
class D2A_Manager : public Task {
public:
    D2A_Manager(Task* parent);
    virtual ~D2A_Manager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    Texture_unk mTextures[3][2];   // 0x48: inferred: window textures
    D2A_MessageTable_unk mTexts;   // 0x168
    D2A_Task* mIcons[10];          // 0x180: icon layouts
    D2A_Task* mCurrent;            // 0x1A8: inferred: d2anime task being updated (atn/D2A_Task.cpp)
    ID2A_TaskInstance* mInstance;  // 0x1AC: inferred: layout task being updated (D2A_TaskInstanceBase)
};

} // namespace atn

extern "C" atn::D2A_Manager* lbl_8087F138; // inferred: the instance

#endif
