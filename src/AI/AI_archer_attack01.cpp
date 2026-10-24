#include "game/CharaTask.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/SkillEffectManager.h"
#include "std/string.h"
#include "std/new.h"
#include "atn/Memory.h"

struct AIStringWithAllocator_unk;

extern "C" {
extern void* lbl_8087EE68;
extern CharaManager<PlayerTask>* lbl_8087F8A0;
extern unsigned char lbl_807C68C0[];
extern unsigned char lbl_807C6A40[];
extern unsigned char lbl_807C6B30[];
extern unsigned char lbl_807307A0[];
extern float lbl_80880798;
extern float lbl_808807B8;
extern unsigned char lbl_807C6B78[];
extern unsigned char lbl_807C6B84[];
extern unsigned char lbl_8087D6F0;
extern unsigned char lbl_8087D6F1;
void __register_global_object(void* object, void (*destructor)(void*, int), void* dso);
int fn_80018078(void* routes, CharaTask* chara);
int fn_80018BAC(void* routes, CharaTask* chara);
int fn_8021A984(HitInfo_unk* hit);
int fn_80599C90(Bullet_unk* bullet, CharaTask* chara);
int fn_8001AEDC(int mode, int* a, int* b, int* c, int* d);
int fn_8001BEB0(CharaTask* chara);
int fn_8001B4A4();
int fn_8001AEBC(int size);
int rand();
int fn_80023C84();
int fn_8003DBA4();
int fn_800281A0();
int fn_80028D00();
int fn_80029500();
int fn_8002AD40();
int fn_8001FB08();
int fn_800209F0();
void fn_8001C284();
void fn_8001E730();
void* fn_8001EA18(void*);
unsigned char* fn_8001C260();
unsigned char* fn_8001C26C();
unsigned char* fn_8001C278();
int fn_8001ECD0(void*, int);
void __ct__Q32AI6Script18AI_archer_attack01Fv(void*);
void __ct__Q32AI6Script17AI_archer_guard01Fv(void*);
void __ct__Q32AI6Script12AI_arganan01Fv(void*);
void __ct__Q32AI6Script24AI_buffer_attackstable01Fv(void*);
void __ct__Q32AI6Script17AI_buffer_guard01Fv(void*);
void __ct__Q32AI6Script17AI_buffer_guard02Fv(void*);
void __ct__Q32AI6Script22AI_buffer_pointguard01Fv(void*);
void __ct__Q32AI6Script21AI_em_archer_attack02Fv(void*);
void __ct__Q32AI6Script19AI_em_bridge_archerFv(void*);
void __ct__Q32AI6Script22AI_em_monster_attack01Fv(void*);
void __ct__Q32AI6Script11AI_em_scoutFv(void*);
void __ct__Q32AI6Script20AI_em_sword_attack01Fv(void*);
void __ct__Q32AI6Script20AI_em_sword_attack02Fv(void*);
void __ct__Q32AI6Script24AI_em_sword_pointguard01Fv(void*);
void __ct__Q32AI6Script13AI_em_type003Fv(void*);
void __ct__Q32AI6Script13AI_em_type010Fv(void*);
void __ct__Q32AI6Script13AI_em_type102Fv(void*);
void __ct__Q32AI6Script13AI_em_type103Fv(void*);
void __ct__Q32AI6Script13AI_em_type104Fv(void*);
void __ct__Q32AI6Script13AI_em_type105Fv(void*);
void __ct__Q32AI6Script13AI_em_type106Fv(void*);
void __ct__Q32AI6Script19AI_fr_archer_pick01Fv(void*);
void __ct__Q32AI6Script19AI_fr_buffer_pick01Fv(void*);
void __ct__Q32AI6Script19AI_fr_follow_playerFv(void*);
void __ct__Q32AI6Script18AI_fr_magic_pick01Fv(void*);
void __ct__Q32AI6Script18AI_fr_magic_pick02Fv(void*);
void __ct__Q32AI6Script13AI_fr_runawayFv(void*);
void __ct__Q32AI6Script20AI_fr_sword_attack01Fv(void*);
void __ct__Q32AI6Script18AI_fr_sword_pick01Fv(void*);
void __ct__Q32AI6Script22AI_fr_sword_pointguardFv(void*);
void __ct__Q32AI6Script24AI_fr_sword_pointguard01Fv(void*);
void __ct__Q32AI6Script13AI_fr_type005Fv(void*);
void __ct__Q32AI6Script21AI_fr_warppointescapeFv(void*);
void __ct__Q32AI6Script11AI_jackal01Fv(void*);
void __ct__Q32AI6Script17AI_magic_attack01Fv(void*);
void __ct__Q32AI6Script19AI_magic_attack01_2Fv(void*);
void __ct__Q32AI6Script17AI_magic_attack02Fv(void*);
void __ct__Q32AI6Script23AI_magic_attackstable01Fv(void*);
void __ct__Q32AI6Script15AI_magic_cure01Fv(void*);
void __ct__Q32AI6Script16AI_magic_guard01Fv(void*);
void __ct__Q32AI6Script18AI_magic_guard01_2Fv(void*);
void __ct__Q32AI6Script16AI_magic_guard02Fv(void*);
void __ct__Q32AI6Script14AI_np_approachFv(void*);
void __ct__Q32AI6Script19AI_np_approach_plusFv(void*);
void __ct__Q32AI6Script19AI_np_follow_playerFv(void*);
void __ct__Q32AI6Script19AI_np_tagger_randomFv(void*);
void __ct__Q32AI6Script13AI_np_type001Fv(void*);
void __ct__Q32AI6Script13AI_np_type002Fv(void*);
void __ct__Q32AI6Script13AI_np_type003Fv(void*);
void __ct__Q32AI6Script13AI_np_type004Fv(void*);
void __ct__Q32AI6Script13AI_np_type005Fv(void*);
void __ct__Q32AI6Script19AI_np_wait_reactionFv(void*);
void __ct__Q32AI6Script25AI_np_wait_reaction_minusFv(void*);
void __ct__Q32AI6Script26AI_np_wait_reaction_minus2Fv(void*);
void __ct__Q32AI6Script24AI_np_wait_reaction_plusFv(void*);
void __ct__Q32AI6Script10AI_quark01Fv(void*);
void __ct__Q32AI6Script10AI_quark02Fv(void*);
void __ct__Q32AI6Script18AI_silence_guard01Fv(void*);
void __ct__Q32AI6Script16AI_sword_guard01Fv(void*);
void __ct__Q32AI6Script22AI_sword_leaderguard01Fv(void*);
void __ct__Q32AI6Script22AI_sword_leaderguard02Fv(void*);
void __ct__Q32AI6Script22AI_sword_leaderguard03Fv(void*);
void __ct__Q32AI6Script21AI_sword_pointguard01Fv(void*);
void __ct__Q32AI6Script17AI_return_guard01Fv(void*);
void fn_8001E1D0(void*, void*, const AIStringWithAllocator_unk*);
}

extern "C" int fn_8001BF44() {
    return 0;
}

extern "C" int fn_8001BF4C(CharaTask* chara) {
    return fn_80018078(lbl_8087EE68, chara);
}

extern "C" int fn_8001BF58(CharaTask* chara) {
    return fn_80018BAC(lbl_8087EE68, chara);
}

extern "C" SkillEffectManager* lbl_8087F9E8;

extern "C" int fn_8001BF64(CharaTask* chara) {
    int count = 0;
    Bullet_unk* bullet = 0;
    for (unsigned int i = 0; i < 32; i++) {
        bullet = lbl_8087F9E8->GetBullet_unk(i);
        if (bullet != 0 && bullet->IsActive_unk() && fn_8021A984(bullet->m04) &&
            fn_80599C90(bullet, chara)) {
            count++;
        }
    }
    return count;
}

extern "C" int fn_8001C01C(CharaTask* chara) {
    int ret = 0;
    if (chara != 0) {
        ret = chara->mC58.mE0_12;
    }
    return ret;
}

