#include "game/MapPartsGimmickLoc_unk.h"
#include "gmk/SwingPart_unk.h"
#include "game/GimmickCommon.h"
#include "game/MapPartsArea.h"
#include "game/MapArea.h"
#include "atn/RenderManager.h"
#include "atn/EffectManager.h"
#include "game/MapManager.h"
#include "game/AreaEffConfig.h"
#include "game/GimmickManager.h"
#include "game/TownTask.h"
#include "atn/Loader_unk.h"
#include "std/string.h"
#include "std/string_members.h"
#include "std/vector.h"
#include "game/MapInstance_unk.h"
#include "game/MapTask.h"
#include "atn/Csv.h"

std::string fn_8006B0C8(const std::string& path); // inferred: directory of a path

// inferred: chunk of locators of the file ('locs': 0x3C bytes per locator, with a shear; 'loct':
// 0x30 bytes; same layout as BinLocs_unk of atn/LH_Binary.cpp and BuildingFile_unk of
// game/MapPartsLocatorSet.cpp); names invented
struct GimmickLocTable_unk {
    // inferred: weak copy 0x80498E7C
    MapLocator_unk* GetLocator_unk(int i) {
        if (mKind == 'locs') {
            return (MapLocator_unk*)(mLocators + i * 0x3C);
        }
        return (MapLocator_unk*)(mLocators + i * 0x30);
    }

    unsigned int mKind;
    unsigned char m04[0x10 - 0x04];
    const char* mTexture; // 0x10: inferred: texture of the part (in "<dir>/textures/")
    int mCount;           // 0x14
    char* mLocators;      // 0x18
};

// inferred: locator file of the gimmicks (only the chunk of locators is read here)
struct GimmickLocFile_unk {
    unsigned char m00[0x10];
    GimmickLocTable_unk mTable; // 0x10
};

// inferred: constructor of the names (inlined in the constructor of the records, 0x804997E4) and empty
// destructor (inlined in ~MapPartsGimmickLoc, 0x804970D4; weak copy 0x80041C0C), as in game/AreaEffConfig.cpp
template <int N, class C, class Tr>
inline atn::static_string<N, C, Tr>::static_string() {
    mStr[0] = '\0';
}
template <int N, class C, class Tr>
inline atn::static_string<N, C, Tr>::~static_string() {}

extern "C" int rand();
// inferred: random number in [0, n) (0 when n is not positive)
static inline int Random_unk(int n) {
    return n > 0 ? rand() % n : 0;
}

MapPartsGimmickLoc* MapPartsGimmickLoc::SetTask(atn::Task* parent, int type, atn::CsvTab& csv) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) MapPartsGimmickLoc(parent, type, csv);
    }
    return 0;
}

// inferred: the locator file is named in the csv; the rest of the csv goes to vf28
MapPartsGimmickLoc::MapPartsGimmickLoc(atn::Task* parent, int type, atn::CsvTab& csv)
    : MapPartsBase(parent, type), m50(0), m54(0), mName(""), m120(1), m121(0) {
    m124.x = 0.0f;
    m124.y = 0.0f;
    m124.z = 0.0f;
    m130 = 0.0f;
    mFile.vf0C(csv.fn_8005B9CC());
    vf28(&csv);
}

// inferred: deletes the swinging parts; the members free the records, the parts array and the file
MapPartsGimmickLoc::~MapPartsGimmickLoc() {
    for (unsigned int i = 0; i < mScenes.size(); i++) {
        delete mScenes[i];
    }
}

extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: options of the part from the csv: POSITION x y z (a colour), HIDE_BIRDVIEW (hidden from the bird
// view) and HIDE_BIRDVIEW_Y y (hidden below y)
void MapPartsGimmickLoc::vf28(atn::CsvTab* csv) {
    const char* key = csv->fn_8005B9CC();
    do {
        if (strcmp(key, "") != 0) {
            if (strcmp(key, "POSITION") == 0) {
                m124.x = fn_800DC288(csv->fn_8005B9CC());
                m124.y = fn_800DC288(csv->fn_8005B9CC());
                m124.z = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp(key, "HIDE_BIRDVIEW") == 0) {
                m121 = 1;
                m130 = 1e16f;
            } else if (strcmp(key, "HIDE_BIRDVIEW_Y") == 0) {
                m121 = 1;
                m130 = fn_800DC288(csv->fn_8005B9CC());
            } else {
                break;
            }
        }
        key = csv->fn_8005B9CC();
    } while (key != 0 && strcmp("", key) != 0);
}

// inferred: reads the locator file and starts the load of its texture (inlined in vf0C: its locals are
// on the stack in the order of an inline; as MapPartsLocatorSet::fn_8049B1F0)
static inline void LoadFile_unk(MapPartsGimmickLoc* self) {
    Loader_unk loader;
    fn_804714A4(&loader);
    fn_804714B0(&loader, self->mFile.fn_80470580(), (void*)self->mFile.fn_8047059C(), LoaderCallback_unk(), 0);
    if (self->mFile.fn_80473EFC() == 3) {
        GimmickLocFile_unk* file = (GimmickLocFile_unk*)self->mFile.fn_80470580();
        if (self->mName == "" && strcmp(file->mTable.mTexture, "") != 0) {
            const char* path = self->mFile.fn_80473F18();
            std::string dir(fn_8006B0C8(std::string(path)));
            dir += "/textures/";
            dir += file->mTable.mTexture;
            self->mName = dir.c_str();
        }
        if (!(self->mName == "")) {
            self->mTexture.fn_800D5908(self->mName.mStr);
        }
    }
}

// NONMATCHING: 99.84% - r27/r28/r29 of the inlined std::string constructor (path, temporary, length)
// inferred: loading: the locator file and its texture (0), the gimmicks (1, fn_804985E4), the
// effects and swinging parts (3), then ready (4)
int MapPartsGimmickLoc::vf0C() {
    switch (m50) {
    case 0: {
        if (mFile.fn_80473F50() != 0) {
            return 0;
        }
        LoadFile_unk(this);
        m50 = 1;
    }
    case 1:
        if (((GimmickUvOwner_unk*)&mTexture)->fn_800D59B8() != 0) {
            return 0;
        }
        fn_804985E4();
        m50 = 3;
    case 3:
        for (unsigned int i = 0; i < mRecords.size(); i++) {
            if (mRecords.mData[i].mEff.IsLoading()) {
                return 0;
            }
        }
        for (unsigned int i = 0; i < mScenes.size(); i++) {
            if (mScenes.mData[i]->fn_803FFF40() == 0) {
                return 0;
            }
        }
        if (fn_800D3FA4() == 0) {
            return 0;
        }
        m50 = 4;
    }
    if (m50 == 4) {
        if (m54 == 0) {
            m54 = 1;
            for (unsigned int i = 0; i < mRecords.size(); i++) {
                int n = mRecords.mData[i].m90;
                mRecords.mData[i].m98 = n > 0 ? rand() % n : 0;
            }
        }
        if (lbl_8087FA20 != 0 && (lbl_8087FA20->mFlags & 2) == 2) {
            GimmickManager::instance_->fn_803EE374(this);
        }
        return 1;
    }
    return 0;
}

extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);

