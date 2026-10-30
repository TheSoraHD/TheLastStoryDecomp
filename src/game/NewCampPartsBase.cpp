#include "game/NewCampPartsBase.h"
#include "atn/D2A_Task.h"
#include "atn/D2A_TaskInstanceC.h"
#include "atn/D3Anime.h"
#include "atn/D3AnimeInstance.h"
#include "atn/Model_unk.h"
#include "atn/SoundRef.h"
#include "game/CharaStatus.h"
#include "game/MenuCommonTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" atn::SoundRef fn_80117228(int id);        // inferred: plays a system sound effect
extern "C" void* lbl_8087EF70;                       // inferred: pad
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" float fn_800A08D4(atn::LH_Motion* motion);       // inferred: motion length
extern "C" int fn_800A0548(atn::LH_Motion* motion, MotionPose_unk* pose, unsigned int hash, char* work,
                           float frame); // inferred: pose of the node `hash` at `frame`
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value); // inferred: sets a layout value
extern "C" int fn_8021946C(int chara, int a, int b);
extern "C" void fn_80530D88(atn::Task* camp, int a, atn::ModelBase* model);
// debug menu (see game/CosEditBase.cpp)
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
CharaParam_unk fn_80134800(CharaStatus* status, int id, int kindA, int indexA, int itemA, int kindB, int indexB,
                           int itemB);

// inferred: camp task (NewCampPartsBase::m48; game/NewCampTask.cpp has no source yet); only the members
// used here
struct NewCampCamp_unk {
    unsigned char m0000[0x11F0];
    NewCampPartsBase* mCurrent; // 0x11F0: inferred: parts shown now
};

// inferred: camera (lbl_8087EFB4); m15C is the view matrix
struct NewCampCamera_unk {
    unsigned char m000[0x15C];
    atn::Matrix3x4 mView; // 0x15C
};
extern "C" NewCampCamera_unk* lbl_8087EFB4;
// inferred: d2anime manager of the menus; m1A8/m1AC: the current anime and layout
struct NewCampD2AManager_unk {
    unsigned char m000[0x1A8];
    void* m1A8;
    void* m1AC;
};
extern "C" NewCampD2AManager_unk* lbl_8087F138;

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))
#define __MIN_unk(a, b) ((a) < (b) ? (a) : (b))

// inferred: componentwise product (name invented)
inline atn::Vector3 MulSub_unk(const atn::Vector3& a, const atn::Vector3& c, const atn::Vector3& b) {
    return atn::Vector3((a.x - c.x) * b.x, (a.y - c.y) * b.y, (a.z - c.z) * b.z);
}
inline atn::Vector3 Mul_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return atn::Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

// inferred: scale of the columns of a matrix (names invented)
inline atn::Vector3 GetCol_unk(const atn::Matrix3x4& m, int i) {
    float z = m.m[2][i];
    float y = m.m[1][i];
    float x = m.m[0][i];
    return atn::Vector3(x, y, z);
}
inline atn::Vector3 GetScale_unk(const atn::Matrix3x4& m) {
    return atn::Vector3(GetCol_unk(m, 0).Length_unk(), GetCol_unk(m, 1).Length_unk(), GetCol_unk(m, 2).Length_unk());
}

// inferred: world matrix of the node `node` of the entry model (names invented)
inline atn::Matrix3x4 NodeWorld_unk(NewCampPartsEntry_unk* entry, const char* node) {
    atn::Matrix3x4 mtx(((atn::Model*)GetModelRes_unk(entry->mModel))->GetNodeMtx_unk(node));
    PSMTXConcat(entry->mModel->mMtx.m, mtx.m, mtx.m);
    return mtx;
}
inline atn::Matrix3x4 ScaleMtx_unk(const atn::Vector3& v) {
    atn::Matrix3x4 m;
    PSMTXScale(m.m, v.x, v.y, v.z);
    return m;
}
inline atn::Matrix3x4 MulMtx_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 m;
    PSMTXConcat(a.m, b.m, m.m);
    return m;
}

