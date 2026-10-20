#include "game/PickTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/ChaseManager.h"
#include "game/PlayerTask.h"

// inferred: 0x934-byte character slot of the battle manager (only the character is known)
struct PickSlot_unk {
    CharaTask* mChara;
    unsigned char m04[0x934 - 0x4];
};
// inferred: battle manager (lbl_8087F048, see game/CharaTask.cpp)
// inferred: character list of the battle manager (names invented)
struct PickSlots_unk {
    CharaTask* GetChara_unk(int i) {
        PickSlot_unk* s = mSlots;
        return s[i].mChara;
    }

    PickSlot_unk mSlots[0x48];
};
struct PickBattle_unk {
    unsigned char m00[0xCBF0];
    PickSlots_unk mSlots; // 0xCBF0
    unsigned char m363B0_pad[0x363B0 - (0xCBF0 + 0x48 * 0x934)];
    int mCount; // 0x363B0
};
extern "C" PickBattle_unk* lbl_8087F048;
extern "C" int fn_80102890(PickBattle_unk* mgr, CharaTask* chara); // inferred: slot index of a character

// inferred: area of the map (0x30 bytes)
struct PickArea_unk {
    int m00;
    atn::Vector3 mPos;  // 0x04
    int m10;
    float mRotY;        // 0x14
    atn::Vector3 mSize; // 0x18
    int mId;            // 0x24
    unsigned char m28[0x30 - 0x28];
};
struct PickAreaTable_unk {
    unsigned char m00[0xF4];
    unsigned int mNumAreas; // 0xF4
    PickArea_unk* mAreas;   // 0xF8
};
// inferred: field object (see gmk/ReinforcePoint.cpp)
struct PickField_unk {
    unsigned char m00[0x10D8];
    PickAreaTable_unk* m10D8;
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4;
};
extern "C" PickField_unk* lbl_8087F430;
extern "C" int fn_80370174(PickField_unk* field, int id); // inferred: event flag
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;     // inferred: player manager
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int flags, void* colli, int arg);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
// inferred: oriented box (half size, then the matrix)
struct PickBox_unk {
    atn::Vector3 mSize;
    atn::Matrix3x4 mMtx;
};
extern "C" int fn_80051CD8(const CharaTask::Sphere_unk* sphere, const PickBox_unk* box); // inferred: sphere in box
extern "C" void fn_8011CD84(CharaD74_unk* label, CharaTask* chara, int kind); // inferred: shows a label
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
// inferred: link of the character lists (CharaTask + 0x14A8)
struct PickCharaNode_unk {
    unsigned char m0000[0x14A8];
    unsigned int m14A8;
};

// inferred: sound names (separate 8-aligned objects before the table in .rodata)
static const char sSound0[] = "SE_UI001_019";
static const char sSound1[] = "SE_UI001_020";
static const char sSound2[] = "SE_UI001_021";
static const char sSound3[] = "SE_UI001_022";
static const char* const sSounds[] = {sSound0, sSound1, sSound2, sSound3};

PickTask* PickTask::sInstance;

PickTask* PickTask::fn_80548D54(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (3, "", "", 0) PickTask(parent);
    }
    return sInstance;
}

PickTask::~PickTask() {
    sInstance = 0;
}

int PickTask::vf0C() {
    return fn_800D3FA4() != 0;
}

// inferred: bit 31 of the flag word through an accessor (compared with 1); name invented
inline unsigned int GetFlag31_unk(const CharaTask* chara) {
    return chara->mFlag31;
}


// inferred: dot product of `d` with the facing direction of `chara` (name invented)
inline float FacingDot_unk(CharaTask* chara, const atn::Vector3* d) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx rot;
    PSMTXRotRad(rot, 'y', chara->mRot.y);
    PSMTXMultVec(rot, &dir, &dir);
    return PSVECDotProduct(d, &dir);
}
// inferred: flag test with the flag as a parameter (not folded into a bit extraction); name invented
inline bool HasLabelFlag_unk(const CharaD74_unk* label, unsigned int flag) {
    return (label->m20 & flag) == flag;
}

inline void RotateY_unk(Mtx m, float rad) {
    Mtx r;
    PSMTXRotRad(r, 'y', rad);
    Mtx t;
    PSMTXConcat(m, r, t);
    __MtxCopy(m, t);
}