// inferred: rotations z, y, x of a matrix in place (as RotateEuler_unk of game/MapInstance_unk.cpp)
inline void RotateEulerZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    if (0.0f != rot.z) {
        atn::Matrix3x4::RotateAxis_unk(m, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        atn::Matrix3x4::RotateAxis_unk(m, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        atn::Matrix3x4::RotateAxis_unk(m, 'x', rot.x);
    }
}

// inferred: m = m * rot_z * rot_y * rot_x (as Matrix3x4::RotateZYX_unk, with the rotations in a helper:
// the stack slots of the three rotations come out in the order of the original)
inline void RotateRecord_unk(atn::Matrix3x4& self, const atn::Vector3& rot) {
    atn::Matrix3x4 t;
    atn::Matrix3x4 m;
    atn::Matrix3x4* p = &m;
    p->SetIdentity();
    RotateEulerZYX_unk(*p, rot);
    PSMTXConcat(self.m, p->m, t.m);
    self = t;
}

// inferred: matrix of a record: translation, then rotations z, y, x (weak copy 0x80498354; inlined
// three times in vf10)
inline void UpdateRecordMatrix_unk(MapPartsGimmickLocRecord_unk& rec) {
    atn::Matrix3x4 t;
    PSMTXTrans(t.m, rec.m2C.x, rec.m2C.y, rec.m2C.z);
    rec.mMtx = t;
    RotateRecord_unk(rec.mMtx, rec.m38);
}

// inferred: random value in [min, max] in steps of 1/1000
static inline float RandRange_unk(float min, float max) {
    return (max - min) * (rand() % 1001 / 1000.0f) + min;
}

extern "C" void fn_8023A108(atn::EffectManager* mgr, unsigned int owner, int group, int count);

// NONMATCHING: 99.64% - order of the hoisted stack addresses of mode 2 (the matrix of the last
// UpdateRecordMatrix_unk is set up first) and of the record address of the count test
// inferred: effects of the records: mode 0 records are started once (with the effect colour and a random
// scale), mode 1 records are restarted every m8C (+- m90) frames; then the swinging parts are updated
void MapPartsGimmickLoc::vf10() {
    if (m120 == 0) {
        return;
    }
    atn::EffectManager* effects = lbl_8087F3C0;
    if (m54 == 1) {
        for (unsigned int i = 0; i < mRecords.size(); i++) {
            if (mRecords[i].m88 != 0) {
                continue;
            }
            UpdateRecordMatrix_unk(mRecords[i]);
            effects->mSystem.mLayer = 1;
            if (mRecords[i].mName == "be002_014") {
                effects->mSystem.m78 = 1;
            }
            fn_80232B7C((unsigned int)&mRecords[i], 0);
            MapPartsGimmickLocRecord_unk* rec = &mRecords[i];
            float min;
            float max = rec->m94;
            min = -max;
            fn_8023A680(effects, (EffectRes_unk*)&mRecords[i], -1, 5, (int)&mRecords[i].mMtx, 0, 0,
                        (const EffectColor_unk*)&rec->mColor, 0, -1, 1,
                        mRecords[i].m44 + RandRange_unk(min, max));
            if ((mRecords[i].m9C & 1) == 1) {
                fn_8023A02C(effects, (unsigned int)&mRecords[i], 0, 8);
            }
            fn_8023A02C(effects, (unsigned int)&mRecords[i], 0, 0x40);
            int count = mRecords[i].m8C;
            if (count > 0 || mRecords[i].m90 > 0) {
                if (mRecords[i].m90 > 0) {
                    count += rand() % (mRecords[i].m90 * 2) - mRecords[i].m90;
                }
                fn_8023A108(effects, (unsigned int)&mRecords[i], 0, count);
            }
            effects->mSystem.mLayer = 0;
            if (mRecords[i].mName == "be002_014") {
                effects->mSystem.m78 = 0;
            }
        }
        m54 = 2;
    } else if (m54 == 2) {
        for (unsigned int i = 0; i < mRecords.size(); i++) {
            if (mRecords[i].m88 == 1) {
                if (mRecords[i].m98 <= 0) {
                    UpdateRecordMatrix_unk(mRecords[i]);
                    effects->mSystem.mLayer = 1;
                    fn_80232B7C((unsigned int)&mRecords[i], 0);
                    MapPartsGimmickLocRecord_unk* rec = &mRecords[i];
                    float min;
                    float max = rec->m94;
                    min = -max;
                    fn_8023A680(effects, (EffectRes_unk*)&mRecords[i], -1, 5, (int)&mRecords[i].mMtx, 0, 0,
                                (const EffectColor_unk*)&rec->mColor, 0, -1, 1,
                                mRecords[i].m44 + RandRange_unk(min, max));
                    effects->mSystem.mLayer = 0;
                    if ((mRecords[i].m9C & 1) == 1) {
                        fn_8023A02C(effects, (unsigned int)&mRecords[i], 0, 8);
                    }
                    fn_8023A02C(effects, (unsigned int)&mRecords[i], 0, 0x40);
                    mRecords[i].m98 = mRecords[i].m8C;
                    if (mRecords[i].m90 > 0) {
                        mRecords[i].m98 += rand() % (mRecords[i].m90 * 2) - mRecords[i].m90;
                    }
                } else {
                    mRecords[i].m98--;
                }
            }
            UpdateRecordMatrix_unk(mRecords[i]);
        }
    }
    for (unsigned int i = 0; i < mScenes.size(); i++) {
        mScenes[i]->fn_804003E0();
    }
}

// inferred: draws the objects of m6C while shown
void MapPartsGimmickLoc::vf14() {
    if (m120 != 0) {
        for (unsigned int i = 0; i < mScenes.size(); i++) {
            mScenes.mData[i]->fn_804006E0();
        }
    }
}

// inferred: locators of swinging parts: the locator and the swing type (std::pair in the original)
// (a POD: the insertion copies it without the placement-new test)
struct GimmickLocSwing_unk {
    MapLocator_unk* first;
    int second;
};
// inferred: std::make_pair (weak copy 0x80498F50)
inline GimmickLocSwing_unk MakeSwing_unk(MapLocator_unk* loc, int type) {
    GimmickLocSwing_unk swing = {loc, type};
    return swing;
}

// inferred: setters of the gimmick and of the model instance; inline members in the original (weak copies
// 0x80498EAC-0x80498F4C), written here as free inlines (GimmickCommon.h is included by ~130 units)
inline void SetGimmickPos_unk(GimmickCommon* g, const atn::Vector3& v) {
    g->m6C = v;
}
inline void SetGimmickRot_unk(GimmickCommon* g, const atn::Vector3& v) {
    g->m78 = v;
}
inline void SetGimmickScale_unk(GimmickCommon* g, const atn::Vector3& v) {
    g->m90 = v;
}
inline void SetGimmickM58_unk(GimmickCommon* g, int v) {
    g->m58 = v;
}
inline void SetGimmickA4_unk(GimmickCommon* g, int v) {
    g->mA4 = v;
}
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
inline atn::Matrix3x4 ScaleMatrix_unk(const atn::Vector3& s) {
    atn::Matrix3x4 m;
    PSMTXScale(m.m, s.x, s.y, s.z);
    return m;
}
inline atn::ModelInstance* GetInst_unk(SwingPart_unk* part) {
    return &part->mInst;
}
inline void SetLightMap_unk(SwingPart_unk* part, atn::Texture_unk* tex) {
    part->mInst.m7C = tex;
}
// NONMATCHING: 88.57% (weak copy 0x80498F14) - the original rounds the distance (frsp) before the
// divisions; tried reading back the member and a double parameter
// inferred: LOD distances of a model instance (as in game/MapInstance_unk.cpp)
inline void SetLod_unk(atn::ModelInstance* inst, float dist, const float* range) {
    inst->m70 = dist;
    inst->m74 = -1.0f * range[0] / dist;
    inst->m78 = 1.0f - (range[1] + dist) / dist;
}
inline const char* CStr_unk(const atn::static_string<32, char, std::char_traits<char> >& s) {
    return s.mStr;
}
inline void SetCsvM630_unk(atn::Csv& csv, int v) {
    csv.m630 = v;
}

extern "C" GimmickCommon* fn_803EC16C(atn::Task* parent, int id, int index); // inferred: creates a gimmick
extern "C" MapTask* fn_803830A0();                                           // inferred: current map
extern "C" unsigned int fn_800E0AA8(const std::vector<MapLocator_unk*>* v);  // inferred: size (weak copy)
extern "C" atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot);              // inferred: rotation matrix
extern "C" atn::Matrix3x4 fn_800F80A8(const atn::Vector3& pos);              // inferred: translation matrix
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);

