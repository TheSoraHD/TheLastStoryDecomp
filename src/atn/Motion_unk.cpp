// atn/Motion_unk.cpp (file name invented): cache of the map models loaded from a csv list
// ("FOLDER", "MODEL", "MODEL_LOD"), the out-of-line members of atn::LH_Motion and the motion
// helpers of the models (pose of a node, MotionPose_unk, atn::MotionCommandData), then the motion banks
// kept in static maps (0x800A0A68-0x800A2664, not written yet) and the random generator of
// MP::CryptMP (0x800A2664, not written yet). The __sinit is the one of <iostream>.
#include "atn/Csv.h"
#include "atn/EffectModel_unk.h"
#include "atn/LH_Binary.h"
#include "atn/LH_Motion.h"
#include "atn/Light_unk.h"
#include "atn/Memory.h"
#include "game/MotionCmdData_unk.h"
#include "gmk/MapLift.h"
#include "std/ios_init.h"
#include "std/string.h"
#include "std/string_members.h"
#include "std/vector.h"

extern "C" {
unsigned long strlen(const char* s);
int strcmp(const char* a, const char* b);
int sprintf(char* buf, const char* fmt, ...);
}
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" double fn_800DC288(const char* s);        // inferred: atof

// inferred: file name and folder of a path
extern "C" std::basic_string<char> fn_8006B174(const std::basic_string<char>& path);
std::basic_string<char> fn_8006B0C8(const std::basic_string<char>& path);

extern "C" void fn_8072D210(float angle, float* s, float* c); // inferred: sine and cosine (256 units a turn)

extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* node);
extern "C" void fn_80092F1C(atn::Model* model, const char* path, const char* node, float distance);
extern "C" void fn_80095CD0(atn::Model* model); // inferred: update

// inferred: map model loaded from a csv list (0x22C bytes; created by fn_8009F788). Names invented.
struct MapModel_unk {
    MapModel_unk(const char* path) : m08(0), m0C(0), mModel(9), m228(0) {
        std::basic_string<char> name = fn_8006B174(std::basic_string<char>(path));
        mFile.vf0C(name.c_str());
        m224 = fn_800DC6B4(name.c_str());
        mModel.GetModel_unk()->m98 = 0.00001f * (unsigned short)m224;
    }

    atn::LH_Binary mFile;   // 0x00: inferred: the csv list
    int m08;                // inferred: the csv was read
    int m0C;                // inferred: the model is loaded
    EffectModel_unk mModel; // 0x10
    unsigned int m224;      // inferred: hash of the file name
    int m228;               // inferred: reference count
};

extern "C" void fn_800A005C(MapModel_unk* model);
extern "C" int fn_8009FFCC(MapModel_unk* model);

// inferred: all the map models
static std::vector<MapModel_unk*>& GetMapModels_unk() {
    static std::vector<MapModel_unk*> sModels;
    return sModels;
}

// NONMATCHING: 99.82% - iterator register of the second loop, pool name of the vector allocation error
extern "C" MapModel_unk* fn_8009F788(const char* path) {
    std::basic_string<char> name = fn_8006B174(std::basic_string<char>(path));
    unsigned int hash = fn_800DC6B4(name.c_str());
    MapModel_unk** it;
    for (it = GetMapModels_unk().begin(); it != GetMapModels_unk().end(); it++) {
        if (*it != 0 && hash == (*it)->m224) {
            return *it;
        }
    }
    MapModel_unk* model = new (6, "", "", 0) MapModel_unk(path);
    int found = 0;
    for (it = GetMapModels_unk().begin(); it != GetMapModels_unk().end(); it++) {
        if (*it == 0) {
            *it = model;
            found = 1;
            break;
        }
    }
    if (!found) {
        GetMapModels_unk().push_back(model);
    }
    return model;
}

// NONMATCHING: 98.95% - the model is reloaded after the delete test in the original
extern "C" void fn_8009FE24() {
    for (MapModel_unk** it = GetMapModels_unk().begin(); it != GetMapModels_unk().end(); it++) {
        MapModel_unk* m = *it;
        if (m != 0 && m->m228 == 0) {
            delete m;
            *it = 0;
        } else if (m != 0) {
            if (fn_8009FFCC(*it) == 0 && ((*(unsigned int*)((char*)*it + 0x21C)) & 1)) {
                fn_80095CD0((*it)->mModel.GetModel_unk());
            }
        }
    }
}

extern "C" int fn_8009FFCC(MapModel_unk* model) {
    if (model->m08 == 0) {
        if (model->mFile.fn_80473F50()) {
            return 1;
        }
        fn_800A005C(model);
        model->mFile.fn_80473F88();
        model->m08 = 1;
    }
    if (model->m0C == 0) {
        if (model->mModel.GetModel_unk()->fn_8008B140()) {
            return 1;
        }
        model->m0C = 1;
    }
    return 0;
}

