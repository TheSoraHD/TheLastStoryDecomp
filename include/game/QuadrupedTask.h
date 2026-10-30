#ifndef GAME_QUADRUPEDTASK_H
#define GAME_QUADRUPEDTASK_H

#include "game/EnemyTask.h"
#include "game/ResHandle.h"

namespace atn {
struct MotionBuffer;
struct NodeChunk;
} // namespace atn
struct RouteTable_unk;

// inferred from __vt__13QuadrupedTask (0x807956F0): four-legged enemy (the white leopard and the war lion
// carry their own copies of most of this code). The enemy factory (fn_805A5374) allocates 0x1568 bytes.
// Members keep their offsets (names unknown).
class QuadrupedTask : public EnemyTask {
public:
    QuadrupedTask(atn::Task* parent, void* data); // 0x80566420
    virtual ~QuadrupedTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual void vf24();
    virtual void vf34(void* target, int arg, float a, float b);
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf74(atn::Vector3* pos);
    virtual void vf8C(atn::Vector3* pos);

    void fn_80566FB4(); // inferred: state machine (m58C)
    void fn_80567424(CharaTask* target);
    void fn_805675FC();
    void fn_80567968(const atn::Vector3& target, int arg, float speed, float rate); // inferred: as Em403WarLion::fn_80324A38
    void fn_805686F0(atn::MotionBuffer* nodes, int count); // inferred: node pose callback
    void fn_805688CC(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk); // inferred: node matrix callback
    // inferred: moves of the AI modes 0-4 (speed and rotation to move with)
    void fn_80568EE0(float* speed, atn::Vector3* rot);
    void fn_80568F10(float* speed, atn::Vector3* rot);
    void fn_805693CC(float* speed, atn::Vector3* rot);
    void fn_80569660(float* speed, atn::Vector3* rot);
    void fn_8056A03C(float* speed, atn::Vector3* rot);

    atn::Vector3 m14B0; // inferred: y is the lean of the waist
    unsigned char m14BC[0x14CC - 0x14BC];
    float m14CC; // inferred: height of the left hand over the root
    unsigned char m14D0[0x14F8 - 0x14D0];
    float m14F8;
    float m14FC;
    float m1500;
    float m1504;
    float m1508;
    int m150C;
    int m1510;
    int m1514;
    int m1518;
    float m151C;
    float m1520;
    float m1524;
    float m1528;
    float m152C;
    float m1530;
    unsigned char m1534[0x1560 - 0x1534];
    ResHandle m1560; // inferred: debug menu group
    int m1564;
};

#endif
