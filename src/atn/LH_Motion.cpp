#include "atn/LH_Motion.h"
#include "atn/LoadCore.h"
#include "std/string.h"
#include "atn/Loader_unk.h"

extern "C" int fn_800DC500(const char* name, unsigned long len); // inferred: name to id
extern "C" char* fn_806825B4(const char* s, int c);               // inferred: strrchr
extern "C" void* lbl_8087F518;                                    // inferred: resource manager
extern "C" void* fn_804766B0(void* manager, const char* path, int arg); // inferred: get/load a motion


namespace atn {

// Declared after LH_Motion (in the header): the vtables are emitted in reverse order of declaration.
// inferred: motion resource (path stored without directory and extension)
class LoadMotion : public LoadCore {
public:
    LoadMotion(const char* path); // 0x80476220
    virtual ~LoadMotion();        // 0x8047656C
    virtual void vf0C();          // 0x804765C4

    int m5C; // inferred: result of fn_800DC500 on the motion name
    int m60; // 4 for "fc_*" files, -1 otherwise
};

// inferred: file name part of a path (after the last '\\' or '/')
inline const char* __FileName_unk(const char* path) {
    const char* s = fn_806825B4(path, 0x5C);
    if (s == 0) {
        s = fn_806825B4(path, 0x2F);
        if (s == 0) {
            return path;
        }
        return s + 1;
    }
    return s + 1;
}

LoadMotion::LoadMotion(const char* path) : LoadCore(path) {
    m5C = 0;
    m60 = 0;
    std::string name(path);
    std::string::size_type pos = name.rfind('/');
    if (pos != std::string::npos) {
        name = std::string(name, pos + 1);
    }
    pos = name.rfind('.');
    if (pos != std::string::npos) {
        name.erase(pos);
    }
    m5C = fn_800DC500(name.c_str(), name.size());
    const char* file = __FileName_unk(path);
    if (strlen(file) > 3 && file[0] == 'f' && file[1] == 'c' && file[2] == '_') {
        m60 = 4;
    } else {
        m60 = -1;
    }
}

LoadMotion::~LoadMotion() {}

void LoadMotion::vf0C() {
    if (mState == 2) {
        fn_80473104(3);
        Loader_unk loader;
        fn_804714A4(&loader);
        fn_804714B0(&loader, m54, m50, LoaderCallback_unk(), 0);
    }
}

void LH_Motion::vf0C(const char* path) {
    fn_80473F88();
    mCore = fn_804766B0(lbl_8087F518, path, 1);
}

} // namespace atn
