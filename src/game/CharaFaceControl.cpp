#include "game/CharaFaceControl.h"
#include "game/CharaTask.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/Model_unk.h"
#include "std/vector.h"
#include <boost/bind.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s);        // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" int fn_802180A8(const char* name);       // inferred: motion id from its name
extern "C" void fn_8072D210(float angle, float* s, float* c); // inferred: sine and cosine (256 units a turn)
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
// inferred: reads a node edit from a "NODE_EDIT" line (as in game/EquipItemData.cpp)
extern "C" void fn_80089EE4(Event::PoseKey_unk* edit, atn::CsvTab* csv);
// inferred: node pose helpers of the engine (names unknown)
extern "C" void fn_80099010(void* pose, atn::Model::NodeRes_unk* node);   // inferred: resets a pose
extern "C" void fn_80098948(void* dst, int arg, void* a, void* b, float t); // inferred: blends two poses
// inferred: pose of the node `hash` of a motion at `frame`; zero on success
extern "C" int fn_800A0548(atn::LoadHandle* motion, void* pose, unsigned int hash, void* work, float frame);
// inferred: frame parameters (as gmk::ArenaFloor)
struct FaceParams_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
extern "C" FaceParams_unk* lbl_8087EFA8;
// inferred: work buffer of fn_800A0548 (0x12 bytes are cleared before each call)
extern "C" {
unsigned char lbl_807C7AF8[0x18];
}

namespace atn {
// inferred: node pose (as atn::Model::Node_unk)
struct MotionBuffer {
    unsigned int m00;
    Vector3 m04;
    Quaternion_unk m10;
    Vector3 m20;
};
} // namespace atn

// inferred: node pose filled from a motion (0x2C bytes, as in gmk/MapLift.h)
struct MotionPose_unk {
    MotionPose_unk(); // 0x800A08E0
    int m00;
    unsigned char m04[0x2C - 0x04];
};

CharaFaceControl::CharaFaceControl() : m00(0), m5C(0), m60(-1.0f), m78(0), m7C(1.0f) {}

// inferred: the original copies the callback and swaps it into the model (the frame and the swap
// temporaries only match this way: perhaps an inline setter of atn::Model taking the function by
// reference)
void CharaFaceControl::fn_8011D424(atn::Model* model) {
    m00 = model;
    boost::function<void(atn::MotionBuffer*, int)> callback(boost::bind(&CharaFaceControl::fn_8011D844, this, _1, _2));
    atn::Model* target = m00;
    boost::function<void(atn::MotionBuffer*, int)> copy(callback);
    copy.swap(target->m370);
}

// inferred: same as Quaternion_unk::fn_801498F0 (game/CharaTask.cpp)
static inline atn::Quaternion_unk ToQuat_unk(const atn::Vector3& rot) {
    atn::Quaternion_unk q;
    float rx = rot.x;
    float ry = rot.y;
    float rz = rot.z;
    float cx, cy, cz;
    float sx, sy, sz;
    fn_8072D210(40.743664f * (rx / 2.0f), &sx, &cx);
    fn_8072D210(40.743664f * (ry / 2.0f), &sy, &cy);
    fn_8072D210(40.743664f * (rz / 2.0f), &sz, &cz);
    float cycz = cy * cz;
    float sysz = sy * sz;
    q.w = cx * cycz + sx * sysz;
    q.x = sx * cycz - cx * sysz;
    q.y = cz * (cx * sy) + sz * (sx * cy);
    q.z = sz * (cx * cy) - cz * (sx * sy);
    return q;
}

