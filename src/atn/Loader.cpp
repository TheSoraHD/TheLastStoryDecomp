// atn::Loader: path resolution (language suffix, override directory, data/<kind>/ prefix from the
// extension), creation of the loader, update of the pending loads, cache purge, loading screen and
// the archives opened at boot. Other methods: atn/LoadManager_unk.cpp and atn/FileSystem_unk.cpp.
#include "atn/LoadManager.h"
#include "atn/MemoryManager.h"
#include "atn/LH_Binary.h"
#include "atn/PfsArchive_unk.h"
#include "atn/FileList.h"
#include "std/string.h"
#include <string.h>

extern "C" char* strstr(const char* s, const char* sub);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" char* fn_806825B4(const char* s, int c); // inferred: strrchr
extern "C" int fn_805F9EF0(const char* path);      // inferred: DVDConvertPathToEntrynum
extern "C" void fn_8006A9A4(std::string& path);    // inferred: path normalisation
extern "C" std::string fn_8006AC08(const std::string& path); // inferred: extension of a path
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" std::string fn_8006AF38(const char* a, const std::string& b);
namespace std {
string operator+(const string& lhs, const char* rhs); // 0x8006D008
}

// inferred: engine memory manager (see atn/DwcAllocator_unk.cpp)
extern "C" void* fn_800838AC(atn::MemoryManager* mgr, unsigned long size, int heap, const char* file,
                             const char* func, int line);

extern "C" void fn_8046C1C8();
extern "C" void fn_8046C2D4(atn::Loader* mgr, atn::PfsArchive_unk* archive, atn::LoadCore* core,
                            const char* path, int index);

// inferred: d2anime helpers (see game/MenuCommonTask.cpp)
struct D2APoint_unk {
    D2APoint_unk() {}
    float x;
    float y;
    float mScaleX;
    float mScaleY;
    float m10;
};
extern "C" atn::LoaderD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop);
extern "C" D2APoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void* fn_801FEC74(void* layout, const char* name);
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" std::basic_string<wchar_t> fn_800A4450(void* text, int index, int table);
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b);
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A55D4(void* pad, int player, int button);
extern "C" int fn_800A5B38(void* pad); // inferred: battery level
extern "C" void* lbl_8087EF68;
extern "C" int fn_800A4460(void* obj);
// inferred: render manager (see game/MenuCommonTask.cpp); only the member used here
struct LoaderRender_unk {
    unsigned char m00[0x44];
    int mWide; // 0x44: inferred: 16:9 screen
};
extern "C" LoaderRender_unk* lbl_8087EEE0;

extern "C" atn::Loader* lbl_8087F518 = 0;

// inferred: accessor of the loader (weak copy fn_8020A3E4, see db/CharaParam_unk.cpp)
inline atn::Loader* ResMgr_unk() {
    return lbl_8087F518;
}

namespace atn {

// inferred: sets the value of the pane `name` (see game/MenuCommonTask.cpp)
inline void SetPaneValue_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline D2APoint_unk GetPanePos_unk(Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}

// inferred: the file exists on disc
static int fn_8046CBC0(const char* path) {
    std::string s(path);
    fn_8006A9A4(s);
    return fn_805F9EF0(s.c_str()) != -1;
}

// inferred: full path of a resource (std::string version of fn_8046D1EC). Called by
// atn/FileList.cpp (0x8006AB54).
std::string fn_8046CC90(const std::string& in) {
    std::string path = in[0] == '/' ? std::string(in.c_str() + 1) : in;
    if (ResMgr_unk() != 0 && ResMgr_unk()->fn_8046D184() == 0) {
        return path;
    }
    std::string ext = fn_8006AC08(path);
    if (ResMgr_unk() != 0 && ResMgr_unk()->fn_8046D18C() != 0) {
        std::string name = fn_8006B174(path);
        FileList list;
        list.fn_8006C3F0(ResMgr_unk()->fn_8046D194(), ext.c_str(), 0x10001);
        int index = list.fn_8006C300(name.c_str(), 0);
        if (index >= 0) {
            return std::string(list.GetStrings(index));
        }
    }
    if (ResMgr_unk() != 0 && !ResMgr_unk()->fn_8046D1C4().empty()) {
        char buf[0x100];
        ResMgr_unk()->fn_8046DBF8(buf, path.c_str());
        std::string full = ResMgr_unk()->fn_8046D1C4() + "/" + buf;
        if (fn_8046CBC0(full.c_str())) {
            return full;
        }
    }
    const char* dir = 0;
    if (ext == "model") {
        dir = "data/model/";
    } else if (ext == "texture") {
        dir = "data/texture/";
    } else if (ext == "shader") {
        dir = "data/shader/";
    } else if (ext == "efp") {
        dir = "data/efp/";
    } else if (ext == "eff") {
        dir = "data/eff/";
    } else if (ext == "effconfig") {
        dir = "data/effectconfig/";
    } else if (ext == "hocb" || ext == "hcb") {
        dir = "data/collision/";
    } else if (ext == "building") {
        dir = "data/building/";
    } else if (ext == "locator") {
        dir = "data/locator/";
    } else if (ext == "gmk") {
        dir = "data/gimmick/";
    } else if (ext == "motion") {
        dir = "data/motion/";
    } else if (ext == "area") {
        dir = "data/area/";
    } else if (ext == "map" || ext == "twn") {
        dir = "data/map/";
    } else if (ext == "chr") {
        dir = "data/character/";
    } else if (ext == "d2b") {
        dir = "data/d2anime/";
    } else if (ext == "material") {
        dir = "data/material/";
    }
    if (dir != 0) {
        return fn_8006AF38(dir, fn_8006B174(path));
    }
    return path;
}

} // namespace atn

