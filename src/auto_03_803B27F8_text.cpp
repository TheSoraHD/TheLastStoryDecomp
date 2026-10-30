// auto_03_803B27F8_text.cpp: starts at 0x803B27F8. The virtual-base destructor thunks of
// basic_istream/basic_iostream/basic_stringstream<char> (0x803B27AC-0x803B27F8) belong to
// game/GameEventScript (its vtables are in that unit's .data); they are emitted by the compiler there.
#include "atn/SequenceBoot_unk.h"
#include "atn/FileList.h"
#include "atn/DrawHelper.h"
struct ElemA { char data[0x2D4]; ~ElemA(); };
struct ElemB { char data[0x240]; ~ElemB(); };
struct InnerA { char pad_0x00[0x11C4]; ElemA arr[0x1C]; ~InnerA() {} };
struct InnerB { ElemB arr[7]; ~InnerB() {} };

struct Heap {
    char pad_0x0000[0x539C];
    InnerB field_0x539C;
    char pad_0x635C[0x3B04];
    InnerA field_0x9E60;
    char pad_0xFF54[0x3554];
    ~Heap() {}
};

typedef union UnkName {
    int words[16];
    char chars[0x40];
} UnkName;

typedef struct UnkArg {
    UnkName field_0x00;
    int field_0x40;
} UnkArg;

typedef struct UnkElem {
    int field_0x00;
    int field_0x04;
    char pad_0x08[0xB0];
} UnkElem;

typedef struct Unk {
    void* field_0x00;
    char pad_0x04[0x48 - 0x04];
    int field_0x48;
    UnkElem field_0x4C[17];
    char pad_0xC84[0xD64 - 0xC84];
    int field_0xD64;
    int field_0xD68;
    UnkArg field_0xD6C;
    unsigned char field_0xDB0;
    unsigned char field_0xDB1;
    char pad_0xDB2[2];
    UnkElem field_0xDB4[17];
    char pad_0x19EC[0x1A00 - 0x19EC];
    char field_0x1A00[0x13560];
    void* field_0x14F60;
} Unk;

void operator delete(void* p);

extern "C" {

extern char lbl_8074FBA4[];
extern char lbl_8078B3A0[];
extern char lbl_807C8508[];
extern char lbl_807C85C0[];
extern void* lbl_8087EF70;
extern Heap* lbl_8087F460;
extern int lbl_8087DD60;
extern int lbl_8087DD64;
extern int lbl_8087F630;
extern const char* lbl_8087DD68;
extern const float lbl_80885C00;
extern const float lbl_80885C04;
extern const float lbl_80885C08;
extern const float lbl_80885C0C;
extern const float lbl_80885C10;
extern const float lbl_80885C14;

unsigned long strlen(const char*);
void* memcpy(void* dst, const void* src, unsigned long n);
void* memset(void* dst, int c, unsigned long n);
int sprintf(char* dst, const char* fmt, ...);

void* fn_80084320(int size, int field_a, const char* name1, const char* name2, int field_b);
void fn_803B82C0(void* self, void* arg1);
void fn_803BE8B4(void* elem);
void* fn_803B33F8(void* self);
void fn_803B85EC(void* self);
void fn_803B8758(void* self, int arg);
int fn_803B87A8(void* self);
void dtor_80084684(void* self);
void fn_803B834C(void* self, int arg);
int fn_800A55AC(void* a, int b, int c);
int fn_800A555C(void* a, int b, int c);
int fn_803BEAB4(UnkElem* elem);
void fn_803B83A4(char* buf, int* out, int idx);
void* fn_8006BA8C(char* buf, int* out, int flag);
void fn_803BE850(Heap* h);
void fn_803B3530(void* a, char* b, char* c, int d, int e);
void fn_800D2338(void* self);
Heap* fn_803BDB60(Heap* self);
void fn_803B8B08(void* self, UnkElem* elem);
void fn_803B8160(void* p);

Unk* fn_803B2870(Unk* self, void* arg1, const UnkArg* arg2);

void* fn_803B27F8(void* a, void* b) {
    void* p;
    if (a != 0) {
        p = fn_80084320(0x14F80, 1, lbl_8074FBA4, lbl_8074FBA4, 0);
        if (p != 0) {
            p = fn_803B2870((Unk*)p, a, (const UnkArg*)b);
        }
    } else {
        p = 0;
    }
    return p;
}

#pragma push
#pragma use_lmw_stmw on
Unk* fn_803B2870(Unk* self, void* arg1, const UnkArg* arg2) {
    fn_803B82C0(self, arg1);
    self->field_0x00 = lbl_8078B3A0;
    self->field_0xD68 = 0;
    self->field_0xD6C = *arg2;
    self->field_0xDB0 = 0;
    self->field_0xDB1 = 0;
    UnkElem* p = self->field_0xDB4;
    do {
        fn_803BE8B4(p);
        ++p;
    } while (p < &self->field_0xDB4[17]);
    self->field_0x14F60 = fn_803B33F8(self);
    if (strlen(self->field_0xD6C.field_0x00.chars) != 0) {
        fn_803B85EC(self);
    } else if (self->field_0xD6C.field_0x40 != 0) {
        fn_803B8758(self, -1);
    }
    return self;
}
#pragma pop

} // extern "C"