// inferred: world matrix of the node `node` of the entry model, identity without it (names invented)
inline atn::Matrix3x4 GetNodeWorld_unk(NewCampPartsEntry_unk* entry, const char* node) {
    atn::Matrix3x4 mtx;
    mtx.SetIdentity();
    atn::Model* model = (atn::Model*)GetModelData_unk(entry->mModel);
    if (model->GetNodeMtx_unk(node) != 0) {
        PSMTXConcat(entry->GetInstance_unk()->mMtx.m, model->GetNodeMtx_unk(node), mtx.m);
    }
    return mtx;
}

NewCampPartsBase::NewCampPartsBase(atn::Task* parent)
    : atn::Task(parent), m48(parent), m4C(0), m58(-1.0f), m64(0.0f, 0.0f, 0.0f), m70(0.0f, 0.0f, 0.0f), m7C(0),
      m80(0), m84(0), m88(0), m8C(0), m90(0), m94(1), m95(0), m96(0), mA4(0.0f, 0.0f, 0.0f), mB0(1.0f, 1.0f, 1.0f),
      mBC(1.0f, 1.0f, 1.0f), mC8(0.0f, 0.0f, 0.0f) {
    m50.Reset_unk(0);
    m54.Reset_unk(0);
}

NewCampPartsBase::~NewCampPartsBase() {}

// NONMATCHING: 67.6% - the string pool (the original has three empty strings before "reference";
// this "" is the second one) and the saved registers of the loop (entries/file/counter)
void NewCampPartsBase::fn_8050FB38(int count, NewCampPartsEntry_unk* entries, const NewCampPartsFile_unk* file,
                                   int load, int kind) {
    for (int i = 0; i < count; i++) {
        if (strcmp(file->mAnime, "") == 0) {
            entries->mAnime = 0;
        } else {
            entries->m04 = kind;
            if (entries->m04 == 1) {
                entries->mAnime = (atn::D3Anime*)atn::D3AnimeInstance::Create(this, file->mAnime, load);
                entries->mAnime->fn_800D246C(1);
            } else {
                entries->mAnime = atn::D3Anime::Create(this, file->mAnime, load);
                entries->mAnime->fn_800D246C(1);
            }
        }
        file++;
        entries++;
    }
}

// NONMATCHING: 68.6% - string pool (see fn_8050FB38) and loop registers
void NewCampPartsBase::fn_8050FC04(int count, NewCampPartsMotion_unk* motions,
                                   const NewCampPartsMotionFile_unk* file) {
    for (int i = 0; i < count; i++) {
        const char* path = file->mPath;
        if (strcmp(path, "") != 0) {
            motions->mMotion.fn_800A03E4(path);
            motions->mHash = fn_800DC6B4(file->mNode);
        }
        file++;
        motions++;
    }
}

// NONMATCHING: 85.3% - the original passes `load` in r4 to D3AnimeInstance::Load() too, and the
// loop registers differ
void NewCampPartsBase::fn_8050FC88(int count, NewCampPartsEntry_unk* entries, int load) {
    for (int i = 0; i < count; i++) {
        if (entries->m04 == 1) {
            atn::D3AnimeInstance* anime = (atn::D3AnimeInstance*)entries->mAnime;
            if (anime != 0) {
                anime->Load();
            }
        } else {
            atn::D3Anime* anime = entries->mAnime;
            if (anime != 0) {
                anime->Load(load);
            }
        }
        entries++;
    }
}

int NewCampPartsBase::fn_8050FD20() {
    return ((NewCampCamp_unk*)m48)->mCurrent == this;
}

// NONMATCHING: 91.4% - registers of the two loops
void NewCampPartsBase::fn_8050FD38(NewCampScene_unk* scene, NewCampPartsEntry_unk* entries,
                                   const NewCampPartsFile_unk* file, int count, const char* name) {
    if (count > 0) {
        for (NewCampNode_unk* node = scene->mNodes; node != 0; node = node->mNext) {
            if (node->mType == 4) {
                const NewCampPartsFile_unk* f = file;
                NewCampPartsEntry_unk* e = entries;
                for (int i = 0; i < count; i++) {
                    if (strstr(node->mName, name) != 0) {
                        fn_8050FE44(node, e, f);
                    }
                    f++;
                    e++;
                }
            }
        }
        for (int i = 0; i < count; i++) {
            if (entries->m04 == 1) {
                if (entries->mAnime != 0) {
                    entries->mAnime->fn_800D246C(0);
                }
            } else if (entries->mAnime != 0) {
                entries->mAnime->fn_800D246C(0);
            }
            entries->m3C = file->m08;
            entries++;
            file++;
        }
    }
}

