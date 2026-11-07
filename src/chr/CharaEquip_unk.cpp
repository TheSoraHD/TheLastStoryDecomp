// chr/CharaEquip_unk.cpp: equipment of a character (model of the item, its option parts and the
// effect it plays). File name invented from the class (chr::CharaEquip_unk, CharaTask::m680[]).
#include "chr/CharaEquip_unk.h"
#include "game/GameTask.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "game/Inventory.h"
#include "db/EquipParam_unk.h"
#include "db/OptionParts_unk.h"
#include "atn/EffectManager.h"
#include "atn/ModelCamera_unk.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"
#include "Event/Manager.h"
#include "atn/RenderManager.h"
#include "atn/Scene.h"
#include "atn/EfpHandle_unk.h"
#include "game/GameField_unk.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int rand();
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float

extern "C" GameConfig_unk* lbl_8087F0A8;
struct Arena_unk;
extern "C" Arena_unk* lbl_8087F610; // inferred: arena session
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: battle task (lbl_8087F048, __vt__10BattleTask; no header yet): effect of the colour mark of
// each party member at 0x3663C
struct BulletMgr_unk {
    unsigned char m00000[0x3663C];
    atn::EfpFile mMarkEfp[7];
};
extern "C" BulletMgr_unk* lbl_8087F048;
// inferred: equipment of an arena character (local view as in game/CharaTask.cpp, names invented)
struct ArenaEquip_unk {
    int mId;
    unsigned short mFlags;
    unsigned char m06[0x08 - 0x06];
    unsigned int mOptions; // inferred: option bits
    int mPalettes[8];      // 0x0C: inferred: palette of each colour slot
};
extern "C" ArenaEquip_unk* fn_804EB650(Arena_unk* arena, ::CharaTask* chara, int slot);
// inferred: db/OptionParts_unk.cpp
struct OptionColorOut_unk;
extern "C" void fn_8020C4C8(int* palettes, OptionParts_unk* parts, OptionColorOut_unk* out);
extern "C" void fn_8020C5D4(OptionParts_unk* parts, const char* data, int size);
extern "C" void fn_8020CC64(atn::Model* model, OptionParts_unk* parts, OptionColorOut_unk* out);
extern "C" void fn_8020DE0C(OptionParts_unk* parts, const char* name);
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale); // inferred: plays an effect
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" std::string fn_8006B2D8(const std::string& path); // inferred: name without extension

// inferred: model of the parts every character wears under the item (slots 0 and 1 of a costume)
static const char* const sBaseModels[4] = {"pc101_up_z.chr", "pc101_lo_z.chr", "", ""};

// inferred: same inline as game/CharaTask.cpp (its literals are @STRING@ objects in .sdata)
inline void CreateCamera_unk(atn::Model* model) {
    model->m208.reset(new ((MemoryTag)6, "", "", 0) ModelCamera_unk());
}