// NONMATCHING: 96.5% - register allocation of the second loop (the original keeps i*0x120 in r14 and spills
// wasSafe/near/detect to the stack); `n = min(count, 72)` (bge/b/li in the original) comes out right only
// with a local list pointer (`l->mCount < 72 ? l->mCount : 72`), which shifts other registers; the player
// sub-state test `== 6 || == 7` uses two cmpwi in the original only because r0 is live there (bool of
// fn_8011D21C), here it is folded into a range check.
// inferred: every frame, builds m4C (which enemy sees which character) and updates the alert label of every
// enemy (CharaD74_unk) from what it sees, its safe/unsafe areas and the player
void PickTask::vf10() {
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 0) {
        return;
    }
    if (lbl_8087F098->mState != 0) {
        return;
    }
    CharaTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (player == 0) {
        return;
    }
    int playerIndex = fn_80102890(lbl_8087F048, player);
    if (playerIndex < 0) {
        return;
    }
    bool onLedge = GetFlag31_unk(player) == 1;
    memset(m4C, 0, sizeof(m4C));
    int n = lbl_8087F048->mCount;
    if (n >= 0x48) {
        n = 0x48;
    }
    int numSeers = 0;
    for (int i = 0; i < n; i++) {
        CharaTask* a = lbl_8087F048->mSlots.GetChara_unk(i);
        if (a != 0 && a->mType == 2 && a->CanAct_unk() && (a->m54C & 0x2000) != 0x2000 && a->mC58.mC0 > 0 &&
            a->mC58.m11C.fn_8011D1FC()) {
            float warn = a->mC58.m11C.m18;
            float sight = a->mC58.m11C.m1C;
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    continue;
                }
                CharaTask* b = lbl_8087F048->mSlots.GetChara_unk(j);
                if (b == 0 || (b->mType != 2 && b->mType != 0)) {
                    continue;
                }
                if (!b->CanAct_unk() || (b->m54C & 0x2000) == 0x2000 || b->mC58.mC0 <= 0) {
                    continue;
                }
                if (!b->mC58.m11C.fn_8011D1FC() && b->mType != 0) {
                    continue;
                }
                atn::Vector3 d = b->mPos - a->mPos;
                float distSq = fn_805F9920(&d);
                if (distSq > 0.0f) {
                    PSVECNormalize(&d, &d);
                }
                if (distSq < sight * sight) {
                    atn::Vector3 dirA(0.0f, 0.0f, 1.0f);
                    Mtx rotA;
                    PSMTXRotRad(rotA, 'y', a->mRot.y);
                    PSMTXMultVec(rotA, &dirA, &dirA);
                    if (PSVECDotProduct(&d, &dirA) >= 0.5f &&
                        (!b->mFlag31 || FacingDot_unk(b, &d) > 0.0f) &&
                        fn_8004ECC0(lbl_8087EE98, 0, &a->mColli.m48, &b->mColli.m48, 0x80000000, 0, 0) == 0) {
                        m4C[i][j] |= 1;
                    }
                }
                if (distSq < warn * warn) {
                    m4C[i][j] |= 2;
                }
            }
            numSeers++;
        }
    }
    for (int i = 0; i < n; i++) {
        CharaTask* c = lbl_8087F048->mSlots.GetChara_unk(i);
        CharaD74_unk* label = &c->mC58.m11C;
        if (!c->CanAct_unk() || c->mType != 2 || c->mC58.mC0 <= 0) {
            label->m01 = 0;
            continue;
        }
        if ((c->mStatus.m0C & 0x10) == 0x10 || (c->mStatus.m0C & 0x40) == 0x40) {
            label->m01 = 0;
            continue;
        }
        if (label->m00 == 0) {
            continue;
        }
        int prev = label->m01;
        bool safe = false;
        label->m01 = label->m00;
        bool wasSafe = HasLabelFlag_unk(label, 2);
        if (label->m1C > 0.0f) {
            safe = true;
            for (int k = 0; k < n; k++) {
                if (m4C[k][i] != 0) {
                    safe = false;
                    break;
                }
            }
        }
        if (label->m0C > 0 || label->m10 > 0) {
            PickAreaTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
            if (table != 0) {
                bool inSafe = false;
                bool inUnsafe = false;
                for (unsigned int k = 0; k < table->mNumAreas; k++) {
                    PickArea_unk* area = &table->mAreas[k];
                    if (area->mId == label->m0C || area->mId == label->m10) {
                        CharaTask::Sphere_unk* sphere = (CharaTask::Sphere_unk*)&c->m614;
                        PickBox_unk box;
                        box.mMtx.SetIdentity();
                        RotateY_unk(box.mMtx.m, area->mRotY);
                        atn::Vector3 off(0.0f, area->mSize.y, 0.0f);
                        atn::Vector3 pos = area->mPos + off;
                        box.mMtx.m[0][3] = pos.x;
                        box.mMtx.m[1][3] = pos.y;
                        box.mMtx.m[2][3] = pos.z;
                        box.mSize = area->mSize;
                        box.mSize.y += 10.0f;
                        if (fn_80051CD8(sphere, &box)) {
                            if (area->mId == label->m0C) {
                                inSafe = true;
                                break;
                            }
                            inUnsafe = true;
                        }
                    }
                }
                if (inSafe) {
                    safe = true;
                } else if (inUnsafe) {
                    safe = false;
                }
            }
        }
        if (!safe && lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0xD8) == 1 && numSeers == 1) {
            atn::Vector3 d = player->mPos - c->mPos;
            if (fn_805F9920(&d) < 4900.0f) {
                safe = true;
            }
        }
        if (safe) {
            label->m20 |= 2;
        } else {
            label->m20 &= ~2;
        }
        if (prev != label->m01) {
            switch (label->m01) {
            case 3:
            case 4:
            case 5:
            case 6:
                switch (prev) {
                case 3:
                case 4:
                case 5:
                case 6:
                    break;
                default:
                    fn_800C31F4(sSounds[0], 0, -1, 1.0f);
                    break;
                }
                break;
            case 1:
                if ((label->m20 & 4) != 4) {
                    fn_800C31F4(sSounds[1], 0, -1, 1.0f);
                }
                break;
            }
        }
        if (label->m01 != 1 && label->m01 != 2 && !wasSafe && (label->m20 & 2) == 2) {
            fn_800C31F4(sSounds[2], 0, -1, 1.0f);
        }
        atn::Vector3 d = player->mPos - c->mPos;
        float distSq = fn_805F9920(&d);
        if (distSq > 0.0f) {
            PSVECNormalize(&d, &d);
        }
        float radius = 5.0f + (c->m5B0 + player->m5B0);
        bool special = label->fn_8011D21C() == 1;
        bool kind46 = false;
        if (4 <= label->m00 && label->m00 <= 6) {
            kind46 = true;
        }
        bool kind45 = false;
        if (4 <= label->m00 && label->m00 <= 5) {
            kind45 = true;
        }
        bool near = distSq < radius * radius;
        bool detect = distSq < label->m14 * label->m14;
        bool alert = false;
        bool lost = false;
        int seen = m4C[i][playerIndex] & 1;
        if (player->m55C == 6 && (player->m560 == 6 || player->m560 == 7)) {
            onLedge = true;
            seen = 0;
            near = false;
            detect = false;
        }
        if (!special) {
            ((PickCharaNode_unk*)c)->m14A8 &= ~0x04000000;
            if (player->mFlag28 && seen) {
                lost = true;
            } else if (!kind45) {
                for (int k = 0; k < n; k++) {
                    CharaTask* o = lbl_8087F048->mSlots.GetChara_unk(k);
                    int kind = o->mC58.m11C.m00;
                    if (c == o) {
                        continue;
                    }
                    if (!o->CanAct_unk() || (o->m54C & 0x2000) == 0x2000) {
                        continue;
                    }
                    if (!o->fn_8016F3D0(c, 1) || o->mC58.mC0 <= 0) {
                        continue;
                    }
                    if (label->m1C > 0.0f) {
                        if (!o->mC58.m11C.fn_8011D1FC() || kind == 2 || kind == 3) {
                            continue;
                        }
                        if (kind == 1 && m4C[i][k] != 0) {
                            alert = true;
                            break;
                        }
                        if (!kind46 && 4 <= kind && kind <= 5 && (m4C[i][k] & 1) == 1) {
                            alert = true;
                            break;
                        }
                    } else {
                        if (o->mC58.m11C.fn_8011D1FC() && kind == 1 && !kind46 && (m4C[i][k] & 2) == 2) {
                            alert = true;
                            break;
                        }
                    }
                }
            }
        }
        if ((detect && !onLedge) || near) {
            lost = true;
        }
        switch (label->m00) {
        case 2:
            if (seen || alert) {
                fn_8011CD84(label, c, 3);
            }
            break;
        case 3:
            if (alert) {
                fn_8011CD84(label, c, 4);
            } else if (!seen) {
                fn_8011CD84(label, c, 2);
            } else if (++label->m02 > 150) {
                lost = true;
            }
            break;
        case 4:
            if (seen) {
                lost = true;
            } else if (c->mRoute.m2C != 0) {
                fn_8011CD84(label, c, 5);
            } else {
                if (seen) {
                    label->m02++;
                } else if (label->m02 > 0) {
                    int v = label->m02 - 3;
                    label->m02 = v > 0 ? v : 0;
                }
                if (label->m02 > 150) {
                    lost = true;
                }
            }
            break;
        case 5:
            if (seen) {
                lost = true;
            } else if (c->m55C != 6) {
                fn_8011CD84(label, c, 6);
            }
            break;
        case 6:
            if (alert) {
                fn_8011CD84(label, c, 4);
            } else if (seen) {
                lost = true;
            } else if (c->mRoute.m2C != 0) {
                fn_8011CD84(label, c, 2);
            }
            break;
        case 1:
            label->m04++;
            break;
        }
        if (label->m00 != 1 && lost) {
            if (++label->m04 < 45 && !near) {
                continue;
            }
            fn_8011CD84(label, c, 1);
        }
    }
}