// NONMATCHING: 97.1% - only the string pool ("reference" at +3 in the original)
void NewCampPartsBase::fn_8050FE44(NewCampNode_unk* node, NewCampPartsEntry_unk* entry,
                                   const NewCampPartsFile_unk* file) {
    for (int i = 0; i < node->mModelNum; i++) {
        NewCampPartsModel_unk* model = node->mModels[i];
        unsigned int hash = model->mHash;
        if (hash == fn_800DC6B4(file->mModel)) {
            entry->mModel = model;
            if (GetModelData_unk(entry->mModel) != 0 &&
                ((atn::Model*)GetModelData_unk(entry->mModel))->GetNodeMtx_unk("reference") != 0) {
                atn::Matrix3x4 mtx(((atn::Model*)GetModelData_unk(entry->mModel))->GetNodeMtx_unk("reference"));
                PSMTXConcat(entry->mModel->mMtx.m, mtx.m, mtx.m);
                if (entry->m04 == 1) {
                    atn::D3AnimeInstance* anime = (atn::D3AnimeInstance*)entry->mAnime;
                    if (anime != 0) {
                        anime->mPos = mtx.GetTrans_unk();
                    }
                } else {
                    atn::D3Anime* anime = entry->mAnime;
                    if (anime != 0) {
                        anime->mPos = mtx.GetTrans_unk();
                    }
                }
            }
            entry->mMtx = GetModel_unk(entry->mModel)->mMtx;
        }
    }
}

int NewCampPartsBase::vf0C() {
    int ret = 1;
    if (fn_800D3FA4() == 0) {
        ret = 0;
    }
    if (m5C.fn_80473F50() != 0) {
        ret = 0;
    }
    if (vf4C() != 0) {
        ret = 0;
    }
    return ret;
}

void NewCampPartsBase::vf10() {}

void NewCampPartsBase::vf54(NewCampPartsEntry_unk* entry, const char* node) {
    vf50(entry, GetModelData_unk(entry->mModel), node);
}

// NONMATCHING: 86.8% - the offset/scale computation uses six more saved FPRs and a bigger frame
void NewCampPartsBase::vf50(NewCampPartsEntry_unk* entry, atn::ModelBase* model, const char* node) {
    if (entry != 0) {
        if (entry->GetInstance_unk() != 0) {
            atn::Matrix3x4 mtx(entry->GetInstance_unk()->mMtx);
            atn::Vector3 pos = mtx.GetTrans_unk() + mA4;
            pos = MulSub_unk(pos, mC8, mBC) + mC8;
            mtx.m[0][3] = 0.0f;
            mtx.m[1][3] = 0.0f;
            mtx.m[2][3] = 0.0f;
            atn::Vector3 scale = Mul_unk(mB0, mBC);
            Mtx s;
            PSMTXScale(s, scale.x, scale.y, scale.z);
            atn::Matrix3x4 tmp;
            PSMTXConcat(mtx.m, s, tmp.m);
            mtx = tmp;
            mtx.m[0][3] = pos.x;
            mtx.m[1][3] = pos.y;
            mtx.m[2][3] = pos.z;
            entry->GetInstance_unk()->fn_8009EE30(mtx);
        }
        if (entry->mModel != 0 && entry->GetInstance_unk() != 0 && model != 0) {
            atn::Matrix3x4* view = &lbl_8087EFB4->mView;
            atn::Vector3 pos = GetModel_unk(entry->mModel)->mMtx.GetTrans_unk();
            PSMTXMultVec(entry->mModel->mMtx.m, &pos, &pos);
            PSMTXMultVec(view->m, &pos, &pos);
            ((atn::Model*)model)->m98 = -pos.z;
            if (entry->mAnime != 0) {
                atn::Matrix3x4 mtx;
                mtx.SetIdentity();
                if (((atn::Model*)model)->GetNodeMtx_unk(node) != 0) {
                    mtx = atn::Matrix3x4(((atn::Model*)model)->GetNodeMtx_unk(node));
                    PSMTXConcat(entry->mModel->mMtx.m, mtx.m, mtx.m);
                }
                if (entry->m04 == 1) {
                    ((atn::D3AnimeInstance*)entry->mAnime)->mPos = mtx.GetTrans_unk();
                } else {
                    entry->mAnime->mPos = mtx.GetTrans_unk();
                }
            }
        }
    }
}