// NONMATCHING: 84.11% - evaluation order of the sprintf arguments and register allocation
extern "C" void fn_800A005C(MapModel_unk* model) {
    atn::CsvTab csv((const char*)model->mFile.fn_80470580(), model->mFile.fn_8047059C());
    csv.fn_8005BCE8(atn::lbl_8087D710);
    std::basic_string<char> folder = fn_8006B0C8(std::basic_string<char>(model->mFile.fn_80473F18()));
    float scale = 0.6f;
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "FOLDER") == 0) {
            folder = csv.fn_8005B9CC();
        } else if (strcmp(key, "MODEL") == 0) {
            char path[0x100];
            sprintf(path, "%s/%s", folder.c_str(), csv.fn_8005B9CC());
            const char* node = csv.fn_8005B9CC();
            if (strcmp(node, "") == 0) {
                fn_8008AD4C(model->mModel.GetModel_unk(), path, 0);
            } else {
                char nodePath[0x100];
                sprintf(nodePath, "%s/%s", folder.c_str(), node);
                fn_8008AD4C(model->mModel.GetModel_unk(), path, nodePath);
            }
        } else if (strcmp(key, "MODEL_LOD") == 0) {
            char path[0x100];
            sprintf(path, "%s/%s", folder.c_str(), csv.fn_8005B9CC());
            char nodePath[0x100];
            sprintf(nodePath, "%s/%s", folder.c_str(), csv.fn_8005B9CC());
            fn_80092F1C(model->mModel.GetModel_unk(), path, nodePath, scale * fn_800DC288(csv.fn_8005B9CC()));
        }
    } while (csv.fn_8005BBF8() != 0);
}

namespace atn {

LH_Motion::LH_Motion() {
    m08 = 0;
}

void LH_Motion::fn_800A03E4(const char* path) {
    if (m08 != 0) {
        fn_80473F88();
        m08 = 0;
    }
    vf0C(path);
}

int LH_Motion::fn_800A0448() {
    if (fn_80473F50()) {
        return 1;
    }
    if (m08 == 0) {
        m08 = fn_80476884();
    }
    return 0;
}

} // namespace atn

// inferred: animated node of a motion (tracks of scale, rotation and translation)
struct MotionNode_unk {
    unsigned char m00[0x14];
    unsigned int m14; // inferred: name hash
    unsigned int m18; // inferred: 1 translation, 2 rotation, 4 scale
    SceneTrack_unk* m1C;
};
// inferred: loaded motion
struct MotionData_unk {
    unsigned char m00[0x10];
    float m10; // inferred: length
    int m14;   // inferred: number of nodes
    MotionNode_unk** m18;
};

// NONMATCHING: 90.45% - order of the track/key increments and scheduling of the stores
extern "C" void fn_800A0610(atn::LH_Motion* motion, MotionPose_unk* pose, MotionNode_unk* node, short* keys,
                            float frame);

extern "C" int fn_800A04A0(atn::LH_Motion* motion, MotionPose_unk* pose, const char* name, short* keys, float frame) {
    unsigned int hash = fn_800DC6B4(name);
    MotionData_unk* data = (MotionData_unk*)motion->m08;
    for (int i = 0; i < data->m14; i++) {
        MotionNode_unk* node = data->m18[i];
        if (node->m14 == hash) {
            fn_800A0610(motion, pose, node, keys, frame);
            return 1;
        }
    }
    return 0;
}

extern "C" int fn_800A0548(atn::LH_Motion* motion, MotionPose_unk* pose, unsigned int hash, short* keys, float frame) {
    MotionData_unk* data = (MotionData_unk*)motion->m08;
    for (int i = 0; i < data->m14; i++) {
        MotionNode_unk* node = data->m18[i];
        if (node->m14 == hash) {
            fn_800A0610(motion, pose, node, keys, frame);
            return 1;
        }
    }
    return 0;
}

// inferred: the original calls it out of line from fn_800A09D0; the empty release asserts keep it
// over the autoinline threshold
// NONMATCHING: 97.25% - volatile register allocation (data/count/offset)
extern "C" int fn_800A05AC(atn::LH_Motion* motion, unsigned int hash, int start) {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    if (motion->m08 != 0) {
        for (int i = start; i < ((MotionData_unk*)motion->m08)->m14; i++) {
            MotionNode_unk* node = ((MotionData_unk*)motion->m08)->m18[i];
            if (node->m14 == hash) {
                return i;
            }
        }
    }
    return -1;
}