// NONMATCHING: 0.00% - the inferred copy loops compile to a different routine shape.
// inferred: copies the script's fixed parameter blocks into the global AI state.
extern "C" unsigned char* fn_8001C088() {
    unsigned char* src = fn_8001C260();
    unsigned char* dst = fn_8001C26C();

    *(int*)dst = 0;
    *(int*)(dst + 4) = *(int*)(src + 4);
    *(int*)(dst + 8) = *(int*)(src + 0x64);
    *(int*)(dst + 0xC) = *(int*)(src + 0x68);
    *(int*)(dst + 0x10) = *(int*)(src + 0x6C);
    for (int i = 0; i < 5; i++) {
        *(int*)(dst + 0x14 + i * 0x14) = *(int*)(src + 0xA4 + i * 0x14);
        *(int*)(dst + 0x18 + i * 0x14) = *(int*)(src + 0xA8 + i * 0x14);
        *(int*)(dst + 0x1C + i * 0x14) = *(int*)(src + 0xAC + i * 0x14);
        *(int*)(dst + 0x20 + i * 0x14) = *(int*)(src + 0xB0 + i * 0x14);
        *(int*)(dst + 0x24 + i * 0x14) = *(int*)(src + 0xB4 + i * 0x14);
    }
    for (int i = 0; i < 5; i++) {
        *(int*)(dst + 0x78 + i * 0x14) = *(int*)(src + 0x108 + i * 0x14);
        *(int*)(dst + 0x7C + i * 0x14) = *(int*)(src + 0x10C + i * 0x14);
        *(int*)(dst + 0x80 + i * 0x14) = *(int*)(src + 0x110 + i * 0x14);
        *(int*)(dst + 0x84 + i * 0x14) = *(int*)(src + 0x114 + i * 0x14);
        *(int*)(dst + 0x88 + i * 0x14) = *(int*)(src + 0x118 + i * 0x14);
    }
    for (int i = 0; i < 5; i++) {
        *(int*)(dst + 0xDC + i * 4) = *(int*)(src + 0x16C + i * 4);
    }

    unsigned char* tail = fn_8001C278();
    CharaManager<PlayerTask>* manager = lbl_8087F8A0;
    *(int*)tail = manager != 0 ? *(int*)((unsigned char*)manager + 0x48) : 0;
    *(int*)(tail + 4) = 0;
    return tail;
}

// NONMATCHING: 80.53% - MWCC lowers the global state pointers through different code than the target.
// inferred: applies the selected script's two indexed parameter values to global state.
extern "C" unsigned char* fn_8001C1C0(unsigned char* script) {
    fn_8001C088();
    unsigned char* result = (unsigned char*)fn_8001EA18(script);
    if (result != 0) {
        unsigned char* src = lbl_807C68C0;
        unsigned char* dst = lbl_807C6A40;
        if (*(int*)(src + 4) != 0) {
            int value = fn_8001ECD0(result, 0xE);
            if (value >= 0) {
                *(int*)(src + 4) = value;
                *(int*)(dst + 4) = value;
            }
        }
        int value = fn_8001ECD0(result, *(int*)(src + 4));
        if (value >= 0) {
            *(int*)(dst + 4) = value;
        }
    }
    return result;
}

// inferred: this global sentinel is registered with the destructor for its three linked buckets.
extern "C" void fn_8003E6B8(void* object, int freeObject);
extern "C" void fn_8003E688() {
    struct ListHead_unk {
        void* first;
        void* last;
        void* end;
    };
    ListHead_unk* head = (ListHead_unk*)lbl_807C6B84;
    head->first = 0;
    head->last = 0;
    head->end = &head->last;
    __register_global_object(head, fn_8003E6B8, lbl_807C6B78);
}

// NONMATCHING: 89.40% - conversion to float and generated constant do not match MWCC's sequence.
extern "C" int fn_8003CBE0() {
    int count = *(int*)(lbl_807C68C0 + 0x30);
    *(int*)lbl_807C6A40 = 1;
    *(int*)(lbl_807C6A40 + 0x1C) = (int)(lbl_808807B8 * (float)count);
    return 1;
}

extern "C" int fn_8003068C() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x30);
    *(int*)(lbl_807C6A40 + 0x20) = 0x5A;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x64;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_80033588() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80035FD0() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0x5A;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80032FA8() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0xF;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 89.57% - stack output order and register allocation differ.
extern "C" int fn_8002EB20() {
    int d = *(int*)(lbl_807C68C0 + 0x7C);
    int c = 0x1E;
    int b = 0;
    int a = 0;
    *(int*)(lbl_807C6A40 + 0x18) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x1C) = d;
    *(int*)lbl_807C6A40 = 1;
    fn_8001AEDC(8, &d, &c, &b, &a);
    return 1;
}

// NONMATCHING: 70.77% - argument setup and local register allocation differ.
// inferred: a 0x12-byte helper allocation is passed as the first output word.
extern "C" int fn_80036AA8() {
    int a = fn_8001AEBC(0x12);
    int b = *(int*)(lbl_807C6A40 + 0x18);
    int c = 0;
    int d = 0;
    fn_8001AEDC(0xD, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    return 6;
}

// NONMATCHING: 88.31% - local output aggregate and register allocation differ.
// inferred: the helper receives four consecutive output words; only the first is initialized from m80.
extern "C" int fn_8002B818() {
    int results[4] = {0, 0, 0, 0};
    int value = *(int*)(lbl_807C68C0 + 0x7C);
    results[0] = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x18) = value;
    *(int*)(lbl_807C6A40 + 0x1C) = 1;
    *(int*)lbl_807C6A40 = 1;
    fn_8001AEDC(0, &results[0], &results[1], &results[2], &results[3]);
    return 3;
}

// NONMATCHING: 88.31% - local output aggregate and register allocation differ.
// inferred: duplicate script stub of fn_8002B818 at a second address.
extern "C" int fn_80035C20() {
    int results[4] = {0, 0, 0, 0};
    int value = *(int*)(lbl_807C68C0 + 0x7C);
    results[0] = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x18) = value;
    *(int*)(lbl_807C6A40 + 0x1C) = 1;
    *(int*)lbl_807C6A40 = 1;
    fn_8001AEDC(0, &results[0], &results[1], &results[2], &results[3]);
    return 3;
}

extern "C" int fn_80035274() {
    *(int*)(lbl_807C6A40 + 0x18) = 0x4B;
    *(int*)(lbl_807C6A40 + 0x1C) = 0x4B;
    *(int*)(lbl_807C6A40 + 0x20) = 0x64;
    *(int*)(lbl_807C6A40 + 0x24) = 0xA;
    *(int*)(lbl_807C6A40 + 0x28) = 0;
    *(int*)(lbl_807C6A40 + 0x2C) = fn_8001B4A4();
    *(int*)(lbl_807C6A40 + 0x30) = 0x12C;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

// inferred: three-word MSL string header; the high bit selects the heap-buffer representation.
struct AIString_unk {
    unsigned int mFlags;
    unsigned int mSize;
    char* mData;
};

// inferred: the registry uses an rb-tree node header followed by a string key at +0xC.
struct AIScriptMapNode_unk {
    AIScriptMapNode_unk* mLeft;
    AIScriptMapNode_unk* mRight;
    AIScriptMapNode_unk* mParent;
    AIString_unk mKey;
};

extern "C" int memcmp(const void*, const void*, unsigned long);

// NONMATCHING: 88.17% - the loop setup and register allocation differ from the tree-search body.
// inferred: finds the insertion position for a string key and returns the last visited node.
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key,
                                            AIScriptMapNode_unk** parent,
                                            unsigned char* insertLeft,
                                            unsigned char* insertFirst) {
    AIScriptMapNode_unk* node = *(AIScriptMapNode_unk**)((unsigned char*)tree + 4);
    AIScriptMapNode_unk* candidate = (AIScriptMapNode_unk*)((unsigned char*)tree + 4);
    *parent = 0;
    *insertLeft = 1;
    *insertFirst = 1;
    while (node != 0) {
        candidate = node;
        AIString_unk* nodeKey = &node->mKey;
        unsigned int nodeSize;
        const char* nodeData;
        if ((nodeKey->mFlags >> 31) == 0) {
            nodeSize = *(unsigned char*)nodeKey & 0x7F;
            nodeData = (const char*)nodeKey + 1;
        } else {
            nodeSize = nodeKey->mSize;
            nodeData = nodeKey->mData;
        }
        std::string* keyString = (std::string*)key;
        int cmp = keyString->compare(0, keyString->size(), nodeData, nodeSize);
        if (cmp < 0) {
            node = node->mLeft;
            *insertLeft = 1;
        } else {
            *parent = node;
            node = node->mRight;
            *insertLeft = 0;
            *insertFirst = 0;
        }
    }
    return candidate;
}

void operator delete(void* ptr);

extern "C" void* fn_8001E168(AIString_unk* str, int freeObject) {
    if (str != 0) {
        if (str != 0) {
            if ((str->mFlags >> 31) != 0) {
                operator delete(str->mData);
            }
        }
        if (freeObject > 0) {
            operator delete(str);
        }
    }
    return str;
}

// inferred: 0x8001E0B8 copies the 12-byte string followed by the allocator word.
struct AIStringWithAllocator_unk {
    AIString_unk mString;
    void* mAllocator;
};

extern "C" AIStringWithAllocator_unk* fn_8001E0B8(AIStringWithAllocator_unk* dst,
                                                   const AIStringWithAllocator_unk* src,
                                                   const unsigned int* allocator) {
    if ((src->mString.mFlags >> 31) == 0) {
        dst->mString = src->mString;
    } else {
        dst->mString.mFlags = 0;
        dst->mString.mSize = 0;
        dst->mString.mData = 0;
        std::string* str = (std::string*)&dst->mString;
        str->reserve(src->mString.mSize);
        std::random_access_iterator_tag tag;
        str->__replace_unk(0, 0, src->mString.mData,
                           src->mString.mData + src->mString.mSize,
                           tag);
    }
    dst->mAllocator = (void*)*allocator;
    return dst;
}

