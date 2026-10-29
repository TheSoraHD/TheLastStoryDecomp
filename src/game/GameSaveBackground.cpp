#include "game/GameSaveBackground.h"
#include "atn/Memory.h"

extern "C" char* strcpy(char* dst, const char* src);

// inferred: globals of unknown classes
struct Unk_8087F0A8 {
    unsigned char m000[0x198];
    int m198;
};
struct Unk_8087F518 {
    unsigned char m0000[0x3EF4];
    int m3EF4;
};
extern "C" Unk_8087F0A8* lbl_8087F0A8;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void fn_8046EED8(Unk_8087F518* obj);
extern "C" int lbl_8087DCC8;
extern "C" int lbl_8087DCCC;
extern "C" int lbl_8087F434;

GameSaveBackground* GameSaveBackground::fn_803B8C6C(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) GameSaveBackground(parent);
    }
    return 0;
}

int GameSaveBackground::vf0C() {
    return fn_800D3FA4() != 0;
}

void GameSaveBackground::vf10() {
    int out;
    int work;
    int work2[2];
    char name[0x100];
    if (m154) {
        if (lbl_8087EF68 == 0 || fn_800A4228(lbl_8087EF68, work2) == 0) {
            fn_803B8270();
            if (lbl_8087EF68 != 0 && lbl_8087EF68->m168 != 0) {
                m15C = -1;
                return;
            }
            if (m160) {
                m160 = 0;
                fn_803B83A4(name, &out, -1);
                int arg = out;
                m158 = &mHeader;
                m14C = sizeof(Unk_803CE3B8);
                strcpy(mName, name);
                fn_803B8200(0);
                fn_800A48B8(lbl_8087EF68, name, &mHeader, sizeof(Unk_803CE3B8), arg, &work);
            }
            if (m15C >= 0) {
                if (m15C == 0) {
                    lbl_8087DCCC = 1;
                    lbl_8087F434 = 0;
                } else {
                    lbl_8087DCC8 = 1;
                }
                m15C = -1;
            }
        }
    }
}

void GameSaveBackground::fn_803B8E60(void* data, int arg1, int arg2) {
    int out;
    int work;
    char name[0x100];
    if (lbl_8087F0A8->m198 == 0) {
        fn_803B83A4(name, &out, arg1);
        m160 = 1;
        m15C = arg1;
        mHeader.fn_803CE408();
        mHeader.fn_803CE448();
        int arg = out;
        if (arg2 != 0 && lbl_8087F518->m3EF4 != 0 && lbl_8087F518 != 0) {
            fn_8046EED8(lbl_8087F518);
        }
        m158 = data;
        m14C = 0x13560;
        strcpy(mName, name);
        fn_803B8200(0);
        fn_800A48B8(lbl_8087EF68, name, data, 0x13560, arg, &work);
    }
}

void GameSaveBackground::fn_803B8F50(const char* name, void* data, int size, int arg, int flag) {
    int work;
    if (flag && lbl_8087F518->m3EF4 != 0 && lbl_8087F518 != 0) {
        fn_8046EED8(lbl_8087F518);
    }
    m158 = data;
    m14C = size;
    strcpy(mName, name);
    fn_803B8200(0);
    fn_800A48B8(lbl_8087EF68, name, data, size, arg, &work);
}
