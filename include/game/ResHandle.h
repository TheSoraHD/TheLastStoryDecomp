#ifndef GAME_RESHANDLE_H
#define GAME_RESHANDLE_H

// inferred: global resource manager (lbl_8087EF10). fn_8008937C loads a resource by name
// through the manager's member at 0x10; fn_800897D8 releases it.
struct ResManager {
    unsigned char m00[0x10];
    unsigned char m10[1];
};
extern "C" ResManager* lbl_8087EF10;
extern "C" void* fn_8008937C(void* loader, const char* name);
extern "C" void fn_800897D8(ResManager* mgr, void* res);

// inferred: owning handle to a resource of the manager above
struct ResHandle {
    ResHandle() : mRes(0) {}
    ~ResHandle() {
        if (mRes != 0 && lbl_8087EF10 != 0) {
            fn_800897D8(lbl_8087EF10, mRes);
        }
    }
    void* Load(const char* name) {
        if (mRes == 0 && lbl_8087EF10 != 0) {
            mRes = fn_8008937C(lbl_8087EF10->m10, name);
            return mRes;
        }
        return 0;
    }
    void* mRes;
};

#endif
