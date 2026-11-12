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

std::string fn_8006B0C8(const std::string& path); // inferred: directory of a path

// inferred: header of the locator file of the gimmicks (only the texture name is read here)
struct GimmickLocFile_unk {
    unsigned char m00[0x20];
    const char* mTexture; // 0x20: inferred: texture of the part (in "<dir>/textures/")
};

// inferred: empty destructor of the name (inlined in ~MapPartsGimmickLoc, 0x804970D4; weak copy 0x80041C0C)
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
    m124[0] = 0.0f;
    m124[1] = 0.0f;
    m124[2] = 0.0f;
    m124[3] = 0.0f;
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
                m124[0] = fn_800DC288(csv->fn_8005B9CC());
                m124[1] = fn_800DC288(csv->fn_8005B9CC());
                m124[2] = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp(key, "HIDE_BIRDVIEW") == 0) {
                m121 = 1;
                m124[3] = 1e16f;
            } else if (strcmp(key, "HIDE_BIRDVIEW_Y") == 0) {
                m121 = 1;
                m124[3] = fn_800DC288(csv->fn_8005B9CC());
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
        if (self->mName == "" && strcmp(file->mTexture, "") != 0) {
            const char* path = self->mFile.fn_80473F18();
            std::string dir(fn_8006B0C8(std::string(path)));
            dir += "/textures/";
            dir += file->mTexture;
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

// inferred: draws the objects of m6C while shown
void MapPartsGimmickLoc::vf14() {
    if (m120 != 0) {
        for (unsigned int i = 0; i < mScenes.size(); i++) {
            mScenes.mData[i]->fn_804006E0();
        }
    }
}

void* MapPartsGimmickLoc::fn_80498E7C(int index) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(this);
    if (*reinterpret_cast<unsigned int*>(bytes) == 0x6C6F6373) {
        unsigned char* entries = *reinterpret_cast<unsigned char**>(bytes + 0x18);
        return entries + index * 0x3C;
    }
    unsigned char* entries = *reinterpret_cast<unsigned char**>(bytes + 0x18);
    return entries + index * 0x30;
}

void MapPartsGimmickLocObj_unk::fn_80498EAC(const atn::Vector3& value) {
    m6C = value;
}

void MapPartsGimmickLocObj_unk::fn_80498EC0(const atn::Vector3& value) {
    m78 = value;
}

void MapPartsGimmickLocObj_unk::fn_80498ED4(const atn::Vector3& value) {
    m90 = value;
}

void MapPartsGimmickLocObj_unk::fn_80498EE8(int value) {
    m58 = value;
}

void MapPartsGimmickLocObj_unk::fn_80498EF0(int value) {
    mA4 = value;
}

extern "C" void PSMTXScale(void* matrix, float x, float y, float z);

void MapPartsGimmickLoc::fn_80498EF8(const atn::Vector3& scale) {
    PSMTXScale(this, scale.x, scale.y, scale.z);
}

void MapPartsGimmickLoc::fn_80498F08() {}

void MapPartsGimmickLoc::fn_80498F0C(int value) {
    *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(this) + 0x7C) = value;
}

// inferred: normalizes the two horizontal limits around the supplied value.
const float lbl_808870E8 = 1.0f;
const float lbl_808870F0 = -1.0f;

// NONMATCHING: 0% - generated body is 52 bytes; target is 56 bytes.
void MapPartsGimmickLocObj_unk::fn_80498F14(float value, const atn::Vector3& range) {
    m6C.y = value;
    m6C.z = lbl_808870F0 * range.x / value;
    m78.x = lbl_808870E8 - (range.y + value) / value;
}

void MapPartsGimmickLoc::fn_80498F4C() {}

void MapPartsGimmickLoc::fn_80499388(int value) {
    *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(this) + 0x630) = value;
}

bool MapPartsGimmickLocArray8_unk::fn_80499684() const {
    return mSize == 0;
}

unsigned int MapPartsGimmickLocArray8_unk::fn_80499694() const {
    return mSize;
}

void* MapPartsGimmickLocArray8_unk::fn_80499724(int index) const {
    return static_cast<unsigned char*>(mData) + index * 8;
}

void* MapPartsGimmickLocArray4_unk::fn_8049981C(int index) const {
    return static_cast<unsigned char*>(mData) + index * 4;
}

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
            const atn::D2A_DrawColor* color = area->mEffConfig->fn_80043014(0, mRecords.mData[i].mName);
            mRecords.mData[i].mColor = *color;
            if (m54 == 2) {
                lbl_8087F3C0->fn_8023A184((unsigned int)&mRecords.mData[i], 0, (const EffectColor_unk*)&mRecords.mData[i].mColor);
            }
        }
    }
}

// inferred: m = m * rot(axis, rad)
static inline void RotateAxis_unk(atn::Matrix3x4& m, char axis, float rad) {
    Mtx t;
    Mtx r;
    PSMTXRotRad(r, axis, rad);
    PSMTXConcat(m.m, r, t);
    __MtxCopy(m.m, t);
}

// NONMATCHING: 98.32% - stack slots and registers of the three inlined rotations (z, y, x)
// inferred: moves and turns the records by the placement of the part
void MapPartsGimmickLoc::fn_8049994C(const atn::Vector3& pos, const atn::Vector3& rot) {
    for (unsigned int i = 0; i < mRecords.size(); i++) {
        mRecords.mData[i].m2C += pos;
        atn::Matrix3x4 m;
        atn::Matrix3x4* p = &m;
        p->SetIdentity();
        if (0.0f != rot.z) {
            RotateAxis_unk(*p, 'z', rot.z);
        }
        if (0.0f != rot.y) {
            RotateAxis_unk(*p, 'y', rot.y);
        }
        if (0.0f != rot.x) {
            RotateAxis_unk(*p, 'x', rot.x);
        }
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

// inferred: hidden below the height m124[3] (as MapPartsLocatorSet::vf20)
bool MapPartsGimmickLoc::vf20(const atn::Vector3& pos) {
    bool hidden = false;
    if (m121 && m124[3] > 10.0f + pos.y) {
        hidden = true;
    }
    return hidden;
}
