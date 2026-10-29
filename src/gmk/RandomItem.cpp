#include "gmk/RandomItem.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaStatus.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: field (lbl_8087F430) with its route tables
struct RandomItemField_unk {
    unsigned char m00[0x10D0];
    int m10D0; // inferred: level of the random tables
    unsigned char m10D4[0x10D8 - 0x10D4];
    RouteTable_unk* m10D8;
};
extern "C" RandomItemField_unk* lbl_8087F430;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" int rand();
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct RandomItemEffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct RandomItemColor_unk {
    RandomItemColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" RandomItemEffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(RandomItemEffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const RandomItemColor_unk& color,
                            int arg7, int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(RandomItemEffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" int fn_80435908(gmk::RandomItem* item); // inferred: places the item at a random point
struct RandomTable_unk;
extern "C" RandomTable_unk* fn_8021D720(int level, int id); // inferred: random table of a level and id
extern "C" bool fn_8021D770(RandomTable_unk* table, int* item, char* name, int index); // inferred: random item
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
// inferred: sphere and oriented box of the collision tests
struct RandomItemSphere_unk {
    atn::Vector3 mCenter;
    float mRadius;
};
struct RandomItemBox_unk {
    atn::Vector3 mSize;    // inferred: half size
    atn::Matrix3x4 mMtx;
};
extern "C" int fn_80051CD8(const RandomItemSphere_unk* sphere, const RandomItemBox_unk* box); // inferred: sphere in box
// inferred: debug drawing (lbl_8087EEB0)
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063D3C(void* dbg, const atn::Vector3* pos, unsigned int color, float radius, float arg);
extern "C" void fn_800629F0(void* dbg, const atn::Vector3* pos, const atn::Vector3* size, unsigned int color,
                            const atn::Matrix3x4* mtx, float arg);

inline void CopyMtx_unk(const Mtx src, Mtx dst) {
    __MtxCopy(dst, src);
}
inline void SetTrans_unk(atn::Matrix3x4& m, const atn::Vector3& v) {
    m.m[0][3] = v.x;
    m.m[1][3] = v.y;
    m.m[2][3] = v.z;
}
inline void SetLocTrans_unk(atn::Matrix3x4& m, const gmk::RandomItemLoc_unk& loc) {
    atn::Vector3 off(0.0f, loc.mSize.y, 0.0f);
    SetTrans_unk(m, loc.mPos + off);
}
// inferred: matrix of a place: rotation around y, then the position raised by half the box
inline void MakeLocMtx_unk(atn::Matrix3x4& m, const gmk::RandomItemLoc_unk& loc) {
    Mtx t;
    Mtx r;
    m.SetIdentity();
    PSMTXRotRad(r, 'y', loc.mRotY);
    PSMTXConcat(m.m, r, t);
    __MtxCopy(m.m, t);
    atn::Vector3 off(0.0f, loc.mSize.y, 0.0f);
    SetTrans_unk(m, loc.mPos + off);
}

namespace gmk {

GimmickCommon* RandomItem::fn_80432CF0(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) RandomItem(parent, id, arg);
    }
    return 0;
}

RandomItem::RandomItem(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), m118(0), m11C(0), m120(10.0f), m124(0), m128(0), m144(0), m148(0), m14C(0),
      m150(0), m154(0.0f, 0.0f, 0.0f), m160(0), m164(5), m168(0), m260(0) {
    m54 = 0;
    mAnime.fn_8044D9BC(this);
}

RandomItem::~RandomItem() {}

int RandomItem::vf0C() {
    return fn_803EC91C() == 0;
}

void RandomItem::Start() {
    m54 = 1;
    RouteTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (table == 0) {
        return;
    }
    RoutePoint_unk* point;
    int n = table->mNumPoints;
    for (unsigned int i = 0; i < mDrops.mSize; i++) {
        RandomItemDrop_unk& drop = mDrops[i];
        for (int j = 1; j <= n; j++) {
            point = &table->mPoints[j - 1];
            RandomItemSphere_unk sphere;
            sphere.mCenter = point->mPos;
            sphere.mRadius = 0.5f * point->mRadius;
            for (unsigned int k = 0; k < drop.mLocs.mSize; k++) {
                RandomItemLoc_unk& loc = drop.mLocs[k];
                RandomItemBox_unk box;
                MakeLocMtx_unk(box.mMtx, loc);
                box.mSize = loc.mSize;
                if (fn_80051CD8(&sphere, &box) != 0) {
                    drop.mPoints.push_back(point);
                    break;
                }
            }
        }
    }
}

void RandomItem::vf10() {
    if (m54 == 0) {
        m118 = 0;
        if (m11C != 0) {
            fn_80435684(0);
        }
    } else {
        fn_8043465C();
        fn_804348C4();
    }
}

// NONMATCHING: 99.9% - the two loop-invariant addresses (concat temporary and matrix) are hoisted
// in the other order; tried the matrix code in an inline helper and both argument orders of the copy
void RandomItem::vf14() {
    if (m128 == 0) {
        return;
    }
    if ((unsigned int)(m11C - 2) <= 1) {
        fn_80063D3C(lbl_8087EEB0, &m154, 0xFFFFFF00, 3.0f, 0.0f);
    }
    for (unsigned int i = 0; i < mDrops.mSize; i++) {
        RandomItemDrop_unk* drop = &mDrops[i];
        unsigned int color = 0xFFFFAA00;
        if (drop != m144) {
            color = 0x66FFAA00;
        }
        for (unsigned int j = 0; j < drop->mLocs.mSize; j++) {
            RandomItemLoc_unk& loc = drop->mLocs[j];
            atn::Matrix3x4 mtx;
            Mtx r;
            Mtx t;
            mtx.SetIdentity();
            PSMTXRotRad(r, 'y', loc.mRotY);
            PSMTXConcat(mtx.m, r, t);
            CopyMtx_unk(t, mtx.m);
            SetLocTrans_unk(mtx, loc);
            fn_800629F0(lbl_8087EEB0, &lbl_807C7030, &loc.mSize, color, &mtx, 0.0f);
        }
    }
}

int RandomItem::vf1C() {
    if (mEfp.fn_80237874() != 0 || mEfpItem.fn_80237874() != 0 || mEfpGet.fn_80237874() != 0) {
        return 1;
    }
    return mAnime.fn_8044E1BC() != 0;
}

// NONMATCHING: 98.7% - register allocation of the inlined Vector_unk reserve/Assign_unk copies
// (source and destination addresses of the copy loops are swapped, r17-r23 permuted); tried explicit
// and implicit assignments of the elements and other orders of the "LOC" lookup
void RandomItem::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_ITEM") == 0) {
            mEfpItem.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_GET") == 0) {
            mEfpGet.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "DROP") == 0) {
            m120 = fn_800DC288(csv.fn_8005B9CC());
            m124 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(field, "ITEM") == 0) {
            RandomItemDrop_unk drop;
            drop.mId = atoi(csv.fn_8005B9CC());
            drop.m04 = atoi(csv.fn_8005B9CC());
            drop.m818 = fn_800DC288(csv.fn_8005B9CC());
            drop.m81C = atoi(csv.fn_8005B9CC());
            drop.m820 = atoi(csv.fn_8005B9CC());
            drop.m824 = atoi(csv.fn_8005B9CC());
            drop.m828 = atoi(csv.fn_8005B9CC());
            drop.m82C = atoi(csv.fn_8005B9CC());
            mDrops.push_back(drop);
        } else if (strcmp(field, "LOC") == 0) {
            int id = atoi(csv.fn_8005B9CC());
            for (unsigned int i = 0; i < mDrops.mSize; i++) {
                RandomItemDrop_unk& drop = mDrops[i];
                if (id == drop.mId) {
                    RandomItemLoc_unk loc;
                    loc.mPos.x = fn_800DC288(csv.fn_8005B9CC());
                    loc.mPos.y = fn_800DC288(csv.fn_8005B9CC());
                    loc.mPos.z = fn_800DC288(csv.fn_8005B9CC());
                    loc.mRotY = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
                    loc.mSize.x = fn_800DC288(csv.fn_8005B9CC());
                    loc.mSize.y = fn_800DC288(csv.fn_8005B9CC());
                    loc.mSize.z = fn_800DC288(csv.fn_8005B9CC());
                    drop.mLocs.push_back(loc);
                    break;
                }
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

int RandomItem::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    return m54 = msg->mType;
}

void RandomItem::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

// inferred: picks the entry whose places contain the player and switches the item on or off
void RandomItem::fn_8043465C() {
    if (lbl_8087F8A0 == 0 || lbl_8087F8A0->mHead == 0) {
        return;
    }
    CharaTask* player = (CharaTask*)lbl_8087F8A0->mHead;
    if (m54 == 0) {
        return;
    }
    if ((m11C == 2 && m168 != 0) || m11C == 3) {
        return;
    }
    RandomItemSphere_unk sphere;
    sphere.mCenter = player->m614;
    sphere.mRadius = player->m620;
    RandomItemDrop_unk* prev = m144;
    m144 = 0;
    for (unsigned int i = 0; i < mDrops.mSize; i++) {
        RandomItemDrop_unk& drop = mDrops[i];
        for (unsigned int k = 0; k < drop.mLocs.mSize; k++) {
            RandomItemLoc_unk& loc = drop.mLocs[k];
            RandomItemBox_unk box;
            MakeLocMtx_unk(box.mMtx, loc);
            box.mSize = loc.mSize;
            if (fn_80051CD8(&sphere, &box) != 0) {
                m144 = &drop;
                break;
            }
        }
        if (m144 != 0) {
            break;
        }
    }
    if (prev != m144) {
        if (m144 != 0) {
            if (m11C != 4) {
                fn_80435684(1);
            }
        } else if (m11C != 0) {
            fn_80435684(0);
        }
    }
}

// inferred: state change of the item (0 off, 1 waiting, 2 shown, 3 taken, 4 done)
void RandomItem::fn_80435684(int state) {
    switch (state) {
    case 0:
        m11C = state;
        m168 = 0;
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        break;
    case 1:
        m11C = state;
        m148 = m144->m81C;
        if (m144->m820 > 0) {
            m148 += rand() % m144->m820;
        }
        m160 = 0;
        m168 = 0;
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        break;
    case 2:
        m11C = state;
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        if (fn_80435908(this) != 0) {
            m168 = 1;
            m14C = m144->m824;
            if (lbl_8087F3C0 != 0) {
                lbl_8087F3C0->mB8 = 1;
                fn_80232B7C(this, 0);
                fn_8023A8B4(lbl_8087F3C0, &mEfp, 0, 0, m154, lbl_807C7030,
                            RandomItemColor_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
                lbl_8087F3C0->mB8 = 0;
            }
        }
        break;
    case 3:
        m11C = state;
        m150 = m144->m828 - m144->m82C * m160;
        m168 = 1;
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
            lbl_8087F3C0->mB8 = 1;
            fn_80232B7C(this, 0);
            fn_8023A8B4(lbl_8087F3C0, &mEfpItem, 0, 0, m154, lbl_807C7030,
                        RandomItemColor_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
            lbl_8087F3C0->mB8 = 0;
        }
        break;
    case 4:
        m11C = state;
        m168 = 0;
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        break;
    }
}

} // namespace gmk

// NONMATCHING: 93.7% - 0x10 bytes smaller frame and other saved registers (the temporaries of the
// ray test are laid out differently).
// inferred: places the item at a random route point of the current entry that is between half the
// range and the range from the player and can be seen from it (nonzero when one was found)
extern "C" int fn_80435908(gmk::RandomItem* item) {
    item->m154.x = 0.0f;
    item->m154.y = 0.0f;
    item->m154.z = 0.0f;
    if (item->m144 == 0 || lbl_8087F8A0 == 0 || lbl_8087F430 == 0) {
        return 0;
    }
    RandomTable_unk* table = fn_8021D720(lbl_8087F430->m10D0, item->m144->m04);
    if (table == 0) {
        return 0;
    }
    int id;
    if (!fn_8021D770(table, &id, 0, 0)) {
        return 0;
    }
    if (id <= 0) {
        return 0;
    }
    CharaTask* player = (CharaTask*)lbl_8087F8A0->mHead;
    atn::FixedArray_unk<RoutePoint_unk*, 0x200> points;
    for (unsigned int i = 0; i < item->m144->mPoints.mSize; i++) {
        RoutePoint_unk* point = item->m144->mPoints[i];
        atn::Vector3 d = point->mPos - player->mPos;
        float distSq = fn_805F9920(&d);
        float range = item->m144->m818;
        if (!((0.5f * range) * (0.5f * range) < distSq) || !(distSq < range * range)) {
            continue;
        }
        atn::Vector3 from = player->mPos + atn::Vector3(0.0f, 3.0f, 0.0f);
        float r = 0.5f * point->mRadius;
        atn::Vector3 start = from;
        atn::Vector3 to = point->mPos;
        float len = (to - start).Length_unk();
        if (!(atn::Abs_unk(len) < 1.1920929e-7f)) {
            to -= start;
            to.Normalize_unk();
            float k = len - r;
            to.x = to.x * k + start.x;
            to.y = to.y * k + start.y;
            to.z = to.z * k + start.z;
        }
        if (fn_8004ECC0(lbl_8087EE98, 0, &start, &to, 0x80000010, 0, 0) == 0) {
            points.push_back(point);
        }
    }
    if (points.mSize != 0) {
        item->m154 = points[rand() % points.mSize]->mPos;
        return 1;
    }
    return 0;
}

// inferred: the item is waiting to be taken (called by GameScene with the gimmick found by
// GimmickManager::fn_803EEE10; name unknown)
extern "C" int fn_80435CBC(gmk::RandomItem* item) {
    if (item->m11C == 2 && item->m168 != 0) {
        return 1;
    }
    return 0;
}
