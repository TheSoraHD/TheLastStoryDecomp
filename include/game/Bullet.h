#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#include "game/HitInfo_unk.h"
#include "math/Vec.h"

class CharaTask;
struct CollResult_unk;

// inferred: shot parameters (0x1C bytes, copied into the replay event by HomingBullet::vf28)
struct BulletShot_unk {
    float m00;
    int m04;
    int m08;
    HitInfo_unk* m0C; // inferred: skill
    int m10;
    float m14;
    float m18;
};

// inferred: event recorded by the replay (0x44 bytes; kind 0: shot, 1: removed, 2: homing shot,
// 3: homing removed). Names invented.
struct ReplayShot_unk {
    ReplayShot_unk() : m00(0.1f), m04(0), m08(0), m10(0), m14(0.0f), m18(1.0f) {}
    float m00;
    int m04;
    int m08;
    HitInfo_unk* m0C;
    int m10;
    float m14;
    float m18;
};
struct ReplayEvent_unk {
    int mKind;
    ReplayShot_unk mShot;
    atn::Vector3 mPos;
    atn::Vector3 mDir;
    CharaTask* mOwner;
    int m3C;
    class Bullet* mBullet;
};
extern "C" void fn_805706C0(void* recorder, ReplayEvent_unk* ev); // inferred: records an event

// inferred: field object; m5590 is the replay recorder
struct HomingField_unk {
    unsigned char m0000[0x5590];
    void* m5590;
};

// inferred: data of listener event 12 (a bullet was shot); names invented
struct ShotEvent_unk {
    CharaTask* mOwner;
    int mId;
    atn::Vector3 mPos;
    atn::Vector3 mDir;
    int mArg;
    void* mParam; // inferred: HomingParam_unk of a homing bullet, 0 otherwise
};

// inferred: from RTTI "Bullet" (vtable at 0x80779F10, game/Bullet). Names invented.
class Bullet {
public:
    Bullet();                                   // 0x8010ED4C
    virtual ~Bullet();                          // 0x8010EE38
    virtual void vf0C(int a, int b);            // 0x8010F30C: inferred: removes the bullet
    virtual void vf10();                        // 0x8010F720: inferred: update
    virtual void vf14(const Mtx m);             // 0x8010FF74
    virtual void vf18();                        // 0x80101330
    virtual int vf1C(CharaTask* chara, const atn::Vector3* pos, void* arg); // 0x80112218: inferred: hit
    virtual void vf20(CharaTask* chara, const atn::Vector3* pos, atn::Vector3* dir); // 0x80110768: inferred: reflected
    virtual CharaTask* vf24();                  // 0x80110760

    // inferred: fires the bullet for `owner` (listener event 12)
    void fn_8010EE78(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* owner,
                     int count, unsigned int flags, float speed);
    void fn_8010EFC8(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, int arg, int count,
                     unsigned int flags, float speed);
    // inferred: fires the bullet of an object of type `type` (gmk::BulletPoint)
    void fn_8010EFF0(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, int type, int count,
                     unsigned int flags, float speed);
    // inferred: starts the bullet (`record`: add a replay event)
    void fn_8010F018(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, int count,
                     unsigned int flags, int record, float speed);
    void fn_8010F26C(BulletShot_unk* shot); // inferred: starts the effect of the shot
    void fn_8010F41C();                     // inferred: stops the effects
    void fn_8010F4BC();                     // inferred: updates the matrix of the effect
    void fn_801100A0();                     // inferred: hit effect
    void fn_80110858(CollResult_unk* res); // inferred: the bullet hits `res`
    void fn_801120B0();
    void fn_80112174();

    bool HasFlag_unk(unsigned int flag) const { return (m04 & flag) == flag; }

    unsigned int m04;       // inferred: flags
    atn::Vector3 m08;       // inferred: position
    atn::Vector3 m14;       // inferred: previous position
    atn::Vector3 m20;       // inferred: velocity
    atn::Matrix3x4 m2C;
    float m5C;              // 0x5C..0x74: BulletShot_unk
    int m60;
    int m64;
    HitInfo_unk* m68;       // inferred: bullet data (m44 range, m4C speed bonus, m02 kind)
    int m6C;
    float m70;
    float m74;
    float m78;              // inferred: distance travelled
    int m7C;
    float m80;              // inferred: speed
    int m84;                // inferred: state
    CharaTask* m88;         // inferred: owner
    int m8C;
    int m90;
    float m94;              // inferred: delay before the bullet disappears
    int m98;
    CharaTask* m9C;         // inferred: character the bullet follows (node "spine")
};

#endif
