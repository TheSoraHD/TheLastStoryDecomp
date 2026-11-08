#ifndef ATN_PBONE_H
#define ATN_PBONE_H

// Physics bones of the models (atn::PBoneBasic, atn::PBoneOutForce; code in atn/PBoneBasic.cpp),
// created by atn::Model::fn_800973C0 from the "BONE" lines of the model CSV.
#include "atn/Csv.h"
#include "math/Vec.h"
#include "std/string.h"

// inferred: view of the model used by the bones (defined in atn/PBoneBasic.cpp)
struct PBoneModel_unk;

namespace atn {

// inferred: interface of the physics bones
class PBoneInterface {
public:
    virtual ~PBoneInterface() {}
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14() = 0;
    virtual void vf18() = 0;
    virtual int vf1C(PBoneModel_unk* model, CsvBase<char, 9, 13, 34>* csv) = 0;
    virtual void vf20(int enable) = 0;
    virtual int vf24() = 0;
    virtual void vf28(Vector3& force) = 0;
    virtual void vf2C(void* menu) = 0;
    virtual const char* vf30() = 0;
    virtual void vf34() = 0;
};

class PBoneBasic : public PBoneInterface {
public:
    PBoneBasic();
    virtual ~PBoneBasic();
    virtual void vf0C(); // inferred: reset
    virtual void vf10(); // inferred: update
    virtual void vf14(); // inferred: rest pose
    virtual void vf18(); // inferred: debug draw
    virtual int vf1C(PBoneModel_unk* model, CsvBase<char, 9, 13, 34>* csv); // inferred: load
    virtual void vf20(int enable) { m04 = enable; }
    virtual int vf24() { return m04; }
    virtual void vf28(Vector3& force);
    virtual void vf2C(void* menu); // inferred: debug menu
    virtual const char* vf30() { return mName.data(); }
    virtual void vf34(); // inferred: physics step

    int m04;          // inferred: enabled
    Vector3 m08;      // inferred: tip position
    Vector3 m14;      // inferred: direction
    Vector3 m20;      // inferred: velocity
    Vector3 m2C;      // inferred: previous root position / external force
    int m38;          // inferred: root node
    int m3C;          // inferred: tip node
    int m40;
    int m44;
    float m48;        // inferred: Y limit min
    float m4C;        // inferred: Z limit min
    float m50;
    float m54;        // inferred: Y limit max
    float m58;        // inferred: Z limit max
    float m5C;        // inferred: length
    float m60;        // inferred: wait
    PBoneModel_unk* m64;
    int m68;          // inferred: debug display
    std::string mName; // 0x6C
};

// inferred: bone pushed by an external force
class PBoneOutForce : public PBoneBasic {
public:
    PBoneOutForce();
    virtual ~PBoneOutForce();
    // inferred: non-const reference (a copy from a const vector would store x and y first)
    virtual void vf28(Vector3& force) {
        if (m04 != 0) {
            m2C = force;
        }
    }
    virtual void vf34();
};

} // namespace atn

#endif
