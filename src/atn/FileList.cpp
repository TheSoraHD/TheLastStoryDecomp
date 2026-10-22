#include "atn/FileList.h"
#include "atn/Memory.h"
#include "std/string.h"

// File and path utilities of the engine (path normalisation, language suffixes, loaded file
// buffers) and atn::FileList (files of a directory). Functions 0x8006A950-0x8006BBCC are free
// functions called as extern "C" by the game; only some are reconstructed.

extern "C" char* strstr(const char* s, const char* sub);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);

// inferred: resource manager (lbl_8087F518, atn::Loader): releases a buffer of fn_8046DC5C
namespace atn {
// inferred: minimal view of atn::Loader (include/atn/LoadManager.h declares these functions with
// other signatures)
class Loader {
public:
    int fn_8046F4D4(const char* path); // inferred: some mounted archive has `path`
};
} // namespace atn
typedef atn::Loader Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);
struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();                  // inferred: memory manager
extern "C" void fn_80083AD4(MemoryManager_unk* mgr, void* p); // inferred: frees a block
extern "C" void* fn_800839EC(MemoryManager_unk* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line, int arg);
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" char* strcpy(char* dst, const char* src);

// inferred: DVDFileInfo of the RVL SDK (0x3C bytes; the length is at 0x34)
struct DVDFileInfo_unk {
    unsigned char m00[0x34];
    unsigned long mLength;
    unsigned char m38[4];
};
extern "C" int fn_805FA270(const char* path, DVDFileInfo_unk* info); // DVDOpen
extern "C" int DVDReadPrio(DVDFileInfo_unk* info, void* addr, long length, long offset, long prio);
extern "C" int DVDClose(DVDFileInfo_unk* info);

// inferred: language suffixes of the localised files, indexed by the language (0 Japanese)
static const char* const sLangSuffix[7] = {"_jp", "_en", "_de", "_fr", "_es", "_it", "_xx"};

extern "C" {
int lbl_8087EEC0; // inferred: language of the files
int lbl_8087EEC4; // inferred: language of the text
}

// inferred: converts the backslashes of a path to slashes
extern "C" void fn_8006A950(char* path) {
    unsigned long len = strlen(path);
    for (unsigned long i = 0; i < len; i++) {
        if (*path == '\\') {
            *path = '/';
        }
        path++;
    }
}

// inferred: replaces "_xx" in `path` by the suffix of the file language
extern "C" char* fn_8006B404(char* path) {
    char* p = strstr(path, sLangSuffix[6]);
    if (p != 0) {
        strncpy(p, sLangSuffix[lbl_8087EEC0], 3);
        return path;
    }
    return path;
}

namespace atn {
std::string fn_8046CC90(const std::string& in); // inferred: path resolved by the loader (atn/Loader.cpp)
}

// inferred: converts the backslashes of a path to slashes (std::string version)
inline void __ToSlashes_unk(std::string& path) {
    for (unsigned long i = 0; i < path.size(); i++) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }
}

extern "C" void fn_8006A9A4(std::string& path) {
    for (unsigned long i = 0; i < path.size(); i++) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }
}

extern "C" std::string fn_8006AB54(const char* path);
extern "C" int fn_805F9EF0(const char* path); // inferred: DVDConvertPathToEntrynum

// inferred: nonzero when the file exists (in a mounted archive or on the disc)
extern "C" int fn_8006AA20(const char* name) {
    std::string path = fn_8006AB54(name);
    if (lbl_8087F518 != 0 && lbl_8087F518->fn_8046F4D4(path.c_str())) {
        return 1;
    }
    __ToSlashes_unk(path);
    return fn_805F9EF0(path.c_str()) != -1;
}

// inferred: path resolved by the loader, from a C string
extern "C" std::string fn_8006AB54(const char* path) {
    return atn::fn_8046CC90(std::string(path));
}

// inferred: view of the std::string words (the operator+ bodies below read the private layout
// as the friend operator+ of std/string.h does)
struct StringView_unk {
    unsigned long mWord0; // bit 31: long string; short: size in the low 7 bits of the first byte
    unsigned long mSize;
    const char* mData;
};

// inferred: extension of a path (after the last '.', "" without one)
// NONMATCHING: 99.4% - volatile registers of the inlined rfind (r6/r7 swapped)
extern "C" std::string fn_8006AC08(const std::string& path) {
    unsigned long pos = path.rfind('.');
    if (pos != std::string::npos) {
        return std::string(path, pos + 1);
    }
    return std::string("");
}

// inferred: a + b (C string first; std::string operator+(const char*, const std::string&))
extern "C" std::string fn_8006AF38(const char* a, const std::string& b) {
    std::string tmp;
    unsigned long n = strlen(a);
    const StringView_unk& v = (const StringView_unk&)b;
    unsigned long sz;
    const char* p;
    if (!(v.mWord0 >> 31)) {
        p = (const char*)&b + 1;
        sz = *(const unsigned char*)&b & 0x7F;
    } else {
        p = v.mData;
        sz = v.mSize;
    }
    tmp.reserve(n + sz);
    tmp.replace(0, 0, a, a + n).replace(n, 0, p, p + sz);
    return tmp;
}

