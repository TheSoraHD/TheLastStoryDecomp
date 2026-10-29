#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include "game/CharaTask.h"
#include "atn/Csv.h"
#include "atn/LH_Binary.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/tokenizer.hpp>

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC6B4(const char* s);   // inferred: string hash
extern "C" GimmickCommon* fn_80435CE4(atn::Task* parent, int id, int arg); // inferred: gimmick constructors
extern "C" GimmickCommon* fn_804093B4(atn::Task* parent, int id, int arg);
extern "C" void fn_800D2338(atn::Task* task);              // inferred: deletes a task
extern "C" int fn_8049D5DC(char* name, int a, int b, int c); // inferred: name of a map
extern "C" int fn_8006AA20(const char* path);                // inferred: the file exists


typedef boost::function<void(const gmk::SeTable&)> SeTableCallback_unk;

gmk::SeTable::~SeTable() {}

// inferred from __vt__15LoadSeTableTask (0x8078CA78): reads a sound table and passes each row to
// a callback. Its members are defined in the class body (literals in @STRING@ objects).
class LoadSeTableTask : public atn::Task {
public:
    LoadSeTableTask(atn::Task* parent, const char* path, SeTableCallback_unk callback)
        : atn::Task(parent), mCallback(callback), mDone(0) {
        mLoader.vf0C(path);
    }

    // NONMATCHING: the original calls the atn::Csv constructor (0x8004203C), the gmk::SeTable
    // constructor (0x803EEB38), the tokenizer members and boost::function::operator() (0x803EF3B0,
    // not emitted here) out of line, but inlines the std::string constructor/destructor and the
    // iterator comparison; this compile inlines everything. Probably the body is in an inline
    // helper over budget (docs/inlining.md, R5 and open case).
    virtual int vf0C() {
        if (mLoader.fn_80473F50() != 0) {
            return 0;
        }
        if (mDone == 0) {
            int size = mLoader.fn_8047059C();
            const char* text = (const char*)mLoader.fn_80470580();
            atn::Csv csv(text, size);
            csv.SetData(text, size);
            while (csv.fn_8005BBF8() != 0) {
                if (*csv.fn_8005B9CC() != '\0') {
                    continue;
                }
                const char* field = csv.fn_8005B9CC();
                if (strcmp(field, "include") == 0) {
                    char path[0x100];
                    sprintf(path, "/database/gimmick/%s.csv", csv.fn_8005B9CC());
                    Create(this, path, mCallback);
                    continue;
                }
                gmk::SeTable table;
                table.mId = atoi(field + 2) * 1000 + atoi(field + 6);
                csv.fn_8005B9CC();
                table.m04 = atoi(csv.fn_8005B9CC());
                table.m08 = atoi(csv.fn_8005B9CC());
                table.m0C = atoi(csv.fn_8005B9CC());
                table.mName = csv.fn_8005B9CC();
                table.mPos.x = fn_800DC288(csv.fn_8005B9CC());
                table.mPos.y = fn_800DC288(csv.fn_8005B9CC());
                table.mPos.z = fn_800DC288(csv.fn_8005B9CC());
                table.mVolume = fn_800DC288(csv.fn_8005B9CC());
                table.mHash = fn_800DC6B4(csv.fn_8005B9CC());
                std::string flags(csv.fn_8005B9CC());
                typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
                Tokenizer tok(flags, boost::char_separator<char>(":"));
                for (Tokenizer::iterator it = tok.begin(); it != tok.end(); it++) {
                    if (*it == "2D") {
                        table.mFlags |= 1;
                    } else if (*it == "LOOP") {
                        table.mFlags |= 2;
                    } else if (*it == "FADEIN") {
                        table.mFlags |= 4;
                    } else if (*it == "PITCHUP") {
                        table.mFlags |= 8;
                    } else if (*it == "PITCHDOWN") {
                        table.mFlags |= 0x10;
                    } else if (*it == "CONTINUE") {
                        table.mFlags |= 0x20;
                    }
                }
                table.m48 = atoi(csv.fn_8005B9CC());
                mCallback(table);
            }
            mLoader.fn_80473F88();
            mDone = 1;
        }
        return fn_800D3FA4();
    }

    static LoadSeTableTask* Create(atn::Task* parent, const char* path, SeTableCallback_unk callback) {
        return new (1, "", "", 0) LoadSeTableTask(parent, path, callback);
    }

    atn::LH_Binary mLoader;        // 0x48
    SeTableCallback_unk mCallback; // 0x50
    int mDone;                     // 0x64
};