// inferred: creates what the locator file places: gimmicks ("gm<type>_<id>_<index>"; in the town map 10
// some of them are swinging parts), swinging parts and effects ("be")
void MapPartsGimmickLoc::fn_804985E4() {
    if (mFile.fn_80473EFC() != 3) {
        return;
    }
    GimmickLocTable_unk* table = &((GimmickLocFile_unk*)mFile.fn_80470580())->mTable;
    std::string dir(fn_8006B0C8(std::string(mFile.fn_80473F18())));
    std::vector<MapLocator_unk*> effects;
    std::vector<GimmickLocSwing_unk> swings;
    for (int i = 0; i < table->mCount; i++) {
        MapLocator_unk* loc = table->GetLocator_unk(i);
        const char* name = loc->mName;
        if (strncmp(name, "gm", 2) == 0) {
            int id = atoi(name + 2) * 1000 + atoi(loc->mName + 6);
            int index = atoi(loc->mName + 10);
            bool swing = false;
            int type = 0;
            if (fn_803830A0() != 0 && id / 1000 == 10) {
                switch (fn_803830A0()->GetMapId()) {
                case 1:
                case 3:
                case 6:
                    switch (id) {
                    case 10017:
                    case 10018:
                    case 10033:
                    case 10034:
                    case 10036:
                    case 10045:
                    case 10046:
                    case 10047:
                        type = 1;
                        swing = true;
                        break;
                    case 10013:
                    case 10019:
                    case 10020:
                    case 10021:
                        type = 0;
                        swing = true;
                        break;
                    }
                    break;
                case 2:
                case 4:
                case 5:
                    switch (id) {
                    case 10017:
                    case 10018:
                    case 10033:
                    case 10034:
                    case 10036:
                    case 10045:
                    case 10046:
                    case 10047:
                        type = 3;
                        swing = true;
                        break;
                    case 10013:
                    case 10019:
                    case 10020:
                    case 10021:
                        type = 2;
                        swing = true;
                        break;
                    }
                    break;
                }
            }
            if (swing) {
                swings.push_back(MakeSwing_unk(loc, type));
            } else {
                atn::Vector3 rot(atn::DegToRad_unk(loc->mRot.x), atn::DegToRad_unk(loc->mRot.y),
                                 atn::DegToRad_unk(loc->mRot.z));
                const char* text = loc->mLod->mName;
            atn::Csv csv(text, strlen(text));
                SetCsvM630_unk(csv, 1);
                csv.fn_8005B9CC();
                int m58 = atoi(csv.fn_8005B9CC());
                int a4 = atoi(csv.fn_8005B9CC());
                GimmickCommon* g = fn_803EC16C(this, id, index);
                if (g != 0) {
                    SetGimmickPos_unk(g, loc->mPos + m124);
                    SetGimmickRot_unk(g, rot);
                    SetGimmickScale_unk(g, loc->mScale);
                    SetGimmickM58_unk(g, m58);
                    g->fn_803EDB0C((GimmickUvOwner_unk*)&mTexture, (GimmickUvTable_unk*)loc);
                    SetGimmickA4_unk(g, a4);
                }
            }
        } else if (strncmp(name, "be", 2) == 0) {
            effects.push_back(loc);
        }
    }
    if (!swings.empty()) {
        mScenes.Resize_unk(swings.size(), 6, "SwingObjectInstances");
        for (unsigned int i = 0; i < swings.size(); i++) {
            MapLocator_unk* loc = swings[i].first;
            int id = atoi(loc->mName + 2) * 1000 + atoi(loc->mName + 6);
            int index = atoi(loc->mName + 10);
            atn::Vector3 rot(atn::DegToRad_unk(loc->mRot.x), atn::DegToRad_unk(loc->mRot.y),
                             atn::DegToRad_unk(loc->mRot.z));
            const char* text = loc->mLod->mName;
            atn::Csv csv(text, strlen(text));
            SetCsvM630_unk(csv, 1);
            csv.fn_8005B9CC();
            if ((atoi(csv.fn_8005B9CC()) & 2) == 2) {
                swings[i].second |= 2;
            }
            SwingPart_unk* part = new ((MemoryTag)6, "", "", 0)
                SwingPart_unk(id, (void*)index, (SwingPartPos_unk*)loc, swings[i].second);
            mScenes[i] = part;
            atn::Matrix3x4 m = ScaleMatrix_unk(loc->mScale);
            m.fn_801495E8(fn_800109E0(rot));
            m.fn_801495E8(fn_800F80A8(loc->mPos + m124));
            GetInst_unk(mScenes[i])->fn_8009EE30(m);
            SetLightMap_unk(mScenes[i], &mTexture);
            MapLod_unk* lod = loc->mLod;
            SetLod_unk(GetInst_unk(mScenes[i]), lod->m0C, &lod->m04);
        }
    }
    mRecords.Resize_unk(fn_800E0AA8(&effects), 7, "BeInfoList");
    std::string base("/map/be/");
    for (unsigned int i = 0; i < fn_800E0AA8(&effects); i++) {
        MapLocator_unk* loc = effects[i];
        const char* text = loc->mLod->mName;
        atn::Csv csv(text, strlen(text));
        SetCsvM630_unk(csv, 1);
        atn::Vector3 rot(atn::DegToRad_unk(loc->mRot.x), atn::DegToRad_unk(loc->mRot.y),
                         atn::DegToRad_unk(loc->mRot.z));
        const char* name = csv.fn_8005B9CC();
        mRecords[i].mName = name;
        mRecords[i].mEff.LoadRequest((base + CStr_unk(mRecords[i].mName) + ".eff").c_str());
        mRecords[i].m2C = loc->mPos + m124;
        mRecords[i].m38 = rot;
        mRecords[i].m44 = loc->mScale.x;
        UpdateRecordMatrix_unk(mRecords[i]);
        mRecords[i].mColor.Set_unk(1.0f, 1.0f, 1.0f, 1.0f);
        mRecords[i].m88 = atoi(csv.fn_8005B9CC());
        mRecords[i].m8C = atoi(csv.fn_8005B9CC());
        mRecords[i].m90 = atoi(csv.fn_8005B9CC());
        mRecords[i].m94 = fn_800DC288(csv.fn_8005B9CC());
        mRecords[i].m9C = atoi(csv.fn_8005B9CC());
        mRecords[i].m98 = mRecords[i].m90 > 0 ? rand() % mRecords[i].m90 : 0;
    }
}

