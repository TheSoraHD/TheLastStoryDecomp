#include "db/GrowthParseGlobal.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

static const char sFileNameStr[] = "/boot/growth_curve.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static db::GrowthEntry_unk* sEntries;
static int sCount;

namespace db {

atn::BaseNode* GrowthParseGlobal::vf0C(const char* name) {
    if (strcmp(name, "Level") == 0) {
        atn::VariableNode* node = fn_800A68D4();
        node->m04 = m18;
        return node;
    }
    atn::ValueNode* node = fn_800A686C();
    node->m04 = fn_800DC288(name);
    return node;
}

} // namespace db

extern "C" void fn_8020F84C() {
    unsigned long size = 0;
    void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
    atn::Csv csv((const char*)data, size);
    int count = 0;
    db::GrowthParseGlobal tree;
    if (sEntries == 0) {
        if (strcmp(csv.fn_8005B9CC(), "Version") == 0) {
            atoi(csv.fn_8005B9CC());
        }
        csv.fn_8005BBF8();
        while (csv.fn_8005BBF8() != 0) {
            if (strcmp(csv.fn_8005B9CC(), "") != 0) {
                count++;
            }
        }
        sEntries = new (3, "", "", 0) db::GrowthEntry_unk[count];
        sCount = count;
        csv.vf08((const char*)data, size);
        csv.fn_8005BBF8();
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (strlen(field) == 0) {
                continue;
            }
            db::GrowthEntry_unk* entry = &sEntries[i];
            entry->mId = atoi(field);
            csv.fn_8005B9CC(); // inferred: a column that is not used
            tree.m18 = &entry->m40.m08;
            tree.fn_800A5E00(csv.fn_8005B9CC());
            entry->m40.m04.fn_800A6FF0(&tree);
            tree.m18 = &entry->m04.m08;
            tree.fn_800A5E00(csv.fn_8005B9CC());
            entry->m04.m04.fn_800A6FF0(&tree);
            tree.m18 = &entry->m10.m08;
            tree.fn_800A5E00(csv.fn_8005B9CC());
            entry->m10.m04.fn_800A6FF0(&tree);
            tree.m18 = &entry->m1C.m08;
            tree.fn_800A5E00(csv.fn_8005B9CC());
            entry->m1C.m04.fn_800A6FF0(&tree);
            tree.m18 = &entry->m28.m08;
            tree.fn_800A5E00(csv.fn_8005B9CC());
            entry->m28.m04.fn_800A6FF0(&tree);
            tree.m18 = &entry->m34.m08;
            tree.fn_800A5E00(csv.fn_8005B9CC());
            entry->m34.m04.fn_800A6FF0(&tree);
            i++;
        }
    }
    fn_8046DD20(lbl_8087F518, data);
}

extern "C" db::GrowthEntry_unk* fn_8020FCE8(int id) {
    for (int i = 0; i < sCount; i++) {
        if (id == sEntries[i].mId) {
            return &sEntries[i];
        }
    }
    return 0;
}

db::GrowthEntry_unk::GrowthEntry_unk() : mId(0) {}