GimmickManager* GimmickManager::fn_803EDFBC(atn::Task* parent) {
    if (lbl_8087F4A0 == 0) {
        lbl_8087F4A0 = new (12, "", "", 0) GimmickManager(parent);
    }
    return lbl_8087F4A0;
}

GimmickManager::GimmickManager(atn::Task* parent)
    : atn::Task(parent), mHead(0), mCount(0), m50(2), m54(0), m58(0), mLoadTask(0), mLoading(0), m88(0) {
    m74 = mRes.Load("GimmickManager");
    mTypes.Alloc(0x44);
}

GimmickManager::~GimmickManager() {
    lbl_8087F4A0 = 0;
}

int GimmickManager::vf0C() {
    if (m88 == 0) {
        m88 = 1;
        m80 = fn_80435CE4(this, 63000, -1);
        m80->fn_800D246C(1);
        fn_803EE41C(m80);
        m84 = fn_804093B4(this, 18000, -1);
        m84->fn_800D246C(1);
        fn_803EE41C(m84);
    } else if (fn_800D3FA4() != 0) {
        m80->mFlags |= 4;
        m84->mFlags |= 4;
        return 1;
    }
    return 0;
}

void GimmickManager::vf10() {
    if (mLoading != 0 && (mLoadTask->mFlags & 2) == 2) {
        fn_800D2338(mLoadTask);
        mLoadTask = 0;
        mLoading = 0;
    }
}

void GimmickManager::vf14() {}

void GimmickManager::fn_803EE374(void* arg) {
    for (GimmickCommon* g = mHead; g != 0; g = g->m5C) {
        if ((g->mFlags & 2) == 2 && (arg == 0 || g->m20 == arg)) {
            g->Start();
        }
    }
}

void GimmickManager::fn_803EE3F0(GimmickCommon* gimmick) {
    mCount++;
    if (mHead == 0) {
        mHead = gimmick;
    } else {
        mHead->fn_803ED8D8(gimmick);
    }
}

void GimmickManager::fn_803EE41C(GimmickCommon* gimmick) {
    if (mHead != 0) {
        if (mHead == gimmick) {
            if (mHead->m5C != 0) {
                mHead = mHead->m5C;
            } else {
                mHead = 0;
            }
        } else {
            mHead->fn_803ED8F4(gimmick);
        }
    }
}

int GimmickManager::fn_803EE45C(CharaTask* query) {
    int id; // declared here: in the loop, the id and the gimmick pointer swap registers
    if (query == 0 || query->fn_8017CB2C() == 0) {
        return 0;
    }
    for (GimmickCommon* g = mHead; g != 0; g = g->m5C) {
        if (g->m50 == 6 && g->m54 != 1) {
            id = query->m58;
            if (id == g->vf64(9)) {
                return 1;
            }
        }
    }
    return 0;
}

void GimmickManager::fn_803EE514() {
    for (GimmickCommon* g = mHead; g != 0; g = g->m5C) {
        g->vf30();
    }
}

void GimmickManager::fn_803EE560() {
    for (GimmickCommon* g = mHead; g != 0; g = g->m5C) {
        g->vf34();
    }
}

// inferred: path of the sound table of a map. The pool has an extra NUL after the formats of this
// file, as in game/GimmickCommon.cpp.
static inline int __GetSePath_unk(char* path, int a, int b, int c) {
    char name[0x20];
    if (fn_8049D5DC(name, a, b, c) != 0) {
        sprintf(path, "/database/gimmick/%s_se.csv\0", name);
        return 1;
    }
    return 0;
}

void GimmickManager::fn_803EE5AC(int a, int b, int c) {
    if (m50 != a || m54 != b || m58 != c) {
        char path[0x100];
        if (__GetSePath_unk(path, a, b, c) && fn_8006AA20(path) != 0) {
            mSeTables.clear();
            mLoading = 0;
            mLoadTask = LoadSeTableTask::Create(this, path, boost::bind(&GimmickManager::fn_803EE7FC, this, _1));
            m50 = a;
            m54 = b;
            m58 = c;
            mLoading = 1;
        }
    }
}

GimmickCommon* GimmickManager::fn_803EEE10(int id, int arg) {
    for (GimmickCommon* g = mHead; g != 0; g = g->m5C) {
        if (id == g->mId && arg == g->m4C) {
            return g;
        }
    }
    return 0;
}

void* GimmickManager::fn_803EEE44(int type) {
    if (m74 == 0) {
        return 0;
    }
    if (mTypes[type] == 0) {
        char name[0x20];
        sprintf(name, "gm%03d\0", type);
        mTypes[type] = fn_8008937C(m74, name);
    }
    return mTypes[type];
}