// inferred: a + b (std::string operator+(const std::string&, const std::string&))
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b) {
    std::string tmp;
    unsigned long na;
    unsigned long nb;
    const char* pa;
    const char* pb;
    const StringView_unk& va = (const StringView_unk&)a;
    if (!(va.mWord0 >> 31)) {
        pa = (const char*)&a + 1;
        na = *(const unsigned char*)&a & 0x7F;
    } else {
        pa = va.mData;
        na = va.mSize;
    }
    const StringView_unk& vb = (const StringView_unk&)b;
    if (!(vb.mWord0 >> 31)) {
        pb = (const char*)&b + 1;
        nb = *(const unsigned char*)&b & 0x7F;
    } else {
        pb = vb.mData;
        nb = vb.mSize;
    }
    tmp.reserve(na + nb);
    tmp.replace(0, 0, pa, pa + na).replace(na, 0, pb, pb + nb);
    return tmp;
}

// inferred: position of the last directory separator (npos: none); name invented
inline unsigned long __LastSeparator_unk(const std::string& path) {
    for (unsigned long i = 0; i < path.size(); i++) {
        unsigned long pos = path.size() - 1 - i;
        char c = path[pos];
        if (c == '\\' || c == '/') {
            return pos;
        }
    }
    return std::string::npos;
}

// inferred: directory of a path (before the last separator; npos keeps the whole path)
std::string fn_8006B0C8(const std::string& path) {
    return std::string(path, 0, __LastSeparator_unk(path));
}

// inferred: file name of a path (after the last separator)
extern "C" std::string fn_8006B174(const std::string& path) {
    unsigned long pos = __LastSeparator_unk(path);
    if (pos == std::string::npos) {
        return path;
    }
    return std::string(path, pos + 1);
}

// inferred: path without its extension
// NONMATCHING: 99.5% - volatile registers of the inlined rfind (as fn_8006AC08)
extern "C" std::string fn_8006B2D8(const std::string& path) {
    unsigned long pos = path.rfind('.');
    if (pos != std::string::npos) {
        return std::string(path, 0, pos);
    }
    return path;
}

// inferred: copy of a path with "_xx" replaced by the suffix of the file language
extern "C" std::string fn_8006B46C(const std::string& path) {
    char buf[0x100];
    strcpy(buf, path.c_str());
    char* p = strstr(buf, sLangSuffix[6]);
    if (p != 0) {
        strncpy(p, sLangSuffix[lbl_8087EEC0], 3);
    }
    return std::string(buf);
}

// inferred: sets the file language (ignored when out of range); inlined in fn_8006B53C
static inline void __SetLang_unk(int lang) {
    if (lang < 0) {
        return;
    }
    if (lang >= 6) {
        return;
    }
    lbl_8087EEC0 = lang;
}

// inferred: as fn_8006B46C with the text language
// NONMATCHING: 75.9% - the inlined range checks are merged into one cmplwi 5 here; the original keeps
// both compares (as in fn_8006BA54, where early returns give them; inlined, 7 forms tried)
extern "C" std::string fn_8006B53C(const std::string& path) {
    int lang = lbl_8087EEC0;
    __SetLang_unk(lbl_8087EEC4);
    std::string r = fn_8006B46C(path);
    __SetLang_unk(lang);
    return r;
}

extern "C" int fn_8006BA30() {
    return lbl_8087EEC0;
}

// inferred: suffix of the file language without the underscore ("jp", "en"...)
extern "C" const char* fn_8006BA38() {
    return sLangSuffix[lbl_8087EEC0] + 1;
}

// inferred: sets the file language (ignored when out of range)
extern "C" void fn_8006BA54(int lang) {
    if (lang < 0) {
        return;
    }
    if (lang >= 6) {
        return;
    }
    lbl_8087EEC0 = lang;
}

extern "C" int fn_8006BA6C() {
    return lbl_8087EEC4;
}

extern "C" void fn_8006BA74(int lang) {
    if (lang < 0) {
        return;
    }
    if (lang >= 6) {
        return;
    }
    lbl_8087EEC4 = lang;
}

// inferred: loads a whole file (through the loader unless `own` is set: then read from the disc into a
// new block rounded up to 32 bytes)
extern "C" void* fn_8006BA8C(const char* path, unsigned long* size, int own) {
    if (lbl_8087F518 != 0 && own == 0) {
        return fn_8046DC5C(lbl_8087F518, path, size, 0x20);
    }
    DVDFileInfo_unk info;
    void* data = 0;
    if (fn_805FA270(path, &info)) {
        *size = info.mLength;
        data = fn_800839EC(fn_800827E0(), (*size + 0x1F) & ~0x1F, 0x20, 1, "", "", 0, 0);
        DVDReadPrio(&info, data, (*size + 0x1F) & ~0x1F, 0, 2);
        DVDClose(&info);
    }
    return data;
}

// inferred: releases a loaded file (through the loader unless `own` is set)
extern "C" void fn_8006BB6C(void* data, int own) {
    if (lbl_8087F518 != 0 && own == 0) {
        fn_8046DD20(lbl_8087F518, data);
    } else if (data != 0) {
        fn_80083AD4(fn_800827E0(), data);
    }
}

namespace atn {

FileList::FileList() {
    m04 = 0;
    mEntries.__shrink_unk(0); // inferred: some vector call that destroys the (zero) elements
}

FileList::~FileList() {
    m04 = 0;
    mEntries.clear();
}

} // namespace atn