using namespace atn;

// NONMATCHING: 99.89% - registers only: in the inlined fn_8046CBC0 the original keeps the string
// pointer in r27 and its length in r26 (swapped here); fn_8046CBC0 itself matches.
// inferred: full path of a resource: override directory, language suffix and data/<kind>/ prefix
// from the extension
extern "C" void fn_8046D1EC(char* out, const char* in) {
    const char* path = *in == '/' ? in + 1 : in;
    Loader* loader = lbl_8087F518;
    if (loader != 0 && loader->m400C == 0) {
        strcpy(out, path);
        return;
    }
    const char* ext = fn_806825B4(path, '.');
    if (ext == 0) {
        strcpy(out, path);
        return;
    }
    if (loader != 0 && lbl_8087F518->m4008 != 0) {
        std::string name = fn_8006B174(std::string(path));
        FileList list;
        list.fn_8006C3F0(lbl_8087F518->m3F08, ext + 1, 0x10001);
        int index = list.fn_8006C300(name.c_str(), 0);
        if (index >= 0) {
            strcpy(out, list.GetStrings(index));
            return;
        }
    }
    if (lbl_8087F518 != 0 && !lbl_8087F518->m4010.empty()) {
        char buf[0x100];
        lbl_8087F518->fn_8046DBF8(buf, path);
        std::string full = lbl_8087F518->m4010 + "/" + buf;
        if (fn_8046CBC0(full.c_str())) {
            strcpy(out, full.c_str());
            return;
        }
    }
    const char* dir = 0;
    if (strcmp(ext + 1, "model") == 0) {
        dir = "data/model/";
    } else if (strcmp(ext + 1, "texture") == 0) {
        dir = "data/texture/";
    } else if (strcmp(ext + 1, "shader") == 0) {
        dir = "data/shader/";
    } else if (strcmp(ext + 1, "efp") == 0) {
        dir = "data/efp/";
    } else if (strcmp(ext + 1, "eff") == 0) {
        dir = "data/eff/";
    } else if (strcmp(ext + 1, "effconfig") == 0) {
        dir = "data/effectconfig/";
    } else if (strcmp(ext + 1, "hocb") == 0 || strcmp(ext + 1, "hcb") == 0) {
        dir = "data/collision/";
    } else if (strcmp(ext + 1, "building") == 0) {
        dir = "data/building/";
    } else if (strcmp(ext + 1, "locator") == 0) {
        dir = "data/locator/";
    } else if (strcmp(ext + 1, "gmk") == 0) {
        dir = "data/gimmick/";
    } else if (strcmp(ext + 1, "motion") == 0) {
        dir = "data/motion/";
    } else if (strcmp(ext + 1, "area") == 0) {
        dir = "data/area/";
    } else if (strcmp(ext + 1, "map") == 0 || strcmp(ext + 1, "twn") == 0) {
        dir = "data/map/";
    } else if (strcmp(ext + 1, "chr") == 0) {
        dir = "data/character/";
    } else if (strcmp(ext + 1, "d2b") == 0) {
        dir = "data/d2anime/";
    } else if (strcmp(ext + 1, "material") == 0) {
        dir = "data/material/";
    }
    if (dir != 0) {
        const char* name = fn_806825B4(path, '/');
        const char* name2 = fn_806825B4(path, '\\');
        if (name < name2) {
            name = name2;
        }
        sprintf(out, "%s%s", dir, name != 0 ? name + 1 : path);
    } else {
        strcpy(out, path);
    }
}

