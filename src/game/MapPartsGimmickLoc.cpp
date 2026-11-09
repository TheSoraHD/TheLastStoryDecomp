#include "game/MapPartsGimmickLoc_unk.h"
#include "game/GimmickCommon.h"
#include "game/MapPartsArea.h"
#include "game/MapArea.h"
#include "atn/RenderManager.h"
#include "atn/EffectManager.h"
#include "game/MapManager.h"
#include "game/AreaEffConfig.h"

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
    : MapPartsBase(parent, type), m50(0), m54(0), m60(0), m64(0), m68(0), m6C(0), mName(""), m120(1), m121(0) {
    m124[0] = 0.0f;
    m124[1] = 0.0f;
    m124[2] = 0.0f;
    m124[3] = 0.0f;
    mFile.vf0C(csv.fn_8005B9CC());
    vf28(&csv);
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
    for (unsigned int i = 0; i < m60; i++) {
        atn::Sphere sphere(m64[i].m2C, 5.0f);
        MapArea* area = MapManager::instance_->FindArea(sphere);
        if (area != 0 && area->mEffConfig != 0) {
            const atn::D2A_DrawColor* color = area->mEffConfig->fn_80043014(0, m64[i].mName);
            m64[i].mColor = *color;
            if (m54 == 2) {
                lbl_8087F3C0->fn_8023A184((unsigned int)&m64[i], 0, (const EffectColor_unk*)&m64[i].mColor);
            }
        }
    }
}

// inferred: picks once a random variant of each record
void MapPartsGimmickLoc::fn_80499B9C() {
    if (m54 == 0) {
        m54 = 1;
        int n;
        for (unsigned int i = 0; i < m60; i++) {
            n = m64[i].m90;
            m64[i].m98 = n > 0 ? rand() % n : 0;
        }
    }
}

// inferred: stops the effects of the records
void MapPartsGimmickLoc::fn_80499C48() {
    m54 = 0;
    atn::EffectManager* effects = lbl_8087F3C0;
    for (unsigned int i = 0; i < m60; i++) {
        fn_80239DAC(effects, (unsigned int)&m64[i], 0, 0);
    }
}

// inferred: gives the scene to the gimmicks (children) and to the objects of m6C
void MapPartsGimmickLoc::fn_80499CCC(atn::Scene* scene) {
    for (atn::Task* child = m18; child != 0; child = child->m1C) {
        static_cast<GimmickCommon*>(child)->vf3C(scene);
    }
    for (unsigned int i = 0; i < m68; i++) {
        m6C[i]->m64 = scene;
    }
}

// inferred: the scene of each object of m6C is the one of the area it is in (default scene otherwise)
void MapPartsGimmickLoc::fn_80499D60(MapPartsArea* areas) {
    for (unsigned int i = 0; i < m68; i++) {
        atn::Sphere sphere = m6C[i]->mSphere;
        MapArea* area = areas->fn_804962B0(&sphere);
        if (area != 0) {
            m6C[i]->m64 = &area->GetScene_unk();
        } else {
            m6C[i]->m64 = atn::RenderManager::instance_->fn_800C16B4();
        }
    }
}
