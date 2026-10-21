#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#include "game/HitInfo_unk.h"
#include "math/Vec.h"

class CharaTask;

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

// inferred: from RTTI "Bullet" (vtable at 0x80779F10, game/Bullet, not decompiled); layout from
// HomingBullet. Names invented.
class Bullet {
public:
    Bullet();                                   // 0x8010ED4C
    virtual ~Bullet();                          // 0x8010EE38
    virtual void vf0C(int a, int b);            // 0x8010F30C: inferred: removes the bullet
    virtual void vf10();                        // 0x8010F720: inferred: update
    virtual void vf14(const Mtx m);             // 0x8010FF74
    virtual void vf18();                        // 0x80101330
    virtual void vf1C(CharaTask* chara, const atn::Vector3* pos, void* arg); // 0x80112218: inferred: hit
    virtual void vf20(CharaTask* chara, const atn::Vector3* pos, const atn::Vector3* dir); // 0x80110768
    virtual CharaTask* vf24();                  // 0x80110760

    bool HasFlag_unk(unsigned int flag) const { return (m04 & flag) == flag; }

    unsigned int m04;       // inferred: flags
    atn::Vector3 m08;       // inferred: position
    atn::Vector3 m14;       // inferred: previous position
    atn::Vector3 m20;       // inferred: velocity
    Mtx m2C;
    unsigned char m5C[0x68 - 0x5C];
    HitInfo_unk* m68;       // inferred: bullet data (m44 range, m4C speed bonus, m02 kind)
    unsigned char m6C[0x78 - 0x6C];
    float m78;              // inferred: distance travelled
    unsigned char m7C[0x80 - 0x7C];
    float m80;              // inferred: speed
    int m84;                // inferred: state
    CharaTask* m88;         // inferred: owner
    int m8C;
    int m90;
    float m94;              // inferred: delay before the bullet disappears
    unsigned char m98[0xA0 - 0x98];
};

// inferred: non-virtual members of Bullet (called out of line)
extern "C" void fn_8010F018(Bullet* self, BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir,
                            int count, int arg, int arg2); // inferred: starts the bullet
extern "C" void fn_8010F30C(Bullet* self, int a, int b);  // Bullet::vf0C
extern "C" void fn_8010F4BC(Bullet* self);                // inferred: updates the collision
extern "C" void fn_801100A0(Bullet* self);                // inferred: hit effect
extern "C" void fn_801120B0(Bullet* self);
extern "C" void fn_80112174(Bullet* self);

#endif
