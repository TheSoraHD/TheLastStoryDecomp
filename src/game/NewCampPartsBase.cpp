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
inline atn::Vector3 Mul_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return atn::Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

// inferred: scale of the columns of a matrix (names invented)
inline atn::Vector3 GetScale_unk(const atn::Matrix3x4& m) {
    atn::Vector3 x(m.m[0][0], m.m[1][0], m.m[2][0]);
    atn::Vector3 y(m.m[0][1], m.m[1][1], m.m[2][1]);
    atn::Vector3 z(m.m[0][2], m.m[1][2], m.m[2][2]);
    return atn::Vector3(PSVECMag(&x), PSVECMag(&y), PSVECMag(&z));
}

// inferred: world matrix of the node `node` of the entry model, identity without it (names invented)
inline atn::Matrix3x4 GetNodeWorld_unk(NewCampPartsEntry_unk* entry, const char* node) {
    atn::Matrix3x4 mtx;
    mtx.SetIdentity();
    atn::Model* model = (atn::Model*)GetModelData_unk(entry->mModel);
    if (model->GetNodeMtx_unk(node) != 0) {
        PSMTXConcat(GetModel_unk(entry->mModel)->mMtx.m, model->GetNodeMtx_unk(node), mtx.m);
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

void NewCampPartsBase::fn_8050FE44(NewCampNode_unk* node, NewCampPartsEntry_unk* entry,
                                   const NewCampPartsFile_unk* file) {
    for (int i = 0; i < node->mModelNum; i++) {
        NewCampPartsModel_unk* model = node->mModels[i];
        if (model->mHash == fn_800DC6B4(file->mModel)) {
            entry->mModel = model;
            if (GetModelData_unk(model) != 0) {
                atn::Model* data = (atn::Model*)GetModelData_unk(model);
                if (data->GetNodeMtx_unk("reference") != 0) {
                    atn::Matrix3x4 mtx = GetNodeWorld_unk(entry, "reference");
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
            }
            if (GetModel_unk(entry->mModel) != 0) {
                entry->mMtx = GetModel_unk(entry->mModel)->mMtx;
            }
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

void NewCampPartsBase::vf50(NewCampPartsEntry_unk* entry, atn::ModelBase* model, const char* node) {
    if (entry != 0) {
        if (GetModel_unk(entry->mModel) != 0) {
            atn::Matrix3x4 mtx(GetModel_unk(entry->mModel)->mMtx);
            atn::Vector3 pos = mtx.GetTrans_unk() + mA4;
            pos = Mul_unk(pos - mC8, mBC) + mC8;
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
            GetModel_unk(entry->mModel)->fn_8009EE30(mtx);
        }
        if (GetModel_unk(entry->mModel) != 0 && model != 0) {
            atn::Matrix3x4* view = &lbl_8087EFB4->mView;
            atn::Vector3 pos = GetModel_unk(entry->mModel)->mMtx.GetTrans_unk();
            PSMTXMultVec(GetModel_unk(entry->mModel)->mMtx.m, &pos, &pos);
            PSMTXMultVec(view->m, &pos, &pos);
            ((atn::Model*)model)->m98 = -pos.z;
            if (entry->mAnime != 0) {
                atn::Matrix3x4 mtx;
                mtx.SetIdentity();
                if (((atn::Model*)model)->GetNodeMtx_unk(node) != 0) {
                    PSMTXConcat(GetModel_unk(entry->mModel)->mMtx.m, ((atn::Model*)model)->GetNodeMtx_unk(node),
                                mtx.m);
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
    if (entry->mAnime != 0 && GetModel_unk(entry->mModel) != 0) {
        if (entry->m04 == 1) {
            ((atn::D3AnimeInstance*)entry->mAnime)->mScale = GetScale_unk(GetModel_unk(entry->mModel)->mMtx);
        } else {
            entry->mAnime->mScale = GetScale_unk(GetModel_unk(entry->mModel)->mMtx);
        }
    }
}

void NewCampPartsBase::vf58(NewCampPartsEntry_unk* entry, const char* node) {
    if (entry->mModel != 0 && entry->mAnime != 0 && GetModelData_unk(entry->mModel) != 0) {
        atn::Model* model = (atn::Model*)GetModelData_unk(entry->mModel);
        if (model->GetNodeMtx_unk(node) != 0) {
            atn::Matrix3x4 mtx = GetNodeWorld_unk(entry, node);
            mtx.m[0][3] = 0.0f;
            mtx.m[1][3] = 0.0f;
            mtx.m[2][3] = 0.0f;
            atn::Vector3 scale = GetScale_unk(mtx);
            Mtx s;
            PSMTXScale(s, 1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
            atn::Matrix3x4 rot;
            PSMTXConcat(mtx.m, s, rot.m);
            if (entry->m04 == 1) {
                ((atn::D3AnimeInstance*)entry->mAnime)->mRot = rot;
            } else {
                entry->mAnime->mRot = rot;
            }
        }
    }
}

void NewCampPartsBase::vf60(NewCampPartsEntry_unk* entry, NewCampPartsMotion_unk* motion, int flags, float step) {
    if (entry != 0 && GetModel_unk(entry->mModel) != 0) {
        int play = 1;
        if (motion == 0 || motion->mHash == 0) {
            play = 0;
        }
        if (play != 0) {
            if (motion->mHash != 0) {
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
                if ((flags & 2) == 2 && motion->mPose.m10.w != 0.0f) {
                    PSMTXQuat(mtx.m, &motion->mPose.m10);
                }
                if ((flags & 4) == 4) {
                    Mtx s;
                    PSMTXScale(s, motion->mPose.mScale.x, motion->mPose.mScale.y, motion->mPose.mScale.z);
                    PSMTXConcat(mtx.m, s, mtx.m);
                }
                if ((flags & 1) == 1) {
                    mtx.m[0][3] = motion->mPose.mPos.x;
                    mtx.m[1][3] = motion->mPose.mPos.y;
                    mtx.m[2][3] = motion->mPose.mPos.z;
                }
                PSMTXConcat(entry->mMtx.m, mtx.m, mtx.m);
                GetModel_unk(entry->mModel)->fn_8009EE30(mtx);
                if (entry->mAnime != 0) {
                    if (entry->m04 == 1) {
                        ((atn::D3AnimeInstance*)entry->mAnime)->mScale = motion->mPose.mScale;
                    } else {
                        entry->mAnime->mScale = motion->mPose.mScale;
                    }
                }
            }
        } else {
            GetModel_unk(entry->mModel)->fn_8009EE30(entry->mMtx);
        }
    }
}

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

void NewCampPartsBase::fn_80511258(NewCampPartsEntry_unk* entry, atn::D3Anime* anime, const atn::Vector3& pos,
                                   const atn::Vector3& scale, const char* node) {
    fn_80512474(entry, anime, atn::Vector3(pos), atn::Vector3(scale), node);
}

void NewCampPartsBase::fn_805112A8(NewCampPartsEntry_unk* entry, atn::D3AnimeInstance* anime,
                                   const atn::Vector3& pos, const atn::Vector3& scale, const char* node) {
    fn_80512028(entry, anime, atn::Vector3(pos), atn::Vector3(scale), node);
}

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

void NewCampPartsBase::fn_805118B4(atn::Task* task, const char* name, NewCampPartsEntry_unk* entry,
                                   const NewCampPartsPoint_unk& pos, int b) {
    atn::Matrix3x4 mtx = GetNodeWorld_unk(entry, "reference");
    MenuCommonTask::sInstance->fn_804A4EEC(task, name, mtx, b, pos.x, pos.y, pos.m08, pos.m0C, pos.m10);
}

void NewCampPartsBase::fn_805119C8(atn::Task* task, const char* name, NewCampPartsEntry_unk* entry,
                                   const NewCampPartsPoint_unk& pos, int b) {
    atn::Matrix3x4 mtx = GetNodeWorld_unk(entry, "reference");
    MenuCommonTask::sInstance->fn_804A5160(task, name, mtx, b, pos.x, pos.y, pos.m08, pos.m0C, pos.m10);
}

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

void NewCampPartsBase::fn_80512028(NewCampPartsEntry_unk* entry, atn::D3AnimeInstance* anime,
                                   const atn::Vector3& pos, const atn::Vector3& scale, const char* node) {
    if (anime != 0 && GetModel_unk(entry->mModel) != 0 && GetModelData_unk(entry->mModel) != 0) {
        atn::Model* model = (atn::Model*)GetModelData_unk(entry->mModel);
        if (model->GetNodeMtx_unk(node) != 0) {
            if (entry->mModel == 0) {
                return;
            }
            anime->fn_800D246C(0);
            anime->mFlags &= ~4;
            atn::Matrix3x4 mtx = GetNodeWorld_unk(entry, node);
            mtx.m[0][3] = 0.0f;
            mtx.m[1][3] = 0.0f;
            mtx.m[2][3] = 0.0f;
            atn::Vector3 s = GetScale_unk(mtx);
            Mtx inv;
            PSMTXScale(inv, 1.0f / s.x, 1.0f / s.y, 1.0f / s.z);
            PSMTXConcat(mtx.m, inv, anime->mRot.m);
            atn::Matrix3x4 world = GetNodeWorld_unk(entry, node);
            anime->mPos = world.GetTrans_unk() + pos;
            atn::Vector3 ms = GetScale_unk(GetModel_unk(entry->mModel)->mMtx);
            anime->mScale = atn::Vector3(ms.x * scale.x, ms.y * scale.y, ms.z * scale.z);
        }
    }
}

void NewCampPartsBase::fn_80512474(NewCampPartsEntry_unk* entry, atn::D3Anime* anime, const atn::Vector3& pos,
                                   const atn::Vector3& scale, const char* node) {
    if (anime != 0 && GetModel_unk(entry->mModel) != 0 && GetModelData_unk(entry->mModel) != 0) {
        atn::Model* model = (atn::Model*)GetModelData_unk(entry->mModel);
        if (model->GetNodeMtx_unk(node) != 0) {
            if (entry->mModel == 0) {
                return;
            }
            anime->fn_800D246C(0);
            anime->mFlags &= ~4;
            atn::Matrix3x4 mtx = GetNodeWorld_unk(entry, node);
            mtx.m[0][3] = 0.0f;
            mtx.m[1][3] = 0.0f;
            mtx.m[2][3] = 0.0f;
            atn::Vector3 s = GetScale_unk(mtx);
            Mtx inv;
            PSMTXScale(inv, 1.0f / s.x, 1.0f / s.y, 1.0f / s.z);
            PSMTXConcat(mtx.m, inv, anime->mRot.m);
            atn::Matrix3x4 world = GetNodeWorld_unk(entry, node);
            anime->mPos = world.GetTrans_unk() + pos;
            atn::Vector3 ms = GetScale_unk(GetModel_unk(entry->mModel)->mMtx);
            anime->mScale = atn::Vector3(ms.x * scale.x, ms.y * scale.y, ms.z * scale.z);
        }
    }
}