extern "C" void fn_800A05FC(atn::LH_Motion* motion, MotionPose_unk* pose, int index, short* keys, float frame) {
    fn_800A0610(motion, pose, ((MotionData_unk*)motion->m08)->m18[index], keys, frame);
}

// inferred: radians to the units of fn_8072D210 (as in game/CharaTask.cpp)
static inline float ToAngleUnits_unk(float rad) {
    return 40.743664f * rad;
}

// inferred: quaternion from euler angles (inline copy of atn::Quaternion_unk::fn_801498F0)
static inline atn::Quaternion_unk EulerToQuat_unk(const atn::Vector3& rot) {
    atn::Quaternion_unk q;
    float rx = rot.x;
    float ry = rot.y;
    float rz = rot.z;
    float cx, cy, cz;
    float sx, sy, sz;
    fn_8072D210(ToAngleUnits_unk(rx / 2.0f), &sx, &cx);
    fn_8072D210(ToAngleUnits_unk(ry / 2.0f), &sy, &cy);
    fn_8072D210(ToAngleUnits_unk(rz / 2.0f), &sz, &cz);
    float cycz = cy * cz;
    float sysz = sy * sz;
    q.w = cx * cycz + sx * sysz;
    q.x = sx * cycz - cx * sysz;
    q.y = cz * (cx * sy) + sz * (sx * cy);
    q.z = sz * (cx * cy) - cz * (sx * sy);
    return q;
}

extern "C" void fn_800A0610(atn::LH_Motion* motion, MotionPose_unk* pose, MotionNode_unk* node, short* keys,
                            float frame) {
    int track = 0;
    int key = 0;
    if (node->m18 & 4) {
        pose->mScale.x = node->m1C[track++].fn_8047202C(keys[key++], frame);
        pose->mScale.y = node->m1C[track++].fn_8047202C(keys[key++], frame);
        pose->mScale.z = node->m1C[track++].fn_8047202C(keys[key++], frame);
    }
    if (node->m18 & 2) {
        atn::Vector3 rot;
        rot.x = node->m1C[track++].fn_8047202C(keys[key++], frame);
        rot.y = node->m1C[track++].fn_8047202C(keys[key++], frame);
        rot.z = node->m1C[track++].fn_8047202C(keys[key++], frame);
        pose->m10 = EulerToQuat_unk(rot);
    }
    if (node->m18 & 1) {
        pose->mPos.x = node->m1C[track++].fn_8047202C(keys[key++], frame);
        pose->mPos.y = node->m1C[track++].fn_8047202C(keys[key++], frame);
        pose->mPos.z = node->m1C[track++].fn_8047202C(keys[key++], frame);
    }
    pose->m00 |= node->m18;
}

extern "C" float fn_800A08D4(atn::LH_Motion* motion) {
    return ((MotionData_unk*)motion->m08)->m10;
}

MotionPose_unk::MotionPose_unk() {
    m00 = 0;
    mPos.x = 0.0f;
    mPos.y = 0.0f;
    mPos.z = 0.0f;
    m10.x = 0.0f;
    m10.y = 0.0f;
    m10.z = 0.0f;
    m10.w = 0.0f;
    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
}

// inferred: nonzero while one of the effects or efp files is loading
// NONMATCHING: 99.02% - order of the two loop increments (as atn/LightUtil_unk)
int atn::MotionCommandData::fn_800A091C() {
    for (unsigned int i = 0; i < mEffects.mSize; i++) {
        if (mEffects.mData[i].IsLoading()) {
            return 1;
        }
    }
    for (unsigned int i = 0; i < mEfps.mSize; i++) {
        if (mEfps.mData[i].IsLoading()) {
            return 1;
        }
    }
    return 0;
}

// inferred: model resource seen by fn_800A09D0 (node table at 0x44/0x4C)
struct MotionModelRes_unk {
    unsigned char m00[0x44];
    int m44;
    unsigned char m48[0x4C - 0x48];
    struct Node_unk {
        int m00; // inferred: index
        unsigned int m04; // inferred: name hash
    }* m4C;
};

// inferred: index of each node of a model in a motion (-1: not animated)
extern "C" void fn_800A09D0(short* out, atn::Model* model, atn::LH_Motion* motion) {
    MotionModelRes_unk* res = (MotionModelRes_unk*)model->m16C;
    MotionModelRes_unk::Node_unk* node;
    int n = res->m44;
    node = res->m4C;
    int start = 0;
    for (int i = 0; i < n; node++, i++) {
        int index = node->m00;
        if (motion->m08 != 0) {
            int found = fn_800A05AC(motion, node->m04, start);
            out[index] = found;
            if (found != -1) {
                start = found;
            }
        } else {
            out[index] = -1;
        }
    }
}