namespace atn {

extern "C" Loader* fn_8046D7D8(Task* parent) {
    if (lbl_8087F518 == 0 && parent != 0) {
        lbl_8087F518 = new (1, "", "", 0) Loader(parent);
    }
    return lbl_8087F518;
}

// NONMATCHING: 99.72% - the original computes the "/test" pointer into r31 (the string pool base),
// here into r30 (tried the path copy as constructor body and as operator=).
// fn_8046CBB0/fn_8046CBBC are called out of line: defined in this file they are inlined, so they
// belong to another file (probably atn/LoadManager_unk.cpp, whose split ends just before them).
Loader::Loader(Task* parent)
    : Task(parent), m3ED8(0), m3EDC(0), m3EE0(0), m3EE4(0), m3EE8(60), m3EEC(0), m3EF0(0), m3EF4(1),
      m3EF8(1), m3EFC("/"), m3F08("/test"), m4008(0), m400C(1) {
    fn_8046CBB0();
}

// NONMATCHING: 98.05% - registers of the four inlined list destructors (the original gives the
// list r29 and the node/end r27/r28; here the list gets r27).
Loader::~Loader() {
    fn_8046CBBC();
    lbl_8087F518 = 0;
}

void Loader::fn_8046DBF8(char* out, const char* path) {
    strcpy(out, path);
    char* p = strstr(out, "_xx.");
    if (p != 0) {
        strncpy(p, "_jp.", 4);
    }
}

extern "C" void* fn_8046DC5C(Loader* loader, const char* path, unsigned long* size, int align) {
    LH_Binary bin;
    bin.fn_80473FCC(path);
    *size = bin.fn_8047059C();
    void* data = fn_800838AC(atn::MemoryManager::Instance(), *size, 2, "", "", 0);
    memcpy(data, bin.fn_80470580(), bin.fn_8047059C());
    bin.fn_80473F88();
    return data;
}

extern "C" void fn_8046DD20(Loader* loader, void* data) {
    if (data != 0) {
        atn::MemoryManager::Instance()->Free(data);
    }
}

int Loader::vf0C() {
    return fn_800D3FA4();
}

// inferred: the archive data is loaded (as in atn/LoadManager_unk.cpp)
inline int IsLoaded_unk(PfsArchive_unk* archive) {
    int loaded = 0;
    if (archive->mData.fn_80473EFC() == 3 && archive->mData.fn_80470580() != 0) {
        loaded = 1;
    }
    return loaded;
}

// inferred: `core` waits for an archive index
inline int IsPending_unk(Loader* loader, LoadCore* core) {
    for (std::list<PendingLoad_unk>::iterator p = loader->mPending.begin(); p != loader->mPending.end(); ++p) {
        if ((*p).mCore == core) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 96.1% - registers (this r28, iterator r27, resource r26 in the original) and the
// original tests begin != end before the first loop (loop guard); tried while/for loops, the
// pending search as goto, as inline helper.
void Loader::vf10() {
    fn_8046C1C8();
    for (std::list<LoadCore*>::iterator it = mList.begin(); it != mList.end();) {
        LoadCore* core = *it;
        if (core->GetState_unk() == 2) {
            core->vf0C();
        }
        int pending = IsPending_unk(this, core);
        if (pending == 0 && (core->GetState_unk() == 3 || core->GetState_unk() == 4)) {
            if (core->mRefCount == 0) {
                delete core;
            } else {
                fn_8046F5CC(core);
            }
            it = mList.erase(it);
        } else if (pending == 0 && core->GetState_unk() == 5) {
            delete core;
            it = mList.erase(it);
        } else {
            ++it;
        }
    }
    int count = 0;
    for (std::list<PendingLoad_unk>::iterator p = mPending.begin(); p != mPending.end();) {
        PfsArchive_unk* archive = (*p).mArchive;
        if (IsLoaded_unk(archive)) {
            fn_8046C2D4(this, (*p).mArchive, (*p).mCore, (*p).mPath.m, (*p).mIndex);
            p = mPending.erase(p);
            if (++count > 0x30) {
                break;
            }
        } else if ((*p).mCore->mRefCount <= 0) {
            mList.erase(__find_if_unk(mList.begin(), mList.end(), __HashIs_unk((*p).mCore->m48)));
            delete (*p).mCore;
            p = mPending.erase(p);
        } else {
            ++p;
        }
    }
}

// NONMATCHING: 98.45% - registers: the original swaps r28/r29 (pool base/end of the archive list)
// and the temporaries of the pending search, and copies the iterator to r3 in the loop test.
void Loader::vf14() {
    if (m3EE0 != 0 && m3EE0->IsFlag_unk(2)) {
        if (m3EF4 == 0) {
            m3EE4 = 0;
            m3EE0->mFlags |= 4;
            m3EE0->m100 = 0.0f;
            m3EEC = 0;
            m3EF0 = 0;
        }
        if (m3EE4 > 0) {
            m3EE0->m104 = 1.0f;
            if (--m3EE8 < 0 && lbl_8087EF70 != 0 && fn_800A55D4(lbl_8087EF70, 0, 4) != 0) {
                m3EE0->m104 = 3.0f;
            }
            m3EE4--;
            if (m3EE0->IsFlag_unk(4) && (lbl_8087EF68 == 0 || fn_800A4460(lbl_8087EF68) == 0)) {
                m3EE0->mFlags &= ~4;
                m3EE0->m108 = 0x13;
                if (lbl_8087EEE0 != 0) {
                    if (lbl_8087EEE0->mWide != 0) {
                        SetPaneValue_unk(m3EE0->mLayout, "ofsX", 0.0f);
                        SetPaneValue_unk(m3EE0->mLayout, "ofsY", 0.0f);
                    } else {
                        SetPaneValue_unk(m3EE0->mLayout, "ofsX", 5.0f);
                        SetPaneValue_unk(m3EE0->mLayout, "ofsY", -10.0f);
                    }
                }
            }
        } else if (!m3EE0->IsFlag_unk(4)) {
            float t = m3EE0->m100;
            if (t < 3.0f || 80.0f <= t) {
                m3EE0->mFlags |= 4;
                m3EE0->m100 = 0.0f;
                m3EE8 = 60;
                m3EEC = 0;
                m3EF0 = 0;
            }
            SetPaneValue_unk(m3EE0->mLayout, "batteryMsg", 0.0f);
        }
    }
    while (m3EDC != 0) {
        m3EDC = 0;
        fn_8046E410();
    }
    std::list<PfsArchive_unk*>::iterator it = mArchives.begin();
    std::list<PfsArchive_unk*>::iterator end = mArchives.end();
    while (it != end) {
        PfsArchive_unk* archive = *it;
        if (archive->mRefCount <= 0) {
            int pending = 0;
            for (std::list<PendingLoad_unk>::iterator p = mPending.begin(); p != mPending.end(); ++p) {
                if ((*p).mArchive == archive) {
                    pending = 1;
                    break;
                }
            }
            if (pending == 0) {
                if (archive->m74 != 0) {
                    it = mArchives.erase(it);
                    delete archive;
                } else {
                    if (IsLoaded_unk(archive)) {
                        archive->mData.fn_80473F88();
                    }
                    ++it;
                }
            } else {
                archive->fn_804702BC();
                ++it;
            }
        } else {
            archive->fn_804702BC();
            ++it;
        }
    }
}

} // namespace atn

// inferred: comparison of the cached resources (path hash)
struct LoadCoreLess_unk {
    bool operator()(atn::LoadCore* a, atn::LoadCore* b) const { return a->m48 < b->m48; }
};

template <class T>
inline void Swap_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <class T>
struct Ref_unk {
    typedef T& type;
};
template <class T>
struct Ref_unk<T&> {
    typedef T& type;
};

// inferred: median of three of the quicksort below (MSL __sort; see game/CharaTask.cpp)
template <class T, class Compare>
void Median3_unk(T* a, T* b, T* c, Compare& comp) {
    bool ca = !comp(*c, *a);
    bool bc = !comp(*b, *c);
    if (ca && bc) {
        return;
    }
    if (!ca && !bc) {
        Swap_unk(*a, *b);
    } else {
        if (comp(*b, *a)) {
            Swap_unk(*a, *b);
        }
        if (ca) {
            Swap_unk(*b, *c);
        } else {
            Swap_unk(*a, *c);
        }
    }
}

template <class T, class Compare>
inline T* MinElement_unk(T* first, T* last, Compare& comp) {
    T* result = first;
    if (first != last) {
        for (T* it = first + 1; it != last; it++) {
            if (comp(*it, *result)) {
                result = it;
            }
        }
    }
    return result;
}

// NONMATCHING: 96.45% (both instantiations) - quicksort of the cache (MSL __sort, same
// reconstruction as game/CharaTask.cpp Sort_unk, also NONMATCHING there): 0x8046E55C (comparison by
// value) and 0x8046E890 (by reference, recursive part); registers and partition loops differ.
template <class T, class Compare>
void Sort_unk(T* first, T* last, Compare comp) {
    typedef typename Ref_unk<Compare>::type CompareRef;
    static int sOffset = -4; // inferred: lbl_8087E050 and lbl_8087E054 (one per instantiation)
    while (true) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            if (first != last) {
                for (T* end = last - 1; first != end; first++) {
                    T* min = MinElement_unk(first, last, comp);
                    if (min != first) {
                        Swap_unk(*min, *first);
                    }
                }
            }
            return;
        }
        T* a = first + (n / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        T* b = first + (n * 3 / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        T* j;
        T* pivot = last - 1;
        Median3_unk(a, b, pivot, comp);
        T* i = first;
        j = pivot;
        while (comp(*i, *pivot)) {
            i++;
        }
        while (i != --j && !comp(*j, *pivot)) {
        }
        if (i < j) {
            Swap_unk(*i, *j);
            i++;
            while (true) {
                while (comp(*i, *pivot)) {
                    i++;
                }
                while (!comp(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                Swap_unk(*i, *j);
                i++;
            }
        }
        if (i == first) {
            T* back = last - 1;
            Swap_unk(*i, *pivot);
            i++;
            if (!comp(*first, *back)) {
                while (i != last && !comp(*first, *i)) {
                    i++;
                }
                if (i < back) {
                    Swap_unk(*i, *back);
                }
            }
            j = back;
            if (i < j) {
                while (true) {
                    while (!comp(*first, *i)) {
                        i++;
                    }
                    while (comp(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    Swap_unk(*i, *j);
                    i++;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            Sort_unk<T, CompareRef>(first, i, comp);
            first = i;
        } else {
            Sort_unk<T, CompareRef>(i, last, comp);
            last = i;
        }
    }
}

namespace atn {

// NONMATCHING: 95.1% - the original reloads mCount for the erase and keeps other registers for
// the erase loop (tried unsigned/int indices, end pointer as argument or computed inside).
void Loader::fn_8046E410() {
    if (m48.mCount == 0) {
        return;
    }
    LoadCore** p = m48.begin();
    LoadCore** back = &m48.mItems[m48.mCount - 1];
    unsigned int last = m48.mCount - 1;
    unsigned int i = 0;
    while (i < last) {
        LoadCore* core = m48.mItems[i];
        if (core->mRefCount == 0) {
            Swap_unk(*back, *p);
            delete core;
            back--;
            last--;
        } else {
            p++;
            i++;
        }
    }
    if (last < m48.mCount - 1) {
        m48.erase(&m48.mItems[last + 1]);
    }
    Sort_unk(m48.begin(), m48.end(), LoadCoreLess_unk());
}

void Loader::fn_8046EC7C() {
    if (m3EE0 == 0) {
        m3EE0 = fn_801F3FF8(this, "data/d2anime/ui_loading.d2a", 0);
    }
}

int Loader::fn_8046ECC8() {
    return !m3EE0->IsFlag_unk(2);
}

// NONMATCHING: 97.7% - the original loads the size argument of fn_8006EF48 before the branch of
// the inlined data() and keeps the layout pointers in r28 (tried a named wstring, a width helper).
extern "C" void fn_8046ECDC(Loader* loader, int show) {
    SetPaneValue_unk(loader->m3EE0->mLayout, "batteryMsg", 0.0f);
    void* text = fn_801FEC74(loader->m3EE0->mLayout, "string0011");
    if (text != 0) {
        D2APoint_unk pos = GetPanePos_unk(loader->m3EE0, "message_0011");
        float w = fn_8006EF48(lbl_8087EEC8, fn_800A4450(text, 0, 0).data(), pos.mScaleX, 0.0f, 1, 4);
        SetPaneValue_unk(loader->m3EE0->mLayout, "dotpos", 0, 35.0f + w);
    }
    if (show) {
        if (loader->m3EEC == 0) {
            loader->m3EF0 = 0;
            if (lbl_8087EF70 != 0 && fn_800A5B38(lbl_8087EF70) <= 1) {
                SetPaneValue_unk(loader->m3EE0->mLayout, "batteryMsg", 1.0f);
                loader->m3EF0 = 1;
            }
            loader->m3EEC = 1;
            loader->m3EE8 = 60;
        } else {
            SetPaneValue_unk(loader->m3EE0->mLayout, "batteryMsg", loader->m3EF0 != 0 ? 1.0f : 0.0f);
        }
    }
    if (loader->m3EF4 != 0) {
        loader->m3EE4 = 10;
    }
}

extern "C" void fn_8046EED8(Loader* loader) {
    loader->m3EE4 = 0;
    loader->m3EE0->mFlags |= 4;
    loader->m3EE0->m100 = 0.0f;
    loader->m3EEC = 0;
    loader->m3EF0 = 0;
}

// NONMATCHING: 98.2% - registers of the inlined std::list insert (known problem of std/list.h,
// see Loader::Get_unk in atn/LoadManager.h) and flags/archive registers swapped.
extern "C" PfsArchive_unk* fn_8046EF08(Loader* loader, const char* name, const char* index, int preload) {
    PfsArchive_unk* archive;
    int flags = 1;
    if (preload == 0) {
        flags |= 2;
    }
    archive = new (2, "", "", 0) PfsArchive_unk(name, index, flags);
    loader->mArchives.push_back(archive);
    return archive;
}

// NONMATCHING: 98.8% - registers of the inlined std::list insert (see fn_8046EF08).
// The index name of each archive comes first in the string pool
extern "C" void fn_8046F014(Loader* loader) {
    const char* index = "pack/filesystem.pkh";
    const char* name = "pack/filesystem.pk";
    loader->mMounted.push_back(new (2, "", "", 0) PfsArchive_unk(name, index, 0));
    const char* name2;
    const char* index2 = "pack/eventpacks.pkh";
    name2 = "pack/eventpacks.pk";
    loader->mMounted.push_back(new (2, "", "", 0) PfsArchive_unk(name2, index2, 0));
    const char* name3;
    const char* index3 = "pack/levels.pkh";
    name3 = "pack/levels.pk";
    loader->mMounted.push_back(new (2, "", "", 0) PfsArchive_unk(name3, index3, 0));
}

extern "C" void fn_8046F2C4(Loader* loader) {
    fn_8046EF08(loader, "preload/boot.pk", "preload/boot.pkh", 1);
    fn_8046EF08(loader, "preload/camp.pk", "preload/camp.pkh", 1);
    fn_8046EF08(loader, "preload/change_dg.pk", "preload/change_dg.pkh", 1);
    fn_8046EF08(loader, "preload/change_dg_cache.pk", "preload/change_dg_cache.pkh", 1);
    fn_8046EF08(loader, "preload/game_start_dg.pk", "preload/game_start_dg.pkh", 1);
    fn_8046EF08(loader, "preload/na000_00.pk", "preload/na000_00.pkh", 1);
    fn_8046EF08(loader, "preload/na000_00_town.pk", "preload/na000_00_town.pkh", 1);
    fn_8046EF08(loader, "preload/saveload.pk", "preload/saveload.pkh", 1);
    fn_8046EF08(loader, "preload/shop.pk", "preload/shop.pkh", 1);
    fn_8046EF08(loader, "preload/title00.pk", "preload/title00.pkh", 1);
    fn_8046EF08(loader, "preload/title01.pk", "preload/title01.pkh", 1);
    fn_8046EF08(loader, "preload/title02.pk", "preload/title02.pkh", 1);
    fn_8046EF08(loader, "preload/title03.pk", "preload/title03.pkh", 1);
    fn_8046EF08(loader, "preload/title04.pk", "preload/title04.pkh", 1);
    fn_8046EF08(loader, "preload/title05.pk", "preload/title05.pkh", 1);
    fn_8046EF08(loader, "preload/title06.pk", "preload/title06.pkh", 1);
    fn_8046EF08(loader, "preload/title07.pk", "preload/title07.pkh", 1);
    fn_8046EF08(loader, "preload/title08.pk", "preload/title08.pkh", 1);
}

} // namespace atn