void NewCampPartsBase::vf5C(NewCampPartsEntry_unk* entry) {
    if (entry->mAnime != 0 && entry->GetInstance_unk() != 0) {
        if (entry->m04 == 1) {
            ((atn::D3AnimeInstance*)entry->mAnime)->mScale = GetScale_unk(entry->GetInstance_unk()->mMtx);
        } else {
            entry->mAnime->mScale = GetScale_unk(entry->GetInstance_unk()->mMtx);
        }
    }
}

// NONMATCHING: 94.8% - frame layout of the temporaries
void NewCampPartsBase::vf58(NewCampPartsEntry_unk* entry, const char* node) {
    if (entry->mModel != 0 && entry->mAnime != 0 && GetModelRes_unk(entry->mModel) != 0 &&
        ((atn::Model*)GetModelRes_unk(entry->mModel))->GetNodeMtx_unk(node) != 0) {
        atn::Matrix3x4 mtx = NodeWorld_unk(entry, node);
        mtx.m[0][3] = 0.0f;
        mtx.m[1][3] = 0.0f;
        mtx.m[2][3] = 0.0f;
        atn::Vector3 s = GetScale_unk(mtx);
        mtx = MulMtx_unk(mtx, ScaleMtx_unk(atn::Vector3(1.0f / s.x, 1.0f / s.y, 1.0f / s.z)));
        if (entry->m04 == 1) {
            ((atn::D3AnimeInstance*)entry->mAnime)->mRot = mtx;
        } else {
            entry->mAnime->mRot = mtx;
        }
    }
}

// NONMATCHING: 97.2% - the first null test keeps the pointer, and the copy after the scale swaps
// r3/r4. Tried: matrix helpers returning by value, declaration order of the temporaries
void NewCampPartsBase::vf60(NewCampPartsEntry_unk* entry, NewCampPartsMotion_unk* motion, unsigned int flags,
                            float step) {
    if (entry == 0) {
        return;
    }
    if (entry->GetInstance_unk() == 0) {
        return;
    }
    int play = 1;
    if (motion == 0 || motion->mMotion.m08 == 0) {
        play = 0;
    }
    if (play != 0) {
        atn::Vector3 scale(1.0f, 1.0f, 1.0f);
        if (motion->mMotion.m08 == 0) {
            return;
        }
        motion->mFrame += step;
        if (motion->mFrame >= fn_800A08D4(&motion->mMotion)) {
            motion->mFrame = fn_800A08D4(&motion->mMotion);
        }
        if (motion->mFrame <= 0.0f) {
            motion->mFrame = 0.0f;
        }
        fn_800A0548(&motion->mMotion, &motion->mPose, motion->mHash, motion->mWork, motion->mFrame);
        atn::Matrix3x4 mtx;
        mtx.SetIdentity();
        atn::Matrix3x4 q;
        atn::Matrix3x4 world;
        if ((flags & 2) == 2 && motion->mPose.m10.w != 0.0f) {
            PSMTXQuat(q.m, &motion->mPose.m10);
            mtx = q;
        }
        if ((flags & 4) == 4) {
            Mtx s;
            PSMTXScale(s, motion->mPose.mScale.x, motion->mPose.mScale.y, motion->mPose.mScale.z);
            atn::Matrix3x4 tmp;
            PSMTXConcat(mtx.m, s, tmp.m);
            mtx = tmp;
        }
        if ((flags & 1) == 1) {
            mtx.m[0][3] = motion->mPose.mPos.x;
            mtx.m[1][3] = motion->mPose.mPos.y;
            mtx.m[2][3] = motion->mPose.mPos.z;
        }
        PSMTXConcat(entry->mMtx.m, mtx.m, world.m);
        mtx = world;
        entry->GetInstance_unk()->fn_8009EE30(mtx);
        if (entry->mAnime != 0) {
            if (entry->m04 == 1) {
                ((atn::D3AnimeInstance*)entry->mAnime)->mScale = motion->mPose.mScale;
            } else {
                entry->mAnime->mScale = motion->mPose.mScale;
            }
        }
    } else {
        GetModel_unk(entry->mModel)->fn_8009EE30(entry->mMtx);
    }
}