// inferred: destructor of the temporary that builds a GameLoadTask::Option (called by
// atn/SequenceUnitParam1_const_int.cpp after registering the GameLoad sequences)
struct GameLoadTask {
    struct OptionTmp_unk {
        ~OptionTmp_unk();
    };
};

GameLoadTask::OptionTmp_unk::~OptionTmp_unk() {}

extern "C" {


Unk* fn_803B29E8(Unk* self, int flag) {
    if (self != 0) {
        fn_803B834C(self, 0);
        if (flag > 0) {
            dtor_80084684(self);
        }
    }
    return self;
}

int fn_803B2A40(Unk* self) {
    return fn_803B87A8(self) == 0;
}

static inline bool RangeEq(const char* first, const char* last, const char* lit) {
    for (; first != last; ++first, ++lit) {
        if (*first != *lit) {
            return false;
        }
    }
    return true;
}

static inline bool NameIs(const Unk* self, const char* lit) {
    int n = strlen(self->field_0xD6C.field_0x00.chars);
    return lit[n] == 0 &&
           RangeEq(self->field_0xD6C.field_0x00.chars,
                   &self->field_0xD6C.field_0x00.chars[n], lit);
}

static inline void ReplaceHeap(void) {
    Heap* old = lbl_8087F460;
    if (old != 0) {
        delete old;
        lbl_8087F460 = 0;
    }
    Heap* p = (Heap*)fn_80084320(0x134A8, 1, lbl_8074FBA4, lbl_8074FBA4, 0);
    if (p != 0) {
        p = fn_803BDB60(p);
    }
    lbl_8087F460 = p;
}

static inline void BuildSlot(Unk* self, int idx) {
    int local_0x8;
    char buf[0x100];
    fn_803B83A4(buf, &local_0x8, idx);
    memset(self->field_0x1A00, 0, 0x13560);
    fn_803B3530(self->field_0x14F60, buf, self->field_0x1A00, 0x13560, local_0x8);
    memcpy(lbl_807C8508, &self->field_0x4C[idx], 0xB8);
    self->field_0xD68 = 3;
}

void fn_803B2A68(Unk* self) {
    if (self->field_0xD64 == 0) {
        return;
    }
    switch (self->field_0xD68) {
    case 0:
        self->field_0xD68 = 1;
        break;
    case 1: {
        if (NameIs(self, lbl_8074FBA4 + 1)) {
        }
        if (fn_800A55AC(lbl_8087EF70, 0, 0) != 0 || fn_800A55AC(lbl_8087EF70, 0, 0x1A) != 0) {
            if (NameIs(self, lbl_8074FBA4 + 1)) {
                int v = 0x10;
                if (self->field_0x48 >= 0) {
                    v = self->field_0x48 - 1;
                }
                self->field_0x48 = v;
            } else {
                int v = 0x10;
                if (self->field_0x48 != 0) {
                    v = self->field_0x48 - 1;
                }
                self->field_0x48 = v;
            }
        } else if (fn_800A55AC(lbl_8087EF70, 0, 1) != 0 || fn_800A55AC(lbl_8087EF70, 0, 0x19) != 0) {
            self->field_0x48 = (self->field_0x48 + 1) % 17;
        }
        if (fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
            if (self->field_0x48 < 0) {
                self->field_0xD68 = 4;
            } else if (self->field_0xDB0 != 0) {
                if (fn_803BEAB4(&self->field_0xDB4[self->field_0x48]) != 0) {
                    int local_0x10;
                    int local_0xC;
                    char buf_0x118[0x100];
                    sprintf(buf_0x118, lbl_8074FBA4 + 0xD);
                    ReplaceHeap();
                    void* r30 = fn_8006BA8C(buf_0x118, &local_0xC, 0);
                    fn_803B83A4(buf_0x118, &local_0x10, self->field_0x48);
                    memcpy(lbl_8087F460, (char*)r30 + local_0x10 + 0xB8, 0x134A8);
                    atn::FileFreeBuffer((char*)r30, false);
                    fn_803BE850(lbl_8087F460);
                    self->field_0xD68 = 4;
                }
            } else {
                if (fn_803BEAB4(&self->field_0x4C[self->field_0x48]) != 0) {
                    BuildSlot(self, self->field_0x48);
                }
            }
        }
        if (fn_800A555C(lbl_8087EF70, 0, 5) != 0) {
            sSequenceBoot_unk->SetNext_unk(lbl_8074FBA4 + 0x29);
            fn_800D2338(self);
        }
        break;
    }
    case 3:
        if (((unsigned char*)self->field_0x14F60)[0x154] == 0) {
            self->field_0xD68 = 4;
        }
        break;
    case 4:
        if (self->field_0xDB0 == 0 && self->field_0x48 >= 0) {
            ReplaceHeap();
            int i = self->field_0x48;
            lbl_8087DD60 = i;
            int kind = self->field_0x4C[i].field_0x04;
            lbl_8087DD64 = kind;
            if (kind < 4) {
                memcpy(lbl_8087F460, &self->field_0x1A00[0x38], 0x134A8);
            } else {
                memcpy(lbl_8087F460, &self->field_0x1A00[0xB8], 0x134A8);
            }
            fn_803BE850(lbl_8087F460);
        }
        sSequenceBoot_unk->SetNext_unk(self->field_0xD6C.field_0x00.chars);
        fn_800D2338(self);
        break;
    default:
        break;
    }
}

void fn_803B2FBC(Unk* self, int idx) {
    if (idx >= 0) {
        ReplaceHeap();
        lbl_8087DD60 = idx;
        int kind = self->field_0x4C[idx].field_0x04;
        lbl_8087DD64 = kind;
        if (kind < 4) {
            memcpy(lbl_8087F460, &self->field_0x1A00[0x38], 0x134A8);
        } else {
            memcpy(lbl_8087F460, &self->field_0x1A00[0xB8], 0x134A8);
        }
        fn_803BE850(lbl_8087F460);
    }
}

void fn_803B30D8(Unk* self) {
    if (self->field_0xD64 != 0) {
        if (self->field_0xDB0 != 0) {
            fn_803B8B08(self, self->field_0xDB4);
        } else {
            fn_803B8B08(self, self->field_0x4C);
        }
        if (NameIs(self, lbl_8074FBA4 + 1)) {
            const char* name;
            if (lbl_8087F630 == 0) {
                name = lbl_8087DD68;
            } else {
                sprintf(lbl_807C85C0, lbl_8074FBA4 + 0x2F);
                name = lbl_807C85C0;
            }
            lbl_8087EEB0->fn_800616C0(lbl_80885C00, lbl_80885C04, lbl_80885C08, lbl_80885C0C, lbl_80885C0C, name, self->field_0x48 == -1 ? 0xFFFFFF00 : 0xFFFFFFFF, lbl_80885C08, 1, 1, 0);
        }
        if (self->field_0xDB0 != 0) {
            lbl_8087EEB0->fn_800616C0(lbl_80885C10, lbl_80885C14, lbl_80885C08, lbl_80885C0C, lbl_80885C0C, lbl_8074FBA4 + 0x59, 0xFF00FFFF, lbl_80885C08, 1, 1, 0);
        }
    }
}

void fn_803B3270(Unk* self, int idx) {
    BuildSlot(self, idx);
}

void fn_803B3310(Unk* self, int idx) {
    void* p = self->field_0x14F60;
    if (p == 0) {
        self->field_0xD68 = 1;
    } else {
        if (p != 0) {
            fn_803B8160(p);
            memset(&self->field_0x4C[idx], 0, 0xB8);
        }
        self->field_0xD68 = 5;
    }
}

}