// NONMATCHING: 85.74% - scheduling and stack slots of the Vector3 temporaries of the three edit loops
// (the original keeps fewer FPRs and three more GPRs); the code shape is the same
void CharaFaceControl::fn_8011D844(atn::MotionBuffer* nodes, int arg) {
    fn_8011E200(&m04[!m5C], &m44[!m5C], -1.0f);
    fn_8011E200(&m04[m5C], &m44[m5C], -1.0f);
    if (m04[!m5C].IsActive_unk() && atn::Abs_unk(m04[m5C].m08 - 1.0f) < 1.1920929e-7f) {
        m04[!m5C].m1C = 0;
    }
    if (m60 >= 0.0f) {
        float weight;
        if (m60 <= m64) {
            weight = m70 * (m60 / m64);
        } else {
            weight = m70 * ((m6C - m60) / m68);
        }
        if (m78 && m60 <= m6C - m68 && m60 >= m64) {
            weight = m70;
        }
        if (weight > 0.0001f) {
            fn_8011E200(&m04[m5C], &m44[m5C], weight);
        }
        m60 -= lbl_8087EFA8->mTimeScale;
    }
    if (m44[!m5C].mSize != 0 || m44[m5C].mSize != 0) {
        atn::Model::Res_unk* res = m00->m16C;
        FixedArray_unk<Event::PoseKey_unk, 32> edits;
        edits.mCount = 0;
        for (unsigned int i = 0; i < m44[!m5C].mSize; i++) {
            Event::PoseKey_unk edit = m44[!m5C].mData[i];
            float weight = m04[!m5C].m08;
            edit.m20 = atn::Lerp_unk(atn::Vector3(1.0f, 1.0f, 1.0f), edit.m20, weight);
            edit.m08 *= weight;
            edit.m14 *= weight;
            edits.PushBack_unk(edit);
        }
        for (unsigned int i = 0; i < m44[m5C].mSize; i++) {
            Event::PoseKey_unk edit = m44[m5C].mData[i];
            bool found = false;
            for (unsigned int j = 0; j < edits.mCount; j++) {
                if (edits.mData[j].m00 == m44[m5C].mData[i].m00) {
                    Event::PoseKey_unk& old = edits.mData[j];
                    float weight = m04[m5C].m08;
                    edit.m08 = atn::Lerp_unk(old.m08, edit.m08, weight);
                    edit.m14 = atn::Lerp_unk(old.m14, edit.m14, weight);
                    edit.m20 = atn::Lerp_unk(old.m20, edit.m20, weight);
                    old = edit;
                    found = true;
                    break;
                }
            }
            if (!found) {
                float weight = m04[m5C].m08;
                edit.m20 = atn::Lerp_unk(atn::Vector3(1.0f, 1.0f, 1.0f), edit.m20, weight);
                edit.m08 *= weight;
                edit.m14 *= weight;
                edits.PushBack_unk(edit);
            }
        }
        for (int i = 0; i < res->m44; i++) {
            atn::Model::NodeRes_unk* node = res->m48[i];
            int index = node->m18;
            for (unsigned int j = 0; j < edits.mCount; j++) {
                if (node->m14 == edits.mData[j].m00) {
                    atn::MotionBuffer* pose = &nodes[index];
                    pose->m00 = 0;
                    fn_80099010(pose, node);
                    pose->m04 += edits.mData[j].m08;
                    atn::Quaternion_unk rot = ToQuat_unk(edits.mData[j].m14);
                    atn::Quaternion_unk result;
                    PSQUATMultiply(&pose->m10, &rot, &result);
                    pose->m10 = result;
                    pose->m20.x *= edits.mData[j].m20.x;
                    pose->m20.y *= edits.mData[j].m20.y;
                    pose->m20.z *= edits.mData[j].m20.z;
                    break;
                }
            }
        }
    }
}

#define FACE_MAX(a, b) ((a) > (b) ? (a) : (b))
#define FACE_MIN(a, b) ((a) < (b) ? (a) : (b))