// NONMATCHING: 99.5% - only the string pool (see fn_8050FB38)
void NewCampPartsBase::vf14() {
    if (m4C > 0) {
        for (int i = 0; i < m4C; i++) {
        vf58(&m50[i], "reference");
        vf54(&m50[i], "reference");
        }
    }
}

void NewCampPartsBase::fn_80510D64(int wrap, int se) {
    if (m84 > 0) {
        void* pad = lbl_8087EF70;
        int prev = m80;
        if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
            if (--m80 < 0) {
                if (wrap != 0) {
                    m80 = m84 - 1;
                } else {
                    m80 = 0;
                }
            }
        } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
            m80++;
            if (m80 >= m84) {
                if (wrap != 0) {
                    m80 = 0;
                } else {
                    m80 = m84 - 1;
                }
            }
        }
        if (m80 != prev) {
            fn_80117228(se);
        }
    }
}

void NewCampPartsBase::fn_80510E94(int wrap, int se) {
    void* pad = lbl_8087EF70;
    int prev = m80;
    if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
        if (--m80 < 0) {
            if (wrap != 0) {
                m80 = m84 - 1;
            } else {
                m80 = 0;
            }
        }
    } else if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
        m80++;
        if (m80 >= m84) {
            if (wrap != 0) {
                m80 = 0;
            } else {
                m80 = m84 - 1;
            }
        }
    }
    if (m80 != prev) {
        fn_80117228(se);
    }
}

void NewCampPartsBase::fn_80510FB8(int wrap, int se) {
    void* pad = lbl_8087EF70;
    int prev = m80;
    if (fn_800A55AC(pad, 0, 0) != 0 || fn_800A55AC(pad, 0, 0x1A) != 0) {
        if (--m80 < 0) {
            if (wrap != 0) {
                m80 = m84 - 1;
            } else {
                m80 = 0;
            }
        }
    } else if (fn_800A55AC(pad, 0, 1) != 0 || fn_800A55AC(pad, 0, 0x19) != 0) {
        m80++;
        if (m80 >= m84) {
            if (wrap != 0) {
                m80 = 0;
            } else {
                m80 = m84 - 1;
            }
        }
    } else if (fn_800A55AC(pad, 0, 2) != 0 || fn_800A55AC(pad, 0, 0x17) != 0) {
        m80 -= m8C;
        if (m80 < 0) {
            if (wrap != 0) {
                m80 = prev % m8C + m8C * ((m84 - 1) / m8C);
            } else {
                m80 = prev;
            }
            if (m80 >= m84) {
                m80 = m84 - 1;
            }
        }
    } else if (fn_800A55AC(pad, 0, 3) != 0 || fn_800A55AC(pad, 0, 0x18) != 0) {
        m80 += m8C;
        if (m80 >= m84) {
            if (wrap != 0) {
                m80 = prev % m8C;
            } else {
                m80 = prev;
            }
            if (m80 >= m84) {
                m80 = m84 - 1;
            }
        }
    }
    m88 = m8C * (m80 / m8C);
    if (m80 != prev) {
        fn_80117228(se);
    }
}

void NewCampPartsBase::vf3C(int arg) {
    for (int i = 0; i < m4C; i++) {
        if (m50[i].mAnime != 0 && m50[i].m3C != 0) {
            m50[i].mAnime->mVisible = arg;
        }
    }
}

// NONMATCHING: 69.4% - the original copies z of each vector right after loading it (neither
// __VecCopy nor __VecCopyZFirst)
void NewCampPartsBase::fn_80511258(NewCampPartsEntry_unk* entry, atn::D3Anime* anime, const atn::Vector3& pos,
                                   const atn::Vector3& scale, const char* node) {
    fn_80512474(entry, anime, atn::Vector3(pos), atn::Vector3(scale), node);
}

// NONMATCHING: 69.4% - see fn_80511258
void NewCampPartsBase::fn_805112A8(NewCampPartsEntry_unk* entry, atn::D3AnimeInstance* anime,
                                   const atn::Vector3& pos, const atn::Vector3& scale, const char* node) {
    fn_80512028(entry, anime, atn::Vector3(pos), atn::Vector3(scale), node);
}