// inferred: the insertions of the two vectors are emitted here, before the next function (their
// "vector length error" comes before "<all>" in the pool), as in game/AreaEffConfig.cpp
template void std::vector<GimmickLocSwing_unk>::push_back(const GimmickLocSwing_unk& x);
template void std::vector<MapLocator_unk*>::push_back(MapLocator_unk* const& x);

// inferred: colour of the effect of each record from the AreaEffConfig of its area (given to the
// EffectManager in mode 2)
void MapPartsGimmickLoc::SetAreaEffConfig() {
    if (MapManager::instance_ == 0) {
        return;
    }
    for (unsigned int i = 0; i < mRecords.size(); i++) {
        atn::Sphere sphere(mRecords.mData[i].m2C, 5.0f);
        MapArea* area = MapManager::instance_->FindArea(sphere);
        if (area != 0 && area->mEffConfig != 0) {
            const atn::D2A_DrawColor* color = area->mEffConfig->fn_80043014(0, mRecords.mData[i].mName.mStr);
            mRecords.mData[i].mColor = *color;
            if (m54 == 2) {
                lbl_8087F3C0->fn_8023A184((unsigned int)&mRecords.mData[i], 0, (const EffectColor_unk*)&mRecords.mData[i].mColor);
            }
        }
    }
}

