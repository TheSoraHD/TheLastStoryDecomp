#include "game/FriendArganan.h"
#include "math/Vec.h"

// inferred: only the fields cleared by FriendArganan are known
struct HitInfo_unk {
    unsigned char m00[0x10];
    atn::Vector3 m10;
    unsigned char m1C[0x68 - 0x1C];
    int m68;
    unsigned char m6C[0x90 - 0x6C];
    int m90;
};

FriendArganan::FriendArganan(atn::Task* parent, int type, void* data) : PlayerTask(parent, type, data) {}

FriendArganan::~FriendArganan() {}

// inferred: Arganan takes no damage nor knockback
void FriendArganan::vf3C(HitInfo_unk* hit) {
    hit->m10.x *= 0.0f;
    hit->m10.y *= 0.0f;
    hit->m10.z *= 0.0f;
    hit->m90 = 0;
    hit->m68 = 0;
}