namespace chr {

CharaEquip_unk::CharaEquip_unk(int slot, int id, int color, ::CharaTask* chara, unsigned int* options,
                               ::CharaTask* other, int noOptions, int arg7, int arg8)
    : m000(slot), m004(id), m008(color), m00C(noOptions), m010(0), m014(0), m018(arg7), m01C(0), m020(arg8),
      m024(0x810505, 0x20), m3F4(0), m400(1.0f), m404(1.0f), m408(1.0f), m40C(chara), m410(other), m414(0),
      m420(3), m424(options), m434(0), m438(-1) {
    if (id == -1) {
        return;
    }
    EquipParamEntry_unk* entry = fn_8020ED84(slot, id);
    m414 = (CharaParam_unk*)entry;
    atn::FixedString_unk<0x20> name = entry->mModel;
    if (m00C != 0 && 0 <= m000 && m000 < 4) {
        name = sBaseModels[m000];
        m424 = 0;
    }
    if (fn_8020EF4C((EquipParamEntry_unk*)m414)) {
        if (slot != 0) {
            m010 = 1;
        } else {
            m014 = 1;
        }
    }
    if (m010 != 0) {
        name = "";
        m424 = 0;
    }
    if (strlen(name.c_str()) != 0) {
        char model[0x40];
        strcpy(model, name.c_str());
        fn_80219558(chara->m50);
        if (other != 0) {
            fn_80219558(other->m50);
        }
        if (chara->fn_8016E484() && m014 == 0) {
            model[4] = '1';
        }
        char path[0x100];
        sprintf(path, "/database/equipment/%s", model);
        m418.vf0C(path);
        m420 = 0;
    }
    if (lbl_8087F0A8->m18C != 0 && slot == 0) {
        int member = fn_80219558(m40C->m50);
        if (member < 7) {
            int* colors = &Inventory::instance_->mEquipShown[member].mProtectors[0].mId;
            for (int i = 0; i < 8; i++) {
                colors[i] = rand() % 64;
            }
        }
    }
}

CharaEquip_unk::~CharaEquip_unk() {
    fn_80566398(0);
}

void CharaEquip_unk::fn_80563A1C(const char* name) {
    m418.vf0C(name);
    m420 = 0;
}

void CharaEquip_unk::fn_80563A60(atn::SimpleVector<unsigned int>* list, void* arg) {
    m024.fn_80096218(list, arg);
    list->push_back(m418.fn_80473F34());
    list->push_back(m428.fn_80473F34());
}

int CharaEquip_unk::fn_8056405C() {
    switch (m420) {
    case 0:
        if (m418.fn_80473F50() != 0) {
            return 1;
        }
        fn_80564190();
        m420 = 1;
    case 1:
        if (m428.fn_80473F50() != 0) {
            return 1;
        }
        fn_80564340();
        m420 = 2;
    case 2:
        if (m024.fn_8008B140() != 0) {
            return 1;
        }
        fn_80565064();
        CreateCamera_unk(&m024);
        m420 = 3;
    }
    return 0;
}

void CharaEquip_unk::fn_80564190() {
    atn::CsvTab csv(m418.fn_80470580(), m418.fn_8047059C());
    do {
        const char* field = csv.fn_8005B9CC();
        if (*field != ';' && strcmp(field, "EDIT_DATABASE") == 0) {
            char path[0x100];
            char name[0x100];
            strcpy(name, csv.fn_8005B9CC());
            int member = fn_80219558(m40C->m50);
            if (m000 >= 0 && m000 < 4) {
                switch (member) {
                case 0:
                case 4:
                case 2:
                case 6:
                    name[13] = 'a';
                    break;
                case 3:
                case 5:
                case 1:
                    name[13] = 'b';
                    break;
                }
            }
            sprintf(path, "/database/cosedit/%s", name);
            m428.vf0C(path);
        }
    } while (csv.fn_8005BBF8() != 0);
}

} // namespace chr

