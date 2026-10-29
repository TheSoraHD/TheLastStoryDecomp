#ifndef GAME_CHARAFACECONTROL_H
#define GAME_CHARAFACECONTROL_H

#include "atn/Array_unk.h"
#include "atn/LH_Binary.h"
#include "Event/PoseKey_unk.h"

namespace atn {
struct Model;
}
namespace atn {
struct MotionBuffer;
}

// inferred: name from the RTTI of the boost::function callback (bind_t<void, mf2<void,
// CharaFaceControl, atn::MotionBuffer*, int>, ...>, 0x80736EF0); object at CharaTask+0x1188. Two
// face motions (m04) and two node edit lists (m44) are cross-faded; m5C is the current one.
class CharaFaceControl {
public:
    // Name from the RSO (__dt__Q216CharaFaceControl10MotionPartFv, 0x8011D320). inferred: face
    // motion of an entry (0x10 bytes): motion id, weight and the hashes of the nodes it is limited
    // to (all nodes when empty). Member names invented.
    struct MotionPart {
        // inferred: weak copy 0x8011EAF8
        MotionPart() : m00(-1), m04(0.0f) {}
        // inferred: inlined in CharaFaceControl::fn_8011E81C (the buffer is cleared first)
        MotionPart(int motion) {
            m00 = motion;
            m04 = 1.0f;
        }
        int m00;
        float m04;
        atn::Buffer_unk<int> m08;
    };

    // inferred: face motion being played (0x20 bytes). Named MotionSet because the RSO only lists
    // the nested types CharaFaceControl::MotionPart and CharaFaceControl::MotionSet and this one
    // holds the MotionPart array; member names invented.
    struct MotionSet {
        // inferred: weak copies 0x8011D2E0 and 0x8011D394
        MotionSet() : m00(0.0f), m04(1.0f), m08(0.0f), m0C(0.1f), m10(1.0f), m1C(0), m1D(1), m1E(1) {}
        // inferred: a motion is playing (name invented)
        bool IsActive_unk() const { return mKeys.mSize != 0 && m1C; }

        float m00;  // inferred: frame
        float m04;  // inferred: frame step
        float m08;  // inferred: weight
        float m0C;  // inferred: weight step (0.1 in, -0.1 out)
        float m10;  // inferred: maximum weight
        atn::Buffer_unk<MotionPart> mKeys; // 0x14
        unsigned char m1C; // inferred: active
        unsigned char m1D; // inferred: also moves the eyes
        unsigned char m1E; // inferred: also moves the head and the neck
        unsigned char m1F; // inferred: also moves the hair
    };

    CharaFaceControl(); // 0x8011D24C
    void fn_8011D424(atn::Model* model); // inferred: installs the node callback of the model
    void fn_8011D844(atn::MotionBuffer* nodes, int arg); // inferred: node callback
    void fn_8011E200(MotionSet* entry, atn::SimpleVector<Event::PoseKey_unk>* edits, float weight);
    // inferred: plays the face motion `motion` (CharaObj, CharaTask)
    void fn_8011E81C(int motion, unsigned char eyes, unsigned char head, unsigned char hair, float speed,
                     float weight);
    // inferred: plays the face motions of a loaded face file
    void fn_8011EB18(atn::Buffer_unk<MotionPart>* keys, unsigned char eyes, unsigned char head, float speed,
                     float weight);
    void fn_8011EDE0(int fade); // inferred: stops the face motion (fading it out when `fade` is set)
    void fn_8011EE58(float weight, float m74, float total, float fadeOut, float fadeIn); // inferred: blink timer

    atn::Model* m00;
    MotionSet m04[2];
    atn::SimpleVector<Event::PoseKey_unk> m44[2];
    int m5C;   // inferred: current entry (0 or 1)
    float m60; // inferred: timer
    float m64;
    float m68;
    float m6C;
    float m70;
    float m74;
    unsigned char m78;
    float m7C;
};
typedef CharaFaceControl Chara1188_unk;

// inferred: face file loaded by Event::DialogHead ("MOTION" and "NODE_EDIT" lines); name invented
struct FaceData_unk : public atn::LH_Binary {
    void fn_8011EEA0(const char* path); // inferred: loads `path`
    int fn_8011EF14();                  // inferred: parses the file, nonzero while loading

    atn::Buffer_unk<CharaFaceControl::MotionPart> mKeys; // 0x08
    atn::SimpleVector<Event::PoseKey_unk> mEdits;          // 0x10
};

#endif
