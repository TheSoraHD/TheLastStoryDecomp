#include "atn/Csv.h"
#include "atn/FileList.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s);        // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: hash of a name
extern "C" std::string fn_8006B174(const std::string& s); // inferred: normalised copy of a name
extern "C" int fn_802180A8(const char* s);          // inferred: id of a name (another table)

// inferred: loads a whole file / releases it
extern "C" void* fn_8006BA8C(const char* path, unsigned long* size, int arg);

// inferred: rows of the "@move" section of /boot/chara_motion_param.csv (8 bytes)
struct CharaMoveParam_unk {
    unsigned int mHash;
    float m04;
};

// inferred: rows of the "@attack" section (0x58 bytes)
struct CharaAttackParam_unk {
    unsigned int mHash;
    int m04;
    float m08[4];
    float m18[4];
    int m28[4];
    int m38[4]; // inferred: ids from fn_802180A8, -1 for an empty field
    int m48[4];
};

static const char sFileNameStr[] = "/boot/chara_motion_param.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static CharaMoveParam_unk* sMoveParams;
static unsigned int sMoveCount;
static CharaAttackParam_unk* sAttackParams;
static unsigned int sAttackCount;

// inferred: loads /boot/chara_motion_param.csv
extern "C" void fn_80209300() {
    unsigned long size = 0;
    void* data = fn_8006BA8C(sFileName, &size, 0);
    atn::Csv csv((const char*)data, size);
    int section = 0;
    while (csv.fn_8005BBF8() != 0) {
        const char* field = csv.fn_8005B9CC();
        if (strlen(field) == 0) {
            continue;
        }
        if (*field == '#') {
            continue;
        }
        if (*field == '@') {
            if (strcmp(field, "@move") == 0) {
                section = 0;
            } else if (strcmp(field, "@attack") == 0) {
                section = 1;
            }
            continue;
        }
        if (section == 1) {
            sAttackCount++;
        }
        if (section != 1) {
            sMoveCount++;
        }
    }
    sMoveParams = new (1, "", "", 0) CharaMoveParam_unk[sMoveCount];
    sAttackParams = new (1, "", "", 0) CharaAttackParam_unk[sAttackCount];
    section = 0;
    csv.vf08((const char*)data, size);
    int attack = 0;
    int move = 0;
    while (csv.fn_8005BBF8() != 0) {
        const char* field = csv.fn_8005B9CC();
        if (strlen(field) == 0) {
            continue;
        }
        if (*field == '#') {
            continue;
        }
        if (*field == '@') {
            if (strcmp(field, "@move") == 0) {
                section = 0;
            } else if (strcmp(field, "@attack") == 0) {
                section = 1;
            }
            continue;
        }
        if (section == 1) {
            CharaAttackParam_unk* param = &sAttackParams[attack];
            param->mHash = fn_800DC6B4(field);
            param->m04 = atoi(csv.fn_8005B9CC());
            for (unsigned int i = 0; i < 4; i++) {
                param->m08[i] = fn_800DC288(csv.fn_8005B9CC());
                param->m18[i] = fn_800DC288(csv.fn_8005B9CC());
            }
            for (unsigned int i = 0; i < 4; i++) {
                param->m28[i] = atoi(csv.fn_8005B9CC());
            }
            for (unsigned int i = 0; i < 4; i++) {
                const char* name = csv.fn_8005B9CC();
                if (strcmp(name, "") == 0) {
                    param->m38[i] = -1;
                } else {
                    param->m38[i] = fn_802180A8(name);
                }
            }
            for (unsigned int i = 0; i < 4; i++) {
                const char* name = csv.fn_8005B9CC();
                if (strcmp(name, "") == 0) {
                    param->m48[i] = -1;
                } else {
                    param->m48[i] = fn_802180A8(name);
                }
            }
            attack++;
        } else {
            CharaMoveParam_unk* param = &sMoveParams[move];
            param->mHash = fn_800DC6B4(field);
            param->m04 = fn_800DC288(csv.fn_8005B9CC());
            move++;
        }
    }
    atn::FileFreeBuffer((char*)data, false);
}

extern "C" CharaMoveParam_unk* fn_802096A8(const char* name) {
    unsigned int hash = fn_800DC6B4(fn_8006B174(std::string(name)).c_str());
    for (unsigned int i = 0; i < sMoveCount; i++) {
        if (hash == sMoveParams[i].mHash) {
            return &sMoveParams[i];
        }
    }
    return 0;
}

extern "C" CharaAttackParam_unk* fn_802097C4(const char* name) {
    unsigned int hash = fn_800DC6B4(fn_8006B174(std::string(name)).c_str());
    for (unsigned int i = 0; i < sAttackCount; i++) {
        if (hash == sAttackParams[i].mHash) {
            return &sAttackParams[i];
        }
    }
    return 0;
}