extern "C" AIScriptMapNode_unk* fn_8003DD50(void* tree,
                                             AIScriptMapNode_unk* candidate,
                                             unsigned char insertLeft,
                                             unsigned char insertFirst,
                                             const AIStringWithAllocator_unk* key);

// NONMATCHING: 47.60% - the tree insertion helper still has a different stack and register layout.
// inferred: returns the inserted/existing node and a flag in the caller-provided 8-byte pair.
extern "C" void fn_8001E1D0(void* result, void* tree,
                             const AIStringWithAllocator_unk* value) {
    AIScriptMapNode_unk* parent = 0;
    unsigned char insertLeft = 1;
    unsigned char insertFirst = 1;
    AIScriptMapNode_unk* candidate = fn_8003E530(
        tree, &value->mString, &parent, &insertLeft, &insertFirst);

    bool insert = parent == 0;
    if (parent != 0) {
        const AIString_unk* key = &value->mString;
        const AIString_unk* existing = &parent->mKey;
        unsigned int keySize = (key->mFlags >> 31) != 0
                                   ? key->mSize
                                   : (*(const unsigned char*)key & 0x7F);
        const char* keyData = (key->mFlags >> 31) != 0
                                  ? key->mData
                                  : (const char*)key + 1;
        unsigned int existingSize = (existing->mFlags >> 31) != 0
                                        ? existing->mSize
                                        : (*(const unsigned char*)existing & 0x7F);
        const char* existingData = (existing->mFlags >> 31) != 0
                                       ? existing->mData
                                       : (const char*)existing + 1;
        unsigned int commonSize = keySize < existingSize ? keySize : existingSize;
        int cmp = memcmp(keyData, existingData, commonSize);
        if (cmp == 0) {
            cmp = keySize < existingSize ? -1 : (keySize == existingSize ? 0 : 1);
        }
        insert = cmp < 0;
    }

    AIScriptMapNode_unk* node;
    unsigned char flag;
    if (insert) {
        node = fn_8003DD50(tree, candidate, insertLeft, insertFirst, value);
        flag = lbl_8087D6F0;
    } else {
        node = parent;
        flag = lbl_8087D6F1;
    }
    *(AIScriptMapNode_unk**)result = node;
    *((unsigned char*)result + 4) = flag;
}

extern "C" int fn_80021038() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_8002113C() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80021404() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80021C3C() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80021D40() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 93.89% - MWCC assigns the return discriminator to r3 instead of r0.
extern "C" int fn_80022598() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

extern "C" int fn_80022524() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80022628() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_8002FFE0() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_8001EE58() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x30);
    *(int*)(lbl_807C6A40 + 0x20) = 0x5A;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_8001FC40() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x30);
    *(int*)(lbl_807C6A40 + 0x20) = 0x5A;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x64;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_80020B28() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 0x50;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_800211B0() {
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x50;
    *(int*)lbl_807C6A40 = 1;
    int d = 0;
    int c = 0;
    int b = 0;
    int a = 0;
    fn_8001AEDC(0, &a, &b, &c, &d);
    return 1;
}

extern "C" int fn_80021508() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x50;
    *(int*)lbl_807C6A40 = 1;
    int d = 0;
    int c = 0;
    int b = 0;
    int a = 0;
    fn_8001AEDC(0, &a, &b, &c, &d);
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_80021DB4() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x50;
    *(int*)lbl_807C6A40 = 1;
    int d = 0;
    int c = 0;
    int b = 0;
    int a = 0;
    fn_8001AEDC(0, &a, &b, &c, &d);
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

// NONMATCHING: 93.89% - MWCC assigns the return discriminator to r3 instead of r0.
extern "C" int fn_80021478() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

// NONMATCHING: 93.89% - control flow and return discriminator differ from the script stub.
extern "C" int fn_800210AC() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

// NONMATCHING: 93.89% - control flow and return discriminator differ from the script stub.
extern "C" int fn_80021CB0() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

extern "C" int fn_80022BD4() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80022F44() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80023C54() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        *(int*)lbl_807C6A40 = 1;
        return 7;
    }
    return -1;
}