// NONMATCHING: 97.36% - register allocation (the original keeps the motion, the node and the entry
// in r28-r31 and only five name hashes in registers); same instructions otherwise
void CharaFaceControl::fn_8011E200(Chara1188Entry_unk* entry, atn::SimpleVector<Event::PoseKey_unk>* edits,
                                   float weight) {
    if (!entry->IsActive_unk()) {
        return;
    }
    bool fixed = weight >= 0.0f;
    if (!fixed) {
        entry->m08 = FACE_MAX(0.0f, FACE_MIN(entry->m10, entry->m08 + entry->m0C));
    }
    atn::Model::Node_unk* poses = m00->m220;
    MotionPose_unk pose;
    atn::Model::Res_unk* res = m00->m16C;
    unsigned int head = fn_800DC6B4("head");
    unsigned int neck = fn_800DC6B4("neck");
    unsigned int eyeL = fn_800DC6B4("eye_L");
    unsigned int eyeR = fn_800DC6B4("eye_R");
    unsigned int eyelibL = fn_800DC6B4("eyelib_L");
    unsigned int eyelibR = fn_800DC6B4("eyelib_R");
    unsigned int lowerEyelibL = fn_800DC6B4("lowereyelib_L");
    unsigned int lowerEyelibR = fn_800DC6B4("lowereyelib_R");
    unsigned int hairB = fn_800DC6B4("hair_b");
    unsigned int hairB1 = fn_800DC6B4("hair_b1");
    unsigned int hairB2 = fn_800DC6B4("hair_b2");
    unsigned int hairBLong = fn_800DC6B4("hair_b_long");
    unsigned int hairF = fn_800DC6B4("hair_f");
    unsigned int hairF1 = fn_800DC6B4("hair_f1");
    unsigned int hairL = fn_800DC6B4("hair_l");
    unsigned int hairLEnd = fn_800DC6B4("hair_l_end");
    unsigned int hairR = fn_800DC6B4("hair_r");
    unsigned int hairREnd = fn_800DC6B4("hair_r_end");
    for (unsigned int i = 0; i < entry->mKeys.mSize; i++) {
        FaceKey_unk* key = &entry->mKeys.mData[i];
        atn::LoadHandle* motion = m00->fn_80097D40(key->m00 + fixed);
        if (motion == 0) {
            continue;
        }
        float w;
        if (weight > 0.0f) {
            w = weight * key->m04;
        } else {
            w = entry->m08 * key->m04;
        }
        bool full = false;
        if (atn::Abs_unk(w - 1.0f) < 1.1920929e-7f && m7C >= 0.999f) {
            full = true;
        }
        for (int j = 0; j < res->m44; j++) {
            atn::Model::NodeRes_unk* node = res->m48[j];
            int index = node->m18;
            if (weight > 0.0f && !(node->m14 == eyelibL || node->m14 == eyelibR || node->m14 == lowerEyelibL ||
                                   node->m14 == lowerEyelibR)) {
                continue;
            }
            if (!entry->m1D && (node->m14 == eyeL || node->m14 == eyeR)) {
                continue;
            }
            if (!entry->m1E && (node->m14 == neck || node->m14 == head)) {
                continue;
            }
            if (!entry->m1F &&
                (node->m14 == hairB || node->m14 == hairB1 || node->m14 == hairB2 || node->m14 == hairBLong ||
                 node->m14 == hairF || node->m14 == hairF1 || node->m14 == hairL || node->m14 == hairLEnd ||
                 node->m14 == hairR || node->m14 == hairREnd)) {
                continue;
            }
            if (key->m08.mSize != 0) {
                bool found = false;
                for (unsigned int k = 0; k < key->m08.mSize; k++) {
                    if (node->m14 == key->m08.mData[k]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    continue;
                }
            }
            memset(lbl_807C7AF8, 0, 0x12);
            pose.m00 = 0;
            if (full) {
                if (fn_800A0548(motion, &poses[index], node->m14, lbl_807C7AF8, entry->m00) == 0 &&
                    (node->m14 == neck || node->m14 == head)) {
                    fn_80099010(&poses[index], node);
                    fn_80099010(&pose, node);
                    fn_80098948(&poses[index], 1, &poses[index], &pose, w);
                }
            } else {
                float w2 = (node->m14 == neck || node->m14 == head) ? w * m7C : w;
                if (fn_800A0548(motion, &pose, node->m14, lbl_807C7AF8, entry->m00) != 0) {
                    fn_80099010(&poses[index], node);
                    fn_80099010(&pose, node);
                    fn_80098948(&poses[index], 1, &poses[index], &pose, w2);
                } else if (node->m14 == neck || node->m14 == head) {
                    fn_80099010(&poses[index], node);
                    fn_80099010(&pose, node);
                    fn_80098948(&poses[index], 1, &poses[index], &pose, w2);
                }
            }
        }
    }
    if (!fixed) {
        entry->m00 += entry->m04;
    }
    if (entry->m08 <= 0.0f) {
        entry->m1C = 0;
    }
}

void CharaFaceControl::fn_8011E81C(int motion, unsigned char eyes, unsigned char head, unsigned char hair,
                                   float speed, float weight) {
    if (m04[m5C].IsActive_unk()) {
        if (motion == m04[m5C].mKeys.mData[0].m00) {
            return;
        }
        m04[m5C].m0C = 0.0f;
        m5C = !m5C;
    }
    FaceKey_unk key(motion);
    m04[m5C].mKeys.Resize_unk(1, 1);
    m04[m5C].mKeys.mData[0] = key;
    m04[m5C].m00 = 0.0f;
    m04[m5C].m04 = 1.0f;
    m04[m5C].m08 = weight;
    m04[m5C].m0C = 0.1f;
    m04[m5C].m10 = speed;
    m04[m5C].m1C = 1;
    m04[m5C].m1D = eyes;
    m04[m5C].m1E = head;
    m04[m5C].m1F = hair;
    m44[m5C].clear();
}

// NONMATCHING: 99.21% - registers of the inlined Buffer_unk copy (the new size and the element
// pointer swap r21/r23)
void CharaFaceControl::fn_8011EB18(atn::Buffer_unk<FaceKey_unk>* keys, unsigned char eyes, unsigned char head,
                                   float speed, float weight) {
    if (m04[m5C].IsActive_unk()) {
        if (keys->mData[0].m00 == m04[m5C].mKeys.mData[0].m00) {
            return;
        }
        m04[m5C].m0C = 0.0f;
        m5C = !m5C;
    }
    m04[m5C].mKeys = *keys;
    m04[m5C].m00 = 0.0f;
    m04[m5C].m04 = 1.0f;
    m04[m5C].m08 = weight;
    m04[m5C].m0C = 0.1f;
    m04[m5C].m10 = speed;
    m04[m5C].m1C = 1;
    m04[m5C].m1D = eyes;
    m04[m5C].m1E = head;
    m44[m5C].clear();
}

void CharaFaceControl::fn_8011EDE0(int fade) {
    if (fade != 0) {
        if (m04[m5C].IsActive_unk()) {
            m04[m5C].m0C = -0.1f;
        }
    } else {
        m04[!m5C].m1C = 0;
        m04[m5C].m1C = 0;
    }
}

void CharaFaceControl::fn_8011EE58(float weight, float m74_, float total, float fadeOut, float fadeIn) {
    m70 = weight;
    m74 = m74_;
    m64 = fadeIn;
    m68 = fadeOut;
    if ((float)__fabs(total) < 1.1920929e-7f) {
        m6C = m64 + m68;
    } else {
        m6C = total;
    }
    m60 = m6C;
}

void FaceData_unk::fn_8011EEA0(const char* path) {
    mKeys.Free_unk();
    vf0C(path);
}

// NONMATCHING: 99.30% - the inlined std::vector<int>::push_back loads the size before the capacity
// (known difference of include/std/vector.h) and some registers differ
int FaceData_unk::fn_8011EF14() {
    if (mKeys.mSize == 0) {
        if (fn_80473F50() != 0) {
            return 1;
        }
        int size = fn_8047059C();
        atn::CsvTab csv((const char*)fn_80470580(), size);
        csv.fn_8005BCE8(lbl_8087D710);
        int motions = 0;
        int edits = 0;
        do {
            const char* field = csv.fn_8005B9CC();
            if (field[0] != '\0' && field[0] != ';') {
                if (strcmp(field, "MOTION") == 0) {
                    motions++;
                } else if (strcmp(field, "NODE_EDIT") == 0) {
                    edits++;
                }
            }
        } while (csv.fn_8005BBF8() != 0);
        if (motions > 0) {
            mKeys.Resize_unk(motions, 1);
        }
        if (edits > 0) {
            mEdits.Resize_unk(edits);
        }
        int key = 0;
        int edit = 0;
        csv.Reset_unk();
        do {
            const char* field = csv.fn_8005B9CC();
            if (field[0] == '\0' || field[0] == ';') {
                continue;
            }
            if (strcmp(field, "MOTION") == 0) {
                FaceKey_unk* k = &mKeys.mData[key++];
                k->m00 = fn_802180A8(csv.fn_8005B9CC());
                if (k->m00 < 0) {
                    continue;
                }
                k->m04 = fn_800DC288(csv.fn_8005B9CC());
                std::vector<int> nodes;
                for (;;) {
                    if (*csv.fn_8005B9CC() == '\0') {
                        break;
                    }
                    nodes.push_back(fn_800DC6B4(field));
                }
                if (nodes.size() != 0) {
                    k->m08.Resize_unk(nodes.size(), 1);
                    for (unsigned int i = 0; i < k->m08.mSize; i++) {
                        k->m08.mData[i] = nodes[i];
                    }
                }
            } else if (strcmp(field, "NODE_EDIT") == 0) {
                fn_80089EE4(&mEdits.mData[edit++], &csv);
            }
        } while (csv.fn_8005BBF8() != 0);
        fn_80473F88();
    }
    return 0;
}