// inferred: moves and turns the records by the placement of the part
void MapPartsGimmickLoc::fn_8049994C(const atn::Vector3& pos, const atn::Vector3& rot) {
    for (unsigned int i = 0; i < mRecords.size(); i++) {
        mRecords.mData[i].m2C += pos;
        atn::Matrix3x4 m;
        m.SetIdentity();
        RotateEulerZYX_unk(m, rot);
        PSMTXMultVec(m.m, &mRecords.mData[i].m2C, &mRecords.mData[i].m2C);
        mRecords.mData[i].m38 += rot;
    }
}

// inferred: picks once a random variant of each record
void MapPartsGimmickLoc::fn_80499B9C() {
    if (m54 == 0) {
        m54 = 1;
        int n;
        for (unsigned int i = 0; i < mRecords.size(); i++) {
            n = mRecords.mData[i].m90;
            mRecords.mData[i].m98 = n > 0 ? rand() % n : 0;
        }
    }
}

// inferred: stops the effects of the records
void MapPartsGimmickLoc::fn_80499C48() {
    m54 = 0;
    atn::EffectManager* effects = lbl_8087F3C0;
    for (unsigned int i = 0; i < mRecords.size(); i++) {
        fn_80239DAC(effects, (unsigned int)&mRecords.mData[i], 0, 0);
    }
}