extern "C" int fn_800240C0() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_800240F0() {
    *(int*)(lbl_807C6A40 + 0x1C) = 0xF;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80027444() {
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_800276D4() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80022F74() {
    *(int*)(lbl_807C6A40 + 0x1C) = 0x1E;
    *(int*)(lbl_807C6A40 + 0x20) = 0x5A;
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80023DBC() {
    *(int*)(lbl_807C6A40 + 0x18) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_8002269C() {
    *(int*)(lbl_807C6A40 + 0x18) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x88);
    *(int*)(lbl_807C6A40 + 0x50) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80024718() {
    *(int*)(lbl_807C6A40 + 0x18) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x24) = 0;
    *(int*)(lbl_807C6A40 + 0x28) = 0;
    *(int*)(lbl_807C6A40 + 0x2C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80028178() {
    *(int*)(lbl_807C6A40 + 0x1C) = 0x1E;
    *(int*)(lbl_807C6A40 + 0x20) = 0x5A;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80028CD8() {
    *(int*)(lbl_807C6A40 + 0x1C) = 0x1E;
    *(int*)(lbl_807C6A40 + 0x20) = 0x5A;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80028E34() {
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x30);
    *(int*)(lbl_807C6A40 + 0x20) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_8002973C() {
    *(int*)(lbl_807C6A40 + 0x18) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x24) = 0;
    *(int*)(lbl_807C6A40 + 0x28) = 0;
    *(int*)(lbl_807C6A40 + 0x2C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_8002AE78() {
    *(int*)(lbl_807C6A40 + 0xE0) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_8002D36C() {
    *(int*)(lbl_807C6A40 + 0x18) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x24) = 0;
    *(int*)(lbl_807C6A40 + 0x28) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80024D6C() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x18) = 0x5A;
    *(int*)(lbl_807C6A40 + 0x1C) = 0xF;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_8002552C() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x18) = 0x5A;
    *(int*)(lbl_807C6A40 + 0x1C) = 0xF;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_8002D704() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x18) = 0x5A;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_8002DF20() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x18) = 0x5A;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_8002B9D0() {
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        *(int*)lbl_807C6A40 = 1;
        return 7;
    }
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_8002C2F0() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x18) = 0;
    *(int*)(lbl_807C6A40 + 0x1C) = 0x46;
    *(int*)(lbl_807C6A40 + 0xE0) = 0;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_8002EF74() {
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x88);
    *(int*)(lbl_807C6A40 + 0x8C) = *(int*)(lbl_807C68C0 + 0x8C);
    *(int*)(lbl_807C6A40 + 0x3C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80025DE0() {
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x88);
    *(int*)(lbl_807C6A40 + 0x8C) = *(int*)(lbl_807C68C0 + 0x8C);
    *(int*)(lbl_807C6A40 + 0x3C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

// NONMATCHING: 89.70% - the local-slot allocation and scratch-store scheduling differ.
extern "C" int fn_80026A40() {
    int d = 0;
    int c = 0xA;
    int b = 0;
    int a = 0;
    *(int*)(lbl_807C6A40 + 0x18) = 0x28;
    d = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x1C) = d;
    *(int*)lbl_807C6A40 = 1;
    fn_8001AEDC(8, &d, &c, &b, &a);
    return 1;
}

extern "C" int fn_8002765C() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0xD2;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 9;
    }
    return -1;
}

extern "C" int fn_80027B2C() {
    if (fn_800281A0() == 0) {
        *(int*)lbl_807C6A40 = 1;
        return 0;
    }
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_8002868C() {
    if (fn_80028D00() == 0) {
        *(int*)lbl_807C6A40 = 1;
        return 0;
    }
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_8002922C() {
    if (fn_80029500() == 0) {
        *(int*)lbl_807C6A40 = 1;
        return 0;
    }
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_8002A6F0() {
    if (fn_8002AD40() == 0) {
        *(int*)lbl_807C6A40 = 1;
        return 0;
    }
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 78.32% - MWCC places the parameter pointer in the wrong stack slot.
extern "C" int fn_80029A14() {
    int d = 0;
    int c = 0;
    int b = 0;
    int a = (int)(lbl_807307A0 + 0x40A);
    fn_8001AEDC(0x14, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 5;
    return 1;
}

extern "C" int fn_80029A84() {
    int d = 0;
    int c = 0;
    int b = 5;
    int a = *(int*)(lbl_807C6A40 + 0x18);
    fn_8001AEDC(0xC, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    *(int*)(lbl_807C6A40 + 0x28) = 1;
    return 1;
}

extern "C" int fn_80029AFC() {
    int d = 0;
    int c = 0;
    int b = *(int*)lbl_807C68C0;
    int a = 0;
    fn_8001AEDC(7, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 6;
    return 1;
}

extern "C" int fn_80029D18() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x30);
    *(int*)(lbl_807C6A40 + 0x20) = 0x5A;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

// NONMATCHING: 78.32% - MWCC places the parameter pointer in the wrong stack slot.
extern "C" int fn_800249EC() {
    int d = 0;
    int c = 0;
    int b = 0;
    int a = (int)(lbl_807307A0 + 0x40A);
    fn_8001AEDC(0x14, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 5;
    return 1;
}

extern "C" int fn_80024B50() {
    int d = 0;
    int c = 0;
    int b = *(int*)(lbl_807C68C0 + 0x20);
    int a = 0;
    fn_8001AEDC(7, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 6;
    return 1;
}

extern "C" int fn_8002B88C() {
    int d = 0;
    int c = 0;
    int b = *(int*)(lbl_807C6A40 + 0x18);
    int a = *(int*)lbl_807C6B30;
    fn_8001AEDC(0xD, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    return 6;
}

extern "C" int fn_80035C94() {
    int d = 0;
    int c = 0;
    int b = *(int*)(lbl_807C6A40 + 0x18);
    int a = *(int*)lbl_807C6B30;
    fn_8001AEDC(0xD, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    return 6;
}

extern "C" int fn_80033F38() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x64;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_80034760() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x64;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_8003574C() {
    *(int*)(lbl_807C6A40 + 0x18) = 0x4B;
    *(int*)(lbl_807C6A40 + 0x1C) = 0;
    *(int*)(lbl_807C6A40 + 0x20) = 0x4B;
    *(int*)(lbl_807C6A40 + 0x24) = 0xA;
    *(int*)(lbl_807C6A40 + 0x28) = 0;
    *(int*)(lbl_807C6A40 + 0x2C) = fn_8001B4A4();
    *(int*)(lbl_807C6A40 + 0x30) = 0x12C;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80038674() {
    *(int*)(lbl_807C6A40 + 0x18) = 0;
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x88);
    *(int*)(lbl_807C6A40 + 0x3C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_800388C4() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x64;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_80038D90() {
    int result;
    *(int*)(lbl_807C6A40 + 0x18) = 0x5A;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x64;
    result = 1;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_80039908() {
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x88);
    *(int*)(lbl_807C6A40 + 0x3C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_8003A580() {
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x88);
    *(int*)(lbl_807C6A40 + 0x3C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_8003B2A4() {
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x88);
    *(int*)(lbl_807C6A40 + 0x30) = *(int*)(lbl_807C68C0 + 0x90);
    *(int*)(lbl_807C6A40 + 0x3C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_8003C074() {
    *(int*)(lbl_807C6A40 + 0x1C) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x88);
    *(int*)(lbl_807C6A40 + 0x2C) = *(int*)(lbl_807C68C0 + 0x8C);
    *(int*)(lbl_807C6A40 + 0x3C) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

// NONMATCHING: 93.89% - MWCC does not preserve the target return-discriminator register.
extern "C" int fn_800386C0() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

// NONMATCHING: 93.89% - MWCC does not preserve the target return-discriminator register.
extern "C" int fn_80038C18() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

// NONMATCHING: 93.89% - MWCC does not preserve the target return-discriminator register.
extern "C" int fn_80039594() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

// NONMATCHING: 93.89% - MWCC does not preserve the target return-discriminator register.
extern "C" int fn_8003A044() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

// NONMATCHING: 98.78% - global-base register selection differs from MWCC's original allocation.
extern "C" int fn_80020914() {
    int result;
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int amount = *(int*)(lbl_807C68C0 + 0x4C);
        int base = *(int*)(lbl_807C68C0 + 0x48);
        int half = (amount + (int)((unsigned int)amount >> 31)) >> 1;
        int start = base - half;
        int range = base + half - start;
        if (range > 0) {
            start += rand() % range;
        }
        int a = 0;
        int b = 0;
        int c = 0;
        int d = 0;
        a = start;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 7;
    } else {
        result = -1;
    }
    if (result == 7) {
        *(int*)lbl_807C6A40 = 1;
        return 7;
    }
    return -1;
}

// NONMATCHING: 91.85% - random-range arithmetic and return discrimination still differ.
extern "C" int fn_80022308() {
    int result = -1;
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int amount = *(int*)(lbl_807C68C0 + 0x4C);
        int base = *(int*)(lbl_807C68C0 + 0x48);
        int half = (amount + ((unsigned int)amount >> 31)) >> 1;
        int start = base - half;
        int range = base + half - start;
        if (range > 0) {
            start += rand() % range;
        }
        int d = 0;
        int c = 0;
        int b = 0;
        int a = start;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 7;
    }
    if (result == 7) {
        *(int*)lbl_807C6A40 = 1;
        return 7;
    }
    return -1;
}

// NONMATCHING: 91.85% - random-range arithmetic and return discrimination still differ.
extern "C" int fn_80023B78() {
    int result = -1;
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int amount = *(int*)(lbl_807C68C0 + 0x4C);
        int base = *(int*)(lbl_807C68C0 + 0x48);
        int half = (amount + ((unsigned int)amount >> 31)) >> 1;
        int start = base - half;
        int range = base + half - start;
        if (range > 0) {
            start += rand() % range;
        }
        int d = 0;
        int c = 0;
        int b = 0;
        int a = start;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 7;
    }
    if (result == 7) {
        *(int*)lbl_807C6A40 = 1;
        return 7;
    }
    return -1;
}

extern "C" int fn_80030220() {
    int d = 0;
    int c = 0;
    int b = 0xA;
    int a = *(int*)(lbl_807C6A40 + 0x3C);
    fn_8001AEDC(0xC, &a, &b, &c, &d);
    *(int*)lbl_807C6A40 = 1;
    ++*(int*)(lbl_807C6A40 + 0x40);
    return 6;
}

extern "C" int fn_80023764() {
    if (fn_80023C84() == 0) {
        *(int*)lbl_807C6A40 = 1;
        return 0;
    }
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_8002403C() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = *(int*)(lbl_807C6A40 + 0x1C);
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 9;
    }
    return -1;
}

// NONMATCHING: 93.89% - MWCC does not preserve the target return-discriminator register.
extern "C" int fn_80023FAC() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

// NONMATCHING: 93.89% - MWCC does not preserve the target return-discriminator register.
extern "C" int fn_800244B0() {
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    return result;
}

// NONMATCHING: 91.68% - return paths through the second script helper differ.
extern "C" int fn_80024DCC() {
    int result = -1;
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        *(int*)lbl_807C6A40 = 1;
        result = 6;
    }
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 6) {
            *(int*)lbl_807C6A40 = 1;
            return 6;
        }
        if (next == 4) {
            *(int*)lbl_807C6A40 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

// NONMATCHING: 76.72% - MWCC's parameter stack slots and scratch-store order differ.
extern "C" int fn_80026E94() {
    int a = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)(lbl_807C6A40 + 0x18) = 0x28;
    *(int*)(lbl_807C6A40 + 0x1C) = a;
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = 0;
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x84);
    *(int*)lbl_807C6A40 = 1;
    int b = 0xA;
    int c = 0;
    int d = 0;
    fn_8001AEDC(8, &a, &b, &c, &d);
    return 1;
}

extern "C" int fn_8002B434() {
    int result;
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        // inferred: these four zeroed outputs occupy one consecutive stack aggregate.
        struct Results_unk {
            int d;
            int c;
            int b;
            int a;
        } results = {0, 0, 0, 0};
        fn_8001AEDC(0, &results.a, &results.b, &results.c, &results.d);
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    } else {
        result = -1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_8002B4C8() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 88.97% - float comparison and parameter stack-slot allocation differ.
extern "C" int fn_80027458() {
    if (*(int*)(lbl_807C68C0 + 8) != 0 && *(float*)(lbl_807C68C0 + 0x1C) < lbl_80880798) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0xA;
        fn_8001AEDC(6, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 4;
    }
    return -1;
}

// NONMATCHING: 88.97% - float comparison and parameter stack-slot allocation differ.
extern "C" int fn_80027704() {
    if (*(int*)(lbl_807C68C0 + 8) != 0 && *(float*)(lbl_807C68C0 + 0x1C) < lbl_80880798) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 3;
        fn_8001AEDC(6, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 4;
    }
    return -1;
}

// NONMATCHING: 88.97% - float comparison and parameter stack-slot allocation differ.
extern "C" int fn_800282D4() {
    if (*(int*)(lbl_807C68C0 + 8) != 0 && *(float*)(lbl_807C68C0 + 0x1C) < lbl_80880798) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 3;
        fn_8001AEDC(6, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        return 4;
    }
    return -1;
}

// NONMATCHING: 91.85% - random-range arithmetic and return discrimination still differ.
extern "C" int fn_80021A20() {
    int result = -1;
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        int amount = *(int*)(lbl_807C68C0 + 0x4C);
        int base = *(int*)(lbl_807C68C0 + 0x48);
        int half = (amount + ((unsigned int)amount >> 31)) >> 1;
        int start = base - half;
        int range = base + half - start;
        if (range > 0) {
            start += rand() % range;
        }
        int d = 0;
        int c = 0;
        int b = 0;
        int a = start;
        fn_8001AEDC(0, &a, &b, &c, &d);
        *(int*)lbl_807C6A40 = 1;
        result = 7;
    }
    if (result == 7) {
        *(int*)lbl_807C6A40 = 1;
        return 7;
    }
    return -1;
}

// NONMATCHING: 88.52% - argument-register allocation differs around the AI call.
extern "C" int fn_8002FBD4() {
    *(int*)(lbl_807C6A40 + 0x18) = 0x2711;
    *(int*)(lbl_807C6A40 + 0x1C) = 0x32;
    *(int*)lbl_807C6A40 = 1;
    int d = 0;
    int c = 0;
    int b = 0x32;
    int a = 0x2711;
    fn_8001AEDC(8, &a, &b, &c, &d);
    return 1;
}

extern "C" int fn_800302A0() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        if (*(int*)(lbl_807C6A40 + 0x50) >= 0xC8) {
            *(int*)lbl_807C6A40 = 1;
            return 7;
        } else {
            *(int*)lbl_807C6A40 = 1;
            return 1;
        }
    }
    return -1;
}

extern "C" int fn_80035D04() {
    *(int*)lbl_807C6A40 = 1;
    return 3;
}

extern "C" int fn_80036A88() {
    *(int*)(lbl_807C6A40 + 0x18) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 3;
}

extern "C" int fn_80035D18() {
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80036B20() {
    *(int*)lbl_807C6A40 = 1;
    return 3;
}

extern "C" int fn_80036C94() {
    *(int*)(lbl_807C6A40 + 0x18) = 0x1E;
    *(int*)(lbl_807C6A40 + 0x1C) = 0x64;
    *(int*)(lbl_807C6A40 + 0x20) = 0x190;
    *(int*)lbl_807C6A40 = 1;
    return 3;
}

extern "C" int fn_80035EB4() {
    if (*(int*)(lbl_807C68C0 + 8) != 0) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_80036CC4() {
    *(int*)(lbl_807C6A40 + 0x18) = 0;
    *(int*)(lbl_807C6A40 + 0x1C) = 0;
    *(int*)(lbl_807C6A40 + 0x20) = 0x64;
    *(int*)(lbl_807C6A40 + 0x24) = 0;
    *(int*)(lbl_807C6A40 + 0x28) = 1;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80036F8C() {
    *(int*)(lbl_807C6A40 + 0x18) = 0;
    *(int*)(lbl_807C6A40 + 0x1C) = 0;
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = 0;
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_800371E0() {
    *(int*)(lbl_807C6A40 + 0x18) = 0;
    *(int*)(lbl_807C6A40 + 0x1C) = 0;
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = 0;
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_800374E0() {
    *(int*)(lbl_807C6A40 + 0x18) = 0;
    *(int*)(lbl_807C6A40 + 0x1C) = 0;
    *(int*)(lbl_807C6A40 + 0x20) = *(int*)(lbl_807C68C0 + 0x80);
    *(int*)(lbl_807C6A40 + 0x24) = 0;
    *(int*)(lbl_807C6A40 + 0x28) = *(int*)(lbl_807C68C0 + 0x7C);
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80031758() {
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        *(int*)lbl_807C6A40 = 1;
        return 7;
    }
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80031DE4() {
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        *(int*)lbl_807C6A40 = 1;
        return 7;
    }
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80032424() {
    int result = 1;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_80032F04() {
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_80033020() {
    *(int*)(lbl_807C6A40 + 0xE0) = 0;
    *(int*)lbl_807C6A40 = 1;
    return 1;
}

extern "C" int fn_800337F4() {
    int result = 1;
    *(int*)(lbl_807C6A40 + 0x24) = 0x3C;
    *(int*)(lbl_807C6A40 + 0x28) = 0x64;
    *(int*)lbl_807C6A40 = 1;
    if (fn_8001BEB0(*(CharaTask**)lbl_807C68C0)) {
        result = 7;
    }
    return result;
}

extern "C" int fn_8001F674() {
    if (fn_8001FB08() == 0) {
        *(int*)lbl_807C6A40 = 1;
        return 0;
    }
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" int fn_800204D8() {
    if (fn_800209F0() == 0) {
        *(int*)lbl_807C6A40 = 1;
        return 0;
    }
    int result;
    if (*(int*)(lbl_807C6A40 + 0x10) >= 1) {
        result = -1;
    } else {
        *(int*)lbl_807C6A40 = 1;
        result = 1;
    }
    if (result == 1) {
        *(int*)lbl_807C6A40 = 1;
        return 1;
    }
    return -1;
}

extern "C" void fn_8001C068(CharaTask* chara, const char* name) {
    if (chara != 0 && name != 0) {
        strncpy((char*)chara->mC58.m1B0, name, 0x100);
    }
}

extern "C" int fn_8001C038(CharaTask*) {
    int ret = 0;
    CharaManager<PlayerTask>* manager = lbl_8087F8A0;
    // Select the head explicitly so both null paths share the same check.
    manager = manager != 0 ? (CharaManager<PlayerTask>*)manager->mHead : 0;
    if (manager != 0) {
        ret = ((PlayerTask*)manager)->mFlag28;
    }
    return ret;
}

extern "C" void fn_8001C1B4() {
    fn_8001C284();
}

extern "C" void fn_8001C1B8() {
    fn_8001E730();
}

extern "C" void fn_8001C1BC() {}

// NONMATCHING: 92.27% - registry population is reconstructed; local layout and call scheduling differ.
// inferred: registers each script name with a heap-3 instance in the global AI script tree.
extern "C" void fn_8001C284() {
    unsigned char pairs[64][8];
    AIStringWithAllocator_unk values[64];
    void* objects[64];
#define REGISTER_AI_SCRIPT(I, NAME, CTOR) do { \
        std::string key(NAME); \
        objects[I] = ::operator new(4, 3, "", "", 0); \
        if (objects[I] != 0) { CTOR(objects[I]); } \
        fn_8001E0B8(&values[I], (const AIStringWithAllocator_unk*)&key, (const unsigned int*)&objects[I]); \
        fn_8001E1D0(pairs[I], lbl_807C6B84, &values[I]); \
        fn_8001E168(&values[I].mString, -1); \
    } while (0)
    REGISTER_AI_SCRIPT(0, "archer_attack01", __ct__Q32AI6Script18AI_archer_attack01Fv);
    REGISTER_AI_SCRIPT(1, "archer_guard01", __ct__Q32AI6Script17AI_archer_guard01Fv);
    REGISTER_AI_SCRIPT(2, "arganan01", __ct__Q32AI6Script12AI_arganan01Fv);
    REGISTER_AI_SCRIPT(3, "buffer_attackstable01", __ct__Q32AI6Script24AI_buffer_attackstable01Fv);
    REGISTER_AI_SCRIPT(4, "buffer_guard01", __ct__Q32AI6Script17AI_buffer_guard01Fv);
    REGISTER_AI_SCRIPT(5, "buffer_guard02", __ct__Q32AI6Script17AI_buffer_guard02Fv);
    REGISTER_AI_SCRIPT(6, "buffer_pointguard01", __ct__Q32AI6Script22AI_buffer_pointguard01Fv);
    REGISTER_AI_SCRIPT(7, "em_archer_attack02", __ct__Q32AI6Script21AI_em_archer_attack02Fv);
    REGISTER_AI_SCRIPT(8, "em_bridge_archer", __ct__Q32AI6Script19AI_em_bridge_archerFv);
    REGISTER_AI_SCRIPT(9, "em_monster_attack01", __ct__Q32AI6Script22AI_em_monster_attack01Fv);
    REGISTER_AI_SCRIPT(10, "em_scout", __ct__Q32AI6Script11AI_em_scoutFv);
    REGISTER_AI_SCRIPT(11, "em_sword_attack01", __ct__Q32AI6Script20AI_em_sword_attack01Fv);
    REGISTER_AI_SCRIPT(12, "em_sword_attack02", __ct__Q32AI6Script20AI_em_sword_attack02Fv);
    REGISTER_AI_SCRIPT(13, "em_sword_pointguard01", __ct__Q32AI6Script24AI_em_sword_pointguard01Fv);
    REGISTER_AI_SCRIPT(14, "em_type003", __ct__Q32AI6Script13AI_em_type003Fv);
    REGISTER_AI_SCRIPT(15, "em_type010", __ct__Q32AI6Script13AI_em_type010Fv);
    REGISTER_AI_SCRIPT(16, "em_type102", __ct__Q32AI6Script13AI_em_type102Fv);
    REGISTER_AI_SCRIPT(17, "em_type103", __ct__Q32AI6Script13AI_em_type103Fv);
    REGISTER_AI_SCRIPT(18, "em_type104", __ct__Q32AI6Script13AI_em_type104Fv);
    REGISTER_AI_SCRIPT(19, "em_type105", __ct__Q32AI6Script13AI_em_type105Fv);
    REGISTER_AI_SCRIPT(20, "em_type106", __ct__Q32AI6Script13AI_em_type106Fv);
    REGISTER_AI_SCRIPT(21, "fr_archer_pick01", __ct__Q32AI6Script19AI_fr_archer_pick01Fv);
    REGISTER_AI_SCRIPT(22, "fr_buffer_pick01", __ct__Q32AI6Script19AI_fr_buffer_pick01Fv);
    REGISTER_AI_SCRIPT(23, "fr_follow_player", __ct__Q32AI6Script19AI_fr_follow_playerFv);
    REGISTER_AI_SCRIPT(24, "fr_magic_pick01", __ct__Q32AI6Script18AI_fr_magic_pick01Fv);
    REGISTER_AI_SCRIPT(25, "fr_magic_pick02", __ct__Q32AI6Script18AI_fr_magic_pick02Fv);
    REGISTER_AI_SCRIPT(26, "fr_runaway", __ct__Q32AI6Script13AI_fr_runawayFv);
    REGISTER_AI_SCRIPT(27, "fr_sword_attack01", __ct__Q32AI6Script20AI_fr_sword_attack01Fv);
    REGISTER_AI_SCRIPT(28, "fr_sword_pick01", __ct__Q32AI6Script18AI_fr_sword_pick01Fv);
    REGISTER_AI_SCRIPT(29, "fr_sword_pointguard", __ct__Q32AI6Script22AI_fr_sword_pointguardFv);
    REGISTER_AI_SCRIPT(30, "fr_sword_pointguard01", __ct__Q32AI6Script24AI_fr_sword_pointguard01Fv);
    REGISTER_AI_SCRIPT(31, "fr_type005", __ct__Q32AI6Script13AI_fr_type005Fv);
    REGISTER_AI_SCRIPT(32, "fr_warppointescape", __ct__Q32AI6Script21AI_fr_warppointescapeFv);
    REGISTER_AI_SCRIPT(33, "jackal01", __ct__Q32AI6Script11AI_jackal01Fv);
    REGISTER_AI_SCRIPT(34, "magic_attack01", __ct__Q32AI6Script17AI_magic_attack01Fv);
    REGISTER_AI_SCRIPT(35, "magic_attack01_2", __ct__Q32AI6Script19AI_magic_attack01_2Fv);
    REGISTER_AI_SCRIPT(36, "magic_attack02", __ct__Q32AI6Script17AI_magic_attack02Fv);
    REGISTER_AI_SCRIPT(37, "magic_attackstable01", __ct__Q32AI6Script23AI_magic_attackstable01Fv);
    REGISTER_AI_SCRIPT(38, "magic_cure01", __ct__Q32AI6Script15AI_magic_cure01Fv);
    REGISTER_AI_SCRIPT(39, "magic_guard01", __ct__Q32AI6Script16AI_magic_guard01Fv);
    REGISTER_AI_SCRIPT(40, "magic_guard01_2", __ct__Q32AI6Script18AI_magic_guard01_2Fv);
    REGISTER_AI_SCRIPT(41, "magic_guard02", __ct__Q32AI6Script16AI_magic_guard02Fv);
    REGISTER_AI_SCRIPT(42, "np_approach", __ct__Q32AI6Script14AI_np_approachFv);
    REGISTER_AI_SCRIPT(43, "np_approach_plus", __ct__Q32AI6Script19AI_np_approach_plusFv);
    REGISTER_AI_SCRIPT(44, "np_follow_player", __ct__Q32AI6Script19AI_np_follow_playerFv);
    REGISTER_AI_SCRIPT(45, "np_tagger_random", __ct__Q32AI6Script19AI_np_tagger_randomFv);
    REGISTER_AI_SCRIPT(46, "np_type001", __ct__Q32AI6Script13AI_np_type001Fv);
    REGISTER_AI_SCRIPT(47, "np_type002", __ct__Q32AI6Script13AI_np_type002Fv);
    REGISTER_AI_SCRIPT(48, "np_type003", __ct__Q32AI6Script13AI_np_type003Fv);
    REGISTER_AI_SCRIPT(49, "np_type004", __ct__Q32AI6Script13AI_np_type004Fv);
    REGISTER_AI_SCRIPT(50, "np_type005", __ct__Q32AI6Script13AI_np_type005Fv);
    REGISTER_AI_SCRIPT(51, "np_wait_reaction", __ct__Q32AI6Script19AI_np_wait_reactionFv);
    REGISTER_AI_SCRIPT(52, "np_wait_reaction_minus", __ct__Q32AI6Script25AI_np_wait_reaction_minusFv);
    REGISTER_AI_SCRIPT(53, "np_wait_reaction_minus2", __ct__Q32AI6Script26AI_np_wait_reaction_minus2Fv);
    REGISTER_AI_SCRIPT(54, "np_wait_reaction_plus", __ct__Q32AI6Script24AI_np_wait_reaction_plusFv);
    REGISTER_AI_SCRIPT(55, "quark01", __ct__Q32AI6Script10AI_quark01Fv);
    REGISTER_AI_SCRIPT(56, "quark02", __ct__Q32AI6Script10AI_quark02Fv);
    REGISTER_AI_SCRIPT(57, "silence_guard01", __ct__Q32AI6Script18AI_silence_guard01Fv);
    REGISTER_AI_SCRIPT(58, "sword_guard01", __ct__Q32AI6Script16AI_sword_guard01Fv);
    REGISTER_AI_SCRIPT(59, "sword_leaderguard01", __ct__Q32AI6Script22AI_sword_leaderguard01Fv);
    REGISTER_AI_SCRIPT(60, "sword_leaderguard02", __ct__Q32AI6Script22AI_sword_leaderguard02Fv);
    REGISTER_AI_SCRIPT(61, "sword_leaderguard03", __ct__Q32AI6Script22AI_sword_leaderguard03Fv);
    REGISTER_AI_SCRIPT(62, "sword_pointguard01", __ct__Q32AI6Script21AI_sword_pointguard01Fv);
    REGISTER_AI_SCRIPT(63, "return_guard01", __ct__Q32AI6Script17AI_return_guard01Fv);
#undef REGISTER_AI_SCRIPT
}


extern "C" unsigned char* fn_8001C260() {
    return lbl_807C68C0;
}

extern "C" unsigned char* fn_8001C26C() {
    return lbl_807C6A40;
}

extern "C" unsigned char* fn_8001C278() {
    return lbl_807C6B30;
}

extern "C" int fn_8001EDF0() { return -1; }
extern "C" int fn_8001EDF8() { return -1; }
extern "C" int fn_8001EE30() { return -1; }
extern "C" int fn_8001EE38() { return -1; }
extern "C" int fn_8001EE40() { return -1; }
extern "C" int fn_8001EE48() { return -1; }
extern "C" int fn_8001EDE8() { return -1; }
extern "C" int fn_8001EE00() { return -1; }
extern "C" int fn_8001EE08() { return -1; }
extern "C" int fn_8001EE10() { return -1; }
extern "C" int fn_8001EE18() { return -1; }
extern "C" int fn_8001EE20() { return -1; }
extern "C" int fn_8001EE28() { return -1; }
extern "C" int fn_8001EE50() { return -1; }
extern "C" int fn_8003CDA8() { return -1; }

// AI/AI_archer_attack01 vtable constructors.
// inferred: factories in fn_8001C284 call these after allocation; each installs its own vtable.
// The class declarations below only model these constructors; object layouts remain unknown.
namespace AI {
namespace Script {
struct AI_archer_attack01 { AI_archer_attack01(); };
struct AI_archer_guard01 { AI_archer_guard01(); };
struct AI_arganan01 { AI_arganan01(); };
struct AI_buffer_attackstable01 { AI_buffer_attackstable01(); };
struct AI_buffer_guard01 { AI_buffer_guard01(); };
struct AI_buffer_guard02 { AI_buffer_guard02(); };
struct AI_buffer_pointguard01 { AI_buffer_pointguard01(); };
struct AI_em_archer_attack02 { AI_em_archer_attack02(); };
struct AI_em_bridge_archer { AI_em_bridge_archer(); };
struct AI_em_monster_attack01 { AI_em_monster_attack01(); };
struct AI_em_scout { AI_em_scout(); };
struct AI_em_sword_attack01 { AI_em_sword_attack01(); };
struct AI_em_sword_attack02 { AI_em_sword_attack02(); };
struct AI_em_sword_pointguard01 { AI_em_sword_pointguard01(); };
struct AI_em_type003 { AI_em_type003(); };
struct AI_em_type010 { AI_em_type010(); };
struct AI_em_type102 { AI_em_type102(); };
struct AI_em_type103 { AI_em_type103(); };
struct AI_em_type104 { AI_em_type104(); };
struct AI_em_type105 { AI_em_type105(); };
struct AI_em_type106 { AI_em_type106(); };
struct AI_fr_archer_pick01 { AI_fr_archer_pick01(); };
struct AI_fr_buffer_pick01 { AI_fr_buffer_pick01(); };
struct AI_fr_follow_player { AI_fr_follow_player(); };
struct AI_fr_magic_pick01 { AI_fr_magic_pick01(); };
struct AI_fr_magic_pick02 { AI_fr_magic_pick02(); };
struct AI_fr_runaway { AI_fr_runaway(); };
struct AI_fr_sword_attack01 { AI_fr_sword_attack01(); };
struct AI_fr_sword_pick01 { AI_fr_sword_pick01(); };
struct AI_fr_sword_pointguard { AI_fr_sword_pointguard(); };
struct AI_fr_sword_pointguard01 { AI_fr_sword_pointguard01(); };
struct AI_fr_type005 { AI_fr_type005(); };
struct AI_fr_warppointescape { AI_fr_warppointescape(); };
struct AI_jackal01 { AI_jackal01(); };
struct AI_magic_attack01 { AI_magic_attack01(); };
struct AI_magic_attack01_2 { AI_magic_attack01_2(); };
struct AI_magic_attack02 { AI_magic_attack02(); };
struct AI_magic_attackstable01 { AI_magic_attackstable01(); };
struct AI_magic_cure01 { AI_magic_cure01(); };
struct AI_magic_guard01 { AI_magic_guard01(); };
struct AI_magic_guard01_2 { AI_magic_guard01_2(); };
struct AI_magic_guard02 { AI_magic_guard02(); };
struct AI_np_approach { AI_np_approach(); };
struct AI_np_approach_plus { AI_np_approach_plus(); };
struct AI_np_follow_player { AI_np_follow_player(); };
struct AI_np_tagger_random { AI_np_tagger_random(); };
struct AI_np_type001 { AI_np_type001(); };
struct AI_np_type002 { AI_np_type002(); };
struct AI_np_type003 { AI_np_type003(); };
struct AI_np_type004 { AI_np_type004(); };
struct AI_np_type005 { AI_np_type005(); };
struct AI_np_wait_reaction { AI_np_wait_reaction(); };
struct AI_np_wait_reaction_minus { AI_np_wait_reaction_minus(); };
struct AI_np_wait_reaction_minus2 { AI_np_wait_reaction_minus2(); };
struct AI_np_wait_reaction_plus { AI_np_wait_reaction_plus(); };
struct AI_quark01 { AI_quark01(); };
struct AI_quark02 { AI_quark02(); };
struct AI_silence_guard01 { AI_silence_guard01(); };
struct AI_sword_guard01 { AI_sword_guard01(); };
struct AI_sword_leaderguard01 { AI_sword_leaderguard01(); };
struct AI_sword_leaderguard02 { AI_sword_leaderguard02(); };
struct AI_sword_leaderguard03 { AI_sword_leaderguard03(); };
struct AI_sword_pointguard01 { AI_sword_pointguard01(); };
struct AI_return_guard01 { AI_return_guard01(); };
}
}

extern "C" {
extern char __vt__Q32AI6Script18AI_archer_attack01[];
extern char __vt__Q32AI6Script17AI_archer_guard01[];
extern char __vt__Q32AI6Script12AI_arganan01[];
extern char __vt__Q32AI6Script24AI_buffer_attackstable01[];
extern char __vt__Q32AI6Script17AI_buffer_guard01[];
extern char __vt__Q32AI6Script17AI_buffer_guard02[];
extern char __vt__Q32AI6Script22AI_buffer_pointguard01[];
extern char __vt__Q32AI6Script21AI_em_archer_attack02[];
extern char __vt__Q32AI6Script19AI_em_bridge_archer[];
extern char __vt__Q32AI6Script22AI_em_monster_attack01[];
extern char __vt__Q32AI6Script11AI_em_scout[];
extern char __vt__Q32AI6Script20AI_em_sword_attack01[];
extern char __vt__Q32AI6Script20AI_em_sword_attack02[];
extern char __vt__Q32AI6Script24AI_em_sword_pointguard01[];
extern char __vt__Q32AI6Script13AI_em_type003[];
extern char __vt__Q32AI6Script13AI_em_type010[];
extern char __vt__Q32AI6Script13AI_em_type102[];
extern char __vt__Q32AI6Script13AI_em_type103[];
extern char __vt__Q32AI6Script13AI_em_type104[];
extern char __vt__Q32AI6Script13AI_em_type105[];
extern char __vt__Q32AI6Script13AI_em_type106[];
extern char __vt__Q32AI6Script19AI_fr_archer_pick01[];
extern char __vt__Q32AI6Script19AI_fr_buffer_pick01[];
extern char __vt__Q32AI6Script19AI_fr_follow_player[];
extern char __vt__Q32AI6Script18AI_fr_magic_pick01[];
extern char __vt__Q32AI6Script18AI_fr_magic_pick02[];
extern char __vt__Q32AI6Script13AI_fr_runaway[];
extern char __vt__Q32AI6Script20AI_fr_sword_attack01[];
extern char __vt__Q32AI6Script18AI_fr_sword_pick01[];
extern char __vt__Q32AI6Script22AI_fr_sword_pointguard[];
extern char __vt__Q32AI6Script24AI_fr_sword_pointguard01[];
extern char __vt__Q32AI6Script13AI_fr_type005[];
extern char __vt__Q32AI6Script21AI_fr_warppointescape[];
extern char __vt__Q32AI6Script11AI_jackal01[];
extern char __vt__Q32AI6Script17AI_magic_attack01[];
extern char __vt__Q32AI6Script19AI_magic_attack01_2[];
extern char __vt__Q32AI6Script17AI_magic_attack02[];
extern char __vt__Q32AI6Script23AI_magic_attackstable01[];
extern char __vt__Q32AI6Script15AI_magic_cure01[];
extern char __vt__Q32AI6Script16AI_magic_guard01[];
extern char __vt__Q32AI6Script18AI_magic_guard01_2[];
extern char __vt__Q32AI6Script16AI_magic_guard02[];
extern char __vt__Q32AI6Script14AI_np_approach[];
extern char __vt__Q32AI6Script19AI_np_approach_plus[];
extern char __vt__Q32AI6Script19AI_np_follow_player[];
extern char __vt__Q32AI6Script19AI_np_tagger_random[];
extern char __vt__Q32AI6Script13AI_np_type001[];
extern char __vt__Q32AI6Script13AI_np_type002[];
extern char __vt__Q32AI6Script13AI_np_type003[];
extern char __vt__Q32AI6Script13AI_np_type004[];
extern char __vt__Q32AI6Script13AI_np_type005[];
extern char __vt__Q32AI6Script19AI_np_wait_reaction[];
extern char __vt__Q32AI6Script25AI_np_wait_reaction_minus[];
extern char __vt__Q32AI6Script26AI_np_wait_reaction_minus2[];
extern char __vt__Q32AI6Script24AI_np_wait_reaction_plus[];
extern char __vt__Q32AI6Script10AI_quark01[];
extern char __vt__Q32AI6Script10AI_quark02[];
extern char __vt__Q32AI6Script18AI_silence_guard01[];
extern char __vt__Q32AI6Script16AI_sword_guard01[];
extern char __vt__Q32AI6Script22AI_sword_leaderguard01[];
extern char __vt__Q32AI6Script22AI_sword_leaderguard02[];
extern char __vt__Q32AI6Script22AI_sword_leaderguard03[];
extern char __vt__Q32AI6Script21AI_sword_pointguard01[];
extern char __vt__Q32AI6Script17AI_return_guard01[];
}

namespace AI {
namespace Script {
AI_archer_attack01::AI_archer_attack01() { *(void**)this = ::__vt__Q32AI6Script18AI_archer_attack01; } // 8001E0A8
AI_archer_guard01::AI_archer_guard01() { *(void**)this = ::__vt__Q32AI6Script17AI_archer_guard01; } // 8001E340
AI_arganan01::AI_arganan01() { *(void**)this = ::__vt__Q32AI6Script12AI_arganan01; } // 8001E350
AI_buffer_attackstable01::AI_buffer_attackstable01() { *(void**)this = ::__vt__Q32AI6Script24AI_buffer_attackstable01; } // 8001E360
AI_buffer_guard01::AI_buffer_guard01() { *(void**)this = ::__vt__Q32AI6Script17AI_buffer_guard01; } // 8001E370
AI_buffer_guard02::AI_buffer_guard02() { *(void**)this = ::__vt__Q32AI6Script17AI_buffer_guard02; } // 8001E380
AI_buffer_pointguard01::AI_buffer_pointguard01() { *(void**)this = ::__vt__Q32AI6Script22AI_buffer_pointguard01; } // 8001E390
AI_em_archer_attack02::AI_em_archer_attack02() { *(void**)this = ::__vt__Q32AI6Script21AI_em_archer_attack02; } // 8001E3A0
AI_em_bridge_archer::AI_em_bridge_archer() { *(void**)this = ::__vt__Q32AI6Script19AI_em_bridge_archer; } // 8001E3B0
AI_em_monster_attack01::AI_em_monster_attack01() { *(void**)this = ::__vt__Q32AI6Script22AI_em_monster_attack01; } // 8001E3C0
AI_em_scout::AI_em_scout() { *(void**)this = ::__vt__Q32AI6Script11AI_em_scout; } // 8001E3D0
AI_em_sword_attack01::AI_em_sword_attack01() { *(void**)this = ::__vt__Q32AI6Script20AI_em_sword_attack01; } // 8001E3E0
AI_em_sword_attack02::AI_em_sword_attack02() { *(void**)this = ::__vt__Q32AI6Script20AI_em_sword_attack02; } // 8001E3F0
AI_em_sword_pointguard01::AI_em_sword_pointguard01() { *(void**)this = ::__vt__Q32AI6Script24AI_em_sword_pointguard01; } // 8001E400
AI_em_type003::AI_em_type003() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type003; } // 8001E410
AI_em_type010::AI_em_type010() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type010; } // 8001E420
AI_em_type102::AI_em_type102() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type102; } // 8001E430
AI_em_type103::AI_em_type103() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type103; } // 8001E440
AI_em_type104::AI_em_type104() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type104; } // 8001E450
AI_em_type105::AI_em_type105() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type105; } // 8001E460
AI_em_type106::AI_em_type106() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type106; } // 8001E470
AI_fr_archer_pick01::AI_fr_archer_pick01() { *(void**)this = ::__vt__Q32AI6Script19AI_fr_archer_pick01; } // 8001E480
AI_fr_buffer_pick01::AI_fr_buffer_pick01() { *(void**)this = ::__vt__Q32AI6Script19AI_fr_buffer_pick01; } // 8001E490
AI_fr_follow_player::AI_fr_follow_player() { *(void**)this = ::__vt__Q32AI6Script19AI_fr_follow_player; } // 8001E4A0
AI_fr_magic_pick01::AI_fr_magic_pick01() { *(void**)this = ::__vt__Q32AI6Script18AI_fr_magic_pick01; } // 8001E4B0
AI_fr_magic_pick02::AI_fr_magic_pick02() { *(void**)this = ::__vt__Q32AI6Script18AI_fr_magic_pick02; } // 8001E4C0
AI_fr_runaway::AI_fr_runaway() { *(void**)this = ::__vt__Q32AI6Script13AI_fr_runaway; } // 8001E4D0
AI_fr_sword_attack01::AI_fr_sword_attack01() { *(void**)this = ::__vt__Q32AI6Script20AI_fr_sword_attack01; } // 8001E4E0
AI_fr_sword_pick01::AI_fr_sword_pick01() { *(void**)this = ::__vt__Q32AI6Script18AI_fr_sword_pick01; } // 8001E4F0
AI_fr_sword_pointguard::AI_fr_sword_pointguard() { *(void**)this = ::__vt__Q32AI6Script22AI_fr_sword_pointguard; } // 8001E500
AI_fr_sword_pointguard01::AI_fr_sword_pointguard01() { *(void**)this = ::__vt__Q32AI6Script24AI_fr_sword_pointguard01; } // 8001E510
AI_fr_type005::AI_fr_type005() { *(void**)this = ::__vt__Q32AI6Script13AI_fr_type005; } // 8001E520
AI_fr_warppointescape::AI_fr_warppointescape() { *(void**)this = ::__vt__Q32AI6Script21AI_fr_warppointescape; } // 8001E530
AI_jackal01::AI_jackal01() { *(void**)this = ::__vt__Q32AI6Script11AI_jackal01; } // 8001E540
AI_magic_attack01::AI_magic_attack01() { *(void**)this = ::__vt__Q32AI6Script17AI_magic_attack01; } // 8001E550
AI_magic_attack01_2::AI_magic_attack01_2() { *(void**)this = ::__vt__Q32AI6Script19AI_magic_attack01_2; } // 8001E560
AI_magic_attack02::AI_magic_attack02() { *(void**)this = ::__vt__Q32AI6Script17AI_magic_attack02; } // 8001E570
AI_magic_attackstable01::AI_magic_attackstable01() { *(void**)this = ::__vt__Q32AI6Script23AI_magic_attackstable01; } // 8001E580
AI_magic_cure01::AI_magic_cure01() { *(void**)this = ::__vt__Q32AI6Script15AI_magic_cure01; } // 8001E590
AI_magic_guard01::AI_magic_guard01() { *(void**)this = ::__vt__Q32AI6Script16AI_magic_guard01; } // 8001E5A0
AI_magic_guard01_2::AI_magic_guard01_2() { *(void**)this = ::__vt__Q32AI6Script18AI_magic_guard01_2; } // 8001E5B0
AI_magic_guard02::AI_magic_guard02() { *(void**)this = ::__vt__Q32AI6Script16AI_magic_guard02; } // 8001E5C0
AI_np_approach::AI_np_approach() { *(void**)this = ::__vt__Q32AI6Script14AI_np_approach; } // 8001E5D0
AI_np_approach_plus::AI_np_approach_plus() { *(void**)this = ::__vt__Q32AI6Script19AI_np_approach_plus; } // 8001E5E0
AI_np_follow_player::AI_np_follow_player() { *(void**)this = ::__vt__Q32AI6Script19AI_np_follow_player; } // 8001E5F0
AI_np_tagger_random::AI_np_tagger_random() { *(void**)this = ::__vt__Q32AI6Script19AI_np_tagger_random; } // 8001E600
AI_np_type001::AI_np_type001() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type001; } // 8001E610
AI_np_type002::AI_np_type002() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type002; } // 8001E620
AI_np_type003::AI_np_type003() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type003; } // 8001E630
AI_np_type004::AI_np_type004() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type004; } // 8001E640
AI_np_type005::AI_np_type005() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type005; } // 8001E650
AI_np_wait_reaction::AI_np_wait_reaction() { *(void**)this = ::__vt__Q32AI6Script19AI_np_wait_reaction; } // 8001E660
AI_np_wait_reaction_minus::AI_np_wait_reaction_minus() { *(void**)this = ::__vt__Q32AI6Script25AI_np_wait_reaction_minus; } // 8001E670
AI_np_wait_reaction_minus2::AI_np_wait_reaction_minus2() { *(void**)this = ::__vt__Q32AI6Script26AI_np_wait_reaction_minus2; } // 8001E680
AI_np_wait_reaction_plus::AI_np_wait_reaction_plus() { *(void**)this = ::__vt__Q32AI6Script24AI_np_wait_reaction_plus; } // 8001E690
AI_quark01::AI_quark01() { *(void**)this = ::__vt__Q32AI6Script10AI_quark01; } // 8001E6A0
AI_quark02::AI_quark02() { *(void**)this = ::__vt__Q32AI6Script10AI_quark02; } // 8001E6B0
AI_silence_guard01::AI_silence_guard01() { *(void**)this = ::__vt__Q32AI6Script18AI_silence_guard01; } // 8001E6C0
AI_sword_guard01::AI_sword_guard01() { *(void**)this = ::__vt__Q32AI6Script16AI_sword_guard01; } // 8001E6D0
AI_sword_leaderguard01::AI_sword_leaderguard01() { *(void**)this = ::__vt__Q32AI6Script22AI_sword_leaderguard01; } // 8001E6E0
AI_sword_leaderguard02::AI_sword_leaderguard02() { *(void**)this = ::__vt__Q32AI6Script22AI_sword_leaderguard02; } // 8001E6F0
AI_sword_leaderguard03::AI_sword_leaderguard03() { *(void**)this = ::__vt__Q32AI6Script22AI_sword_leaderguard03; } // 8001E700
AI_sword_pointguard01::AI_sword_pointguard01() { *(void**)this = ::__vt__Q32AI6Script21AI_sword_pointguard01; } // 8001E710
AI_return_guard01::AI_return_guard01() { *(void**)this = ::__vt__Q32AI6Script17AI_return_guard01; } // 8001E720
}
}