namespace chr {

// NONMATCHING: 98.1% - the original keeps the "is long" flag of `material` in r27 for the second
// c_str() of each "MODEL"/"MODEL_LOD" row (cntlzw chain); here it is tested again
void CharaEquip_unk::fn_80564340() {
    atn::CsvTab csv(m418.fn_80470580(), m418.fn_8047059C());
    std::string model;
    std::string material;
    do {
        const char* field = csv.fn_8005B9CC();
        if (*field == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            model = csv.fn_8005B9CC();
            material = csv.fn_8005B9CC();
            if (m014 == 0) {
                int member = fn_80219558(m40C->m50);
                model = fn_8006B2D8(fn_8006B174(model));
                if (m000 >= 0 && m000 < 4) {
                    switch (member) {
                    case 0:
                    case 4:
                    case 2:
                    case 6:
                        model[8] = 'a';
                        break;
                    case 3:
                    case 5:
                    case 1:
                        model[8] = 'b';
                        break;
                    }
                }
                model += ".model";
                if (strcmp(material.c_str(), "") != 0) {
                    material = fn_8006B2D8(fn_8006B174(material));
                    switch (m000) {
                    case 0:
                    case 1:
                        switch (member) {
                        case 0:
                        case 4:
                            material[8] = 'a';
                            break;
                        case 2:
                            material[8] = 'a';
                            material += "_qu";
                            break;
                        case 3:
                            material[8] = 'b';
                            material += "_se";
                            break;
                        case 6:
                            material[8] = 'a';
                            material += "_ja";
                            break;
                        case 5:
                        case 1:
                            material[8] = 'b';
                            break;
                        }
                        break;
                    case 2:
                    case 3:
                        switch (member) {
                        case 0:
                        case 4:
                        case 2:
                        case 6:
                            material[8] = 'a';
                            break;
                        case 3:
                        case 5:
                        case 1:
                            material[8] = 'b';
                            break;
                        }
                        break;
                    }
                    material += ".material";
                }
            }
            if (strcmp(material.c_str(), "") == 0) {
                m024.fn_8008AD4C(model.c_str(), 0);
            } else {
                m024.fn_8008AD4C(model.c_str(), material.c_str());
            }
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            model = csv.fn_8005B9CC();
            if (strcmp(material.c_str(), "") == 0) {
                m024.fn_80092F1C(model.c_str(), 0, fn_800DC288(csv.fn_8005B9CC()));
            } else {
                m024.fn_80092F1C(model.c_str(), material.c_str(), fn_800DC288(csv.fn_8005B9CC()));
            }
        } else if (strcmp(field, "SCALE") == 0) {
            m400 = fn_800DC288(csv.fn_8005B9CC());
            m404 = fn_800DC288(csv.fn_8005B9CC());
            m408 = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
    if (m000 < 4) {
        m430.reset(new ((MemoryTag)6, "", "", 0) OptionParts_unk);
        fn_8020C5D4(m430.get(), (const char*)m428.fn_80470580(), m428.fn_8047059C());
        int member = fn_80219558(m40C->m50);
        if (m018 != 0 && member < 7 && m424 != 0 && m00C == 0 && m010 == 0 && m014 == 0) {
            std::string name(m428.fn_80473F18());
            name = fn_8006B2D8(fn_8006B174(name));
            fn_8020DE0C(m430.get(), name.c_str());
            if (Inventory::instance_ != 0) {
                if (lbl_8087F610 != 0) {
                    ArenaEquip_unk* equip = fn_804EB650(lbl_8087F610, m40C, m000);
                    *m424 = equip->mOptions;
                    fn_8020C4C8(equip->mPalettes, m430.get(), (OptionColorOut_unk*)m424);
                } else {
                    fn_8020C4C8(&Inventory::instance_->mEquipShown[member].mProtectors[0].mId, m430.get(),
                                (OptionColorOut_unk*)m424);
                }
            }
        }
        fn_8020CC64(&m024, m430.get(), (OptionColorOut_unk*)m424);
    }
}

// inferred: clears the bits of the option parts without level; names invented
inline void CharaEquip_unk::ClearLevels_unk() {
    if (m430) {
        for (int i = 0; i < m430->mPartCount; i++) {
            if (m430->mParts[i].mLevel == 0) {
                m438 &= ~(1 << i);
            }
        }
    }
}

inline void CharaEquip_unk::ShowParts_unk() {
    if (m430) {
        for (int i = 0; i < m430->mPartCount; i++) {
            unsigned int bit = 1 << i;
            m024.fn_8009373C(m430->mParts[i].mKey.c_str(), bit == (bit & m438));
        }
    }
}

void CharaEquip_unk::fn_80565064() {
    atn::CsvTab csv(m418.fn_80470580(), m418.fn_8047059C());
    do {
        const char* field = csv.fn_8005B9CC();
        if (*field == ';') {
            continue;
        }
        if (strcmp(field, "BOUND_CENTER_NODE") == 0) {
            const char* name = csv.fn_8005B9CC();
            int node = strcmp(name, "") == 0 ? -1 : m024.GetNodeNoByName(name, 0);
            name = csv.fn_8005B9CC();
            int parent = strcmp(name, "") == 0 ? -1 : m024.GetNodeNoByName(name, 0);
            m024.fn_80095D44(node, parent);
            m024.mAC = 1.5f;
        } else if (strcmp(field, "OPTION_PARTS") == 0) {
            char name[0x100];
            strcpy(name, csv.fn_8005B9CC());
            m024.fn_8009373C(name, fn_800DC12C(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MATERIAL_HIDE") == 0) {
            m024.fn_80093D98(csv.fn_8005B9CC(), 0);
        }
    } while (csv.fn_8005BBF8() != 0);
    if (m000 < 4 && m020 != 0) {
        m438 = -1;
        if (m424 != 0) {
            m438 = *m424;
        }
        ClearLevels_unk();
        fn_80565428();
        fn_80565568();
        ShowParts_unk();
    }
}

// NONMATCHING: 98.75% - registers of the hide pointer (r6 here r7) and of m430 in the loop
void CharaEquip_unk::fn_80565428() {
    m024.fn_80093EEC(1);
    if (m430) {
        for (int i = 0; i < m430->mHideCount; i++) {
            int show = 1;
            for (int j = 0; j < m430->mHides[i].mCount; j++) {
                unsigned int bit = 1 << m430->mHides[i].mParts[j];
                if (bit == (bit & m438)) {
                    show = 0;
                    break;
                }
            }
            m024.fn_80093D98(m430->mHides[i].mName.c_str(), show);
        }
    }
}

// NONMATCHING: 97% - the original tests the character with bne+b and puts the temporary of the sum
// above the temporaries of the prefix on the stack (an inline helper for the prefix is not inlined)
void CharaEquip_unk::fn_80565568() {
    if (m000 == 0 && m430.get() != 0) {
        ::CharaTask* chara = m40C;
        if (chara == 0) {
            return;
        }
        atn::Model* model = chara->GetModel_unk();
        std::string prefix(fn_8006B174(std::string(model->fn_8008B130())), 0, 6);
        model->fn_80093EEC(1);
        for (int i = 0; i < m430->mBaseHideCount; i++) {
            int show = 1;
            for (int j = 0; j < m430->mBaseHides[i].mCount; j++) {
                unsigned int bit = 1 << m430->mBaseHides[i].mParts[j];
                if (bit == (bit & m438)) {
                    show = 0;
                    break;
                }
            }
            model->fn_80093D98((prefix + m430->mBaseHides[i].mName.c_str()).c_str(), show);
        }
        if (fn_80219558(m40C->m50) == 0) {
            model->fn_8009373C("op_hane", !fn_8020EF4C((EquipParamEntry_unk*)m414));
        }
        if (fn_80219558(m40C->m50) == 1) {
            model->fn_8009373C("op_necklace01", !fn_8020EF4C((EquipParamEntry_unk*)m414));
            model->fn_8009373C("op_necklace02", !fn_8020EF4C((EquipParamEntry_unk*)m414));
        }
    }
}

void CharaEquip_unk::fn_805657CC(int color, unsigned int* bits) {
    m008 = color;
    m424 = bits;
    m438 = -1;
    if (m424 != 0) {
        m438 = *m424;
    }
    ClearLevels_unk();
    fn_80565428();
    fn_80565568();
    ShowParts_unk();
}

void CharaEquip_unk::fn_805659B4() {
    m438 = -1;
    if (m424 != 0) {
        m438 = *m424;
    }
    ClearLevels_unk();
    fn_80565428();
    fn_80565568();
    if (m430) {
        for (int i = 0; i < m430->mPartCount; i++) {
            unsigned int bit = 1 << i;
            m024.fn_8009373C(m430->mParts[i].mKey.c_str(), bit == (bit & m438));
        }
    }
}

#define EQUIP_MAX(a, b) ((a) > (b) ? (a) : (b))

inline float Mag_unk(const atn::Vector3& v) {
    return PSVECMag(&v);
}

// inferred: largest component (the largest scale of a matrix); name invented
inline float MaxScale_unk(const atn::Vector3& scale) {
    return EQUIP_MAX(scale.x, EQUIP_MAX(scale.y, scale.z));
}

// inferred: scene of a model (its own, else the one of the renderer); name invented
inline atn::Scene* SceneOf_unk(atn::Model& model) {
    return (model.m1DC == 0) ? atn::RenderManager::instance_->mScene : model.m1DC;
}

// NONMATCHING: 99.2% - stack slots of the column temporaries and the address of the character
// matrix kept in r31 by the original
void CharaEquip_unk::fn_80565B98() {
    if (m024.mLods[0].m08 == 0) {
        return;
    }
    switch (m000) {
    case 0:
    case 1:
    case 2:
    case 3: {
        const atn::Matrix3x4& mtx = m40C->GetModel_unk()->mMtx;
        static_cast<atn::Matrix3x4&>(m024.mMtx) = mtx;
        m024.m54 = MaxScale_unk(atn::Vector3(Mag_unk(atn::Vector3(mtx.m[0][0], mtx.m[1][0], mtx.m[2][0])),
                                             Mag_unk(atn::Vector3(mtx.m[0][1], mtx.m[1][1], mtx.m[2][1])),
                                             Mag_unk(atn::Vector3(mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]))));
        m024.fn_80091C10(m40C->GetModel_unk(), m3F8.mData, 0);
        if (lbl_8087F540 == 0 || lbl_8087F540->fn_80481668() == 0) {
            fn_805660DC();
        }
        break;
    }
    case 4: {
        if (m3F4 == 0) {
            m3F4 = (int)m40C->GetModel_unk()->GetNodeMtx_unk("leftarmwepoint");
        }
        atn::Matrix3x4 mtx = *(atn::Matrix3x4*)m3F4;
        atn::Matrix3x4 scaleMtx;
        PSMTXScale(scaleMtx.m, m400, m404, m408);
        atn::Matrix3x4 result;
        PSMTXConcat(mtx.m, scaleMtx.m, result.m);
        static_cast<atn::Matrix3x4&>(m024.mMtx) = mtx = result;
        m024.m54 = MaxScale_unk(atn::Vector3(Mag_unk(atn::Vector3(mtx.m[0][0], mtx.m[1][0], mtx.m[2][0])),
                                             Mag_unk(atn::Vector3(mtx.m[0][1], mtx.m[1][1], mtx.m[2][1])),
                                             Mag_unk(atn::Vector3(mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]))));
        m024.CalcMatrixStack(atn::ModelNodeCallback_unk());
        break;
    }
    }
}

void CharaEquip_unk::fn_80565F34() {
    if (m024.mLods[0].m08 == 0) {
        return;
    }
    if (m40C != 0) {
        m024.mFlags |= 0x10000;
        m024.m1DC = SceneOf_unk(*m40C->GetModel_unk());
        if (SceneOf_unk(m024) != 0) {
            m024.m1E0 = (int)SceneOf_unk(m024)->fn_800C258C(&m40C->GetModel_unk()->fn_80094F98().mCenter);
        }
    } else {
        m024.mFlags &= ~0x10000;
        m024.m1E0 = 0;
    }
    m024.fn_8008CD60();
}

// NONMATCHING: 92.9% - the original keeps separate induction registers for m48 and the node array, and
// its new[] uses the "" pair of another unit (lbl_8087D6E8/EC, docs/trucos-mwcc.md: not renamed)
void CharaEquip_unk::fn_80566010() {
    if (m024.mLods[0].m08 != 0) {
        int count = ((ModelData_unk*)m024.mLods[0].m08)->m44;
        m3F8.Resize_unk(count, 6);
        atn::Model* model = m40C->GetModel_unk();
        for (int i = 0; i < count; i++) {
            m3F8.mData[i] = model->fn_800928B0(((ModelData_unk*)m024.mLods[0].m08)->m48[i]->m14, 0);
        }
    }
}

// inferred: plays an effect on a model (name invented)
inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
}

// NONMATCHING: 99.4% - the original tests the frozen flag with bne+b to the body (an inline helper
// for the test gives a bool instead)
// inferred: plays the effect of the colour mark of the character (party slot 0 only)
void CharaEquip_unk::fn_805660DC() {
    if (m000 != 0) {
        return;
    }
    int member = (m40C != 0) ? fn_80219558(m40C->m50) : -1;
    if (lbl_8087F610 != 0) {
        if (m40C == 0 || m40C->mStatus.m231 == 0) {
            fn_80566398(1);
            return;
        }
    } else if (!(member >= 0 && member < 7 && Inventory::instance_ != 0 &&
                 Inventory::instance_->mPlayers[member].m231 != 0)) {
        fn_80566398(1);
        return;
    }
    if (lbl_8087F430 == 0 || !((GameTask*)lbl_8087F430)->fn_803754C4()) {
        if (lbl_8087F3C0 == 0) {
            return;
        }
        if (lbl_8087F3C0->IsFlag_unk(1)) {
            return;
        }
        if (lbl_8087F3C0->IsFlag_unk(4)) {
            return;
        }
    }
    {
        if (--m434 <= 0) {
            m434 = 20;
            int mark;
            if (lbl_8087F610 != 0) {
                mark = m40C->mStatus.m231 - 1;
            } else {
                mark = Inventory::instance_->mPlayers[member].m231 - 1;
            }
            if (lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_803754C4()) {
                lbl_8087F3C0->mSystem.m70 = 1;
                lbl_8087F3C0->mSystem.mLayer = 6;
            } else {
                lbl_8087F3C0->mSystem.mLayer = 2;
            }
            m01C = 1;
            fn_80239DAC(lbl_8087F3C0, (unsigned int)this, 0, 1);
            fn_80232B7C((unsigned int)this, 0);
            if (lbl_8087F048 != 0) {
                PlayEffect_unk(&lbl_8087F048->mMarkEfp[mark], m40C->GetModel_unk());
            }
            if (lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_803754C4()) {
                lbl_8087F3C0->mSystem.mLayer = 0;
                lbl_8087F3C0->mSystem.m70 = 0;
            } else {
                lbl_8087F3C0->mSystem.mLayer = 0;
            }
        }
    }
}

void CharaEquip_unk::fn_80566398(int flag) {
    if (m01C != 0 && lbl_8087F3C0 != 0) {
        fn_80239DAC(lbl_8087F3C0, (unsigned int)this, 0, flag);
    }
    m01C = 0;
    m434 = 20;
}

void CharaEquip_unk::fn_805663F8() {
    m024.mFlags &= ~4;
}

void CharaEquip_unk::fn_80566408() {
    m024.mFlags |= 4;
}

} // namespace chr