// inferred: gives the scene to the gimmicks (children) and to the objects of m6C
void MapPartsGimmickLoc::fn_80499CCC(atn::Scene* scene) {
    for (atn::Task* child = m18; child != 0; child = child->m1C) {
        static_cast<GimmickCommon*>(child)->vf3C(scene);
    }
    for (unsigned int i = 0; i < mScenes.size(); i++) {
        mScenes.mData[i]->mInst.m64 = scene;
    }
}

// inferred: the scene of each object of m6C is the one of the area it is in (default scene otherwise)
void MapPartsGimmickLoc::fn_80499D60(MapPartsArea* areas) {
    for (unsigned int i = 0; i < mScenes.size(); i++) {
        atn::Sphere sphere = mScenes.mData[i]->mInst.mBound;
        MapArea* area = areas->fn_804962B0(&sphere);
        if (area != 0) {
            mScenes.mData[i]->mInst.m64 = &area->GetScene_unk();
        } else {
            mScenes.mData[i]->mInst.m64 = atn::RenderManager::instance_->fn_800C16B4();
        }
    }
}

extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" unsigned int fn_800DC6B4(const char* s);             // inferred: string hash

// inferred: shows or hides the part when `hash` is the hash of its file name (or of "<all>"): picks the
// variants and wakes the gimmicks, or stops the effects and the gimmicks
void MapPartsGimmickLoc::vf2C(unsigned int hash, int draw) {
    std::string name = fn_8006B174(std::string(mFile.fn_80473F18()));
    if (hash == fn_800DC6B4(name.c_str()) || hash == fn_800DC6B4("<all>")) {
        if ((m120 = draw != 0)) {
            if (m54 == 0) {
                m54 = 1;
                for (unsigned int i = 0; i < mRecords.size(); i++) {
                    int n = mRecords.mData[i].m90;
                    mRecords.mData[i].m98 = n > 0 ? rand() % n : 0;
                }
            }
            fn_800D4DB8();
        } else {
            m54 = 0;
            unsigned int i = 0;
            atn::EffectManager* effects = lbl_8087F3C0;
            for (; i < mRecords.size(); i++) {
                fn_80239DAC(effects, (unsigned int)&mRecords.mData[i], 0, 0);
            }
            fn_800D4558();
        }
    }
}

// inferred: hidden below the height m124[3] (as MapPartsLocatorSet::vf20)
bool MapPartsGimmickLoc::vf20(const atn::Vector3& pos) {
    bool hidden = false;
    if (m121 && m130 > 10.0f + pos.y) {
        hidden = true;
    }
    return hidden;
}