// NONMATCHING: 99.4% - only the string pool (see fn_8050FB38)
void NewCampPartsBase::vf40(void* menu) {
    void* group = fn_8008937C(menu, m98.c_str());
    fn_80087E9C(group, "OffsetPos", &mA4, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "OffsetScl", &mB0, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "LocatorScl", &mBC, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
    fn_80087E9C(group, "BasePos", &mC8, 0, 0, -1000000.0f, 1000000.0f, 1.0f);
}

void fn_80511668(int a, int b, int c);
void fn_80511704();
void fn_8051170C();

// NONMATCHING: 78.8% - the bit fields of the D2A task and the casts of GetTask()
extern "C" void fn_805113E8(atn::D3Anime* anime, int visible, int loop, float frame, float speed) {
    if (anime != 0) {
        anime->fn_800D246C(0);
        anime->mVisible = visible;
        if (anime->GetTask() != 0) {
            atn::D2A_Task* task = (atn::D2A_Task*)anime->GetTask();
            task->mRepeat = loop;
            ((atn::D2A_Task*)anime->GetTask())->mSpeed = speed;
            ((atn::D2A_Task*)anime->GetTask())->mFrame = frame;
            ((atn::D2A_Task*)anime->GetTask())->mData.m9C = (void (*)(atn::D2A_Common*, atn::D2A_DrawColor*, void*))fn_80511668;
            ((atn::D2A_Task*)anime->GetTask())->mData.mA0 = (void*)fn_80511704;
            ((atn::D2A_Task*)anime->GetTask())->mData.mA4 = (int)fn_8051170C;
        }
    }
}

// NONMATCHING: 72.9% - see fn_805113E8
extern "C" void fn_805114D4(atn::D3AnimeInstance* anime, int visible, int loop, float frame, float speed) {
    typedef atn::D2A_TaskInstanceBase<32, 8> Inst;
    if (anime != 0) {
        anime->fn_800D246C(0);
        anime->mVisible = visible;
        if (anime->GetTask() != 0) {
            ((Inst*)anime->GetTask())->mLoop = loop != 0;
            ((Inst*)anime->GetTask())->mSpeed = speed;
            ((Inst*)anime->GetTask())->mFrame = frame;
            fn_801F6C10((Inst*)anime->GetTask())->m9C = (void (*)(atn::D2A_Common*, atn::D2A_DrawColor*, void*))fn_80511668;
            fn_801F6C10((Inst*)anime->GetTask())->mA0 = (void*)fn_80511704;
            fn_801F6C10((Inst*)anime->GetTask())->mA4 = (int)fn_8051170C;
        }
    }
}

extern "C" void fn_805115D0(atn::D2A_Task* task, int a, int loop, float frame, float speed) {
    if (task != 0) {
        task->fn_800D246C(0);
        task->mRepeat = loop;
        task->mSpeed = speed;
        task->mFrame = frame;
        task->mData.m9C = (void (*)(atn::D2A_Common*, atn::D2A_DrawColor*, void*))fn_80511668;
        task->mData.mA0 = (void*)fn_80511704;
        task->mData.mA4 = (int)fn_8051170C;
    }
}

// inferred: empty in this build (a log of the anime event?); the original calls it with bl
__declspec(noinline) void fn_80511664(int event) {}

void fn_80511668(int a, int b, int c) {
    fn_80511664(1);
    if (lbl_8087F138 != 0 && MenuCommonTask::sInstance != 0) {
        if (lbl_8087F138->m1AC != 0) {
            MenuCommonTask::sInstance->fn_804A71F0(lbl_8087F138->m1AC, a, b, c);
        } else if (lbl_8087F138->m1A8 != 0) {
            MenuCommonTask::sInstance->fn_804A660C(lbl_8087F138->m1A8, a, b, c);
        }
    }
}

void fn_80511704() {
    fn_80511664(4);
}

void fn_8051170C() {
    fn_80511664(2);
}

void NewCampPartsBase::vf44() {
    for (int i = 0; i < m4C; i++) {
        if (GetModelData_unk(m50[i].mModel) != 0) {
            fn_80530D88(m48, 0, GetModelData_unk(m50[i].mModel));
        }
    }
}

// NONMATCHING: 80.7% - string pool and the access to the layout of the d2anime task
void NewCampPartsBase::vf48() {
    for (int i = 0; i < m4C; i++) {
        NewCampPartsEntry_unk* entry = &m50[i];
        if (entry->m04 == 1) {
            atn::D3AnimeInstance* anime = (atn::D3AnimeInstance*)entry->mAnime;
            if (anime != 0 && anime->GetTask() != 0) {
                fn_801F6C80((atn::D2A_TaskInstanceBase<32, 8>*)((atn::D3AnimeInstance*)m50[i].mAnime)->GetTask(),
                            "frameOn", 0.0f);
            }
        } else {
            atn::D3Anime* anime = entry->mAnime;
            if (anime != 0 && anime->GetTask() != 0) {
                atn::D2A_Task* task = (atn::D2A_Task*)m50[i].mAnime->GetTask();
                fn_801FECE0(&task->mData.m10, fn_800DC6B4("frameOn"), 0.0f);
            }
        }
    }
}

// NONMATCHING: 65.9% - string pool and the scheduling around it
void NewCampPartsBase::fn_805118B4(atn::Task* task, const char* name, NewCampPartsEntry_unk* entry,
                                   const NewCampPartsPoint_unk& pos, int b) {
    atn::Matrix3x4 mtx(((atn::Model*)GetModelRes_unk(entry->mModel))->GetNodeMtx_unk("reference"));
    PSMTXConcat(GetModel_unk(entry->mModel)->mMtx.m, mtx.m, mtx.m);
    MenuCommonTask::sInstance->fn_804A4EEC(task, name, mtx, b, pos.x, pos.y, pos.m08, pos.m0C, pos.m10);
}

// NONMATCHING: 65.9% - see fn_805118B4
void NewCampPartsBase::fn_805119C8(atn::Task* task, const char* name, NewCampPartsEntry_unk* entry,
                                   const NewCampPartsPoint_unk& pos, int b) {
    atn::Matrix3x4 mtx(((atn::Model*)GetModelRes_unk(entry->mModel))->GetNodeMtx_unk("reference"));
    PSMTXConcat(GetModel_unk(entry->mModel)->mMtx.m, mtx.m, mtx.m);
    MenuCommonTask::sInstance->fn_804A5160(task, name, mtx, b, pos.x, pos.y, pos.m08, pos.m0C, pos.m10);
}

// NONMATCHING: 88.3% - string pool and the order of the clamps
extern "C" void fn_80511ADC(atn::D2A_Task* task, CharaStatus* status, int chara, int full) {
    if (task != 0) {
        CharaParam_unk p = fn_80134800(status, chara, 0, 0, 0, 0, 0, 0);
        float exp = status->m1A8.mExp / __MAX_unk((float)(status->m1A8.mExp + status->fn_8012B028()), 1.0f);
        if (p.m78 >= 99) {
            exp = 0.0f;
        }
        if (full != 0) {
            task->SetNumber_unk("nowHP", p.m84, 0);
            fn_801FECE0(&task->mData.m10, fn_800DC6B4("hpRate"), 1.0f);
        } else {
            task->SetNumber_unk("nowHP", (int)status->m04, 0);
            fn_801FECE0(&task->mData.m10, fn_800DC6B4("hpRate"), status->m04 / __MAX_unk((float)p.m84, 1.0f));
        }
        task->SetNumber_unk("nowLv", p.m78, 0);
        fn_801FECE0(&task->mData.m10, fn_800DC6B4("expRate"), exp);
        fn_801FECE0(&task->mData.m10, fn_800DC6B4("tpRate"), __MIN_unk(__MAX_unk(status->m228, 0.0f), 1.0f));
        fn_801FECE0(&task->mData.m10, fn_800DC6B4("tpOn"), fn_8021946C(chara, status->mParamA.m78, 5) != 0);
    }
}

// NONMATCHING: 95.7% - string pool
extern "C" void fn_80511D9C(atn::D2A_TaskInstanceBase<32, 8>* task, CharaStatus* status, int chara, int full) {
    if (task != 0) {
        CharaParam_unk p = fn_80134800(status, chara, 0, 0, 0, 0, 0, 0);
        float exp = status->m1A8.mExp / __MAX_unk((float)(status->m1A8.mExp + status->fn_8012B028()), 1.0f);
        if (p.m78 >= 99) {
            exp = 0.0f;
        }
        if (full != 0) {
            fn_801F8598(task, "nowHP", p.m84, 0);
            fn_801F6C80(task, "hpRate", 1.0f);
        } else {
            fn_801F8598(task, "nowHP", (int)status->m04, 0);
            fn_801F6C80(task, "hpRate", status->m04 / __MAX_unk((float)p.m84, 1.0f));
        }
        fn_801F8598(task, "nowLv", p.m78, 0);
        fn_801F6C80(task, "expRate", exp);
        fn_801F6C80(task, "tpRate", __MIN_unk(__MAX_unk(status->m228, 0.0f), 1.0f));
        fn_801F6C80(task, "tpOn", fn_8021946C(chara, status->mParamA.m78, 5) != 0);
    }
}

void NewCampPartsBase::vf34(int state) {
    m7C = state;
}

void NewCampPartsBase::vf2C() {}

void NewCampPartsBase::vf20() {}

// NONMATCHING: 92.6% - frame 0x40 bigger (one more matrix temporary) and the `entry->mModel` test
// branches the other way
void NewCampPartsBase::fn_80512028(NewCampPartsEntry_unk* entry, atn::D3AnimeInstance* anime,
                                   const atn::Vector3& pos, const atn::Vector3& scale, const char* node) {
    if (anime != 0) {
        if (entry->GetInstance_unk() != 0) {
            if (GetModelRes_unk(entry->mModel) != 0) {
                if (((atn::Model*)GetModelRes_unk(entry->mModel))->GetNodeMtx_unk(node) != 0) {
                    if (entry->mModel == 0) {
                        return;
                    }
                    anime->fn_800D246C(0);
                    anime->mFlags &= ~4;
                    {
                        atn::Matrix3x4 mtx = NodeWorld_unk(entry, node);
                        mtx.m[0][3] = 0.0f;
                        mtx.m[1][3] = 0.0f;
                        mtx.m[2][3] = 0.0f;
                        atn::Vector3 s = GetScale_unk(mtx);
                        mtx = MulMtx_unk(mtx, ScaleMtx_unk(atn::Vector3(1.0f / s.x, 1.0f / s.y, 1.0f / s.z)));
                        anime->mRot = mtx;
                    }
                    {
                        atn::Matrix3x4 mtx = NodeWorld_unk(entry, node);
                        anime->mPos = mtx.GetTrans_unk() + pos;
                    }
                    anime->mScale = Mul_unk(GetScale_unk(entry->GetInstance_unk()->mMtx), scale);
                }
            }
        }
    }
}

// NONMATCHING: 92.6% - see fn_80512028
void NewCampPartsBase::fn_80512474(NewCampPartsEntry_unk* entry, atn::D3Anime* anime, const atn::Vector3& pos,
                                   const atn::Vector3& scale, const char* node) {
    if (anime != 0) {
        if (entry->GetInstance_unk() != 0) {
            if (GetModelRes_unk(entry->mModel) != 0) {
                if (((atn::Model*)GetModelRes_unk(entry->mModel))->GetNodeMtx_unk(node) != 0) {
                    if (entry->mModel == 0) {
                        return;
                    }
                    anime->fn_800D246C(0);
                    anime->mFlags &= ~4;
                    {
                        atn::Matrix3x4 mtx = NodeWorld_unk(entry, node);
                        mtx.m[0][3] = 0.0f;
                        mtx.m[1][3] = 0.0f;
                        mtx.m[2][3] = 0.0f;
                        atn::Vector3 s = GetScale_unk(mtx);
                        mtx = MulMtx_unk(mtx, ScaleMtx_unk(atn::Vector3(1.0f / s.x, 1.0f / s.y, 1.0f / s.z)));
                        anime->mRot = mtx;
                    }
                    {
                        atn::Matrix3x4 mtx = NodeWorld_unk(entry, node);
                        anime->mPos = mtx.GetTrans_unk() + pos;
                    }
                    anime->mScale = Mul_unk(GetScale_unk(entry->GetInstance_unk()->mMtx), scale);
                }
            }
        }
    }
}
